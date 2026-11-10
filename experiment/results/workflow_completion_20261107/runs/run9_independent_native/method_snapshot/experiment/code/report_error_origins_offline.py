"""Attach conservative, source-bound diagnoses to the read-only inventory."""
import collections
import json
from pathlib import Path
from audit_error_origins_offline import OUT, DATA, read, save, aggregate

# These diagnoses concern the workflow267 artifacts inspected in this audit.
# They are never transferred to a different generated source by case name alone.
FINDINGS = {
 'advanced_011': ('contract_and_model', '报警文本遗漏且间歇鸣叫缺失', '需求明确fire alert，契约却声称没有固定文本要求，代码只输出temp/smoke/flame/level；模型音调恒随报警状态，无间歇计时。'),
 'advanced_028': ('configuration_contract_and_model', '呼叫队列及等待时间遗漏', '原文床位D4与显示CLK=D4冲突；模型只保存一个callActive，确认时直接清除全部，没有保存多个待处理呼叫，ageOfOldestMs不累计。'),
 'complex_019': ('contract_and_model', '按控制步倒计时代替按秒', '契约没有时间输入；模型每tick减remainingSeconds，蜂鸣倒计时从0开始且不装载两秒时长，无法按真实秒运行。具体开始按键失败还存在电平解释问题。'),
 'simple_028': ('contract_and_model', '开关命令没有状态保持', '模型没有LED持久状态，两个关闭分支均false；契约又将pullup=true与active_high=true并用，按键语义不一致。'),
 'pceval_traffic_light': ('contract_and_model', '同轮穿透全部交通灯阶段', '契约没有时间输入；模型同一tick内依次进入黄灯、红灯并返回绿灯，没有真实持续1000/2000ms。'),
 'pceval_4_digit_7_segment_display': ('contract_and_model', '缺少秒计时且段输出错误', '契约没有时间输入；模型每tick推进step，且io_3(段D)恒false，不可能按需求在数字3/4间正确变化。'),
 'pceval_photoresistor_basic': ('contract_and_model', '报告格式和周期遗漏', '契约标签LDR Value没有冒号空格，模型每步报告且没有500ms门控，代码输出LDR Value加浮点值，不符合指定格式和周期。'),
 'pceval_LCD_display_basic': ('contract_and_model', '显示驱动能力被错误假定', '契约只列普通数字引脚，规划文字却把SPI和Adafruit_ILI9341调用交给SysML；模型输出恒false，没有初始化/写入文本。测试要求的Hello world须结合完整原文核对，不能称完整LCD协议仿真已通过。'),
 'simple_004': ('model', '数值映射遗漏', '契约将adc_count到pwm_count映射交给模型；模型io_1=io_0；代码直接限幅255，四个不同输入都得到255。'),
 'simple_006': ('model', '计时器每轮错误减周期', 'elapsedAccumMs只增加不复位；达到2000后，or条件始终成立，每轮从cycleAccumMs再减2000，后续周期不再正常鸣叫。'),
 'simple_009': ('model', '串口事件没有锁存状态', '输出仅在io_1_valid时计算ON，否则false；接收事件结束后LED立即关闭，缺少命令状态保持。'),
 'simple_011': ('model', 'PWM命令没有锁存', '输出io_1_valid时取输入，否则0；接收完成后亮度回零，而非保持设置值。'),
 'simple_012': ('contract_and_model', '重复串口输出', '契约同时定义serial_text_output和serial_channels，模型同时触发；代码输出Hello Arduino和额外report gate消息。29ms是相邻消息间隔，不能解释为主计时周期。'),
 'simple_013': ('model', '清零后判断触发', '模型在accumulatedMs>=1000时先减1000，输出又判断更新后的accumulatedMs>=1000，正常步长下不触发。'),
 'simple_021': ('contract_and_model', '周期功能未实现', '契约缺少计时输入；模型tone输出恒为0，无法每秒鸣叫。'),
 'simple_030': ('model', '先覆盖历史状态再检测边沿', 'tick先previousIo0=io_0，输出再计算io_0 and not previousIo0，该条件恒假，计数值无法按要求报告。'),
 'medium_001': ('model', 'PWM数值量程错误', '模型输出1023-ADC且适配比例1，较大范围被限幅255；测试得到255,255,255,223。'),
 'medium_003': ('configuration_contract_and_model', '错误输入与单位', '原文LM35=A1；配置/契约引入A0后又补A1；模型只用A0的原始ADC与30比较，未按温度单位换算。'),
 'medium_008': ('contract', '按钮语义自相矛盾', 'pullup=true且active_high=true；旧审计已确认规划文字把按下LOW解释为true，结构化绑定却HIGH=true；代码按HIGH检测，计数发生在释放沿。'),
 'medium_026': ('contract_and_model', '没有实现闪烁', '契约只有ADC输入；模型phaseMs=phaseMs、ledOn=ledOn，LED状态永不变化。'),
 'medium_027': ('model', '周期输出永不触发', 'accumulatedMs>=5000时减5000，然后用更新后的accumulatedMs>=5000作为输出触发。'),
 'complex_003': ('model', '距离条件反向且动作未实现', '需求10cm内靠近；模型用distance>=10触发，且没有三次短鸣和LED快速闪烁状态。'),
 'complex_004': ('model', '边沿检测更新顺序错误', '模型先prevIncPressed=io_0、prevDecPressed=io_1，再检测io and not prev；两条计数条件均恒假。'),
 'complex_007': ('contract_and_model', '串口控制输入缺失', '需求ON/OFF/BLINK/PWM/BUZZ/STATUS；契约与模型缺少串口接收输入；ledState、pwmValue、buzzerHz没有命令驱动更新。'),
 'complex_009': ('contract_and_model', '开始逻辑恒等且无真实秒计时', 'running赋值嵌套取反后仍等于原running；初始false不能启动。模型还按控制步秒数加一，契约无经过时间输入。'),
 'complex_011': ('model', '持续报警代替间歇报警', '模型LED和tone仅由报警布尔条件组合决定，没有闪烁/间歇相位；测试观察不到持续的LED边沿。'),
 'complex_013': ('model', '蜂鸣命令功能遗漏', '需求串口BEEP；模型蜂鸣器输出io_3恒为0，不能执行命令。计数失败还需另查。'),
 'complex_014': ('contract_and_model', '温度单位及状态转换遗漏', '契约输入为原始ADC；模型直接显示该值且showMax没有置true路径，无法实现按键显示摄氏历史最高温3秒。'),
 'complex_024': ('model', '输出触发被更新顺序消除', '先previousPressed=io_0，输出再判断io_0 and not previousPressed，串口恒不触发；蜂鸣提示还在同轮被清除。'),
 'complex_027': ('model', '轮询报告条件被提前消除', 'pollTimerMs达到3000后先减3000，再作为serial_0/1_trigger条件，正常步长下报告不触发。'),
 'advanced_003': ('contract_and_model', 'STATUS响应输出缺失', '契约没有STATUS响应输出路径；模型在STATUS分支仅done，无法返回状态。'),
 'advanced_019': ('model', '接近条件及时间记录错误', '以距离>=2且有效作为接近条件，未限制近距离；dangerElapsedMs自赋值不累计，prevDanger更新后结束触发恒假。并非所有绿色灯断言均已独立归因。'),
 'embedbench_1': ('model', '灯光序列错误', '10~15秒分支把两灯都设false，需求此时应两灯同时亮；模型15秒复位也不符合给定序列。'),
 'embedbench_3': ('contract_and_model', '每控制步轮色代替每两秒', '模型每tick更新colorIndex，契约没有时间输入且control_period为5ms，缺少每色2秒逻辑。'),
 'embedbench_4': ('contract_and_model', '颜色序列完全缺失', '模型三个颜色输出全部恒false，无状态和计时，不能执行每3秒颜色序列。'),
 'embedbench_5': ('contract', '按钮接地接线与电平配置冲突', '电路按钮接地；契约active_high=true、pullup=false，模型按true上升沿翻转，实际启动/按压不符合接线。'),
 'pceval_led_RGB_basic': ('contract', '共阳电平定义错误', '明确COM接5V，契约仍把三通道active_high设true；模型输出false,false,true，代码得到LOW,LOW,HIGH。'),
 'pceval_button_led': ('contract', '按钮有效电平反向', '验收明确松开HIGH、按下LOW；契约active_high=true，模型直接输出输入，代码表现相反。'),
 'pceval_button_duration': ('contract_and_model', '按压时长没有累计', '契约缺少时间输入；模型pressedDurationS不断设0，无法测量按压时长。'),
 'pceval_humidity_sensor_basic': ('model', '读到的湿度没有进入报告', 'lastHumidity初始0后从未用io_0更新，报告始终输出0而非传感器湿度。'),
 'pceval_serial_monitor': ('contract_and_model', '接收与回显功能缺失', '契约只有输出，没有串口接收输入；模型输出固定Received前缀与0，无法回显收到的数据。'),
 'pceval_serial_RGB_led': ('contract_and_model', '串口颜色命令缺失', '契约没有串口命令输入；模型三个颜色和报告触发均恒false。'),
 'pceval_temperature_sensor_basic': ('contract', '输出文本格式错误', '需求Current temperature: {temperature value}，契约标签temperature；trace为temperature-10.50。此项是格式失败，不是没有读到温度。'),
 'pceval_led_blink_basic': ('contract_and_model', '启动消息遗漏', '需求LED Blink test starts；契约仅unused报告值，模型没有该文本输出。LED本身每秒翻转，因此不能描述为灯完全不能闪。'),
 'pceval_button_buzzer': ('literal_configuration_only', '引脚模式文字要求未满足', '现有失败仅INPUT要求与INPUT_PULLUP不一致；不能据此认定按键蜂鸣功能失败。'),
 'pceval_button_servo_motor': ('literal_configuration_only', '引脚模式文字要求未满足', '现有失败仅INPUT要求与INPUT_PULLUP不一致；不能据此认定舵机角度控制失败。'),
}

UNRESOLVED = {
 'advanced_005': '模型只切换两类显示而需求含三类；当前报表断言涉及接收窗口和5秒触发，尚未逐步复现触发与串口耗时，保留归因待定。',
 'advanced_017': '模型同轮将buzzerTone置1000后又清0、报告条件使用更新后locked，是明确疑点；已有失败集中在选手编号，尚需隔离按键极性与复位场景，不能宣称这些编号失败已定位。',
 'complex_001': '报警原因的0/1文本可能被contains断言误当成实际原因；无效显示占位也依赖验收约定，需要复核断言与原验收一致性。',
 'complex_005': '失败表现像低电平按钮测试与高电平契约不一致；原需求未说明按钮接法，不能仅凭默认测试认定契约错误。',
 'complex_025': '测试按低电平按钮切换，候选契约为高电平；原文没有明确按钮接法，需统一硬件约定后判断。模型仅在切换时采样阈值也是独立疑点。',
 'complex_030': '输出依赖按钮极性及DHT缓存；模型有清计时后判断报告的独立缺陷，但当前失败主要是切页保持，尚未将每条失败与具体原因对应。',
 'medium_005': '模型按高电平按下，测试按低电平按下；原文没有按钮接法，行为相反不能自动归为模型逻辑错误。',
 'medium_010': 'INPUT_PULLUP与HIGH按下定义存在冲突嫌疑；需核对规划文字/接线约定，避免把默认测试接法当原文事实。',
 'medium_024': '计数发生在释放沿；契约高电平、测试低电平且原文未明确接法，需确认统一配置约定。',
 'medium_029': '已有明确低电平验收约定与历史契约不符；需确认该约定是否确实在此轮生成前已冻结，不能把后来细化要求追溯为原模型错误。',
 'simple_020': '除按钮极性假设外，leading_zero=false与0000要求冲突；原行为失败混合两者，尚需分别检查。',
 'simple_022': '原文没声明按钮接线，测试低电平、契约高电平；暂不把输入解释差异认定为模型缺陷。',
 'pceval_7_segment_display_basic': '需逐段对照实际电路连接顺序和共阳极性；仅显示?不足以区分段码错误与观察器映射问题。',
 'pceval_7_segment_display_counter': '模型段码与图中段序有疑点；需要逐位计算模型、适配输出和独立段码，不能仅靠解码?确定来源。',
 'pceval_7_segment_display_serial': '模型使用接收有效脉冲直接驱动段码、缺少锁存；当前显示8还可能涉及共阳及段序，需分别隔离，暂不作单一根因结论。',
}


def main():
    rows = read(OUT / 'inventory.json')
    seeds = {x['case']: x for x in rows if x['group'] == 'workflow_267'}
    diagnoses = []
    for case, (origin, category, explanation) in FINDINGS.items():
        seed = seeds[case]
        for row in rows:
            if row['case'] != case or row.get('source_sha256') != seed.get('source_sha256'):
                continue
            if row.get('upstream_artifacts') != seed.get('upstream_artifacts'):
                continue
            row.update(attribution=origin, diagnosis=explanation, defect_category=category,
                       diagnosis_scope='At least one identified defect; not exhaustive per-assertion causality')
            diagnoses.append(dict(group=row['group'], case=case, origin=origin,
                category=category, explanation=explanation, source=row['source'],
                source_sha256=row['source_sha256'], upstream=row['upstream_artifacts'],
                evidence=row.get('evidence'), dossier=row.get('dossier')))
    for row in rows:
        if row['group'] == 'workflow_267' and row['case'] in UNRESOLVED:
            row.update(attribution='reviewed_causality_unresolved', diagnosis=UNRESOLVED[row['case']])
    save(OUT / 'inventory.json', rows)
    save(OUT / 'localized_findings.json', diagnoses)
    primary = [x for x in rows if x['group'] == 'workflow_267' and x['behavior_status'] == 'failed']
    counts = collections.Counter(x['attribution'] for x in primary)
    summary = read(OUT / 'summary.json')
    summary.update(primary_failed_cases=len(primary), primary_failure_attribution=dict(counts),
                   attribution_records=len(diagnoses), localized_seed_cases=len(FINDINGS))
    save(OUT / 'summary.json', summary)
    lines = ['# 历史实验错误来源审计', '',
        '结论：契约和SysML中已存在多种可核实缺陷，是应优先处理的来源；不能把全部失败归到这两层，也不能从已测试子集外推296题总体。', '',
        '## 范围与证据', '',
        f'- 扫描13组×296题，共{len(rows)}条冻结记录；逐一校验{summary["source_files_checked"]}个原始源文件哈希。',
        '- 关联完整行为测试、10题试跑（已包含于完整表）、旧16题执行、已有静态审查及可找到的上游产物。',
        '- 静态审查的confirmed仍是审查标签，不等同于执行复现。没有定位的记录保留not_yet_localized。',
        '- 重点逐案追踪历史workflow267的61条已保存行为失败；其余版本只在源码与上游产物身份完全一致时复用归因。',
        '- 没有修改工作流、候选程序、历史评分；没有网络请求或重新运行候选程序。', '',
        '## 全量行为记录概况', '',
        '| 组 | 总数 | 原编译成功 | 行为失败记录 | 完整场景检查通过 | 部分场景通过 | 其他/未确认 |',
        '|---|---:|---:|---:|---:|---:|---:|']
    for g, v in summary['groups'].items():
        c = v['behavior']; fail=c.get('failed',0); passed=c.get('passed',0); partial=c.get('partial_checks_passed_not_full',0)
        lines.append(f'| {g} | {v["total"]} | {v["compiled"]} | {fail} | {passed} | {partial} | {v["total"]-fail-passed-partial} |')
    lines += ['', '以上“失败”是原始断言结果，尚包含引脚模式等文字合规差异，不等于所有程序主功能失效。部分通过、环境未知不能当作失败。', '',
        '## workflow267失败归因', '', str(dict(counts)), '',
        '| 题目 | 来源判断 | 具体证据解释 |', '|---|---|---|']
    for row in primary:
        lines.append(f'| {row["case"]} | {row["attribution"]} | {row.get("diagnosis", "已关联契约/模型/执行断言；尚不能可靠区分模型、配置解释、转换或测试问题，保留未定位。")} |')
    lines += ['', '## 已确认的共同验证缺口', '',
        '周期计时、按钮边沿等错误在语法/结构合法的情况下进入最终代码。旧28份审计已记录structural_pass、functional_status=unverified，以及行为检查not_requested。req_test_map文字不是执行断言。', '',
        '根因不能简化为大模型能力不足：契约解释、模型行为、配置标准化、验证覆盖与反馈路由均有责任。现有材料未证明确定性转换器完全正确，也未证明它是主因。', '',
        '## 编译未成功不能混为一类', '',
        '- advanced_013：SysML表达式三轮验证未通过，未生成Arduino代码。',
        '- advanced_018：原文风扇D5与TM1637 DIO=D5冲突，契约拒绝有合理依据；这是需求/配置冲突。',
        '- LLM后端advanced_005：TM1637Display没有displayInt，是真实库接口编译错误，不能归到确定性后端。',
        '- 生成失败阶段统计保存在summary.json；未找到匹配版本的日志时不跨版本填补。', '',
        '## 限制和后续', '',
        '本报告完成全量记录扫描和重点失败链追踪，但未对所有独立版本的每条断言完成因果证明。未定位项不能计入契约/模型缺陷比例。不能据此给出实板功能通过率。',
        '下一步按localized_findings.json中的精确产物修复；先处理计时触发/边沿更新顺序、契约按钮极性和缺失输入，再用相同断言回归及新题检验。',
        '完整逐产物材料在dossiers/；包括原始需求、契约、SysML、代码、失败断言、已有审查。']
    (OUT / 'REPORT_ZH.md').write_text('\n'.join(lines)+'\n', encoding='utf-8')
    assert sum(counts.values()) == 61
    assert all(x['source_sha256'] == seeds[x['case']]['source_sha256'] for x in diagnoses)
    print(json.dumps(dict(primary_counts=counts, diagnoses=len(diagnoses)), ensure_ascii=False))


if __name__ == '__main__':
    main()
