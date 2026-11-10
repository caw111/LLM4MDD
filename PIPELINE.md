# SysML v2 已接入的默认工作流

默认流程是：

需求 → LLM 提出设备请求与串口报告组 → 程序构造并冻结接口注册表 → LLM 生成行为控制器与需求映射 → 程序装配 SysML v2 → 官方参考实现校验 → 确定性编译 → IL XML → 已有 ILTranslator → C 模型内核 → 确定性 Arduino 硬件适配。

接口注册表的格式、每次生成的保存空间和保证范围见 [PIPELINE_DETAILED.md](PIPELINE_DETAILED.md#程序构造接口与串口注册表)。设备请求示例见 [serial_report.proposal.json](tools/examples/serial_report.proposal.json)。

接口准备阶段会规范化已知等价字段、按唯一源接线补回遗漏引脚，并记录处理过程；真实冲突仍拒绝。串口接收示例见 [serial_command.proposal.json](tools/examples/serial_command.proposal.json) 和 [serial_integer.proposal.json](tools/examples/serial_integer.proposal.json)，程序生成数值输入及本步有效标记，控制行为由 SysML 实现。

语法、引用解析或编译失败时，将上一版模型与结构化诊断反馈给 LLM，最多修复 2 次（可通过 max_repair_attempts 调整）。失败返回 HTTP 422，工具未安装/运行失败返回 503，不继续使用未通过的工件。

## 启动

在仓库根目录运行：

```powershell
powershell -File tools/sysml/install.ps1
python -m uvicorn app.main:app --host 127.0.0.1 --port 8001
```

另开终端启动前端：

```powershell
npm --prefix frontend ci
npm --prefix frontend run dev
```

需要 requirements.txt 中的 Python 依赖和现有 CodeGenerator2 的 ILTranslator.exe。当前环境的旧 .venv 来自其他平台，无法执行；使用可用的 Windows Python 或新建 Windows 虚拟环境。

安装脚本获取 GitHub 上的 SysML v2 Pilot Implementation 2026-08 / 0.62.0，校验下载包 SHA256，将校验器、标准库和独立 Java 21 放在 .tools/sysml，不修改系统 Java 或 PATH。许可证随上游包保留（EPL-2.0）。SYSML_JAVA 可指定其他 Java 21 JDK 的 java 可执行文件。

## API 与产物

| 接口 | 用途 |
|---|---|
| POST /generate/interface-contract | 由设备请求构造、校验并冻结接口，保存独立接口和串口注册表；兼容已有完整契约 |
| POST /generate/sysml2-bundle | 生成、官方校验、编译检查、反馈修复，保存模型和 .validation.json 诊断记录 |
| POST /validate/sysml2 | 仅进行官方语言校验，输入 system_sysml2 |
| POST /compile/sysml2 | 确定性编译，输入 system_sysml2，可选 root 和 out_dir |
| POST /convert/il-to-c | 运行已有 ILTranslator，输入 il_file_path 和 out_c_dir |
| POST /convert/c-to-arduino | 带接口契约时校验模型来源链并确定性生成硬件适配，模型内核固定拼入；旧入口有独立兼容限制 |

编译输出包括 subsystem_ir.json 和 model.il.xml。纯数据流模型另输出 model.tblink.xml。包含状态动作的图使用版本 2，保留 states、updates 和执行顺序，直接输出兼容现有 ILTranslator 的 IL；旧 TBLink 不支持这种扩展，因此不输出会丢失状态行为的 TBLink。

前端和 experiment/code/generate_benchmark_arduino.py 均使用新流程。实验编译失败会记为失败，不再偷偷回退到 LLM C。旧 /generate/c-from-step1 已改为新编译器的兼容接口，不再让 LLM 重解释模型。直接 /chat 和独立的旧 IR 生成接口仍可用于对照实验，不属于默认模型流程。

Arduino 编辑接口会拒绝改写编译模型核心；行为修改应从 SysML 重新编译。参数更新先检查字面量，旧模块编辑复用静态检查。

增量生成会保存并恢复上一轮 system_sysml2 和 req_test_map，将它们与当前模块信息一起传入提示。

## 示例与验证

- tools/sysml/noise_monitor.sysml：计算与阈值判断。
- tools/sysml/modular_monitor.sysml：模块实例、bind 和跨模块引用。
- tools/sysml/counter.sysml：持久计数与条件复位。
- tools/sysml/ordered_updates.sysml：显式有序的多状态更新。

```powershell
python -m unittest discover -s tests -p "sysml*unittest.py" -v
npm --prefix frontend run build
```

端到端测试需要现有 ILTranslator 和 g++。测试使用真实官方解析/校验器、真实 ILTranslator 和真实宿主 C++ 编译；LLM 回复通过测试替身提供，不消耗 API 额度。硬件采样和执行器由宿主测试替身模拟，测试不等于实板验证。

完整语义和边界见 [PIPELINE_DETAILED.md](PIPELINE_DETAILED.md)。

## 模块说明与反馈可见性

模块卡片提供完整说明弹窗，包括职责与处理规则、输入依赖和输出用途。新模块规划提示要求给出 3–5 句与需求对应的说明，保存/加载沿用 subtitle 字段；历史简短说明需重新规划后才会补充，不自动编造设备参数。

SysML 请求明确允许两次反馈修复。完整 JSON 围栏只做封装归一化，随后仍进行官方语言校验和编译约束检查。前端展示逐轮诊断；全部尝试失败则停止后续步骤。该接口默认等待上限改为 20 分钟，可用 VITE_API_TIMEOUT_MS 覆盖。

对比与消融实验设计见 [CONTROLLED_EXPERIMENT.md](experiment/CONTROLLED_EXPERIMENT.md)。实验调用真实模型，不能用回归测试的通过数替代实验效果。
