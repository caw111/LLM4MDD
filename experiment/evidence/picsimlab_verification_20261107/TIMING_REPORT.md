# PICSimLab 后台计时试跑（2026-11-07）

范围：当前冻结的 296 份交付代码，不包括其他直接生成/消融版本。仅完成小批计时，未启动全量仿真。

运行方式：WSL Ubuntu 内的官方 PICSimLab 0.9.3 Linux64_NOGUI；DISPLAY/WAYLAND_DISPLAY/PULSE_SERVER 清空、声音禁用；原生 Linux Arduino CLI 和 AVR GCC；单任务、单 CPU 核心、nice=15。Windows 启动使用 windowsHide=true，无 detached 控制台，不运行 PowerShell 或 Windows 编译工具。模拟器进程实测 RSS 约 14 MB；该数字不包含整个 WSL 虚拟机内存。

## 实际结果

|题目|覆盖场景|断言数|程序时间|墙钟时间|结果|
|---|---|---:|---:|---:|---|
|embedbench_19|LED 分段亮灭|3|5.584 s|17.585 s|场景通过|
|embedbench_5|按钮切换 LED|2|6.057 s|18.844 s|场景通过|
|embedbench_1|双 LED 定时顺序|8|17.106 s|48.008 s|场景通过|

合计 3 题、13 个场景断言通过。输入由 PICSimLab 的理想按钮模型驱动，直接运行已冻结的 AVR HEX；不修改候选程序。保留原始 TCP 命令日志、GPIO 波形、timer0_millis 低 16 位和方向寄存器波形。三题最长不到 18 秒，因此低 16 位时钟不会回绕。按同步波形中的程序时钟进行采样；现场 TCP 快照与延迟也单独保留。

校准期间发现 pinsl 复位后方向字段可能滞后，以及重复暂停导致采样延迟，已改用真实 DDR 寄存器与同步波形检查。场景通过不代表整题的全部需求已覆盖。原始计划均为 partial，当前试跑亦不计入论文原有的整题功能正确率。

原生编译计时：embedbench_3 52.14s；simple_002 50.26s；simple_003 48.48s，三题均通过。加上先前保存的 14 题，本轮共有 17 份编译成功记录。

## 全量耗时估算

283 题保存了共 430 份测试计划，虚拟时间合计 9363 秒（约 2.60 小时）。实测墙钟/程序时间约 2.94 倍，线性外推约 7.6 小时仿真。剩余 279 题编译按当前样例均值估算约 3.9 小时。两者合计约 11.5 小时；考虑小样本和日常电脑负载，准备齐全后的一轮批量运行暂估 10–15 小时。

该估算不包含外设模型开发与缺失用例设计。66 题需求提及 TM1637，当前 PICSimLab 可用部件列表没有原生 TM1637 模型；13 题缺少保存的测试计划，两项有重叠（合并 72 题）。其他已有外设与断言类型的适配也尚未全部完成。因此，当前不能承诺从现在起 10–15 小时内完成全部 296 题的完整功能仿真；不能把未支持/未验证算作通过或代码失败。

证据：runtime_estimate.json；gpio_pilot_summary.json；cases/<id>/picsimlab_pilot/{result.json,remote_commands.json,gpio.vcd,millis.vcd,ddre.vcd}；cases/<id>/compilation.json。
