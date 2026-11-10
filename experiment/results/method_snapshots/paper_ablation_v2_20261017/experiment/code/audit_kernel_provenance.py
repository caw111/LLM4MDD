"""Observe the production adapter boundary without changing generation sources.

This audit does not alter experiment scores or send a generated sketch to a board.
It asks whether a matching signature alone proves a supplied C kernel came from
the model that passed the frozen dependency contract.
"""
import hashlib
import argparse
import json
from pathlib import Path
import sys
import tempfile
from unittest.mock import patch

ROOT=Path(__file__).resolve().parents[2]
sys.path.insert(0,str(ROOT))
from fastapi.testclient import TestClient
from app.main import app
from app.services.behavior_contract import check_contract
from app.services.ir_contract import validate_ir


def audit(destination=None):
    contract=json.loads((ROOT/'tools/examples/three_servos.contract.json').read_text(encoding='utf-8'))
    ports=contract['interfaces']
    inputs=[dict(name=p['name'],type=p['type']) for p in ports if p['direction']=='input']
    outputs=[dict(name=p['name'],type=p['type']) for p in ports if p['direction']=='output']
    ir=dict(version=2,name='ConstantWitness',page='ProgramPage1',execution='step-actions-then-outputs',
            inputs=inputs,outputs=outputs,states=[],updates=[],nodes=[],edges=[])
    for index,p in enumerate(outputs):
        name='constant'+str(index)
        ir['nodes'].append(dict(id=name,type='Expression',inports=[],outports=[dict(name='value',type='f64')],params={'Expression':'0.0'}))
        ir['edges'].append(dict(src=dict(kind='node',id=name,port='value'),dst=dict(kind='output',name=p['name'])))
    coverage=check_contract(validate_ir(ir),contract)
    header='''struct ProgramPage1_state {};
extern struct ProgramPage1_state ProgramPage1_instance;
void ProgramPage1_Init(struct ProgramPage1_state* self);
void ProgramPage1(int pot1,int pot2,double* angle1,double* angle2,double* angle3);
'''
    source='''struct ProgramPage1_state ProgramPage1_instance;
void ProgramPage1_Init(struct ProgramPage1_state* self) {}
void ProgramPage1(int pot1,int pot2,double* angle1,double* angle2,double* angle3) {
  *angle1=0.0;*angle2=0.0;*angle3=0.0;
}
'''
    with tempfile.TemporaryDirectory() as folder,TestClient(app) as client,patch('app.main._call_llm',side_effect=AssertionError('No LLM allowed')):
        target=Path(folder)/'witness.ino'
        response=client.post('/convert/c-to-arduino',json=dict(c_code=source,compiled_kernel=dict(header=header,source=source),
            interface_contract=contract,out_path=str(target)))
        report=dict(scope='Adapter API provenance audit only; not a claim that the ordinary UI sent a wrong kernel.',
                    canonical_ir_coverage=coverage,adapter_http_status=response.status_code,artifact_written=target.exists(),
                    response_keys=list(response.json()) if isinstance(response.json(),dict) else [],
                    adapter_diagnostic=response.json().get('detail'),
                    new_llm_calls=0,physical_uploads=0,
                    source_hashes={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in [ROOT/'app/main.py',ROOT/'app/services/artifact_provenance.py',ROOT/'app/services/interface_planner.py',ROOT/'app/services/arduino_adapter.py',ROOT/'app/services/hardware_adapter.py']})
        report['gap_confirmed']=bool(coverage['issues']) and response.status_code==200 and target.exists()
        report['unproven_kernel_rejected']=bool(coverage['issues']) and response.status_code==422 and not target.exists()
    destination=destination or ROOT/'experiment/analysis/kernel_provenance_audit_v1.json'
    if destination.exists():raise ValueError('Preserve the existing audit; choose a new report version')
    destination.write_text(json.dumps(report,ensure_ascii=False,indent=2),encoding='utf-8')
    print(json.dumps({k:report[k] for k in ('gap_confirmed','adapter_http_status','artifact_written','new_llm_calls','physical_uploads')}))


if __name__=='__main__':
    parser=argparse.ArgumentParser();parser.add_argument('--out',default='experiment/analysis/kernel_provenance_audit_v1.json')
    args=parser.parse_args();audit(ROOT/args.out)
