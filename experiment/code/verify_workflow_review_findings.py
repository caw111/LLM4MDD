"""Reproduce two review findings against unchanged historical workflow kernels.

This is a development diagnosis, not a replacement for the frozen full review.
"""
import hashlib
import json
from pathlib import Path
import shutil
import subprocess


ROOT = Path(__file__).resolve().parents[2]
REVIEW = ROOT / 'experiment/output/code_review_all_20261023'
OUT = REVIEW / 'calibration_audit/historical_workflow'
HARNESSES = {
    'advanced_008': r'''
#include <iostream>
int main() {
  int32_t display=0,tone=0,e1=0,e2=0,e3=0; bool led=false,trigger=false;
  auto step=[&](bool add,bool select,bool start){
    ProgramPage1(add,select,start,&display,&tone,&led,&trigger,&e1,&e2,&e3);
  };
  ProgramPage1_Init(&ProgramPage1_instance);
  // A positive control ensures this harness can see changes in kernel state.
  sysml_state_countdown_display=123; sysml_state_processComplete=true;
  step(false,false,false);
  if(display!=123 || !led) return 2;
  ProgramPage1_Init(&ProgramPage1_instance);
  int changed=0;
  for(int mask=0;mask<8;++mask) {
    step(mask&1,mask&2,mask&4);
    if(display || tone || led || trigger || e1 || e2 || e3) ++changed;
  }
  for(int i=0;i<1000;++i) step(false,false,false);
  std::cout << "{\"button_combinations\":8,\"combinations_with_output_change\":" << changed
    << ",\"display_after_1000_steps\":" << display << ",\"tone_hz\":" << tone
    << ",\"completed\":" << led << ",\"stage1_elapsed_ms\":" << e1
    << ",\"positive_control_observed\":true}";
}
''',
    'advanced_009': r'''
#include <iostream>
int main() {
  double fan=0,temp=0; int32_t display=0,count=0; bool trigger=false;
  auto step=[&](double distance,bool valid){
    ProgramPage1(distance,valid,distance,valid,25.0,50.0,true,false,
      &fan,&display,&trigger,&count,&temp);
  };
  ProgramPage1_Init(&ProgramPage1_instance);
  sysml_state_peopleCount=5;step(400,false);
  if(count!=5 || display!=5) return 2;
  ProgramPage1_Init(&ProgramPage1_instance);step(400,false);
  int32_t invalidCount=count;
  ProgramPage1_Init(&ProgramPage1_instance);step(400,true);
  int32_t farCount=count; double farFan=fan;
  ProgramPage1_Init(&ProgramPage1_instance);step(5,true);
  std::cout << "{\"invalid_echo_count\":" << invalidCount
    << ",\"simultaneous_valid_400cm_count\":" << farCount
    << ",\"simultaneous_valid_400cm_fan_command\":" << farFan
    << ",\"simultaneous_valid_5cm_count\":" << count
    << ",\"positive_control_observed\":true}";
}
''',
}


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    config = json.loads((REVIEW / 'execution.json').read_text(encoding='utf-8'))
    jobs = {j['key']: j for j in config['jobs']}
    compiler = shutil.which('g++')
    if not compiler:
        raise RuntimeError('Local g++ is unavailable; no finding was reproduced')
    results = []
    for case, driver in HARNESSES.items():
        row = next(r for r in config['rows'] if r['group'] == 'workflow_280' and r['case'] == case)
        assert row['compile_ok']
        job = jobs[row['review_key']]
        original = ROOT / job['artifact']
        assert sha(original) == job['code_sha256']
        code = original.read_text(encoding='utf-8')
        marker = '// END COMPILED SYSML MODEL'
        assert code.count(marker) == 1
        kernel = code[:code.index(marker) + len(marker)]
        folder = OUT / case
        folder.mkdir(parents=True, exist_ok=True)
        source, binary = folder / 'counterexample.cpp', folder / 'counterexample.exe'
        source.write_text(kernel + '\n' + driver, encoding='utf-8')
        command = [compiler, '-std=c++17', str(source), '-o', str(binary)]
        build = subprocess.run(command, capture_output=True, text=True, timeout=60)
        (folder / 'compile.log').write_text(build.stdout + build.stderr, encoding='utf-8')
        assert build.returncode == 0, 'Witness did not build; do not count as code failure'
        run = subprocess.run([str(binary)], capture_output=True, text=True, timeout=10)
        assert run.returncode == 0, 'Witness positive control or execution failed'
        measured = json.loads(run.stdout)
        assert measured['positive_control_observed']
        if case == 'advanced_008':
            assert measured['combinations_with_output_change'] == 0
            conclusion = 'All eight button combinations leave outputs zero; countdown setup/start are unimplemented in this kernel.'
            quote = 'sysml_state_countdown_display = 0;'
        else:
            assert measured['invalid_echo_count'] == 0
            assert measured['simultaneous_valid_400cm_count'] == measured['simultaneous_valid_5cm_count'] == 1
            conclusion = 'Simultaneous valid echo initialization counts a person; distances 400cm and 5cm yield the same count. Valid echo is used as a person-trigger event.'
            quote = 'sysml_state_peopleCount = (sysml_state_peopleCount + 1);'
        assert sha(original) == job['code_sha256']
        result = dict(case=case, groups=[r['group'] for r in config['rows']
                      if r.get('review_key') == row['review_key']],
                      original_artifact=job['artifact'], original_sha256=sha(original),
                      unchanged_kernel_sha256=hashlib.sha256(kernel.encode()).hexdigest(),
                      driver_sha256=hashlib.sha256(driver.encode()).hexdigest(),
                      compiler=compiler, compiler_sha256=sha(Path(compiler)), command=command,
                      witness_source_sha256=sha(source), witness_binary_sha256=sha(binary),
                      measured=measured, conclusion=conclusion,
                      evidence=dict(code_quote=quote, line=code[:code.index(quote)].count('\n') + 1),
                      limitation='Unchanged control-kernel execution with synthetic inputs; no AVR/hardware timing validation; not a full-dataset or original LLM-review verdict.')
        (folder / 'result.json').write_text(json.dumps(result, ensure_ascii=False, indent=2), encoding='utf-8')
        results.append(result)
    (OUT / 'results.json').write_text(json.dumps(results, ensure_ascii=False, indent=2), encoding='utf-8')
    print(json.dumps([dict(case=r['case'], measured=r['measured']) for r in results]))


if __name__ == '__main__':
    main()
