"""Retrospective, conservative supplementary audit of frozen Arduino projects.

Does not generate or modify candidates. A requirement clause without a reviewed
semantic argument remains U; passing test samples alone are not a proof of it.
"""
from __future__ import annotations

import argparse
import ast
import collections
import hashlib
import json
import re
import statistics
import sys
from pathlib import Path

sys.path.insert(0, str(Path.home() / '.cache/llm4mdd-metrics-python'))
from tree_sitter import Language, Parser
import tree_sitter_cpp

PARSER = Parser(Language(tree_sitter_cpp.language()))


def walk(node):
    yield node
    for child in node.named_children:
        yield from walk(child)


def txt(node):
    return node.text.decode('utf-8', errors='replace') if node else ''


class Code:
    """Bounded C++ AST analysis; unresolved dynamic values stay unresolved."""
    def __init__(self, source, mega=False):
        self.source = source
        self.tree = PARSER.parse(source.encode())
        self.nodes = list(walk(self.tree.root_node))
        self.errors = self.tree.root_node.has_error
        self.mega = mega
        self.env = {f'A{i}': 54+i if mega else 14+i for i in range(16 if mega else 6)}
        self.env.update(LOW=0, HIGH=1, INPUT=0, OUTPUT=1, INPUT_PULLUP=2,
                        LED_BUILTIN=13, true=1, false=0)
        self.arrays = {}
        self.functions = {}
        for node in self.nodes:
            if node.type == 'function_definition':
                dec = node.child_by_field_name('declarator')
                fdec = next((n for n in walk(dec) if n.type == 'function_declarator'), None)
                name = txt(fdec.child_by_field_name('declarator')) if fdec else ''
                params = []
                if fdec:
                    for param in fdec.child_by_field_name('parameters').named_children:
                        ident = [n for n in walk(param) if n.type == 'identifier']
                        params.append(txt(ident[-1]) if ident else '')
                self.functions.setdefault(name, []).append((node, params))
        # Only immutable declarations are folded; mutable globals may change.
        pending = []
        for node in self.nodes:
            if node.type == 'preproc_def':
                pending.append((txt(node.child_by_field_name('name')), txt(node.child_by_field_name('value'))))
            if node.type == 'init_declarator':
                parent = node.parent
                if not re.search(r'\b(const|constexpr)\b', txt(parent)):
                    continue
                dec = node.child_by_field_name('declarator')
                val = node.child_by_field_name('value')
                ids = [x for x in walk(dec) if x.type == 'identifier']
                if not ids:
                    continue
                name = txt(ids[0])
                if val and val.type == 'initializer_list':
                    self.arrays[name] = [txt(x) for x in val.named_children]
                else:
                    pending.append((name, txt(val)))
        for _ in range(8):
            change = False
            for name, expr in pending:
                value = self.value(expr, self.env)
                if value is not None and self.env.get(name) != value:
                    self.env[name] = value
                    change = True
            if not change:
                break
        self.calls = []
        self.reached = set()
        self.expand('setup', {}, 0)
        self.expand('loop', {}, 0)

    def value(self, expression, env):
        expression = expression.strip()
        expression = re.sub(r'(?<=\d)[uUlLfF]+\b', '', expression)
        expression = re.sub(r'\((?:u?int(?:8|16|32|64)_t|unsigned\s+\w+|int|byte|long)\)', '', expression)
        match = re.fullmatch(r'(\w+)\[(.+)\]', expression)
        if match:
            vals = self.arrays.get(match[1])
            index = self.value(match[2], env)
            return self.value(vals[index], env) if vals and isinstance(index, int) and 0 <= index < len(vals) else None
        try:
            tree = ast.parse(expression, mode='eval').body
            def ev(n):
                if isinstance(n, ast.Constant) and isinstance(n.value, (int, float)):
                    return n.value
                if isinstance(n, ast.Name):
                    return env[n.id]
                if isinstance(n, ast.UnaryOp):
                    v = ev(n.operand)
                    if isinstance(n.op, ast.USub): return -v
                    if isinstance(n.op, ast.UAdd): return v
                    if isinstance(n.op, ast.Invert): return ~v
                if isinstance(n, ast.BinOp):
                    a, b = ev(n.left), ev(n.right)
                    ops = {ast.Add: lambda: a+b, ast.Sub: lambda: a-b, ast.Mult: lambda: a*b,
                           ast.Div: lambda: int(a/b), ast.FloorDiv: lambda: a//b,
                           ast.Mod: lambda: a%b, ast.LShift: lambda: a<<b,
                           ast.RShift: lambda: a>>b, ast.BitOr: lambda: a|b, ast.BitAnd: lambda: a&b}
                    return ops[type(n.op)]()
                raise ValueError()
            v = ev(tree)
            return v if isinstance(v, (int, float)) else None
        except (ValueError, SyntaxError, KeyError, TypeError, ZeroDivisionError, OverflowError):
            return None

    def expand(self, name, bindings, depth):
        if depth > 16 or len(self.functions.get(name, [])) != 1:
            return
        signature = (name, tuple(sorted((k, str(v)) for k, v in bindings.items())))
        if signature in self.reached:
            return
        self.reached.add(signature)
        node, params = self.functions[name][0]
        env = dict(self.env, **bindings)
        for param in params:
            if param not in bindings:
                env.pop(param, None)
        body = node.child_by_field_name('body')
        for call in walk(body):
            if call.type != 'call_expression':
                continue
            # Do not attribute lambda bodies to the containing function.
            parent = call.parent
            excluded = False
            conditional_preprocessor = False
            while parent and parent != node:
                if parent.type in ('lambda_expression', 'function_definition'):
                    excluded = True
                if parent.type in ('preproc_if', 'preproc_ifdef', 'preproc_elif', 'preproc_else'):
                    conditional_preprocessor = True
                if parent.type == 'if_statement':
                    cond = txt(parent.child_by_field_name('condition')).strip('() ')
                    v = self.value(cond, env)
                    cons = parent.child_by_field_name('consequence')
                    alt = parent.child_by_field_name('alternative')
                    if v is not None:
                        if not v and cons and cons.start_byte <= call.start_byte < cons.end_byte: excluded = True
                        if v and alt and alt.start_byte <= call.start_byte < alt.end_byte: excluded = True
                if parent.type in ('while_statement', 'for_statement'):
                    condition = parent.child_by_field_name('condition')
                    v = self.value(txt(condition).strip('() '), env) if condition else None
                    statement = parent.child_by_field_name('body')
                    if v == 0 and statement and statement.start_byte <= call.start_byte < statement.end_byte:
                        excluded = True
                if parent.type == 'compound_statement':
                    for sibling in parent.named_children:
                        if sibling.start_byte >= call.start_byte:
                            break
                        if sibling.end_byte <= call.start_byte and sibling.type in ('return_statement', 'throw_statement', 'break_statement', 'continue_statement'):
                            excluded = True
                parent = parent.parent
            if excluded:
                continue
            fun = txt(call.child_by_field_name('function'))
            args_node = call.child_by_field_name('arguments')
            args = [txt(x) for x in args_node.named_children] if args_node else []
            vals = [self.value(x, env) for x in args]
            evidence = {'function': fun, 'args': args, 'values': vals,
                        'line': call.start_point.row+1, 'caller': name,
                        'preprocessor_uncertain': conditional_preprocessor,
                        'code': txt(call)[:500]}
            self.calls.append(evidence)
            if fun in self.functions and len(self.functions[fun]) == 1:
                subparams = self.functions[fun][0][1]
                subbindings = {p: v for p, v in zip(subparams, vals) if v is not None}
                self.expand(fun, subbindings, depth+1)


def item(rule, instance, status, evidence, critical=False):
    return {'rule': rule, 'instance': instance, 'status': status,
            'critical': critical, 'evidence': evidence}


def embedded(code, required, baud=None):
    """Check bounded API capabilities, with required PWM omissions retained."""
    result = []
    grouped = collections.defaultdict(list)
    for call in code.calls:
        if call['function'] in ('analogWrite', 'analogRead', 'attachInterrupt', 'pinMode', 'digitalRead', 'digitalWrite', 'tone', 'Serial.begin'):
            minimum = {'analogWrite': 2, 'analogRead': 1, 'attachInterrupt': 3, 'pinMode': 2,
                       'digitalRead': 1, 'digitalWrite': 2, 'tone': 2, 'Serial.begin': 1}
            if len(call['args']) < minimum[call['function']]:
                continue  # A function object/overload need not be the Arduino API.
            arg = call['values'][0] if call['values'] else None
            # Unresolved expressions at distinct source sites are distinct instances.
            instance = str(arg) if arg is not None else f"unresolved@{call['line']}"
            grouped[call['function'], instance].append(call)
    pwm_pins = set(range(2, 14)) | {44, 45, 46} if code.mega else {3, 5, 6, 9, 10, 11}
    for (fun, instance), calls in grouped.items():
        v = calls[0]['values'][0] if calls[0]['values'] else None
        status = 'U'
        rule = {'analogWrite': 'PWM_CAPABILITY', 'analogRead': 'ADC_CHANNEL', 'attachInterrupt': 'IRQ_MAPPING',
                'pinMode': 'GPIO_RANGE', 'digitalRead': 'GPIO_RANGE', 'digitalWrite': 'GPIO_RANGE',
                'tone': 'GPIO_RANGE', 'Serial.begin': 'UART_CONFIGURATION'}[fun]
        if not any(c['preprocessor_uncertain'] for c in calls):
            if rule == 'GPIO_RANGE' and v is not None:
                status = 'P' if isinstance(v, int) and 0 <= v < (70 if code.mega else 20) else 'F'
            elif fun == 'Serial.begin' and v is not None and baud is not None:
                status = 'P' if v == baud else 'F'
            elif fun == 'analogWrite' and v is not None:
                endpoints_only = all(len(c['values']) > 1 and c['values'][1] in (0, 255) for c in calls)
                definite_interior = any(len(c['values']) > 1 and c['values'][1] is not None and c['values'][1] not in (0, 255) for c in calls)
                valid_gpio = 0 <= v < (70 if code.mega else 20)
                status = 'P' if v in pwm_pins or (valid_gpio and endpoints_only) else 'F' if not valid_gpio or definite_interior else 'U'
            elif fun == 'analogRead' and v is not None:
                valid = set(range(16)) | set(range(54, 70)) if code.mega else set(range(6)) | set(range(14, 20))
                status = 'P' if v in valid else 'F'
            elif fun == 'attachInterrupt':
                arg = calls[0]['args'][0] if calls[0]['args'] else ''
                match = re.fullmatch(r'digitalPinToInterrupt\s*\((.+)\)', arg)
                if match:
                    pin = code.value(match[1], code.env)
                    if pin is not None:
                        status = 'P' if pin in ({2,3,18,19,20,21} if code.mega else {2,3}) else 'F'
                elif v is not None:
                    status = 'P' if 0 <= v < (6 if code.mega else 2) else 'F'
        result.append(item(rule, instance, status, calls, status == 'F'))
    for device in required:
        kind = device.get('kind', '')
        resolved = device.get('resolved', {})
        pin_text = str(resolved.get('pin', ''))
        expected = code.value(pin_text[1:] if re.fullmatch(r'D\d+', pin_text) else pin_text, code.env)
        if kind == 'analog_input':
            functions = {'analogRead'}
        elif kind == 'digital_input':
            functions = {'digitalRead'}
        elif kind in ('pwm_output', 'digital_output'):
            functions = {'analogWrite', 'digitalWrite'} if kind == 'digital_output' else {'analogWrite'}
        elif kind == 'tone_output':
            functions = {'tone', 'digitalWrite'}
        else:
            functions = set()
        def physical_pin(call):
            value = call['values'][0] if call['values'] else None
            if call['function'] == 'analogRead' and isinstance(value, int) and 0 <= value < (16 if code.mega else 6):
                return value + (54 if code.mega else 14)
            return value
        matching = [c for c in code.calls if c['function'] in functions and c['values'] and physical_pin(c) is not None and physical_pin(c) == expected and not c['preprocessor_uncertain']]
        status = 'P' if expected is not None and matching else 'U'
        result.append(item('REQUIRED_INTERFACE_BINDING', kind+':'+json.dumps(resolved, sort_keys=True), status,
                           {'device': device, 'calls': matching,
                            'scope': 'Physical input/output API binding only; behavior and acquisition/display library semantics are separate.'}))
        if kind not in ('analog_input', 'digital_input', 'digital_output', 'pwm_output', 'tone_output'):
            result.append(item('DEVICE_INITIALIZATION', kind+':'+json.dumps(resolved, sort_keys=True), 'U',
                               'Required GPIO direction/library initialization and implicit bus setup need further path-aware review.'))
        if device.get('kind') != 'pwm_output':
            continue
        raw = str(device.get('resolved', {}).get('pin', ''))
        match = re.fullmatch(r'D?(\d+)', raw)
        pin = int(match[1]) if match else None
        if pin is None:
            continue
        existing = next((x for x in result if x['rule'] == 'PWM_CAPABILITY' and x['instance'] == str(pin)), None)
        if existing:
            existing['required_device'] = device
            # A required variable PWM output cannot use a non-PWM pin even if only endpoints are emitted.
            if pin not in pwm_pins:
                existing['status'] = 'F'
                existing['critical'] = True
        else:
            unresolved = any(x['rule'] == 'PWM_CAPABILITY' and x['status'] == 'U' for x in result)
            # shortcut: custom register-level PWM/library implementations require manual review.
            status = 'U'
            result.append(item('PWM_CAPABILITY', str(pin), status,
                               {'required_device': device, 'reason': 'Required PWM has no resolved Arduino API binding; custom implementations or omissions need review.'}))
    # Timer conflicts are not inferred from mere library includes.
    servos = [c for c in code.calls if c['function'].endswith('.attach')]
    tones = [c for c in code.calls if c['function'] == 'tone']
    pwm = [c for c in code.calls if c['function'] == 'analogWrite']
    for pin in sorted({c['values'][0] for c in pwm if c['values'] and c['values'][0] is not None}):
        if not tones and not servos:
            continue
        # Exact overlap/lifetime requires more than a call graph, so these remain U.
        result.append(item('TIMER_COMPATIBILITY', f'PWM:{pin}', 'U',
                           {'pwm': [c for c in pwm if c['values'][0] == pin], 'possible_timer_users': tones+servos,
                            'reason': 'Lifetime and library implementation not proven by this bounded analyzer.'}))
    # Deduplicate one rule + resolved physical pin across GPIO APIs.
    merged = {}
    for x in result:
        key = (x['rule'], x['instance'])
        if key not in merged:
            merged[key] = x
        else:
            old = merged[key]
            old['evidence'] = [old['evidence'], x['evidence']]
            old['status'] = 'F' if 'F' in (old['status'], x['status']) else 'U' if 'U' in (old['status'], x['status']) else 'P'
            old['critical'] = old['status'] == 'F'
    return list(merged.values()) or [item('INTERFACE_REVIEW', 'required-interfaces', 'U',
                                      'No supported API binding resolved; required interfaces still need review.')]


def clauses(requirement):
    """Source-text clauses, not test IDs, form a shared retrospective draft."""
    prose = requirement.split('Given circuit (must preserve wiring):')[0]
    if '**Detail Rules**:' in prose:
        prose = prose.split('**Detail Rules**:', 1)[1]
    chunks = re.split(r'[。；;\n]+|(?<=[.!?])\s+(?=[A-Z])', prose)
    result = []
    seen = set()
    for chunk in chunks:
        chunk = chunk.strip().strip('*').strip()
        chunk = re.sub(r'^(?:\d+[.)]\s*|[-•]\s*)', '', chunk).strip()
        if len(chunk) < 8 or chunk.endswith(':') or chunk.endswith('：'):
            continue
        if chunk in seen:
            continue
        seen.add(chunk)
        result.append({'id': f'R{len(result)+1:03}', 'source_quote': chunk,
                       'status': 'U', 'reason': 'Retrospective clause draft; implementation semantics not independently reviewed.'})
    return result or [{'id': 'R001', 'source_quote': prose.strip(), 'status': 'U',
                       'reason': 'Requirement needs manual decomposition.'}]


def counts(items):
    c = collections.Counter(x['status'] for x in items)
    n = sum(c[s] for s in ('P','F','U'))
    return {'N': n, **{s: c[s] for s in ('P','F','U')},
            'confirmed_percent': 100*c['P']/n if n else None,
            'unknown_percent': 100*c['U']/n if n else None,
            'critical_failed': sum(x.get('critical', False) and x['status'] == 'F' for x in items)}


def self_check():
    c = Code('const int LED=9; void f(int p){analogWrite(p,128);} void setup(){} void loop(){f(LED); if(false){analogWrite(4,128);}}')
    r = embedded(c, [])
    assert any(x['rule'] == 'PWM_CAPABILITY' and x['status'] == 'P' and x['instance'] == '9' for x in r)
    assert not any(x['rule'] == 'PWM_CAPABILITY' and x['instance'] == '4' for x in r)
    c = Code('void setup(){} void loop(){analogWrite(4,128); analogWrite(4,1);}')
    r = [x for x in embedded(c, []) if x['rule'] == 'PWM_CAPABILITY']
    assert len(r) == 1 and r[0]['status'] == 'F'
    c = Code('void setup(){} void loop(){analogWrite(4,0); analogWrite(4,255);}')
    assert next(x for x in embedded(c, []) if x['rule'] == 'PWM_CAPABILITY')['status'] == 'P'
    c = Code('void setup(){attachInterrupt(digitalPinToInterrupt(4),0,1);} void loop(){}')
    assert next(x for x in embedded(c, []) if x['rule'] == 'IRQ_MAPPING')['status'] == 'F'
    c = Code('void setup(){} void loop(){analogWrite(dynamicPin,128);}')
    assert next(x for x in embedded(c, []) if x['rule'] == 'PWM_CAPABILITY')['status'] == 'U'
    assert c.value('D3', c.env) is None
    assert Code('void setup(){} void loop(){}', mega=True).value('A15', {'A15':69}) == 69
    assert len(clauses('按下按钮点亮LED；松开按钮熄灭LED。')) == 2
    c = Code('void setup(){} void loop(){return; analogWrite(4,128);}')
    assert not any(x['rule'] == 'PWM_CAPABILITY' for x in embedded(c, []))
    c = Code('void setup(){} void loop(){analogWrite(4,v);}')
    assert next(x for x in embedded(c, []) if x['rule'] == 'PWM_CAPABILITY')['status'] == 'U'
    c = Code('void setup(){} void loop(){tone(2000);}')
    assert not any(x['status'] == 'F' for x in embedded(c, []))
    return {'passed': True, 'cases': 11}


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--bundle', required=True)
    ap.add_argument('--output', required=True)
    args = ap.parse_args()
    out = Path(args.output)
    out.mkdir(parents=True, exist_ok=True)
    checks = self_check()
    bundle = json.loads(Path(args.bundle).read_text(encoding='utf-8'))
    checklist = {cid: {'requirement_sha256': hashlib.sha256(p['requirement'].encode()).hexdigest(),
                       'items': clauses(p['requirement'])} for cid, p in bundle['profiles'].items()}
    (out/'requirement_checklists.draft.json').write_text(json.dumps(checklist, ensure_ascii=False, indent=2), encoding='utf-8')
    analyses = {}
    for index, (key, source) in enumerate(bundle['sources'].items()):
        c = Code(source['source'], mega='mega' in source['meta']['fqbn'])
        config = source['meta']['configuration_resolution']
        devices = list(config.get('devices', []))
        original = source['meta']['requirement']
        # The resolved inventory is partial. Keep every circuit peripheral in the
        # denominator, including devices whose indirect wiring is not resolved.
        if 'Given circuit (must preserve wiring):' in original:
            tail = original.split('Given circuit (must preserve wiring):', 1)[1].lstrip()
            try:
                circuit, _ = json.JSONDecoder().raw_decode(tail)
                for part in circuit.get('parts', []):
                    kind = part.get('type', '')
                    if any(s in kind for s in ('arduino', 'resistor', 'breadboard')):
                        continue
                    devices.append({'kind': kind, 'resolved': {'circuit_part': part.get('id')},
                                    'source': 'original circuit; pin-level library binding not inferred'})
            except (ValueError, TypeError):
                devices.append({'kind': 'unparsed_circuit', 'resolved': {}})
        items = embedded(c, devices, (config.get('serial') or {}).get('baud'))
        # Syntax/preprocessor ambiguity cannot support decisive source-level claims.
        if c.errors:
            for x in items:
                x['status'] = 'U'
                x['parser_error'] = True
        analyses[key] = {'source_sha256': hashlib.sha256(source['source'].encode()).hexdigest(),
                         'parser_has_error': c.errors, 'rules': items, 'rule_counts': counts(items),
                         'memory': source['memory'], 'reachable_calls': len(c.calls)}
        if (index+1) % 200 == 0:
            print(json.dumps({'analyzed_unique': index+1}), flush=True)
    rows = []
    for rec in bundle['records']:
        key = rec.get('execution_key')
        a = analyses.get(key)
        req = checklist[rec['case']]['items']
        # Missing generated artifact is an observable implementation omission.
        reqcounts = counts(req if a else [dict(x, status='F') for x in req])
        rulecounts = a['rule_counts'] if a else counts([item('REQUIRED_IMPLEMENTATION', 'missing-source', 'F', 'No generated artifact', True)])
        rows.append({**rec, 'rule_counts': rulecounts, 'requirement_draft_counts': reqcounts,
                     'memory': a['memory'] if a else None,
                     'source_audit': f'source_audits/{key}.json' if a else None})
    (out/'source_audits').mkdir(exist_ok=True)
    for key, a in analyses.items():
        (out/'source_audits'/f'{key}.json').write_text(json.dumps(a, ensure_ascii=False, indent=2), encoding='utf-8')
    (out/'all_project_metrics.json').write_text(json.dumps(rows, ensure_ascii=False, indent=2), encoding='utf-8')
    summaries = []
    for group in bundle['groups']:
        for dataset in ('all', 'internal', 'PCEval', 'EmbedBench'):
            rr = [r for r in rows if r['group'] == group and (dataset == 'all' or r['dataset'] == dataset)]
            n, compiled, passed = len(rr), sum(r['compile_ok'] for r in rr), sum(r['status'] == 'passed' for r in rr)
            memory = [r['memory'] for r in rr if r['compile_ok'] and r['memory']]
            summaries.append({'group': group, 'dataset': dataset, 'projects': n,
                              'compiled': compiled, 'basic_passed': passed,
                              'compile_percent': 100*compiled/n, 'basic_percent': 100*passed/n,
                              'functional_given_compile_percent': 100*passed/compiled if compiled else None,
                              'compiled_but_basic_failed': compiled-passed,
                              'missing_source_projects': sum(not r.get('execution_key') for r in rr),
                              'compile_failed_with_source': sum(bool(r.get('execution_key')) and not r['compile_ok'] for r in rr),
                              'explicit_hardware_violation_projects': sum(bool(r.get('execution_key')) and r['rule_counts']['critical_failed'] > 0 for r in rr),
                              'macro_rules_percent': statistics.mean(r['rule_counts']['confirmed_percent'] for r in rr),
                              'macro_rules_unknown_percent': statistics.mean(r['rule_counts']['unknown_percent'] for r in rr),
                              'critical_rule_failed_projects': sum(r['rule_counts']['critical_failed'] > 0 for r in rr),
                              'rule_totals': {s: sum(r['rule_counts'][s] for r in rr) for s in ('N','P','F','U')},
                              'requirement_draft_totals': {s: sum(r['requirement_draft_counts'][s] for r in rr) for s in ('N','P','F','U')},
                              'memory_sample_count': len(memory),
                              **{f'mean_{s}': statistics.mean(m[s] for m in memory) if memory else None
                                 for s in ('flash_bytes','sram_static_bytes','flash_percent','sram_static_percent')}})
    result = {'schema': 'supplementary-offline-audit-v1', 'self_check': checks,
              'projects': len(rows), 'unique_sources': len(analyses),
              'semantic_requirement_review_complete': False,
              'semantic_review_blocker': 'Configured model endpoint returned HTTP 403; checklist also needs human semantic review.',
              'summaries': summaries}
    (out/'summary.json').write_text(json.dumps(result, ensure_ascii=False, indent=2), encoding='utf-8')
    (out/'self_check.json').write_text(json.dumps(checks), encoding='utf-8')
    print(json.dumps({'finished': True, 'projects': len(rows), 'unique_sources': len(analyses)}), flush=True)


if __name__ == '__main__':
    main()
