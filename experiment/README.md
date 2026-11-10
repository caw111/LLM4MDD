# 实验与复现

[历史实验与主机复测](RESULTS.md) 收录早期评测和累计补齐数据。results/ 内保留原始判定与来源信息，并提供以相对路径组织的复测索引。

## 数据

[实验材料索引](evidence/README.md) 提供后续 PICSimLab 结果、补充指标、案例和原始轨迹下载方式。

- input/ 保留四种难度各 30 题的本地需求。
- formal/manifest.json 保留共 296 题的有效需求、数据来源和目标开发板。
- results/groups.json 保留 13 组各 296 题的源码及预处理代码索引。
- results/final_cases.json 保留 296 题最终选用代码、来源、测试计划和执行结果索引。
- results/artifacts/ 按文件 SHA-256 去重保存源码。
- results/generation_artifacts.json 索引各批次现存的接口契约、模型和编译中间文件。
- results/cases/ 保留原始断言结果、修改说明、差异、编译日志及压缩执行轨迹。
- results/summary/ 保留历史比较、单轮消融、固定 16 题和最终修复的原始汇总。
- results/checksums.json 用于逐文件完整性校验。

原始 JSON 的绝对路径属于来源记录。跨机器访问使用 groups.json 和 final_cases.json 中的相对路径。trace.jsonl.gz 解压后得到原始逐行 JSON 轨迹。

## 离线复测

```powershell
python experiment/code/replay_results.py
python experiment/code/replay_results.py --case complex_019
python experiment/code/replay_results.py --case pceval_clock --case pceval_exercise_counter
```

第一条核对保存结果和文件哈希。带 --case 时，脚本用生成代码的预处理 C++ 文件编译主机程序，执行保存的输入和断言，输出本次结果。通过 CXX 指定 C++17 g++ 编译器。--compile 另调用 PATH 中的 arduino-cli 重做目标板编译。

主机复测使用虚拟时间与外设接口模型。原有失败断言与依据需求补充的断言分别保存，复测入口执行记录中已确认通过的计划，便于核验交付结果。完整原始判定仍在 recorded_result.json 中。

历史核心通过包含不同证据类型：257 题有通过的自动测试计划，21 题来自无自动断言的轨迹与需求复核，5 题在保留原断言失败后经核心功能复核通过，1 题来自已有电路观察。相应 requirement_review.json 和原始执行结果同时保留。对后两类主机记录，复测只确认执行或原失败可重现，会明确输出仍需参考人工复核；不会把原始失败改成自动断言通过。simple_001 仅有历史电路观察，不能使用 --case 自动复测。

formal/manifest.json 的 split、repetitions 等字段来自早期独立测试集计划，当前结果使用全部 296 题。该计划保存在 protocols/FORMAL_DRAFT.md，不代表已完成五轮实验。

## 重新生成与修复实验

code/run_current_comparison.py 为工作流与直接生成的对比入口。code/run_paper_ablation.py 为组件消融入口。它们依赖后端、模型配置、SysML 和 Arduino 工具链。

analysis/manual_sketch_patch_rules.py 保存逐题代码调整，analysis/manual_core_test_plans.py 保存补充输入和断言。analysis/run_manual_code_repairs.py 等脚本记录历史修复运行过程，其历史输入目录已归档；发布包的离线复测使用 code/replay_results.py。

## 统计范围

固定单轮消融覆盖 296 题 × 7 组，目标板编译结果完整，独立功能评测采用预先固定的 16 题。三轮计划只完成部分记录，单轮统计单独保存。

最终 284/296 来自 73 道原先通过题、209 道历史失败题的复测确认和 2 道原先未确认题的复测。26 道缺失旧源码的题使用其他存档候选补齐，其中 16 道来自直接生成批次。人工修复期间暂停 SimulIDE，保存的通过证据为目标板编译与主机核心行为测试。
