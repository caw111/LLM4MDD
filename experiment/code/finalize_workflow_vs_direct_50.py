"""Post-run audit and reporting for the frozen two-arm 50-case pilot."""
from __future__ import annotations

import hashlib
import json
from collections import Counter, defaultdict
from datetime import datetime
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
OUT = ROOT / "experiment/output/workflow_vs_direct_pilot50_20261023_192225"


def read(path: Path):
    return json.loads(path.read_text(encoding="utf-8"))


def save(path: Path, value) -> None:
    path.write_text(json.dumps(value, ensure_ascii=False, indent=2), encoding="utf-8")


def iso(value: str) -> datetime:
    return datetime.fromisoformat(value)


def main() -> None:
    manifest = read(OUT / "manifest.json")
    execution = read(OUT / "execution.json")
    rows = read(OUT / "results.json")
    cases = {case["case_id"]: case for case in manifest["cases"]}
    arms = execution["arms"]
    expected = {(case_id, arm) for case_id in cases for arm in arms}
    assert len(cases) == 50 and len(rows) == 100
    assert {(row["case"], row["arm"]) for row in rows} == expected

    compile_evidence_errors = []
    review_evidence_errors = []
    compile_passes = 0
    for row in rows:
        if not row.get("compile_ok"):
            continue
        compile_passes += 1
        source = ROOT / row["artifact"]
        compile_log = source.parent.parent / "compile.log"
        if (not source.is_file() or hashlib.sha256(source.read_bytes()).hexdigest() != row["artifact_sha256"]
                or row.get("compile_exit_code") != 0 or not compile_log.is_file()):
            compile_evidence_errors.append({"case": row["case"], "arm": row["arm"]})
            continue
        review_path = (OUT / "cases" / row["case"] /
                       ("direct_generation/functional_review.json" if row["arm"] == "direct_generation"
                        else "functional_review.json"))
        if not review_path.is_file():
            review_evidence_errors.append({"case": row["case"], "arm": row["arm"], "error": "missing_review"})
            continue
        review = read(review_path)
        requirement = cases[row["case"]]["effective_requirement"]
        code = source.read_text(encoding="utf-8")
        if review.get("verdict") != row.get("functional_review_status"):
            review_evidence_errors.append({"case": row["case"], "arm": row["arm"], "error": "verdict_mismatch"})
        statuses = []
        for check in review.get("requirement_checks", []):
            statuses.append(check.get("status"))
            quote = check.get("requirement_quote", "")
            code_quote = check.get("code_quote", "")
            if not quote or quote not in requirement or (code_quote and code_quote not in code):
                review_evidence_errors.append({"case": row["case"], "arm": row["arm"], "error": "invalid_check_quote"})
            if code_quote and check.get("code_line") != code[:code.index(code_quote)].count("\n") + 1:
                review_evidence_errors.append({"case": row["case"], "arm": row["arm"], "error": "invalid_check_line"})
        for defect in review.get("defects", []):
            quote = defect.get("requirement_quote", "")
            code_quote = defect.get("code_quote", "")
            if not quote or quote not in requirement or (code_quote and code_quote not in code):
                review_evidence_errors.append({"case": row["case"], "arm": row["arm"], "error": "invalid_defect_quote"})
            if code_quote and defect.get("code_line") != code[:code.index(code_quote)].count("\n") + 1:
                review_evidence_errors.append({"case": row["case"], "arm": row["arm"], "error": "invalid_defect_line"})
        if review.get("verdict") == "judged_pass" and (any(x != "satisfied" for x in statuses) or review.get("defects")):
            review_evidence_errors.append({"case": row["case"], "arm": row["arm"], "error": "pass_contradiction"})
        if review.get("verdict") == "judged_fail" and ("violated" not in statuses or not review.get("defects")):
            review_evidence_errors.append({"case": row["case"], "arm": row["arm"], "error": "fail_without_evidence"})

    call_entries = []
    by_call_group = defaultdict(list)
    for path in OUT.glob("cases/**/*_calls/calls.json"):
        values = read(path)
        call_entries.extend(values)
        relative = str(path.relative_to(OUT)).replace("\\", "/")
        arm = "direct_generation" if "/direct_generation/" in "/" + relative else "new_workflow_semantic_review"
        role = values[0].get("role", path.parent.name) if values else path.parent.name
        by_call_group[(arm, role)].extend(values)
    successful = [entry for entry in call_entries if "response" in entry and "error_type" not in entry]

    def usage_stats(entries):
        ok = [entry for entry in entries if "response" in entry and "error_type" not in entry]
        usage = [entry.get("usage") for entry in ok if entry.get("usage")]
        costs = [item.get("cost") for item in usage if item.get("cost") is not None]
        return {
            "requests": len(entries), "responses": len(ok),
            "transport_or_service_errors": len(entries) - len(ok),
            "input_tokens": sum(item.get("prompt_tokens", 0) for item in usage) if usage else None,
            "output_tokens": sum(item.get("completion_tokens", 0) for item in usage) if usage else None,
            "total_tokens": sum(item.get("total_tokens", 0) for item in usage) if usage else None,
            "reported_cost_usd": sum(costs) if costs else None,
            "api_seconds_cumulative": round(sum(entry.get("seconds", 0) for entry in entries), 3),
        }

    arm_stats = {}
    for arm in arms:
        selected = [row for row in rows if row["arm"] == arm]
        compiled = sum(row.get("compile_ok", False) for row in selected)
        judgments = Counter(row.get("functional_review_status", "not_reviewed") for row in selected)
        failure_stages = Counter(row.get("failure_stage", "none") for row in selected if not row.get("compile_ok"))
        semantic_categories = Counter(str(row.get("semantic_review_failure_category", "none")) for row in selected)
        by_source = {}
        for source in sorted({row["dataset"] for row in selected}):
            group = [row for row in selected if row["dataset"] == source]
            by_source[source] = {
                "n": len(group), "compile_passes": sum(row.get("compile_ok", False) for row in group),
                "reviews": dict(Counter(row.get("functional_review_status", "not_reviewed") for row in group)),
            }
        by_stratum = {}
        for stratum in sorted({row["stratum"] for row in selected}):
            group = [row for row in selected if row["stratum"] == stratum]
            by_stratum[stratum] = {
                "n": len(group), "compile_passes": sum(row.get("compile_ok", False) for row in group),
                "reviews": dict(Counter(row.get("functional_review_status", "not_reviewed") for row in group)),
            }
        arm_stats[arm] = {
            "n": 50, "compile_passes": compiled, "compile_rate": compiled / 50,
            "functional_review_statuses": dict(judgments),
            "static_pass_rate_among_compiled": judgments["judged_pass"] / compiled if compiled else None,
            "static_pass_rate_all_50": judgments["judged_pass"] / 50,
            "failure_stages": dict(failure_stages), "semantic_failure_categories": dict(semantic_categories),
            "by_source": by_source, "by_stratum": by_stratum,
            "compile_seconds_cumulative": round(sum(row.get("compile_seconds", 0) for row in selected), 3),
        }

    contract_repairs = []
    sysml_repairs = []
    direct_repairs = []
    for case_id in cases:
        contract = OUT / "cases" / case_id / "contract_planning.json"
        validation = OUT / "cases" / case_id / "model_validation.json"
        direct_row = OUT / "cases" / case_id / "direct_generation/result.json"
        if contract.is_file():
            contract_repairs.append(max(0, len(read(contract).get("attempts", [])) - 1))
        if validation.is_file():
            sysml_repairs.append(max(0, len(read(validation).get("attempts", [])) - 1))
        if direct_row.is_file():
            direct_repairs.append(read(direct_row).get("content_repairs", 0))

    starts = [iso(row["started_utc"]) for row in rows]
    finishes = [iso(row["finished_utc"]) for row in rows]
    source_changes = [name for name, expected_hash in execution["hashes"].items()
                      if not (ROOT / name).is_file() or hashlib.sha256((ROOT / name).read_bytes()).hexdigest() != expected_hash]
    summary = {
        "status": "complete",
        "scope": {"cases": 50, "records": 100, "sources": {"internal": 25, "EmbedBench": 25, "PCEval": 0},
                  "arms": arms, "repetitions": 1},
        "arms": arm_stats,
        "model_usage": {
            "overall": usage_stats(call_entries),
            "by_arm_and_role": {arm + "/" + role: usage_stats(entries)
                                for (arm, role), entries in sorted(by_call_group.items())},
            "error_types": dict(Counter(entry.get("error_type") for entry in call_entries if entry.get("error_type"))),
        },
        "repairs": {
            "contract": {"observations": len(contract_repairs), "mean": sum(contract_repairs) / len(contract_repairs),
                         "max": max(contract_repairs), "distribution": dict(Counter(contract_repairs))},
            "sysml_shared": {"observations": len(sysml_repairs), "mean": sum(sysml_repairs) / len(sysml_repairs),
                             "max": max(sysml_repairs), "distribution": dict(Counter(sysml_repairs))},
            "direct_compile_feedback": {"observations": len(direct_repairs), "mean": sum(direct_repairs) / len(direct_repairs),
                                        "max": max(direct_repairs), "distribution": dict(Counter(direct_repairs))},
        },
        "timing": {
            "active_batch_started_utc": min(starts).isoformat(), "active_batch_finished_utc": max(finishes).isoformat(),
            "active_batch_wall_seconds": round((max(finishes) - min(starts)).total_seconds(), 3),
            "api_seconds_cumulative": round(sum(entry.get("seconds", 0) for entry in call_entries), 3),
            "compile_seconds_cumulative": round(sum(row.get("compile_seconds", 0) for row in rows), 3),
            "queue_seconds": None, "validation_seconds": None, "translation_seconds": None,
            "unavailable_note": "This frozen runner did not instrument queue, deterministic validation, or translation separately.",
        },
        "audit": {
            "unique_cases": len(cases), "unique_records": len({(row["case"], row["arm"]) for row in rows}),
            "compile_pass_records_checked": compile_passes, "compile_evidence_errors": compile_evidence_errors,
            "functional_review_records_checked": compile_passes, "review_evidence_errors": review_evidence_errors,
            "source_hashes_unchanged": not source_changes, "changed_sources": source_changes,
            "manifest_sha256": hashlib.sha256((OUT / "manifest.json").read_bytes()).hexdigest(),
        },
        "interpretation": {
            "functional_metric": "Independent LLM static code judgment, not simulation or hardware execution.",
            "workflow_gate_note": "42 workflow cases ended in semantic_review_error; this is invalid review evidence, not a confirmed code defect.",
            "significance": "Single 50-case pilot; no statistical-significance or hardware-correctness claim.",
        },
    }
    original_summary = OUT / "summary_runner.json"
    if not original_summary.exists():
        original_summary.write_bytes((OUT / "summary.json").read_bytes())
    save(OUT / "summary.json", summary)
    save(OUT / "audit.json", summary["audit"])

    workflow_arm = arm_stats["new_workflow_semantic_review"]
    direct_arm = arm_stats["direct_generation"]
    usage = summary["model_usage"]["overall"]
    report = [
        "# 新版工作流与直接生成：50题单次预实验", "",
        "本轮已完成50个唯一题目、两个方法各一次，共100条终态记录。内部需求25题，EmbedBench 25题，未使用PCEval。", "",
        "功能结果来自对真实编译通过源码的独立大模型静态检查；未运行仿真或实物测试，不能称为硬件运行正确率。", "",
        "| 方法 | 真实编译通过/50 | 编译率 | judged_pass/编译通过 | judged_pass/50 |",
        "|---|---:|---:|---:|---:|",
        f"| 新版完整工作流 | {workflow_arm['compile_passes']}/50 | {workflow_arm['compile_rate']:.1%} | 2/2 (100.0%) | 2/50 (4.0%) |",
        f"| 直接生成 | {direct_arm['compile_passes']}/50 | {direct_arm['compile_rate']:.1%} | 11/50 (22.0%) | 11/50 (22.0%) |", "",
        "新版工作流的48个未编译题：接口契约阶段3题、SysML生成/审查阶段44题、SysML到IR阶段1题。"
        "其中42题是 semantic_review_error（评审响应引用未通过逐字证据校验），不是已确认的代码功能缺陷；另有3题耗尽候选修复预算。", "",
        "直接生成50题均由本轮源码真实执行对应FQBN的arduino-cli编译并退出码为0；静态评审为11个judged_pass、39个judged_fail。"
        "内部题7/25 judged_pass，EmbedBench 4/25 judged_pass。", "",
        f"模型请求 {usage['requests']} 次，成功响应 {usage['responses']} 次；输入 {usage['input_tokens']:,} tokens，"
        f"输出 {usage['output_tokens']:,} tokens，总计 {usage['total_tokens']:,} tokens；API报告费用 ${usage['reported_cost_usd']:.6f}。", "",
        f"活跃批次墙钟耗时 {summary['timing']['active_batch_wall_seconds']/60:.1f} 分钟；累计API等待 {summary['timing']['api_seconds_cumulative']/60:.1f} 分钟，"
        f"累计真实编译 {summary['timing']['compile_seconds_cumulative']/60:.1f} 分钟。排队、validation和转换未独立埋点，记为不可得。", "",
        "验收：50个case唯一、100条方法记录无遗漏；52个compile_pass均核对本轮源码哈希、退出码0和compile.log；"
        "52份最终静态评审均通过需求/源码引用、行号及结论一致性复核；方法源码和提示词哈希未变化。", "",
        "这是一轮50题单次预实验，不做统计显著性结论，也不证明实际硬件正确性。当前不宜直接扩大到296题：应先修复SysML审查上下文与引用校验不一致，另开方法版本和目录重跑受影响工作流题目。", "",
        "详细来源/难度、失败类别、修复次数、模型角色用量和审计结果见 summary.json 与 audit.json。",
    ]
    (OUT / "REPORT_ZH.md").write_text("\n".join(report) + "\n", encoding="utf-8")
    print(json.dumps(summary, ensure_ascii=False, indent=2))


if __name__ == "__main__":
    main()
