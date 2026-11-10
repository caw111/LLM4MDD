"""Compile frozen delivery sources for MCU simulation; never change the sketches."""
import argparse
from concurrent.futures import ThreadPoolExecutor, as_completed
import hashlib
import json
import os
from pathlib import Path
import queue
import shutil
import subprocess
import time

ROOT=Path(__file__).resolve().parents[2]
CLI=Path(os.environ.get('PICSIMLAB_ARDUINO_CLI',str(ROOT/'.tools/arduino/arduino-cli.exe')))
NM=Path(os.environ.get('PICSIMLAB_AVR_NM','C:/Users/27872/AppData/Local/Arduino15/packages/arduino/tools/avr-gcc/7.3.0-atmel3.6.1-arduino7/bin/avr-nm.exe'))


def run(command,**kwargs):
    if os.name=='nt':
        raise RuntimeError('Windows compiler launches are disabled: run this batch with native Linux tools inside WSL.')
    assert not str(command[0]).lower().endswith('.exe'),'Windows child processes are not allowed in the background batch'
    return subprocess.run(command,**kwargs)


def read(p):return json.loads(p.read_text(encoding='utf8'))


def save(p,value):
    p.parent.mkdir(parents=True,exist_ok=True)
    temporary=p.with_suffix(p.suffix+'.tmp');temporary.write_text(json.dumps(value,ensure_ascii=False,indent=2),encoding='utf8')
    temporary.replace(p)


def worker(case,out,slots):
    folder=out/'cases'/case['id'];result=folder/'compilation.json'
    if result.exists():return read(result)
    slot=slots.get();started=time.monotonic()
    try:
        source=folder/'source.ino';digest=hashlib.sha256(source.read_bytes()).hexdigest()
        assert digest==case['source_sha256']
        cache=Path(os.environ.get('PICSIMLAB_BUILD_CACHE',str(out/'compiler_workers/linux')))
        work=cache/str(slot)/('mega' if 'mega' in case['fqbn'] else 'uno')
        sketch=work/'sketch';sketch.mkdir(parents=True,exist_ok=True);shutil.copyfile(source,sketch/'sketch.ino')
        command=[str(CLI),'compile','--fqbn',case['fqbn'],'--jobs','1','--build-path',str(work/'build'),str(sketch)]
        p=run(command,capture_output=True,timeout=240)
        (folder/'compile.stdout.log').write_bytes(p.stdout);(folder/'compile.stderr.log').write_bytes(p.stderr)
        row=dict(case=case['id'],dataset=case['dataset'],board=case['fqbn'],source_sha256=digest,
                 command=command,compile_ok=p.returncode==0,exit_code=p.returncode,toolchain_platform=os.name)
        if row['compile_ok']:
            firmware=folder/'firmware';firmware.mkdir(exist_ok=True)
            for name in ('sketch.ino.hex','sketch.ino.elf'):
                source_file=work/'build'/name;assert source_file.is_file(),str(source_file)
                target=firmware/name;shutil.copyfile(source_file,target)
                row[name.rsplit('.',1)[1]]=dict(path=str(target.relative_to(ROOT)),sha256=hashlib.sha256(target.read_bytes()).hexdigest())
            preprocessed=work/'build/sketch/sketch.ino.cpp'
            if preprocessed.exists():shutil.copyfile(preprocessed,firmware/'compiled_candidate.cpp')
            symbols=run([str(NM),'-n',str(firmware/'sketch.ino.elf')],capture_output=True,text=True,encoding='utf8',errors='replace',timeout=20)
            (folder/'symbols.txt').write_text(symbols.stdout,encoding='utf8')
            for line in symbols.stdout.splitlines():
                fields=line.split()
                if len(fields)==3 and fields[2]=='timer0_millis':row['millis_ram_address']=int(fields[0],16)-0x800000
        row['seconds']=round(time.monotonic()-started,2);save(result,row);return row
    except Exception as exc:
        row=dict(case=case['id'],dataset=case['dataset'],compile_ok=False,compile_status='infrastructure_unverified',error_type=type(exc).__name__,error=str(exc)[:1500])
        save(result,row);return row
    finally:slots.put(slot)


def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--out',type=Path,default=ROOT/'experiment/output/picsimlab_verification_20261107')
    p.add_argument('--workers',type=int,default=1);p.add_argument('--case',action='append');args=p.parse_args()
    if os.name=='nt':p.error('Use WSL and native Linux Arduino tools; Windows compilation is disabled to prevent pop-up windows.')
    out=args.out.resolve();out.relative_to(ROOT/'experiment/output');assert 1<=args.workers<=12
    all_cases=read(out/'manifest.json')['cases']
    cases=list(all_cases)
    if args.case:cases=[c for c in cases if c['id'] in args.case]
    # Cheap sketches first make the running simulator useful while complex builds continue.
    cases.sort(key=lambda c:(c['dataset']!='EmbedBench',len(c['requirement']),c['id']))
    slots=queue.Queue()
    for n in range(args.workers):slots.put(n)
    with ThreadPoolExecutor(max_workers=args.workers) as pool:
        futures=[pool.submit(worker,c,out,slots) for c in cases]
        for future in as_completed(futures):
            row=future.result();print(row['case'],row['compile_ok'],flush=True)
            results=[read(f) for f in (out/'cases').glob('*/compilation.json')]
            save(out/'compilation_summary.json',dict(planned=len(all_cases),batch_planned=len(cases),completed=len(results),compiled=sum(r['compile_ok'] for r in results),
                unverified=sum(r.get('compile_status')=='infrastructure_unverified' for r in results)))


if __name__=='__main__':main()
