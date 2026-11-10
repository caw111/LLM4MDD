"""Paired development checks of the actual contract-first pipeline, with fixed oracles.

This is not the formal six-arm experiment. GPIO, ADC/PWM and state-interaction
development cases cannot establish general non-inferiority over the full corpus.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import time

from run_controlled_experiment import ROOT, Calls, DIRECT_PROMPT, extract, metrics, compile_code
from run_formal_experiment import BackendFailure
import app.main as backend_api
from app.services.interface_planner import plan_interface_contract, ContractPlanningError
from app.services.sysml import generate_validated_bundle, sysml_bundle_prompt, SysMLValidationError
from app.services.sysml_compiler import validate_executable_sysml
from experiment_metrics import functional_rates, paired
from method_snapshot import runtime_identity

INTERPRETATIONS = {
    'complex_001': '验收约定：目标Uno，DHT11数据D7，HC-SR04 TRIG=D8/ECHO=D9，TM1637 CLK=D4/DIO=D5，无源蜂鸣器D6。DHT采样开始间隔2000..2200ms；距离采样开始间隔60..180ms，回波等待必须有25000us超时，有效范围2..400cm，距离=回波微秒/58。温度默认先显示，之后每2000ms交替显示温度和距离，显示整数且量化容差1，页面切换容差150ms；所选测量无效时空白，初始温度无效须空白。每次新读数到输出的响应容差200ms，DHT缓存期间保留最近读数；失败必须标为无效，不能阻塞其他设备或模型计时。仅有效温度严格>32℃或有效距离严格<15cm触发报警，先比较原始测量再取显示整数；任一原因成立立即开始1000Hz鸣叫，500ms响/500ms静，完整周期1000ms，边沿容差100ms；所有原因消失后200ms内静音。持续报警时原因变化不重启蜂鸣相位。串口115200：报警开始、原因变化后200ms内，以及持续报警每1000ms（容差200ms）报告所有当前成立原因。温度原因包含ALARM_TEMP，距离原因包含ALARM_DISTANCE，可同行或分行；不得报告不成立原因，其余状态文本不限。首次采样200ms内开始；设备故障恢复后继续真实测量。以上补充约定对各组相同。',
    'medium_007': '验收约定：目标板Uno；HC-SR04 TRIG=D8、ECHO=D9，TM1637 CLK=D4、DIO=D5。距离以cm为单位，回波微秒数除58；有效范围2..400cm，显示整数厘米（量化容差1cm，前导零或空白均可）。采样开始间隔60..160ms，变化250ms内反映（包含测量等待和显示传输）。每次等待回波必须有25000us超时；无回波、卡高或越界时显示空白，恢复后继续显示真实距离。初始显示空白；测量值和有效性必须区分，不得将超时当成0cm。原需求未指定的CLK/DIO采用上述公开接线，各组相同。',
    'medium_022': '验收约定：目标板Uno；HC-SR04 TRIG=D8、ECHO=D9，TM1637 CLK=D4、DIO=D5，LED=D13，高电平亮。距离以cm为单位，回波微秒数除58；有效范围2..400cm，显示整数厘米（量化容差1cm，前导零或空白均可）。有效距离严格小于10cm时LED亮，等于或大于10cm时灭；阈值判断使用测量距离而非显示取整值。采样开始间隔60..160ms，变化250ms内反映（包含测量等待和显示传输）。每次等待回波必须有25000us超时；无回波、卡高或越界时显示空白且LED灭，恢复后继续响应。初始显示空白且LED灭；测量值和有效性必须区分，不得将超时当成0cm。原需求未指定的传感器及显示引脚采用上述公开接线，各组相同。',
    'embedbench_79': '验收约定：目标板Mega2560；保留原接线，D2加一、D3减一、D4清零，均INPUT_PULLUP低电平按下；A3原始ADC线性映射重复间隔100+floor(ADC*900/1023)毫秒。初始00，按下立即执行一步；长按重复，间隔从上一次步进算起，旋钮变化立即改变当前间隔，松开停止，新的按下重新计时。上溢99回0，下溢0回99。清零优先并重置重复计时；加减同时按下时不步进。响应及重复时间容差20ms。数码管保留前导零，sr1十位、sr2个位。',
    'embedbench_104': '验收约定：目标板Mega2560；A3、A4分别控制D2、D3的PWM，原始ADC0..1023线性映射0..255，容差1计数；A5映射floor(ADC*99/1023)得到显示整数0..99。偶数只允许D2按A3设定亮度输出，D3为0；奇数相反，D2为0、D3按A4亮度输出，亮度设为0时允许熄灭。初始显示00且两个LED熄灭，之后连续响应真实采样值，变化50ms内反映。sr1十位、sr2个位，保留前导零，遵守原接线及共阳极。',
    'embedbench_16': '验收约定：目标板Mega2560；A0接sr1 DS、A2接共用SHCP、A1接共用STCP，sr1 Q7S串接sr2 DS。两片各自Q0..Q6接对应数码管A..G，均为共阳极；sr1显示十位、sr2显示个位。D2按键使用INPUT_PULLUP，低电平按下；每个按下沿加37，长按和松开不重复加，超过99清零（不是减100）。初始化显示00，每次按下20ms内更新，保留前导零。',
    'embedbench_25': '验收约定：目标板 Mega2560；A3 电位器控制 D2 舵机，A4 电位器同时控制 D3、D4 舵机。各输入 0..1023 线性映射目标角度 0..180 度；量化容差 1.1 度，输入变化后目标角度命令响应不超过 50ms。检查的是命令随输入连续映射，不代表已经验证实物轴运动速度、负载或平滑度。',
    'simple_001': '验收约定：初始化完成后 20ms 内点亮，随后保持常亮。',
    'simple_002': '验收约定：每秒闪烁一次指完整周期为1000ms；初始熄灭，500ms时首次点亮，此后每500ms翻转。时间容差10ms。',
    'simple_003': '验收约定：使用INPUT_PULLUP，低电平表示按下；输出响应时间不超过20ms。',
    'simple_008': '验收约定：初始亮度0，采用线性三角波；1000ms从0渐亮至255，再用1000ms渐暗至0，完整周期2000ms并持续循环。亮度容差5个PWM计数，更新间隔不超过20ms。',
    'medium_004': '验收约定：D2使用INPUT_PULLUP，低电平表示按下；按下时A0的0..1023线性映射到D9的PWM值0..255（取整容差1），松开时输出0。输入变化响应时间不超过20ms。',
    'medium_028': '验收约定：D2使用INPUT_PULLUP，低电平表示按下；每次按下沿切换运行/停止，长按或松开不切换。初始停止且D13熄灭；启动时从熄灭开始，每500ms翻转；停止立即熄灭，再启动重新计时。输入响应及翻转时间容差20ms。',
    'simple_021': '验收约定：使用无源蜂鸣器D6，音调1000Hz；初始静音，第一次在启动后1000ms开始，此后每1000ms开始一次，每次持续100ms；开始及结束时间容差20ms。',
    'medium_029': '验收约定：D6使用无源蜂鸣器，音调1000Hz，每次持续100ms；初始运行且静音，累计运行3000ms后首次发声，此后每累计运行3000ms开始一次。D2使用INPUT_PULLUP，低电平按下沿切换暂停/恢复，长按及松开不切换。暂停立即静音，保留计时和剩余鸣叫时长，恢复后继续，不能重新开始完整周期。响应及鸣叫时间容差20ms。',
    'simple_019': '验收约定：A0真实ADC读数0..1023线性映射为0..9999整数，向下取整；TM1637显示完整十进制数，允许前导零或空白。CLK=D4、DIO=D5，显示始终开启。输入变化后的显示响应时间不超过50ms，包括库的总线传输时间。',
}
ORACLE_TASKS = {'simple_001':1, 'simple_002':2, 'simple_003':3,
                'simple_008':8, 'medium_004':4, 'medium_007':7, 'medium_022':22, 'medium_028':28, 'simple_021':21, 'medium_029':29, 'simple_019':19, 'embedbench_25':125, 'embedbench_16':116, 'embedbench_79':179, 'embedbench_104':204, 'complex_001':301}


def save(path, value):
    path.parent.mkdir(parents=True, exist_ok=True)
    temporary = path.with_suffix(path.suffix+'.tmp')
    temporary.write_text(json.dumps(value, ensure_ascii=False, indent=2), encoding='utf-8')
    temporary.replace(path)


class BudgetCalls(Calls):
    def __call__(self, prompt):
        if len(self.entries) >= 6:
            raise ValueError('Six-call method budget exhausted')
        return super().__call__(prompt)


def behavior(attempt, task, board='arduino:avr:uno'):
    from app.services.hardware_adapter import board_profile
    board_macro = board_profile(board)['macro']
    folder = attempt/'host_oracle'
    folder.mkdir(exist_ok=True)
    oracle_dir = ROOT/'experiment/development/oracles'
    shutil.copyfile(oracle_dir/'gpio_arduino.h', folder/'Arduino.h')
    shutil.copyfile(oracle_dir/'TM1637Display.h', folder/'TM1637Display.h')
    shutil.copyfile(oracle_dir/'Servo.h', folder/'Servo.h')
    shutil.copyfile(oracle_dir/'DHT.h', folder/'DHT.h')
    shutil.copyfile(oracle_dir/'mixed_environment_trace.h', folder/'mixed_environment_trace.h')
    shutil.copyfile(oracle_dir/'gpio_trace.cpp', folder/'oracle.cpp')
    shutil.copyfile(oracle_dir/'ArduinoHooks.cpp', folder/'ArduinoHooks.cpp')
    # Use Arduino's preprocessed source so prototypes match the actual compiler.
    cpp = attempt/'build/sketch/sketch.ino.cpp'
    binary = folder/'oracle.exe'
    compiler,archiver=shutil.which('g++'),shutil.which('ar')
    if not compiler or not archiver:
        return {'functional_status':'oracle_build_unverified','oracle_error':'Host g++ and ar are required'}
    commands=[
        [compiler,'-std=c++17','-c',str(folder/'ArduinoHooks.cpp'),'-o',str(folder/'ArduinoHooks.o')],
        [archiver,'rcs',str(folder/'ArduinoHooks.a'),str(folder/'ArduinoHooks.o')],
        [compiler,'-std=c++17','-D'+board_macro,'-I',str(folder),str(cpp),str(folder/'oracle.cpp'),str(folder/'ArduinoHooks.a'),'-o',str(binary)],
    ]
    build_log=''
    for command in commands:
        build=subprocess.run(command,capture_output=True,text=True,encoding='utf-8',errors='replace',timeout=60)
        build_log+=build.stdout+build.stderr
        (folder/'build.log').write_text(build_log,encoding='utf-8')
        if build.returncode:return {'functional_status':'oracle_build_unverified'}
    try:
        run = subprocess.run([str(binary), str(task)], capture_output=True, text=True, timeout=10)
        (folder/'trace.log').write_text(run.stdout+run.stderr, encoding='utf-8')
        status = 'passed' if run.returncode==0 else ('oracle_runtime_unverified' if run.returncode==3 else 'failed')
        return {'functional_status': status, 'oracle_returncode': run.returncode}
    except subprocess.TimeoutExpired:
        return {'functional_status': 'oracle_timeout_unverified', 'oracle_error': 'Host clock model could not complete execution within 10 seconds'}


def compiled(case, folder, code):
    folder.mkdir(parents=True, exist_ok=True)
    check, log = compile_code(folder, code, Path(shutil.which('arduino-cli')), case['fqbn'])
    result = {**check, 'artifact': str((folder/'sketch/sketch.ino').relative_to(ROOT)),
              'artifact_sha256': hashlib.sha256((folder/'sketch/sketch.ino').read_bytes()).hexdigest(),
              'artifact_source_sha256_lf': hashlib.sha256(code.replace('\r\n','\n').encode('utf-8')).hexdigest(),
              'functional_status': 'not_generated_or_compile_failed'}
    if check['compile_ok']:
        result.update(behavior(folder, ORACLE_TASKS[case['id']], case['fqbn']))
    return result, log


def lower_verified(source,contract,accepted_ir,folder):
    compiled_model=backend_api.compile_sysml2_model(backend_api.CompileSysmlRequest(
        system_sysml2=source,interface_contract=contract,out_dir=str(folder/'ir')))
    if compiled_model.contents['subsystem_ir']!=accepted_ir:
        raise ValueError('Production compilation differs from the IR accepted during model repair')
    try:
        generated=backend_api.convert_il_to_c(backend_api.ConvertIlToCRequest(
            il_file_path=compiled_model.files['il'],out_c_dir=str(folder/'c'),
            interface_contract=contract,provenance=compiled_model.contents['provenance']))
    except backend_api.HTTPException as exc:
        detail=exc.detail
        if isinstance(detail,dict) and detail.get('failed_step')=='ILTranslator' and 'ILParser Error:' in detail.get('stdout',''):
            save(folder/'backend_failure.json',detail)
            raise BackendFailure(detail['stdout'][-2000:]) from exc
        raise
    save(folder/'provenance.json',dict(model=compiled_model.contents['provenance'],kernel=generated.provenance))
    return generated


def adapt_verified(generated,contract,folder):
    result=backend_api.convert_c_to_arduino(backend_api.ConvertCToArduinoRequest(
        c_code=generated.contents['ProgramPage1.c'],interface_contract=contract,
        compiled_kernel=backend_api.CompiledKernel(header=generated.contents['define.h'],source=generated.contents['ProgramPage1.c']),
        provenance=generated.provenance,out_path=str(folder/'arduino.ino')))
    save(folder/'arduino_provenance.json',result.provenance)
    return result.arduino_code


def evaluate(case, repeat, root, model):
    folder = root/case['id']/f'repeat_{repeat}'
    rows = []
    requirement = case['requirement']+'\n'+INTERPRETATIONS[case['id']]
    def record(arm, calls, **outcome):
        row = dict(case=case['id'], repeat=repeat, arm=arm, compile_ok=False,
                   functional_status='not_generated_or_compile_failed', **metrics(calls.entries))
        row.update(outcome); rows.append(row); save(folder/'results.json', rows)
    direct = BudgetCalls(folder/'direct_calls', model)
    try:
        _, raw = direct(DIRECT_PROMPT+requirement)
        code = extract(raw)
        result, log = compiled(case, folder/'direct/attempt_0', code)
        record('direct', direct, **result)
        attempt = 0
        while not result['compile_ok'] and len(direct.entries)<6:
            attempt += 1
            _, raw = direct(DIRECT_PROMPT+requirement+'\nPrevious sketch:\n'+code+'\nRepair compiler diagnostics without dropping behavior:\n'+log[-10000:])
            code=extract(raw)
            result, log=compiled(case,folder/f'direct_feedback/attempt_{attempt}',code)
        record('direct_feedback', direct, **result)
    except RuntimeError as exc:
        for arm in ('direct','direct_feedback'):
            if arm not in {r['arm'] for r in rows}:
                record(arm,direct,infrastructure_error=str(exc),functional_status='unverified')
    workflow=BudgetCalls(folder/'workflow_calls',model)
    stage='interface_contract'
    backend_evidence={}
    try:
        contract, report=plan_interface_contract(workflow,requirement,{},max_repairs=2,target_board=case['fqbn'])
        save(folder/'contract.json',contract);save(folder/'contract_planning.json',report)
        stage='sysml_generation'
        accepted={}
        def validator(source):
            checked=validate_executable_sysml(source)
            if checked['ok']: accepted['ir']=checked['ir']
            return checked
        _,bundle,validation=generate_validated_bundle(workflow,sysml_bundle_prompt(requirement,contract=contract),
            max_repairs=min(2,5-len(workflow.entries)),validator=validator,contract=contract)
        save(folder/'model.json',bundle);save(folder/'model_validation.json',validation)
        stage='ir_to_code'
        generated=lower_verified(bundle['system_sysml2'],contract,accepted['ir'],folder/'lowered')
        backend_evidence={'backend_ok':True}
        stage='hardware_adapter'
        code=adapt_verified(generated,contract,folder/'lowered')
        stage='arduino_compile'
        result,_=compiled(case,folder/'workflow/attempt_0',code)
        record('workflow',workflow,model_ok=True,contract_sha256=report['contract_sha256'],
               provenance_status='verified_model_chain',**backend_evidence,**result)
    except (ContractPlanningError,SysMLValidationError) as exc:
        save(folder/(stage+'_failures.json'),{'attempts':exc.attempts})
        record('workflow',workflow,failure_stage=stage,error=str(exc))
    except BackendFailure as exc:
        record('workflow',workflow,failure_stage='ir_to_code',error=str(exc),backend_ok=False)
    except RuntimeError as exc:
        record('workflow',workflow,failure_stage=stage,infrastructure_error=str(exc),functional_status='unverified',**backend_evidence)
    except ValueError as exc:
        record('workflow',workflow,failure_stage=stage,error=str(exc),**backend_evidence)
    except backend_api.HTTPException as exc:
        if exc.status_code in (400,422):
            record('workflow',workflow,failure_stage=stage,error=str(exc.detail),**backend_evidence)
        else:
            record('workflow',workflow,failure_stage=stage,infrastructure_error=str(exc.detail),functional_status='unverified',**backend_evidence)
    return rows


def main():
    os.chdir(ROOT)
    parser=argparse.ArgumentParser()
    parser.add_argument('--out',required=True)
    parser.add_argument('--case',action='append',choices=list(INTERPRETATIONS))
    parser.add_argument('--repetitions',type=int,default=3)
    parser.add_argument('--model',default=os.getenv('OPENAI_MODEL','openai/gpt-5.4-nano'))
    args=parser.parse_args()
    if args.repetitions<1: parser.error('repetitions must be positive')
    root=(ROOT/args.out).resolve();root.relative_to((ROOT/'experiment/output').resolve())
    root.mkdir(parents=True,exist_ok=False)
    manifest=json.loads((ROOT/'experiment/formal/manifest.json').read_text(encoding='utf-8'))
    selected=args.case or list(INTERPRETATIONS)
    cases=[c for c in manifest['cases'] if c['id'] in selected and c['split']=='development']
    if len(cases)!=len(set(selected)): raise ValueError('Only development tasks allowed')
    sources=[Path(__file__),ROOT/'app/main.py',ROOT/'tools/sysml/SysMLBridge.java',
             ROOT/'experiment/code/run_controlled_experiment.py',ROOT/'experiment/code/run_formal_experiment.py',
             ROOT/'experiment/code/experiment_metrics.py',ROOT/'experiment/code/method_snapshot.py',
             ROOT/'experiment/development/oracles/gpio_arduino.h',ROOT/'experiment/development/oracles/gpio_trace.cpp',ROOT/'experiment/development/oracles/TM1637Display.h',ROOT/'experiment/development/oracles/Servo.h',ROOT/'experiment/development/oracles/DHT.h',ROOT/'experiment/development/oracles/mixed_environment_trace.h',ROOT/'experiment/development/oracles/ArduinoHooks.cpp']
    sources+=list((ROOT/'app/services').glob('*.py'))+list((ROOT/'tools/prompts').glob('*.md'))
    hashes={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources}
    for source in sources:
        target=root/'method_snapshot'/source.relative_to(ROOT);target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(source,target)
    save(root/'execution.json',{'cases':cases,'public_interpretations':INTERPRETATIONS,'repetitions':args.repetitions,
        'model':args.model,'call_limits':{'direct':1,'direct_feedback':6,'workflow':6},'hashes':hashes,'runtime':runtime_identity(),
        'oracle_scope':'Host execution of Arduino-preprocessed sketches; not physical hardware or formal non-inferiority',
        'pipeline_scope':'Requirement to contract, SysML, IR, IL, C, deterministic adapter and target compilation. Optional UI module-diagram planning is not called or costed.'})
    rows=[];started=time.monotonic()
    for repeat in range(1,args.repetitions+1):
        for case in cases:
            rows+=evaluate(case,repeat,root,args.model)
            save(root/'results.json',rows)
            print(case['id'],repeat,[(r['arm'],r['compile_ok'],r['functional_status']) for r in rows[-3:]],flush=True)
    planned=len(cases)*args.repetitions
    summary={'planned_per_arm':planned,'seconds':time.monotonic()-started,'scope':'development hardware-command diagnostic; no population guarantee',
             'arms':{arm:{'compile_passes':sum(r['compile_ok'] for r in rows if r['arm']==arm),
                          'functional':functional_rates([r for r in rows if r['arm']==arm])} for arm in ('direct','direct_feedback','workflow')},
             'paired_functional':paired(rows,'workflow','direct','functional')}
    save(root/'summary.json',summary)
    if any(hashlib.sha256((ROOT/name).read_bytes()).hexdigest()!=digest for name,digest in hashes.items()):
        raise RuntimeError('Method changed during the run; results must not be pooled')


if __name__=='__main__': main()
