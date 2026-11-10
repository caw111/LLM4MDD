"""Bounded nonblocking Serial framing; command interpretation stays in the model."""
import json
import re

from app.services.interface_registry import SERIAL_INPUTS


def validate_receivers(ports):
    receivers = [p for p in ports.values() if p['binding']['kind'] in SERIAL_INPUTS]
    validity = [p for p in ports.values() if p['binding']['kind'] == 'serial_valid']
    if len(receivers) > 1:
        raise ValueError('Physical Serial supports one receive parser; multiple receivers would consume the same byte stream')
    for p in receivers:
        b, name = p['binding'], p['name']
        unit = 's' if b['kind'] == 'serial_datetime_input' else '1'
        if p['type'] != 'i32' or p['unit'] != unit or p['sample_period_ms'] != p['control_period_ms']:
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
        elif b['kind'] == 'serial_packet_input':
            commands=b.get('commands')
            if (not isinstance(commands,dict) or not 1<=len(commands)<=32 or any(
                    not isinstance(token,str) or not re.fullmatch(r'[A-Za-z][A-Za-z0-9_]{0,30}',token)
                    or type(code) is not int or not 1<=code<=214747 for token,code in commands.items())):
                raise ValueError('Packet commands must map ASCII names to codes1..214747')
            if type(b.get('min')) is not int or type(b.get('max')) is not int or not 0<=b['min']<=b['max']<=9999:
                raise ValueError('Packet payload bounds must fit0..9999')
        elif b['kind'] == 'serial_integer_input':
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
    elif b['kind'] == 'serial_packet_input':
        options='\n'.join('  if(strcmp(begin,'+json.dumps(token)+')==0) {*value='+str(code)+'L*10000L+(int32_t)payload;return true;}' for token,code in b['commands'].items())
        parse='''  char* space=strchr(begin,' ');if(!space) return false;*space++='\\0';
  while(*space==' '||*space=='\\t')++space;if(!*space)return false;
  uint32_t payload=0;
  for(char* cursor=space;*cursor;++cursor){if(*cursor<'0'||*cursor>'9')return false;payload=payload*10+(*cursor-'0');if(payload>9999)return false;}
  if(payload<MINIMUM || payload>MAXIMUM) return false;
OPTIONS
  return false;'''.replace('MINIMUM',str(b['min'])).replace('MAXIMUM',str(b['max'])).replace('OPTIONS',options)
    elif b['kind'] == 'serial_string_input':
        parse = '''  strncpy(hw_rx_text,begin,63);hw_rx_text[63]='\\0';
  *value=(*value>=2147483647L)?1:*value+1;
  return true;'''
    elif b['kind'] == 'serial_datetime_input':
        parse = '''  if(strlen(begin)!=19 || begin[4]!='-' || begin[7]!='-' || begin[10]!='T' || begin[13]!=':' || begin[16]!=':') return false;
  for(uint8_t i=0;i<19;++i) if(i!=4&&i!=7&&i!=10&&i!=13&&i!=16 && (begin[i]<'0'||begin[i]>'9')) return false;
  unsigned y=0,m=0,d=0,h=0,mi=0,s=0;
  if(sscanf(begin,"%u-%u-%uT%u:%u:%u",&y,&m,&d,&h,&mi,&s)!=6 || y<2000 || y>2037 || m<1 || m>12 || d<1 || h>23 || mi>59 || s>59) return false;
  static const uint8_t days[]={31,28,31,30,31,30,31,31,30,31,30,31};
  unsigned maximum=days[m-1]+(m==2 && (y%4==0 && (y%100!=0||y%400==0)));
  if(d>maximum) return false;
  *value=(int32_t)DateTime(y,m,d,h,mi,s).unixtime();return true;'''
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
static char hw_rx_text[64];
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
