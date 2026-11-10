> 本文件保存早期 12 题预实验设计。最终对比、消融和修复结果见 [RESULTS.md](RESULTS.md)。

# 当前工作流的对比与消融实验

本实验测试需求到嵌入式代码的 SysML 核心工作流，不包含前端模块蓝图生成和串口部署。结果不能替代功能仿真，也不能直接证明需求满足率。

## 固定设计

- 模型：gpt-4o-mini；温度 0.2；每次输出上限 6000 tokens；反馈上限 2 次。
- 种子 42，在查看生成结果前固定 12 个样本：自有四个难度各一个、PCEval 四级各一个、EmbedBench 随机四个。
- 每个样本一轮，是小样本预实验；不能作统计显著性结论。没有根据结果删除失败样本。
- 输入仅为需求及给定电路。参考代码与测试脚本不传给 LLM。记录源文件 SHA256、生成器源码 SHA256、模型实际返回名称、编译环境、完整提示和返回值。
- EmbedBench 电路均为 Mega，部分描述提到 UNO；统一声明以原电路 Mega 2560 为准，所有组同样处理。其他样本用 UNO。

## 五组

| 组 | 处理 |
|---|---|
| direct | 需求直接生成 Arduino，单次，无反馈 |
| direct_feedback | 从同一份直接生成结果出发，编译失败时反馈编译器诊断，最多修复两次 |
| workflow | SysML 生成、真实参考校验器和编译约束反馈修复、确定性 IR/IL/C、受约束 Arduino 适配 |
| no_sysml_feedback | 关闭 SysML 反馈；若首稿通过，复用完整组相同下游工件；首稿失败则终止 |
| llm_codegen | 复用完整组校验通过的模型，改由 LLM 直接生成 Arduino，允许两次编译诊断修复 |

workflow/no_sysml_feedback 的区别仅为模型阶段反馈，适合观察该阶段的贡献。direct/direct_feedback 用于观察代码反馈本身的贡献。workflow/llm_codegen 可用于探索确定性生成的贡献，但两者下游反馈诊断不同，不能当成严格等反馈预算的因果实验。共享前缀控制随机差异并节省调用；各组费用包括共享前缀，不能相加，actual_usage.json 才是实际调用合计。

## 指标与记录

主指标为真实 Arduino AVR 目标编译通过数/全部样本数；模型生成失败也算端到端未通过。缺少库单独标记 missing_dependency，不将其称为语义错误。附带报告调用数、tokens、模型时间和 API 返回的费用。没有 Wokwi token，功能仿真明确记为未运行。

controlled_pilot_20260926 是发现 JSON 围栏解析问题前的开发预实验，已中止并保留原始记录；不能和修复后结果混合。
controlled_pilot_v2_20260926 在兼容完整 JSON 围栏、安装数据集所需库后使用同一份 manifest。修复不跳过 SysML 校验。

## 复现

在仓库根目录运行：

```powershell
python experiment/code/run_controlled_experiment.py --out experiment/output/controlled_pilot_v2_20260926
```

先确保 .tools/arduino/arduino-cli.exe 可用、Arduino AVR 包和 environment.json 中的库已安装、项目 LLM 环境变量已配置。上述命令会真实调用 API；已完成五组的样本会跳过。每个样本的 results.json、calls.json、validation.json、编译日志和工件保存在独立目录。

论文必须根据实际结果描述优势或不足。即使模型校验成功率提高，也不能据此宣称功能正确率提高；需要后续用数据集原测试进行 Wokwi 仿真、跨种子重复和配对统计。
