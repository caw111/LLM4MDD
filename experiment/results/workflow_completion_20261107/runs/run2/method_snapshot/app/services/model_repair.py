"""Local repair evidence and typed examples from frozen public obligations only.

These are feedback, never automatic candidate replacement or validation bypass.
"""
import json
import re


def periodic_failure(coverage, contract):
    if not isinstance(coverage, dict) or not isinstance(contract, dict):
        return None
    periodic = coverage.get('periodic', {})
    if periodic.get('status') != 'failed':
        return None
    check = periodic.get('counterexample', {}).get('check')
    if not isinstance(check, dict) or check not in contract.get('periodic_checks', []):
        return None
    return check


def obligation_keys(coverage, contract):
    check = periodic_failure(coverage, contract)
    # Preserve the exact frozen obligation. A moved counterexample is not an
    # identical error, but it still does not satisfy that same obligation.
    keys=[json.dumps(dict(code='CONTRACT_PERIODIC', check=check), sort_keys=True)] if check else []
    if isinstance(coverage,dict) and isinstance(contract,dict):
        numeric=coverage.get('numeric',{})
        assertion=numeric.get('counterexample',{}).get('check')
        if numeric.get('status')=='failed' and isinstance(assertion,dict) and assertion in contract.get('numeric_checks',[]):
            keys.append(json.dumps(dict(code='CONTRACT_NUMERIC',check=assertion),sort_keys=True))
        source=coverage.get('source_behavior',{})
        assertion=source.get('counterexample',{}).get('check')
        if source.get('status')=='failed' and isinstance(assertion,dict) and assertion in contract.get('source_behavior_checks',[]):
            keys.append(json.dumps(dict(code='SOURCE_BEHAVIOR',check=assertion),sort_keys=True))
    return keys


def update_locations(controller):
    """Conservative advisory for adjacent update/guard pairs, not a lint gate."""
    found = []
    def walk(block, pointer):
        if not isinstance(block, list):
            return
        for index, statement in enumerate(block):
            if not isinstance(statement, dict):
                continue
            here = pointer+'/'+str(index)
            target, value = statement.get('assign'), statement.get('value')
            following = block[index+1] if index+1 < len(block) else None
            if isinstance(target, str) and isinstance(value, str) and isinstance(following, dict):
                guard = following.get('if')
                normalized = re.sub(r'\s+', '', value)
                if (isinstance(guard, str) and re.fullmatch(re.escape(target)+r'[+-](?:[A-Za-z][A-Za-z0-9_]*|\d+(?:\.\d+)?)', normalized)
                        and re.search(r'(?<![A-Za-z0-9_])'+re.escape(normalized)+r'(?![A-Za-z0-9_])', re.sub(r'\s+', '', guard))):
                    found.append(dict(assignment=here, guard=pointer+'/'+str(index+1)+'/if',
                        state=target, assigned_expression=value, guard_expression=guard,
                        advice='The assignment has already committed. If the guard tests the UPDATED state, '
                               'read the state directly instead of applying the same delta again. '
                               'A deliberately required look-ahead may be valid; this is advisory, not a rejection rule.'))
            for branch in ('then', 'else'):
                walk(statement.get(branch), here+'/'+branch)
    if isinstance(controller, dict):
        walk(controller.get('updates'), '/controller/updates')
    return found[:16]


def periodic_example(coverage, contract, controller):
    check = periodic_failure(coverage, contract)
    if check is None:
        return None
    return periodic_component_example(check, contract, controller)


def periodic_component_example(check, contract, controller):
    """A suggested fragment for a quoted frozen obligation, never candidate replacement."""
    if check not in contract.get('periodic_checks', []):
        return None
    ports = {p['name']: p for p in contract['interfaces']}
    clock = ports[check['delta']]
    if clock.get('binding', {}).get('kind') != 'elapsed_ms':
        return None
    # Names must not alias existing declarations, component exports or references.
    existing = json.dumps([controller, contract['interfaces']], ensure_ascii=False)
    def fresh(stem):
        for index in range(1000):
            name = stem+str(index)
            if name not in existing:
                return name
        raise ValueError('Cannot allocate a fresh repair component example name')
    pulse = fresh('repairPulse')
    components = []
    enabled = 'true'
    if 'pause_input' in check:
        toggle = fresh('repairRun')
        components.append(dict(kind='edge_toggle',name=toggle,input=check['pause_input'],edge='rising',
            initial=check['initial_running'],previous_input=ports[check['pause_input']]['initial']))
        enabled = 'component_'+toggle+'_value'
    components.append(dict(kind='periodic_pulse',name=pulse,delta=check['delta'],enabled=enabled,
        period_ms=check['period_ms'],pulse_ms=check['pulse_ms'],first_pulse=check['first_pulse'],
        pause=check.get('pause_policy','preserve')))
    return dict(target_output=check['output'], evidence=check['evidence'],
        controller_fragment=dict(components=components,outputs={check['output']:{'if':'component_'+pulse+'_value',
            'then':check['active_value'],'else':check['inactive_value']}}),
        instructions='This is a partial controller fragment for the cited periodic obligation, not a full replacement model. '
        'Use its exact frozen input/output names, values and timing parameters. Replace the faulty timer implementation '
        'for this output with these components when it has exactly these required semantics; do not keep a second timer '
        'that overrides the same behavior. Preserve every other required output, dependency, state, guard and requirement mapping. '
        'If other requirements add behavior not expressed here, integrate it explicitly or retain a correct full state machine. '
        'Return the complete controller and req_test_map; all parser, IR, contract and later functional checks still apply.')
