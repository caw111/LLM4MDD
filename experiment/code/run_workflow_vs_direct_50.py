"""Run the frozen 25-internal/25-EmbedBench two-arm pilot once."""
from __future__ import annotations

import argparse
import copy
import json
import os
import shutil
import time
from collections import Counter
from concurrent.futures import ThreadPoolExecutor, as_completed
from datetime import datetime, timezone
from pathlib import Path

import run_current_comparison as workflow
import run_new_workflow_pceval50 as semantic_arm
from app.services.llm_json import json_prompt
from method_snapshot import runtime_identity


ROOT = workflow.ROOT
MANIFEST = ROOT / "experiment/formal/workflow_vs_direct_50_seed42_manifest.json"
CLI = workflow.CLI


def utc() -> str:
    return datetime.now(timezone.utc).isoformat()


def final_review(calls, requirement: str, board: str, code: str, folder: Path) -> dict:
    request = semantic_arm.functional_review_prompt(requirement, board, code)
    invalid_retries = 0
    try:
        _, raw = calls(request)
        decision = semantic_arm.validate_functional_review(raw, requirement, code)
    except (ValueError, json.JSONDecodeError) as first_error:
        invalid_retries = 1
        _, raw = calls(json_prompt(str(request) + "\nYour previous response was invalid: " + str(first_error) +
                                   "\nReturn a corrected object under the same evidence rules."))
        decision = semantic_arm.validate_functional_review(raw, requirement, code)
    decision["invalid_response_retries"] = invalid_retries
    workflow.save(folder / "functional_review.json", decision)
    return decision


def direct_generate(case: dict, case_folder: Path, generation_model: str, review_model: str) -> dict:
    folder = case_folder / "direct_generation"
    result_path = folder / "result.json"
    if result_path.exists():
        old = json.loads(result_path.read_text(encoding="utf-8"))
        if not old.get("infrastructure_error"):
            return old
    calls = semantic_arm.ModelCalls(folder / "generation_calls", generation_model,
                                    role="direct_generation", budget=3)
    review = semantic_arm.ModelCalls(folder / "functional_review_calls", review_model,
                                     role="independent_final_code_review", budget=2,
                                     reasoning_effort="medium")
    requirement = case["effective_requirement"]
    row = dict(case=case["case_id"], dataset=case["source_name"], stratum=case["difficulty"],
               arm="direct_generation", repeat=1, compile_ok=False,
               compile_status="not_generated", functional_review_status="not_run",
               started_utc=utc(), content_repairs=0)
    prompt = workflow.DIRECT_PROMPT + requirement
    stage = "direct_generation"
    started = time.monotonic()
    try:
        code = None
        for attempt in range(3):
            _, raw = calls(prompt)
            code = workflow.extract(raw)
            source_path = folder / f"candidate_{attempt}.ino"
            source_path.parent.mkdir(parents=True, exist_ok=True)
            source_path.write_text(code, encoding="utf-8")
            stage = "arduino_compile"
            compiled, log = workflow.compile_candidate(case, folder / f"compile_attempt_{attempt}", code)
            row.update(compiled, generation_attempts=attempt + 1, content_repairs=attempt)
            if row["compile_ok"] or row["compile_status"] == "missing_dependency":
                break
            if attempt < 2:
                stage = "direct_compile_repair"
                prompt = (workflow.DIRECT_PROMPT + requirement + "\nPrevious sketch:\n" + code +
                          "\nRepair compiler diagnostics without dropping requirements:\n" + log[-10000:])
        if row["compile_ok"]:
            stage = "independent_final_code_review"
            decision = final_review(review, requirement, case["fqbn"], code, folder)
            row.update(functional_review_status=decision["verdict"], functional_status=decision["verdict"])
        elif row["compile_status"] == "missing_dependency":
            row.update(functional_review_status="not_reviewed", failure_stage="missing_dependency")
        else:
            row.update(functional_review_status="not_reviewed", failure_stage="compile_failed")
    except Exception as exc:
        row.update(failure_stage=stage, error=str(exc)[:3000])
        if isinstance(exc, RuntimeError) and semantic_arm.STOP.is_set():
            row.update(infrastructure_error=type(exc).__name__, functional_status="unverified")
    row.update(generation_metrics=workflow.metrics(calls.entries),
               functional_review_metrics=workflow.metrics(review.entries),
               wall_seconds=round(time.monotonic() - started, 3), finished_utc=utc())
    workflow.save(result_path, row)
    return row


def run_case(case: dict, root: Path, generation_model: str, semantic_review_model: str,
             final_review_model: str) -> list[dict]:
    case_folder = root / "cases" / case["case_id"]
    case_folder.mkdir(parents=True, exist_ok=True)
    workflow.save(case_folder / "frozen_case.json", case)
    rows = []
    workflow_row = semantic_arm.evaluate(case, root / "cases", generation_model, semantic_review_model)
    workflow_row["arm"] = "new_workflow_semantic_review"
    rows.append(workflow_row)
    if not semantic_arm.STOP.is_set():
        rows.append(direct_generate(case, case_folder, generation_model, final_review_model))
    workflow.save(case_folder / "case_results.json", rows)
    return rows


def collect(root: Path, config: dict) -> dict:
    rows = []
    for path in sorted((root / "cases").glob("*/case_results.json")):
        rows.extend(json.loads(path.read_text(encoding="utf-8")))
    expected = {(case["case_id"], arm) for case in config["cases"] for arm in config["arms"]}
    observed = {(row["case"], row["arm"]) for row in rows}
    compile_counts = {arm: sum(row.get("compile_ok", False) for row in rows if row["arm"] == arm)
                      for arm in config["arms"]}
    review_counts = {arm: dict(Counter(row.get("functional_review_status", "not_reviewed")
                                      for row in rows if row["arm"] == arm)) for arm in config["arms"]}
    entries = [entry for path in root.glob("cases/**/*_calls/calls.json")
               for entry in json.loads(path.read_text(encoding="utf-8"))]
    successful = [entry for entry in entries if "response" in entry and "error_type" not in entry]
    usage = [entry.get("usage") for entry in successful if entry.get("usage")]
    costs = [item.get("cost") for item in usage if item.get("cost") is not None]
    source_changes = [path for path, value in config["hashes"].items()
                      if not (ROOT / path).is_file() or workflow.digest(ROOT / path) != value]
    complete = observed == expected and not any(row.get("infrastructure_error") for row in rows)
    summary = dict(
        status="complete" if complete else "incomplete",
        planned_cases=50,
        planned_records=100,
        completed_records=len(rows),
        pending_records=[{"case": case, "arm": arm} for case, arm in sorted(expected - observed)],
        compile_passes=compile_counts,
        compile_rates=({arm: compile_counts[arm] / 50 for arm in config["arms"]} if complete else None),
        functional_review_statuses=review_counts,
        static_pass_rates_among_compiled=({arm: ((review_counts[arm].get("judged_pass", 0) / compile_counts[arm])
                                                      if compile_counts[arm] else None) for arm in config["arms"]}
                                            if complete else None),
        static_pass_rates_all_50=({arm: review_counts[arm].get("judged_pass", 0) / 50 for arm in config["arms"]}
                                  if complete else None),
        model_requests=len(entries), model_responses=len(successful),
        input_tokens=(sum(item.get("prompt_tokens", 0) for item in usage) if usage else None),
        output_tokens=(sum(item.get("completion_tokens", 0) for item in usage) if usage else None),
        total_tokens=(sum(item.get("total_tokens", 0) for item in usage) if usage else None),
        reported_cost_usd=(sum(costs) if costs else None),
        cost_note=(None if costs else "费用不可获得"),
        source_hashes_unchanged=not source_changes, changed_sources=source_changes,
        generated_utc=utc(),
    )
    workflow.save(root / "results.json", rows)
    workflow.save(root / "summary.json", summary)
    report = ["# 新版工作流与直接生成：50题预实验", "",
              f"状态：{'完成' if complete else '未完成'}。内部25题、EmbedBench 25题，PCEval 0题。", "",
              f"已产生结果记录 {len(rows)}/100；待处理 {len(expected-observed)}。",
              "本轮不做仿真；功能结论是独立大模型对最终源码的静态判断，不是硬件运行正确率。"]
    if complete:
        report += ["", "| 方法 | 真实编译通过/50 | 静态 judged_pass/编译通过 | 静态 judged_pass/50 |",
                   "|---|---:|---:|---:|"]
        for arm in config["arms"]:
            compiled = compile_counts[arm]
            judged = review_counts[arm].get("judged_pass", 0)
            middle = f"{judged}/{compiled}" if compiled else "null"
            report.append(f"| {arm} | {compiled}/50 | {middle} | {judged}/50 |")
    else:
        report += ["", "批次尚未完成，不发布最终比例。待处理ID和方法见 summary.json。"]
    (root / "REPORT_ZH.md").write_text("\n".join(report) + "\n", encoding="utf-8")
    return summary


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--out", required=True)
    parser.add_argument("--generation-model", default="openai/gpt-5.4-nano")
    parser.add_argument("--semantic-review-model", default="openai/gpt-6.1-sol")
    parser.add_argument("--final-review-model", default="openai/gpt-6.1-sol")
    parser.add_argument("--workers", type=int, default=8)
    parser.add_argument("--first", type=int, help="Run the first N frozen cases as an in-batch pipeline gate")
    parser.add_argument("--report-only", action="store_true")
    args = parser.parse_args()
    os.chdir(ROOT)
    os.environ["PATH"] = str(CLI.parent) + os.pathsep + os.environ["PATH"]
    root = (ROOT / args.out).resolve()
    root.relative_to((ROOT / "experiment/output").resolve())
    root.mkdir(parents=True, exist_ok=True)
    manifest = json.loads(MANIFEST.read_text(encoding="utf-8"))
    cases = manifest["cases"]
    assert len(cases) == len({case["case_id"] for case in cases}) == 50
    assert Counter(case["source_name"] for case in cases) == Counter(internal=25, EmbedBench=25)
    files = [Path(__file__), Path(semantic_arm.__file__), MANIFEST, ROOT / "app/main.py",
             ROOT / "experiment/code/run_current_comparison.py",
             ROOT / "experiment/code/run_controlled_experiment.py",
             ROOT / "experiment/code/run_formal_experiment.py",
             ROOT / "experiment/code/run_contract_development.py",
             ROOT / "experiment/code/method_snapshot.py",
             ROOT / "tools/sysml/SysMLBridge.java"]
    files += list((ROOT / "app/services").glob("*.py")) + list((ROOT / "tools/prompts").glob("*.md"))
    hashes = {str(path.relative_to(ROOT)): workflow.digest(path) for path in files if path.is_file()}
    config = dict(
        cases=cases, arms=["new_workflow_semantic_review", "direct_generation"], repetitions=1,
        generation_model=args.generation_model,
        sysml_semantic_review_model=args.semantic_review_model,
        final_code_review_model=args.final_review_model,
        parameters={"generation_max_tokens": 6000, "review_max_completion_tokens": 6000,
                    "generation_temperature": 0.2, "review_reasoning_effort": "medium"},
        budgets={"contract": "initial+2 repairs", "sysml_shared_structural_semantic": "initial+2 repairs",
                 "direct_compile_feedback": "initial+2 repairs", "final_review_invalid_response_retry": 1},
        semantic_review_enabled=True,
        concurrency={"case_workers": args.workers, "java": 4, "translator": 2, "arduino_compile": 1},
        manifest_sha256=workflow.digest(MANIFEST), hashes=hashes, runtime=runtime_identity(), created_utc=utc(),
        functional_evaluation="independent_static_code_review_only; no simulation or hardware execution",
    )
    execution = root / "execution.json"
    if execution.exists():
        frozen = json.loads(execution.read_text(encoding="utf-8"))
        comparable = copy.deepcopy(config)
        comparable["created_utc"] = frozen.get("created_utc")
        if frozen != comparable and not args.report_only:
            raise SystemExit("Frozen execution configuration changed; use a new output directory")
        config = frozen
    else:
        workflow.save(execution, config)
        shutil.copyfile(MANIFEST, root / "manifest.json")
        for name in hashes:
            target = root / "method_snapshot" / name
            target.parent.mkdir(parents=True, exist_ok=True)
            shutil.copyfile(ROOT / name, target)
        workflow.save(root / "method_snapshot" / "hashes.json", hashes)
    if args.report_only:
        print(json.dumps(collect(root, config), ensure_ascii=False, indent=2))
        return
    run_cases = cases[:args.first] if args.first else cases
    # The two-case gate is intentionally at most two-way; the remaining batch uses the frozen limit of eight.
    workers = min(args.workers, len(run_cases), 2 if args.first else args.workers)
    with ThreadPoolExecutor(max_workers=workers) as pool:
        futures = {pool.submit(run_case, case, root, args.generation_model, args.semantic_review_model,
                               args.final_review_model): case["case_id"] for case in run_cases}
        for index, future in enumerate(as_completed(futures), 1):
            case_id = futures[future]
            rows = future.result()
            print(index, "/", len(run_cases), case_id,
                  [(row["arm"], row.get("compile_status"), row.get("functional_review_status")) for row in rows],
                  flush=True)
            if semantic_arm.STOP.is_set():
                for pending in futures:
                    pending.cancel()
                break
    print(json.dumps(collect(root, config), ensure_ascii=False, indent=2))


if __name__ == "__main__":
    main()
