"""Revalidate archived development candidates under public toggle obligations.

Never overwrite old scores or count a standalone example as an LLM repair.
"""
import argparse
import hashlib
import json
from pathlib import Path
from unittest.mock import patch

from run_contract_development import ROOT, lower_verified, adapt_verified, compiled, save
from app.services.interface_planner import add_public_behavior_checks
from app.services.sysml_assembly import assemble_sysml, ExpressionSyntaxError
from app.services.sysml_compiler import validate_executable_sysml
from app.services.behavior_contract import check_contract
from app.services.model_repair import periodic_example, periodic_failure
from method_snapshot import runtime_identity


def read(p):return json.loads(p.read_text(encoding='utf-8'))
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def replay(original,output):
    execution=read(original/'execution.json')
    if any(c['split']!='development' for c in execution['cases']):raise ValueError('Development archive only')
    output.mkdir(parents=True,exist_ok=False)
    sources=[*sorted((ROOT/'app/services').glob('*.py')),ROOT/'app/main.py',ROOT/'tools/sysml/SysMLBridge.java',
        *[p for p in (ROOT/'experiment/development/oracles').glob('*') if p.is_file()],
        *[ROOT/'experiment/code'/n for n in ('run_contract_development.py','run_formal_experiment.py','run_controlled_experiment.py','method_snapshot.py')],Path(__file__).resolve()]
    hashes={str(p.relative_to(ROOT)):sha(p) for p in sources}
    for p in sources:
        target=output/'source_snapshot'/p.relative_to(ROOT);target.parent.mkdir(parents=True,exist_ok=True);target.write_bytes(p.read_bytes())
    report=dict(scope='Zero-LLM diagnostic replay and isolated public component examples; old outcomes unchanged; not new generation success.',
        new_llm_calls=0,source_hashes=hashes,original_hashes={str((original/'execution.json').relative_to(ROOT)):sha(original/'execution.json')},
        runtime=runtime_identity(),accepted_model_replays=[],failed_expression_replays=[],isolated_examples=[])
    with patch('app.main._call_llm',side_effect=AssertionError('No LLM during replay')):
        for case in execution['cases']:
            requirement=case['requirement']+'\n'+execution['public_interpretations'][case['id']]
            for repeat in range(1,execution['repetitions']+1):
                folder=original/case['id']/f'repeat_{repeat}'
                contract_path=folder/'contract.json'
                if not contract_path.exists():continue
                report['original_hashes'][str(contract_path.relative_to(ROOT))]=sha(contract_path)
                contract=add_public_behavior_checks(read(contract_path),requirement)
                target=output/case['id']/f'repeat_{repeat}'
                save(target/'derived_public_contract.json',contract)
                model_path=folder/'model.json'
                if model_path.exists():
                    report['original_hashes'][str(model_path.relative_to(ROOT))]=sha(model_path)
                    model=read(model_path);source=assemble_sysml(model['controller'],contract)
                    if source!=model['system_sysml2']:raise ValueError('Replay changed archived model text')
                    checked=validate_executable_sysml(source)
                    if not checked['ok']:raise ValueError(checked['issues'])
                    coverage=check_contract(checked['ir'],contract)
                    save(target/'coverage.json',coverage)
                    report['accepted_model_replays'].append(dict(case=case['id'],repeat=repeat,source_identical=True,
                        public_status=coverage['status'],issues=coverage['issues']))
                    if periodic_failure(coverage,contract):
                        check=periodic_failure(coverage,contract)
                        inputs={p['name'] for p in contract['interfaces'] if p['direction']=='input'}
                        outputs={p['name'] for p in contract['interfaces'] if p['direction']=='output'}
                        if (inputs!={check['delta'],*([check['pause_input']] if 'pause_input' in check else [])}
                                or outputs!={check['output']} or contract.get('required_states')):
                            raise ValueError('Isolated example cannot cover additional interfaces or required states')
                        example=periodic_example(coverage,contract,model['controller'])
                        spec=dict(states=[],updates=[],**example['controller_fragment'])
                        isolated=assemble_sysml(spec,contract);valid=validate_executable_sysml(isolated)
                        if not valid['ok']:raise ValueError(valid['issues'])
                        if check_contract(valid['ir'],contract)['status']=='failed':raise ValueError('Example fails public assertion')
                        save(target/'isolated_example.json',dict(origin='not_an_llm_repair',controller=spec))
                        generated=lower_verified(isolated,contract,valid['ir'],target/'example_lowered')
                        code=adapt_verified(generated,contract,target/'example_lowered')
                        outcome,_=compiled(case,target/'example_verified',code)
                        report['isolated_examples'].append(dict(case=case['id'],repeat=repeat,**outcome))
                    print(case['id'],repeat,coverage['status'],flush=True)
                failure_path=folder/'sysml_generation_failures.json'
                if failure_path.exists():
                    report['original_hashes'][str(failure_path.relative_to(ROOT))]=sha(failure_path)
                    for attempt in read(failure_path)['attempts']:
                        spec=json.loads(attempt['response'])['controller']
                        try:
                            assemble_sysml(spec,contract)
                            outcome=dict(early_expression_rejection=False)
                        except ExpressionSyntaxError as exc:
                            outcome=dict(early_expression_rejection=True,path=exc.path,message=str(exc))
                        report['failed_expression_replays'].append(dict(case=case['id'],repeat=repeat,attempt=attempt['attempt'],**outcome))
                save(output/'report.json',report)
    report['source_unchanged']=all(sha(ROOT/p)==h for p,h in hashes.items())
    report['originals_unchanged']=all(sha(ROOT/p)==h for p,h in report['original_hashes'].items())
    report['complete']=report['source_unchanged'] and report['originals_unchanged']
    save(output/'report.json',report)


if __name__=='__main__':
    parser=argparse.ArgumentParser()
    parser.add_argument('--original',required=True);parser.add_argument('--out',required=True)
    args=parser.parse_args();replay(ROOT/args.original,ROOT/args.out)
