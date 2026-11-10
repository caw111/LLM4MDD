"""Typed behavior components expanded to ordinary, reference-checked SysML actions.

No IR, hardware code, requirement inference or task-specific dispatch lives here.
Components execute in listed order, before user updates, and export read-only bools.
"""
import copy
import re

from app.services.ir_contract import fields, identifier

PREFIX = 'component_'
MAX_I32 = 2147483647


def expand_components(spec, inputs):
    def protect_writes(value):
        if isinstance(value, dict):
            target = value.get('assign')
            if isinstance(target, str) and any(part.startswith(PREFIX) for part in target.split('.')):
                raise ValueError('Component storage is read-only outside its component: ' + target)
            for child in value.values():
                protect_writes(child)
        elif isinstance(value, list):
            for child in value:
                protect_writes(child)
    protect_writes(spec.get('updates'))
    components = spec.get('components', [])
    if not isinstance(components, list) or len(components) > 16:
        raise ValueError('components must be a list of at most 16 behavioral components')
    if not components:
        return {key: value for key, value in spec.items() if key != 'components'}
    if not isinstance(spec['states'], list) or not isinstance(spec['updates'], list):
        raise ValueError('states and updates must be lists')
    symbols = {p['name']: p for p in inputs}
    states, updates, exports = [], [], set()

    def state(name, typ, initial):
        states.append(dict(name=name, type=typ, initial=initial))

    def assign(name, value):
        return dict(assign=name, value=value)

    def branch(condition, yes, no=None):
        return {'if': condition, 'then': yes, **({'else': no} if no else {})}

    def boolean(value, label):
        if type(value) is not bool:
            raise ValueError(label + ' must be an explicit JSON Boolean')
        return str(value).lower()

    def scalar(value, typ, *, literal=False):
        if literal and typ == 'bool' and type(value) is bool:
            return str(value).lower()
        if literal and value in ('true', 'false'):
            return value
        if not isinstance(value, str) or value not in symbols or symbols[value]['type'] != typ:
            raise ValueError('Component input must reference a declared ' + typ +
                             ' input or an earlier component export: ' + str(value))
        return value

    for item in components:
        if not isinstance(item, dict):
            raise ValueError('Each component must be an object')
        name = identifier(item.get('name'))
        base = PREFIX + name + '_'
        value = base + 'value'
        if value in exports:
            raise ValueError('Duplicate component name: ' + name)
        if item.get('kind') == 'edge_toggle':
            fields(item, ['kind', 'name', 'input', 'edge', 'initial', 'previous_input'])
            signal = scalar(item['input'], 'bool')
            if item['edge'] not in ('rising', 'falling'):
                raise ValueError('edge_toggle.edge must be rising or falling')
            previous = base + 'previous'
            state(value, 'bool', boolean(item['initial'], 'edge_toggle.initial'))
            state(previous, 'bool', boolean(item['previous_input'], 'edge_toggle.previous_input'))
            edge = signal + ' and not ' + previous if item['edge'] == 'rising' else 'not ' + signal + ' and ' + previous
            updates.extend([branch(edge, [assign(value, 'not ' + value)]), assign(previous, signal)])
        elif item.get('kind') == 'periodic_pulse':
            fields(item, ['kind', 'name', 'delta', 'enabled', 'period_ms', 'pulse_ms', 'first_pulse', 'pause'])
            delta = scalar(item['delta'], 'i32')
            if symbols[delta].get('unit', 'ms') != 'ms':
                raise ValueError('periodic_pulse.delta must have unit ms')
            if 'binding' in symbols[delta] and symbols[delta]['binding'].get('kind') != 'elapsed_ms':
                raise ValueError('periodic_pulse.delta must use the elapsed_ms hardware binding')
            enabled = scalar(item['enabled'], 'bool', literal=True)
            period, width = item['period_ms'], item['pulse_ms']
            if type(period) is not int or not 1 <= period <= MAX_I32:
                raise ValueError('period_ms must be an integer in 1..2147483647')
            if type(width) is not int or not 1 <= width <= period:
                raise ValueError('pulse_ms must be an integer in 1..period_ms')
            if item['first_pulse'] not in ('after_period', 'immediate', 'after_silence') or item['pause'] not in ('preserve', 'reset'):
                raise ValueError('first_pulse must be after_period/immediate/after_silence; pause must be preserve/reset')
            if item['first_pulse']=='after_silence' and width==period:
                raise ValueError('after_silence requires pulse_ms < period_ms; use immediate for zero initial silence')
            phase, started, previous, remainder = (base + suffix for suffix in ('phase', 'started', 'enabled', 'remainder'))
            initial = str(item['first_pulse'] == 'immediate').lower()
            initial_phase = str(width) if item['first_pulse']=='after_silence' else '0'
            for n, typ, init in [(phase, 'i32', initial_phase), (started, 'bool', initial),
                                 (previous, 'bool', 'false'), (remainder, 'i32', '0'), (value, 'bool', 'false')]:
                state(n, typ, init)
            advance = [branch(delta + ' >= ' + str(period) + ' - ' + phase, [assign(started, 'true')]), assign(remainder, delta)]
            # Reduce a nonnegative i32 delta without floating-point rounding, loops,
            # signed overflow, or an unsupported SysML modulo operator. Each binary
            # multiple is subtracted at most once. All intermediate values fit i32.
            multiples, multiple = [], period
            while multiple <= MAX_I32:
                multiples.append(multiple)
                multiple *= 2
            for multiple in reversed(multiples):
                advance.append(branch(remainder + ' >= ' + str(multiple), [assign(remainder, remainder + ' - ' + str(multiple))]))
            advance.append(branch(remainder + ' >= ' + str(period) + ' - ' + phase,
                [assign(phase, remainder + ' - (' + str(period) + ' - ' + phase + ')')],
                [assign(phase, phase + ' + ' + remainder)]))
            # delta covers the interval ending at THIS step. Attribute that interval
            # to the previous enable state, then apply the new gate immediately.
            updates.append(branch(previous + ' and ' + delta + ' > 0', advance))
            if item['pause'] == 'reset':
                updates.append(branch('not (' + enabled + ')', [assign(phase, initial_phase), assign(started, initial)]))
            updates.extend([assign(value, '(' + enabled + ') and ' + started + ' and ' + phase + ' < ' + str(width)),
                            assign(previous, enabled)])
        else:
            raise ValueError('Unsupported component kind; use edge_toggle or periodic_pulse')
        exports.add(value)
        symbols[value] = {'name': value, 'type': 'bool'}

    # Reserve generated storage. Free-form behavior can consume exported values,
    # but cannot overwrite component state or couple to its implementation details.
    for port in inputs:
        if port['name'].startswith(PREFIX):
            raise ValueError('Input collides with reserved component_ namespace')

    def check(value, key=None):
        if isinstance(value, dict):
            for k, v in value.items():
                check(v, k)
        elif isinstance(value, list):
            for v in value:
                check(v, key)
        elif isinstance(value, str):
            names = re.findall(r'\bcomponent_[A-Za-z0-9_]+\b', value)
            if names and (key in ('name', 'assign', 'left', 'right') or any(n not in exports for n in names)):
                raise ValueError('Component storage is reserved; only component_NAME_value is readable: ' + value)
    original = {k: v for k, v in spec.items() if k != 'components'}
    check(original)
    result = copy.deepcopy(original)
    result['states'] = states + result['states']
    result['updates'] = updates + result['updates']
    return result
