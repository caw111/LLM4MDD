# PICSimLab 296题仿真验证结果（2026-11-08）

本轮覆盖冻结的最新交付代码296题，分为本地120题、PCEval 50题、EmbedBench 126题。全部296份目标AVR固件实际进入PICSimLab执行，最终完整批次共443个场景，443次均正常执行到预定仿真时长。最终批次于北京时间2026-11-08 04:31:07结束，批次用时22分14秒；此前环境构建、外设接入和独立校准时间另计。

## 主结果：基本功能场景复核

采用用户要求的“核心功能满足基本需要”口径：保留有效功能输入/输出要求；将已确认的非核心提示格式、未规定的初始化时限和目标平台测量分辨率问题逐题单列。没有改动待测代码，没有将未观察到或没有断言的题目计为通过。本表是探索性修复后制品复核，不是预注册基准结果，也不表示逐条需求和所有边界场景均通过。

| 数据集 | 题数 | 编译产物核验 | 基本功能场景通过 | 判据不匹配 | 待确认 | 通过比例 |
|---|---:|---:|---:|---:|---:|---:|
| 本地数据集 | 120 | 120/120 | 95 | 24 | 1 | 79.17% |
| PCEval | 50 | 50/50 | 41 | 9 | 0 | 82.00% |
| EmbedBench | 126 | 126/126 | 125 | 1 | 0 | 99.21% |
| 合计 | 296 | 296/296 | 261 | 34 | 1 | 88.18% |

34题“判据不匹配”不能直接等同于34份代码错误：其中仍包括接线冲突、测试假设、时间/格式约束及外设观察限制。逐题原始期望、实际输出、未确认原因和证据路径保存在 [final_all296_cases.json](final_all296_cases.json)。这一结果没有达到此前期望的95%。

## 统计口径与修正记录

- 完整批次原始判据并集：169通过、119不匹配、8待确认。该并集包含旧描述性场景，不能直接作为基本功能通过率。
- 校正被独立控制程序证实的观察器问题后，全部保存判据同时通过为185题；原始记录不覆盖。校正包括物理PWM的LED状态解释、低频蜂鸣器采样、定时器量化频率容差，以及不把空的诊断计划作为功能判据。
- 不根据通过与否选择计划：固定取每题最后一个包含可执行断言的保存计划作为核心计划。该固定计划口径为253通过、42不匹配、1待确认。时钟题使用同一固件、同一期望追加段选引脚采集，消除了原先观察不足，但发现显示为0000的实际不匹配。
- 另有8题按原始需求和基本功能口径完成具名复核，计入主表261题。每题理由保存在 [basic_function_qualifications.json](basic_function_qualifications.json)：7题为PCEval的模式枚举、提示/标签、额外启动窗口或近500ms报告条件；1题为连续舵机轨迹的20ms控制帧测量分辨率。所有原始严格结果仍保留。
- 源需求中未规定的setup≤500ms，不能被描述为显示功能失效；例如DHT22液晶屏题的温湿度内容和变化检查均通过，约962ms初始化是唯一原始不匹配项。用户已明确采用的250ms响应实验条件没有在固定计划统计中被放宽。
- 已确认21个旧描述性/转换场景存在具体问题，详见 [oracle_audit_20261108.json](oracle_audit_20261108.json)。例如23次按键仅生成4次脉冲、未施加描述中的电位器输入、每2秒变化的状态配上错误时间点，以及将字符串off错误视为真。旧场景仍可在逐题记录中复查。

## 另列的条件通过

以下2题不并入261题主结果：

| 题目 | 实测结果 | 条件 |
|---|---|---|
| medium_017 | 通过保持电平控制场景 | 按钮作为保持式开关；原题未明确是边沿切换还是保持电平。原切换场景不匹配保留。 |
| advanced_028 | 通过呼叫、确认和显示场景 | 依既有configuration-defaults-v3策略将冲突的TM1637 CLK从D4调整至D8。原始D4同时接床位按钮的结果保留。 |

游戏题embedbench_0另加了依据实际LED输出施加按钮输入的闭环测试。“每轮正确响应”和“先按错再正确重试”两种场景均通过，均得到5次成功和最终5分；此外原来的5轮无响应超时场景也保留。

## 观察到的目标平台问题示例

pceval_clock的RTC输入为2024-11-24 23:59:57，按题意应显示2359。补齐段选引脚采集后实际读到0000。目标avr-g++预定义宏确认double为4字节、24位有效二进制位；生成代码将Unix秒数转换为double后再除以3600/60。1732492797转换为该浮点格式会舍入成1732492800，即午夜。该计算与实际提前显示0000一致，是有固件和波形支持的数值精度问题分析，详见 [numeric_precision_evidence.json](numeric_precision_evidence.json)。这说明主机上通过的64位double计算不能替代目标AVR上的执行验证。

## 实际执行方式与校准

执行的是实际ATmega328P/ATmega2560 HEX固件，使用PICSimLab 0.9.3 NOGUI AVR构建。输入从MCU引脚、ADC电压、UART字节及原生传感器模型进入；输出从实际GPIO/PWM边沿、UART字节和ILI9341像素读取。没有用主机Arduino API桩代替程序执行。

该构建包含版本化测试夹具和后端修正，不能称为未经修改的PICSimLab原版。SPI/I2C时钟、Mega缺失PWM模式、相位修正计数及定时器输出覆盖GPIO锁存器的问题已修正并保存方法快照。DHT、HC-SR04、RTC、MPU6050、ILI9341等使用原生模型；TM1637、模拟多路开关及游戏输入控制使用公开保存的协议/电气夹具。基于芯片手册校准的双向PWM周期为2×TOP×分频系数，[Microchip ATmega2560数据手册](https://www.microchip.com/bin/mchp/product-ds.ATmega2560.pdf)支持该计算。

独立控制程序确认：Uno 6个和Mega 15个PWM引脚的占空比与频率均通过；SPI为1MHz、I2C为100kHz，均在1%校准范围内。混合外设正向控制通过，故意错误的舵机、显示和串口期望均被拒绝。超声回波相对误差小于0.1%；中断负载下标准pulseIn报告的偏差另行披露在校准结果中。

这是数字/外设功能层面的固件仿真，未验证实际供电、电流负载和舵机机械负载。部分只有局部场景覆盖的题目在计划中标有partial，不应据此宣称全部需求已被证明。

## 完整性与文件

- [汇总JSON](final_all296_summary.json)
- [296题逐题结果、失败期望和实际值](final_all296_cases.json)
- [296份源文件和HEX哈希核验](final_integrity_verification.json)
- [固定后端方法快照](method_snapshots/native-electrical-v11/provenance.json)
- 每题源文件、编译日志、固件、电路、输入、压缩电气轨迹、输出观察和判据记录位于cases/<题目>/；补充场景位于additional_results/。

296份待测源代码和296份固件哈希全部匹配冻结清单。本轮编译产物核验包括复用此前已经验证的17份及新编译的279份；不能将其解释为原始工作流自动生成编译成功率100%。本轮没有调用工作流重新生成实验代码，也没有修改这些待测代码。

## 仍有判据不匹配或待确认的题目

| 题目 | 数据集 | 结论 | 原计划未满足判据（前4项） |
|---|---|---|---|
| advanced_004 | internal | 判据不匹配 | clock_advanced、alarm_led_blinks、alarm_sound_activity、missed |
| advanced_005 | internal | 判据不匹配 | display_tm:4:5_250、gpio_12_4500、gpio_12_8700、living_room_temperature_c_5000 |
| advanced_009 | internal | 待确认 | complete_activation_selects_temperature〔观察未确认〕、temperature_view_persists_after_release〔观察未确认〕、temperature_updates_upward〔观察未确认〕、temperature_updates_downward〔观察未确认〕 |
| advanced_015 | internal | 判据不匹配 | level_6500 |
| advanced_018 | internal | 判据不匹配 | heater_500 |
| advanced_020 | internal | 判据不匹配 | core_serial_bbd201cdc7、core_display_activity_b3d6965108、core_buzzer_bursts_b763a7e966 |
| advanced_021 | internal | 判据不匹配 | core_gpio_91e6151624、core_display_activity_8263b1027f |
| advanced_022 | internal | 判据不匹配 | core_edges_abfc1c6f7e、core_serial_2eba002523、core_gpio_2af6b47d93 |
| advanced_025 | internal | 判据不匹配 | core_display_activity_31eff0159b |
| advanced_028 | internal | 判据不匹配 | core_display_activity_9648cae584 |
| advanced_029 | internal | 判据不匹配 | core_buzzer_bursts_78148fb234、core_display_activity_8263b1027f |
| complex_001 | internal | 判据不匹配 | no_boundary_alarm |
| complex_002 | internal | 判据不匹配 | initial_mode_display_tracks_endpoint_change、display_tracks_endpoints_after_first_press、display_tracks_endpoints_after_second_press |
| complex_006 | internal | 判据不匹配 | display_tm:4:5_300 |
| complex_010 | internal | 判据不匹配 | default_temperature〔观察未确认〕、stable_mode_during_hold、completed_held_press_humidity〔观察未确认〕、mode_stays_after_release〔观察未确认〕 |
| complex_022 | internal | 判据不匹配 | pwm_9_2940、pwm_9_3440、pwm_9_3940 |
| complex_026 | internal | 判据不匹配 | display_tm:4:5_600 |
| complex_027 | internal | 判据不匹配 | gpio_13_24250、temperature_0、distance_0、distance_3000 |
| embedbench_87 | EmbedBench | 判据不匹配 | gpio_8_1300、gpio_8_3500、gpio_9_3500、gpio_8_5700 |
| medium_002 | internal | 判据不匹配 | exact_boundary_led、exact_boundary_silent |
| medium_007 | internal | 判据不匹配 | maximum_valid、above_range_blank、near_upper_limit |
| medium_017 | internal | 判据不匹配 | core_pwm_25e7cef7cd、core_pwm_8ae1f3b757、core_pwm_d733f51980、core_pwm_c4fd10c610 |
| medium_019 | internal | 判据不匹配 | selection_3400、selection_5000、selection_11400、selection_13000 |
| pceval_button_LCD_display | PCEval | 判据不匹配 | setup_deadline、initial_released_display_including_setup、first_press_response、first_release_response |
| pceval_calendar_display | PCEval | 判据不匹配 | startup_deadline、initial_date、initial_weekday、rtc_midnight_date |
| pceval_clock | PCEval | 判据不匹配 | clock_0、clock_1 |
| pceval_distance_sensor | PCEval | 判据不匹配 | 400_equal_400、401_greater_than_400、200_equal_200、return_400_equal_400 |
| pceval_distance_sensor_basic | PCEval | 判据不匹配 | core_serial_d6f960fdbe |
| pceval_multiple_timezone | PCEval | 判据不匹配 | rtc_report_2300、rtc_report_14200、rtc_report_5000、rtc_report_8300 |
| pceval_parking_spot_monitor | PCEval | 判据不匹配 | gpio_4_4200、gpio_3_4200 |
| pceval_photoresistor_basic | PCEval | 判据不匹配 | adc_zero_start、adc_full_scale、adc_low_intermediate、adc_midpoint_lower |
| pceval_serial_LCD_display | PCEval | 判据不匹配 | setup_deadline、first_display〔观察未确认〕、first_echo、whitespace_display〔观察未确认〕 |
| simple_014 | internal | 判据不匹配 | serial_900、serial_3900、serial_6900 |
| simple_021 | internal | 判据不匹配 | initial_silence |
| simple_029 | internal | 判据不匹配 | core_sample_relation_798b855547 |
