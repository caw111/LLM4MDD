# SysML 独立语义审查

`/generate/sysml2-bundle` 可在原有 SysML 语法、类型、引用、可执行编译和确定性契约检查之后，执行一次独立的大模型语义审查。只有两类检查都通过，候选才会从该接口返回。此审查扩大需求检查范围，不是功能正确性证明，也不代表实物验证通过。

## 启用

请求中增加：

```json
{
  "prompt": "原始需求",
  "interface_contract": {},
  "semantic_review_enabled": true,
  "semantic_review_model": "由部署方配置的审查模型标识",
  "max_semantic_review_calls": 3,
  "max_repair_attempts": 2
}
```

`semantic_review_enabled` 默认为 `false`，以保持历史实验重放和原调用方含义不变。生产调用方应显式设为 `true`。`semantic_review_model` 未提供时沿用请求模型，但仍使用新的、无生成历史消息的请求上下文；代码不绑定供应商或具体模型。直接调用 `generate_validated_bundle()` 时，通过 `semantic_review` 注入审查调用，并同时传入干净的 `original_requirement`。

`max_repair_attempts` 约束生成/修复次数；`max_semantic_review_calls` 是独立审查调用预算。结构检查失败不消耗审查预算。确认的模型缺陷可消耗剩余生成修复预算；每个新候选都会重新执行全部确定性检查和审查。

## 状态与证据

验证报告保存策略版本、候选 SHA-256、审查实际模型、原始响应、严格校验结果、逐轮问题、完成原因、usage、两类调用计数、修复次数和最终失败类别。`final_compile_passed`、`deterministic_behavior_passed` 与语义审查状态分别记录。

- `model` 且 `confirmed`：问题、准确引用、触发序列和因果解释进入模型修复提示。
- `contract` 且 `confirmed`：以 `contract_semantic_issue` 终止，不修改冻结契约，也不在 SysML 中掩盖极性或单位问题。
- `requirement` 且 `confirmed`：以 `requirement_semantic_issue` 终止，等待上游澄清。
- `uncertain`、响应截断、非法 JSON、引用无效、结论矛盾、超时或网络错误：均不放行；评审服务问题不会计为模型功能错误。

历史实验目录和评分不会被修改。若后续启用该机制开展实验，应使用新的方法标记、冻结版本、独立预算和新输出目录。mock 测试只验证工作流与证据校验，不能证明真实审查模型能发现缺陷。
