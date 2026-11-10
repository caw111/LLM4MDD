"""Run the pinned, unmodified SysML v2 reference validator and export its AST."""
from __future__ import annotations
import hashlib
import json
import os
import re
from pathlib import Path
import shutil
import subprocess
import tempfile
from app.services.failure_analysis import priority, diagnostic_fingerprint, classify_issue
from app.services.llm_json import json_prompt, check_completion

ROOT = Path(__file__).resolve().parents[2]
RUNTIME = ROOT / '.tools' / 'sysml'
VERSION = '2026-08 / 0.62.0'

class SysMLToolError(RuntimeError):
    """Installation or process failure; regenerating a model cannot fix this."""


def _normalize_clock_aliases(controller, contract):
    """Map legacy prompt-example clock names to the sole frozen elapsed input."""
    if not isinstance(controller, dict) or not isinstance(contract, dict):
        return []
    clocks = [p['name'] for p in contract.get('interfaces', [])
              if p.get('direction') == 'input' and p.get('binding', {}).get('kind') == 'elapsed_ms']
    if len(clocks) != 1:
        return []
    actual = clocks[0]
    aliases = {'elapsedMilliseconds', 'elapsed', 'deltaMs'}
    changed = []

    def visit(value, key=None):
        if isinstance(value, dict):
            for child_key, child in value.items():
                value[child_key] = visit(child, child_key)
            return value
        if isinstance(value, list):
            return [visit(child, key) for child in value]
        if isinstance(value, str) and key not in {'name', 'requirement_id', 'test_point_id'}:
            updated = re.sub(r'\b(?:' + '|'.join(map(re.escape, sorted(aliases, key=len, reverse=True))) + r')\b',
                             actual, value)
            if updated != value:
                changed.append(f'clock_alias:{value}->{updated}')
            return updated
        return value

    visit(controller)
    return changed


def validate_sysml(text: str, *, timeout: int = 90) -> dict:
    if not isinstance(text, str) or not text.strip():
        return {'ok': False, 'issues': [{'severity': 'error', 'code': 'EMPTY_MODEL',
                'message': 'SysML model must be a non-empty string.'}], 'elements': []}
    jar = RUNTIME / 'kernel/sysml/jupyter-sysml-kernel-0.62.0-all.jar'
    library = RUNTIME / 'kernel/sysml/sysml.library'
    source = ROOT / 'tools/sysml/SysMLBridge.java'
    local_java = sorted((RUNTIME / 'java').glob('*/bin/java.exe'))
    java = os.environ.get('SYSML_JAVA') or (str(local_java[0]) if local_java else shutil.which('java'))
    if not jar.is_file() or not library.is_dir() or not java:
        raise SysMLToolError('SysML runtime missing. Run tools/sysml/install.ps1 first.')
    classes = RUNTIME / 'classes'
    classes.mkdir(parents=True, exist_ok=True)
    stamp = classes / 'source.sha256'
    digest = hashlib.sha256(source.read_bytes()).hexdigest()
    try:
        if not (classes / 'SysMLBridge.class').is_file() or not stamp.exists() or stamp.read_text() != digest:
            javac = str(Path(java).with_name('javac.exe' if os.name == 'nt' else 'javac'))
            proc = subprocess.run([javac, '-encoding', 'UTF-8', '-cp', str(jar), '-d', str(classes), str(source)],
                                  capture_output=True, text=True, encoding='utf-8', errors='replace', timeout=timeout)
            if proc.returncode:
                raise SysMLToolError('Bridge compilation failed: ' + proc.stderr[-4000:])
            stamp.write_text(digest)
        with tempfile.TemporaryDirectory(prefix='sysml-') as folder:
            model = Path(folder) / 'model.sysml'
            result = Path(folder) / 'result.json'
            model.write_text(text, encoding='utf-8')
            proc = subprocess.run([java, '-Dfile.encoding=UTF-8', '-cp', os.pathsep.join([str(classes), str(jar)]),
                                   'SysMLBridge', library.resolve().as_posix(), str(model), str(result)],
                                  capture_output=True, text=True, encoding='utf-8', errors='replace', timeout=timeout)
            if proc.returncode or not result.is_file():
                raise SysMLToolError('SysML validator failed: ' + proc.stderr[-4000:])
            data = json.loads(result.read_text(encoding='utf-8'))
            data['validator_version'] = VERSION
            return data
    except (OSError, subprocess.TimeoutExpired, json.JSONDecodeError) as exc:
        raise SysMLToolError(str(exc)) from exc

class SysMLValidationError(ValueError):
    def __init__(self, attempts):
        self.attempts = attempts
        super().__init__('SysML generation did not pass validation within the repair budget.')


def sysml_bundle_prompt(requirement: str, *, contract=None, complete_constraints=True) -> str:
    if contract is not None and contract.get('configuration_resolution'):
        resolution = contract['configuration_resolution']
        before, match, after = requirement.rpartition(resolution['original_requirement'])
        requirement = (before + resolution['effective_requirement'] + after if match else
                       requirement + '\nAuthoritative resolved requirements:\n' + resolution['effective_requirement'])
    template = 'contract_controller.prompt.md' if contract is not None else 'system_sysml2.prompt.md'
    rules = (ROOT / 'tools/prompts' / template).read_text(encoding='utf-8')
    if contract is not None:
        from app.services.behavior_contract import validate_sysml_contract
        validate_sysml_contract(contract)
        bindings = {p.get('binding', {}).get('kind') for p in contract['interfaces']}
        selected = 'timer' if 'elapsed_ms' in bindings else ('sample' if any(p['direction']=='input' for p in contract['interfaces']) else 'constant')
        if selected == 'sample' and 'analog_input' in bindings and re.search(r'奇偶|偶数|奇数|\beven\b|\bodd\b', requirement, re.I):
            selected = 'numeric_parity'
        if selected == 'timer' and 'tone_output' in bindings and not any(p['direction']=='input' and p.get('binding',{}).get('kind')!='elapsed_ms' for p in contract['interfaces']):
            selected = 'periodic_pulse'
        if selected == 'timer' and 'tone_output' in bindings and 'digital_input' in bindings and re.search(r'暂停|恢复|pause|resume', requirement, re.I):
            selected = 'paused_pulse'
        if selected == 'timer' and 'digital_input' in bindings and re.search(r'按下沿|启停|开始/停止|start/stop|edge', requirement, re.I):
            selected = 'edge_timer'
        rules = re.sub(r'<!-- example:(\w+) -->([\s\S]*?)<!-- /example -->',
                       lambda m: m[2] if m[1]==selected else '', rules)
    rules = rules.replace('<<USER_REQUIREMENTS>>', requirement)
    if complete_constraints:
        from app.services.sysml_assembly import assemble_sysml
        fixture = json.loads((ROOT / 'tools/examples/sysml_prompt_example.json').read_text(encoding='utf-8'))
        if contract is not None and selected in ('periodic_pulse', 'paused_pulse'):
            # Show the already-validated timing pattern as a complete response,
            # so an unrelated counter fixture does not displace component declarations.
            block = re.search(r'<!-- example:' + selected + r' -->([\s\S]*?)<!-- /example -->',
                              (ROOT / 'tools/prompts/contract_controller.prompt.md').read_text(encoding='utf-8'))[1]
            behavior = json.JSONDecoder().raw_decode(block[block.index('{"states"'):])[0]
            ports = [dict(name='deltaMs', direction='input', type='i32', unit='ms', initial=0,
                          binding={'kind':'elapsed_ms'}),
                     dict(name='frequency', direction='output', type='i32', unit='Hz', initial=0,
                          binding={'kind':'tone_output','pin':6,'invalid_policy':'silence'})]
            if selected == 'paused_pulse':
                ports.insert(0, dict(name='pressed', direction='input', type='bool', unit='1', initial=False,
                                    binding={'kind':'digital_input','pin':2,'pullup':True,'active_high':False}))
            for port in ports:
                port.update(sample_period_ms=10, meaning=port['name'], hardware='Illustrative fixture', device=port['binding']['kind'])
            requirement = dict(id='ExampleR1', text='Execute the declared pulse period, duration, initial mode and pause policy.',
                               assertion_ids=['example_pulse'])
            fixture = dict(contract=dict(version=1,control_period_ms=10,interfaces=ports,requirements=[requirement]),
                           controller=behavior,req_test_map={'requirements_to_testpoints':[dict(
                               requirement_id='ExampleR1',requirement_text=requirement['text'],test_point_id='ExampleT1',
                               test_method='trace',stimulus='Elapsed deltas and press edges',oracle='Declared pulse/initial/pause policy')]})
            _normalize_clock_aliases(fixture['controller'], fixture['contract'])
        source = assemble_sysml(fixture['controller'], fixture['contract'])
        response = {'controller': fixture['controller']} if contract is not None else {'system_sysml2': source}
        response['req_test_map'] = fixture['req_test_map']
        rules += '\n' + (ROOT / 'tools/prompts/sysml_executable_constraints.prompt.md').read_text(encoding='utf-8')
        rules += ('\nComplete illustrative response example (its ExampleR IDs, names and values are fixture-only):\n' +
                  json.dumps(response, ensure_ascii=False, indent=2) +
                  '\nFor the actual task use ONLY its frozen port names and requirement IDs/text, and its actual behavior. '
                  'The fixture is a syntax/order/type demonstration; do not copy its interface or invent reset/count behavior.\n')
        if contract is not None:
            rules += ('\nAssembler-produced SysML for that same example (reference only; your response stays controller JSON):\n' +
                      source + '\nReturn only the complete controller/req_test_map JSON for the ACTUAL requirements.\n')
    return rules


def repair_guidance(issues, *, contract_assembly=False) -> str:
    """Translate known diagnostics into corrections without relaxing validation."""
    text = ' '.join(str(issue.get('code', '')) + ' ' + str(issue.get('message', '')) for issue in issues).lower()
    hints = []
    if 'conditional guard' in text or 'conditional requires a boolean condition' in text:
        hints.append('Repair the condition at the reported expression, including nested conditionals. '
                     'A Real or Integer measurement cannot be used directly as a Boolean guard. '
                     'For paired measurement/validity inputs, use the bool validity input for availability; '
                     'for thresholds use the explicit comparison from the requirement. Example when integer rounding '
                     'and a 10000 blank sentinel are required: {"if":"distanceValid","then":{"floor":"distanceCm"},"else":10000}. '
                     'Use your frozen interface names and invalid-output policy. Do not invent a numeric != 0 test '
                     'for validity, replace the sensor with a constant, or change the frozen interface types.')
    if 'contract_numeric' in text:
        hints.append('CONTRACT_NUMERIC is an executed counterexample of the frozen numeric obligation. '
                     'For counter_edges, preserve both wrap endpoints, initial state, normalized buttons and reset priority. '
                     'Use explicit boundary branches: decrement at the minimum selects the maximum; increment at the maximum '
                     'selects the minimum. Do not apply a nonnegative remainder formula to a negative intermediate. '
                     'For mapped_parity, compute the required mapped integer FIRST and test that integer for even/odd; '
                     'testing the raw ADC is different. Reuse a named calculation for the mapped count. '
                     'Keep each actuator connected to its own sampled brightness input. Preserve rounding, scales, units, '
                     'response bounds and every other requirement. Never change or remove numeric_checks to pass validation.')
    if 'contract_periodic' in text:
        hints.append('CONTRACT_PERIODIC is an executed timing counterexample from explicit public requirements. '
                     'Preserve the frozen period, pulse duration, first pulse, tolerance and pause policy. '
                     'The period is start-to-start: advance phase during BOTH the active pulse and silence, '
                     'freeze only when required by pause semantics, and resume the remaining phase/pulse. '
                     'Updates execute immediately in order: after assigning remaining := remaining - delta, '
                     'test remaining <= 0, not remaining - delta <= 0. Do not reset the full period at pulse end. '
                     'Use matching periodic_pulse and edge_toggle components or repair the complete state machine; '
                     'never remove the assertion, time input, or required behavior.')
    if 'expression_syntax' in text:
        hints.append('Fix the exact expression quoted by EXPRESSION_SYNTAX before changing action ordering. '
                     'Follow its /controller JSON pointer. For a conditional VALUE use '
                     '{"if":"remaining > 0","then":"remaining - delta","else":0}, rendered as '
                     '(if remaining > 0 ? remaining - delta else 0); never use if ... then ... else as an expression string. '
                     'Do not use C % in SysML; distinguish ordinary integer arithmetic from temporal phase logic. '
                     'A structured expression uses one supported operator key with its required operands: '
                     '{"and":["pressed",{"not":"previous"}]} or {">=":["elapsed","3000"]}. '
                     'and/or/xor need 2..256 operands; comparisons and arithmetic need exactly two; not takes one expression. '
                     'Keep expression objects distinct from if statements whose branches are action lists. '
                     'and(a,b), or(a,b), xor(a,b) are not SysML function calls; use (a and b), (a or b), (a xor b). '
                     'For nested operands, preserve parentheses and every operand: '
                     'and(not(paused), remaining > 0) becomes ((not paused) and (remaining > 0)). '
                     'Do not change state semantics or drop conditions to remove a parser error.')
    if 'remainder' in text:
        from app.services.sysml_assembly import REMAINDER_GUIDANCE
        hints.append(REMAINDER_GUIDANCE)
    if 'contract_startup' in text or 'contract_pause' in text:
        hints.append('The diagnostic is an executed counterexample from the frozen public contract, not a syntax error. '
                     'Preserve the exact contract and source requirements. Initial silence must survive the FIRST model step '
                     'with delta=0; phase<duration alone incorrectly sounds at phase=0. A normalized pressed input uses a '
                     'RISING edge; update its memory after detecting the edge. Paused output must be silent regardless '
                     'of remaining pulse duration. Use edge_toggle plus periodic_pulse when their explicit parameters '
                     'match the original requirement, or repair the full state machine. Do not remove assertions or inputs.')
    if 'component' in text or any(word in text for word in ('period_ms', 'pulse_ms', 'first_pulse', 'edge_toggle')):
        hints.append('For components, use the exact documented fields and original requirement parameters. '
                     'List edge_toggle BEFORE a periodic_pulse that consumes its component_NAME_value. '
                     'Only that Boolean export is readable; never redeclare or assign component_ storage. '
                     'A normalized pressed input uses rising for a press edge. Use the frozen i32 elapsed_ms '
                     'input for delta; do not substitute a sensor, constant or absolute clock. '
                     'Keep first_pulse and preserve/reset policy explicit. If the component cannot express '
                     'the required behavior, implement the full behavior using ordinary states and updates.')
    if contract_assembly and 'duplicate declared name in model specification' in text:
        hints.append('The duplicate is a controller declaration, not a missing JSON wrapper. '
                     'Inputs and outputs are already declared by the frozen interface contract. '
                     'Do not declare their names again in controller.states, calculations, parts or components. '
                     'Keep the EXACT contract output key in controller.outputs. '
                     'For a stateless mapping use {"states":[],"updates":[],"outputs":{"lamp":"pressed"}} '
                     'with the actual contract names; no lamp state or calculation is needed. '
                     'If an internal calculation or persistent state is required, rename ONLY that internal declaration '
                     'and ALL references to it, then expose the value through the unchanged output key. '
                     'For example {"states":[{"name":"remembered","type":"bool","initial":false}],'
                     '"updates":[{"assign":"remembered","value":"pressed"}],"outputs":{"lamp":"remembered"}}. '
                     'Preserve the original initializer, expression, guards and update order when renaming; '
                     'do not rename external ports, delete required state or replace inputs with constants.')
    if 'duplicate scalar declaration' in text:
        hints.append('The diagnostic names the exact duplicated scalar. Keep ONE output declaration, '
                     'with its required expression, e.g. out attribute lamp : Boolean = lit;. '
                     'Do not also declare out attribute lamp : Boolean = false;. '
                     'If the output needs persistent memory, use a DISTINCT internal attribute '
                     'lit : Boolean := false; and expose lamp = lit. A toggled state must be Boolean, '
                     'not Integer. Preserve required state and behavior when merging declarations.')
    if any(word in text for word in ('succession', 'chain', 'order multiple', 'connectorrelated', 'multiple unsequenced')):
        hints.append('Put persistent updates in one perform action tick. For multiple statements, '
                     'use first start; then assign ...; then assign ...; and connect every statement. '
                     'Do not invent an order for concurrent behavior: preserve the required schedule.')
        hints.append('Ordering is local to each block. A then if in the outer action does NOT order two assignments '
                     'inside its branch. Legal branch: if reset { first start; then assign count := 0; '
                     'then assign doubled := 0; } else { first start; then assign count := count + 1; '
                     'then assign doubled := count * 2; }. Keep each first start inside its own branch braces.')
    if 'algebraic cycle' in text:
        hints.append('An output cannot drive itself: out attribute result : Real = result; is a cycle. '
                     'Use the genuine sampled input or preceding computation required by the specification. '
                     'Introduce persistent state only for required memory. A genuinely constant requirement such as '
                     'always-on LED permits out attribute ledOn : Boolean = true;, but sensor-dependent behavior does not.')
    if any(word in text for word in ('initial', 'assignment', 'stateusagetype')):
        hints.append('First distinguish a stateless output from persistent memory. '
                     'A direct sampled mapping must be written as: in attribute pressed : Boolean; '
                     'out attribute lamp : Boolean = pressed;. It needs NO perform action and NO state. '
                     'If the rejected code assigns lamp := pressed, replace the output declaration with '
                     'the value expression and remove that assignment; do not add a dummy counter to satisfy the diagnostic. '
                     'For behavior that genuinely needs memory, declare an internal initialized attribute such as '
                     'attribute remembered : Boolean := false; update remembered in perform action tick; '
                     'expose out attribute lamp : Boolean = remembered;. '
                     'Do not assign to a derived output or an external input. Preserve all required stateful behavior.')
    if any(word in text for word in ('type', 'incompatible')):
        hints.append('Use ScalarValues::Boolean, Integer or Real consistently. '
                     'Boolean guards must be Boolean; do not assign Real expressions to Integer states. '
                     'For intentional Real-to-Integer conversion use RealFunctions::floor(value), which rounds down; '
                     'The interface contract is frozen: never change its output types to silence this error. '
                     'For contract assembly, a numeric expression can use {"floor": originalExpression}; '
                     'use it only where the requirement permits downward rounding. For an i32 conditional output '
                     'both branches must be Integer, e.g. {"if":"enabled","then":{"floor":"sample * 200.0 / 1023.0"},"else":0}. '
                     'For f64 outputs preserve fractional values instead. Preserve every input, scale, threshold and branch; '
                     'choose the rounding required by the original task, not by deleting computations.')
    if any(word in text for word in ('bind', 'driver', 'value expression', 'at least one output')):
        hints.append('Give every output a value expression or exactly one compatible binding driver. '
                     'Bind child inputs to root inputs or computed values. Never replace missing behavior with constants.')
        hints.append('Legal binding inside the root part def: in attribute sample : Real; '
                     'part child : Processor; bind child.sample = sample; '
                     'where Processor declares in attribute sample : Real;. '
                     'Use = in bind, not := or an arrow; do not add a second value driver.')
    if 'linking' in text:
        hints.append('Check referenced names and scopes; import ScalarValues::* and declare the referenced '
                     'features. Preserve hardware operations in the adapter through explicit model inputs/outputs.')
    if 'bundle_format' in text:
        field = 'controller' if contract_assembly else 'system_sysml2'
        hints.append('Return one complete JSON object with ' + field + ' and req_test_map.requirements_to_testpoints; '
                     'each mapping needs requirement_id, requirement_text, test_point_id, test_method, stimulus and oracle.')
    if not hints:
        hints.append('Correct the reported syntax or unsupported construct using the executable examples in the '
                     'original prompt. Keep all required behavior, state and output semantics.')
    return '\n'.join('- ' + hint for hint in hints)


def repair_context(source, issues):
    """Numbered local excerpts; the full candidate is always also supplied."""
    lines = source.splitlines()
    selected = set()
    for issue in issues:
        line = issue.get('line')
        if isinstance(line, int) and 1 <= line <= len(lines):
            selected.update(range(max(1, line - 2), min(len(lines), line + 2) + 1))
    return '\n'.join(f'{n:4}: {lines[n - 1]}' for n in sorted(selected)) or '(No source location supplied.)'


def generate_validated_bundle(call, prompt: str, *, max_repairs: int = 2, validator=validate_sysml, contract=None):
    """Only return artifacts from a successful attempt; tool failures do not burn retries."""
    if not isinstance(max_repairs, int) or isinstance(max_repairs, bool) or not 0 <= max_repairs <= 5:
        raise ValueError('max_repairs must be an integer between 0 and 5')
    attempts = []
    if contract is not None:
        from app.services.behavior_contract import validate_sysml_contract
        validate_sysml_contract(contract)
        prompt += '\nFrozen interface and requirement inventory (preserve exactly; traceability is not proof of behavior):\n' + json.dumps(contract, ensure_ascii=False)
        root_inputs = [p for p in contract['interfaces'] if p['direction'] == 'input']
        clock_inputs = [p['name'] for p in root_inputs if p.get('binding', {}).get('kind') == 'elapsed_ms']
        prompt += '\nExact root input names (use these instead of example names): ' + json.dumps([p['name'] for p in root_inputs])
        if clock_inputs:
            prompt += ('\nThe registered elapsed-time input for this contract is exactly ' + json.dumps(clock_inputs[0]) +
                       '. Use that name for the real delta in every timed expression; elapsedMilliseconds, elapsed, '
                       'and deltaMs are only documentation examples unless one is the exact registered name.')
        else:
            prompt += ('\nThis contract has NO elapsed_ms input. Do not use a periodic_pulse component, '
                       'do not use elapsedMilliseconds, elapsed or deltaMs in any expression, and do not invent '
                       'a timing/state dependency absent from the exact root inputs.')
        prompt += '\nExact controller.outputs keys (use these instead of example names): ' + json.dumps([p['name'] for p in contract['interfaces'] if p['direction']=='output'])
        prompt += '\nRequired output expression types and units: ' + json.dumps({p['name']: {'type': p['type'], 'unit': p['unit'], 'kind': p.get('binding', {}).get('kind')} for p in contract['interfaces'] if p['direction']=='output'})
        clock_example = clock_inputs[0] if clock_inputs else 'REGISTERED_CLOCK_INPUT'
        prompt += ('\nFor Boolean operators/comparisons prefer structured objects, e.g. {"and":["pressed",{"not":"previousPressed"}]} '
                   'and {">=":[' + json.dumps(clock_example) + ',1000]}. Conditional values use {"if":condition,"then":value,"else":value}. '
                   'Tone outputs return Integer Hz (0 for silence), never a Boolean pulse. Fixed text outputs return Boolean report gates. '
                   'Infinite repeating behavior must remain repeating; never enumerate a finite list of future pulse windows.')
        from app.services.model_repair import periodic_component_example
        for check in contract.get('periodic_checks', []):
            fragment = periodic_component_example(check, contract, {})
            if fragment:
                prompt += '\nSuggested component fragment for this EXACT quoted public periodic obligation (integrate all other behavior):\n' + json.dumps(fragment, ensure_ascii=False)
        from app.services.interface_semantics import model_boundary_prompt
        prompt += model_boundary_prompt(contract)
        from app.services.interface_registry import registry_prompt
        prompt += registry_prompt(contract)
    current_prompt = prompt
    for index in range(max_repairs + 1):
        model, text = call(json_prompt(current_prompt))
        issues = []
        bundle = None
        result = {}
        normalizations = []
        try:
            check_completion(text)
            # Accept only a complete JSON fence, never extract arbitrary embedded prose.
            payload = text.strip()
            fence = re.fullmatch(r"```(?:json)?\s*\n([\s\S]*?)\n```", payload, re.IGNORECASE)
            bundle = json.loads(fence.group(1) if fence else payload)
            if contract is not None:
                from app.services.sysml_assembly import assemble_sysml, fields, expression
                fields(bundle, ['controller', 'req_test_map'])
                from app.services.expression_normalization import normalize_controller
                bundle['controller'], converted = normalize_controller(bundle['controller'], expression)
                normalizations.extend(converted)
                normalizations.extend(_normalize_clock_aliases(bundle['controller'], contract))
                bundle['system_sysml2'] = assemble_sysml(bundle['controller'], contract)
            elif not isinstance(bundle, dict) or not isinstance(bundle.get('system_sysml2'), str):
                raise ValueError('Return an object containing system_sysml2 as a string.')
            test_map = bundle.get('req_test_map')
            if isinstance(test_map, list):
                # Lossless container migration only: every row and requirement
                # is still validated below, and the raw response is retained.
                test_map = {'requirements_to_testpoints': test_map}
                bundle['req_test_map'] = test_map
                normalizations.append('req_test_map_array_to_object')
            if not isinstance(test_map, dict):
                raise ValueError('req_test_map must be an OBJECT, not an array. Use {"req_test_map":{"requirements_to_testpoints":[{"requirement_id":"R1","requirement_text":"original requirement","test_point_id":"T1","test_method":"trace","stimulus":"input","oracle":"expected output"}]}}. Preserve the controller and all actual requirement IDs/text.')
            mapping = test_map.get('requirements_to_testpoints')
            if not isinstance(mapping, list) or not mapping:
                raise ValueError('req_test_map.requirements_to_testpoints must be a non-empty array.')
            required = {'requirement_id', 'requirement_text', 'test_point_id', 'test_method', 'stimulus', 'oracle'}
            for item in mapping:
                if not isinstance(item, dict) or required - item.keys() or any(item[k] is None or item[k] == '' for k in required):
                    raise ValueError('Every requirement mapping must contain non-empty ' + ', '.join(sorted(required)))
            result = validator(bundle['system_sysml2'])
            issues = result['issues']
            ok = result['ok']
            if ok:
                from app.services.behavior_contract import check_contract
                ir = result.get('ir')
                if contract is not None and ir is None:
                    from app.services.sysml_compiler import compile_sysml
                    ir = compile_sysml(bundle['system_sysml2'], validated=result)
                result['coverage'] = check_contract(ir, contract)
                if contract is not None:
                    mapped = {item['requirement_id'] for item in mapping}
                    missing = {item['id'] for item in contract['requirements']} - mapped
                    if missing:
                        result['coverage']['status'] = 'failed'
                        result['coverage']['issues'].append({'severity': 'error', 'code': 'REQUIREMENT_MAPPING',
                            'stage': 'requirements', 'message': 'Missing independent requirement IDs: ' + ', '.join(sorted(missing))})
                issues.extend(result['coverage']['issues'])
                ok = result['coverage']['status'] != 'failed'
        except (ValueError, TypeError, AttributeError) as exc:
            from app.services.sysml_assembly import ExpressionSyntaxError
            issues = [{'severity': 'error', 'code': getattr(exc, 'code', 'EXPRESSION_SYNTAX' if isinstance(exc,ExpressionSyntaxError) else 'BUNDLE_FORMAT'), 'message': str(exc)}]
            if getattr(exc,'path',None):issues[0]['path']=exc.path
            ok = False
        errors = [i for i in issues if i.get('severity', 'error') == 'error']
        focus = [i for i in errors if priority(i) == min(map(priority, errors))] if errors else issues
        fingerprint = diagnostic_fingerprint(focus)
        repeated = bool(fingerprint and attempts and fingerprint == attempts[-1]['diagnostic_fingerprint'])
        from app.services.model_repair import obligation_keys, update_locations, periodic_example
        failed_obligations = obligation_keys(result.get('coverage'), contract)
        repeated_requirement = bool(failed_obligations and attempts and
                                    failed_obligations == attempts[-1].get('failed_obligation_keys'))
        controller = bundle.get('controller') if isinstance(bundle, dict) else None
        locations = update_locations(controller) if not ok else []
        example = periodic_example(result.get('coverage'), contract, controller) if not ok and contract is not None else None
        source = bundle.get('system_sysml2', '') if isinstance(bundle, dict) else ''
        if not isinstance(source, str):
            source = ''
        attempts.append({'attempt': index + 1, 'model': model, 'response': text, 'issues': issues, 'ok': ok,
                         'completion': getattr(text, 'completion', {}),
                         'normalizations': normalizations,
                         'generation_mode': 'contract_assembly' if contract is not None else 'sysml_text',
                         'rendered_sysml2': source,
                         'stages': result.get('stages', {}), 'focus_issues': focus,
                         'coverage': result.get('coverage', {'status': 'not_run'}),
                         'diagnostic_fingerprint': fingerprint, 'no_progress': repeated,
                         'failed_obligation_keys': failed_obligations, 'repeated_requirement': repeated_requirement,
                         'repair_locations': locations, 'repair_example': example,
                         'failure_stage': classify_issue(focus[0])[0] if focus and not ok else None})
        if ok:
            return model, bundle, {'validator_version': VERSION, 'attempts': attempts,
                                   'coverage': result.get('coverage', {'status': 'unverified'})}
        if any(i.get('code') == 'LLM_REFUSAL' for i in issues):
            break
        current_prompt = (prompt + '\n\nThe previous candidate FAILED validation. Repair it without dropping requirements. '
                          'Never skip validation, delete devices/timing/state, or replace sampled inputs with constants. '
                          'Return the entire corrected JSON bundle.\nPrevious candidate:\n' + text +
                          '\nPrevious complete SysML model:\n' + source +
                          '\nDiagnostics (line/column refer to system_sysml2); fix this priority group first:\n' + json.dumps(focus, ensure_ascii=False) +
                          '\nSource context:\n' + repair_context(source, focus) +
                          '\nFix syntax first, then references, then types, then executable semantics. '
                          'Other diagnostics may be cascades; the full validator runs again after every repair. '
                          f'{len(issues) - len(focus)} secondary diagnostics retained in the audit report.' +
                          '\nTargeted repair guidance:\n' + repair_guidance(focus, contract_assembly=contract is not None))
        if contract is not None:
            current_prompt += ('\nThis path uses contract assembly. Return exactly controller and req_test_map. '
                               'Repair expressions, states or ordered statements in controller; the application '
                               'rebuilds and fully validates system_sysml2. Do not return raw SysML or change the frozen interface. '
                               'Translate any SysML repair example above into the controller schema.')
        if locations:
            current_prompt += '\nSpecific controller locations to inspect (advisory, preserve intentional behavior):\n'+json.dumps(locations, ensure_ascii=False)
        if example:
            current_prompt += '\nTyped repair example using THIS frozen contract, rather than generic port names:\n'+json.dumps(example, ensure_ascii=False)
        if repeated_requirement:
            current_prompt += ('\nSAME REQUIREMENT STILL FAILING: two consecutive candidates violate the same frozen public obligation. '
                               'The counterexample may have moved; this is not a claim that the traces are identical. '
                               'Inspect the named obligation: numeric checks concern mapped-value selection and counter wrap/reset; '
                               'periodic checks concern timer state and immediate update order. Use a matching typed '
                               'component fragment above only when its semantics apply. Do not change the requirement '
                               'or declare success because the failing sample moved.')
        if repeated:
            current_prompt += ('\nNO PROGRESS: the same primary diagnostics occurred in two consecutive candidates. '
                               'Perform a local repair of the cited construct using the legal pattern above. '
                               'Check its enclosing scope and each referenced declaration. Do not repeat the previous edit '
                               'or rewrite unrelated behavior. Keep all interfaces, thresholds and timing requirements.')
    raise SysMLValidationError(attempts)
