# 工具入口

- sysml/install.ps1：安装固定版本的 SysML 参考校验器和 Java。
- generate_sysml2_bundle_cli.py、generate_subsystem_ir_cli.py：模型与中间表示生成入口。
- prompts/、examples/：提示词、契约和模型示例。
- package_experiment_results.py：本次整理使用的原始材料打包脚本，输入为整理前的 experiment/output/。该目录现已移到仓库外的本地归档，原始 JSON 也保留了当时的绝对路径；打包脚本属于整理过程记录，不能在全新克隆中直接运行。已发布材料的校验及离线复测入口是 experiment/code/replay_results.py。
