"""Rescore unchanged development artifacts with a separately snapshotted oracle.

Never overwrites original scores or calls a generation model. This is a change
to measurement coverage, not an improvement to the archived generation method.
"""
import argparse
import hashlib
import json
from pathlib import Path
import shutil

from run_contract_development import ROOT, ORACLE_TASKS, behavior, save
from experiment_metrics import functional_rates, paired
from method_snapshot import runtime_identity


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def replay(original, destination):
    original=original.resolve();destination=destination.resolve()
    original.relative_to((ROOT/'experiment/output').resolve())
    destination.relative_to((ROOT/'experiment/output').resolve())
    execution=json.loads((original/'execution.json').read_text(encoding='utf-8'))
    rows=[r for path in original.glob('*/repeat_*/results.json') for r in json.loads(path.read_text(encoding='utf-8'))]
    arms=('direct','direct_feedback','workflow')
    expected={(case['id'],repeat,arm) for case in execution['cases'] for repeat in range(1,execution['repetitions']+1) for arm in arms}
    if len(rows)!=len(expected) or {(r['case'],r['repeat'],r['arm']) for r in rows}!=expected:
        raise ValueError('Replay requires all planned generation records; incomplete originals must remain incomplete')
    original_files={original/'execution.json',*original.glob('*/repeat_*/results.json')}
    for row in rows:
        if row.get('artifact'):
            artifact=(ROOT/row['artifact']).resolve();artifact.relative_to(ROOT)
            original_files.add(artifact)
            if row['compile_ok']:
                original_files.add(artifact.parent.parent/'build/sketch/sketch.ino.cpp')
    original_hashes={str(path.relative_to(ROOT)):sha(path) for path in original_files}
    destination.mkdir(parents=True,exist_ok=False)
    sources=[Path(__file__),ROOT/'experiment/code/run_contract_development.py',ROOT/'experiment/code/experiment_metrics.py',ROOT/'experiment/code/method_snapshot.py',
             ROOT/'experiment/development/oracles/gpio_arduino.h',ROOT/'experiment/development/oracles/gpio_trace.cpp',ROOT/'experiment/development/oracles/TM1637Display.h',ROOT/'experiment/development/oracles/Servo.h',ROOT/'experiment/development/oracles/DHT.h',ROOT/'experiment/development/oracles/mixed_environment_trace.h',ROOT/'experiment/development/oracles/ArduinoHooks.cpp']
    hashes={str(p.relative_to(ROOT)):sha(p) for p in sources}
    for source in sources:
        target=destination/'oracle_snapshot'/source.relative_to(ROOT)
        target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(source,target)
    report={'original':str(original.relative_to(ROOT)),'original_execution_sha256':sha(original/'execution.json'),
        'oracle_source_hashes':hashes,'original_file_hashes':original_hashes,
        'new_llm_calls':0,'runtime':runtime_identity(),
        'scope':'Independent host replay of unchanged Arduino-preprocessed code. No physical hardware verification. Original scores retained.',
        'rows':[]}
    save(destination/'report.json',report)
    for row in rows:
        updated={**row,'original_functional_status':row['functional_status']}
        if row['compile_ok']:
            artifact=(ROOT/row['artifact']).resolve();artifact.relative_to(ROOT)
            original_hash=sha(artifact)
            normalized_hash=hashlib.sha256(artifact.read_text(encoding='utf-8').encode('utf-8')).hexdigest()
            if original_hash!=row['artifact_sha256'] and normalized_hash!=row['artifact_sha256']:
                raise ValueError('Artifact changed since original scoring: '+str(artifact))
            preprocessed=artifact.parent.parent/'build/sketch/sketch.ino.cpp'
            target=destination/row['case']/f"repeat_{row['repeat']}"/row['arm']
            copied=target/'build/sketch/sketch.ino.cpp'
            copied.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(preprocessed,copied)
            board=next(c['fqbn'] for c in execution['cases'] if c['id']==row['case'])
            updated.update(behavior(target,ORACLE_TASKS[row['case']],board))
            updated.update(replayed_preprocessed_sha256=sha(copied),replayed_artifact_sha256=original_hash,
                           replay_directory=str(target.relative_to(ROOT)))
        report['rows'].append(updated)
        save(destination/'report.json',report)
    report['arms']={arm:functional_rates([r for r in report['rows'] if r['arm']==arm]) for arm in arms}
    report['paired_functional']=paired(report['rows'],'workflow','direct','functional')
    report['source_snapshot_unchanged']=all(sha(ROOT/name)==digest for name,digest in hashes.items())
    if not report['source_snapshot_unchanged']:
        save(destination/'report.json',report)
        raise RuntimeError('Oracle changed during replay')
    report['originals_unchanged']=all((ROOT/name).is_file() and sha(ROOT/name)==digest for name,digest in original_hashes.items())
    if not report['originals_unchanged']:
        save(destination/'report.json',report)
        raise RuntimeError('Original records or artifacts changed during replay')
    report['complete']=all(v['complete'] for v in report['arms'].values())
    save(destination/'report.json',report)
    print(json.dumps({'complete':report['complete'],'arms':report['arms']},ensure_ascii=False))
    return report


if __name__=='__main__':
    parser=argparse.ArgumentParser();parser.add_argument('--original',required=True);parser.add_argument('--out',required=True)
    args=parser.parse_args();replay(ROOT/args.original,ROOT/args.out)
