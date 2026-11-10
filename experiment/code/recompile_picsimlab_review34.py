"""Clean per-case recompilation of frozen sources; no candidate file is edited."""
import concurrent.futures,hashlib,json,os,shutil,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];OUT=ROOT/'experiment/output/picsimlab_verification_20261107';DEST=OUT/'binary_review_34_20261108/clean_firmware';BASE=Path.home()/'.cache/llm4mdd-review34-clean';ARD=Path.home()/'.cache/llm4mdd-arduino'
def run(case):
 cid=case['id'];folder=DEST/cid;folder.mkdir(parents=True,exist_ok=True)
 if (folder/'compilation.json').exists():return json.loads((folder/'compilation.json').read_text())
 source=OUT/'cases'/cid/'source.ino';data=source.read_bytes();assert hashlib.sha256(data).hexdigest()==case['source_sha256']
 work=BASE/cid;sketch=work/'sketch';sketch.mkdir(parents=True,exist_ok=True);(sketch/'sketch.ino').write_bytes(data)
 command=[str(ARD/'arduino-cli'),'--config-file',str(ARD/'native_config.yaml'),'compile','--fqbn',case['fqbn'],'--jobs','1','--build-path',str(work/'build'),str(sketch)]
 p=subprocess.run(command,capture_output=True,timeout=240);(folder/'compile.log').write_bytes(p.stdout+p.stderr)
 row=dict(case=cid,compile_ok=p.returncode==0,source_sha256=case['source_sha256'],command=command)
 if p.returncode==0:
  for name in ('sketch.ino.hex','sketch.ino.elf'):shutil.copyfile(work/'build'/name,folder/name)
  nm=ARD/'data/packages/arduino/tools/avr-gcc/7.3.0-atmel3.6.1-arduino7/bin/avr-nm'
  (folder/'symbols.txt').write_bytes(subprocess.run([str(nm),'-n',str(folder/'sketch.ino.elf')],capture_output=True,check=True).stdout)
  sha=hashlib.sha256((folder/'sketch.ino.hex').read_bytes()).hexdigest();old=hashlib.sha256((OUT/'cases'/cid/'firmware/sketch.ino.hex').read_bytes()).hexdigest()
  row.update(hex=dict(sha256=sha),previous_hex_sha256=old,hex_identical=sha==old)
 (folder/'compilation.json').write_text(json.dumps(row,indent=2));return row
def main():
 os.nice(15)
 ids={c['case'] for c in json.loads((OUT/'final_all296_cases.json').read_text()) if c['basic_core_status']=='failed'}
 cases=[c for c in json.loads((OUT/'manifest.json').read_text())['cases'] if c['id'] in ids]
 rows=[]
 with concurrent.futures.ThreadPoolExecutor(max_workers=2) as pool:
  for row in pool.map(run,cases):
   rows.append(row);print(row['case'],row['compile_ok'],row.get('hex_identical'),flush=True)
 (OUT/'binary_review_34_20261108/clean_compilation_summary.json').write_text(json.dumps(rows,indent=2))
if __name__=='__main__':main()
