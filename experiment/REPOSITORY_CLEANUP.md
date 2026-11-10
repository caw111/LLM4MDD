# 2026-11-07 仓库整理记录

保留当前 FastAPI/React 应用、接口契约和 SysML 语义反馈实现、回归测试、三套数据集的有效输入，以及支撑最终表格的历史对比、消融和代码修复材料。

原 experiment/output/ 的约 69 万个文件和 11GB 内容被提取为 results/ 的去重材料包。原文件未销毁，连同临时截图、重复数据、旧嵌套仓库、冗余实验输出和原暂存区索引，移入本地仓库相邻的 LLM4MDD_archive_20261107/。虚拟环境、工具链、模型密钥和前端依赖由忽略规则排除，仍可供本机使用。Git 历史保留。

results/ 内的原始执行 JSON、代码、差异和压缩轨迹保留来源信息；新索引使用相对路径。generation_artifacts.json 保留现存契约、模型、IR/IL 和内核文件的映射，method_snapshots/ 保留历史方法源码。

原始计划和已完成结果分别标注。旧计划中的五轮独立测试集以及消融三轮的全部记录均未完成，不能由方案文档推断其结果。最终 284/296 的来源及判定类型详见 RESULTS.md。此次整理未重新调用模型或修改实验草图。

整理验证：前端生产构建成功；SysML 语义审查 17 项、配置策略 17 项、数据路径回归 1 项通过。硬件契约测试 5 项中 4 项通过，1 项因真实 Java SysML 校验器超过 90 秒而失败，未将其记为通过。

抽查 complex_019、pceval_multiplexer_photoresistor、advanced_002、simple_010、pceval_button_buzzer 的保存输入，复现结果与保存的执行状态一致。前两题与 simple_010 断言通过；advanced_002 仍为无自动断言执行，pceval_button_buzzer 仍保留原断言失败，两者的核心结论依赖保存的需求复核。没有新增 SimulIDE 或实物验证。

complex_019 的修复源码另经 arduino-cli 对 Arduino Uno 重新编译通过。
