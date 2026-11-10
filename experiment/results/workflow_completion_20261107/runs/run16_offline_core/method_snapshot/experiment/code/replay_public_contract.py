"""Recheck archived development models using public requirements; never regenerate.

New assertions and diagnostics are measurement evidence, not new generation
successes. Original contracts, models, and experiment scores remain untouched.
"""
import argparse
import hashlib
import json
import re
from pathlib import Path
import shutil
import sys

ROOT=Path(__file__).resolve().parents[2]
sys.path.insert(0,str(ROOT))
from app.services.interface_planner import add_public_behavior_checks
from app.services.behavior_contract import validate_sysml_contract, check_contract
from app.services.hardware_adapter import validate_hardware_contract
from app.services.sysml_assembly import assemble_sysml, ExpressionSyntaxError
from app.services.sysml_compiler import compile_sysml


def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
def read(path):return json.loads(path.read_text(encoding='utf-8'))
def save(path,value):path.write_text(json.dumps(value,ensure_ascii=False,indent=2),encoding='utf-8')


def replay(original,destination,case_ids=None):
    original=original.resolve();destination=destination.resolve()
    original.relative_to((ROOT/'experiment/output').resolve())
    destination.relative_to((ROOT/'experiment/analysis').resolve())
    destination.mkdir(parents=True,exist_ok=False)
    execution=read(original/'execution.json')
    if any(c['split']!='development' for c in execution['cases']):raise ValueError('Development artifacts only')
    selected=set(case_ids) if case_ids is not None else {c['id'] for c in execution['cases']}
    if not selected or selected-{c['id'] for c in execution['cases']}:
        raise ValueError('Select nonempty development case IDs present in the original manifest')
    cases=[c for c in execution['cases'] if c['id'] in selected]
    sources=[Path(__file__),ROOT/'tools/sysml/SysMLBridge.java',*sorted((ROOT/'app/services').glob('*.py'))]
    hashes={str(p.relative_to(ROOT)):sha(p) for p in sources}
    for source in sources:
        target=destination/'source_snapshot'/source.relative_to(ROOT);target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(source,target)
    report=dict(original=str(original.relative_to(ROOT)),source_hashes=hashes,original_hashes={str((original/'execution.json').relative_to(ROOT)):sha(original/'execution.json')},
                new_llm_calls=0,selected_cases=sorted(selected),
                scope='Public contract and syntax replay only; no regeneration, no hardware score replacement.',rows=[])
    save(destination/'report.json',report)
    for case in cases:
        requirement=case['requirement']+'\n'+execution['public_interpretations'][case['id']]
        for repeat in range(1,execution['repetitions']+1):
            folder=original/case['id']/f'repeat_{repeat}'
            row=dict(case=case['id'],repeat=repeat)
            inputs=[folder/'contract.json',folder/'results.json']
            model=folder/'model.json';failed=folder/'sysml_generation_failures.json'
            candidate_path=model if model.exists() else failed
            inputs.append(candidate_path)
            for path in inputs:
                if path.exists():report['original_hashes'][str(path.relative_to(ROOT))]=sha(path)
            if not (folder/'contract.json').exists() or not candidate_path.exists():
                row.update(status='not_replayed',reason='No archived contract/model candidate')
                report['rows'].append(row);continue
            target=destination/case['id']/f'repeat_{repeat}';target.mkdir(parents=True)
            contract=add_public_behavior_checks(read(folder/'contract.json'),requirement)
            validate_hardware_contract(contract);validate_sysml_contract(contract)
            save(target/'public_contract.json',contract)
            candidates=[read(model)] if model.exists() else [a['response'] for a in read(failed)['attempts']]
            row['candidates']=[]
            for index,candidate in enumerate(candidates):
                outcome=dict(candidate=index+1)
                try:
                    if isinstance(candidate,str):
                        payload=candidate.strip()
                        fence=re.fullmatch(r'```(?:json)?\s*\n([\s\S]*?)\n```',payload,re.I)
                        candidate=json.loads(fence.group(1) if fence else payload)
                    source=assemble_sysml(candidate['controller'],contract)
                    (target/f'candidate_{index+1}.sysml').write_text(source,encoding='utf-8')
                    ir=compile_sysml(source);coverage=check_contract(ir,contract)
                    outcome.update(status=coverage['status'],coverage=coverage)
                    if 'system_sysml2' in candidate:outcome['same_rendered_model']=source==candidate['system_sysml2']
                except ExpressionSyntaxError as exc:
                    outcome.update(status='expression_rejected',code='EXPRESSION_SYNTAX',message=str(exc))
                except (ValueError,KeyError,TypeError) as exc:
                    outcome.update(status='rejected',message=str(exc))
                row['candidates'].append(outcome)
            report['rows'].append(row);save(destination/'report.json',report)
            print(case['id'],repeat,[r['status'] for r in row['candidates']],flush=True)
    report['source_unchanged']=all(sha(ROOT/name)==digest for name,digest in hashes.items())
    report['originals_unchanged']=all(sha(ROOT/name)==digest for name,digest in report['original_hashes'].items())
    report['complete']=report['source_unchanged'] and report['originals_unchanged'] and len(report['rows'])==len(cases)*execution['repetitions']
    save(destination/'report.json',report)
    if not report['complete']:raise RuntimeError('Replay evidence changed or records incomplete')


if __name__=='__main__':
    parser=argparse.ArgumentParser();parser.add_argument('--original',required=True);parser.add_argument('--out',required=True)
    parser.add_argument('--cases',nargs='+',help='Explicit development IDs; omitted means every original case')
    args=parser.parse_args();replay(ROOT/args.original,ROOT/args.out,args.cases)
