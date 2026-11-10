"""Bounded nonblocking Serial framing; command interpretation stays in the model."""
import json

from app.services.interface_registry import SERIAL_INPUTS


def validate_receivers(ports):
    receivers = [p for p in ports.values() if p['binding']['kind'] in SERIAL_INPUTS]
    validity = [p for p in ports.values() if p['binding']['kind'] == 'serial_valid']
    if len(receivers) > 1:
        raise ValueError('Physical Serial supports one receive parser; multiple receivers would consume the same byte stream')
    for p in receivers:
        b, name = p['binding'], p['name']
        if p['type'] != 'i32' or p['unit'] != '1' or p['sample_period_ms'] != p['control_period_ms']:
            raise ValueError(name + ': Serial input requires i32 unit 1, updated every control step')
        if b.get('framing') not in ('line', 'line_or_idle') or type(b.get('idle_timeout_ms')) is not int or not 1 <= b['idle_timeout_ms'] <= 2147483647:
            raise ValueError(name + ': framing must be line/line_or_idle and idle_timeout_ms a positive i32')
        if b['kind'] == 'serial_command_input':
            commands = b.get('commands')
            if (not isinstance(commands, dict) or not 1 <= len(commands) <= 32 or
                    any(not isinstance(token, str) or not token or len(token) > 31 or token != token.strip() or
                        any(ord(c) < 32 or ord(c) > 126 for c in token) or type(code) is not int or
                        not -2147483648 <= code <= 2147483647 for token, code in commands.items())):
                raise ValueError(name + '.commands must map 1..32 exact printable ASCII tokens (1..31 bytes) to i32 codes')
        else:
            low, high = b.get('min'), b.get('max')
            if type(low) is not int or type(high) is not int or not -2147483648 <= low <= high <= 2147483647:
                raise ValueError(name + ': min/max must be inclusive ordered i32 bounds')
            if not low <= p['initial'] <= high:
                raise ValueError(name + ': initial is outside the serial integer bounds')
        matching = [v for v in validity if v['binding'].get('value') == name]
        if (len(matching) != 1 or matching[0]['name'] != name + '_valid' or matching[0]['type'] != 'bool' or
                matching[0]['unit'] != '1' or matching[0]['initial'] is not False or
                matching[0]['sample_period_ms'] != p['control_period_ms']):
            raise ValueError(name + ': requires one matching ' + name + '_valid Boolean input, initially false, updated every step')
    for p in validity:
        value = ports.get(p['binding'].get('value'))
        if value is None or value['binding']['kind'] not in SERIAL_INPUTS:
            raise ValueError(p['name'] + '.binding.value must reference a serial receive input')


def receiver_code(port):
    """One completed frame per step preserves events in bursts; 32 reads bound CPU work."""
    b, name = port['binding'], port['name']
    def integer(value):
        return 'INT32_MIN' if value == -2147483648 else str(value) + 'L'
    if b['kind'] == 'serial_command_input':
        parse = '\n'.join('  if (strcmp(begin, ' + json.dumps(token) + ') == 0) { *value = ' + integer(code) + '; return true; }'
                          for token, code in b['commands'].items()) + '\n  return false;'
    else:
        parse = '''  char* cursor = begin;
  bool negative = *cursor == '-';
  if (*cursor == '-' || *cursor == '+') ++cursor;
  if (!*cursor) return false;
  uint32_t magnitude = 0;
  const uint32_t limit = negative ? 2147483648UL : 2147483647UL;
  while (*cursor) {
    if (*cursor < '0' || *cursor > '9') return false;
    uint32_t digit = (uint32_t)(*cursor++ - '0');
    if (magnitude > (limit - digit) / 10UL) return false;
    magnitude = magnitude * 10UL + digit;
  }
  int32_t parsed = negative ? (magnitude == 2147483648UL ? INT32_MIN : -(int32_t)magnitude) : (int32_t)magnitude;
  if (parsed < MIN_VALUE || parsed > MAX_VALUE) return false;
  *value = parsed;
  return true;'''.replace('MIN_VALUE', integer(b['min'])).replace('MAX_VALUE', integer(b['max']))
    idle = ('if ((hw_rx_size || hw_rx_overflow) && (uint32_t)(hw_now - hw_rx_last_byte) >= ' +
            str(b['idle_timeout_ms']) + 'UL) return hw_rx_finish(value);') if b['framing'] == 'line_or_idle' else ''
    declarations = '''#include <string.h>
static char hw_rx_buffer[64];
static uint8_t hw_rx_size = 0;
static bool hw_rx_overflow = false;
static uint32_t hw_rx_last_byte = 0;
static bool hw_rx_parse(int32_t* value) {
  hw_rx_buffer[hw_rx_size] = '\\0';
  char* begin = hw_rx_buffer;
  while (*begin == ' ' || *begin == '\\t') ++begin;
  char* end = hw_rx_buffer + hw_rx_size;
  while (end > begin && (end[-1] == ' ' || end[-1] == '\\t')) *--end = '\\0';
  if (!*begin) return false;
PARSE
}
static bool hw_rx_finish(int32_t* value) {
  bool accepted = !hw_rx_overflow && hw_rx_parse(value);
  hw_rx_size = 0; hw_rx_overflow = false;
  return accepted;
}
static bool hw_rx_poll(uint32_t hw_now, int32_t* value) {
  for (uint8_t budget = 0; budget < 32 && Serial.available() > 0; ++budget) {
    int byte = Serial.read();
    if (byte < 0) break;
    hw_rx_last_byte = hw_now;
    if (byte == '\\r' || byte == '\\n') {
      if (!hw_rx_size && !hw_rx_overflow) continue;
      return hw_rx_finish(value);
    }
    if (byte == 0 || byte > 127) hw_rx_overflow = true;
    if (hw_rx_size < 63) hw_rx_buffer[hw_rx_size++] = (char)byte;
    else hw_rx_overflow = true;
  }
  if (Serial.available() > 0) return false;
  IDLE
  return false;
}'''.replace('PARSE', parse).replace('IDLE', idle)
    return declarations, 'hw_value_' + name + '_valid = hw_rx_poll(hw_now, &hw_value_' + name + ');'
