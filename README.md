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

[实验表格与统计范围](experiment/RESULTS.md) 给出编译率、固定 16 题功能测试及人工修复后的全套核心复测。

[实验材料索引](experiment/evidence/README.md) 收录 PICSimLab 对比与消融、296 题交付快照和补充指标。交付快照在 PICSimLab 核心功能口径下通过 283/296（95.61%）。下文的历史主机结果和此处的电路仿真结果分别记录。

历史整理交付集核心功能确认通过 284/296（95.95%），包含人工代码调整及存档补齐，原始记录保持不变。

[2026-11-07 补齐与离线复测](experiment/WORKFLOW_COMPLETION_20261107.md) 将累计核心确认提高到 **289/296（97.64%）**。此前缺少生成代码的 16 题均取得目标板编译证据：15 题来自工作流生成/缓存恢复，1 题人工补建 SysML。累计工作流编译证据为 295/296，计入该人工模型为 296/296。这些累计开发结果与原有单轮对比、消融分别保存。主机测试观察 Arduino API 输出，本轮没有新增电路仿真结论。

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
