"""Check a candidate mismatch against the previously supplied independent reference.

A failing reference flags an oracle/environment question; it does not prove a
candidate correct, and is never converted to a functional pass automatically.
"""
import argparse
from concurrent.futures import ThreadPoolExecutor,as_completed
import hashlib
import json
import os
from pathlib import Path
import queue
import shutil
import subprocess
import time

import run_picsimlab_all as sim

ARDUINO=Path.home()/'.cache/llm4mdd-arduino'


def check(case,index,entry,slots):
    slot=slots.get()
    try:
        output=sim.OUT/'cases'/case['id']/'simulation'/sim.VERSION/f'reference_{index:02d}'
        result=output/'preflight.json'
        if result.exists():return sim.load(result)
        output.mkdir(parents=True,exist_ok=True)
        code=entry['plan'].get('reference_cpp')
        if not code:
            row=dict(case=case['id'],plan=index,status='no_saved_independent_reference');sim.save(result,row);return row
        work=sim.BASE/'reference_compilers'/str(slot)/('mega' if 'mega' in case['fqbn'] else 'uno')
        sketch=work/'sketch';sketch.mkdir(parents=True,exist_ok=True)
        (sketch/'sketch.ino').write_text(code,encoding='utf8');(output/'reference_source.ino').write_text(code,encoding='utf8')
        command=[str(ARDUINO/'arduino-cli'),'--config-file',str(ARDUINO/'native_config.yaml'),'compile','--fqbn',case['fqbn'],'--jobs','1','--build-path',str(work/'build'),str(sketch)]
        compiled=subprocess.run(command,capture_output=True,timeout=240)
        (output/'compile.stdout.log').write_bytes(compiled.stdout);(output/'compile.stderr.log').write_bytes(compiled.stderr)
        if compiled.returncode:
            row=dict(case=case['id'],plan=index,status='reference_compilation_unverified',exit_code=compiled.returncode)
            sim.save(result,row);return row
        for name in ('sketch.ino.hex','sketch.ino.elf'):shutil.copyfile(work/'build'/name,output/name)
        nm=ARDUINO/'data/packages/arduino/tools/avr-gcc/7.3.0-atmel3.6.1-arduino7/bin/avr-nm'
        symbols=subprocess.run([str(nm),'-n',str(output/'sketch.ino.elf')],capture_output=True,check=True).stdout
        (output/'symbols.txt').write_bytes(symbols)
        digest=hashlib.sha256(code.encode()).hexdigest();firmware_sha=hashlib.sha256((output/'sketch.ino.hex').read_bytes()).hexdigest()
        fixture=dict(case,source_sha256=digest,_variant='reference',_symbols_path=str(output/'symbols.txt'),
                     _firmware_path=str(output/'sketch.ino.hex'),_compiled_override=dict(compile_ok=True,source_sha256=digest,hex=dict(sha256=firmware_sha)))
        # The reference and candidate use the same firmware engine, wiring and input plan.
        row=sim.run_plan(fixture,entry,slot,index)
        row.update(preflight_status='reference_passed' if row['status']=='scenarios_passed' else 'oracle_or_environment_requires_review')
        sim.save(result,row);return row
    except Exception as exc:
        return dict(case=case['id'],plan=index,status='reference_infrastructure_unverified',error=str(exc))
    finally:slots.put(slot)


def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--version',default='native-electrical-v4');p.add_argument('--workers',type=int,default=4);p.add_argument('--case',action='append');args=p.parse_args()
    os.nice(5);os.sched_setaffinity(0,set(sorted(os.sched_getaffinity(0))[:16]))
    sim.VERSION=args.version;sim.APP=sim.BASE/'engines'/sim.VERSION/'picsimlab_NOGUI';sim.RUN=sim.BASE/'reference_workers'/sim.VERSION
    slots=queue.Queue()
    for i in range(args.workers):slots.put(i)
    candidates=sim.load(sim.OUT/'manifest.json')['cases']
    jobs=[]
    for case in candidates:
        if args.case and case['id'] not in args.case:continue
        case_result=sim.OUT/'cases'/case['id']/'simulation'/sim.VERSION/'case_result.json'
        if not case_result.exists():continue
        for row in sim.load(case_result)['plans']:
            if row.get('status')!='failed':continue
            index=row['plan_index']
            folder=sim.OUT/row.get('result_directory',f"cases/{case['id']}/simulation/{sim.VERSION}/plan_{index:02d}")
            entry=dict(name=row['plan_name'],plan=sim.load(folder/'original_test_plan.json'))
            jobs.append((case,index,entry))
    rows=[]
    with ThreadPoolExecutor(max_workers=args.workers) as pool:
        futures=[pool.submit(check,*job,slots) for job in jobs]
        for f in as_completed(futures):
            row=f.result();rows.append(row);print(row['case'],row.get('preflight_status',row['status']),flush=True)
            sim.save(sim.OUT/f'{sim.VERSION}_reference_summary.json',dict(planned=len(jobs),completed=len(rows),
                counts={s:sum(r.get('preflight_status',r['status'])==s for r in rows) for s in {r.get('preflight_status',r['status']) for r in rows}}))


if __name__=='__main__':main()
