"""用归档模型检验新生产来源链，不调用 LLM，不覆盖原实验评分。"""
import argparse
import hashlib
import json
from pathlib import Path
import shutil
from unittest.mock import patch

from run_contract_development import ROOT, lower_verified, adapt_verified, compiled, save
from method_snapshot import runtime_identity


def read(path):return json.loads(path.read_text(encoding='utf-8'))
def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()


def replay(original,destination,case_ids):
    original=original.resolve();destination=destination.resolve()
    original.relative_to(ROOT/'experiment/output');destination.relative_to(ROOT/'experiment/output')
    execution=read(original/'execution.json')
    cases=[c for c in execution['cases'] if c['id'] in case_ids]
    if len(cases)!=len(set(case_ids)) or any(c['split']!='development' for c in cases):
        raise ValueError('Select only development cases from the original manifest')
    destination.mkdir(parents=True,exist_ok=False)
    sources=[Path(__file__),ROOT/'app/main.py',ROOT/'experiment/code/run_contract_development.py',
             ROOT/'experiment/code/run_formal_experiment.py',ROOT/'experiment/code/run_controlled_experiment.py',
             ROOT/'experiment/code/method_snapshot.py',ROOT/'tools/sysml/SysMLBridge.java',
             *sorted((ROOT/'app/services').glob('*.py')),*sorted((ROOT/'experiment/development/oracles').glob('*'))]
    hashes={str(p.relative_to(ROOT)):sha(p) for p in sources if p.is_file()}
    for name in hashes:
        target=destination/'source_snapshot'/name;target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(ROOT/name,target)
    report=dict(scope='Archived first-repeat models through production APIs, target compile and independent host oracle; '
                      'not a new generation success rate or population guarantee.',
                selected_cases=case_ids,new_llm_calls=0,runtime=runtime_identity(),source_hashes=hashes,
                original_hashes={str((original/'execution.json').relative_to(ROOT)):sha(original/'execution.json')},rows=[])
    save(destination/'report.json',report)
    with patch('app.main._call_llm',side_effect=AssertionError('No LLM in artifact replay')):
        for case in cases:
            folder=original/case['id']/'repeat_1'
            paths=[folder/'model.json',folder/'contract.json',folder/'lowered/ir.json',folder/'results.json']
            originals=[r for r in read(paths[-1]) if r['arm']=='workflow']
            if len(originals)!=1 or not originals[0]['compile_ok']:raise ValueError('Replay requires an accepted archived model')
            old_artifact=ROOT/originals[0]['artifact'];paths.append(old_artifact)
            for path in paths:report['original_hashes'][str(path.relative_to(ROOT))]=sha(path)
            model,contract,ir=map(read,paths[:3]);target=destination/case['id']
            generated=lower_verified(model['system_sysml2'],contract,ir,target/'lowered')
            code=adapt_verified(generated,contract,target/'lowered')
            result,_=compiled(case,target/'verified',code)
            report['rows'].append(dict(case=case['id'],same_arduino_text=code==old_artifact.read_text(encoding='utf-8'),
                provenance_status=generated.provenance_status,**result))
            save(destination/'report.json',report)
            print(case['id'],result['compile_ok'],result['functional_status'],flush=True)
    report['source_unchanged']=all(sha(ROOT/name)==value for name,value in hashes.items())
    report['originals_unchanged']=all(sha(ROOT/name)==value for name,value in report['original_hashes'].items())
    report['complete']=len(report['rows'])==len(cases) and report['source_unchanged'] and report['originals_unchanged']
    save(destination/'report.json',report)
    if not report['complete']:raise RuntimeError('Replay incomplete or source/original changed')


if __name__=='__main__':
    parser=argparse.ArgumentParser()
    parser.add_argument('--original',required=True);parser.add_argument('--out',required=True)
    parser.add_argument('--cases',nargs='+',required=True)
    args=parser.parse_args();replay(ROOT/args.original,ROOT/args.out,args.cases)
