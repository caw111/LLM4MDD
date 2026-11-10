#!/usr/bin/env python3
"""
Batch-generate Arduino code for benchmark requirements with two methods:
1) LLM4MMD flow (Step1->Step2->Step3->Step4->Step5-like chain)
2) Direct LLM API call (/convert/c-to-arduino)

Outputs:
- experiment/output/LLM4MMD/{simple,medium,complex}/{i}_{id}.ino
- experiment/output/LLM/{simple,medium,complex}/{i}_{id}.ino
"""

from __future__ import annotations

import argparse
import json
import re
import sys
import time
import urllib.error
import urllib.parse
import urllib.request
from dataclasses import dataclass
from pathlib import Path
from typing import Any


DIFFICULTIES = ("simple", "medium", "complex", "advanced")


@dataclass
class ApiClient:
    base_url: str
    timeout: int

    def _url(self, path: str) -> str:
        return f"{self.base_url.rstrip('/')}{path}"

    def post_json(self, path: str, payload: dict[str, Any]) -> dict[str, Any]:
        data = json.dumps(payload, ensure_ascii=False).encode("utf-8")
        req = urllib.request.Request(
            self._url(path),
            data=data,
            headers={"Content-Type": "application/json"},
            method="POST",
        )
        try:
            with urllib.request.urlopen(req, timeout=self.timeout) as resp:
                raw = resp.read().decode("utf-8", errors="replace")
        except urllib.error.HTTPError as exc:
            detail = exc.read().decode("utf-8", errors="replace")
            raise RuntimeError(f"HTTP {exc.code} {path}: {detail}") from exc
        except urllib.error.URLError as exc:
            raise RuntimeError(f"Request failed {path}: {exc}") from exc

        try:
            return json.loads(raw)
        except json.JSONDecodeError as exc:
            raise RuntimeError(f"Non-JSON response from {path}: {raw[:500]}") from exc


def _safe_name(text: str, fallback: str) -> str:
    cleaned = re.sub(r"[^a-zA-Z0-9_\-]+", "_", text.strip())
    cleaned = re.sub(r"_+", "_", cleaned).strip("_")
    return cleaned or fallback


def _read_json(path: Path) -> list[dict[str, Any]]:
    obj = json.loads(path.read_text(encoding="utf-8"))
    if not isinstance(obj, list):
        raise ValueError(f"Input must be a JSON array: {path}")
    out: list[dict[str, Any]] = []
    for row in obj:
        if isinstance(row, dict):
            out.append(row)
    return out


def _pick_requirement(item: dict[str, Any]) -> str:
    # Compatible with old/new datasets.
    for key in ("requirement", "input", "prompt"):
        val = item.get(key)
        if isinstance(val, str) and val.strip():
            return val.strip()
    return ""


def _extract_c_text(step2_res: dict[str, Any]) -> str:
    contents = step2_res.get("contents") if isinstance(step2_res.get("contents"), dict) else {}
    files = step2_res.get("files") if isinstance(step2_res.get("files"), dict) else {}

    for key in ("generated_model_c", "noise_monitor_c", "source_c", "model_c", "source"):
        val = contents.get(key)
        if isinstance(val, str) and val.strip():
            return val

    for key in ("generated_model_c", "noise_monitor_c", "source_c", "model_c", "source"):
        f = files.get(key)
        if isinstance(f, str) and f.strip() and Path(f).exists():
            return Path(f).read_text(encoding="utf-8")

    return ""


def _extract_regenerated_c(step4_res: dict[str, Any]) -> str:
    contents = step4_res.get("contents") if isinstance(step4_res.get("contents"), dict) else {}
    chunks: list[str] = []
    for key in ("define.h", "main.c", "ProgramPage1.c"):
        val = contents.get(key)
        if isinstance(val, str) and val.strip():
            chunks.append(val)
    return "\n\n".join(chunks).strip()


def _extract_code_from_chat_text(text: str) -> str:
    src = (text or "").strip()
    if not src:
        return ""

    # Prefer fenced code blocks when present.
    fence_patterns = [
        r"```(?:arduino|cpp|c\+\+|c)?\s*\n([\s\S]*?)\n```",
        r"```\s*\n([\s\S]*?)\n```",
    ]
    for pattern in fence_patterns:
        m = re.search(pattern, src, flags=re.IGNORECASE)
        if m and isinstance(m.group(1), str):
            code = m.group(1).strip()
            if code:
                return code

    # Fallback: accept raw text as code.
    return src


def run_llm4mmd_flow(
    client: ApiClient,
    requirement: str,
    difficulty: str,
    idx: int,
    out_file: Path,
    model: str,
) -> dict[str, Any]:
    run_tag = f"{difficulty}_{idx:03d}"
    tmp_root = Path("outputs") / "experiment_pipeline_tmp" / run_tag
    tmp_root.mkdir(parents=True, exist_ok=True)

    step1_sysml = tmp_root / "system.sysml2"
    step1_req_map = tmp_root / "req_test_map.json"
    compiled_dir = tmp_root / "sysml_compiled"
    step4_dir = tmp_root / "c_regen"
    step1 = client.post_json(
        "/generate/sysml2-bundle",
        {"prompt": requirement, "model": model, "temperature": 0.2, "max_tokens": 4000,
         "out_sysml2_path": str(step1_sysml), "out_req_test_map_path": str(step1_req_map)},
    )
    compiled = client.post_json(
        "/compile/sysml2",
        {"system_sysml2": step1["contents"]["system_sysml2"], "out_dir": str(compiled_dir)},
    )
    step4 = client.post_json(
        "/convert/il-to-c",
        {"il_file_path": compiled["files"]["il"], "out_c_dir": str(step4_dir),
         "codegen_root": "CodeGenerator2/CodeGenerator2", "optimize": False, "target": "Intel"},
    )
    regenerated = _extract_regenerated_c(step4)
    if not regenerated.strip():
        raise RuntimeError("SysML toolchain produced no C; experiment cannot fall back to LLM C.")
    c_for_arduino = regenerated

    # Step5
    step5 = client.post_json(
        "/convert/c-to-arduino",
        {
            "c_code": c_for_arduino,
            "compiled_kernel": {"header": step4["contents"]["define.h"], "source": step4["contents"]["ProgramPage1.c"]},
            "user_requirements": requirement,
            "model": model,
            "temperature": 0.2,
            "out_path": str(out_file),
        },
    )
    arduino_code = step5.get("arduino_code") if isinstance(step5.get("arduino_code"), str) else ""
    if arduino_code.strip():
        out_file.parent.mkdir(parents=True, exist_ok=True)
        out_file.write_text(arduino_code, encoding="utf-8")

    return {
        "saved_path": str(out_file),
        "has_regenerated": bool(regenerated.strip()),
        "model": step5.get("model", model),
    }


def run_direct_llm(
    client: ApiClient,
    requirement: str,
    out_file: Path,
    model: str,
) -> dict[str, Any]:
    direct_prompt = (
        "你是资深Arduino工程师。请基于下面需求，直接生成可编译运行的Arduino UNO完整代码。\n"
        "要求：\n"
        "1) 只输出代码，不要解释文字；\n"
        "2) 不要Markdown代码块；\n"
        "3) 必须包含setup()和loop()。\n\n"
        f"需求：\n{requirement.rstrip()}"
    )

    res = client.post_json(
        "/chat",
        {
            "prompt": direct_prompt,
            "model": model,
            "temperature": 0.2,
        },
    )

    chat_text = res.get("text") if isinstance(res.get("text"), str) else ""
    arduino_code = _extract_code_from_chat_text(chat_text)
    if arduino_code.strip():
        out_file.parent.mkdir(parents=True, exist_ok=True)
        out_file.write_text(arduino_code, encoding="utf-8")

    return {
        "saved_path": str(out_file),
        "model": res.get("model", model),
        "user_requirements_sent": requirement,
        "direct_prompt": direct_prompt,
    }


def main() -> int:
    parser = argparse.ArgumentParser(description="Generate benchmark Arduino outputs for two methods.")
    parser.add_argument("--base-url", default="http://127.0.0.1:8000", help="Backend base URL")
    parser.add_argument("--input-dir", default="experiment/input", help="Input JSON directory")
    parser.add_argument("--output-dir", default="experiment/output", help="Output root directory")
    parser.add_argument(
        "--difficulty",
        required=True,
        choices=DIFFICULTIES,
        help="Run only one difficulty per execution: simple | medium | complex",
    )
    parser.add_argument("--model", default="openai/gpt-5.4-nano", help="LLM model name")
    parser.add_argument("--timeout", type=int, default=300, help="HTTP timeout seconds")
    parser.add_argument("--limit", type=int, default=0, help="Per-difficulty limit, 0=all")
    parser.add_argument("--start-index", type=int, default=1, help="Start index (1-based)")
    args = parser.parse_args()

    input_dir = Path(args.input_dir)
    output_dir = Path(args.output_dir)

    client = ApiClient(base_url=args.base_url, timeout=args.timeout)

    run_ts = time.strftime("%Y%m%d_%H%M%S")
    report: dict[str, Any] = {
        "started_at": run_ts,
        "base_url": args.base_url,
        "model": args.model,
        "difficulty": args.difficulty,
        "results": [],
        "errors": [],
    }

    difficulty = args.difficulty
    input_file = input_dir / f"benchmark_requirements_{difficulty}_30.json"
    if not input_file.exists():
        report["errors"].append(
            {
                "difficulty": difficulty,
                "error": f"Input file not found: {input_file}",
            }
        )
    else:
        items = _read_json(input_file)
        if args.start_index > 1:
            items = items[args.start_index - 1 :]
        if args.limit > 0:
            items = items[: args.limit]

        for offset, item in enumerate(items, start=args.start_index):
            req = _pick_requirement(item)
            item_id = str(item.get("id") or f"{difficulty}_{offset:03d}")
            safe_id = _safe_name(item_id, f"{difficulty}_{offset:03d}")

            if not req:
                report["errors"].append(
                    {
                        "difficulty": difficulty,
                        "index": offset,
                        "id": item_id,
                        "error": "Empty requirement/input/prompt",
                    }
                )
                continue

            flow_out = output_dir / "LLM4MMD" / difficulty / f"{offset:03d}_{safe_id}.ino"
            direct_out = output_dir / "LLM" / difficulty / f"{offset:03d}_{safe_id}.ino"

            row: dict[str, Any] = {
                "difficulty": difficulty,
                "index": offset,
                "id": item_id,
                "input_requirement": req,
                "flow_path": str(flow_out),
                "direct_path": str(direct_out),
                "flow_ok": False,
                "direct_ok": False,
            }

            try:
                flow_meta = run_llm4mmd_flow(client, req, difficulty, offset, flow_out, args.model)
                row["flow_ok"] = True
                row["flow_meta"] = flow_meta
                print(f"[OK][FLOW] {difficulty} #{offset} -> {flow_out}")
            except Exception as exc:
                row["flow_error"] = str(exc)
                print(f"[ERR][FLOW] {difficulty} #{offset}: {exc}", file=sys.stderr)

            try:
                direct_meta = run_direct_llm(client, req, direct_out, args.model)
                row["direct_ok"] = True
                row["direct_meta"] = direct_meta
                print(f"[OK][DIRECT] {difficulty} #{offset} -> {direct_out}")
            except Exception as exc:
                row["direct_error"] = str(exc)
                print(f"[ERR][DIRECT] {difficulty} #{offset}: {exc}", file=sys.stderr)

            report["results"].append(row)

    report_path = output_dir / f"run_report_{run_ts}.json"
    report_path.parent.mkdir(parents=True, exist_ok=True)
    report_path.write_text(json.dumps(report, ensure_ascii=False, indent=2), encoding="utf-8")
    print(f"[DONE] report: {report_path}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
