"""Merge this round with the previous 296-case ledger, using reviewed core evidence."""
from collections import Counter
from pathlib import Path
import json
from run_replays import OUT, PREVIOUS, ROOT, host, read

# Each entry was checked against the frozen requirement and the cited replay.
CHECKS = {
    'pceval_smart_led_system': ('unapproved_draft','core_fail','多组输入下，对应 LED 在 ADC 低于 500 时亮、高于 500 时灭，方向与题目指定的 ADC >500 点亮相反。'),
    'embedbench_15': ('native','core_fail','19 秒内显示一直为 0，未按每秒加 6 推进。'),
    'embedbench_27': ('native','core_fail','21 秒内三路舵机均没有转到任何非零目标，条形灯也始终不亮。'),
    'embedbench_28': ('native','core_fail','近 10 秒内三轴没有执行依次到 180° 的动作，灯条没有出现 3、6、10 段序列。'),
    'embedbench_34': ('native','core_fail','31 秒内显示始终为 0，三路舵机保持初值，未执行计数与联动。'),
    'embedbench_47': ('native','core_fail','两路电位器分别改变后两盏 RGB 灯仍全部关闭，没有按旋钮生成颜色。'),
    'embedbench_53': ('native','core_fail','43 秒内三路舵机没有改变目标，RGB 和灯条保持关闭，未执行联动。'),
    'embedbench_56': ('native','core_fail','超过 30 秒只出现初始红色和显示 0，未按两秒切换灯色和数字。'),
    'embedbench_57': ('native','core_fail','23 秒内两位显示始终为 99，灯色也没有按序轮换。'),
    'embedbench_58': ('native','core_fail','9 秒内只显示 9、舵机目标保持 0°，未执行倒计时和舵机联动。'),
    'embedbench_59': ('native','core_fail','28 秒内数码管始终显示 9，没有倒计时；RGB 也未按序切换。'),
    'embedbench_62': ('native','core_fail','计数约每 10 ms 推进一次，无法实现两秒一个数字。'),
    'embedbench_81': ('native','core_fail','有实际按钮刺激的轨迹中显示始终 00，+1 和 +5 均未生效。'),
    'embedbench_88': ('native','core_fail','20 秒内执行加减按键操作后显示仍为 00，未实现递增、递减或回绕。'),
    'embedbench_90': ('native','core_fail','在 26 秒的回放中未出现要求的第一路舵机 30° 步进，也未开始同步灯序列。'),
    'embedbench_98': ('native','core_fail','超过一分钟的虚拟时间内舵机未执行 10° 步进，输出角度保持 0°。'),
    'embedbench_109': ('native','core_fail','13 秒内两路舵机没有执行 0–180° 扫动，关联灯也没有随端点切换。'),

    'pceval_7_segment_display_basic': ('previously_tested','core_fail','共阳数码管的固定段码不能解码为要求的 5。用于 5 的 a 段被输出为高电平，实际熄灭。'),
    'pceval_7_segment_display_counter': ('previously_tested','core_fail','共阳数码管没有按秒显示 1、2 等，主要输出不可解码的段码，计数变化也停滞。'),
    'pceval_7_segment_display_serial': ('previously_tested','core_fail','串口已收到并回显数字，屏幕在多条有效指令之后仍显示 8。'),
    'pceval_button_duration': ('previously_tested','core_fail','给定接线上执行 4 秒和 12 秒按键，串口没有输出任何时长。'),
    'pceval_button_led': ('previously_tested','core_fail','三次按下和释放都得到相反 LED 状态，松开时亮、按下时灭。'),
    'pceval_parking_spot_monitor': ('unapproved_draft','core_fail','近距离应显示红灯，D4 实际关闭，D2 蓝灯反而点亮。参考通过了同一测试。'),
    'pceval_potentiometer_bar_led': ('unapproved_draft','core_fail','ADC=0 时点亮前多段，违反零输入对应 0 段及比例点亮的核心行为。'),
    'pceval_multiplexer_photoresistor': ('mux','core_fail','各通道施加独立模拟值后，只得到无通道标识的混杂读数，而且漏掉部分通道、读取未连接通道。'),
    'pceval_multiplexer_potentiometer': ('mux','core_fail','五路独立刺激下，第 0 路实为 113 却报 0，第 1 路实为 245 却报 113，存在通道和值错配。'),
    'embedbench_7': ('repair','core_fail','避开相等上下限造成的除零后，三档旋钮亮度仍明显错误：预期约 63/127/191，实际全部饱和。'),

    'embedbench_22': ('extension','core_pass','三路电位器独立影响对应 RGB 通道，0、中间值、满量程及混色测试通过。'),
    'embedbench_31': ('extension','core_pass','分别改变两路电位器时前两舵机独立响应，第三舵机和条形灯跟随两路平均值。'),
    'embedbench_32': ('extension','core_pass','在 0、50%、100% 及返回场景，两舵机反向同步，条形灯对应点亮 0、5、10 段。'),
    'embedbench_39': ('extension','core_pass','每两秒数码管与条形灯同步显示 0 至 9，随后归零并开始下一轮。'),
    'embedbench_45': ('extension','core_pass','两轮七色时序及每色对应的三舵机命令均通过。'),
    'embedbench_46': ('extension','core_pass','两轮七色时序和三舵机对应角度通过。对超过 180 度的指令使用题目允许的 180 度。'),
    'embedbench_68': ('targeted','core_fail','按住按钮旋转电位器时对应 PWM 不更新。松开后跳到另一极值，无法保留调节值。'),
    'embedbench_69': ('targeted','core_fail','初始点亮绿色，第一次按键选中蓝色，后续次序也与红→绿→蓝不符。'),
    'embedbench_72': ('targeted','core_fail','第一次按键前已亮一段，第十次按键时提前清零，无法依次得到 0 到 10 段。'),
    'embedbench_92': ('targeted','core_pass','第一路改变 PWM 亮度，第二路能使输出越过阈值后关闭。采用两路电位器在同一量程比较。'),
    'pceval_led_RGB_basic': ('targeted','core_fail','共阳接线要求 D4/D3/D2 为 1/1/0，持续观察到 0/0/1。'),
    'embedbench_29': ('followup','core_fail','两个中间输入场景中三个区段过早全亮。源码将原始 ADC 和 1–4 的段数阈值直接比较。'),
    'embedbench_44': ('followup','core_fail','ADC 为 365 和 725 时均显示 99 且灯条全幅，参考同两组输入分别显示 35 和 70。'),
    'embedbench_100': ('followup','core_fail','第三电位器在中间输入时条形灯过早全亮，未按 0–1023 比例映射到 0–10 段。'),
    'embedbench_104': ('followup','core_pass','实际显示 00、35、70、99、00，偶数/奇数分别选对 LED，所选灯 PWM 跟随对应电位器。参考代码对部分样本用满亮，按题目给定的 PWM 规则判定。'),
    'embedbench_121': ('followup','core_pass','三次独立按键分别锁存对应电位器的非零 PWM，松开并改变输入后保留亮度，其他灯不变。'),
    'medium_023': ('followup','core_pass','TM1637 显示 15、35、48、15，D5 在高温开启、降温关闭。显示引脚采用冻结测试配置 D4/D2。'),
    'simple_005': ('supplement','core_pass','D3 外部下拉接法下，三次独立按键后 D10 依次亮、灭、亮，松开后保持。'),
    'simple_007': ('supplement','core_pass','A1 在低/高读数间反复改变，D8 对应点亮/熄灭。'),
    'simple_011': ('supplement','core_fail','发送三条非零 PWM 命令后，等待下一条命令期间 D9 均回到 0，亮度未保持。'),
    'simple_013': ('supplement','core_fail','三段有效 DHT11 温度输入期间串口完全没有输出，未实现每秒上报。'),
    'simple_014': ('supplement','core_pass','三组 HC-SR04 距离输入变化时，串口分别报告相应距离。'),
    'simple_015': ('supplement','core_fail','5V ADC 参考下输入 51/82/36 对应约 25/40/18℃，输出使用原 ADC 计数，未换算为摄氏温度。'),
    'simple_016': ('supplement','core_pass','两组小于 20 cm 输入均点亮 D13，三组大于阈值的输入均熄灭。'),
    'simple_022': ('supplement','core_pass','D3 外部下拉接法下，两次长按 D6 均鸣叫，松开后均静音。'),
    'simple_023': ('supplement','core_pass','四组递增 ADC 输入使 D6 产生四个递增的音调频率。'),
    'simple_025': ('supplement','core_fail','第一次按钮动作后 D13 始终熄灭，没有进入闪烁模式。'),
    'medium_005': ('supplement','core_pass','两次 D3 外部下拉按住均鸣叫，松开后分别约 995 ms 停止。'),
    'medium_006': ('supplement','core_fail','ADC 满量程只显示 1023，未映射到题目规定的 9999。'),
    'medium_014': ('supplement','core_fail','三次 BUZZ 命令都没有使 D6 发声，无法完成 500 ms 提示。'),
    'medium_021': ('supplement','core_pass','D9 和 D6 在 ADC 701、800 时同时开启，700 及低于 700 时同时关闭。'),
    'medium_025': ('supplement','core_fail','D9 只有开关两个级别，未根据 A0 实现亮度调节。'),
    'embedbench_1': ('native','core_fail','11 秒时 D2 和 D3 均低，第二盏灯没有按要求在第 10 秒点亮。'),
    'embedbench_2': ('native','core_fail','多次按键的 RGB 轨迹未按要求得到红、绿、蓝和熄灭。'),
    'embedbench_4': ('native','core_fail','三路输出一直关闭，RGB 没有随时间进入下一颜色。'),
    'embedbench_5': ('native','core_fail','接地按钮两次动作后灯状态与应有的切换状态相反。输入脚没有按给定接线启用上拉。'),
    'embedbench_17': ('native','core_fail','第二状态的蓝灯及 RGB 蓝通道仍不亮，程序停在初始红色状态。'),
    'embedbench_19': ('native','core_fail','三次采样均偏离要求的 1 秒亮、2 秒灭、3 秒亮顺序，源码无法进入 3 秒阶段。'),
    'embedbench_23': ('unapproved_draft','core_pass','按原题八个 ADC 区间及边界采样，60 条 RGB 输出断言通过，参考程序也通过。'),
    'embedbench_30': ('native','core_fail','输出在被选范围外点亮条形灯，未按电位器设置控制填充区间。'),
    'embedbench_33': ('native','core_fail','数码管十位与个位接反，计数 01 实际显示 10。'),
    'embedbench_35': ('native','core_fail','最大电位器输入时两位数码管显示 00，原要求为 99。'),
    'embedbench_40': ('native','core_fail','计数随约 10 ms 控制步推进，未实现每两秒增加一位。'),
    'embedbench_43': ('native','core_fail','数码管保持为 0，无所要求的计数和可调递增行为。'),
    'embedbench_50': ('native','core_fail','第一个灯段持续亮，未按两秒推进条形灯段和对应输出。'),
    'embedbench_118': ('native','core_fail','两次上调按钮后舵机仍停在 90°，未到达 120°、150°，关联 LED 未亮。'),
    'embedbench_122': ('native','core_fail','按键后没有锁存对应二进制位，LED 在多组组合输入中仍关闭。'),
    'advanced_006': ('unapproved_draft','core_fail','第二次按键后 D10 开启，超过题目规定的 5 秒仍无结束边沿。'),
    'advanced_020': ('unapproved_draft','core_fail','载荷输入两次从 0 变为 1023，显示始终为 1，载荷百分比没有呈现。'),
    'complex_011': ('previously_tested','core_fail','暗且近时报警触发，但 D13 只从灭变亮一次，D6 随后一直输出 2000 Hz，没有所要求的闪烁和间歇。'),
    'complex_030': ('previously_tested','core_fail','回放中数码管会切换，但串口没有任何三项传感器数据，未实现每两秒上报。'),
}

def read_run(case,kind,runs):
    for run in runs:
        if run['kind']==kind and run.get('result'):
            result=read(Path(run['result']))
            if result.get('execution',{}).get('status') in ('completed','capture-end') and result.get('process_exit_code')==0:
                return run,result
    return None,None

def trace(run):
    path=Path(run['result']).with_name('trace.jsonl')
    return [json.loads(line) for line in path.read_text(encoding='utf-8').splitlines()]

def trace_verdict(case,runs):
    pceval = {
        'pceval_button_buzzer': ('previously_tested','core_pass','三次按下 D9 均发出 262 Hz，松开均静音。内部上拉与给定的外部上拉可并存。'),
        'pceval_button_servo_motor': ('previously_tested','core_pass','两次按下舵机 180°，释放时 90°，同时上报正确角度。内部上拉不改变接地按键的有效电平。'),
        'pceval_led_blink_basic': ('previously_tested','core_pass','D13 按 1 秒亮、1 秒灭重复。开机提示语缺失，按核心闪烁行为判定。'),
        'pceval_photoresistor_basic': ('previously_tested','core_pass','A2 的八个场景均报告正确数值。标签缺冒号且报告快于 500 ms，按核心测光行为判定。'),
        'pceval_distance_sensor_basic': ('repair','core_pass','通过给定 TRIG/ECHO，三组输入 34、80、120 cm 均被正确测得并报告。'),
        'pceval_button_pulldown': ('repair','core_fail','D4 按给定下拉接线在高低间切换四段，串口没有输出任何按键状态。'),
        'pceval_button_pullup': ('repair','core_fail','接线按下拉低时输出 button_pressed0，释放时输出 button_pressed1，报告的按下状态颠倒。'),
        'pceval_humidity_sensor': ('repair','core_fail','湿度低、高、低三段有效输入期间，未向串口报告任一阈值判断结果。'),
        'pceval_humidity_sensor_basic': ('repair','core_fail','输入湿度 30%、66%、45% 时，串口实际始终报告 0。'),
        'pceval_temperature_sensor': ('repair','core_fail','38℃ 后降回 26℃，串口仍持续报告高于阈值，不能反映当前温度。'),
    }
    if case=='pceval_4_digit_7_segment_display':
        run,raw=read_run(case,'previously_tested',runs)
        if run:
            gpio=sorted((e for e in trace(run) if e['type']=='gpio'),key=lambda e:(e['at_us'],e['sequence']))
            state={pin:0 for pin in range(2,14)}
            dwell={pin:Counter() for pin in range(10,14)}
            for first,second in zip(gpio,gpio[1:]):
                state[first['pin']]=int(bool(first['value']))
                dt=second['at_us']-first['at_us']
                active=[pin for pin in range(10,14) if state[pin]]
                if dt>=200 and len(active)==1:
                    mask=sum((1-state[pin])<<(pin-2) for pin in range(2,9))
                    dwell[active[0]][mask]+=dt
            masks=[dwell[pin].most_common(1)[0][0] for pin in range(10,14)]
            assert masks==[122,9,56,15]
            return run,'core_fail','按给定共阳接线解码，四位主要有效段码为 122/9/56/15，未显示要求的 1/2/3/4。'
    if case=='pceval_LCD_display_basic':
        run,raw=read_run(case,'previously_tested',runs)
        if run:
            events=trace(run)
            assert not any(e['type']=='lcd' for e in events)
            assert len([e for e in events if e['type']=='gpio' and e['pin']==13])==1
            return run,'core_fail','程序没有发出 LCD 文本调用，指定的 SPI 时钟 D13 只有初始化电平，未发生可传输屏幕数据的时钟变化。'
    if case=='pceval_servo_motor_basic':
        run,raw=read_run(case,'unapproved_draft',runs)
        if run:
            events=trace(run)
            moves=[e['angle'] for e in events if e['type']=='servo' and e.get('operation')=='write']
            messages={e['text'] for e in events if e['type']=='serial'}
            assert moves.count(0)>2 and moves.count(180)>2 and len(set(moves))>170
            assert {'angle_deg0','angle_deg180'}<=messages
            return run,'core_pass','D9 以约 1° 步长完成多轮 0–180° 来回扫描，串口给出正确端点角度。忽略附加角度标签及中间值。'
    if case=='pceval_distance_sensor':
        run,raw=read_run(case,'distance_retry',runs)
        if run:
            messages={e['text'] for e in trace(run) if e['type']=='serial'}
            if len(messages)<=1:
                return run,'core_fail','在有效距离范围内分别提供偏小、相等和偏大目标，串口没有给出可区分的比较结果。'
    if case in pceval:
        kind,verdict,reason=pceval[case]
        run,raw=read_run(case,kind,runs)
        if run:
            return run,verdict,reason
    if case=='embedbench_11':
        run,raw=read_run(case,'extension',runs)
        if run:
            frames=[e['text'] for e in trace(run) if e['type']=='display']
            assert frames==['0']
            return run,'core_fail','14 秒内输出只有启动时的 0；冻结代码的段码输出也只有 0，没有 A/P 或对应定时。'
    kind = 'supplement' if case=='simple_026' else 'targeted'
    run,result=read_run(case,kind,runs)
    if not run:
        return None
    events=trace(run)
    if case=='medium_002':
        alarm=[(e['at_ms'],e['frequency']>0) for e in events if e['type']=='tone' and e['pin']==6]
        assert [on for _,on in alarm]==[False,True,False,True,False,True,False] and result['status']=='passed'
        return run,'core_pass','D13 和 D6 均通过三组靠近、远离变化。20 cm 边界关闭，19 cm 亮灯并鸣叫。'
    if case=='medium_009':
        displays=[(e['at_ms'],int(e['text'])) for e in events if e['type']=='display' and e['name']=='tm:4:5'][1:]
        assert len(displays)==9
        assert all(1950<=b[0]-a[0]<=2050 for a,b in zip(displays,displays[1:]))
        assert all(v==10+(t//500)%29 for t,v in displays)
        return run,'core_pass','用每 500 ms 变化的温度输入观察到 9 个正确显示值，相邻刷新约 2001 ms。'
    if case=='medium_011':
        pwm=[e for e in events if e['type']=='gpio' and e['pin']==9 and 13000<=e['at_ms']<23000]
        assert len(pwm)==1 and pwm[0]['value']==0
        return run,'core_fail','首次 MODE1 只在开机前 6 秒渐变。第 13 秒再次输入 MODE1 后 D9 降为 0 并保持，无法再次渐变。'
    if case=='complex_026':
        values={e['text'].strip() for e in events if e['type']=='display' and 24100<=e['at_ms']<=44000}
        assert values=={'21'}
        return run,'core_fail','BOTH 命令后的近 20 秒内数码管只显示温度 21，未轮流显示同时输入的湿度 62。'
    if case=='simple_026':
        pwm=[e['value'] for e in events if e['type']=='gpio' and e['pin']==9 and e['pwm']]
        assert pwm.count(255)>=2 and pwm.count(0)>=2
        assert len(set(pwm))>=20
        return run,'core_pass','记录到多个 0 至 255 的完整渐亮周期，达到上限后回到 0 并重复。'
    return None

def case_runs(case):
    case_file=OUT/'cases'/f'{case}.json'
    row=read(case_file) if case_file.exists() else dict(case=case,runs=[])
    runs={r['kind']:r for r in row['runs']}
    for prefix in ('targeted','followup','supplement','extension','repair','mux','distance_retry'):
        p=OUT/f'{prefix}_cases'/f'{case}.json'
        if p.exists():
            runs[prefix]=read(p)
    row['runs']=list(runs.values())
    return row

notes = {
    'simple_004':'D9 PWM 在 ADC=0 时为 0，ADC=256、512、768、1023 时均为 255。源码在低四分之一有调光，原题未指定完整量程映射，暂保留。',
    'medium_017':'D2 高电平期间亮、低电平期间熄灭，松开立即恢复。是否要求按一次锁存状态，以及 A0 亮度范围，原题没有明确。',
    'complex_008':'测距和 LED 通过。D6 在 14 cm、7 cm 时分别连续输出 140 Hz、220 Hz，没有间歇。慢鸣、急鸣应按节奏还是音调判定尚有歧义。',
    'embedbench_14':'计数 10 时空白一秒。单个数码管如何表示恰好 10 未明确，原生断言也未覆盖该时刻。',
}

def review():
    rows=read(PREVIOUS/'results.json')
    by_id={r['id']:r for r in read(ROOT/'experiment/formal/manifest.json')['cases']}
    new=[]
    for row in rows:
        if row['core_status']!='core_unverified':
            continue
        case=row['case']
        audit=case_runs(case)
        audit['dataset']=row['dataset']
        audit['requirement_source']=by_id[case].get('source')
        audit['requirement']=by_id[case]['requirement'].split('Given circuit')[0].strip()
        audit['circuit_status']='本轮未执行 SimulIDE 电路级验证'
        runs=audit['runs']
        decided=trace_verdict(case,runs)
        if case in CHECKS:
            kind,verdict,why=CHECKS[case]
            run,raw=read_run(case,kind,runs)
            expected='passed' if verdict=='core_pass' else 'failed'
            if run and raw['status']==expected:
                decided=(run,verdict,why)
        if decided:
            run,verdict,why=decided
            audit.update(verdict=verdict,reason=why,evidence=run['result'],evidence_type='host_replay')
            row.update(core_status=verdict,evidence=run['result'])
            row.pop('verification_gap',None)
            if verdict=='core_fail':
                row['failure_reason']=dict(stage='core_execution',diagnostic=why,
                    evidence=run['result'])
        else:
            execution=any(r.get('execution',{}).get('status') in ('completed','capture-end') for r in runs if r.get('execution'))
            if case in notes:
                reason=notes[case]
            elif execution:
                statuses={r['status'] for r in runs}
                if 'failed' in statuses and any(r.get('reference_status') not in (None,'passed') for r in runs):
                    reason='候选至少有一条断言失败，且参考程序未通过至少一套使用中的方案。需核对具体断言、输入接线与期望。'
                elif 'failed' in statuses:
                    reason='候选有断言失败，是否构成核心功能缺陷仍需结合原需求及实际轨迹确认。'
                elif 'passed' in statuses:
                    reason='现有可执行断言通过，尚缺足以覆盖核心行为的正向证据。'
                else:
                    reason='已获得轨迹，但没有独立可靠的核心功能断言。'
                audit['assertion_evidence']=[]
                for run in runs:
                    if not run.get('result'):
                        continue
                    raw=read(Path(run['result']))
                    bad=[dict(id=c['id'],requirement=c['requirement_quote'],actual=c['actual'],test=c['test'])
                         for c in raw.get('checks',[]) if not c['passed']]
                    if bad:
                        audit['assertion_evidence'].append(dict(plan=run['plan'],result=run['result'],
                            reference_status=run.get('reference_status'),failed=len(bad),examples=bad[:3]))
            elif runs:
                reason='已尝试回放，宿主机执行或接口观察未完成。具体状态见 runs。'
            else:
                reason=audit.get('error','回放尚未返回可执行方案或结果。')
            audit.update(verdict='core_unverified',reason=reason)
            row['verification_gap']=reason
        if audit.get('prior_static_review'):
            d=read(Path(audit['prior_static_review']))
            audit['prior_static_judgment']=d['verdict']
            audit['prior_static_reason']=d['reason']
        new.append(audit)
    assert len(new)==217 and len(rows)==296
    def counts(rs,key):
        return Counter(r[key] for r in rs)
    summary=dict(total=296,pending_batch_total=217,
        batch_recorded=sum((OUT/'cases'/f"{r['case']}.json").exists() for r in new),
        batch_attempted=sum(bool(r['runs']) for r in new),
        batch_executed=sum(any((x.get('execution') or {}).get('status') in ('completed','capture-end')
                               for x in r['runs']) for r in new),
        batch_verdicts=counts(new,'verdict'),total_verdicts=counts(rows,'core_status'),
        datasets={ds:counts([r for r in rows if r['dataset']==ds],'core_status')
                  for ds in sorted({r['dataset'] for r in rows})},
        batch_datasets={ds:counts([r for r in new if r['dataset']==ds],'verdict')
                        for ds in sorted({r['dataset'] for r in new})},
        circuit_simulations_this_batch=0,
        attempts_by_status=Counter(x['status'] for r in new for x in r['runs']))
    host.save(OUT/'review_217.json',new)
    host.save(OUT/'results_296.json',rows)
    host.save(OUT/'failure_reasons.json',[dict(case=r['case'],dataset=r['dataset'],status=r['core_status'],
        evidence=r['evidence'],reason=r['failure_reason']) for r in rows if r.get('failure_reason')])
    host.save(OUT/'summary.json',summary)
    print(json.dumps(summary,ensure_ascii=False,indent=2))

if __name__=='__main__':
    review()

