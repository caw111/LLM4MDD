"""Paired real-LLM pilot. Reference answers/tests never enter generation prompts."""
from __future__ import annotations
import argparse, ast, copy, hashlib, json, os, random, re, statistics, subprocess, sys, time
from pathlib import Path
ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT))
from app import main
from app.services.sysml import generate_validated_bundle, sysml_bundle_prompt, SysMLValidationError, SysMLToolError
from app.services.sysml_compiler import validate_executable_sysml
from app.services.arduino_adapter import generate_adapter

ARMS = ('direct', 'direct_feedback', 'workflow', 'no_sysml_feedback', 'llm_codegen')

def save(path, value):
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(value, ensure_ascii=False, indent=2), encoding='utf-8')

def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()

def cases(seed, full=False):
    rng = random.Random(seed)
    selected = []
    for level in ('simple','medium','complex','advanced'):
        path = ROOT / f'experiment/input/benchmark_requirements_{level}_30.json'
        rows = json.loads(path.read_text(encoding='utf-8'))
        for row in (rows if full else [rng.choice(rows)]):
            selected.append({'id':row['id'],'dataset':'internal','stratum':level,'requirement':row['requirement'],
                         'source':str(path.relative_to(ROOT)), 'source_sha256':digest(path)})
    for level in ('level1','level2','level3','level4'):
        paths = sorted((ROOT/'tests/PCEval-Dataset/projects'/level).glob('*/description.md'))
        for path in (paths if full else [rng.choice(paths)]):
            circuit = path.parent/'diagram.json'
            selected.append({'id':'pceval_'+path.parent.name,'dataset':'PCEval','stratum':level,
                         'requirement':path.read_text(encoding='utf-8')+'\nGiven circuit (must preserve wiring):\n'+circuit.read_text(encoding='utf-8'),
                         'source':str(path.relative_to(ROOT)), 'source_sha256':digest(path),'circuit_sha256':digest(circuit),
                         'held_out_test':str((path.parent/'scenario.yaml').relative_to(ROOT))})
    path = ROOT/'tests/EmbedAgent/dataset/EmbedBench.json'
    rows = json.loads(path.read_text(encoding='utf-8'))
    eligible = []
    for row in rows:
        diagrams = ast.literal_eval(row['diagram']) if isinstance(row['diagram'],str) else row['diagram']
        uno = diagrams.get('Arduino Mega')
        if uno is not None: eligible.append((row,uno))
    for row,circuit in (eligible if full else rng.sample(eligible,4)):
        selected.append({'id':'embedbench_'+str(row['id']),'dataset':'EmbedBench','stratum':'Mega',
                         'requirement':'Target board is Arduino Mega 2560 as specified in the supplied circuit; use this board when the prose says UNO.\n'+row['problem']+'\nGiven circuit (must preserve wiring):\n'+json.dumps(circuit,ensure_ascii=False),
                         'source':str(path.relative_to(ROOT)), 'source_sha256':digest(path),'row_id':row['id']})
    for case in selected:
        case["fqbn"] = "arduino:avr:mega" if case["dataset"] == "EmbedBench" else "arduino:avr:uno"
    return selected

class Calls:
    def __init__(self, folder, model):
        self.folder, self.model, self.entries = folder, model, []
    def __call__(self,prompt):
        from app.services.llm_json import request_options, completion_text
        if not main._llm_api_key:
            raise RuntimeError('LLM infrastructure unavailable: API key is not configured')
        started=time.monotonic()
        entry={'number':len(self.entries)+1,'prompt':prompt,'requested_model':self.model,'temperature':0.2,'max_tokens':6000}
        options=request_options(prompt)
        entry.update(options)
        self.entries.append(entry)
        try:
            response=main.client.with_options(timeout=180,max_retries=0).chat.completions.create(
                model=self.model,temperature=0.2,max_tokens=6000,
                messages=[{'role':'system','content':'You are an AI assistant specialized in embedded systems and code generation.'},
                          {'role':'user','content':prompt}],**options)
            entry.update(response=response.choices[0].message.content or '', actual_model=response.model,
                         finish_reason=response.choices[0].finish_reason,
                         refusal=getattr(response.choices[0].message,'refusal',None),
                         usage=response.usage.model_dump() if response.usage else None)
            return response.model,completion_text(response,prompt)
        except Exception as exc:
            # Do not persist provider error strings which might contain credentials.
            entry['error_type']=type(exc).__name__
            raise RuntimeError('LLM infrastructure failure: '+type(exc).__name__) from None
        finally:
            entry['seconds']=round(time.monotonic()-started,3)
            save(self.folder/'calls.json',self.entries)

def extract(text):
    match=re.search(r'```(?:cpp|c\+\+|arduino|c)?\s*\n([\s\S]*?)```',text,re.I)
    return (match.group(1) if match else text).strip()

def compile_code(folder,code,cli,fqbn):
    sketch=folder/'sketch';sketch.mkdir(parents=True,exist_ok=True)
    (sketch/'sketch.ino').write_text(code,encoding='utf-8')
    started=time.monotonic()
    try:
        proc=subprocess.run([str(cli),'compile','--fqbn',fqbn,'--build-path',str(folder/'build'),str(sketch)],
                            capture_output=True,text=True,encoding='utf-8',errors='replace',timeout=180)
        log=proc.stdout+'\n'+proc.stderr
        (folder/'compile.log').write_text(log,encoding='utf-8')
        missing=bool(re.search(r'No such file or directory|platform not installed|Error resolving FQBN',log,re.I))
        return {'compile_ok':proc.returncode==0,'compile_status':'passed' if proc.returncode==0 else ('missing_dependency' if missing else 'failed'),
                'compile_exit_code':proc.returncode,'compile_seconds':round(time.monotonic()-started,3)},log
    except subprocess.TimeoutExpired as exc:
        def text(value):
            return value.decode('utf-8',errors='replace') if isinstance(value,bytes) else value or ''
        log=text(exc.stdout)+'\n'+text(exc.stderr)+'\nCompiler timed out after 180 seconds.\n'
        (folder/'compile.log').write_text(log,encoding='utf-8')
        return {'compile_ok':False,'compile_status':'timeout','compile_seconds':round(time.monotonic()-started,3)},log

DIRECT_PROMPT='Generate a complete Arduino sketch for the following requirements and given circuit. Output only C++ code with setup() and loop(). Preserve all requirements and wiring; do not simplify away behavior.\n'

def metrics(entries):
    return {'llm_calls':len(entries),'tokens':sum((e.get('usage') or {}).get('total_tokens',0) for e in entries),
            'reported_cost_usd':sum((e.get('usage') or {}).get('cost',0) or 0 for e in entries),
            'llm_seconds':round(sum(e['seconds'] for e in entries),3)}

def evaluate(case,root,model,cli):
    folder=root/case['id'];folder.mkdir(parents=True,exist_ok=True)
    results=[]
    def result(arm,entries,**kw):
        row={'case':case['id'],'dataset':case['dataset'],'stratum':case['stratum'],'arm':arm,
             'compile_ok':False,'compile_status':'not_generated','functional_status':'not_run_no_wokwi_credentials',
             **metrics(entries),**kw}
        results.append(row);save(folder/'results.json',results)
        print(case['id'],arm,row['compile_status'],flush=True)
        return row
    calls=Calls(folder/'direct',model)
    prompt=DIRECT_PROMPT+case['requirement']
    _,raw=calls(prompt)
    first_entries=copy.deepcopy(calls.entries)
    code=extract(raw)
    check,log=compile_code(folder/'direct'/'attempt_0',code,cli,case['fqbn'])
    result('direct',first_entries,**check)
    for i in range(2):
        if check['compile_ok'] or check['compile_status']=='missing_dependency':break
        _,raw=calls(prompt+'\nPrevious sketch:\n'+code+'\nRepair compiler errors without dropping requirements:\n'+log[-12000:])
        code=extract(raw);check,log=compile_code(folder/'direct'/f'attempt_{i+1}',code,cli,case['fqbn'])
    result('direct_feedback',calls.entries,**check)

    calls=Calls(folder/'workflow',model)
    try:
        _,bundle,report=generate_validated_bundle(calls,sysml_bundle_prompt(case['requirement']),max_repairs=2,validator=validate_executable_sysml)
        save(folder/'workflow'/'validation.json',report)
        save(folder/'workflow'/'bundle.json',bundle)
    except SysMLValidationError as exc:
        save(folder/'workflow'/'validation.json',{'ok':False,'attempts':exc.attempts})
        result('workflow',calls.entries,model_ok=False,failure_stage='sysml')
        result('no_sysml_feedback',calls.entries[:1],model_ok=False,failure_stage='sysml')
        result('llm_codegen',calls.entries,model_ok=False,failure_stage='sysml')
        return results
    initial_ok=report['attempts'][0]['ok']
    model_entries=copy.deepcopy(calls.entries)
    model_info={'model_ok':True,'model_attempts':len(report['attempts']),'model_initial_ok':initial_ok}
    compiled=main.compile_sysml2_model(main.CompileSysmlRequest(system_sysml2=bundle['system_sysml2'],out_dir=str(folder/'workflow'/'ir')))
    generated=main.convert_il_to_c(main.ConvertIlToCRequest(il_file_path=compiled.files['il'],out_c_dir=str(folder/'workflow'/'c'),codegen_root=str(ROOT/'CodeGenerator2/CodeGenerator2'),optimize=False,target='Intel'))
    try:
        _,code=generate_adapter(calls,generated.contents['define.h'],generated.contents['ProgramPage1.c'],case['requirement'],{},max_repairs=2)
        check,_=compile_code(folder/'workflow'/'final',code,cli,case['fqbn'])
        row=result('workflow',calls.entries,**model_info,**check)
    except ValueError as exc:
        row=result('workflow',calls.entries,**model_info,failure_stage='adapter',error=str(exc))
    if initial_ok:
        copy_row={k:v for k,v in row.items() if k not in ('arm','case','dataset','stratum','llm_calls','tokens','reported_cost_usd','llm_seconds')}
        result('no_sysml_feedback',calls.entries,shared_artifact='workflow',**copy_row)
    else:
        result('no_sysml_feedback',model_entries[:1],model_ok=False,failure_stage='sysml')
    # Ablate deterministic lowering, holding the accepted model and requirements fixed.
    llm_codegen=Calls(folder/'llm_codegen',model)
    _,raw=llm_codegen(DIRECT_PROMPT+case['requirement']+'\nImplement this validated SysML model faithfully in Arduino C++:\n'+bundle['system_sysml2'])
    code=extract(raw);check,log=compile_code(folder/'llm_codegen'/'attempt_0',code,cli,case['fqbn'])
    for i in range(2):
        if check['compile_ok'] or check['compile_status']=='missing_dependency':break
        _,raw=llm_codegen(DIRECT_PROMPT+case['requirement']+'\nValidated model:\n'+bundle['system_sysml2']+'\nPrevious sketch:\n'+code+'\nCompiler diagnostics:\n'+log[-12000:])
        code=extract(raw);check,log=compile_code(folder/'llm_codegen'/f'attempt_{i+1}',code,cli,case['fqbn'])
    result('llm_codegen',model_entries+llm_codegen.entries,**model_info,**check)
    return results

def summarize(root):
    rows=[r for p in root.glob('*/results.json') for r in json.loads(p.read_text(encoding='utf-8'))]
    save(root/'results.json',rows)
    lines=['# Paired pilot experiment','', 'Compile means actual Arduino AVR target compilation (UNO for internal/PCEval, Mega 2560 for EmbedBench circuits), not functional correctness. All selected samples remain in the denominator. No Wokwi or hardware success is claimed.', '',
           '| Arm | n | Compile passes | Tokens (mean) | Calls (mean) | Reported cost USD |','|---|---:|---:|---:|---:|---:|']
    for arm in ARMS:
        group=[r for r in rows if r['arm']==arm]
        if group:lines.append(f"| {arm} | {len(group)} | {sum(r['compile_ok'] for r in group)}/{len(group)} | {statistics.mean(r['tokens'] for r in group):.0f} | {statistics.mean(r['llm_calls'] for r in group):.2f} | {sum(r['reported_cost_usd'] for r in group):.5f} |")
    lines+=['','Arms share initial generations and accepted models to control sampling noise. Per-arm accounting includes shared prefixes; do not sum arm costs to estimate actual spend. The call files record actual physical calls.','',
            'This is a one-repeat pilot. Feedback and representation may improve or worsen results; no superiority or significance is assumed. llm_codegen uses compiler feedback, whereas workflow adapter repairs use structural diagnostics, so this arm is not a strict equal-feedback-budget causal estimate.', '',
            'Reference solutions and test scenarios are withheld from prompts. Dataset circuits are provided equally. Missing library dependencies are recorded separately; failed model generation counts as end-to-end failure.']
    (root/'SUMMARY.md').write_text('\n'.join(lines)+'\n',encoding='utf-8')
    physical=[e for p in root.glob('*/*/calls.json') for e in json.loads(p.read_text(encoding='utf-8'))]
    save(root/'actual_usage.json',metrics(physical))

def main_cli():
    parser=argparse.ArgumentParser()
    parser.add_argument('--out',default='experiment/output/controlled_pilot_20260926')
    parser.add_argument('--model',default='gpt-4o-mini')
    parser.add_argument('--seed',type=int,default=42)
    parser.add_argument('--limit',type=int)
    parser.add_argument('--prepare-only',action='store_true')
    args=parser.parse_args();os.chdir(ROOT)
    root=(ROOT/args.out).resolve();root.mkdir(parents=True,exist_ok=True)
    manifest=root/'manifest.json'
    if manifest.exists(): selected=json.loads(manifest.read_text(encoding='utf-8'))['cases']
    else:
        selected=cases(args.seed)
        save(manifest,{'seed':args.seed,'model':args.model,'temperature':0.2,'max_tokens':6000,'repair_budget':2,
                       'selection':'one seeded random item per internal difficulty/PCEval level; four seeded random EmbedBench rows, preserving provided Mega circuits',
                       'repetitions':1,'cases':selected})
    if args.prepare_only:
        print([(c['dataset'],c['id']) for c in selected]);return
    cli=ROOT/'.tools/arduino/arduino-cli.exe'
    if not cli.exists():raise SystemExit('Arduino CLI missing')
    environment={}
    for name,command in [('cli',[str(cli),'version']),('cores',[str(cli),'core','list']),('libraries',[str(cli),'lib','list'])]:
        environment[name]=subprocess.check_output(command,text=True,encoding='utf-8',errors='replace')
    environment['source_hashes'] = {str(p.relative_to(ROOT)): digest(p) for p in [Path(__file__), ROOT/'app/services/sysml.py', ROOT/'app/services/sysml_compiler.py', ROOT/'app/services/arduino_adapter.py', ROOT/'tools/prompts/system_sysml2.prompt.md']}
    save(root/'environment.json',environment)
    for case in selected[:args.limit]:
        target=root/case['id']/'results.json'
        if target.exists() and len(json.loads(target.read_text(encoding='utf-8')))==len(ARMS):continue
        try:evaluate(case,root,args.model,cli)
        except Exception as exc:
            save(root/case['id']/'run_error.json',{'error_type':type(exc).__name__,'message':str(exc)[:500]})
            summarize(root)
            raise
        summarize(root)

if __name__=='__main__':main_cli()
