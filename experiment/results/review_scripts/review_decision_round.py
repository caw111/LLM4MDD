"""Decisions from the original requirements, frozen code, and checked trace intervals."""
from collections import Counter
from pathlib import Path
from run_replays import OUT, host, read

DECISIONS = {}
def decisions(kind,verdict,entries):
    for case,why in entries.items():
        assert case not in DECISIONS
        DECISIONS[case]=kind,verdict,why

decisions('decision','core_pass',{
    'simple_004':'A0 为 0/60/128/200/255 时 D9 输出对应五级 PWM，回到 0 后熄灭。ADC 大于 255 时饱和，原题未规定全量程线性映射。',
    'simple_029':'模拟读数从 768 增至 850、950、1023 时，PWM 从 255 降为 173、73、0。原题未给全量程映射，低段饱和按基本调光判定。',
    'medium_029':'D2 外接下拉时 3 秒处鸣叫，5 秒按键后暂停，12 秒恢复后在 13、16、19 秒再次鸣叫。题目正文未限定按键极性。',
    'complex_005':'D2 外接下拉时第一次按键后显示维持 100，虽然 A0 先后变为 200、140。第二次按键后显示恢复更新，随后显示 140、220、90。',
    'complex_017':'D2 外接下拉时 4.233 秒静音并点亮 LED，10.024 秒再次按键后恢复鸣叫并关闭 LED。静音期间仍输出测距记录，恢复后远近对应不同频率。',
    'embedbench_63':'每 2 秒计数并从 9 回到 0。条形段数逐级跟随，计数 0–3、4–6、7–9 分别选择红、绿、蓝。',
})
decisions('decision','core_fail',{
    'advanced_004':'42 秒回放中输入三个时间并操作按钮，显示始终 0000。冻结程序的时间输入仅识别固定符号，蜂鸣输出恒零，漏服计数自赋值。',
    'advanced_007':'门打开 16 秒，A1 同时从 10 升至 400，制冷和蜂鸣输出保持关闭，显示始终 0。源码中开门计时量没有递增。',
    'advanced_008':'35 秒内操作时间设置、换阶段和开始按钮，倒计时显示仍为 0，蜂鸣器、完成灯及串口无阶段反馈。内核每步将完成状态写为假。',
    'advanced_021':'三个传感器输入均为非零并且变化，D2 多次切换期间显示始终 0。冻结代码多个显示页均写入常量。',
    'advanced_025':'85 秒内经历多次设置按键及升温，显示在启动后停在第 2 阶段，剩余时间没有倒计时。源码的剩余时间只自赋值或写 30000。',
    'complex_021':'有效温湿度、传感器故障和恢复三段输入中，显示始终 0，读数失效时没有 LED 闪烁或串口报警。',
    'complex_025':'进入手动模式后，A0 由 20 变为 180、500、900，阈值显示一直为 7，直到退出手动模式才更新。',
    'complex_028':'进入高速档后蜂鸣器从 13 秒持续发声到 28 秒退出高速，无法给出题目要求的短鸣。',
    'medium_016':'依次切换各个模式后 D13 只在按键动作附近发生电平变化，最长 10 秒维持常亮，代码中两个模式输出同一常量，没有闪烁定时。',
    'medium_020':'四次切换能观察到电位器读数与 1234，但计数页在停留时持续显示 0，源码将计数分支固定为零。',
    'simple_020':'更换为能正确触发的 D2 高电平接线后，两次按键均显示 1111，但释放后的四位显示是三个空白加一个 0。题目明确指定四位 0000。',
    'embedbench_10':'A3 刚到 512 十段已全亮，增加至 1023 段数不再增长。反向时更少一段，不能维持由电位器决定的点亮数量。',
    'embedbench_36':'用正确范围内的 A3/A4 输入替换旧方案中的异常 ADC。两路均为 1023 时应显示 18 的个位 8，实际清屏。',
    'embedbench_38':'两路独立改变时舵机 1 约 12、49、81 度，按接线解码的十位与个位显示为 21、94、18。',
    'embedbench_48':'用实际接线输入后舵机 1、2 均正确变化。舵机 3 在 14.5 秒内一直保持 0 度，缺少 2 秒交替到 180 度。',
    'embedbench_54':'用电位器在多个 5 秒周期内变化，输出只有红色通道变化，绿蓝一直为 0，无法进入其他模式。',
    'embedbench_55':'两路 A3/A4 均为 512 时，原题示例要求 4 段，实际 8 段同时点亮。',
    'embedbench_65':'给接地按钮输入 8 个持续 350 ms 的完整按下与释放周期，RGB 始终灭，没有任何颜色切换。',
    'embedbench_70':'三路电位器预置不同非零 ADC 后，分别操作三个按钮，舵机始终停在 0 度。',
    'embedbench_71':'首次按键前，舵机 1 和 2 已移至 90 和约 44 度。题目要求保持 0 度，直到各自的按钮触发。',
    'embedbench_75':'两路 ADC 不同，连续四次有效按键仍只跟踪第一个电位器，灯段仅在 A3 变化时改变。',
    'embedbench_84':'多个有效按钮动作后，RGB 仍回到同一通道，未按模式顺序改变颜色。',
    'embedbench_89':'十次按键后计数达到 10 时数码管清屏，原题要求显示个位 0，同时按实际计数选择灯色。',
    'embedbench_99':'ADC 512 时亮度已饱和到 255，ADC 1023 时条形灯末段始终未点亮。该题明确要求亮度成比例且满量程亮十段。',
    'embedbench_105':'显示 49 时只亮 LED2，显示 99 时只亮 LED3。原题要求随区间增加累计点亮两盏和三盏。',
    'embedbench_111':'低区间的 ADC=250 时红通道 PWM 只有 62，中区间的 512 时绿通道只有 127，原题要求对应主色满亮。',
})
decisions('previously_tested','core_fail',{
    'advanced_019':'已有回放产生危险提示后立即恢复，串口没有危险持续时间。冻结代码覆盖前一状态再检测结束边沿，持续时间本身也不累加。',
    'advanced_022':'占用提示在回放中能触发，但到期告警路径需要结束时间同时大于和不大于开始时间，条件不可成立，且未接入实际预约时间进度。',
    'advanced_023':'回放确认可报警并输出温度等字段，冻结程序将 GPS 数据保存到状态后未纳入事件输出，缺失带位置的报警日志。',
    'advanced_024':'现有回放停在满载状态。冻结内核在检查复位边沿前先把当前 D7 写成前次值，清空后复位无法触发。',
    'advanced_028':'多床重叠呼叫的确认回放中缺少后续两床的处理日志。内核用单一活动标志保存呼叫，确认时直接清空，没有待处理队列。',
    'complex_001':'高温持续期间 8.088 秒启动鸣叫、8.576 秒停止，未持续循环；冻结代码只累加报警相位，未将其回绕至下一次鸣叫。',
    'complex_014':'输入先升后降再按键，显示仍为当前值 0。源码没有把历史最高显示状态置真，无法进入三秒峰值显示。',
    'complex_016':'读入湿度 35、65 时显示 350、650，驱动按无小数点整数显示，实际菜单数值缩放了十倍。',
    'complex_023':'两次距离 350 cm 到 5 cm 的输入变化能更新距离显示，PWM 输出仍未变化。距离到亮度结果落在 0–1，送至 8 位 PWM 时被截为零。',
    'complex_029':'已观察到短按 D3 可以改计数。源码锁定分支也只检测一次 D3 边沿，没有区分短按与长按，且偶数灯条件退化为计数等于 2。',
    'medium_003':'按题面接线温度应从 A1 读取，但控制函数使用 A0，并直接把原始 ADC 与摄氏阈值 30 比较。低温样本实际仍开启 D5。',
    'medium_008':'D2 上拉未操作时上电即把计数从 0 加到 1，且按住未计数、释放才加。启动时已产生一次虚假计数。',
})
decisions('unapproved_draft','core_fail',{
    'advanced_009':'源码把回波有效直接作为有人事件，并在两路都有回波时重复恢复检测条件，无法可靠区分进出。风扇对非零人数输出固定值，缺少共同按人数和温度调速。',
    'advanced_030':'现有回放可报警，但产量达成率固定用累计值除以 600，没有按目标及经过时间计算，自动无产品停机也未计入停机次数。',
    'complex_006':'实际温度输入为 10 和 45 时，显示仅在 0 和 1 之间切换；蜂鸣器在越阈期间持续输出 1000 Hz，未周期鸣叫。',
    'complex_010':'实际温度等于 30℃ 时，风扇 PWM 已为 102。原文明确要求温度高于 30℃ 才启动。',
    'complex_015':'冻结内核将暗光和接近条件分别写为 ADC 非负及有效距离大于零。ADC 和正常距离都满足时始终开灯，缺少用于实现只在暗且近时开灯的判定。',
    'complex_020':'读数能报告，但 A1 ADC=102 时直接显示温度 102，未转换为 LM35 温度；同一秒内反复上报，未实现每秒一次。',
    'embedbench_13':'回放中可随机改变亮灯数量，但冻结代码按红、黄、绿固定顺序累加，无法产生黄单亮、绿单亮等组合，缺失独立随机选灯。',
})
decisions('archived_design','core_fail',{
    'advanced_010':'冻结程序没有串口接收补货路径，两个库存更新分支均为减法，无法完成核心管理员补货流程。',
    'advanced_014':'冻结程序先写入按钮前值，再在同一周期检测当前按下且前值未按，确认分支永远不可达，结束告警无法用按钮确认。',
    'advanced_015':'串口只会主动发出计数和水位，冻结程序没有串口接收处理，无法执行题目要求的统计查询。',
    'advanced_016':'数码管输出路径只取状态编号，未接入电流和温度页。已有回放确认能输出串口数值，不能替代题目要求的本地轮显。',
    'advanced_029':'两个温度 ADC 的合法上限都是 1023，但主要超温和结霜条件分别使用 3000、2000，告警状态在合法输入下不可达。',
    'complex_018':'设置风扇最低转速的串口命令只把目标变量写为 0，不读取设定值，不能完成原题的可配置自动调速。',
})
decisions('native','core_fail',{
    'embedbench_12':'未按键时数码管在 t=0 已从 0 变为 3，第一次操作后显示 6，计数存在一次虚假增量。',
    'embedbench_3':'没有按键时红绿蓝每约 10 ms 连续切换，要求每种颜色驻留 2 秒。',
    'embedbench_37':'A3 输入满量程 1023 后，数码管清屏而不是显示 9，冻结代码将满量程映射到了单字符无法显示的 10。',
    'embedbench_41':'两秒后计数应显示 01，按照原图的两位接线得到 10。后续数位仍对调。',
    'embedbench_51':'第一阶段输出维持约 10 ms 后立即跨至其他颜色和灯段，未按每阶段 2 秒推进。',
    'embedbench_60':'当输入使 RGB 红通道达到满亮时，负责显示红色级别的 sevseg1 保持空白。源码把未缩放的红绿原始分量拼接后越界。',
    'embedbench_76':'启动同一时刻，显示从 0 直接变成 6，两个舵机从 0 变为 90 和 180，未有按键事件。后续计数因此以错误基线继续。',
    'embedbench_80':'显示 2 时只亮第三段，显示 3 时只亮第四段。原题要求点亮对应数量的连续段。',
    'embedbench_82':'原接线回放中前两个按钮操作没有触发应有的舵机动作，第三个按钮只产生瞬态状态，未保持题设阶段。',
    'embedbench_83':'2.6 秒按键后 RGB 和舵机状态只保持到 2.8 秒释放，要求至少保持 2 秒。',
    'embedbench_85':'冻结程序的模拟输入没有用于计算 RGB 输出，三个通道通过数字开关驱动，无法实现电位器亮度调节。',
    'embedbench_86':'第一模式原图解码 RGB1 红蓝同时亮成为品红，RGB2 只有绿亮。与明确指定的红、蓝组合不符。',
    'embedbench_91':'两盏 LED 在 1、2、3 秒交替，驻留约 1 秒，未与要求的 2 秒舵机档位同步。',
    'embedbench_95':'D10、D11 上观察到独立灯交替输出，但原图要求这两个输出位于 D2、D3。生成程序把这些灯和条形段混接。',
    'embedbench_96':'进入要求条形全亮的阶段时只剩最后一个段输出为高，中间段仍关闭。',
    'embedbench_97':'多个模式在约 10 ms、20 ms 就相继出现，未按题意保持各阶段两秒。',
    'embedbench_102':'启动两秒后个位应为 1，实际 sevseg1 显示 1 而 sevseg2 仍显示 0，十位与个位对调。',
    'embedbench_103':'D2/D3 每 1 秒互换灯状态，题目要求每盏保持 2 秒。舵机命令也与四秒扫描轨迹不符。',
    'embedbench_106':'两只独立指示灯的原接线引脚 D2、D3 没有交替输出。条形灯第一段也被绑定到错误引脚。',
    'embedbench_108':'三灯应显示黄色及青色的两个阶段，蓝色位相反，实际色分别为白和绿。冻结代码的蓝通道条件包含了错误阶段。',
    'embedbench_115':'倒计时初始化后，sevseg1 立即空白，sevseg2 单独从 9 向下计数，未能显示原题两位倒计时值。',
    'embedbench_116':'在完整记录期间两位数码管始终 00，后续阶段应推进的数字及关联灯输出均未出现。',
    'embedbench_9':'第一次操作前条形灯已有两段点亮，操作后的点亮从 A1 一侧累计，题目明确从 A10 一侧开始。',
})

def resolve():
    prior=read(OUT/'review_217.json')
    totals=read(OUT/'results_296.json')
    by_case={r['case']:r for r in totals}
    pending=[r for r in prior if r['verdict']=='core_unverified']
    assert len(pending)==119 and len({r['case'] for r in pending})==119
    assert DECISIONS.keys() <= {r['case'] for r in pending}
    for r in pending:
        case=r['case']
        row=by_case[case]
        decision_path=OUT/'decision_cases'/f'{case}.json'
        if decision_path.is_file():
            r['runs'].append(read(decision_path))
        boundary_path=OUT/'boundary_cases'/f'{case}.json'
        if boundary_path.is_file():
            r['runs'].append(read(boundary_path))
        if case not in DECISIONS:
            if case in GAPS:
                r['reason']=GAPS[case]
                row['verification_gap']=r['reason']
            continue
        kind,verdict,reason=DECISIONS[case]
        run=next(x for x in r['runs'] if x['kind']==kind)
        path=Path(run['result'])
        actual=read(path)
        assert run['execution']['status'] in ('completed','capture-end'),case
        assert actual['cpp_sha256']==r['cpp_sha256'],case
        trace=path.with_name('trace.jsonl')
        assert trace.is_file(),case
        r.update(verdict=verdict,reason=reason,evidence=str(path),trace=str(trace),
                 evidence_type='host_replay_and_requirement_review')
        row.update(core_status=verdict,evidence=str(path))
        row.pop('verification_gap',None)
        if verdict=='core_fail':
            row['failure_reason']=dict(stage='core_execution',diagnostic=reason,evidence=str(path))
    folder=OUT/'decision_round'
    counts=lambda rs,k:dict(Counter(r[k] for r in rs))
    summary=dict(total=len(totals),batch_total=len(pending),batch_verdicts=counts(pending,'verdict'),
        total_verdicts=counts(totals,'core_status'),new_replays=sum(any(x['kind']=='decision' for x in r['runs']) for r in pending),
        datasets={d:counts([r for r in totals if r['dataset']==d],'core_status') for d in sorted({r['dataset'] for r in totals})},
        circuit_simulations_this_batch=0)
    host.save(folder/'review_119.json',pending)
    host.save(folder/'results_296.json',totals)
    host.save(folder/'failure_reasons.json',[dict(case=r['case'],dataset=r['dataset'],status=r['core_status'],
        evidence=r['evidence'],reason=r['failure_reason']) for r in totals if r.get('failure_reason')])
    host.save(folder/'summary.json',summary)
    print(summary)

GAPS={}


decisions('decision','core_pass',{
    'advanced_002':'双超声回放覆盖入口靠近、占用、离开及再次占用。绿灯引导有效，4 cm 与 3 cm 时持续 2000 Hz，12 cm 变为 1200 Hz，30 cm 静音。D2 循环显示两路距离和占用计数。',
    'advanced_012':'暗光时窗帘命令约 100°且亮灯，强光时关闭，暗光高温时保持关闭。D2 进入手动后 D3 连续调整到 10、20、30°，显示同步。按基本行为判定，未标定窗帘机构全开角。',
    'medium_017':'按高电平保持为启用的接法，三次启动的 PWM 分别为 70、130、200，关闭时均为 0。原题未规定松手锁存或全量程线性。',
    'medium_022':'实测 30、9、10、5、20 cm 的显示均正确，D13 仅在 9 和 5 cm 时亮。无回波显示 0 的旧断言不作为正文核心失败。',
    'medium_024':'采用高电平触发接线时，六次按键依次显示 1 至 6，奇数灯亮、偶数灯灭。题面未规定四位计数达到上限后如何回绕。',
    'embedbench_49':'两只 RGB 同步响应三段 ADC，三路舵机跟随电位器约 35、87、149、0 度，每次非初始更新之间至少两秒。按连续色梯度解释跨段保留已饱和通道。',
    'embedbench_107':'计数、亮段数、奇偶灯均同步运行，每两秒一档，9 后回到 0。初始化双灯关闭与运行时 0 属偶数的两条描述有冲突，按运行中的奇偶指示判定。',
    'embedbench_110':'RGB 两组同步完成七色轮转、单色灯每两秒翻转，三舵机同步切换到题面分项指定端点。初始化采用分项的 0、180、90°，分项与总述存在冲突。',
    'embedbench_120':'初始手动亮度随 pot1 为 127，按键后在约 54 至 204 的区间渐变，再按另一个键回到手动 PWM 211。原题初始熄灭和非零电位器立即生效按启动与运行区分。',
})
decisions('boundary','core_pass',{
    'embedbench_61':'两路电位器分别改变红绿及蓝，显示随 RGB 平均亮度变为 1、3、6、3。量化边界允许相邻一级，原题未规定整数舍入方法。',
    'embedbench_112':'分开改变两个电位器时，两组 RGB 的红绿分量各自变化且保持同步，两路均为 1023 时两组全白，LED3 点亮，离开满量程后熄灭。题面未指定其他输入的蓝色分量。',
    'embedbench_124':'下降穿过 0 回到 9，增加穿过 9 回到 0，正常范围内双向更新、奇偶灯同步。启动 LED 短暂关闭后执行 0 为偶数的运行规则。',
})
decisions('decision','core_fail',{
    'advanced_026':'两组 PIR 连续交接时 D9 仍直接跟随最后一个 PIR 关闭，没有延长亮灯。两种方向之外，仅一端触发也计为一次通行，源码未区分触发顺序。',
    'advanced_027':'A2 仅低电平 0.5 秒时，D6 从输入变低起立即报警，恢复时立即停止。未实现必须持续低压才报警。',
    'complex_012':'电位器最小值时数码管显示 1 ms，但 D13 实际每 5 ms 才翻转，完整周期 10 ms。显示值和可执行的闪烁周期不一致。',
    'complex_022':'D2 能切换三种模式，但进入呼吸状态后，PWM 仅在第一轮逐级升降，随后长期为 0，直到切出再进入才出现新一轮。未实现持续呼吸。',
    'embedbench_20':'全程记录显示通常每 2 秒变 10°，但 36 秒到 180° 后直到 40 秒才回落，74 秒到 0° 后直到 78 秒才重启。两端各停了 4 秒。',
    'embedbench_21':'第 6 步完成后，12 秒先额外插入两秒全零状态，14 秒才回到第 2 步。原题要求直接从第 2 步重启。',
    'embedbench_42':'40 秒时屏幕为 20，条形灯应亮前两段。实际第一、二段均灭，第三段亮。',
    'embedbench_52':'两路舵机从 150° 在下一个节拍直接回到 0°，没有任何 180°目标，且回到 0 后条形段仍然亮。',
    'embedbench_64':'补测按原接线连续推进颜色。第四次应为黄，红通道保持关闭，绿、蓝同时开启，实际为青色。',
    'embedbench_73':'三次按钮分别按下后，舵机一直保持约 30°。对应条形灯在释放后只亮约 10 ms，不能执行 60°、90°及各阶段保持。',
    'embedbench_93':'电位器使舵机依次 0→90→180→43→0°，但两只方向灯始终关闭，未指示旋转方向。',
    'embedbench_94':'不同速度旋钮值下，舵机都直接写入端点，没有可调的逐步扫动。第二路 ADC 256 时 LED 灭，512 时仅变为数字高电平。',
    'embedbench_101':'显示每四秒从 9 倒数，但 32 秒的 1 后在 36 秒直接变为 9，两轮都跳过 0。原题要求倒数至 0。',
    'embedbench_113':'按原图还原四阶段。第三阶段 RGB3 应为绿色，实际红绿同亮为黄色。第四阶段单色灯应亮，实际仍灭。',
    'embedbench_114':'第一阶段应持续 2 秒，实测从 0 至 10 秒一直保持第一阶段，导致两路灯色和条形灯没有按节拍推进。',
    'embedbench_119':'两次独立操作 btn1 和 btn2 时，两个舵机始终为 0°。没有实现按键指定的 90°、180°动作。',
    'embedbench_123':'第十次按键刚点亮第十段时溢出 LED 就更新。题面要求第十次保持十段，第十一次超过 10 后才清零并增加溢出次数。',
    'embedbench_125':'原图共阴接线下，按键前灯已亮。第一个模式两次都让三只 RGB 全白，原题要求分别红、绿、蓝。',
})
decisions('boundary','core_fail',{
    'embedbench_66':'程序在按键前已进入 90/45/135°，首次按键后直接进入 180/90/0°，跳过了规定的起始状态与首次过渡。',
    'embedbench_74':'上电后未按键已置为 90/90°。首个有效按键未得到要求的 5 段状态；相隔 0.7 秒的第二次操作也被执行，未落实两秒锁定。',
    'embedbench_87':'一次按压只在前 8 ms 模拟机械抖动，随后持续按住。红色 PWM 在同一次按压从 0 到 32 又到 64，未执行去抖。',
})
decisions('unapproved_draft','core_fail',{
    'advanced_001':'生成程序只有风扇或灯的二选一状态，水泵手动激活变量在全部赋值点均为 false，因此无法选择第三个控制对象。已有轨迹同时显示多路传感器被读取但显示停在单一页。',
})
decisions('previously_tested','core_fail',{
    'advanced_011':'警报状态下蜂鸣器 2000 Hz 连续维持到输入改变，未出现间歇。没有观察到题面明确要求的 fire alert 告警输出。',
    'complex_002':'冻结代码按 ADC×100/1023 计算 LM35 温度，基于 5 V 参考的温度比例应为 ADC×500/1023。回放中 ADC=307 仅显示 30，自动风扇因此基于错误温度调速。',
    'complex_008':'距离和 LED 正常，D6 在 14 cm 时持续输出 140 Hz，7 cm 时持续输出 220 Hz，两段均没有间歇。按题面慢鸣与急促鸣叫指重复节奏判定。',
})
GAPS.update({
    'embedbench_14':'每秒加二以及超过 10 后显示个位已实测。恰好计数为 10 时持续一秒空白，题面只规定超过 10 取个位，电路仅有单个数码管。需先定义恰好等于 10 时的一位显示规则。',
    'embedbench_67':'两次交替的舵机命令已实测正确，开机 10 ms 后第二舵机先到 180°，题面同时要求双轴起始 0°及交替前 0/180°。实体舵机平滑性也需要电路或硬件验证。',
})

if __name__=='__main__':
    resolve()
