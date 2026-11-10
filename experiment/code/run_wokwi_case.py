"""Run an original PCEval scenario against reference or generated firmware."""
import argparse,json,os,shutil,subprocess,time
from pathlib import Path
from dotenv import load_dotenv
from run_controlled_experiment import ROOT,compile_code,save,digest

def resolve_project(case,root=ROOT):
    """允许数据集搬迁，但不得改变冻结任务的文字或电路。"""
    original=Path(case['source'])
    candidates=[root/original]
    try:relative=original.relative_to('tests/PCEval-Dataset')
    except ValueError:relative=None
    if relative is not None:candidates.append(root/'experiment/datasets/PCEval-Dataset'/relative)
    source=next((p for p in candidates if p.is_file()),None)
    if source is None:raise ValueError('Frozen PCEval source is not available in either recorded or relocated dataset path')
    prose,separator,circuit=case['requirement'].partition('Given circuit (must preserve wiring):')
    description=source.read_text(encoding='utf-8-sig')
    diagram=source.parent/'diagram.json'
    if not separator or prose.strip()!=description.strip():
        raise ValueError('Local PCEval description differs from the frozen public requirement')
    if json.loads(circuit)!=json.loads(diagram.read_text(encoding='utf-8-sig')):
        raise ValueError('Local PCEval wiring differs from the frozen public circuit')
    return source.parent,dict(recorded_source=case['source'],resolved_source=str(source.relative_to(root)),
        source_sha256=digest(source),recorded_source_sha256=case.get('source_sha256'),
        circuit_sha256=digest(diagram),recorded_circuit_sha256=case.get('circuit_sha256'),
        equivalence='Description equal after BOM/outer-whitespace normalization; circuit JSON equal. Raw hashes retained.')


def find_cli(name,candidates):
    found=shutil.which(name)
    if found:return Path(found)
    return next((p for p in candidates if p.is_file()),None)


def run(case_id,sketch,folder):
    manifest=json.loads((ROOT/'experiment/formal/manifest.json').read_text(encoding='utf-8'))
    case=next(c for c in manifest['cases'] if c['id']==case_id)
    if case['dataset']!='PCEval':raise ValueError('Only original PCEval scenarios are supported.')
    project,identity=resolve_project(case)
    code_path=sketch or project/'src/main.ino'
    folder=folder.resolve();folder.mkdir(parents=True,exist_ok=True)
    load_dotenv(ROOT/'.env',override=True)
    load_dotenv(ROOT/'.env.local',override=True)
    token=os.getenv('WOKWI_CLI_TOKEN','')
    cli=find_cli('arduino-cli',[ROOT/'.tools/arduino/arduino-cli.exe'])
    simulator=find_cli('wokwi-cli',[ROOT/'.tools/wokwi/wokwi-cli.exe',ROOT/'experiment/.tools/wokwi-cli.exe'])
    identity.update(scenario_sha256=digest(project/'scenario.yaml'),runner_sha256=digest(Path(__file__)))
    if cli is None:
        result=dict(case=case_id,status='infrastructure_missing_arduino_cli',compile_ok=False,dataset_identity=identity)
        save(folder/'result.json',result);return result
    result,log=compile_code(folder,code_path.read_text(encoding='utf-8'),cli,case['fqbn'])
    result.update(case=case_id,source_sha256=digest(code_path),reference=sketch is None,dataset_identity=identity,
                  arduino_cli=str(cli),simulator=str(simulator) if simulator else None)
    if not result['compile_ok']:
        result['status']='reference_compile_failed' if sketch is None else 'compile_failed';save(folder/'result.json',result);return result
    for name in ('diagram.json','scenario.yaml'):shutil.copyfile(project/name,folder/name)
    if (project/'screenshots').is_dir():shutil.copytree(project/'screenshots',folder/'screenshots',dirs_exist_ok=True)
    elf=next((folder/'build').glob('*.elf'));hex_path=next((folder/'build').glob('*.hex'))
    (folder/'wokwi.toml').write_text('[wokwi]\nversion = 1\nelf = '+json.dumps(elf.relative_to(folder).as_posix())+'\nfirmware = '+json.dumps(hex_path.relative_to(folder).as_posix())+'\n',encoding='utf-8')
    if not token:
        result['status']='blocked_missing_token';save(folder/'result.json',result);return result
    if simulator is None:
        result['status']='infrastructure_missing_wokwi_cli';save(folder/'result.json',result);return result
    started=time.monotonic()
    command=[str(simulator),str(folder),'--scenario','scenario.yaml','--timeout','180000','--timeout-exit-code','42']
    try:
        proc=subprocess.run(command,capture_output=True,text=True,encoding='utf-8',errors='replace',timeout=300)
        output=(proc.stdout+'\n'+proc.stderr).replace(token,'[REDACTED]')
        (folder/'simulation.log').write_text(output,encoding='utf-8')
        status='passed' if proc.returncode==0 else 'simulation_failed'
        if any(x in output.lower() for x in ('unauthorized','invalid token','quota','insufficient','payment','forbidden')):status='infrastructure_auth_or_quota'
        if 'Invalid scenario step key' in output:status='oracle_incompatible_command'
        elif 'missing required parameter' in output:status='oracle_invalid_parameter'
        elif 'Pin not found' in output:status='oracle_invalid_pin'
        elif 'Failed to read comparison file' in output:status='oracle_missing_reference_asset'
        result.update(status=status,returncode=proc.returncode,simulation_seconds=round(time.monotonic()-started,3))
    except subprocess.TimeoutExpired:
        result['status']='infrastructure_wall_timeout'
    save(folder/'result.json',result);return result

if __name__=='__main__':
    p=argparse.ArgumentParser();p.add_argument('--case',required=True);p.add_argument('--sketch',type=Path);p.add_argument('--out',required=True,type=Path)
    a=p.parse_args();print(json.dumps(run(a.case,a.sketch,a.out),ensure_ascii=False))
