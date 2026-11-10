"""Externally supplied interface/coverage obligations, never inferred from an LLM map.

Static checks detect omissions; only independent behavioral tests establish behavior.
Unknown contracts explicitly remain unverified.
"""
import json
import copy
import math
import re
from app.services.interface_semantics import validate_model_semantics

TYPES = {'i32', 'f32', 'f64', 'bool'}


def validate_contract(contract):
    if not isinstance(contract, dict) or contract.get('version') != 1:
        raise ValueError('Interface contract must have version=1')
    if not isinstance(contract.get('control_period_ms'), (int, float)) or isinstance(contract['control_period_ms'], bool) or not math.isfinite(contract['control_period_ms']) or contract['control_period_ms'] <= 0:
        raise ValueError('Positive finite control_period_ms required')
    ports = contract.get('interfaces')
    if not isinstance(ports, list) or not ports:
        raise ValueError('interfaces must be a non-empty list')
    names = set()
    for index, port in enumerate(ports):
        required = {'name', 'direction', 'type', 'unit', 'meaning', 'sample_period_ms', 'initial', 'hardware', 'device'}
        if not isinstance(port, dict) or required - port.keys():
            missing = sorted(required - port.keys()) if isinstance(port, dict) else sorted(required)
            raise ValueError('Every interface requires ' + ', '.join(sorted(required)) + '; missing fields: ' +
                             ', '.join('interfaces[' + str(index) + '].' + key for key in missing))
        name = port['name']
        if not isinstance(name, str) or not re.fullmatch(r'[A-Za-z][A-Za-z0-9_]*', name) or name in names:
            raise ValueError('Invalid or duplicate interface name: ' + str(name))
        names.add(name)
        validate_model_semantics(port)
        if port['direction'] not in ('input', 'output') or not isinstance(port['type'], str) or port['type'] not in TYPES:
            raise ValueError('Invalid interface direction or type: ' + name)
        for key in ('unit', 'meaning', 'hardware', 'device'):
            if not isinstance(port[key], str) or not port[key].strip():
                actual = type(port[key]).__name__
                hint = (' Keep the typed hardware mapping object in binding; hardware is its human-readable description.'
                        if key == 'hardware' else '')
                raise ValueError(f'Interface {name}.{key}: expected a non-empty string, got {actual}.' + hint)
        period = port['sample_period_ms']
        if not isinstance(period, (int, float)) or isinstance(period, bool) or not math.isfinite(period) or period <= 0:
            raise ValueError('Positive finite sample period required for ' + name)
        initial = port['initial']
        if port['type'] == 'bool':
            valid = isinstance(initial, bool)
        else:
            valid = isinstance(initial, (int, float)) and not isinstance(initial, bool) and math.isfinite(initial)
            if port['type'] == 'i32':
                valid = valid and isinstance(initial, int) and -(2**31) <= initial < 2**31
        if not valid:
            raise ValueError('Initial value has wrong type/range: ' + name)
    requirements = contract.get('requirements')
    if not isinstance(requirements, list) or not requirements:
        raise ValueError('Independent requirement inventory required')
    ids = set()
    for requirement in requirements:
        if (not isinstance(requirement, dict) or not isinstance(requirement.get('id'), str) or not requirement['id']
                or not isinstance(requirement.get('text'), str) or not requirement['text']
                or not isinstance(requirement.get('assertion_ids'), list) or not requirement['assertion_ids']
                or any(not isinstance(i, str) or not i for i in requirement['assertion_ids'])):
            raise ValueError('Every requirement needs id, original text and independent assertion_ids')
        if requirement['id'] in ids:
            raise ValueError('Duplicate requirement id')
        ids.add(requirement['id'])
    outputs = {p['name'] for p in ports if p['direction'] == 'output'}
    inputs = {p['name'] for p in ports if p['direction'] == 'input'}
    dependencies = contract.get('required_dependencies', {})
    dependency_errors = []
    if not isinstance(dependencies, dict):
        dependency_errors.append('required_dependencies must be an object, got ' + repr(dependencies))
    else:
        for output, sources in dependencies.items():
            path = 'required_dependencies.' + str(output)
            if output not in outputs:
                dependency_errors.append(path + ': undeclared output ' + repr(output))
            if not isinstance(sources, list):
                dependency_errors.append(path + ': expected an array of input names, got ' + repr(sources))
            else:
                for index, source in enumerate(sources):
                    if not isinstance(source, str) or source not in inputs:
                        dependency_errors.append(path + '[' + str(index) + ']: undeclared input ' + repr(source))
                invalid = [source for source in sources if isinstance(source, str) and source not in inputs]
                if invalid:
                    dependency_errors.append(path + ': not declared inputs: ' + ', '.join(invalid))
    if dependency_errors:
        raise ValueError('required_dependencies must map declared outputs to declared inputs; ' +
                         '; '.join(dependency_errors) + '; declared outputs: ' + json.dumps(sorted(outputs)) +
                         '; declared inputs: ' + json.dumps(sorted(inputs)) +
                         '. Trace intermediate outputs back to their original sensor/time inputs. Example: displayNumber depends on potAdc and led depends on displayNumber; declare led:[potAdc], retaining its other input dependencies. Use actual model input references; state/output names are not inputs. Do not delete dependencies or replace sensor values with constants. Preserve required behavior.')
    states = contract.get('required_states', [])
    if not isinstance(states, list) or any(not isinstance(s, dict) or set(s) != {'name', 'type', 'initial'}
                                         or not all(isinstance(s[k], str) for k in s) for s in states):
        raise ValueError('required_states must use scalar IR name/type/initial fields')
    checks = contract.get('startup_checks', [])
    output_ports = {p['name']:p for p in ports if p['direction']=='output'}
    if not isinstance(checks,list) or len(checks)>128:
        raise ValueError('startup_checks must be a bounded list')
    for check in checks:
        if (not isinstance(check,dict) or set(check)!={'output','expected','evidence'}
                or not isinstance(check['output'],str) or check['output'] not in output_ports
                or not isinstance(check['evidence'],str) or not check['evidence'].strip()):
            raise ValueError('startup_checks require a declared output, typed expected value and source evidence')
        typ, value = output_ports[check['output']]['type'], check['expected']
        valid = type(value) is bool if typ=='bool' else type(value) in (int,float) and math.isfinite(value)
        if typ=='i32':valid = type(value) is int and -(2**31)<=value<2**31
        if not valid:
            raise ValueError('startup_checks expected value has wrong type/range: '+check['output'])
    pauses = contract.get('pause_checks', [])
    port_map = {p['name']:p for p in ports}
    if not isinstance(pauses,list) or len(pauses)>16:
        raise ValueError('pause_checks must be a bounded list')
    if pauses and (type(contract['control_period_ms']) is not int or not 1<=contract['control_period_ms']<=2147483647):
        raise ValueError('pause_checks require an integer i32 millisecond control period')
    for check in pauses:
        if (not isinstance(check,dict) or set(check)-{'response_tolerance_ms'}!={'input','delta','output','initial_running','expected','evidence'}
                or any(not isinstance(check[k],str) for k in ('input','delta','output'))
                or check['input'] not in inputs or port_map[check['input']]['type']!='bool'
                or check['delta'] not in inputs or port_map[check['delta']]['type']!='i32'
                or port_map[check['delta']]['unit']!='ms'
                or check['output'] not in outputs or port_map[check['output']]['type']!='i32'
                or type(check['initial_running']) is not bool or type(check['expected']) is not int
                or not -(2**31)<=check['expected']<2**31
                or not isinstance(check['evidence'],str) or not check['evidence'].strip()):
            raise ValueError('pause_checks require bool press input, i32 ms delta, i32 output, initial_running, expected and evidence')
        tolerance=check.get('response_tolerance_ms',0)
        if type(tolerance) is not int or not 0<=tolerance<=2147483647:
            raise ValueError('pause_checks response_tolerance_ms must be a nonnegative i32')
    from app.services.periodic_contract import validate_periodic_checks
    validate_periodic_checks(contract)
    from app.services.interface_registry import validate_registry
    validate_registry(contract)
    from app.services.numeric_contract import validate_numeric_checks
    validate_numeric_checks(contract)
    from app.services.source_behavior import validate_source_checks
    validate_source_checks(contract)
    from app.services.ultrasonic import groups as ultrasonic_groups
    ultrasonic_groups(contract)
    from app.services.dht import groups as dht_groups
    dht_groups(contract)
    return contract


def validate_sysml_contract(contract):
    """Check representability before freezing an interface for the SysML path.

    The shared IR/Arduino contract still supports f32 for direct IR; the current
    SysML subset maps Real to f64 and must never silently promise f32 ports.
    """
    validate_contract(contract)
    incompatible = [p['name'] for p in contract['interfaces'] if p['type'] == 'f32']
    if incompatible:
        raise ValueError('SysML interface cannot represent f32 port(s): ' + ', '.join(incompatible) +
                         '. The executable SysML subset maps Real to f64. Plan f64 before freezing the contract, '
                         'or use direct IR for an explicitly required f32 interface; never change a frozen type during model repair.')
    return contract


def check_contract(ir, contract):
    if contract is None:
        return {'status': 'unverified', 'issues': [], 'reason': 'No independent interface/requirement inventory supplied'}
    validate_contract(contract)
    issues = []
    def fail(code, message):
        issues.append({'severity': 'error', 'code': code, 'stage': 'requirements', 'message': message})
    expected = {(p['direction'], p['name']): p['type'] for p in contract['interfaces']}
    actual = {(direction, p['name']): p['type'] for direction, key in [('input', 'inputs'), ('output', 'outputs')] for p in ir[key]}
    if actual != expected:
        fail('INTERFACE_COVERAGE', 'Model interface differs from independent inventory: expected ' + str(expected) + '; got ' + str(actual))
    # Trace actual expression references, not merely connected but unused ports.
    dependencies = {('input', p['name']): {p['name']} for p in ir['inputs']}
    def key(endpoint):
        return (endpoint['kind'], endpoint.get('id', endpoint.get('name')))
    relations = []
    for node in ir['nodes']:
        used = set(re.findall(r'\b[A-Za-z_]\w*\b', node['params']['Expression']))
        sources = [key(e['src']) for e in ir['edges'] if e['dst'].get('id') == node['id'] and e['dst'].get('port') in used]
        relations.append((('node', node['id']), sources))
    def updates(update, guards=()):
        if update['kind'] == 'sequence':
            for step in update['steps']:
                updates(step, guards)
        else:
            expression = update.get('expression', update.get('condition', ''))
            used = set(re.findall(r'\barg(\d+)\b', expression))
            sources = tuple(key(s) for i, s in enumerate(update['sources']) if str(i) in used)
            if update['kind'] == 'assign':
                relations.append((('state', update['state']), guards + sources))
            else:
                updates(update['then'], guards + sources)
                if update.get('else'):
                    updates(update['else'], guards + sources)
    for update in ir.get('updates', []):
        updates(update)
    relations.extend((key(e['dst']), [key(e['src'])]) for e in ir['edges'] if e['dst']['kind'] == 'output')
    changed = True
    while changed:
        changed = False
        for target, sources in relations:
            before = dependencies.setdefault(target, set()).copy()
            for source in sources:
                dependencies[target].update(dependencies.get(source, set()))
            changed |= before != dependencies[target]
    for output, inputs in contract.get('required_dependencies', {}).items():
        missing = set(inputs) - dependencies.get(('output', output), set())
        if missing:
            fail('CONSTANT_OR_MISSING_BEHAVIOR', output + ' no longer depends on required inputs: ' + ', '.join(sorted(missing)))
    states = {s['name']: s for s in ir.get('states', [])}
    for state in contract.get('required_states', []):
        if states.get(state['name']) != state:
            fail('STATE_COVERAGE', 'Missing or changed persistent state: ' + json.dumps(state))
    startup = {'status':'not_requested', 'checked':0,
               'scope':'First model step with all inputs at their contract initial values; not a hardware or full behavior proof.'}
    if contract.get('startup_checks') and not issues:
        from app.services.ir_execution import IRExecution
        stimulus = {p['name']:p['initial'] for p in contract['interfaces'] if p['direction']=='input'}
        try:
            observed = IRExecution(ir).step(stimulus)
            startup.update(status='passed', inputs=stimulus, observed=observed)
            for check in contract['startup_checks']:
                startup['checked'] += 1
                if observed[check['output']] != check['expected']:
                    startup['status'] = 'failed'
                    fail('CONTRACT_STARTUP', 'First model step violates explicit startup assertion: ' +
                         json.dumps(dict(**check, actual=observed[check['output']], inputs=stimulus),ensure_ascii=False))
        except (ValueError, ArithmeticError) as exc:
            startup['status'] = 'execution_error'
            fail('CONTRACT_EXECUTION', str(exc))
    pauses = {'status':'not_requested','checked':0,
              'scope':'Press-edge pause output invariant on synthetic public stimuli; not complete period/resume verification.'}
    if contract.get('pause_checks') and not issues:
        from app.services.ir_execution import IRExecution
        pauses['status'] = 'passed'
        period=int(contract['control_period_ms'])
        def delta(n):return min(2147483647,n*period)
        stimuli=[(pressed,delta(n)) for pressed,n in [(False,0),(True,1),(True,17),(True,1001),(True,10003),
                 (False,7),(False,10007),(True,3),(True,47),(False,1),(False,1009),(True,11),(True,100003)]]
        # Probe active histories too: the initial trace alone can miss a missing
        # output gate when a pulse is already in progress. Model constants choose
        # only STIMULI; the expected paused output always comes from the contract.
        # This contains no task IDs, hidden timelines or expected beep windows.
        from app.services.ir_execution import parsed
        from pycparser import c_ast
        bound=max(period,120000)
        seeds={delta(n) for n in (1,10,100,1000) if delta(n)<=bound}
        def literals(tree):
            if isinstance(tree,c_ast.Constant):
                try:
                    value=float(tree.value.rstrip('fFlL'))
                    if value.is_integer() and period<=value<=bound:
                        seeds.add(int(value))
                except ValueError:
                    pass
            for _,child in tree.children():literals(child)
        def collect(value):
            if isinstance(value,dict):
                for key,child in value.items():
                    if key in ('Expression','expression','condition','initial') and isinstance(child,str):literals(parsed(child))
                    else:collect(child)
            elif isinstance(value,list):
                for child in value:collect(child)
        collect(ir)
        probes=sorted({max(period,min(bound,n+offset)) for n in seeds for offset in (-period,0,period)})[:48]
        try:
            for check in contract['pause_checks']:
                tolerance=check.get('response_tolerance_ms',0)
                prefix=[(False,0)] if check['initial_running'] else [(False,0),(True,period),(False,period)]
                traces=[stimuli]+[prefix+[(False,n),(False,period),(True,period),(True,max(period,tolerance)),(False,period)] for n in probes]
                template=IRExecution(ir)
                for trace in traces:
                    engine=copy.deepcopy(template)
                    running,previous,paused_for=check['initial_running'],False,tolerance
                    for step,(pressed,dt) in enumerate(trace):
                        stimulus={p['name']:p['initial'] for p in contract['interfaces'] if p['direction']=='input'}
                        stimulus.update({check['input']:pressed,check['delta']:dt})
                        if not running:paused_for+=dt
                        if pressed and not previous:
                            running=not running
                            if not running:paused_for=0
                        previous=pressed
                        observed=engine.step(stimulus)
                        if not running and paused_for>=tolerance:
                            pauses['checked']+=1
                            if observed[check['output']]!=check['expected']:
                                pauses['status']='failed'
                                fail('CONTRACT_PAUSE','Press-edge pause invariant violated: '+json.dumps(
                                    dict(**check,step=step,actual=observed[check['output']],inputs=stimulus,
                                         paused_for_ms=paused_for,trace=trace[:step+1]),ensure_ascii=False))
                                break
                    if pauses['status']=='failed':break
                if pauses['status']=='failed':break
        except (ValueError, ArithmeticError) as exc:
            pauses['status'] = 'execution_error'
            fail('CONTRACT_EXECUTION', str(exc))
    from app.services.periodic_contract import check_periodic
    periodic,periodic_issues=check_periodic(ir,contract) if not issues else ({'status':'not_run','checked':0},[])
    issues.extend(periodic_issues)
    from app.services.numeric_contract import check_numeric
    numeric,numeric_issues=check_numeric(ir,contract) if not issues else ({'status':'not_run','checked':0},[])
    issues.extend(numeric_issues)
    from app.services.source_behavior import check_source_behavior
    source_behavior, source_issues = check_source_behavior(ir, contract) if not issues else ({'status':'not_run','checked':0}, [])
    issues.extend(source_issues)
    return {'status': 'failed' if issues else 'structural_pass', 'issues': issues,
            'functional_status': 'unverified',
            'startup':startup,
            'pause':pauses,
            'periodic':periodic,
            'numeric':numeric,
            'source_behavior':source_behavior,
            'scope': 'Interface, state/dependency coverage and requested public behavior assertions only. Full behavior requires independent tests.'}
