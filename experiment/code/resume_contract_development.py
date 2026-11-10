"""Resume infrastructure-interrupted development jobs into a NEW archive.

Generation and oracle sources must match the original frozen run. Completed jobs
are retained, including method failures; successful provider responses are replayed
with their metadata. Original transport failures remain in the original archive.
"""
import argparse
import copy
import hashlib
import json
from pathlib import Path
import shutil
import time
from unittest.mock import patch

import run_contract_development as runner
from app.services.llm_json import CompletionText, request_options


def read(path):
    return json.loads(path.read_text(encoding='utf-8'))


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def verify_sources(execution, root):
    for name, expected in execution['hashes'].items():
        if sha(runner.ROOT/name) != expected or sha(root/'method_snapshot'/name) != expected:
            raise ValueError('Frozen source mismatch: '+name)


class ResumeCalls(runner.BudgetCalls):
    def __init__(self, folder, model, *, previous, events):
        super().__init__(folder, model)
        self.previous=previous
        self.cached=[e for e in read(previous) if 'response' in e and not e.get('error_type')] if previous.exists() else []
        self.cursor=0
        self.events=events

    def __call__(self, prompt):
        if len(self.entries)>=6:
            raise ValueError('Six-call method budget exhausted')
        if self.cursor < len(self.cached):
            entry=self.cached[self.cursor]
            if entry['prompt'] != prompt or entry.get('response_format') != request_options(prompt).get('response_format'):
                raise RuntimeError('Cached request differs from frozen method; no new request issued')
            if entry.get('requested_model') != self.model:
                raise RuntimeError('Cached model differs from frozen experiment')
            self.cursor+=1
            self.entries.append(copy.deepcopy(entry))
            runner.save(self.folder/'calls.json',self.entries)
            self.events.append(dict(kind='replayed',source=str(self.previous),number=entry['number']))
            return entry['actual_model'],CompletionText(entry['response'],finish_reason=entry.get('finish_reason'),
                refusal=entry.get('refusal'),response_format=entry.get('response_format'))
        try:
            return super().__call__(prompt)
        finally:
            self.events.append(dict(kind='new_transport',folder=str(self.folder),entry=copy.deepcopy(self.entries[-1])))


def resume(source, destination):
    source=source.resolve();destination=destination.resolve()
    source.relative_to(runner.ROOT/'experiment/output')
    destination.relative_to(runner.ROOT/'experiment/output')
    execution=read(source/'execution.json')
    verify_sources(execution,source)
    if execution['public_interpretations'] != runner.INTERPRETATIONS:
        raise ValueError('Public interpretations changed')
    runtime=runner.runtime_identity()
    if runtime != execution['runtime']:
        raise ValueError('Runtime/tool identity changed; do not pool this run')
    evidence={str(p.relative_to(source)):sha(p) for p in source.rglob('*.json')}
    destination.mkdir(parents=True,exist_ok=False)
    shutil.copyfile(source/'execution.json',destination/'execution.json')
    shutil.copytree(source/'method_snapshot',destination/'method_snapshot')
    events=[];rows=[];jobs=[];started=time.monotonic()
    try:
        for repeat in range(1,execution['repetitions']+1):
            for case in execution['cases']:
                relative=Path(case['id'])/f'repeat_{repeat}'
                old=source/relative
                previous_rows=read(old/'results.json') if (old/'results.json').exists() else []
                terminal=len(previous_rows)==3 and not any(r.get('infrastructure_error') for r in previous_rows)
                if terminal:
                    # Keep original artifact paths/hashes. Never retry a method failure.
                    for p in list(old.glob('*.json'))+list(old.glob('*_calls/calls.json')):
                        target=destination/relative/p.relative_to(old)
                        target.parent.mkdir(parents=True,exist_ok=True)
                        shutil.copyfile(p,target)
                    rows.extend(previous_rows)
                    jobs.append(dict(case=case['id'],repeat=repeat,status='retained'))
                else:
                    def factory(folder,model):
                        return ResumeCalls(folder,model,previous=source/folder.relative_to(destination)/'calls.json',events=events)
                    with patch.object(runner,'BudgetCalls',factory):
                        recovered=runner.evaluate(case,repeat,destination,execution['model'])
                    rows.extend(recovered)
                    jobs.append(dict(case=case['id'],repeat=repeat,status='resumed'))
                    print(case['id'],repeat,[(r['arm'],r['compile_ok'],r['functional_status']) for r in recovered],flush=True)
                runner.save(destination/'results.json',rows)
                runner.save(destination/'resume_events.json',events)
    finally:
        unchanged=all((source/name).is_file() and sha(source/name)==digest for name,digest in evidence.items())
        runner.save(destination/'resume.json',dict(source=str(source.relative_to(runner.ROOT)),
            original_json_sha256=evidence,original_unchanged=unchanged,jobs=jobs,seconds=time.monotonic()-started,
            script_sha256=sha(Path(__file__)),events=events,
            interpretation='Inherited response costs are counted once in logical run metrics. New transport events are additional physical calls. '
            'Original failed transports remain archived and must also be included in total operational cost/time. '
            'Original interrupted wall time may include machine suspension; no latency inference.'))
        if not unchanged:raise ValueError('Original archive changed')
        verify_sources(execution,source)
    return rows


if __name__=='__main__':
    parser=argparse.ArgumentParser()
    parser.add_argument('--source',required=True);parser.add_argument('--out',required=True)
    args=parser.parse_args()
    resume(runner.ROOT/args.source,runner.ROOT/args.out)
