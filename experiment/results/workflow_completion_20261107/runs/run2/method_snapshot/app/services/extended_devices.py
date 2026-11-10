"""Registered bus devices and text formatting; application decisions stay in SysML."""
import json
import re
import string


I2C = {'sda_pin', 'scl_pin', 'address'}
LCD_PINS = {'cs_pin', 'dc_pin', 'rst_pin', 'mosi_pin', 'miso_pin', 'sck_pin'}
CATALOG = {
    'rtc_epoch': ('input', 'i32', 's', {'address': 104, 'acquisition': 'periodic'}, I2C | {'acquisition'}),
    'rtc_valid': ('input', 'bool', '1', {'address': 104, 'acquisition': 'periodic'}, I2C | {'acquisition'}),
    'mpu_acceleration': ('input', 'f64', 'm/s^2', {'address': 104}, I2C | {'axis'}),
    'mpu_gyro': ('input', 'f64', 'rad/s', {'address': 104}, I2C | {'axis'}),
    'mpu_gyro_raw': ('input', 'i32', 'raw', {'address': 104}, I2C | {'axis'}),
    'mpu_valid': ('input', 'bool', '1', {'address': 104}, I2C),
    'lcd_text': ('output', 'i32', '1', {'rotation': 1, 'color': 65535, 'background': 0,
                 'text_size': 2, 'x': 0, 'y': 0, 'rst_pin': -1},
                 LCD_PINS | {'rotation', 'color', 'background', 'text_size', 'x', 'y', 'messages', 'source_input'}),
    'lcd_number': ('output', 'f64', '1', {'rotation': 1, 'color': 65535, 'background': 0,
                   'text_size': 2, 'x': 0, 'y': 0, 'rst_pin': -1, 'format': '{0:.2f}'},
                   LCD_PINS | {'rotation', 'color', 'background', 'text_size', 'x', 'y', 'format'}),
    'lcd_datetime': ('output', 'i32', 's', {'rotation': 1, 'color': 65535, 'background': 0,
                     'text_size': 3, 'x': 0, 'y': 0, 'rst_pin': -1, 'format': '{0:date}'},
                     LCD_PINS | {'rotation', 'color', 'background', 'text_size', 'x', 'y', 'format'}),
    'seven_segment_number': ('output', 'i32', '1', {'digit_pins': [], 'common_anode': False,
                             'digit_active_high': True, 'leading_zero': True},
                             {'segment_pins', 'digit_pins', 'common_anode', 'digit_active_high', 'leading_zero'}),
    'tm1637_text': ('output','i32','1',{'brightness':7,'scroll_ms':300},
                    {'clk_pin','dio_pin','brightness','scroll_ms','messages'}),
    'serial_string_input': ('input', 'i32', '1', {'framing': 'line_or_idle', 'idle_timeout_ms': 20},
                           {'baud', 'framing', 'idle_timeout_ms'}),
    'serial_datetime_input': ('input', 'i32', 's', {'framing': 'line_or_idle', 'idle_timeout_ms': 20},
                             {'baud', 'framing', 'idle_timeout_ms'}),
    'serial_packet_input': ('input', 'i32', '1', {'framing':'line_or_idle','idle_timeout_ms':20,'min':0,'max':9999},
                           {'baud','framing','idle_timeout_ms','commands','min','max'}),
    'serial_echo': ('output', 'i32', '1', {}, {'baud', 'source_input'}),
}
INPUTS = {k for k, v in CATALOG.items() if v[0] == 'input'}
OUTPUTS = set(CATALOG) - INPUTS
RECEIVERS = {'serial_string_input', 'serial_datetime_input', 'serial_packet_input'}
SENSORS = INPUTS - RECEIVERS
LCD = {'lcd_text', 'lcd_number', 'lcd_datetime'}
GLYPHS = dict(zip('0123456789ACDEFHILNOPRSTUY- ',
    (63,6,91,79,102,109,125,7,127,111,119,57,94,121,113,118,6,56,84,63,115,80,109,120,62,110,64,0)))


def format_parts(template, count):
    if not isinstance(template, str) or not template or len(template.encode('utf8')) > 512 or '\0' in template:
        raise ValueError('Text format must contain 1..512 UTF-8 bytes without NUL')
    parts = list(string.Formatter().parse(template))
    allowed = {'', 'd', 'iso', 'rtc', 'date', 'weekday'} | {'.' + str(n) + 'f' for n in range(7)}
    if len(parts) > 32 or any(field is not None and (not re.fullmatch(r'0|[1-9][0-9]*', field)
            or int(field) >= count or spec not in allowed or conversion is not None)
            for _, field, spec, conversion in parts):
        raise ValueError('Format fields must reference numeric value indices with d/.0f..6f/iso/rtc/date/weekday')
    return parts


def formatted_lines(template, values, sink):
    lines = []
    for text, field, spec, _ in format_parts(template, len(values)):
        if text:
            lines.append(sink + '.print(' + json.dumps(text, ensure_ascii=False) + ');')
        if field is not None:
            value = values[int(field)]
            if spec in ('iso', 'rtc', 'date', 'weekday'):
                lines.append('{ char text[64]; hw_datetime_text((int32_t)(' + value + '), ' +
                             json.dumps(spec) + ', text); ' + sink + '.print(text); }')
            elif spec == 'd':
                lines.append(sink + '.print((long)(' + value + '));')
            else:
                lines.append(sink + '.print(' + value + (', ' + spec[1] if spec else '') + ');')
    return lines


def pin_fields(binding):
    kind = binding.get('kind') or ''
    if kind.startswith(('rtc_', 'mpu_')):
        return ['sda_pin', 'scl_pin']
    if kind in LCD:
        return [k for k in LCD_PINS if k in binding and binding[k] != -1]
    if kind=='tm1637_text':
        return ['clk_pin','dio_pin']
    return []


def all_pins(binding):
    values = [binding[f] for f in pin_fields(binding)]
    if binding.get('kind') == 'seven_segment_number':
        values += list(binding.get('segment_pins', [])) + list(binding.get('digit_pins', []))
    return values


def validate_port(port, ports, board):
    from app.services.hardware_adapter import physical_pin
    b, kind = port['binding'], port['binding']['kind']
    if kind not in CATALOG:
        return
    direction, typ, unit, _, _ = CATALOG[kind]
    if (port['direction'], port['type']) != (direction, typ) or (kind != 'lcd_number' and port['unit'] != unit):
        raise ValueError(kind + ': scalar type/direction/unit must match its device catalog')
    if kind.startswith(('rtc_', 'mpu_')):
        if kind.startswith('rtc_') and b.get('acquisition') not in ('once','periodic'):
            raise ValueError('RTC acquisition must be once/periodic')
        expected = (18, 19) if board.endswith(':uno') else (20, 21)
        if tuple(physical_pin(b.get(f), board) for f in ('sda_pin', 'scl_pin')) != expected:
            raise ValueError('I2C SDA/SCL must match the target hardware bus')
        if type(b.get('address')) is not int or b['address'] not in ((104,) if kind.startswith('rtc_') else (104, 105)):
            raise ValueError('Unsupported RTC/MPU I2C address')
        if kind in ('mpu_acceleration', 'mpu_gyro', 'mpu_gyro_raw') and b.get('axis') not in ('x', 'y', 'z'):
            raise ValueError('MPU axis must be x, y or z')
        if kind == 'mpu_gyro_raw' and b['axis'] != 'y':
            raise ValueError('Raw MPU acquisition currently supports Y at the documented 250 deg/s range')
    if kind in LCD:
        spi = (11, 12, 13) if board.endswith(':uno') else (51, 50, 52)
        if tuple(physical_pin(b.get(f), board) for f in ('mosi_pin', 'miso_pin', 'sck_pin')) != spi:
            raise ValueError('LCD SPI pins must match the target hardware bus')
        for key, lo, hi in [('rotation', 0, 3), ('text_size', 1, 8), ('x', 0, 319), ('y', 0, 319),
                            ('color', 0, 65535), ('background', 0, 65535)]:
            if type(b.get(key)) is not int or not lo <= b[key] <= hi:
                raise ValueError('Invalid LCD ' + key)
        if kind == 'lcd_text':
            if ('messages' in b) == ('source_input' in b):
                raise ValueError('lcd_text requires exactly one of messages or source_input')
            if 'messages' in b:
                messages = b['messages']
                if not isinstance(messages, dict) or not 1 <= len(messages) <= 16 or any(
                        not re.fullmatch(r'-?(0|[1-9][0-9]*)', str(k)) or not isinstance(v, str)
                        or '\0' in v or len(v.encode('utf8')) > 128 for k, v in messages.items()):
                    raise ValueError('LCD messages must map integer codes to bounded text')
            else:
                validate_text_source(b, ports)
        else:
            format_parts(b['format'], 1)
    if kind == 'serial_echo':
        validate_text_source(b, ports)
    if kind=='tm1637_text':
        if type(b.get('brightness')) is not int or not 0<=b['brightness']<=7 or type(b.get('scroll_ms')) is not int or not 100<=b['scroll_ms']<=5000:
            raise ValueError('TM1637 text requires brightness0..7, scroll_ms100..5000')
        messages=b.get('messages')
        if not isinstance(messages,dict) or not 1<=len(messages)<=16 or any(
                not re.fullmatch(r'-?(0|[1-9][0-9]*)',str(k)) or not -(2**31)<=int(k)<2**31 or
                not isinstance(v,str) or not 1<=len(v)<=32 or any(c not in GLYPHS for c in v.upper())
                for k,v in messages.items()):
            raise ValueError('TM1637 messages must map i32 codes to1..32 representable ASCII glyphs')
    if kind == 'seven_segment_number':
        segments, digits = b.get('segment_pins'), b.get('digit_pins')
        if not isinstance(segments, list) or len(segments) != 7 or not isinstance(digits, list) or len(digits) not in (0, 4):
            raise ValueError('Seven-segment output needs seven A..G pins and zero or four digit pins')
        numbers = [physical_pin(v, board) for v in segments + digits]
        if len(set(numbers)) != len(numbers):
            raise ValueError('Display pins must be distinct')
        if any(type(b.get(k)) is not bool for k in ('common_anode', 'digit_active_high', 'leading_zero')):
            raise ValueError('Display polarities/leading_zero must be Boolean')
        if digits and port['control_period_ms'] > 5:
            raise ValueError('Multiplexed display requires control_period_ms <=5 for refresh')


def validate_text_source(binding, ports):
    source = ports.get(binding.get('source_input'))
    if source is None or source['binding']['kind'] != 'serial_string_input':
        raise ValueError('Text output source_input must name the registered serial_string_input')


def validate_groups(ports):
    devices = {}
    for p in ports.values():
        kind=p['binding']['kind']
        if kind in SENSORS:
            family='rtc' if kind.startswith('rtc_') else 'mpu'
            address=p['binding']['address']
            devices.setdefault((family,address),[]).append(p)
    addresses={}
    for (family,address),channels in devices.items():
        if address in addresses and addresses[address]!=family:
            raise ValueError('Different I2C devices share address '+str(address)+'; preserve actual address-select wiring')
        addresses[address]=family
        if family+'_valid' not in {p['binding']['kind'] for p in channels}:
            raise ValueError('Native '+family+' acquisition requires its registered validity input')
        if len({p['sample_period_ms'] for p in channels})!=1:
            raise ValueError('Channels from one bus sensor require one coherent sample_period_ms')
        if family=='rtc' and len({p['binding']['acquisition'] for p in channels})!=1:
            raise ValueError('RTC epoch/validity must share acquisition once/periodic')


DATETIME = r'''
#include <RTClib.h>
#include <stdio.h>
#include <string.h>
static void hw_datetime_text(int32_t epoch, const char* style, char* text) {
  static const char* days[]={"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};
  DateTime t((uint32_t)epoch);
  if(strcmp(style,"weekday")==0) {strcpy(text,days[t.dayOfTheWeek()]);return;}
  if(strcmp(style,"rtc")==0) snprintf(text,64,"%u/%u/%u (%s) %u:%u:%u",t.year(),t.month(),t.day(),days[t.dayOfTheWeek()],t.hour(),t.minute(),t.second());
  else if(strcmp(style,"date")==0) snprintf(text,64,"%04u-%02u-%02u",t.year(),t.month(),t.day());
  else snprintf(text,64,"%04u-%02u-%02uT%02u:%02u:%02u",t.year(),t.month(),t.day(),t.hour(),t.minute(),t.second());
}
'''


def render(contract):
    """Return declarations, startup, acquisition and output-only transport code."""
    from app.services.hardware_adapter import pin, literal
    ports = contract['interfaces']; board = contract.get('board', 'arduino:avr:uno')
    declarations, setup, sampling, writing, bauds = [], [], [], [], set()
    if any(p['binding']['kind'].startswith(('rtc_', 'mpu_')) for p in ports):
        declarations.append('#include <Wire.h>'); setup.append('Wire.begin();')
    needs_date = any(p['binding']['kind'] in ('rtc_epoch', 'rtc_valid', 'serial_datetime_input', 'lcd_datetime')
                     or any(':' + f + '}' in p['binding'].get('format', '') for f in ('iso', 'rtc', 'date', 'weekday'))
                     for p in ports)
    if needs_date:
        declarations.append(DATETIME)
    sensors = {}
    for p in ports:
        b = p['binding']; kind = b['kind']
        if kind in SENSORS:
            sensors.setdefault(('rtc' if kind.startswith('rtc_') else 'mpu', b['address']), []).append(p)
    for index, ((family, address), channels) in enumerate(sensors.items()):
        name = 'hw_bus_' + str(index)
        periods = {p['sample_period_ms'] for p in channels}
        if family + '_valid' not in {p['binding']['kind'] for p in channels}:
            raise ValueError('Native '+family+' acquisition requires its registered validity input')
        if len(periods) != 1:
            raise ValueError('Channels from one bus sensor require one coherent sample_period_ms')
        raw = any(p['binding']['kind'] == 'mpu_gyro_raw' for p in channels)
        if raw and any(p['binding']['kind'] in ('mpu_acceleration', 'mpu_gyro') for p in channels):
            raise ValueError('Raw and physical MPU requests need separate explicit acquisition contracts')
        if family == 'rtc':
            declarations.append('RTC_DS1307 ' + name + ';')
            begin = name + '.begin()'
        elif raw:
            declarations += ['#include <MPU6050.h>', 'MPU6050 ' + name + '(' + str(address) + ');']
            setup.append(name + '.initialize();'); setup.append(name + '.setFullScaleGyroRange(MPU6050_GYRO_FS_250);')
            begin = name + '.testConnection()'
        else:
            declarations += ['#include <Adafruit_MPU6050.h>', '#include <Adafruit_Sensor.h>', 'Adafruit_MPU6050 ' + name + ';']
            begin = name + '.begin(' + str(address) + ', &Wire)'
        declarations += [f'static bool {name}_ready=false, {name}_sampled=false;', f'static uint32_t {name}_last=0;']
        setup.append(name + '_ready=' + begin + ';')
        block = [f'if (!{name}_sampled || (uint32_t)(hw_now-{name}_last)>={next(iter(periods))}UL) {{',
                 f'  {name}_sampled=true; {name}_last=hw_now;', '  bool sample_valid=false;', f'  if ({name}_ready) {{']
        if family == 'rtc':
            block.append(f'    uint32_t epoch={name}.now().unixtime();')
            block.append('    sample_valid=epoch>=946684800UL && epoch<=2147483647UL;')
        elif not raw:
            block += ['    sensors_event_t acceleration,gyro,temperature;', f'    sample_valid={name}.getEvent(&acceleration,&gyro,&temperature);']
        else:
            block.append('    sample_valid='+name+'.testConnection();')
        for p in channels:
            kind = p['binding']['kind']; local = 'hw_value_' + p['name']
            if kind == 'rtc_epoch': expr = '(int32_t)epoch'
            elif kind.endswith('_valid'): expr = 'sample_valid'
            elif kind == 'mpu_gyro_raw': expr = name + '.getRotationY()'
            else: expr = ('acceleration.acceleration.' if kind == 'mpu_acceleration' else 'gyro.gyro.') + p['binding']['axis']
            block.append('    ' + ('if (sample_valid) ' if not kind.endswith('_valid') else '') + local + '=' + expr + ';')
        block.append('  }')
        for p in channels:
            if p['binding']['kind'].endswith('_valid'):
                block.append('  hw_value_' + p['name'] + '=sample_valid;')
        sampling += block + ['}']
        if family=='rtc' and channels[0]['binding']['acquisition']=='once':
            # Perform the single native read during setup, before controller ticks.
            first = ['if ('+name+'_ready) {', 'uint32_t epoch='+name+'.now().unixtime();',
                     name+'_ready=epoch>=946684800UL && epoch<=2147483647UL;']
            for p in channels:
                if p['binding']['kind']=='rtc_epoch':
                    first.append('if ('+name+'_ready) hw_value_'+p['name']+'=(int32_t)epoch;')
            first += ['}'] + ['hw_value_'+p['name']+'='+name+'_ready;' for p in channels if p['binding']['kind']=='rtc_valid']
            setup += first
            del sampling[-(len(block)+1):]
    displays = {}
    for p in ports:
        if p['binding']['kind'] in LCD:
            b = p['binding']; displays.setdefault(tuple(b[f] for f in ('cs_pin', 'dc_pin', 'rst_pin')), []).append(p)
    for index, (signals, slots) in enumerate(displays.items()):
        name = 'hw_lcd_' + str(index); b = slots[0]['binding']
        reset = '-1' if signals[2] == -1 else pin(signals[2], board)
        if any(any(p['binding'][f] != b[f] for f in ('rotation', 'background')) for p in slots):
            raise ValueError('LCD fields on one screen must agree on rotation/background')
        declarations += ['#include <SPI.h>', '#include <Adafruit_GFX.h>', '#include <Adafruit_ILI9341.h>',
                         f'Adafruit_ILI9341 {name}({pin(signals[0],board)}, {pin(signals[1],board)}, {reset});',
                         f'static bool {name}_drawn=false;']
        setup += [name + '.begin();', name + '.setRotation(' + str(b['rotation']) + ');', name + '.fillScreen(' + str(b['background']) + ');']
        changed = []
        for p in slots:
            local = 'hw_value_' + p['name']; previous = 'hw_previous_' + p['name']
            declarations.append('static ' + ('double' if p['type'] == 'f64' else 'int32_t') + ' ' + previous + '=0;')
            changed.append(local + '!=' + previous)
        block = ['if (!' + name + '_drawn || ' + ' || '.join(changed) + ') {', name + '_drawn=true;', name + '.fillScreen(' + str(b['background']) + ');']
        for p in slots:
            b = p['binding']; local = 'hw_value_' + p['name']
            block += [f'{name}.setCursor({b["x"]},{b["y"]});', f'{name}.setTextColor({b["color"]});', f'{name}.setTextSize({b["text_size"]});']
            if b['kind'] == 'lcd_text':
                if 'source_input' in b:
                    block.append(f'if ({local}>0) {name}.print(hw_rx_text);')
                else:
                    block.append('switch (' + local + ') {')
                    block += ['case ' + str(int(k)) + ': ' + name + '.print(' + json.dumps(v,ensure_ascii=False) + '); break;' for k,v in b['messages'].items()]
                    block.append('default: break; }')
            else:
                block += formatted_lines(b['format'], [local], name)
            block.append('hw_previous_' + p['name'] + '=' + local + ';')
        writing += block + ['}']
    for p in ports:
        b = p['binding']; kind = b['kind']; local = 'hw_value_' + p['name']
        if kind=='tm1637_text':
            prefix='hw_tm_text_'+p['name']
            font=''.join('case '+str(ord(c))+':return '+str(mask)+';' for c,mask in GLYPHS.items())
            declarations += ['#include <TM1637Display.h>','#include <string.h>',
                f'TM1637Display {prefix}({pin(b["clk_pin"],board)},{pin(b["dio_pin"],board)});',
                f'static int32_t {prefix}_previous=INT32_MIN;',f'static const char* {prefix}_text=nullptr;',
                f'static uint32_t {prefix}_start=0;',f'static uint16_t {prefix}_frame=65535;',
                f'static uint8_t {prefix}_glyph(char c) {{switch(c) {{'+font+'default:return 0;}}']
            setup += [f'{prefix}.setBrightness({b["brightness"]},true);',f'{prefix}.clear();']
            block=[f'if({local}!={prefix}_previous) {{',f'{prefix}_previous={local};{prefix}_text=nullptr;',
                   f'{prefix}_start=millis();{prefix}_frame=65535;',f'switch({local}) {{']
            block += ['case '+str(int(k))+': '+prefix+'_text='+json.dumps(v.upper())+'; break;' for k,v in b['messages'].items()]
            block += ['default: break;}',f'if(!{prefix}_text) {{if({local}>=-999&&{local}<=9999) {prefix}.showNumberDec((int){local},false,4,0);else {prefix}.clear();}}','}']
            block += [f'if({prefix}_text) {{',f'uint8_t length=strlen({prefix}_text);',
                      f'uint16_t frame=length<=4?0:((uint32_t)(millis()-{prefix}_start)/{b["scroll_ms"]}UL)%(length+4);',
                      f'if(frame!={prefix}_frame) {{',f'{prefix}_frame=frame;uint8_t segments[4]={{0,0,0,0}};',
                      f'for(uint8_t i=0;i<4;++i) if(frame+i<length) segments[i]={prefix}_glyph({prefix}_text[frame+i]);',
                      f'{prefix}.setSegments(segments,4,0);','}','}']
            writing+=block
        if kind == 'serial_echo':
            bauds.add(b['baud']); previous = 'hw_previous_' + p['name']
            declarations.append('static int32_t ' + previous + '=0;')
            writing.append(f'if ({local}>0 && {local}!={previous}) {{Serial.println(hw_rx_text);{previous}={local};}}')
        if kind == 'report_trigger' and 'format' in b:
            values = ['hw_value_' + v for v in b['values']]
            writing += ['if (' + local + ') {'] + formatted_lines(b['format'], values, 'Serial') + ['Serial.println();', '}']
        if kind == 'seven_segment_number':
            prefix = 'hw_seven_' + p['name']; segments = [pin(v,board) for v in b['segment_pins']]; digits = [pin(v,board) for v in b['digit_pins']]
            declarations.append(f'static const uint8_t {prefix}_segments[]={{'+','.join(segments)+'};')
            declarations.append(f'static const uint8_t {prefix}_masks[]={{63,6,91,79,102,109,125,7,127,111}};')
            setup += ['pinMode(' + v + ',OUTPUT);' for v in segments + digits]
            if digits:
                declarations += [f'static const uint8_t {prefix}_digits[]={{'+','.join(digits)+'};', f'static uint8_t {prefix}_digit=0;']
                off = 'LOW' if b['digit_active_high'] else 'HIGH'; on = 'HIGH' if b['digit_active_high'] else 'LOW'
                block = [f'for(uint8_t i=0;i<4;++i) digitalWrite({prefix}_digits[i],{off});',
                         f'int32_t number={local}; uint8_t digit={prefix}_digit;',
                         'for(uint8_t i=digit;i<3;++i) number/=10;',
                         f'uint8_t mask=({local}>=0 && {local}<=9999)?{prefix}_masks[number%10]:0;']
                if not b['leading_zero']:
                    block.append(f'if(digit<3 && {local} < (digit==0?1000:digit==1?100:10)) mask=0;')
            else:
                block = [f'uint8_t mask=({local}>=0 && {local}<=9)?{prefix}_masks[{local}]:0;']
            block.append(f'for(uint8_t i=0;i<7;++i) digitalWrite({prefix}_segments[i],((mask>>i)&1)?'+('LOW:HIGH' if b['common_anode'] else 'HIGH:LOW')+');')
            if digits:
                block += [f'digitalWrite({prefix}_digits[{prefix}_digit],{on});', f'{prefix}_digit=({prefix}_digit+1)%4;']
            writing += ['{'] + block + ['}']
    return declarations, setup, sampling, writing, bauds
