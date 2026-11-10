import re

from app.schemas import (
    DiagnosticEvent,
    DiagnosticSuggestFixRequest,
    DiagnosticSummaryItem,
    DiagnosticSuggestion,
)


def _normalize_diag_level(value: str | None) -> str:
    raw = str(value or "").strip().upper()
    if raw in {"FATAL", "PANIC", "CRITICAL"}:
        return "FATAL"
    if raw in {"ERROR", "ERR"}:
        return "ERROR"
    if raw in {"WARN", "WARNING"}:
        return "WARN"
    return "INFO"


def _parse_diag_kv_tokens(tokens: list[str]) -> dict[str, str]:
    out: dict[str, str] = {}
    for token in tokens:
        item = token.strip()
        if not item or "=" not in item:
            continue
        key, value = item.split("=", 1)
        key = key.strip().lower()
        value = value.strip()
        if key:
            out[key] = value
    return out


def _parse_diagnostic_line(line: str, line_no: int) -> DiagnosticEvent | None:
    raw = (line or "").strip()
    if not raw:
        return None

    # Structured format recommendation:
    # [DIAG]|ts=...|lvl=ERROR|code=...|module=...|msg=...|k=v
    if "[DIAG]" in raw:
        normalized = raw.replace("[DIAG]", "").strip()
        normalized = normalized[1:] if normalized.startswith("|") else normalized
        parts = [p.strip() for p in normalized.split("|") if p.strip()]
        meta = _parse_diag_kv_tokens(parts)

        message = meta.get("msg") or meta.get("message") or ""
        event = DiagnosticEvent(
            line_no=line_no,
            raw=raw,
            timestamp=meta.get("ts") or meta.get("timestamp"),
            level=_normalize_diag_level(meta.get("lvl") or meta.get("level")),
            code=(meta.get("code") or "UNSPECIFIED").strip() or "UNSPECIFIED",
            module=(meta.get("module") or "general").strip() or "general",
            message=message.strip(),
            context={k: v for k, v in meta.items() if k not in {"ts", "timestamp", "lvl", "level", "code", "module", "msg", "message"}},
        )
        return event

    lower = raw.lower()
    if "error:" in lower or " failed" in lower:
        return DiagnosticEvent(
            line_no=line_no,
            raw=raw,
            level="ERROR",
            code="BUILD_ERROR",
            module="build",
            message=raw,
            context={},
        )

    if "warning:" in lower:
        return DiagnosticEvent(
            line_no=line_no,
            raw=raw,
            level="WARN",
            code="BUILD_WARNING",
            module="build",
            message=raw,
            context={},
        )

    if "警报" in raw or "alarm" in lower:
        return DiagnosticEvent(
            line_no=line_no,
            raw=raw,
            level="WARN",
            code="ALARM_TRIGGERED",
            module="alert_system",
            message=raw,
            context={},
        )

    if "映射后的噪声级别" in raw or "mapped" in lower:
        level_val = re.findall(r"(-?\d+)", raw)
        ctx = {"mapped": level_val[-1]} if level_val else {}
        return DiagnosticEvent(
            line_no=line_no,
            raw=raw,
            level="INFO",
            code="NOISE_LEVEL_SAMPLE",
            module="noise_mapping",
            message=raw,
            context=ctx,
        )

    return None


def _summarize_diagnostic_events(events: list[DiagnosticEvent]) -> list[DiagnosticSummaryItem]:
    buckets: dict[tuple[str, str, str], DiagnosticSummaryItem] = {}
    for event in events:
        key = (event.code, event.level, event.module)
        current = buckets.get(key)
        if current is None:
            buckets[key] = DiagnosticSummaryItem(
                code=event.code,
                level=event.level,
                module=event.module,
                count=1,
                latest_line_no=event.line_no,
            )
        else:
            current.count += 1
            current.latest_line_no = max(current.latest_line_no, event.line_no)

    ordered = sorted(
        buckets.values(),
        key=lambda item: (-item.count, item.level != "ERROR", item.code, item.module),
    )
    return ordered


def _build_diag_recommended_prompt(
    req: DiagnosticSuggestFixRequest,
    summary: list[DiagnosticSummaryItem],
) -> str:
    requirement = (req.requirement or "").strip()
    req_line = requirement if requirement else "(未提供需求原文)"
    summary_lines = [f"- {item.code} | {item.level} | {item.module} | count={item.count}" for item in summary]
    code_excerpt = (req.current_arduino_code or "").strip()
    if len(code_excerpt) > 2500:
        code_excerpt = code_excerpt[:2500] + "\n... (truncated)"

    return (
        "请基于以下信息给出最小修复补丁，且保持模块边界不变:\n"
        f"需求: {req_line}\n"
        "诊断摘要:\n"
        + ("\n".join(summary_lines) if summary_lines else "- (无可用诊断摘要)")
        + "\n\n"
        "请输出: 1) 根因假设 2) 最小改动 3) 回归验证点。\n"
        "当前Arduino代码片段:\n"
        + (code_excerpt or "(未提供)")
    )


def _build_diag_suggestions(summary: list[DiagnosticSummaryItem]) -> tuple[list[str], list[DiagnosticSuggestion]]:
    causes: list[str] = []
    suggestions: list[DiagnosticSuggestion] = []

    has_build_error = any(item.code == "BUILD_ERROR" for item in summary)
    has_alarm = any(item.code == "ALARM_TRIGGERED" for item in summary)
    has_noise_samples = any(item.code == "NOISE_LEVEL_SAMPLE" for item in summary)

    if has_build_error:
        causes.append("构建阶段存在语法或依赖错误，运行期问题分析优先级应后置。")
        suggestions.append(
            DiagnosticSuggestion(
                title="先清理编译错误",
                rationale="编译失败时运行日志不具备可信度，先确保可编译可运行。",
                patch_hint="优先修复编译日志首个 error，并对同类声明/类型错误做批量检查。",
                risk="low",
            )
        )

    if has_alarm:
        causes.append("阈值策略可能过于敏感，存在误报或抖动触发风险。")
        suggestions.append(
            DiagnosticSuggestion(
                title="增加告警去抖和最小间隔",
                rationale="阈值附近噪声波动会导致频繁触发，影响可用性。",
                patch_hint="在 decision 模块引入连续命中计数与触发冷却时间，再决定是否告警。",
                risk="medium",
            )
        )

    if has_noise_samples:
        causes.append("采样值虽有输出，但缺少异常范围检查与传感器失联判定。")
        suggestions.append(
            DiagnosticSuggestion(
                title="补充传感器异常检测",
                rationale="长期同值或突变值应被标记，以便区分硬件问题和算法问题。",
                patch_hint="增加连续同值计数、异常跳变检测，并输出结构化错误码。",
                risk="low",
            )
        )

    if not causes:
        causes.append("日志样本不足或缺少结构化字段，根因判定置信度有限。")
        suggestions.append(
            DiagnosticSuggestion(
                title="升级日志格式",
                rationale="没有事件码和上下文时，难以稳定迭代。",
                patch_hint="按 [DIAG]|ts=|lvl=|code=|module=|msg=|k=v 格式输出关键事件。",
                risk="low",
            )
        )

    return causes, suggestions
