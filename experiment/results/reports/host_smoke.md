# 全部对比与消融组：主机运行冒烟测试

每组296题，仅执行历史编译通过的代码。测试接口和判定条件与workflow267测试完全相同；重复执行仅在源代码SHA、开发板和Arduino预处理文件SHA三者一致时复用。

| 实验组 | 编译通过/296 | 三场景冒烟通过 | 编译通过样本中比例 | 全部296题中比例 | 未确认 | 其中主机接口/库不支持 |
|---|---:|---:|---:|---:|---:|---:|
| 旧直接生成 (historical_direct) | 244 (82.4%) | 190 | 77.9% | 64.2% | 54 | 34 |
| 旧直接生成＋编译反馈 (historical_direct_feedback) | 291 (98.3%) | 228 | 78.4% | 77.0% | 63 | 43 |
| 旧工作流初始轮次（累计修复前） (historical_workflow_initial) | 199 (67.2%) | 194 | 97.5% | 65.5% | 5 | 0 |
| 历史累计工作流267 (workflow_267) | 267 (90.2%) | 260 | 97.4% | 87.8% | 7 | 0 |
| 后续GPT-6.1直接生成 (direct_gpt61) | 291 (98.3%) | 235 | 80.8% | 79.4% | 56 | 39 |
| 后续累计工作流280 (workflow_280) | 280 (94.6%) | 273 | 97.5% | 92.2% | 7 | 0 |
| 单轮直接生成 (ablation_direct) | 250 (84.5%) | 195 | 78.0% | 65.9% | 55 | 29 |
| 直接生成＋编译反馈 (ablation_direct_feedback) | 292 (98.6%) | 222 | 76.0% | 75.0% | 70 | 40 |
| 单轮完整工作流 (ablation_workflow) | 257 (86.8%) | 250 | 97.3% | 84.5% | 7 | 0 |
| 移除契约反馈 (ablation_no_contract_feedback) | 214 (72.3%) | 210 | 98.1% | 70.9% | 4 | 0 |
| 移除模型反馈 (ablation_no_model_feedback) | 200 (67.6%) | 194 | 97.0% | 65.5% | 6 | 0 |
| 盲重采样 (ablation_blind_resampling) | 241 (81.4%) | 235 | 97.5% | 79.4% | 6 | 0 |
| LLM代码后端 (ablation_llm_backend) | 208 (70.3%) | 182 | 87.5% | 61.5% | 26 | 17 |

## 解释与范围

测试实际执行Windows主机进程，执行生成代码的setup与loop。三个场景分别为数字输入LOW/ADC0、数字输入HIGH/ADC1023、数字翻转/ADC阶梯；各观察10秒虚拟时间。三个场景均要求setup返回、loop至少返回两次、完成观察窗口并至少记录一次输出。输出可以来自setup，不要求与需求一致或响应输入。串口没有输入注入，所有传感器及执行器是API替身。

未确认分类见summary.json的counts：host_adapter_build_unsupported表示主机接口/库不支持，不是原Arduino编译失败；loop_alive_but_output_not_confirmed_in_all_scenarios表示循环运行但未在所有场景观察到输出；runtime_not_confirmed包括范围检查异常、观察窗口内setup未返回、超时或崩溃等，须进一步复核。不能把这些类别一概作为实板运行失败。

这些比例是主机接口模拟的冒烟结果，不是硬件可用率、SimulIDE通过率、功能正确率。主机整数宽度、资源和时序不等同AVR；不检查接线、真实外设通信、中断与长期行为。各实验组对Arduino API/第三方库的使用不同，当前接口覆盖不足会影响可比较性，需结合各组未支持数量解读，不能仅依此推断工作流质量。

历史累计workflow267/280包含后续修复结果，单轮消融完整工作流为另一组257/296；两种实验协议分别列出，不能把累计267与单轮消融直接当作受控消融比较。

## 证据

frozen_manifest.json保存分组及源路径/哈希；mapped_results.json保存每题分类及证据；execution_results.json保存去重执行日志；fixture_hashes.json保存固定接口哈希；cases/保留构建和进程输出。缓存复用记录引用上一轮的证据路径，不重新生成结论。原生成代码和原实验结果均未修改。

## 各组未确认分类

- 旧直接生成：{"host_adapter_build_unsupported": 34, "runtime_not_confirmed": 18, "loop_alive_but_output_not_confirmed_in_all_scenarios": 2}
- 旧直接生成＋编译反馈：{"host_adapter_build_unsupported": 43, "runtime_not_confirmed": 18, "loop_alive_but_output_not_confirmed_in_all_scenarios": 2}
- 旧工作流初始轮次（累计修复前）：{"runtime_not_confirmed": 1, "loop_alive_but_output_not_confirmed_in_all_scenarios": 4}
- 历史累计工作流267：{"runtime_not_confirmed": 1, "loop_alive_but_output_not_confirmed_in_all_scenarios": 6}
- 后续GPT-6.1直接生成：{"host_adapter_build_unsupported": 39, "runtime_not_confirmed": 15, "loop_alive_but_output_not_confirmed_in_all_scenarios": 2}
- 后续累计工作流280：{"runtime_not_confirmed": 1, "loop_alive_but_output_not_confirmed_in_all_scenarios": 6}
- 单轮直接生成：{"host_adapter_build_unsupported": 29, "runtime_not_confirmed": 24, "loop_alive_but_output_not_confirmed_in_all_scenarios": 2}
- 直接生成＋编译反馈：{"host_adapter_build_unsupported": 40, "runtime_not_confirmed": 28, "loop_alive_but_output_not_confirmed_in_all_scenarios": 2}
- 单轮完整工作流：{"loop_alive_but_output_not_confirmed_in_all_scenarios": 7}
- 移除契约反馈：{"loop_alive_but_output_not_confirmed_in_all_scenarios": 4}
- 移除模型反馈：{"loop_alive_but_output_not_confirmed_in_all_scenarios": 6}
- 盲重采样：{"loop_alive_but_output_not_confirmed_in_all_scenarios": 6}
- LLM代码后端：{"host_adapter_build_unsupported": 17, "runtime_not_confirmed": 3, "loop_alive_but_output_not_confirmed_in_all_scenarios": 6}

## 数据集分项

| 实验组 | 数据集 | 总题数 | 编译通过 | 主机冒烟已确认通过 | 主机接口/库不支持 |
|---|---|---:|---:|---:|---:|
| 旧直接生成 | internal | 120 | 80 | 64 | 14 |
| 旧直接生成 | EmbedBench | 126 | 117 | 97 | 4 |
| 旧直接生成 | PCEval | 50 | 47 | 29 | 16 |
| 旧直接生成＋编译反馈 | internal | 120 | 119 | 97 | 20 |
| 旧直接生成＋编译反馈 | EmbedBench | 126 | 122 | 102 | 4 |
| 旧直接生成＋编译反馈 | PCEval | 50 | 50 | 29 | 19 |
| 旧工作流初始轮次（累计修复前） | internal | 120 | 95 | 93 | 0 |
| 旧工作流初始轮次（累计修复前） | EmbedBench | 126 | 76 | 75 | 0 |
| 旧工作流初始轮次（累计修复前） | PCEval | 50 | 28 | 26 | 0 |
| 历史累计工作流267 | internal | 120 | 115 | 112 | 0 |
| 历史累计工作流267 | EmbedBench | 126 | 120 | 119 | 0 |
| 历史累计工作流267 | PCEval | 50 | 32 | 29 | 0 |
| 后续GPT-6.1直接生成 | internal | 120 | 116 | 96 | 19 |
| 后续GPT-6.1直接生成 | EmbedBench | 126 | 126 | 108 | 4 |
| 后续GPT-6.1直接生成 | PCEval | 50 | 49 | 31 | 16 |
| 后续累计工作流280 | internal | 120 | 118 | 115 | 0 |
| 后续累计工作流280 | EmbedBench | 126 | 126 | 125 | 0 |
| 后续累计工作流280 | PCEval | 50 | 36 | 33 | 0 |
| 单轮直接生成 | internal | 120 | 85 | 71 | 12 |
| 单轮直接生成 | EmbedBench | 126 | 120 | 97 | 2 |
| 单轮直接生成 | PCEval | 50 | 45 | 27 | 15 |
| 直接生成＋编译反馈 | internal | 120 | 117 | 92 | 19 |
| 直接生成＋编译反馈 | EmbedBench | 126 | 125 | 102 | 2 |
| 直接生成＋编译反馈 | PCEval | 50 | 50 | 28 | 19 |
| 单轮完整工作流 | internal | 120 | 107 | 103 | 0 |
| 单轮完整工作流 | EmbedBench | 126 | 122 | 122 | 0 |
| 单轮完整工作流 | PCEval | 50 | 28 | 25 | 0 |
| 移除契约反馈 | internal | 120 | 85 | 84 | 0 |
| 移除契约反馈 | EmbedBench | 126 | 105 | 105 | 0 |
| 移除契约反馈 | PCEval | 50 | 24 | 21 | 0 |
| 移除模型反馈 | internal | 120 | 87 | 83 | 0 |
| 移除模型反馈 | EmbedBench | 126 | 92 | 92 | 0 |
| 移除模型反馈 | PCEval | 50 | 21 | 19 | 0 |
| 盲重采样 | internal | 120 | 100 | 97 | 0 |
| 盲重采样 | EmbedBench | 126 | 111 | 111 | 0 |
| 盲重采样 | PCEval | 50 | 30 | 27 | 0 |
| LLM代码后端 | internal | 120 | 71 | 56 | 11 |
| LLM代码后端 | EmbedBench | 126 | 110 | 103 | 5 |
| LLM代码后端 | PCEval | 50 | 27 | 23 | 1 |

最终核对：13组均完成；3848条分组记录中3234条历史编译通过，映射至1685份去重执行证据；全部执行源文件及预处理文件哈希复核一致，接口文件哈希与前一轮一致，无runner_error，通过项三个场景的进程退出码均为0。核对细节见audit.json。
