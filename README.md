# LLM4MDD

LLM4MDD 将嵌入式自然语言需求转换为接口契约、经过校验的 SysML 模型、确定性代码及 Arduino 硬件适配代码。后端采用 FastAPI，前端采用 React 和 Vite。

## 仓库结构

| 目录 | 内容 |
|---|---|
| app/ | 接口规划、SysML 校验与修复、代码生成和后端 API |
| frontend/ | 可运行的前端 |
| tools/sysml/ | 固定版本 SysML 校验器的安装脚本和模型示例 |
| tools/prompts/ | 模型提示词 |
| experiment/code/ | 实验生成、评测及已保存结果的复测入口 |
| experiment/analysis/ | 最终代码修复规则、逐题测试计划和审查汇总脚本 |
| experiment/results/ | 13 组历史对比与消融、296 题最终版本、测试输入和执行证据 |
| experiment/evidence/ | PICSimLab 逐题记录、补充指标、SysML 修复案例及原始数据包清单 |
| experiment/datasets/ | 公开数据集及原始说明 |
| experiment/input/ | 120 道本地需求 |
| experiment/formal/ | 296 道有效需求及实验配置 |
| tests/ | 后端、模型语义与硬件适配的回归测试 |

## 实验结果

### LLM4MDD 完整工作流（最终交付版）

最终交付版本目标板编译通过 **296/296**，PICSimLab 核心功能通过 **283/296（95.61%）**，另有 12 题失败、1 题未确认。交付版本包含多批次工作流生成、历史人工修复和存档补齐。该结果用于描述最终交付代码的验收表现。详见 [296 题交付验收](experiment/evidence/picsimlab_verification_20261107/PICSIMLAB_COMPLETE_RESULTS_296_AFTER_REVIEW.md)。

### 早期自动生成对比与消融

2026-10-17 的 GPT-5.4-nano 消融批次中，早期工作流基准启用了接口契约反馈、模型反馈及确定性后端。该组与同轮直接生成组在同一套 296 题上比较，使用该批次冻结的源码，评测前未追加人工逐题修复。任务包含本地数据集 120 题、PCEval 50 题、EmbedBench 126 题。核心功能判定结合 PICSimLab 仿真与逐题复核，两列均以全部 296 题为分母。

| 实验组 | 目标板编译通过 | 核心功能通过 |
|---|---:|---:|
| GPT-5.4-nano 直接生成 | 250/296（84.46%） | 122/296（41.22%） |
| GPT-5.4-nano 直接生成＋编译反馈 | 292/296（98.65%） | 137/296（46.28%） |
| 早期工作流消融基准（冻结版） | 257/296（86.82%） | 96/296（32.43%） |
| GPT-6.1 直接生成（跨模型对照） | 291/296（98.31%） | 207/296（69.93%） |

各消融组、三个数据集的分项和逐题结果见 [PICSimLab 对比与消融](experiment/evidence/picsimlab_comparison_20261108/FINAL_RESULTS.md)。

[实验材料索引](experiment/evidence/README.md) 收录补充指标和原始数据包。[历史主机复测与定向补齐](experiment/RESULTS.md) 单独归档。

## 安装与启动

需要 Python 3.10 或更新版本、Node.js，以及外部 Arduino 和 SysML 工具链。

```powershell
python -m venv .venv
.venv/Scripts/python -m pip install -r requirements.txt
Copy-Item .env.example .env
```

在本机 .env 中配置模型密钥后启动后端。

```powershell
.venv/Scripts/python -m uvicorn app.main:app --host 127.0.0.1 --port 8001
```

在 frontend 目录安装并启动前端。

```powershell
cd frontend
npm.cmd ci
npm.cmd run dev
```

SysML 固定版本安装及代码生成的工具依赖见 [工作流说明](PIPELINE.md) 和 [语义及实现说明](PIPELINE_DETAILED.md)。Windows 校验器安装入口为 tools/sysml/install.ps1。Code2Script、CodeGenerator2 与其编译工具按工作流文档配置。外部工具可以安装在独立目录。后端可通过 CODEGENERATOR2_ROOT 指向 CodeGenerator2 根目录，Windows 目标板编译可通过 WINDOWS_ARDUINO_CLI_PATH 指向 arduino-cli.exe。

## 校验与复测已保存的实验

仅核对实验文件哈希和汇总数字，无需模型接口。

```powershell
python experiment/code/replay_results.py
```

使用 C++17 g++ 执行保存的核心测试。通过 CXX 环境变量指定编译器，或将 g++ 放入 PATH。

```powershell
python experiment/code/replay_results.py --case complex_019
python experiment/code/replay_results.py --case pceval_multiplexer_photoresistor
```

增加 --compile 可同时重新编译目标 Arduino 板。需将 arduino-cli 放入 PATH，并安装对应开发板包及草图所需库。

```powershell
python experiment/code/replay_results.py --case complex_019 --compile
```

复测输出保存在 experiment/replay-output/。重新生成实验需要额外配置模型接口，相关入口和预算见 [实验说明](experiment/README.md)。
