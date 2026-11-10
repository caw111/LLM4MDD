# 34题基本核心功能复核结论

**22题通过，12题不通过。34题均有明确二元判定。**

标准：用户要求的基本核心功能可用：关键输入能够产生相应正确输出，核心状态/计时/控制流程成立。 本表不等于所有历史断言或额外实验限制全部通过。

原生成代码未修改。34题全部独立重新编译成功，生成HEX与冻结固件完全一致，源码和固件哈希再次核验一致。结合源码审查、已有原生固件仿真和针对性补测作出判定。

## 逐题结果

| 题号 | 内容 | 基本功能结论 | 判定依据 |
|---|---|---|---|
| advanced_004 | 服药提醒时钟 | **不通过** | 实际计时约慢20%；设定06:59后，到62秒仍显示06:59，应到07:00时未按时提醒。计时状态用AVR的32位double累加大毫秒数，发生舍入丢失。 |
| advanced_005 | 双房间环境监测 | **通过** | 稳定输入补测的两房间完整5秒报表及独立告警全部通过；原序列的显示轮询和按钮切换也可工作。 |
| advanced_015 | 节水控制 | **通过** | 定时开阀、次数统计、低水位禁止出水和报警通过；仅水位百分比的小幅测距量化偏差触发旧判据。 |
| advanced_018 | 孵化箱温控 | **通过** | 修正测试夹具的DHT初始值后，13项原场景断言全部通过，覆盖加热、显示切换及持续异常60秒报警。 |
| advanced_020 | 电梯控制 | **不通过** | 移动剩余时间被置为1000后没有递减，电梯无法完成楼层移动；负载状态也没有从输入更新。 |
| advanced_021 | 水族箱控制 | **不通过** | 各显示分支均输出0；温度输入按原始ADC使用，较高温度下仍不能正确关闭加热。核心温控和显示不成立。 |
| advanced_022 | 预约与占用管理 | **不通过** | 超时条件同时要求结束时间大于开始时间、开始时间不小于结束时间，逻辑不能成立；没有实际实现超时提醒。 |
| advanced_025 | 分阶段烹饪 | **不通过** | 阶段倒计时没有递减；启动后直接进入第二阶段并停在那里，不能完成后续烹饪阶段。 |
| advanced_028 | 病房呼叫队列 | **不通过** | 独立补测4项均失败：第二床单独呼叫显示0，后续呼叫覆盖等待最久床位，确认一次清除全部呼叫，等待时间日志始终为0。 |
| advanced_029 | 冷库与除霜 | **不通过** | 原始ADC值与3000等温度阈值直接比较，正常ADC量程无法触发相关控制；温度告警、风机和除霜的核心控制不正确。 |
| complex_001 | 环境监测报警 | **通过** | 温度/距离显示轮换和报警有效；名义15cm的回波被pulseIn测为略小于15cm，按实测值触发距离报警符合条件。 |
| complex_002 | 手动自动风扇 | **通过** | 采用既定冲突消解接线后显示正常；独立补测手动电位器、自动温度调速以及非选中输入不干扰，6项实际PWM检查全部通过。 |
| complex_006 | 可调温度阈值 | **通过** | DHT初始温度夹具修正后18项原断言全部通过，显示和阈值报警有效。 |
| complex_010 | 温控风扇和温湿度切换 | **通过** | 采用既定冲突消解接线后，温湿度显示、按钮切换和温控PWM共18项原断言全部通过。 |
| complex_022 | 多模式呼吸灯 | **通过** | 常灭、常亮、呼吸模式及参数切换可工作。旧判据将题目未规定的呼吸相位固定到3个采样点，实际偏差仅约5/255。 |
| complex_026 | 串口菜单温湿度显示 | **通过** | DHT初始温度夹具修正后14项原断言全部通过，菜单切换及按钮开关串口报告有效。 |
| complex_027 | 三秒定时巡检 | **通过** | 串口实际以约3秒周期报告，连续3次近距离检测后输出persistent object detected并点亮LED；旧正则把量化后的浮点距离限定为理想整数。 |
| embedbench_87 | RGB与条形灯 | **不通过** | RGB亮度能按32逐级变化，但条形灯未同步显示对应级数，前7次红色调节时相应灯段保持0；重放复现。完整8引脚独立参考程序40/40通过，排除该GPIO组合的通用仿真故障。 |
| medium_002 | 距离阈值报警 | **通过** | 远近距离报警能正常切换。名义20cm实际略低于20cm，固件按实测值小于20报警，旧边界静音判据不成立。 |
| medium_007 | 测距数码管 | **通过** | 显示随实际距离变化，400cm附近约2cm误差不改变基本测距功能；需求也没有要求超过400cm必须清屏。 |
| medium_017 | 按钮门控调光 | **通过** | 按下期间A0改变LED亮度，释放后关闭；实际PWM60、128、200及重复输入一致性通过。题目未规定必须是按一下保持的锁存开关。 |
| medium_019 | 温度距离切换 | **通过** | 按钮能正确切换温度和距离；120cm显示119、77cm显示76是测距量化，不影响基本显示功能。 |
| pceval_button_LCD_display | 按钮屏幕状态 | **通过** | 实际LCD像素8次按下/释放均显示对应Pressed!/Not pressed!，350ms短按也被显示；文字位置、颜色和字号正确。 |
| pceval_calendar_display | 日期星期显示 | **不通过** | 午夜前RTC原始值仍为2024-02-28，但程序已显示2024-02-29。独立RTC参考确认日期应仍为28日；Unix时间经AVR 32位double舍入提前跨日，属于日期错误。 |
| pceval_clock | 时钟数码管 | **不通过** | 应显示23:59时显示00:00，时间戳转AVR 32位double后被舍入到次日零点；完整数码管引脚补采集已复现。 |
| pceval_distance_sensor | 串口目标距离比较 | **通过** | 新增物理回波仿真分别进入小于、等于、大于三个分支，3/3通过；同一输入下标准Arduino pulseIn整数转换参考也3/3通过。 |
| pceval_distance_sensor_basic | 串口测距 | **通过** | 不同距离下持续输出对应厘米数，120cm附近旧±1cm判据过紧；pulseIn在实际Timer0/UART中断条件下存在小幅量化偏差。 |
| pceval_multiple_timezone | 多时区时钟 | **不通过** | UTC基准20:48附近，UTC+9输出05:47:44、UTC+4输出00:46:56，分钟和秒均错误。大时间戳的浮点转换和运算丢失精度。 |
| pceval_parking_spot_monitor | 车位占用指示 | **通过** | 远近目标下红绿指示正确；名义50cm回波实测略小于50，按实测值占用符合程序阈值。 |
| pceval_photoresistor_basic | 串口光照读数 | **通过** | 实际串口正确输出0、1023、137、511、512、876并在返回输入时恢复相应读数。旧断言因冒号、空格和小数格式差异未匹配。 |
| pceval_serial_LCD_display | 串口内容屏幕显示 | **通过** | 原生LCD像素可识别Hello LCD!、Split message、Idle text、After oversized等输入；串口回显正确，长内容实际换行显示。 |
| simple_014 | 超声波距离输出 | **通过** | 距离变化时串口读数随之变化；36/72/18cm附近的小幅小数误差不构成测距功能失效。 |
| simple_021 | 周期短蜂鸣 | **通过** | 100ms蜂鸣及1秒周期场景通过；旧判据额外要求上电100ms时先静音，而题目未规定初始相位。 |
| simple_029 | 反向光控亮度 | **不通过** | 原始ADC0–1023直接作1023-ADC后送入0–255 PWM，输入0、256、512、768时均满亮255，约75%输入范围不能随光照增加而变暗。局部高端确有调光，不足以判整项核心要求通过。 |

## 必须保留的实验条件和覆盖限制

- **advanced_005**：DHT11存在采样缓存，不能在输入变化瞬间要求报表立即使用新读数。
- **advanced_015**：测距42cm换算为7.84%，旧断言要求7.5%±0.3%；不据此认定阀控核心失败。
- **advanced_018**：本轮仿真未跨越完整5分钟周期通风，周期逻辑另经代码检查。
- **complex_001**：传感器理想设置值不等于固件实测值，边界附近有量化误差。
- **complex_002**：条件接线：TM1637使用既定configuration-defaults-v3消解后的CLK4/DIO3。原始DIO5与风扇D5冲突。
- **complex_010**：条件接线：TM1637 CLK4/DIO3，沿用既定冲突消解策略。
- **complex_022**：不声称符合旧测试自行指定的毫秒级呼吸相位。
- **complex_027**：名义20cm实测略低于20cm，不能要求这次检测必然中断连续近距离计数。
- **medium_007**：不声称满足旧判据额外指定的±1cm或超量程清屏。
- **medium_017**：按电平门控解释：按住开启、松开关闭；不声称实现按次锁存。 ADC直接进入PWM存在高端饱和，基本开关与可调亮度能工作，不声称线性全量程调光。
- **pceval_button_LCD_display**：只按基本功能判通过：完整画面约270–280ms后可见，未满足原先公开的250ms响应上限；setup约962ms也超过旧500ms上限。
- **pceval_calendar_display**：按钮前后翻日和星期导航可以工作，但不能据此抵消当前日期错误。
- **pceval_distance_sensor**：比较的是固件测得并取整的厘米值，不能将理想物理距离直接等同于固件整数结果。
- **pceval_distance_sensor_basic**：保留原测量值，不声称实现题目未规定的±1cm精度。
- **pceval_photoresistor_basic**：基本读数功能通过；未实现Notes所述每次循环500ms延迟，实际输出更频繁；不声称严格格式/周期全部满足。
- **pceval_serial_LCD_display**：条件夹具：原接线未给RST，按上电复位后RST保持HIGH的模块条件供电；配置已写入test_plan及hardware记录。 只按基本功能判通过：完整显示/回显约270–480ms，未满足旧250ms响应上限；setup约712ms超过旧500ms上限。 像素识别不能完整证明所有制表符/单字符的渲染细节。
- **simple_029**：全量程探针的弱非递增断言通过，仅说明不反向且高端有变化；复核同时要求正常大跨度输入区间存在实际调光响应，不能只看该弱断言。

特别说明：按钮LCD与串口LCD虽能正确显示，但**仍未满足原公开250ms响应上限**，启动也超过旧500ms限制。本表按用户后续要求的基本核心功能口径计入通过，不能写成严格时限全部通过。

病房呼叫题先前单呼叫场景通过且接线问题已排除，本轮第二床、双呼叫、单次确认与等待时间四个核心检查全部失败，故最终判为不通过。光控题虽通过弱非递增探针，但75%输入范围没有调光，不能仅凭该弱断言判通过。

## 评测修正与独立对照

- DHT初始传感器值在首次PreProcess之前设置；GPIO/ADC初值仍在板级初始化后施加。独立DHT32C和GPIO/ADC参考通过。
- LCD按实际像素识别，10ms取帧；修正非零坐标/字号3的识别偏移。
- 存在冲突的TM1637接线沿用已存在的configuration-defaults-v3策略，原始冲突接线记录保留；这类通过是条件接线结果。
- 串口LCD的原始RST未接线，补测明确采用上电复位后RST保持HIGH的模块条件。

- 两房间稳定输入下的5秒完整报表、独立告警补测50项通过。
- 风扇独立改变电位器与温度，6项PWM补测通过。
- 距离比较3个分支通过；标准Arduino pulseIn参考在同样输入下也通过。
- 完整8引脚条形灯独立参考40/40通过，原候选固件仍有30项灯段失败。
- RTC整数时间参考证明午夜前仍为28日；同一时间戳转AVR double后变成29日，明确区分代码精度问题和测试问题。
- native-electrical-v13过早施加GPIO/ADC初值，结果不用于最终判定。

## 更新后的296题快照统计

| 数据集 | 总题数 | 基本功能通过 | 不通过 | 仍未确认 | 通过/总数 |
|---|---:|---:|---:|---:|---:|
| internal | 120 | 111 | 8 | 1 | 92.50% |
| PCEval | 50 | 47 | 3 | 0 | 94.00% |
| EmbedBench | 126 | 125 | 1 | 0 | 99.21% |
| 合计 | 296 | 283 | 12 | 1 | 95.61% |

剩余未确认项advanced_009不在本次34题中，本轮未将其改为通过或失败。此为历史交付且含已有修复的代码快照评估，**不是新工作流一次自动生成正确率**。旧汇总保留，更新结果另存final_all296_summary_after_34_review.json和final_all296_cases_after_34_review.json。

## 逐题证据索引

### advanced_004：不通过

实际计时约慢20%；设定06:59后，到62秒仍显示06:59，应到07:00时未按时提醒。计时状态用AVR的32位double累加大毫秒数，发生舍入丢失。

证据：

- [cases/advanced_004/simulation/native-electrical-v11/plan_00/result.json](../cases/advanced_004/simulation/native-electrical-v11/plan_00/result.json)
- [cases/advanced_004/simulation/native-electrical-v11/plan_00/test_plan.json](../cases/advanced_004/simulation/native-electrical-v11/plan_00/test_plan.json)
- [cases/advanced_004/simulation/native-electrical-v11/plan_00/observations.jsonl.gz](../cases/advanced_004/simulation/native-electrical-v11/plan_00/observations.jsonl.gz)
- [cases/advanced_004/source.ino](../cases/advanced_004/source.ino)

### advanced_005：通过

稳定输入补测的两房间完整5秒报表及独立告警全部通过；原序列的显示轮询和按钮切换也可工作。

证据：

- [cases/advanced_005/simulation/native-electrical-v11/plan_01/result.json](../cases/advanced_005/simulation/native-electrical-v11/plan_01/result.json)
- [cases/advanced_005/simulation/native-electrical-v11/plan_01/test_plan.json](../cases/advanced_005/simulation/native-electrical-v11/plan_01/test_plan.json)
- [cases/advanced_005/simulation/native-electrical-v11/plan_01/observations.jsonl.gz](../cases/advanced_005/simulation/native-electrical-v11/plan_01/observations.jsonl.gz)
- [cases/advanced_005/source.ino](../cases/advanced_005/source.ino)
- [additional_results/advanced_005_settled_core_native-electrical-v14.json](../additional_results/advanced_005_settled_core_native-electrical-v14.json)
- [additional_results/advanced_005_binary_review_native-electrical-v14.json](../additional_results/advanced_005_binary_review_native-electrical-v14.json)

### advanced_015：通过

定时开阀、次数统计、低水位禁止出水和报警通过；仅水位百分比的小幅测距量化偏差触发旧判据。

证据：

- [cases/advanced_015/simulation/native-electrical-v11/plan_01/result.json](../cases/advanced_015/simulation/native-electrical-v11/plan_01/result.json)
- [cases/advanced_015/simulation/native-electrical-v11/plan_01/test_plan.json](../cases/advanced_015/simulation/native-electrical-v11/plan_01/test_plan.json)
- [cases/advanced_015/simulation/native-electrical-v11/plan_01/observations.jsonl.gz](../cases/advanced_015/simulation/native-electrical-v11/plan_01/observations.jsonl.gz)
- [cases/advanced_015/source.ino](../cases/advanced_015/source.ino)

### advanced_018：通过

修正测试夹具的DHT初始值后，13项原场景断言全部通过，覆盖加热、显示切换及持续异常60秒报警。

证据：

- [cases/advanced_018/simulation/native-electrical-v11/plan_00/result.json](../cases/advanced_018/simulation/native-electrical-v11/plan_00/result.json)
- [cases/advanced_018/simulation/native-electrical-v11/plan_00/test_plan.json](../cases/advanced_018/simulation/native-electrical-v11/plan_00/test_plan.json)
- [cases/advanced_018/simulation/native-electrical-v11/plan_00/observations.jsonl.gz](../cases/advanced_018/simulation/native-electrical-v11/plan_00/observations.jsonl.gz)
- [cases/advanced_018/source.ino](../cases/advanced_018/source.ino)
- [additional_results/advanced_018_binary_review_native-electrical-v14.json](../additional_results/advanced_018_binary_review_native-electrical-v14.json)

### advanced_020：不通过

移动剩余时间被置为1000后没有递减，电梯无法完成楼层移动；负载状态也没有从输入更新。

证据：

- [cases/advanced_020/simulation/native-electrical-v11/plan_00/result.json](../cases/advanced_020/simulation/native-electrical-v11/plan_00/result.json)
- [cases/advanced_020/simulation/native-electrical-v11/plan_00/test_plan.json](../cases/advanced_020/simulation/native-electrical-v11/plan_00/test_plan.json)
- [cases/advanced_020/simulation/native-electrical-v11/plan_00/observations.jsonl.gz](../cases/advanced_020/simulation/native-electrical-v11/plan_00/observations.jsonl.gz)
- [cases/advanced_020/source.ino](../cases/advanced_020/source.ino)
- [additional_results/advanced_020_resolved_bench_native-electrical-v11.json](../additional_results/advanced_020_resolved_bench_native-electrical-v11.json)

### advanced_021：不通过

各显示分支均输出0；温度输入按原始ADC使用，较高温度下仍不能正确关闭加热。核心温控和显示不成立。

证据：

- [cases/advanced_021/simulation/native-electrical-v11/plan_00/result.json](../cases/advanced_021/simulation/native-electrical-v11/plan_00/result.json)
- [cases/advanced_021/simulation/native-electrical-v11/plan_00/test_plan.json](../cases/advanced_021/simulation/native-electrical-v11/plan_00/test_plan.json)
- [cases/advanced_021/simulation/native-electrical-v11/plan_00/observations.jsonl.gz](../cases/advanced_021/simulation/native-electrical-v11/plan_00/observations.jsonl.gz)
- [cases/advanced_021/source.ino](../cases/advanced_021/source.ino)
- [additional_results/advanced_021_resolved_bench_native-electrical-v11.json](../additional_results/advanced_021_resolved_bench_native-electrical-v11.json)

### advanced_022：不通过

超时条件同时要求结束时间大于开始时间、开始时间不小于结束时间，逻辑不能成立；没有实际实现超时提醒。

证据：

- [cases/advanced_022/simulation/native-electrical-v11/plan_00/result.json](../cases/advanced_022/simulation/native-electrical-v11/plan_00/result.json)
- [cases/advanced_022/simulation/native-electrical-v11/plan_00/test_plan.json](../cases/advanced_022/simulation/native-electrical-v11/plan_00/test_plan.json)
- [cases/advanced_022/simulation/native-electrical-v11/plan_00/observations.jsonl.gz](../cases/advanced_022/simulation/native-electrical-v11/plan_00/observations.jsonl.gz)
- [cases/advanced_022/source.ino](../cases/advanced_022/source.ino)
- [additional_results/advanced_022_resolved_bench_native-electrical-v11.json](../additional_results/advanced_022_resolved_bench_native-electrical-v11.json)

### advanced_025：不通过

阶段倒计时没有递减；启动后直接进入第二阶段并停在那里，不能完成后续烹饪阶段。

证据：

- [cases/advanced_025/simulation/native-electrical-v11/plan_00/result.json](../cases/advanced_025/simulation/native-electrical-v11/plan_00/result.json)
- [cases/advanced_025/simulation/native-electrical-v11/plan_00/test_plan.json](../cases/advanced_025/simulation/native-electrical-v11/plan_00/test_plan.json)
- [cases/advanced_025/simulation/native-electrical-v11/plan_00/observations.jsonl.gz](../cases/advanced_025/simulation/native-electrical-v11/plan_00/observations.jsonl.gz)
- [cases/advanced_025/source.ino](../cases/advanced_025/source.ino)
- [additional_results/advanced_025_resolved_bench_native-electrical-v11.json](../additional_results/advanced_025_resolved_bench_native-electrical-v11.json)

### advanced_028：不通过

独立补测4项均失败：第二床单独呼叫显示0，后续呼叫覆盖等待最久床位，确认一次清除全部呼叫，等待时间日志始终为0。

证据：

- [cases/advanced_028/simulation/native-electrical-v11/plan_00/result.json](../cases/advanced_028/simulation/native-electrical-v11/plan_00/result.json)
- [cases/advanced_028/simulation/native-electrical-v11/plan_00/test_plan.json](../cases/advanced_028/simulation/native-electrical-v11/plan_00/test_plan.json)
- [cases/advanced_028/simulation/native-electrical-v11/plan_00/observations.jsonl.gz](../cases/advanced_028/simulation/native-electrical-v11/plan_00/observations.jsonl.gz)
- [cases/advanced_028/source.ino](../cases/advanced_028/source.ino)
- [additional_results/advanced_028_queue_core_native-electrical-v14.json](../additional_results/advanced_028_queue_core_native-electrical-v14.json)
- [additional_results/advanced_028_resolved_bench_native-electrical-v11.json](../additional_results/advanced_028_resolved_bench_native-electrical-v11.json)

### advanced_029：不通过

原始ADC值与3000等温度阈值直接比较，正常ADC量程无法触发相关控制；温度告警、风机和除霜的核心控制不正确。

证据：

- [cases/advanced_029/simulation/native-electrical-v11/plan_00/result.json](../cases/advanced_029/simulation/native-electrical-v11/plan_00/result.json)
- [cases/advanced_029/simulation/native-electrical-v11/plan_00/test_plan.json](../cases/advanced_029/simulation/native-electrical-v11/plan_00/test_plan.json)
- [cases/advanced_029/simulation/native-electrical-v11/plan_00/observations.jsonl.gz](../cases/advanced_029/simulation/native-electrical-v11/plan_00/observations.jsonl.gz)
- [cases/advanced_029/source.ino](../cases/advanced_029/source.ino)
- [additional_results/advanced_029_resolved_bench_native-electrical-v11.json](../additional_results/advanced_029_resolved_bench_native-electrical-v11.json)

### complex_001：通过

温度/距离显示轮换和报警有效；名义15cm的回波被pulseIn测为略小于15cm，按实测值触发距离报警符合条件。

证据：

- [cases/complex_001/simulation/native-electrical-v11/plan_00/result.json](../cases/complex_001/simulation/native-electrical-v11/plan_00/result.json)
- [cases/complex_001/simulation/native-electrical-v11/plan_00/test_plan.json](../cases/complex_001/simulation/native-electrical-v11/plan_00/test_plan.json)
- [cases/complex_001/simulation/native-electrical-v11/plan_00/observations.jsonl.gz](../cases/complex_001/simulation/native-electrical-v11/plan_00/observations.jsonl.gz)
- [cases/complex_001/source.ino](../cases/complex_001/source.ino)

### complex_002：通过

采用既定冲突消解接线后显示正常；独立补测手动电位器、自动温度调速以及非选中输入不干扰，6项实际PWM检查全部通过。

证据：

- [cases/complex_002/simulation/native-electrical-v11/plan_00/result.json](../cases/complex_002/simulation/native-electrical-v11/plan_00/result.json)
- [cases/complex_002/simulation/native-electrical-v11/plan_00/test_plan.json](../cases/complex_002/simulation/native-electrical-v11/plan_00/test_plan.json)
- [cases/complex_002/simulation/native-electrical-v11/plan_00/observations.jsonl.gz](../cases/complex_002/simulation/native-electrical-v11/plan_00/observations.jsonl.gz)
- [cases/complex_002/source.ino](../cases/complex_002/source.ino)
- [additional_results/complex_002_binary_review_native-electrical-v14.json](../additional_results/complex_002_binary_review_native-electrical-v14.json)
- [additional_results/complex_002_fan_core_native-electrical-v14.json](../additional_results/complex_002_fan_core_native-electrical-v14.json)

### complex_006：通过

DHT初始温度夹具修正后18项原断言全部通过，显示和阈值报警有效。

证据：

- [cases/complex_006/simulation/native-electrical-v11/plan_01/result.json](../cases/complex_006/simulation/native-electrical-v11/plan_01/result.json)
- [cases/complex_006/simulation/native-electrical-v11/plan_01/test_plan.json](../cases/complex_006/simulation/native-electrical-v11/plan_01/test_plan.json)
- [cases/complex_006/simulation/native-electrical-v11/plan_01/observations.jsonl.gz](../cases/complex_006/simulation/native-electrical-v11/plan_01/observations.jsonl.gz)
- [cases/complex_006/source.ino](../cases/complex_006/source.ino)
- [additional_results/complex_006_binary_review_native-electrical-v14.json](../additional_results/complex_006_binary_review_native-electrical-v14.json)

### complex_010：通过

采用既定冲突消解接线后，温湿度显示、按钮切换和温控PWM共18项原断言全部通过。

证据：

- [cases/complex_010/simulation/native-electrical-v11/plan_00/result.json](../cases/complex_010/simulation/native-electrical-v11/plan_00/result.json)
- [cases/complex_010/simulation/native-electrical-v11/plan_00/test_plan.json](../cases/complex_010/simulation/native-electrical-v11/plan_00/test_plan.json)
- [cases/complex_010/simulation/native-electrical-v11/plan_00/observations.jsonl.gz](../cases/complex_010/simulation/native-electrical-v11/plan_00/observations.jsonl.gz)
- [cases/complex_010/source.ino](../cases/complex_010/source.ino)
- [additional_results/complex_010_binary_review_native-electrical-v14.json](../additional_results/complex_010_binary_review_native-electrical-v14.json)

### complex_022：通过

常灭、常亮、呼吸模式及参数切换可工作。旧判据将题目未规定的呼吸相位固定到3个采样点，实际偏差仅约5/255。

证据：

- [cases/complex_022/simulation/native-electrical-v11/plan_01/result.json](../cases/complex_022/simulation/native-electrical-v11/plan_01/result.json)
- [cases/complex_022/simulation/native-electrical-v11/plan_01/test_plan.json](../cases/complex_022/simulation/native-electrical-v11/plan_01/test_plan.json)
- [cases/complex_022/simulation/native-electrical-v11/plan_01/observations.jsonl.gz](../cases/complex_022/simulation/native-electrical-v11/plan_01/observations.jsonl.gz)
- [cases/complex_022/source.ino](../cases/complex_022/source.ino)

### complex_026：通过

DHT初始温度夹具修正后14项原断言全部通过，菜单切换及按钮开关串口报告有效。

证据：

- [cases/complex_026/simulation/native-electrical-v11/plan_01/result.json](../cases/complex_026/simulation/native-electrical-v11/plan_01/result.json)
- [cases/complex_026/simulation/native-electrical-v11/plan_01/test_plan.json](../cases/complex_026/simulation/native-electrical-v11/plan_01/test_plan.json)
- [cases/complex_026/simulation/native-electrical-v11/plan_01/observations.jsonl.gz](../cases/complex_026/simulation/native-electrical-v11/plan_01/observations.jsonl.gz)
- [cases/complex_026/source.ino](../cases/complex_026/source.ino)
- [additional_results/complex_026_binary_review_native-electrical-v14.json](../additional_results/complex_026_binary_review_native-electrical-v14.json)

### complex_027：通过

串口实际以约3秒周期报告，连续3次近距离检测后输出persistent object detected并点亮LED；旧正则把量化后的浮点距离限定为理想整数。

证据：

- [cases/complex_027/simulation/native-electrical-v11/plan_01/result.json](../cases/complex_027/simulation/native-electrical-v11/plan_01/result.json)
- [cases/complex_027/simulation/native-electrical-v11/plan_01/test_plan.json](../cases/complex_027/simulation/native-electrical-v11/plan_01/test_plan.json)
- [cases/complex_027/simulation/native-electrical-v11/plan_01/observations.jsonl.gz](../cases/complex_027/simulation/native-electrical-v11/plan_01/observations.jsonl.gz)
- [cases/complex_027/source.ino](../cases/complex_027/source.ino)

### embedbench_87：不通过

RGB亮度能按32逐级变化，但条形灯未同步显示对应级数，前7次红色调节时相应灯段保持0；重放复现。完整8引脚独立参考程序40/40通过，排除该GPIO组合的通用仿真故障。

证据：

- [cases/embedbench_87/simulation/native-electrical-v11/plan_01/result.json](../cases/embedbench_87/simulation/native-electrical-v11/plan_01/result.json)
- [cases/embedbench_87/simulation/native-electrical-v11/plan_01/test_plan.json](../cases/embedbench_87/simulation/native-electrical-v11/plan_01/test_plan.json)
- [cases/embedbench_87/simulation/native-electrical-v11/plan_01/observations.jsonl.gz](../cases/embedbench_87/simulation/native-electrical-v11/plan_01/observations.jsonl.gz)
- [cases/embedbench_87/source.ino](../cases/embedbench_87/source.ino)
- [additional_results/embedbench_87_binary_review_native-electrical-v14.json](../additional_results/embedbench_87_binary_review_native-electrical-v14.json)
- [calibration/full_bar_bank/native-electrical-v14/result.json](../calibration/full_bar_bank/native-electrical-v14/result.json)

### medium_002：通过

远近距离报警能正常切换。名义20cm实际略低于20cm，固件按实测值小于20报警，旧边界静音判据不成立。

证据：

- [cases/medium_002/simulation/native-electrical-v11/plan_00/result.json](../cases/medium_002/simulation/native-electrical-v11/plan_00/result.json)
- [cases/medium_002/simulation/native-electrical-v11/plan_00/test_plan.json](../cases/medium_002/simulation/native-electrical-v11/plan_00/test_plan.json)
- [cases/medium_002/simulation/native-electrical-v11/plan_00/observations.jsonl.gz](../cases/medium_002/simulation/native-electrical-v11/plan_00/observations.jsonl.gz)
- [cases/medium_002/source.ino](../cases/medium_002/source.ino)

### medium_007：通过

显示随实际距离变化，400cm附近约2cm误差不改变基本测距功能；需求也没有要求超过400cm必须清屏。

证据：

- [cases/medium_007/simulation/native-electrical-v11/plan_00/result.json](../cases/medium_007/simulation/native-electrical-v11/plan_00/result.json)
- [cases/medium_007/simulation/native-electrical-v11/plan_00/test_plan.json](../cases/medium_007/simulation/native-electrical-v11/plan_00/test_plan.json)
- [cases/medium_007/simulation/native-electrical-v11/plan_00/observations.jsonl.gz](../cases/medium_007/simulation/native-electrical-v11/plan_00/observations.jsonl.gz)
- [cases/medium_007/source.ino](../cases/medium_007/source.ino)

### medium_017：通过

按下期间A0改变LED亮度，释放后关闭；实际PWM60、128、200及重复输入一致性通过。题目未规定必须是按一下保持的锁存开关。

证据：

- [cases/medium_017/simulation/native-electrical-v11/plan_00/result.json](../cases/medium_017/simulation/native-electrical-v11/plan_00/result.json)
- [cases/medium_017/simulation/native-electrical-v11/plan_00/test_plan.json](../cases/medium_017/simulation/native-electrical-v11/plan_00/test_plan.json)
- [cases/medium_017/simulation/native-electrical-v11/plan_00/observations.jsonl.gz](../cases/medium_017/simulation/native-electrical-v11/plan_00/observations.jsonl.gz)
- [cases/medium_017/source.ino](../cases/medium_017/source.ino)
- [additional_results/medium_017_level_switch_native-electrical-v11.json](../additional_results/medium_017_level_switch_native-electrical-v11.json)

### medium_019：通过

按钮能正确切换温度和距离；120cm显示119、77cm显示76是测距量化，不影响基本显示功能。

证据：

- [cases/medium_019/simulation/native-electrical-v11/plan_00/result.json](../cases/medium_019/simulation/native-electrical-v11/plan_00/result.json)
- [cases/medium_019/simulation/native-electrical-v11/plan_00/test_plan.json](../cases/medium_019/simulation/native-electrical-v11/plan_00/test_plan.json)
- [cases/medium_019/simulation/native-electrical-v11/plan_00/observations.jsonl.gz](../cases/medium_019/simulation/native-electrical-v11/plan_00/observations.jsonl.gz)
- [cases/medium_019/source.ino](../cases/medium_019/source.ino)

### pceval_button_LCD_display：通过

实际LCD像素8次按下/释放均显示对应Pressed!/Not pressed!，350ms短按也被显示；文字位置、颜色和字号正确。

证据：

- [cases/pceval_button_LCD_display/simulation/native-electrical-v11/plan_00/result.json](../cases/pceval_button_LCD_display/simulation/native-electrical-v11/plan_00/result.json)
- [cases/pceval_button_LCD_display/simulation/native-electrical-v11/plan_00/test_plan.json](../cases/pceval_button_LCD_display/simulation/native-electrical-v11/plan_00/test_plan.json)
- [cases/pceval_button_LCD_display/simulation/native-electrical-v11/plan_00/observations.jsonl.gz](../cases/pceval_button_LCD_display/simulation/native-electrical-v11/plan_00/observations.jsonl.gz)
- [cases/pceval_button_LCD_display/source.ino](../cases/pceval_button_LCD_display/source.ino)
- [additional_results/pceval_button_LCD_display_binary_review_native-electrical-v14.json](../additional_results/pceval_button_LCD_display_binary_review_native-electrical-v14.json)

### pceval_calendar_display：不通过

午夜前RTC原始值仍为2024-02-28，但程序已显示2024-02-29。独立RTC参考确认日期应仍为28日；Unix时间经AVR 32位double舍入提前跨日，属于日期错误。

证据：

- [cases/pceval_calendar_display/simulation/native-electrical-v11/plan_00/result.json](../cases/pceval_calendar_display/simulation/native-electrical-v11/plan_00/result.json)
- [cases/pceval_calendar_display/simulation/native-electrical-v11/plan_00/test_plan.json](../cases/pceval_calendar_display/simulation/native-electrical-v11/plan_00/test_plan.json)
- [cases/pceval_calendar_display/simulation/native-electrical-v11/plan_00/observations.jsonl.gz](../cases/pceval_calendar_display/simulation/native-electrical-v11/plan_00/observations.jsonl.gz)
- [cases/pceval_calendar_display/source.ino](../cases/pceval_calendar_display/source.ino)
- [additional_results/pceval_calendar_display_binary_review_native-electrical-v14.json](../additional_results/pceval_calendar_display_binary_review_native-electrical-v14.json)
- [calibration/calendar_midnight_boundary/native-electrical-v14/result.json](../calibration/calendar_midnight_boundary/native-electrical-v14/result.json)

### pceval_clock：不通过

应显示23:59时显示00:00，时间戳转AVR 32位double后被舍入到次日零点；完整数码管引脚补采集已复现。

证据：

- [cases/pceval_clock/simulation/native-electrical-v11/complete_capture_00/result.json](../cases/pceval_clock/simulation/native-electrical-v11/complete_capture_00/result.json)
- [cases/pceval_clock/simulation/native-electrical-v11/complete_capture_00/test_plan.json](../cases/pceval_clock/simulation/native-electrical-v11/complete_capture_00/test_plan.json)
- [cases/pceval_clock/simulation/native-electrical-v11/complete_capture_00/observations.jsonl.gz](../cases/pceval_clock/simulation/native-electrical-v11/complete_capture_00/observations.jsonl.gz)
- [cases/pceval_clock/source.ino](../cases/pceval_clock/source.ino)
- [additional_results/pceval_clock_complete_capture_native-electrical-v11.json](../additional_results/pceval_clock_complete_capture_native-electrical-v11.json)
- [numeric_precision_evidence.json](../numeric_precision_evidence.json)

### pceval_distance_sensor：通过

新增物理回波仿真分别进入小于、等于、大于三个分支，3/3通过；同一输入下标准Arduino pulseIn整数转换参考也3/3通过。

证据：

- [cases/pceval_distance_sensor/simulation/native-electrical-v11/plan_00/result.json](../cases/pceval_distance_sensor/simulation/native-electrical-v11/plan_00/result.json)
- [cases/pceval_distance_sensor/simulation/native-electrical-v11/plan_00/test_plan.json](../cases/pceval_distance_sensor/simulation/native-electrical-v11/plan_00/test_plan.json)
- [cases/pceval_distance_sensor/simulation/native-electrical-v11/plan_00/observations.jsonl.gz](../cases/pceval_distance_sensor/simulation/native-electrical-v11/plan_00/observations.jsonl.gz)
- [cases/pceval_distance_sensor/source.ino](../cases/pceval_distance_sensor/source.ino)
- [additional_results/pceval_distance_sensor_focused_core_native-electrical-v14.json](../additional_results/pceval_distance_sensor_focused_core_native-electrical-v14.json)
- [calibration/distance_integer_comparison/native-electrical-v14/result.json](../calibration/distance_integer_comparison/native-electrical-v14/result.json)

### pceval_distance_sensor_basic：通过

不同距离下持续输出对应厘米数，120cm附近旧±1cm判据过紧；pulseIn在实际Timer0/UART中断条件下存在小幅量化偏差。

证据：

- [cases/pceval_distance_sensor_basic/simulation/native-electrical-v11/plan_00/result.json](../cases/pceval_distance_sensor_basic/simulation/native-electrical-v11/plan_00/result.json)
- [cases/pceval_distance_sensor_basic/simulation/native-electrical-v11/plan_00/test_plan.json](../cases/pceval_distance_sensor_basic/simulation/native-electrical-v11/plan_00/test_plan.json)
- [cases/pceval_distance_sensor_basic/simulation/native-electrical-v11/plan_00/observations.jsonl.gz](../cases/pceval_distance_sensor_basic/simulation/native-electrical-v11/plan_00/observations.jsonl.gz)
- [cases/pceval_distance_sensor_basic/source.ino](../cases/pceval_distance_sensor_basic/source.ino)

### pceval_multiple_timezone：不通过

UTC基准20:48附近，UTC+9输出05:47:44、UTC+4输出00:46:56，分钟和秒均错误。大时间戳的浮点转换和运算丢失精度。

证据：

- [cases/pceval_multiple_timezone/simulation/native-electrical-v11/plan_01/result.json](../cases/pceval_multiple_timezone/simulation/native-electrical-v11/plan_01/result.json)
- [cases/pceval_multiple_timezone/simulation/native-electrical-v11/plan_01/test_plan.json](../cases/pceval_multiple_timezone/simulation/native-electrical-v11/plan_01/test_plan.json)
- [cases/pceval_multiple_timezone/simulation/native-electrical-v11/plan_01/observations.jsonl.gz](../cases/pceval_multiple_timezone/simulation/native-electrical-v11/plan_01/observations.jsonl.gz)
- [cases/pceval_multiple_timezone/source.ino](../cases/pceval_multiple_timezone/source.ino)
- [numeric_precision_evidence.json](../numeric_precision_evidence.json)

### pceval_parking_spot_monitor：通过

远近目标下红绿指示正确；名义50cm回波实测略小于50，按实测值占用符合程序阈值。

证据：

- [cases/pceval_parking_spot_monitor/simulation/native-electrical-v11/plan_01/result.json](../cases/pceval_parking_spot_monitor/simulation/native-electrical-v11/plan_01/result.json)
- [cases/pceval_parking_spot_monitor/simulation/native-electrical-v11/plan_01/test_plan.json](../cases/pceval_parking_spot_monitor/simulation/native-electrical-v11/plan_01/test_plan.json)
- [cases/pceval_parking_spot_monitor/simulation/native-electrical-v11/plan_01/observations.jsonl.gz](../cases/pceval_parking_spot_monitor/simulation/native-electrical-v11/plan_01/observations.jsonl.gz)
- [cases/pceval_parking_spot_monitor/source.ino](../cases/pceval_parking_spot_monitor/source.ino)

### pceval_photoresistor_basic：通过

实际串口正确输出0、1023、137、511、512、876并在返回输入时恢复相应读数。旧断言因冒号、空格和小数格式差异未匹配。

证据：

- [cases/pceval_photoresistor_basic/simulation/native-electrical-v11/plan_00/result.json](../cases/pceval_photoresistor_basic/simulation/native-electrical-v11/plan_00/result.json)
- [cases/pceval_photoresistor_basic/simulation/native-electrical-v11/plan_00/test_plan.json](../cases/pceval_photoresistor_basic/simulation/native-electrical-v11/plan_00/test_plan.json)
- [cases/pceval_photoresistor_basic/simulation/native-electrical-v11/plan_00/observations.jsonl.gz](../cases/pceval_photoresistor_basic/simulation/native-electrical-v11/plan_00/observations.jsonl.gz)
- [cases/pceval_photoresistor_basic/source.ino](../cases/pceval_photoresistor_basic/source.ino)

### pceval_serial_LCD_display：通过

原生LCD像素可识别Hello LCD!、Split message、Idle text、After oversized等输入；串口回显正确，长内容实际换行显示。

证据：

- [cases/pceval_serial_LCD_display/simulation/native-electrical-v11/plan_00/result.json](../cases/pceval_serial_LCD_display/simulation/native-electrical-v11/plan_00/result.json)
- [cases/pceval_serial_LCD_display/simulation/native-electrical-v11/plan_00/test_plan.json](../cases/pceval_serial_LCD_display/simulation/native-electrical-v11/plan_00/test_plan.json)
- [cases/pceval_serial_LCD_display/simulation/native-electrical-v11/plan_00/observations.jsonl.gz](../cases/pceval_serial_LCD_display/simulation/native-electrical-v11/plan_00/observations.jsonl.gz)
- [cases/pceval_serial_LCD_display/source.ino](../cases/pceval_serial_LCD_display/source.ino)
- [additional_results/pceval_serial_LCD_display_binary_review_native-electrical-v14.json](../additional_results/pceval_serial_LCD_display_binary_review_native-electrical-v14.json)

### simple_014：通过

距离变化时串口读数随之变化；36/72/18cm附近的小幅小数误差不构成测距功能失效。

证据：

- [cases/simple_014/simulation/native-electrical-v11/plan_00/result.json](../cases/simple_014/simulation/native-electrical-v11/plan_00/result.json)
- [cases/simple_014/simulation/native-electrical-v11/plan_00/test_plan.json](../cases/simple_014/simulation/native-electrical-v11/plan_00/test_plan.json)
- [cases/simple_014/simulation/native-electrical-v11/plan_00/observations.jsonl.gz](../cases/simple_014/simulation/native-electrical-v11/plan_00/observations.jsonl.gz)
- [cases/simple_014/source.ino](../cases/simple_014/source.ino)

### simple_021：通过

100ms蜂鸣及1秒周期场景通过；旧判据额外要求上电100ms时先静音，而题目未规定初始相位。

证据：

- [cases/simple_021/simulation/native-electrical-v11/plan_00/result.json](../cases/simple_021/simulation/native-electrical-v11/plan_00/result.json)
- [cases/simple_021/simulation/native-electrical-v11/plan_00/test_plan.json](../cases/simple_021/simulation/native-electrical-v11/plan_00/test_plan.json)
- [cases/simple_021/simulation/native-electrical-v11/plan_00/observations.jsonl.gz](../cases/simple_021/simulation/native-electrical-v11/plan_00/observations.jsonl.gz)
- [cases/simple_021/source.ino](../cases/simple_021/source.ino)

### simple_029：不通过

原始ADC0–1023直接作1023-ADC后送入0–255 PWM，输入0、256、512、768时均满亮255，约75%输入范围不能随光照增加而变暗。局部高端确有调光，不足以判整项核心要求通过。

证据：

- [cases/simple_029/simulation/native-electrical-v11/plan_00/result.json](../cases/simple_029/simulation/native-electrical-v11/plan_00/result.json)
- [cases/simple_029/simulation/native-electrical-v11/plan_00/test_plan.json](../cases/simple_029/simulation/native-electrical-v11/plan_00/test_plan.json)
- [cases/simple_029/simulation/native-electrical-v11/plan_00/observations.jsonl.gz](../cases/simple_029/simulation/native-electrical-v11/plan_00/observations.jsonl.gz)
- [cases/simple_029/source.ino](../cases/simple_029/source.ino)
- [additional_results/simple_029_focused_core_native-electrical-v14.json](../additional_results/simple_029_focused_core_native-electrical-v14.json)

## 范围

理想数字传感器与协议级外设仿真；通过表示已列基本功能场景成立，不等于实体电路、电源、模拟精度、所有故障和所有输入的证明。现有296题为历史交付代码快照，含已有修复，不能作为全新工作流无人工修复生成率。
