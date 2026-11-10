"""Verify isolated repair examples; never count them as LLM repair successes."""
import argparse
import hashlib
import json
from pathlib import Path
from unittest.mock import patch
from run_contract_development import ROOT, lower_verified, adapt_verified, compiled, save
from method_snapshot import runtime_identity
from app.services.model_repair import periodic_example, periodic_failure
from app.services.sysml_assembly import assemble_sysml
from app.services.sysml_compiler import validate_executable_sysml
from app.services.behavior_contract import check_contract


def read(path):return json.loads(path.read_text(encoding='utf-8'))
def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()


def verify(original,output,case_ids):
    execution=read(original/'execution.json')
    cases=[c for c in execution['cases'] if c['id'] in case_ids]
    if len(cases)!=len(set(case_ids)) or any(c['split']!='development' for c in cases):
        raise ValueError('Select existing development tasks only')
    output.mkdir(parents=True,exist_ok=False)
    sources=[*sorted((ROOT/'app/services').glob('*.py')),ROOT/'app/main.py',ROOT/'tools/sysml/SysMLBridge.java',
        *[p for p in sorted((ROOT/'experiment/development/oracles').glob('*')) if p.is_file()],
        *[ROOT/'experiment/code'/name for name in ('run_contract_development.py','run_formal_experiment.py','run_controlled_experiment.py','method_snapshot.py')],Path(__file__).resolve()]
    hashes={str(p.relative_to(ROOT)):sha(p) for p in sources}
    for p in sources:
        target=output/'source_snapshot'/p.relative_to(ROOT);target.parent.mkdir(parents=True,exist_ok=True);target.write_bytes(p.read_bytes())
    report=dict(scope='Standalone component examples derived from frozen public assertions, not automatic original-model edits or new LLM successes.',
        new_llm_calls=0,source_hashes=hashes,original_hashes={str((original/'execution.json').relative_to(ROOT)):sha(original/'execution.json')},runtime=runtime_identity(),rows=[])
    with patch('app.main._call_llm',side_effect=AssertionError('No LLM in example verification')):
        for case in cases:
            for repeat in range(1,execution['repetitions']+1):
                folder=original/case['id']/f'repeat_{repeat}'
                path=folder/'sysml_generation_failures.json'
                if not path.is_file():continue
                contract=read(folder/'contract.json')
                for p in (path,folder/'contract.json'):report['original_hashes'][str(p.relative_to(ROOT))]=sha(p)
                attempt=next((a for a in read(path)['attempts'] if periodic_failure(a.get('coverage'),contract)),None)
                if attempt is None:continue
                example=periodic_example(attempt['coverage'],contract,json.loads(attempt['response'])['controller'])
                check=periodic_failure(attempt['coverage'],contract)
                inputs={p['name'] for p in contract['interfaces'] if p['direction']=='input'}
                outputs={p['name'] for p in contract['interfaces'] if p['direction']=='output'}
                if (inputs!={check['delta'],*([check['pause_input']] if 'pause_input' in check else [])}
                        or outputs!={check['output']} or contract.get('required_states')):
                    raise ValueError('Standalone example cannot replace additional interfaces or required states')
                target=output/case['id']/f'repeat_{repeat}'
                spec=dict(states=[],updates=[],**example['controller_fragment'])
                source=assemble_sysml(spec,contract)
                save(target/'example.json',dict(origin='isolated_public_repair_example',controller=spec,interface_contract=contract))
                (target/'system.sysml').write_text(source,encoding='utf-8')
                checked=validate_executable_sysml(source)
                if not checked['ok']:raise ValueError(checked['issues'])
                coverage=check_contract(checked['ir'],contract)
                if coverage['status']=='failed':raise ValueError(coverage['issues'])
                generated=lower_verified(source,contract,checked['ir'],target/'lowered')
                code=adapt_verified(generated,contract,target/'lowered')
                result,_=compiled(case,target/'verified',code)
                report['rows'].append(dict(case=case['id'],repeat=repeat,original_attempt=attempt['attempt'],**result))
                save(output/'report.json',report)
                print(case['id'],repeat,result['compile_ok'],result['functional_status'],flush=True)
    report['source_unchanged']=all(sha(ROOT/p)==h for p,h in hashes.items())
    report['originals_unchanged']=all(sha(ROOT/p)==h for p,h in report['original_hashes'].items())
    report['complete']=bool(report['rows']) and report['source_unchanged'] and report['originals_unchanged']
    save(output/'report.json',report)


if __name__=='__main__':
    parser=argparse.ArgumentParser()
    for name in ('original','out'):parser.add_argument('--'+name,required=True)
    parser.add_argument('--case',action='append',required=True)
    args=parser.parse_args();verify(ROOT/args.original,ROOT/args.out,args.case)
