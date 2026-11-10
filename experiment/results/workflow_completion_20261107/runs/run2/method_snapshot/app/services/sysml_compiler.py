"""Deterministic lowering of validated SysML scalar dataflow to the existing graph IR."""
from __future__ import annotations
import math
import re
from app.services.sysml import validate_sysml
from app.services.ir_contract import validate_ir

class SysMLCompileError(ValueError):
    def __init__(self, message, element=None):
        self.issue = {'severity': 'error', 'code': 'SYSML_LOWERING', 'message': message,
                      'line': (element or {}).get('line'), 'element': (element or {}).get('qualifiedName')}
        super().__init__(message)


def compile_sysml(source: str, *, root: str | None = None, validated: dict | None = None) -> dict:
    model = validated if validated is not None else validate_sysml(source)
    if not model['ok']:
        raise SysMLCompileError('Model must pass the reference validator before lowering.')
    elements = {e['id']: e for e in model['elements']}
    def obj(ref):
        if not ref or ref.get('id') not in elements:
            raise SysMLCompileError('Reference is outside the executable model: ' + str(ref))
        return elements[ref['id']]
    def members(e):
        return [obj(x) for x in e['fields'].get('ownedMember', [])]
    candidates = [e for e in elements.values() if e['kind'] == 'PartDefinition']
    instances = [e for e in elements.values() if e['kind'] == 'PartUsage' and e.get('name') == 'system']
    if root:
        candidates = [e for e in candidates if root in (e.get('name'), e.get('qualifiedName'))]
    elif len(instances) == 1:
        if any(m['kind'] not in ('Multiplicity', 'MultiplicityRange') for m in members(instances[0])):
            raise SysMLCompileError('Top-level instance overrides are not yet supported.', instances[0])
        for member in members(instances[0]):
            if member['kind'] == 'MultiplicityRange' and not member.get('scalarMultiplicity', False):
                raise SysMLCompileError('The selected system instance must have multiplicity [1].', instances[0])
        candidates = [obj(t) for t in instances[0]['fields'].get('type', []) if t['kind'] == 'PartDefinition']
    elif len(instances) > 1:
        raise SysMLCompileError('Multiple system instances exist; select an explicit root.')
    if len(candidates) != 1:
        raise SysMLCompileError('Select exactly one executable part definition using root, or declare part system : RootType.', None)
    system = candidates[0]
    attributes = []
    locations = {}
    child_scopes = {}
    bindings = []
    actions = []
    def collect(definition, scope, ancestry):
        if definition['id'] in ancestry:
            raise SysMLCompileError('Recursive part instantiation cannot be allocated statically.', definition)
        for relation in definition['fields'].get('ownedRelationship', []):
            if relation['kind'] in ('Subclassification', 'Specialization'):
                raise SysMLCompileError('Inherited part definitions are not yet supported.', definition)
        for e in members(definition):
            for child in members(e):
                if child['kind'] == 'MultiplicityRange' and not child.get('scalarMultiplicity', False):
                    raise SysMLCompileError('Only multiplicity [1] is executable; arrays and optional values need explicit lowering.', e)
            if e['fields'].get('direction') == 'inout':
                raise SysMLCompileError('inout requires explicit state semantics.', e)
            if e['kind'] == 'AttributeUsage':
                item = dict(e, id=scope + ':' + e['id'], name=(scope.replace('.', '__') + '__' if scope else '') + e.get('name', ''))
                item['_scope'] = scope
                item['fields'] = dict(e['fields'])
                if scope:
                    item['fields'].pop('direction', None)
                attributes.append(item)
                locations[(scope, e['id'])] = item['id']
            elif e['kind'] == 'PartUsage':
                if not e.get('name') or not re.fullmatch('[A-Za-z_][A-Za-z0-9_]*', e['name']):
                    raise SysMLCompileError('Part instances require identifier names.', e)
                definitions = [t for t in e['fields'].get('type', []) if t['kind'] == 'PartDefinition']
                if len(definitions) != 1:
                    raise SysMLCompileError('Each part instance needs one concrete definition.', e)
                if any(m['kind'] not in ('Multiplicity', 'MultiplicityRange') for m in members(e)):
                    raise SysMLCompileError('Instance body overrides must be expressed using explicit system bindings.', e)
                child = scope + '.' + e['name'] if scope else e['name']
                child_scopes[(scope, e['id'])] = child
                collect(obj(definitions[0]), child, ancestry + (definition['id'],))
            elif e['kind'] == 'PerformActionUsage':
                actions.append((scope, e))
            elif e['kind'] == 'BindingConnectorAsUsage':
                bindings.append((scope, e))
            elif e['kind'] != 'Documentation':
                raise SysMLCompileError('Unsupported executable member ' + e['kind'] + '; it cannot be silently discarded.', e)
    collect(system, '', ())

    def resolve_feature(ref, scope):
        e = obj(ref)
        if (scope, e['id']) in locations:
            return ('scalar', locations[(scope, e['id'])])
        if (scope, e['id']) in child_scopes:
            return ('part', child_scopes[(scope, e['id'])])
        chain = [obj(r)['fields']['chainingFeature'] for r in e['fields'].get('ownedRelationship', []) if r['kind'] == 'FeatureChaining']
        if chain:
            result = None
            for index, item in enumerate(chain):
                result = resolve_feature(item, scope)
                if index < len(chain)-1:
                    if result[0] != 'part':
                        raise SysMLCompileError('Intermediate feature must be a part instance.', e)
                    scope = result[1]
            return result
        raise SysMLCompileError('Reference cannot be resolved in this concrete system instance.', e)

    def resolve_expression(e, scope):
        refs = [obj(r) for r in e['fields'].get('ownedRelationship', []) if r['kind'] == 'Membership']
        if len(refs) != 1:
            raise SysMLCompileError('Expected one resolved feature membership.', e)
        if e['kind'] == 'FeatureChainExpression':
            args = e['fields'].get('argument', [])
            if len(args) != 1:
                raise SysMLCompileError('Invalid feature-chain base.', e)
            base = resolve_expression(obj(args[0]), scope)
            if base[0] != 'part':
                raise SysMLCompileError('Feature-chain base must be a part instance.', e)
            scope = base[1]
        return resolve_feature(refs[0]['fields']['memberElement'], scope)
    if not attributes:
        raise SysMLCompileError('Executable part has no scalar attributes.', system)
    names = set()
    types = {}
    for e in attributes:
        name = e.get('name', '')
        if not re.fullmatch('[A-Za-z_][A-Za-z0-9_]*', name):
            raise SysMLCompileError('Invalid C-compatible scalar identifier: ' + name, e)
        if name in names:
            raise SysMLCompileError('Duplicate scalar declaration: ' + name + '. Declare each root input/output/state only once; keep the required value expression.', e)
        names.add(name)
        ts = e['fields'].get('type', [])
        mapping = {'ScalarValues::Real': 'f64', 'ScalarValues::Integer': 'i32', 'ScalarValues::Boolean': 'bool'}
        if len(ts) != 1 or ts[0].get('name') not in mapping:
            raise SysMLCompileError('Unsupported scalar type; use ScalarValues::Real, Integer or Boolean.', e)
        types[e['id']] = mapping[ts[0]['name']]
    ir = {'version': 2, 'name': system['name'], 'page': 'ProgramPage1', 'inputs': [], 'outputs': [],
          'nodes': [], 'edges': [], 'states': [], 'updates': [], 'execution': 'step-actions-then-outputs'}
    endpoint = {}
    active = set()
    by_id = {e['id']: e for e in attributes}
    for e in attributes:
        if e['fields'].get('direction') == 'in':
            ir['inputs'].append({'name': e['name'], 'type': types[e['id']]})
            endpoint[e['id']] = {'kind': 'input', 'name': e['name']}
        elif e['fields'].get('direction') == 'out':
            ir['outputs'].append({'name': e['name'], 'type': types[e['id']]})
        elif e['fields'].get('direction') is not None:
            raise SysMLCompileError('inout attributes require state semantics and cannot be lowered as pure dataflow.', e)

    states = {}
    for e in attributes:
        initial = [obj(r) for r in e['fields'].get('ownedRelationship', []) if r['kind'] == 'FeatureValue' and obj(r)['fields'].get('isInitial')]
        if initial:
            if e['fields'].get('direction') == 'in':
                raise SysMLCompileError('An external input cannot also be persistent state.', e)
            states[e['id']] = (e, initial[0])
            endpoint[e['id']] = {'kind': 'state', 'name': e['name']}

    # A binding is equality, not a directed assignment. Select the unique actual driver.
    parents = {e['id']: e['id'] for e in attributes}
    def find(key):
        while parents[key] != key:
            key = parents[key]
        return key
    for scope, binding in bindings:
        ends = [resolve_feature(r, scope) for r in binding['fields'].get('relatedFeature', [])]
        if len(ends) != 2 or any(e[0] != 'scalar' for e in ends):
            raise SysMLCompileError('Bindings require two scalar endpoints.', binding)
        left, right = (e[1] for e in ends)
        if types[left] != types[right]:
            raise SysMLCompileError('Binding endpoint types differ.', binding)
        parents[find(left)] = find(right)
    groups = {}
    for key in parents:
        groups.setdefault(find(key), []).append(key)
    bound = {}
    for group in groups.values():
        if len(group) == 1:
            continue
        drivers = [key for key in group if key in endpoint or any(r['kind'] == 'FeatureValue' for r in by_id[key]['fields'].get('ownedRelationship', []))]
        if len(drivers) != 1:
            raise SysMLCompileError('Each connected scalar group requires exactly one driver; found ' + str(len(drivers)))
        bound.update({key: drivers[0] for key in group if key != drivers[0]})

    def expression(e, dependencies, scope):
        f = e['fields']
        kind = e['kind']
        if kind == 'FeatureReferenceExpression':
            deferred = [obj(r) for r in f.get('ownedRelationship', []) if r['kind'] == 'FeatureMembership']
            if len(deferred) == 1:
                return expression(obj(deferred[0]['fields']['memberElement']), dependencies, scope)
        if kind in ('FeatureReferenceExpression', 'FeatureChainExpression'):
            resolved = resolve_expression(e, scope)
            if resolved[0] != 'scalar':
                raise SysMLCompileError('A scalar expression cannot use a part as a value.', e)
            target = resolved[1]
            if target not in dependencies:
                dependencies.append(target)
            return 'arg' + str(dependencies.index(target)), types[target]
        if kind in ('LiteralInteger', 'LiteralRational', 'LiteralBoolean'):
            v = f.get('value')
            if kind == 'LiteralBoolean':
                return ('true' if v else 'false'), 'bool'
            if not isinstance(v, (int, float)) or not math.isfinite(v):
                raise SysMLCompileError('Invalid numeric literal.', e)
            if kind == 'LiteralInteger' and not -(2**31) <= v < 2**31:
                raise SysMLCompileError('Integer literal exceeds backend i32 range.', e)
            return str(v), ('i32' if kind == 'LiteralInteger' else 'f64')
        if kind == 'InvocationExpression':
            functions = f.get('type', [])
            if (len(functions) != 1 or functions[0].get('name') != 'RealFunctions::floor'
                    or functions[0].get('id') in elements or len(f.get('argument', [])) != 1):
                raise SysMLCompileError('Unsupported function; only reference-library RealFunctions::floor(x) is executable.', e)
            x, typ = expression(obj(f['argument'][0]), dependencies, scope)
            if typ == 'bool':
                raise SysMLCompileError('floor requires a numeric argument.', e)
            # Explicit conversion, with negative values rounded toward -infinity.
            # Like other i32 operations, the mathematical result must fit i32.
            return '((int)(' + x + ') - ((' + x + ') < (double)((int)(' + x + ')) ? 1 : 0))', 'i32'
        if kind != 'OperatorExpression':
            raise SysMLCompileError('Unsupported expression: ' + kind, e)
        op = f['operator']
        args = [expression(obj(a), dependencies, scope) for a in f.get('argument', [])]
        if op == 'if' and len(args) == 3:
            (condition, ct), (yes, yt), (no, nt) = args
            if ct != 'bool':
                guard = obj(f['argument'][0]).get('text', condition).strip()[:180]
                raise SysMLCompileError(f'Conditional guard {guard!r} has type {ct}; expected bool. '
                                        'Numeric measurements are not Boolean validity flags. Use the declared validity input '
                                        'or the explicit comparison required by the original requirement; changing branch values cannot fix this guard.', e)
            if (yt == 'bool') != (nt == 'bool'):
                raise SysMLCompileError(f'Conditional branch types are incompatible: then={yt}, else={nt}. '
                                        'Both branches must be Boolean or both numeric; preserve the required output type.', e)
            return '(' + condition + ' ? ' + yes + ' : ' + no + ')', ('f64' if 'f64' in (yt, nt) else yt)
        if len(args) == 1 and op in ('+', '-', 'not'):
            x, t = args[0]
            if (op == 'not') != (t == 'bool'):
                raise SysMLCompileError('Invalid unary operand type.', e)
            return '(' + ('!' if op == 'not' else op) + x + ')', t
        if len(args) != 2:
            raise SysMLCompileError('Unsupported operator arity: ' + op, e)
        (a, ta), (b, tb) = args
        if op in ('and', 'or', 'xor'):
            if ta != 'bool' or tb != 'bool':
                raise SysMLCompileError('Logical operators require Boolean operands.', e)
            cop, typ = {'and': '&&', 'or': '||', 'xor': '!='}[op], 'bool'
        elif op in ('==', '!=', '<', '<=', '>', '>='):
            if (ta == 'bool') != (tb == 'bool') or (ta == 'bool' and op not in ('==', '!=')):
                raise SysMLCompileError('Incompatible comparison types.', e)
            cop, typ = op, 'bool'
        elif op in ('+', '-', '*', '/'):
            if 'bool' in (ta, tb):
                raise SysMLCompileError('Arithmetic requires numeric operands.', e)
            cop, typ = op, ('f64' if 'f64' in (ta, tb) or op == '/' else 'i32')
            if op == '/':
                a = '(double)(' + a + ')'
        else:
            raise SysMLCompileError('Unsupported operator: ' + op, e)
        return '(' + a + ' ' + cop + ' ' + b + ')', typ

    def lower(e):
        eid = e['id']
        values = [obj(r) for r in e['fields'].get('ownedRelationship', []) if r['kind'] == 'FeatureValue']
        if eid in endpoint:
            if values and endpoint[eid]['kind'] == 'input':
                raise SysMLCompileError('External input defaults/bindings need an explicit adapter.', e)
            return endpoint[eid]
        if eid in active:
            raise SysMLCompileError('Algebraic cycle detected; state/delay must be explicit.', e)
        if eid in bound:
            active.add(eid)
            endpoint[eid] = lower(by_id[bound[eid]])
            active.remove(eid)
            return endpoint[eid]
        if len(values) != 1 or values[0]['fields'].get('isInitial') or values[0]['fields'].get('isDefault'):
            raise SysMLCompileError('Every computed scalar needs one non-default, non-initial value expression.', e)
        active.add(eid)
        dependencies = []
        code, typ = expression(obj(values[0]['fields']['value']), dependencies, e['_scope'])
        if typ != types[eid] and not (typ == 'i32' and types[eid] == 'f64'):
            raise SysMLCompileError('Expression type ' + typ + ' does not match ' + types[eid], e)
        sources = [lower(by_id[d]) for d in dependencies]
        nid = 'compute_' + e['name']
        ir['nodes'].append({'id': nid, 'type': 'Expression',
            'inports': [{'name': 'arg'+str(i), 'type': types[d]} for i, d in enumerate(dependencies)],
            'outports': [{'name': 'result', 'type': types[eid]}], 'params': {'Expression': code}})
        for i, src in enumerate(sources):
            ir['edges'].append({'src': src, 'dst': {'kind': 'node', 'id': nid, 'port': 'arg'+str(i)}})
        endpoint[eid] = {'kind': 'node', 'id': nid, 'port': 'result'}
        active.remove(eid)
        return endpoint[eid]
    for eid, (e, initial) in states.items():
        dependencies = []
        code, typ = expression(obj(initial['fields']['value']), dependencies, e['_scope'])
        if dependencies or (typ != types[eid] and not (typ == 'i32' and types[eid] == 'f64')):
            raise SysMLCompileError('State initialization requires a compatible constant expression.', e)
        ir['states'].append({'name': e['name'], 'type': types[eid], 'initial': code})
    if len(actions) > 1:
        raise SysMLCompileError('Multiple unsequenced performed actions have no deterministic execution order.')

    def lower_action(action, scope):
        fields = action['fields']
        if action['kind'] == 'AssignmentActionUsage':
            target = resolve_feature(fields.get('referent'), scope)
            if target[0] != 'scalar' or target[1] not in states:
                raise SysMLCompileError('Action assignment must target an initialized state attribute.', action)
            dependencies = []
            code, typ = expression(obj(fields.get('valueExpression')), dependencies, scope)
            if typ != types[target[1]] and not (typ == 'i32' and types[target[1]] == 'f64'):
                raise SysMLCompileError('State assignment has incompatible type.', action)
            return {'kind': 'assign', 'state': by_id[target[1]]['name'], 'expression': code,
                    'sources': [lower(by_id[d]) for d in dependencies]}
        if action['kind'] == 'IfActionUsage':
            dependencies = []
            code, typ = expression(obj(fields.get('ifArgument')), dependencies, scope)
            if typ != 'bool':
                raise SysMLCompileError('Action condition must be Boolean.', action)
            return {'kind': 'if', 'condition': code, 'sources': [lower(by_id[d]) for d in dependencies],
                    'then': lower_action(obj(fields['thenAction']), scope),
                    'else': lower_action(obj(fields['elseAction']), scope) if fields.get('elseAction') else None}
        children = [m for m in members(action) if m['kind'] != 'Documentation']
        if action['kind'] not in ('PerformActionUsage', 'ActionUsage'):
            raise SysMLCompileError('Unsupported performed action: ' + action['kind'], action)
        links = [m for m in children if m['kind'] == 'SuccessionAsUsage']
        statements = {m['id']: m for m in children if m['kind'] != 'SuccessionAsUsage'}
        if not links:
            if len(statements) != 1:
                raise SysMLCompileError('Use first start; then ... to order multiple actions explicitly.', action)
            return lower_action(next(iter(statements.values())), scope)
        successor = {}
        for link in links:
            ends = link['fields'].get('relatedFeature', [])
            if len(ends) != 2:
                raise SysMLCompileError('Only binary sequential action successions are supported.', link)
            def action_key(ref):
                if ref.get('name') == 'Actions::Action::start':
                    return '@start'
                if ref.get('name') == 'Actions::Action::done':
                    return '@done'
                if ref['id'] not in statements:
                    raise SysMLCompileError('Succession endpoint lies outside this action block.', link)
                return ref['id']
            left, right = map(action_key, ends)
            if left in successor:
                raise SysMLCompileError('Concurrent action branches require explicit scheduling.', link)
            successor[left] = right
        current = successor.get('@start')
        order = []
        seen = set()
        while current is not None and current != '@done':
            if current in seen:
                raise SysMLCompileError('Cyclic action succession requires a loop lowering.', action)
            seen.add(current)
            order.append(lower_action(statements[current], scope))
            current = successor.get(current)
        if seen != set(statements):
            raise SysMLCompileError('All performed statements must belong to a single chain beginning at start.', action)
        return {'kind': 'sequence', 'steps': order}
    for scope, action in actions:
        if not states:
            raise SysMLCompileError('Performed assignments require explicitly initialized state.', action)
        ir['updates'].append(lower_action(action, scope))
    for e in attributes:
        src = lower(e)
        if e['fields'].get('direction') == 'out':
            ir['edges'].append({'src': src, 'dst': {'kind': 'output', 'name': e['name']}})
    if not ir['outputs']:
        raise SysMLCompileError('Executable model requires at least one output.', system)
    try:
        return validate_ir(ir)
    except ValueError as exc:
        raise SysMLCompileError('Generated IR violates the executable contract: ' + str(exc)) from exc


def ir_to_il(ir: dict) -> str:
    """Emit the IL XML consumed by the installed ILTranslator, without a C/LLM round trip."""
    ir = validate_ir(ir)
    from xml.etree import ElementTree as ET
    types = {'f32': 'float', 'f64': 'double', 'i32': 'int', 'bool': 'bool'}
    root = ET.Element('Root', {'xmlns:xsi': 'http://www.w3.org/2001/XMLSchema-instance',
                              'xsi:noNamespaceSchemaLocation': 'http://www.example.org/IRMetadata'})
    name = 'ProgramPage1'
    state = name + '_state'
    config = ET.SubElement(root, 'Config')
    for tag, fields in [('IterationCount', {'Value': '1'}), ('MainInitFunction', {'Name': name+'_Init'}),
                        ('MainExecFunction', {'Name': name}), ('MainCompositeState', {'Name': state})]:
        ET.SubElement(config, tag, fields)
    file = ET.SubElement(root, 'File', {'Name': name})
    ET.SubElement(file, 'Struct', {'Name': state})
    ET.SubElement(file, 'GlobalVariable', {'Name': name+'_instance', 'Type': state})
    init = ET.SubElement(file, 'Function', {'Name': name+'_Init', 'Type': 'FunctionInit', 'ReturnType': 'void'})
    ET.SubElement(init, 'Ref', {'Actor': 'CompositeActor', 'Path': name})
    ET.SubElement(init, 'Input', {'Name': 'self', 'Type': state, 'IsAddress': '1'})
    init_schedule = ET.SubElement(init, 'Schedule')
    state_names = {item['name']: 'sysml_state_' + item['name'] for item in ir.get('states', [])}
    if state_names:
        # ponytail: one system context per generated program, use struct/self fields for reentrant instances.
        reset = ET.SubElement(init_schedule, 'Calculate', {'Name': name+'_reset'})
        ET.SubElement(reset, 'Ref', {'Actor': 'CompositeActor', 'Path': name+'.reset'})
        for item in ir['states']:
            variable = state_names[item['name']]
            ET.SubElement(file, 'GlobalVariable', {'Name': variable, 'Type': types[item['type']], 'DefaultValue': item['initial']})
            ET.SubElement(reset, 'Expression', {'Expression': variable + ' = ' + item['initial']})
    function = ET.SubElement(file, 'Function', {'Name': name, 'Type': 'FunctionExec', 'ReturnType': 'void'})
    ET.SubElement(function, 'Ref', {'Actor': 'CompositeActor', 'Path': name})
    for category, tag in [('inputs', 'Input'), ('outputs', 'Output')]:
        for port in ir[category]:
            ET.SubElement(function, tag, {'Name': port['name'], 'Type': types[port['type']]})
    calc = ET.SubElement(ET.SubElement(function, 'Schedule'), 'Calculate', {'Name': name+'_body'})
    ET.SubElement(calc, 'Ref', {'Actor': 'CompositeActor', 'Path': name+'.body'})
    for category, tag in [('inputs', 'Input'), ('outputs', 'Output')]:
        for port in ir[category]:
            attrs = {'Name': port['name'], 'Type': types[port['type']]}
            if category == 'inputs':
                attrs['Source'] = port['name']
            ET.SubElement(calc, tag, attrs)
    values = {}
    def source(endpoint):
        if endpoint['kind'] == 'input':
            return endpoint['name']
        if endpoint['kind'] == 'state':
            return state_names[endpoint['name']]
        return values[(endpoint['id'], endpoint['port'])]
    node_map = {node['id']: node for node in ir['nodes']}
    incoming_map = {(edge['dst'].get('id'), edge['dst'].get('port')): edge['src'] for edge in ir['edges'] if edge['dst']['kind'] == 'node'}
    def inline(endpoint):
        if endpoint['kind'] != 'node':
            return source(endpoint)
        node = node_map[endpoint['id']]
        replacements = {port['name']: inline(incoming_map[(node['id'],port['name'])]) for port in node['inports']}
        return '(' + re.sub(r'\b[A-Za-z_]\w*\b', lambda m: replacements.get(m[0],m[0]), node['params']['Expression']) + ')'
    def update_expression(code, sources):
        replacements = {'arg'+str(i): inline(ref) for i,ref in enumerate(sources)}
        return re.sub(r'\b[A-Za-z_]\w*\b', lambda m: replacements.get(m[0],m[0]), code)
    # The installed IL parser rejects a compound right operand of * or /.
    # Introduce typed temporaries in the same action block, never across a lazy branch.
    from pycparser import c_ast, c_generator, c_parser
    from app.services.ir_contract import expression_type
    emitter = c_generator.CGenerator()
    numeric_types = {p['name']: types[p['type']] for p in ir['inputs']}
    numeric_types.update({state_names[p['name']]: types[p['type']] for p in ir.get('states', [])})
    temporary_index = 0
    def expression_kind(node):
        reverse_types = {v:k for k,v in types.items()}
        return types[expression_type(emitter.visit(node), {n:reverse_types[t] for n,t in numeric_types.items()})]

    def temporary(prefix, kind):
        nonlocal temporary_index
        local = prefix + str(temporary_index)
        temporary_index += 1
        if local in {p['name'] for p in ir['inputs'] + ir['outputs']}:
            raise SysMLCompileError('Reserved compiler identifier: ' + local)
        # Hoist declarations only. Evaluations stay in their original branch.
        position = next((i for i,e in enumerate(calc) if e.tag not in ('Ref','Input','Output','LocalVariable')),len(calc))
        calc.insert(position, ET.Element('LocalVariable', {'Name':local,'Type':kind}))
        numeric_types[local] = kind
        return local
    def backend_expression(parent, code):
        nonlocal temporary_index
        parsed = c_parser.CParser().parse('typedef int bool; void f(){ double result = (' + code + '); }')
        node = parsed.ext[1].body.block_items[0].init
        def normalize(item, scope):
            if isinstance(item, c_ast.TernaryOp):
                kind = expression_kind(item)
                condition = normalize(item.cond, scope)
                local = temporary('sysml_conditional_', kind)
                yes = ET.SubElement(scope, 'If', {'Condition':emitter.visit(condition)})
                yes_value = normalize(item.iftrue, yes)
                ET.SubElement(yes, 'Expression', {'Expression':local+' = '+emitter.visit(yes_value)})
                no = ET.SubElement(scope, 'Else')
                no_value = normalize(item.iffalse, no)
                ET.SubElement(no, 'Expression', {'Expression':local+' = '+emitter.visit(no_value)})
                return c_ast.ID(local)
            if isinstance(item, c_ast.BinaryOp) and item.op in ('&&','||'):
                left = normalize(item.left, scope)
                local = temporary('sysml_lazy_', 'bool')
                ET.SubElement(scope, 'Expression', {'Expression':local+' = '+emitter.visit(left)})
                branch = ET.SubElement(scope, 'If', {'Condition':local if item.op=='&&' else '!'+local})
                right = normalize(item.right, branch)
                ET.SubElement(branch, 'Expression', {'Expression':local+' = '+emitter.visit(right)})
                return c_ast.ID(local)
            if isinstance(item, c_ast.BinaryOp):
                item.left, item.right = normalize(item.left, scope), normalize(item.right, scope)
                if item.op in ('*', '/') and isinstance(item.right, c_ast.BinaryOp):
                    local = temporary('sysml_arithmetic_', expression_kind(item.right))
                    ET.SubElement(scope, 'Expression', {'Expression': local + ' = ' + emitter.visit(item.right)})
                    item.right = c_ast.ID(local)
            elif isinstance(item, c_ast.UnaryOp):
                item.expr = normalize(item.expr, scope)
            elif isinstance(item, c_ast.Cast):
                item.expr = normalize(item.expr, scope)
            return item
        before = temporary_index
        normalized = normalize(node, parent)
        return emitter.visit(normalized) if temporary_index != before else code

    def emit_update(parent, update):
        if update['kind'] == 'sequence':
            for step in update['steps']:
                emit_update(parent, step)
        elif update['kind'] == 'assign':
            code = update_expression(update['expression'], update['sources'])
            ET.SubElement(parent, 'Expression', {'Expression': state_names[update['state']] + ' = ' + backend_expression(parent, code)})
        else:
            condition = update_expression(update['condition'], update['sources'])
            branch = ET.SubElement(parent, 'If', {'Condition': backend_expression(parent, condition)})
            emit_update(branch, update['then'])
            if update['else']:
                emit_update(ET.SubElement(parent, 'Else'), update['else'])
    for update in ir.get('updates', []):
        emit_update(calc, update)
    for i, node in enumerate(ir['nodes']):
        if node['type'] != 'Expression' or len(node['outports']) != 1:
            raise SysMLCompileError('IL emitter requires scalar Expression nodes.')
        incoming = {edge['dst']['port']: edge['src'] for edge in ir['edges']
                    if edge['dst']['kind'] == 'node' and edge['dst']['id'] == node['id']}
        replacements = {port['name']: source(incoming[port['name']]) for port in node['inports']}
        code = re.sub(r'\b[A-Za-z_]\w*\b', lambda m: replacements.get(m[0], m[0]), node['params']['Expression'])
        local = 'sysml_value_' + str(i)
        if local in {p['name'] for p in ir['inputs']+ir['outputs']}:
            raise SysMLCompileError('Reserved compiler identifier: ' + local)
        output = node['outports'][0]
        ET.SubElement(calc, 'LocalVariable', {'Name': local, 'Type': types[output['type']]})
        numeric_types[local] = types[output['type']]
        ET.SubElement(calc, 'Expression', {'Expression': local + ' = ' + backend_expression(calc, code)})
        values[(node['id'], output['name'])] = local
    for port in ir['outputs']:
        matches = [edge['src'] for edge in ir['edges'] if edge['dst'] == {'kind': 'output', 'name': port['name']}]
        if len(matches) != 1:
            raise SysMLCompileError('Output must have exactly one driver: ' + port['name'])
        ET.SubElement(calc, 'Expression', {'Expression': '*' + port['name'] + ' = ' + source(matches[0])})
    ET.indent(root)
    return ET.tostring(root, encoding='unicode')


def validate_executable_sysml(source: str) -> dict:
    result = validate_sysml(source)
    result['stages'] = {'sysml_generation': bool(result['ok']), 'sysml_to_ir': None, 'ir_to_il': None}
    if result['ok']:
        try:
            ir = compile_sysml(source, validated=result)
        except SysMLCompileError as exc:
            result['ok'] = False
            result['stages']['sysml_to_ir'] = False
            result['issues'].append(dict(exc.issue, stage='sysml_to_ir'))
        else:
            result['stages']['sysml_to_ir'] = True
            result['ir'] = ir
            try:
                ir_to_il(ir)
            except (ValueError, KeyError) as exc:
                result['ok'] = False
                result['stages']['ir_to_il'] = False
                result['issues'].append({'severity': 'error', 'code': 'IR_BACKEND',
                                         'stage': 'ir_to_code', 'message': str(exc)})
            else:
                result['stages']['ir_to_il'] = True
    return result
