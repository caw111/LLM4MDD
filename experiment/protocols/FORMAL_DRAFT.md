> 历史实验计划，未按此协议完成独立测试集实验。已完成结果以 [RESULTS.md](../RESULTS.md) 为准。

# 正式实验协议（准备中，尚未运行独立测试集）

## 样本与重复

全量 296 题：内部需求 120、PCEval 50、EmbedBench 126。冻结开发集 60 题、独立测试集 236 题（96/40/100）；已查看的 12 题全部留在开发集。每题每方法运行 5 次。计划六组，共 7080 条评估记录；这不是已完成的结果数，也不是 API 调用数。

## 六组与公平性

1. 直接 Arduino 生成，无反馈。
2. 直接 Arduino 生成，带最终编译/行为测试反馈。
3. 完整 SysML→IR→C→Arduino，包括模型反馈。
4. 完整流程关闭模型反馈，其他阶段一致。
5. 跳过 SysML，由 LLM 直接生成 IR，保持 IR 检查、后端生成和硬件适配一致。
6. 从相同的有效 SysML，由 LLM 替代确定性模型代码生成，保持硬件适配及后续反馈一致。

旧预实验的 llm_codegen 直接生成整个 Arduino，不能原样用于第六组。新增组和统一下游反馈仍需实现。所有组的模型版本、输入信息、温度、最大输出与最终测试固定；额外中间反馈记录其成本，并增加相同总 token/费用预算下的比较。失败样本留在端到端分母中。

编译器另做组件级实验：使用固定有效 SysML 和独立输入输出轨迹比较两个代码生成器，避免前置模型失败使消融不可观察。组件结果不能替代自然语言端到端结果。

## 行为 oracle 与指标

PCEval 原始 scenario.yaml 必须先在参考程序上通过。EmbedBench 的测试为描述性时间/状态列表，需要转换成可执行控制与断言后核验。内部需求也需要独立测试；禁止利用被测生成代码修改预期输出。生成提示不得包含参考答案或隐藏测试。

主指标：功能测试通过率、整题全部测试通过率。辅助指标：SysML 首稿/修复通过率、IR/C 成功率、真实目标编译通过率、多模块接口一致性、修复次数、tokens、费用和耗时。按来源、难度、支持范围及错误阶段分别报告。

五次重复需报告离散程度；方法差值使用按题目聚类的配对 bootstrap 95% 区间，不能将同一题五次运行当成五个独立任务。另报语义可执行子集覆盖率。不支持特性、基准问题和基础设施问题分别记录，不能挑选能通过的题作主结果。

## 现状

manifest.json 已冻结并核验 296/60/236 个数。工作流正在开发集调试，正式测试尚未启动。Wokwi CLI 0.27.1 已安装；凭据使用 Git 忽略的 .env.local，不写入报告。run_wokwi_case.py 可编译参考/生成固件并运行原始 PCEval 场景。

正式全量运行还需要：参考测试审计、其余 oracle 转换、六组统一实现、五次重复与断点续跑，以及方法/预算冻结。不能将这些待办写成已完成实验。

## 2026-09-27 环境审计

Wokwi 凭据已配置并实际验证。50 个 PCEval 参考程序均已编译成功；其中 26 个原始仿真场景通过，17 个使用不支持的命令，1 个引脚名无效，2 个场景参数缺失，3 个参考程序未通过断言，1 个超时。结果与日志见 oracle_audit/REPORT_ZH.md。原数据及预期值未改动，依赖与截图复制问题已修复并复测。该审计不是正式 LLM 对比结果。
## 正式执行冻结补充（2026-09-28，测试集生成前）

执行器：experiment/code/run_formal_experiment.py。独立测试集 236 题 × 5 次 × 6 组；模型 GPT-4o-mini，温度 0.2，单次最大输出 6000 tokens。模型/IR 最多两轮诊断修复，适配层最多两轮结构修复；直接反馈组与各流水线组统一最多两轮 Arduino 编译修复。每个结果区分未生成、类型/连接不合法、编译失败及基础设施错误。

功能测试只用于最终评估，不将隐藏场景反馈给生成模型，避免测试泄漏。这修订了前述“编译/行为反馈”设想。主表完整报告 236 题的最终 Arduino 编译结果；功能表使用经参考程序验证的 19 个独立测试任务（95 次/组），同时报告 PCEval 19/40 和整体 19/236 的覆盖率。其他任务的功能正确性未验证，不能以编译通过代替。剩余数据集的行为 oracle 仍是局限，而非已完成工作。

六组：direct（首稿）、direct_feedback（同一首稿加编译修复）、workflow（完整）、no_sysml_feedback（同一 SysML 首稿，关闭模型修复）、direct_ir（跳过 SysML，采用相同的 IR 类型/驱动/环路/表达式校验及后端）、llm_kernel（同一有效 SysML 和相同 C 接口，由 LLM 替代确定性计算内核，复用完整组的初始硬件适配代码）。最后一组的计算内核与确定性内核一样，在下游适配修复中保持固定；编译诊断可修复适配代码，不能修改内核。

共享首稿/有效模型/适配代码是配对设计，不是新增独立抽样；每组成本计入其共享前缀，真实总消费则只统计实际 calls.json。不同方法总调用预算并不相等，因此同时报告耗时、token 和消费，不声称这是等成本比较。组件成功率仅作为条件分析，端到端结果保留所有前置失败。

方法源码、协议参数和 oracle 清单散列保存于执行目录 execution.json / method_snapshot。正式运行后不据测试输出改提示词。断点续跑校验散列并重放已有响应；基础设施错误需要诊断和重跑，不能声明完成。统计以题目为聚类单位，对五次重复先取题内平均，再进行配对 bootstrap 区间。

并发执行备注：运行初期按断点提高并发至 32 个任务线程、8 个参考校验器、4 个编译槽位，未改方法或提示词。重启记录见 concurrency_restart.json；已返回调用完整保留，在途未返回请求可能产生未记录的提供商费用，因此响应汇总费用不等于精确账单总额。

基础设施恢复补充：已观察到 API 连接中断和参考校验器超时。恢复入口 formal_resume.py 对传输失败最多重试两次，不消耗模型修复轮数；参考校验器对同一候选先重试一次。完全相同源文本、相同固定校验器的结果允许缓存复用，缓存按精确源文本与校验器版本/代码散列命名，既有参考验证失败报告可作为缓存来源。不会用 IR 降级失败或 JSON 格式失败冒充参考校验结果。

若同一候选连续两次触发参考校验器 NullPointerException，则记录 REFERENCE_TOOL_CRASH，明确表示未能证明模型有效，而不是伪造语法诊断；该候选不能进入后端，并在原有模型修复预算内反馈重生成。这是运行时健壮性修复，保留崩溃记录。提示词主体、模型、样本、语法/类型规则及成功判定未改。其余持续的基础设施问题仍阻止最终完成声明。


### 2026-09-30 harness recovery
Input-specific ILParser rejection is a backend failure, not an infrastructure outage. Independent arms continue. The frozen LLM-kernel ablation requires the shared backend header and adapter; when these are unavailable its end-to-end result is a backend_dependency failure. Compiler-ablation comparisons are therefore conditional on this shared preparation and do not isolate backend coverage. No compiler, generation prompt, budget, response or pass criterion was changed. Original execution hashes and harness are preserved in recovery_history; amendment hashes are recorded separately. On replay, an identical candidate uses the exact historical compiler-feedback/response pair, with current diagnostic differences archived separately.

## 2026-10-12 开发修订（尚未冻结新正式实验）

以上历史运行描述不能替代本机原始证据。本次先审计保留记录、修复反馈及接口边界，再做小批开发验证；本机缺少旧六组逐次结果，因此不重建或虚构其通过率。正式测试集不用于修提示词；本次没有启动正式测试。待开发方法和独立oracle覆盖稳定后，使用新目录冻结方法、模型、全部工具版本与修复预算，禁止续写旧冻结运行。

四个预设对比及问题：

| 左组减右组 | 问题 |
|---|---|
| direct_feedback − direct | 迭代本身的贡献 |
| workflow − no_sysml_feedback | SysML模型反馈的贡献 |
| workflow − direct_ir | SysML阶段的独立价值 |
| workflow − llm_kernel | 确定性内核是否减少实现错误 |

相同题目且相同repeat才能配对。先计算题内配对差值，再按题目bootstrap，报告95%区间和重复运行标准差。缺少对应repeat不强行配对。新汇总中的全任务功能比例包含未编译任务，另报编译成功样例比例；未测/无oracle保留unknown并将观测比例标记为下界。有效oracle子集另列覆盖率，不能替代全任务分母。

不含传输重试的每组最大方法调用数：direct=1、direct_feedback=3、workflow=8、no_sysml_feedback=6、direct_ir=8、llm_kernel=9（模型3+共享初始适配最多3+内核1+适配编译修复2）。每轮最多6000输出tokens；模型/IR、适配结构、目标编译分别最多2轮修复。预算不等，不把额外计算收益归因于方法；等成本比较需要另立预注册预算，本次未声称完成。LLM内核组依赖共享头文件和初始适配，仍须同时报告全任务结果与有共享工件的条件结果。

新增接口清单只能包含公共需求和硬件信息；隐藏轨迹/预期值不得放入清单或反馈。静态覆盖检查不是功能oracle。新正式实验前仍需补齐独立需求断言、参考测试审计和开发集前后真实调用结果；不会仅凭本地回归通过宣布方法已稳定。

## 契约贯通修订（contract-first-v2，开发中）

新六组入口统一采用生产接口规划器、IR v2 校验器和确定性硬件适配器。没有公共预置契约时，在原始需求与目标板型基础上规划一次，最多两轮修复；同一 task/repeat 的四个契约组共享该契约。公共预置契约必须通过同一硬件、显式需求和板型检查，校验失败不能自动替换。规划失败保留四组失败结果，直接生成两组照常使用原始公共需求，所有任务仍保留在分母。规划成本分别计入每个使用契约的组；物理总费用只统计实际调用一次。生成出的契约是方法的中间产物，不作为额外公共答案提供给直接生成组。

SysML 和直接 IR 的首轮提示都接收完整冻结契约。适配层不调用 LLM；目标编译失败保留失败及诊断，不允许自由重写引脚、输入采样或模型输出映射。LLM 内核组仍复用固定头文件和硬件适配器，生成的计算内核也保持固定，不能借助适配器补偿其错误。旧版本的八/九次调用预算仅适用于历史方法，不适用于新版本。

新版本不含传输重试的最大方法调用数：direct=1、direct_feedback=3、workflow=6（规划3+模型3）、no_sysml_feedback=4（规划3+模型1）、direct_ir=6（规划3+IR3）、llm_kernel=7（规划3+模型3+内核1）。已有公共契约时各契约组减少最多3次规划调用。执行配置记录 call_limits，汇总优先使用对应运行的配置，不能用新预算改写旧实验。预算仍不相等，等成本比较尚未完成。

本修订尚未启动正式测试集运行，新增回归验证入口与调用记账，不构成方法成功率的证据。需继续扩展设备适配和独立行为验证，稳定后在新目录冻结完整方法及工具版本。
