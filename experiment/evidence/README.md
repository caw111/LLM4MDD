# 实验材料索引

| 材料 | 内容 |
|---|---|
| [PICSimLab 对比与消融](picsimlab_comparison_20261108/FINAL_RESULTS.md) | 汇总、逐题判定、生成源码、仿真场景与检查结果 |
| [296 题交付快照](picsimlab_verification_20261107/PICSIMLAB_COMPLETE_RESULTS_296_AFTER_REVIEW.md) | 283/296 道核心功能通过，另附逐题证据和 34 题复核。属于历史交付快照 |
| [补充指标](additional_metrics_20261109/FINAL_RESULTS.md) | 指标定义、逐题结果、资源统计和需求覆盖率审查材料 |
| [周期蜂鸣器修复案例](case_studies/medium_029/) | 同轮直接生成与工作流固件、契约、SysML 诊断和修复记录、主机测试 |
| [历史实验与复现](../README.md) | 三个数据集的输入、原有实验汇总、去重源码、执行轨迹与复测命令 |

## 原始运行文件

[实验数据 Release](https://github.com/caw111/LLM4MDD/releases/tag/experiment-evidence-20261110) 附有四批数据。压缩包保留批次原始相对路径，包含仿真轨迹、固件、原始测试计划、工作流生成记录和补充审查记录。压缩包名称、大小、条目数与 SHA-256 见 [归档清单](archive_manifest.json)。本地环境配置、缓存和可重建的编译对象文件未打包。

在新检出的仓库根目录运行以下命令，可按原路径恢复仿真及统计脚本使用的 experiment/output/ 目录。

```powershell
gh release download experiment-evidence-20261110 --repo caw111/LLM4MDD --pattern '*.zip' --dir .\experiment-assets
Get-ChildItem -LiteralPath .\experiment-assets -Filter '*.zip' |
  ForEach-Object { Expand-Archive -LiteralPath $_.FullName -DestinationPath . }
```

原记录中的绝对路径用于追溯当时的运行环境。随仓库提供的源码、计划和证据文件可通过批次目录的相对路径访问。批量生成和仿真使用的工具版本及运行约定见各批次的方法快照和报告。

## 周期蜂鸣器案例

在 [medium_029](case_studies/medium_029/) 中，直接生成固件和早期 SysML 都在鸣叫时停止周期计时。模型验证记录保留了无暂停条件下 6000 ms 的反例、反馈中的修复片段及修订结果。两个固件的主机测试使用同一套按钮输入，结果保存在 results.json 中。该开发案例与 296 题 PICSimLab 汇总分别统计。
