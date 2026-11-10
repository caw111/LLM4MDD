from __future__ import annotations

import re
import shutil
import subprocess
import tempfile
from dataclasses import dataclass
from pathlib import Path


TRUTHY_VALUES = {"1", "true", "yes", "on"}
PLACEHOLDER_PHRASES = [
    "if needed for debugging",
    "todo",
    "to be implemented",
    "replace with",
    "example only",
    "pseudo code",
    "示例代码",
    "按需调试",
    "待实现",
]


@dataclass
class ArduinoCompileCheck:
    ok: bool
    log: str
    cli_missing: bool = False


def env_flag(value: str | None, default: bool = False) -> bool:
    if value is None:
        return default
    raw = value.strip().lower()
    if not raw:
        return default
    return raw in TRUTHY_VALUES


def _repair_swallowed_comment_code(text: str) -> str:
    out = text or ""
    code_start = (
        r"(?:const|void|int|bool|float|double|long|short|byte|char|unsigned|signed)\b"
        r"|(?:if|for|while|switch|else|return)\b"
        r"|(?:pinMode|digitalWrite|digitalRead|analogRead|analogWrite|delay|tone|noTone|map)\s*\("
        r"|(?:Serial\.(?:begin|print|println))\s*\("
        r"|[A-Za-z_][A-Za-z0-9_]*\s*\("
        r"|}"
    )

    out = re.sub(rf"(//[^\n]*?)(?=\s*{code_start})", r"\1\n", out)
    out = re.sub(r"(//[^\n]*?)\s*}", r"\1\n}", out)

    def _ensure_closed_function(src: str, func_name: str) -> str:
        match = re.search(rf"\bvoid\s+{func_name}\s*\(\s*\)\s*\{{", src)
        if not match:
            return src
        start = match.end() - 1
        depth = 0
        for idx in range(start, len(src)):
            ch = src[idx]
            if ch == "{":
                depth += 1
            elif ch == "}":
                depth -= 1
                if depth == 0:
                    return src
        return src.rstrip() + "\n}\n"

    out = _ensure_closed_function(out, "setup")
    out = _ensure_closed_function(out, "loop")
    return out


def _normalize_param_marker_layout(src: str) -> str:
    raw_lines = (src or "").splitlines()
    joined_lines: list[str] = []
    i = 0
    while i < len(raw_lines):
        line = raw_lines[i]
        if "[PARAM:" in line and "]" not in line:
            buffer = line.rstrip()
            j = i + 1
            while j < len(raw_lines):
                part = raw_lines[j].strip()
                if part:
                    buffer = f"{buffer} {part}"
                if "]" in raw_lines[j]:
                    break
                j += 1
            line = buffer
            i = j
        joined_lines.append(line)
        i += 1

    marker_line_re = re.compile(r"^\s*//\s*\[PARAM:[^\]]+\]\s*$")
    decl_end_re = re.compile(r";\s*(?://.*)?$")
    marker_blob_re = re.compile(r"\[PARAM:(?P<meta>[^\]]+)\]")

    def _parse_blob(meta: str) -> dict[str, str]:
        out: dict[str, str] = {}
        for token in (meta or "").split("|"):
            token = token.strip()
            if "=" not in token:
                continue
            key, value = token.split("=", 1)
            key = key.strip().lower()
            value = value.strip()
            if key:
                out[key] = value
        return out

    def _merge_blob(a: dict[str, str], b: dict[str, str]) -> dict[str, str]:
        merged = dict(a)
        for key, value in b.items():
            if not value:
                continue
            if key not in merged or not merged[key] or len(value) > len(merged[key]):
                merged[key] = value
        return merged

    def _build_blob(meta: dict[str, str]) -> str:
        ordered_keys = ["id", "module", "name", "zh_name", "type", "min", "max"]
        parts: list[str] = []
        for key in ordered_keys:
            val = str(meta.get(key) or "").strip()
            if val:
                parts.append(f"{key}={val}")
        for key in sorted(meta.keys()):
            if key in ordered_keys:
                continue
            val = str(meta.get(key) or "").strip()
            if val:
                parts.append(f"{key}={val}")
        return "|".join(parts)

    out_lines: list[str] = []
    for line in joined_lines:
        stripped = line.strip()
        if marker_line_re.match(stripped):
            if out_lines and decl_end_re.search(out_lines[-1]):
                out_lines[-1] = f"{out_lines[-1].rstrip()} {stripped}"
            else:
                out_lines.append(line)
            continue
        out_lines.append(line)

    canonical_lines: list[str] = []
    for line in out_lines:
        matches = list(marker_blob_re.finditer(line))
        if not matches:
            canonical_lines.append(line)
            continue

        merged_meta: dict[str, str] = {}
        for match in matches:
            merged_meta = _merge_blob(merged_meta, _parse_blob(match.group("meta")))

        line_wo_markers = marker_blob_re.sub("", line)
        line_wo_markers = re.sub(r"\s+$", "", line_wo_markers)
        line_wo_markers = re.sub(r"//\s*$", "", line_wo_markers)
        marker_text = f"[PARAM:{_build_blob(merged_meta)}]"
        canonical_lines.append(f"{line_wo_markers.rstrip()} // {marker_text}")

    normalized = "\n".join(canonical_lines)
    return re.sub(r"\|\s*([a-zA-Z_][a-zA-Z0-9_]*)\s*=\s+", r"|\1=", normalized)


def _extract_fenced_arduino_code(text: str) -> str:
    src = text or ""
    fence_re = re.compile(r"```(?P<lang>[A-Za-z0-9_+#.-]*)[^\n]*\n(?P<code>[\s\S]*?)```")
    matches = list(fence_re.finditer(src))
    if not matches:
        return src

    language_scores = {
        "arduino": 4,
        "ino": 4,
        "cpp": 3,
        "c++": 3,
        "cc": 2,
        "c": 2,
    }

    def _score(match: re.Match[str]) -> tuple[int, int]:
        lang = str(match.group("lang") or "").strip().lower()
        code = str(match.group("code") or "")
        has_sketch = bool(re.search(r"\bvoid\s+setup\s*\(", code) and re.search(r"\bvoid\s+loop\s*\(", code))
        return (10 if has_sketch else 0) + language_scores.get(lang, 0), len(code)

    best = max(matches, key=_score)
    return str(best.group("code") or "")


def sanitize_arduino_code(text: str) -> str:
    out = _extract_fenced_arduino_code(text).strip()
    if re.match(r"^```\s*(?:cpp|arduino|ino|c\+\+|cc|c)?\s*$", out.splitlines()[0] if out.splitlines() else "", flags=re.I):
        out = "\n".join(out.splitlines()[1:])
    elif out.startswith("```cpp"):
        out = out[6:]
    elif out.startswith("```arduino"):
        out = out[10:]
    elif out.startswith("```"):
        out = out[3:]
    if out.endswith("```"):
        out = out[:-3]
    out = _repair_swallowed_comment_code(out)
    out = _normalize_param_marker_layout(out)
    return out.strip()


def _strip_comments_and_strings(src: str) -> str:
    out: list[str] = []
    i = 0
    in_line_comment = False
    in_block_comment = False
    in_string: str | None = None
    escaped = False
    while i < len(src):
        ch = src[i]
        nxt = src[i + 1] if i + 1 < len(src) else ""

        if in_line_comment:
            if ch == "\n":
                in_line_comment = False
                out.append(ch)
            else:
                out.append(" ")
            i += 1
            continue

        if in_block_comment:
            if ch == "*" and nxt == "/":
                in_block_comment = False
                out.extend("  ")
                i += 2
            else:
                out.append("\n" if ch == "\n" else " ")
                i += 1
            continue

        if in_string:
            out.append("\n" if ch == "\n" else " ")
            if escaped:
                escaped = False
            elif ch == "\\":
                escaped = True
            elif ch == in_string:
                in_string = None
            i += 1
            continue

        if ch == "/" and nxt == "/":
            in_line_comment = True
            out.extend("  ")
            i += 2
            continue
        if ch == "/" and nxt == "*":
            in_block_comment = True
            out.extend("  ")
            i += 2
            continue
        if ch in {"'", '"'}:
            in_string = ch
            out.append(" ")
            i += 1
            continue

        out.append(ch)
        i += 1

    return "".join(out)


def _find_function_span(src: str, func_name: str) -> tuple[int, int] | None:
    match = re.search(rf"\bvoid\s+{func_name}\s*\(\s*\)\s*\{{", src)
    if not match:
        return None

    start = match.start()
    body_start = match.end() - 1
    depth = 0
    for idx in range(body_start, len(src)):
        ch = src[idx]
        if ch == "{":
            depth += 1
        elif ch == "}":
            depth -= 1
            if depth == 0:
                return start, idx + 1
    return None


def _extract_void_function_body(src: str, func_name: str) -> str | None:
    span = _find_function_span(src, func_name)
    if not span:
        return None
    open_idx = src.find("{", span[0], span[1])
    close_idx = span[1] - 1
    if open_idx < 0 or close_idx <= open_idx:
        return None
    return src[open_idx + 1 : close_idx]


def _check_balanced_symbols(src: str) -> list[str]:
    issues: list[str] = []
    cleaned = _strip_comments_and_strings(src)
    pairs = [("{", "}"), ("(", ")"), ("[", "]")]
    for opener, closer in pairs:
        depth = 0
        for ch in cleaned:
            if ch == opener:
                depth += 1
            elif ch == closer:
                depth -= 1
                if depth < 0:
                    issues.append(f"Unbalanced symbol: unexpected '{closer}'")
                    break
        if depth > 0:
            issues.append(f"Unbalanced symbol: missing '{closer}'")
    return issues


def validate_arduino_code(text: str) -> list[str]:
    issues: list[str] = []
    src = text or ""
    if not src.strip():
        return ["Arduino output is empty"]
    cleaned = _strip_comments_and_strings(src)
    if "```" in src:
        issues.append("Arduino output contains Markdown code fences")
    if not re.search(r"\bvoid\s+setup\s*\(\s*\)", cleaned):
        issues.append("Missing setup()")
    if not re.search(r"\bvoid\s+loop\s*\(\s*\)", cleaned):
        issues.append("Missing loop()")
    if _has_include_header(src, "define.h"):
        issues.append("Forbidden private header include: define.h")

    loop_body = _extract_void_function_body(cleaned, "loop")
    if loop_body is not None and not _strip_comments_and_strings(loop_body).strip():
        issues.append("loop() is empty; add periodic sensing/control logic or call module helper functions")

    lowered = src.lower()
    for phrase in PLACEHOLDER_PHRASES:
        if phrase.lower() in lowered:
            issues.append(f"Placeholder text remains in Arduino code: {phrase}")

    issues.extend(_check_balanced_symbols(src))

    serial_begin_matches = list(re.finditer(r"\bSerial\.begin\s*\(", cleaned))
    setup_span = _find_function_span(cleaned, "setup")
    if re.search(r"\bSerial\.(print|println)\s*\(", cleaned) and not serial_begin_matches:
        issues.append("Serial.print/println is used but Serial.begin is missing")
    if serial_begin_matches:
        if len(serial_begin_matches) > 1:
            issues.append("Serial.begin is called more than once")
        if setup_span is None:
            issues.append("Serial.begin exists but setup() is not a complete function body")
        else:
            sb = serial_begin_matches[0].start()
            if not (setup_span[0] <= sb < setup_span[1]):
                issues.append("Serial.begin must be called inside setup(), not in loop() or helper functions")

    pin_defs = re.findall(
        r"\bconst\s+(?:int|byte|short|long|uint8_t|uint16_t|uint32_t|unsigned\s+int|unsigned\s+long)\s+([A-Za-z_][A-Za-z0-9_]*_PIN)\b",
        cleaned,
    )
    for pin_name in sorted(set(pin_defs)):
        uses = len(re.findall(rf"\b{re.escape(pin_name)}\b", cleaned))
        if uses <= 1:
            issues.append(f"Pin constant {pin_name} is defined but never used")

    library_requirements = [
        (
            "TM1637",
            r"\bTM1637Display\b|\btm1637",
            "TM1637Display.h",
            r"\bTM1637Display\s+[A-Za-z_][A-Za-z0-9_]*\s*\(",
            "TM1637Display object construction is missing or malformed",
        ),
        ("Servo", r"\bServo\s+[A-Za-z_][A-Za-z0-9_]*\s*(?:;|=|\()", "Servo.h", "", ""),
        (
            "LiquidCrystal",
            r"\bLiquidCrystal\s+[A-Za-z_][A-Za-z0-9_]*",
            "LiquidCrystal.h",
            r"\bLiquidCrystal\s+[A-Za-z_][A-Za-z0-9_]*\s*\(",
            "LiquidCrystal object construction is missing or malformed",
        ),
        (
            "SoftwareSerial",
            r"\bSoftwareSerial\s+[A-Za-z_][A-Za-z0-9_]*",
            "SoftwareSerial.h",
            r"\bSoftwareSerial\s+[A-Za-z_][A-Za-z0-9_]*\s*\(",
            "SoftwareSerial object construction is missing or malformed",
        ),
        (
            "Stepper",
            r"\bStepper\s+[A-Za-z_][A-Za-z0-9_]*",
            "Stepper.h",
            r"\bStepper\s+[A-Za-z_][A-Za-z0-9_]*\s*\(",
            "Stepper object construction is missing or malformed",
        ),
        ("Wire", r"\bWire\s*\.", "Wire.h", "", ""),
        ("SPI", r"\bSPI\s*\.", "SPI.h", "", ""),
    ]
    for label, usage_pattern, required_header, construction_pattern, construction_issue in library_requirements:
        if not re.search(usage_pattern, cleaned, flags=re.IGNORECASE):
            continue
        if not _has_include_header(src, required_header):
            issues.append(f"{label} usage requires #include <{required_header}>")
        if construction_pattern and not re.search(construction_pattern, cleaned):
            issues.append(construction_issue)

    return issues


def _extract_include_headers(src: str) -> list[tuple[int, str]]:
    includes: list[tuple[int, str]] = []
    in_block_comment = False
    include_re = re.compile(r"^\s*#\s*include\s*[<\"](?P<header>[^>\"]+)[>\"]")
    for line_no, line in enumerate((src or "").splitlines(), start=1):
        raw = line
        if in_block_comment:
            if "*/" in raw:
                raw = raw.split("*/", 1)[1]
                in_block_comment = False
            else:
                continue

        while "/*" in raw:
            before, after = raw.split("/*", 1)
            if "*/" in after:
                raw = before + after.split("*/", 1)[1]
                continue
            raw = before
            in_block_comment = True
            break

        stripped = raw.strip()
        if not stripped or stripped.startswith("//"):
            continue
        match = include_re.match(raw)
        if match:
            includes.append((line_no, match.group("header").strip()))
    return includes


def _has_include_header(src: str, expected_header: str) -> bool:
    expected = str(expected_header or "").strip().lower()
    if not expected:
        return False
    return any(header.lower() == expected for _, header in _extract_include_headers(src))


def validate_uno_board_compatibility(code_text: str) -> list[str]:
    src = code_text or ""
    cleaned = _strip_comments_and_strings(src)
    issues: list[str] = []

    forbidden_headers = [
        (re.compile(r"^(?:ESP8266WiFi|WiFi|WiFiClientSecure|WiFiUdp)\.h$", re.I), "WiFi/ESP networking headers are not available on bare Arduino UNO"),
        (re.compile(r"^(?:BluetoothSerial|esp_now|AsyncTCP|ESPAsyncWebServer|ESP32Servo)\.h$", re.I), "ESP32-specific headers are not available on Arduino UNO"),
        (re.compile(r"^(?:driver|freertos|esp32-hal)/", re.I), "ESP32 platform headers are not available on Arduino UNO"),
        (re.compile(r"^(?:iostream|string|vector|map|set|list|deque|unordered_map|unordered_set|thread|chrono)$", re.I), "desktop C++ standard library headers are not Arduino UNO sketch dependencies"),
        (re.compile(r"^stdio\.h$", re.I), "C stdio console I/O is not bare Arduino UNO user I/O; use Serial or hardware outputs"),
    ]
    for line_no, header in _extract_include_headers(src):
        for pattern, reason in forbidden_headers:
            if pattern.search(header):
                issues.append(f"Line {line_no}: #include <{header}> is incompatible with Arduino UNO; {reason}")
                break

    forbidden_patterns = [
        (r"\bINPUT_PULLDOWN\b", "Arduino UNO does not support INPUT_PULLDOWN; use INPUT_PULLUP with inverted logic or external pulldown hardware"),
        (r"\bSerial[1-9]\b", "Arduino UNO exposes only Serial over USB UART; Serial1/Serial2/etc. are not available"),
        (r"\bledc(?:Setup|AttachPin|Write|DetachPin|Read|WriteTone|WriteNote)\s*\(", "ESP32 ledc PWM APIs are not available on Arduino UNO; use analogWrite on UNO PWM pins"),
        (r"\banalog(?:Read|Write)Resolution\s*\(", "analogReadResolution/analogWriteResolution are not available on Arduino UNO"),
        (r"\btouchRead\s*\(", "ESP32 touchRead is not available on Arduino UNO"),
        (r"\bESP\s*\.", "ESP platform APIs are not available on Arduino UNO"),
        (r"\bBluetoothSerial\b", "BluetoothSerial is ESP32-specific and not available on Arduino UNO"),
        (r"\bWiFi\s*\.", "WiFi object APIs require WiFi-capable boards/shields and are not bare Arduino UNO code"),
        (r"\bint\s+main\s*\(", "Arduino sketches must not define main(); use setup() and loop() only"),
        (r"\busing\s+namespace\s+std\s*;", "desktop C++ namespace imports are not appropriate for Arduino UNO sketches"),
        (r"\bstd::(?:cout|cin|cerr|endl)\b", "desktop console I/O is not available on Arduino UNO; use Serial.print/println"),
        (r"(?<![\w:])(?:cout|cin|cerr)\s*(?:<<|>>)", "desktop console I/O is not available on Arduino UNO; use Serial.print/println"),
        (r"\b(?:printf|fprintf|sprintf|snprintf|puts|putchar)\s*\(", "C stdio output is not Arduino UNO user I/O; use Serial.print/println or hardware outputs"),
        (r"\b(?:scanf|fscanf|sscanf|gets|getchar)\s*\(", "C stdio input is not Arduino UNO user I/O; use Serial.read or hardware inputs"),
    ]
    for pattern, message in forbidden_patterns:
        match = re.search(pattern, cleaned)
        if not match:
            continue
        line_no = _line_number_at(cleaned, match.start())
        issues.append(f"Line {line_no}: {message}")

    return issues


def extract_module_marker_ids(code_text: str) -> set[str]:
    pattern = re.compile(
        r"//\s*\[MODULE:(?P<module_id>[^:\]]+):start\]\s*\n(?P<code>.*?)\n//\s*\[MODULE:(?P=module_id):end\]",
        re.S,
    )
    return {str(match.group("module_id") or "").strip() for match in pattern.finditer(code_text or "") if str(match.group("module_id") or "").strip()}


def _iter_module_blocks(code_text: str) -> list[dict[str, object]]:
    pattern = re.compile(
        r"//\s*\[MODULE:(?P<module_id>[^:\]]+):start\]\s*\n(?P<code>.*?)\n//\s*\[MODULE:(?P=module_id):end\]",
        re.S,
    )
    blocks: list[dict[str, object]] = []
    for match in pattern.finditer(code_text or ""):
        module_id = str(match.group("module_id") or "").strip()
        if not module_id:
            continue
        blocks.append(
            {
                "module_id": module_id,
                "code": str(match.group("code") or ""),
                "start": match.start("code"),
                "end": match.end("code"),
                "line": (code_text or "").count("\n", 0, match.start()) + 1,
            }
        )
    return blocks


def validate_module_markers(code_text: str, module_ids: list[str] | None) -> list[str]:
    expected = [str(item or "").strip() for item in (module_ids or []) if str(item or "").strip()]
    if not expected:
        return []

    found = extract_module_marker_ids(code_text)
    issues: list[str] = []
    missing = [module_id for module_id in expected if module_id not in found]
    if missing:
        issues.append(
            "Missing editable MODULE markers for module_id(s): "
            + ", ".join(missing)
            + ". Add // [MODULE:<module_id>:start] and // [MODULE:<module_id>:end] around each module function."
        )

    unknown = sorted(found - set(expected))
    if unknown:
        issues.append("Unexpected MODULE marker id(s) not present in module specs: " + ", ".join(unknown))

    return issues


def validate_module_marker_structure(code_text: str) -> list[str]:
    src = code_text or ""
    marker_line_re = re.compile(
        r"^\s*//\s*\[MODULE:(?P<module_id>[^:\]]+):(?P<kind>start|end)\]\s*$"
    )
    issues: list[str] = []
    stack: list[tuple[str, int]] = []
    start_counts: dict[str, int] = {}

    for line_no, line in enumerate(src.splitlines(), start=1):
        if "[MODULE:" not in line:
            continue

        marker = marker_line_re.match(line)
        if not marker:
            issues.append(
                f"Line {line_no}: MODULE marker must be a standalone line in format // [MODULE:<module_id>:start] or // [MODULE:<module_id>:end]"
            )
            continue

        module_id = str(marker.group("module_id") or "").strip()
        kind = str(marker.group("kind") or "").strip()
        if not module_id:
            issues.append(f"Line {line_no}: MODULE marker has an empty module_id")
            continue

        if kind == "start":
            start_counts[module_id] = start_counts.get(module_id, 0) + 1
            if stack:
                open_id, open_line = stack[-1]
                issues.append(
                    f"Line {line_no}: MODULE '{module_id}' starts before MODULE '{open_id}' from line {open_line} ended; MODULE blocks must not overlap or nest"
                )
            stack.append((module_id, line_no))
            continue

        if not stack:
            issues.append(f"Line {line_no}: MODULE end marker for '{module_id}' has no matching start marker")
            continue

        open_id, open_line = stack.pop()
        if open_id != module_id:
            issues.append(
                f"Line {line_no}: MODULE end marker for '{module_id}' closes currently open MODULE '{open_id}' from line {open_line}"
            )

    for module_id, open_line in stack:
        issues.append(f"Line {open_line}: MODULE start marker for '{module_id}' has no matching end marker")

    for module_id, count in sorted(start_counts.items()):
        if count > 1:
            issues.append(f"MODULE '{module_id}' appears in multiple editable blocks; each module id must have exactly one block")

    return issues


def validate_module_boundaries(code_text: str) -> list[str]:
    src = code_text or ""
    issues: list[str] = []
    nested_marker_re = re.compile(r"//\s*\[MODULE:[^:\]]+:(?:start|end)\]")
    arduino_entrypoint_re = re.compile(r"\bvoid\s+(?P<name>setup|loop)\s*\(\s*\)")

    for block in _iter_module_blocks(src):
        block_code = str(block.get("code") or "")
        module_id = str(block.get("module_id") or "").strip()
        block_start = int(block.get("start") or 0)

        for match in arduino_entrypoint_re.finditer(block_code):
            line_no = _line_number_at(src, block_start + match.start())
            issues.append(
                f"Line {line_no}: MODULE '{module_id}' wraps Arduino {match.group('name')}(); setup() and loop() must stay outside editable MODULE blocks"
            )

        nested_match = nested_marker_re.search(block_code)
        if nested_match:
            line_no = _line_number_at(src, block_start + nested_match.start())
            issues.append(
                f"Line {line_no}: MODULE '{module_id}' contains a nested MODULE marker; MODULE blocks must not overlap or nest"
            )

    return issues


def validate_module_execution(code_text: str) -> list[str]:
    src = code_text or ""
    if not src.strip():
        return []

    cleaned = _strip_comments_and_strings(src)
    signature_re = re.compile(
        r"^\s*(?:void|bool|int|long|short|float|double|byte|char|unsigned\s+long|unsigned\s+int|uint8_t|uint16_t|uint32_t)\s+"
        r"(?P<name>[A-Za-z_][A-Za-z0-9_]*)\s*\([^;{}]*\)\s*\{",
        re.M,
    )
    issues: list[str] = []
    for block in _iter_module_blocks(src):
        block_code = str(block.get("code") or "")
        signature = signature_re.search(block_code)
        if not signature:
            continue

        fn_name = str(signature.group("name") or "").strip()
        if not fn_name or fn_name in {"setup", "loop"}:
            continue

        block_start = int(block.get("start") or 0)
        block_end = int(block.get("end") or block_start)
        outside = cleaned[:block_start] + (" " * max(0, block_end - block_start)) + cleaned[block_end:]
        call_re = re.compile(rf"\b{re.escape(fn_name)}\s*\(")
        called = False
        for call in call_re.finditer(outside):
            line_start = outside.rfind("\n", 0, call.start()) + 1
            line_prefix = outside[line_start : call.start()]
            if re.search(
                r"(?:void|bool|int|long|short|float|double|byte|char|unsigned\s+long|unsigned\s+int|uint8_t|uint16_t|uint32_t)\s+$",
                line_prefix,
            ):
                continue
            called = True
            break

        if not called:
            for call in _iter_function_calls(outside, ["attachInterrupt"]):
                args = call.get("args") if isinstance(call.get("args"), list) else []
                if len(args) >= 2 and str(args[1] or "").strip() == fn_name:
                    called = True
                    break

        if not called:
            issues.append(
                f"Line {block.get('line')}: MODULE '{block.get('module_id')}' defines helper function '{fn_name}' but it is never called or registered from setup(), loop(), or another module"
            )

    return issues


PIN_NAME_RE = re.compile(r"(pin|clk|dio|scl|sda|miso|mosi|cs|ss|rx|tx)", re.I)
LIBRARY_PIN_CONSTRUCTOR_ARG_POSITIONS: dict[str, list[int]] = {
    "TM1637Display": [0, 1],
    "LiquidCrystal": list(range(0, 12)),
    "SoftwareSerial": [0, 1],
    "Stepper": [1, 2, 3, 4],
}


def _normalize_param_type(value: str) -> str:
    return re.sub(r"\s+", " ", str(value or "").strip().lower())


def validate_param_literal_value(value: str, declared_type: str, param_name: str = "") -> str | None:
    raw = str(value or "").strip()
    if not raw:
        return "PARAM default value is empty"
    if any(token in raw for token in [";", "\n", "\r", "//", "/*", "*/", "{", "}"]):
        return f"PARAM default value '{raw}' must be a single literal, not code or a comment"

    compact = re.sub(r"\s+", "", raw)
    normalized_type = _normalize_param_type(declared_type)
    is_pin_param = bool(PIN_NAME_RE.search(str(param_name or "")))

    if is_pin_param and re.fullmatch(r"(?:LED_BUILTIN|A\d+|D?\d+)", compact, flags=re.I):
        return None

    if normalized_type == "bool":
        if re.fullmatch(r"(?:true|false|HIGH|LOW|0|1)", compact, flags=re.I):
            return None
        return f"PARAM default value '{raw}' is not a valid bool literal"

    integer_types = {
        "int",
        "long",
        "short",
        "byte",
        "uint8_t",
        "uint16_t",
        "uint32_t",
        "unsigned long",
        "unsigned int",
    }
    if normalized_type in integer_types:
        if re.fullmatch(r"(?:HIGH|LOW)", compact, flags=re.I):
            return None
        if re.fullmatch(r"[-+]?\d+(?:[uUlL]*)", compact):
            if normalized_type.startswith("unsigned") or normalized_type in {"byte", "uint8_t", "uint16_t", "uint32_t"}:
                if compact.startswith("-"):
                    return f"PARAM default value '{raw}' is negative for unsigned type '{declared_type}'"
            return None
        return f"PARAM default value '{raw}' is not a valid integer literal"

    if normalized_type in {"float", "double"}:
        if re.fullmatch(r"[-+]?(?:(?:\d+(?:\.\d*)?)|(?:\.\d+))(?:[eE][-+]?\d+)?(?:[fF])?", compact):
            return None
        return f"PARAM default value '{raw}' is not a valid floating-point literal"

    return None


def validate_param_markers(code_text: str, module_ids: list[str] | None = None) -> list[str]:
    src = code_text or ""
    expected_modules = {str(item or "").strip() for item in (module_ids or []) if str(item or "").strip()}
    module_start_re = re.compile(r"^\s*//\s*\[MODULE:(?P<module>[^:\]]+):start\]\s*$")
    module_end_re = re.compile(r"^\s*//\s*\[MODULE:(?P<module>[^:\]]+):end\]\s*$")
    marker_re = re.compile(r"\[PARAM:(?P<meta>[^\]]+)\]")
    decl_re = re.compile(
        r"^\s*(?:const\s+)?(?P<type>int|long|short|float|double|bool|byte|uint8_t|uint16_t|uint32_t|unsigned\s+long|unsigned\s+int)\s+"
        r"(?P<name>[A-Za-z_][A-Za-z0-9_]*)\s*=\s*(?P<value>[^;]+);"
    )
    param_like_name_re = re.compile(
        r"(threshold|limit|min|max|gain|factor|interval|period|timeout|hz|freq|sample|window|ratio|duty|speed|pin|clk|dio|scl|sda|miso|mosi|cs|ss|rx|tx)",
        re.I,
    )
    literal_value_re = re.compile(
        r"^\s*(?:[-+]?\d+(?:\.\d+)?(?:[uUlLfF]+)?|A\d+|D?\d+|true|false|HIGH|LOW)\s*$",
        re.I,
    )

    def _parse_marker(meta: str) -> dict[str, str]:
        parsed: dict[str, str] = {}
        for token in (meta or "").split("|"):
            token = token.strip()
            if "=" not in token:
                continue
            key, value = token.split("=", 1)
            key = key.strip().lower()
            if key:
                parsed[key] = value.strip()
        return parsed

    def _parse_numeric_literal(value: str) -> float | None:
        token = str(value or "").strip()
        token = re.sub(r"\s+", "", token)
        token = token.rstrip("uUlLfF")
        if not re.fullmatch(r"[-+]?(?:\d+(?:\.\d*)?|\.\d+)", token):
            return None
        try:
            return float(token)
        except ValueError:
            return None

    issues: list[str] = []
    seen_ids: set[str] = set()
    active_module = ""
    for line_no, line in enumerate(src.splitlines(), start=1):
        start_match = module_start_re.match(line)
        if start_match:
            active_module = str(start_match.group("module") or "").strip()
            continue

        end_match = module_end_re.match(line)
        if end_match:
            active_module = ""
            continue

        marker = marker_re.search(line)
        decl = decl_re.search(line)

        if marker:
            if not decl:
                issues.append(f"Line {line_no}: PARAM marker must be on the same line as a variable declaration")
                continue
            between_decl_and_marker = line[decl.end() : marker.start()]
            if "//" not in between_decl_and_marker:
                issues.append(f"Line {line_no}: PARAM marker must be in a line comment immediately after the declaration")
            if ";" in between_decl_and_marker:
                issues.append(f"Line {line_no}: PARAM marker line contains extra code after the declaration")

            meta = _parse_marker(marker.group("meta"))
            missing_keys = [key for key in ["id", "module", "name", "type"] if not str(meta.get(key) or "").strip()]
            if missing_keys:
                issues.append(f"Line {line_no}: PARAM marker missing required key(s): {', '.join(missing_keys)}")
            marker_module = str(meta.get("module") or "").strip()
            if marker_module and expected_modules and marker_module not in expected_modules:
                issues.append(f"Line {line_no}: PARAM module '{marker_module}' is not one of expected module ids: {', '.join(sorted(expected_modules))}")
            if active_module and marker_module and marker_module != active_module:
                issues.append(f"Line {line_no}: PARAM module '{marker_module}' does not match active MODULE block '{active_module}'")
            param_id = str(meta.get("id") or "").strip()
            if param_id:
                if param_id in seen_ids:
                    issues.append(f"Line {line_no}: duplicate PARAM id: {param_id}")
                seen_ids.add(param_id)
            declared_name = str(decl.group("name") or "").strip()
            marker_name = str(meta.get("name") or "").strip()
            if marker_name and declared_name and marker_name != declared_name:
                issues.append(f"Line {line_no}: PARAM name '{marker_name}' does not match declaration '{declared_name}'")

            marker_type = str(meta.get("type") or "").strip()
            declared_type = str(decl.group("type") or "").strip()
            if marker_type and declared_type and _normalize_param_type(marker_type) != _normalize_param_type(declared_type):
                issues.append(f"Line {line_no}: PARAM type '{marker_type}' does not match declaration type '{declared_type}'")

            literal_issue = validate_param_literal_value(
                str(decl.group("value") or "").strip(),
                marker_type or declared_type,
                marker_name or declared_name,
            )
            if literal_issue:
                issues.append(f"Line {line_no}: {literal_issue}")

            parsed_min = _parse_numeric_literal(str(meta.get("min") or ""))
            parsed_max = _parse_numeric_literal(str(meta.get("max") or ""))
            has_min = str(meta.get("min") or "").strip() != ""
            has_max = str(meta.get("max") or "").strip() != ""
            if has_min and parsed_min is None:
                issues.append(f"Line {line_no}: PARAM min value '{meta.get('min')}' is not a numeric literal")
            if has_max and parsed_max is None:
                issues.append(f"Line {line_no}: PARAM max value '{meta.get('max')}' is not a numeric literal")
            if parsed_min is not None and parsed_max is not None and parsed_min > parsed_max:
                issues.append(f"Line {line_no}: PARAM min {meta.get('min')} is greater than max {meta.get('max')}")

            declared_value = _parse_numeric_literal(str(decl.group("value") or ""))
            if declared_value is not None:
                if parsed_min is not None and declared_value < parsed_min:
                    issues.append(f"Line {line_no}: PARAM default value {decl.group('value').strip()} is below min {meta.get('min')}")
                if parsed_max is not None and declared_value > parsed_max:
                    issues.append(f"Line {line_no}: PARAM default value {decl.group('value').strip()} is above max {meta.get('max')}")
            continue

        if not decl:
            continue

        name = str(decl.group("name") or "").strip()
        value = str(decl.group("value") or "").strip()
        if param_like_name_re.search(name) and literal_value_re.match(value):
            issues.append(f"Line {line_no}: tunable constant '{name}' is missing a same-line PARAM marker")

    return issues


def _normalize_identifier(value: str) -> str:
    return re.sub(r"[^a-z0-9]", "", str(value or "").lower())


def _normalize_pin_value(value: str) -> str:
    raw = str(value or "").strip()
    raw = re.sub(r"\s+", "", raw)
    raw = raw.rstrip("uUlLfF")
    if re.fullmatch(r"D\d+", raw, flags=re.I):
        return raw[1:]
    return raw.upper()


def _pin_values_match(expected: str, actual: str) -> bool:
    return _normalize_pin_value(expected) == _normalize_pin_value(actual)


def parse_explicit_pin_definitions(pin_definitions: dict[str, str] | None) -> list[dict[str, str]]:
    expected: list[dict[str, str]] = []
    for raw_key, raw_value in (pin_definitions or {}).items():
        key = str(raw_key or "").strip()
        value = str(raw_value or "").strip()
        if not key or not value:
            continue

        # Frontend module format: {"alert": "buzzerPin=8, ledPin=D3"}
        if re.search(r"[=:]", value):
            module_id = key
            tokens = [part.strip() for part in re.split(r"[,;\n]+", value) if part.strip()]
            for token in tokens:
                match = re.match(r"(?P<name>[A-Za-z_][A-Za-z0-9_]*)\s*[=:]\s*(?P<value>[A-Za-z0-9_+-]+)", token)
                if not match:
                    continue
                expected.append(
                    {
                        "module": module_id,
                        "name": match.group("name").strip(),
                        "value": match.group("value").strip(),
                    }
                )
            continue

        # Direct API format: {"MIC_PIN": "A1"}
        if PIN_NAME_RE.search(key):
            expected.append({"module": "", "name": key, "value": value})

    return expected


def _extract_param_declarations(code_text: str) -> list[dict[str, str]]:
    marker_re = re.compile(r"\[PARAM:(?P<meta>[^\]]+)\]")
    decl_re = re.compile(
        r"^\s*(?:const\s+)?(?P<type>int|long|short|float|double|bool|byte|uint8_t|uint16_t|uint32_t|unsigned\s+long|unsigned\s+int)\s+"
        r"(?P<name>[A-Za-z_][A-Za-z0-9_]*)\s*=\s*(?P<value>[^;]+);"
    )

    def _parse_marker(meta: str) -> dict[str, str]:
        parsed: dict[str, str] = {}
        for token in (meta or "").split("|"):
            token = token.strip()
            if "=" not in token:
                continue
            key, value = token.split("=", 1)
            key = key.strip().lower()
            if key:
                parsed[key] = value.strip()
        return parsed

    declarations: list[dict[str, str]] = []
    for line_no, line in enumerate((code_text or "").splitlines(), start=1):
        marker = marker_re.search(line)
        decl = decl_re.search(line)
        if not marker or not decl:
            continue
        meta = _parse_marker(marker.group("meta"))
        declarations.append(
            {
                "line": str(line_no),
                "id": str(meta.get("id") or "").strip(),
                "module": str(meta.get("module") or "").strip(),
                "name": str(meta.get("name") or decl.group("name") or "").strip(),
                "decl_name": str(decl.group("name") or "").strip(),
                "value": str(decl.group("value") or "").strip(),
            }
        )
    return declarations


def validate_pin_definitions_applied(code_text: str, pin_definitions: dict[str, str] | None) -> list[str]:
    expected = parse_explicit_pin_definitions(pin_definitions)
    if not expected:
        return []

    declarations = _extract_param_declarations(code_text)
    issues: list[str] = []
    for item in expected:
        expected_module = item["module"]
        expected_name = item["name"]
        expected_value = item["value"]
        normalized_name = _normalize_identifier(expected_name)
        candidates = [
            decl
            for decl in declarations
            if _normalize_identifier(decl.get("name", "")) == normalized_name
            or _normalize_identifier(decl.get("decl_name", "")) == normalized_name
        ]

        if expected_module:
            candidates = [decl for decl in candidates if str(decl.get("module") or "").strip() == expected_module]

        if not candidates:
            module_hint = f" in module '{expected_module}'" if expected_module else ""
            issues.append(
                f"Explicit pin definition '{expected_name}={expected_value}'{module_hint} is not represented by a same-line PARAM declaration"
            )
            continue

        if not any(_pin_values_match(expected_value, str(decl.get("value") or "")) for decl in candidates):
            actual_values = ", ".join(sorted({str(decl.get("value") or "").strip() for decl in candidates if str(decl.get("value") or "").strip()}))
            module_hint = f" in module '{expected_module}'" if expected_module else ""
            issues.append(
                f"Explicit pin definition '{expected_name}'{module_hint} expects value '{expected_value}', but generated value is '{actual_values or '(empty)'}'"
            )

    return issues


def _classify_uno_pin_value(value: str) -> tuple[bool, str]:
    token = str(value or "").strip()
    token = re.sub(r"\s+", "", token)
    token = token.rstrip("uUlL")
    if token == "LED_BUILTIN":
        return True, "digital"

    analog_match = re.fullmatch(r"A(?P<num>\d+)", token, flags=re.I)
    if analog_match:
        pin_num = int(analog_match.group("num"))
        if 0 <= pin_num <= 5:
            return True, "analog"
        return False, "Arduino UNO analog pins are A0-A5"

    digital_match = re.fullmatch(r"D?(?P<num>\d+)", token, flags=re.I)
    if digital_match:
        pin_num = int(digital_match.group("num"))
        if 0 <= pin_num <= 13:
            return True, "digital"
        return False, "Arduino UNO digital pins are D0-D13"

    return False, "pin value must be a UNO literal such as A0-A5, D0-D13, 0-13, or LED_BUILTIN"


def _normalize_uno_digital_pin_number(value: str) -> int | None:
    token = str(value or "").strip()
    token = re.sub(r"\s+", "", token)
    token = token.rstrip("uUlL")
    if token == "LED_BUILTIN":
        return 13
    match = re.fullmatch(r"D?(?P<num>\d+)", token, flags=re.I)
    if not match:
        return None
    pin_num = int(match.group("num"))
    if 0 <= pin_num <= 13:
        return pin_num
    return None


def _normalize_uno_analog_pin_number(value: str) -> int | None:
    token = str(value or "").strip()
    token = re.sub(r"\s+", "", token)
    token = token.rstrip("uUlL")
    analog_match = re.fullmatch(r"A(?P<num>\d+)", token, flags=re.I)
    if analog_match:
        pin_num = int(analog_match.group("num"))
        if 0 <= pin_num <= 5:
            return pin_num
        return None

    channel_match = re.fullmatch(r"(?P<num>\d+)", token)
    if channel_match:
        channel = int(channel_match.group("num"))
        if 0 <= channel <= 5:
            return channel
    return None


def validate_uno_pin_values(code_text: str) -> list[str]:
    declarations = _extract_param_declarations(code_text)
    issues: list[str] = []
    for decl in declarations:
        name = str(decl.get("decl_name") or decl.get("name") or "").strip()
        if not name:
            continue
        if not (PIN_NAME_RE.search(str(decl.get("name") or "")) or PIN_NAME_RE.search(name)):
            continue

        value = str(decl.get("value") or "").strip()
        ok, reason = _classify_uno_pin_value(value)
        if ok:
            continue

        line = str(decl.get("line") or "?").strip() or "?"
        issues.append(
            f"Line {line}: pin parameter '{name}' uses invalid Arduino UNO pin value '{value}'; {reason}"
        )

    return issues


def validate_uno_pin_capabilities(code_text: str) -> list[str]:
    declarations = _extract_param_declarations(code_text)
    pin_values = {
        str(decl.get("decl_name") or decl.get("name") or "").strip(): str(decl.get("value") or "").strip()
        for decl in declarations
        if str(decl.get("decl_name") or decl.get("name") or "").strip()
        and (PIN_NAME_RE.search(str(decl.get("name") or "")) or PIN_NAME_RE.search(str(decl.get("decl_name") or "")))
    }
    if not pin_values:
        return []

    searchable = _strip_comments_and_strings(code_text or "")
    pwm_pins = {3, 5, 6, 9, 10, 11}
    interrupt_pins = {2, 3}
    issues: list[str] = []

    for call in _iter_function_calls(searchable, ["analogRead"]):
        args = call.get("args") if isinstance(call.get("args"), list) else []
        if not args:
            continue
        pin_name = str(args[0] or "").strip()
        if pin_name not in pin_values:
            continue
        pin_num = _normalize_uno_analog_pin_number(pin_values[pin_name])
        if pin_num is not None:
            continue
        line_no = _line_number_at(searchable, int(call.get("start") or 0))
        issues.append(
            f"Line {line_no}: analogRead uses pin parameter '{pin_name}' on '{pin_values[pin_name]}', but Arduino UNO analog input requires A0-A5 or numeric analog channels 0-5"
        )

    for call in _iter_function_calls(searchable, ["analogWrite"]):
        args = call.get("args") if isinstance(call.get("args"), list) else []
        if not args:
            continue
        pin_name = str(args[0] or "").strip()
        if pin_name not in pin_values:
            continue
        pin_num = _normalize_uno_digital_pin_number(pin_values[pin_name])
        if pin_num in pwm_pins:
            continue
        line_no = _line_number_at(searchable, int(call.get("start") or 0))
        issues.append(
            f"Line {line_no}: analogWrite uses pin parameter '{pin_name}' on '{pin_values[pin_name]}', but Arduino UNO PWM output requires D3, D5, D6, D9, D10, or D11"
        )

    for call in _iter_function_calls(searchable, ["digitalPinToInterrupt"]):
        args = call.get("args") if isinstance(call.get("args"), list) else []
        if not args:
            continue
        pin_name = str(args[0] or "").strip()
        if pin_name not in pin_values:
            continue
        pin_num = _normalize_uno_digital_pin_number(pin_values[pin_name])
        if pin_num in interrupt_pins:
            continue
        line_no = _line_number_at(searchable, int(call.get("start") or 0))
        issues.append(
            f"Line {line_no}: digitalPinToInterrupt uses pin parameter '{pin_name}' on '{pin_values[pin_name]}', but Arduino UNO external interrupts require D2 or D3"
        )

    return issues


def validate_reserved_serial_pins(code_text: str) -> list[str]:
    declarations = _extract_param_declarations(code_text)
    pin_declarations = [
        decl
        for decl in declarations
        if str(decl.get("decl_name") or decl.get("name") or "").strip()
        and (PIN_NAME_RE.search(str(decl.get("name") or "")) or PIN_NAME_RE.search(str(decl.get("decl_name") or "")))
    ]
    if not pin_declarations:
        return []

    searchable = _strip_comments_and_strings(code_text or "")
    digital_context_calls = [
        "pinMode",
        "digitalWrite",
        "digitalRead",
        "analogWrite",
        "tone",
        "noTone",
        "pulseIn",
        "shiftOut",
        "shiftIn",
        "attach",
        "attachInterrupt",
        "digitalPinToInterrupt",
    ]

    def _call_uses_var(call_name: str, var_name: str) -> bool:
        var_re = re.compile(rf"\b{re.escape(var_name)}\b")
        for call in _iter_function_calls(searchable, [call_name]):
            args = call.get("args") if isinstance(call.get("args"), list) else []
            if any(var_re.search(str(arg or "")) for arg in args):
                return True
        return False

    def _constructor_uses_var(var_name: str) -> bool:
        var_re = re.compile(rf"\b{re.escape(var_name)}\b")
        for ctor in _iter_library_pin_constructor_calls(searchable):
            class_name = str(ctor.get("class_name") or "")
            args = ctor.get("args") if isinstance(ctor.get("args"), list) else []
            pin_positions = LIBRARY_PIN_CONSTRUCTOR_ARG_POSITIONS.get(class_name, [])
            if any(idx < len(args) and var_re.search(str(args[idx] or "")) for idx in pin_positions):
                return True
        return False

    issues: list[str] = []
    for decl in pin_declarations:
        name = str(decl.get("decl_name") or decl.get("name") or "").strip()
        value = str(decl.get("value") or "").strip()
        pin_num = _normalize_uno_digital_pin_number(value)
        if pin_num not in {0, 1}:
            continue

        used_in_digital_context = any(_call_uses_var(call, name) for call in digital_context_calls) or _constructor_uses_var(name)
        if not used_in_digital_context:
            continue

        line = str(decl.get("line") or "?").strip() or "?"
        issues.append(
            f"Line {line}: pin parameter '{name}' uses Arduino UNO D{pin_num}, which is reserved for USB Serial/upload; "
            "use D2-D13 for editable digital hardware pins unless the value is only a numeric analogRead channel"
        )

    return issues


def _uno_physical_pin_key(value: str, *, numeric_analog_channel: bool = False) -> str | None:
    token = str(value or "").strip()
    token = re.sub(r"\s+", "", token)
    token = token.rstrip("uUlL")
    if token.upper() == "LED_BUILTIN":
        return "D13"

    analog_match = re.fullmatch(r"A(?P<num>\d+)", token, flags=re.I)
    if analog_match:
        pin_num = int(analog_match.group("num"))
        if 0 <= pin_num <= 5:
            return f"A{pin_num}"
        return None

    digital_match = re.fullmatch(r"D?(?P<num>\d+)", token, flags=re.I)
    if not digital_match:
        return None

    pin_num = int(digital_match.group("num"))
    if numeric_analog_channel and 0 <= pin_num <= 5:
        return f"A{pin_num}"
    if 0 <= pin_num <= 13:
        return f"D{pin_num}"
    return None


def validate_pin_assignment_conflicts(code_text: str) -> list[str]:
    declarations = _extract_param_declarations(code_text)
    pin_declarations = [
        decl
        for decl in declarations
        if str(decl.get("decl_name") or decl.get("name") or "").strip()
        and (PIN_NAME_RE.search(str(decl.get("name") or "")) or PIN_NAME_RE.search(str(decl.get("decl_name") or "")))
    ]
    if len(pin_declarations) < 2:
        return []

    searchable = _strip_comments_and_strings(code_text or "")
    digital_context_calls = [
        "pinMode",
        "digitalWrite",
        "digitalRead",
        "analogWrite",
        "tone",
        "noTone",
        "pulseIn",
        "shiftOut",
        "shiftIn",
        "attach",
        "attachInterrupt",
        "digitalPinToInterrupt",
    ]

    def _call_uses_var(call_name: str, var_name: str) -> bool:
        var_re = re.compile(rf"\b{re.escape(var_name)}\b")
        for call in _iter_function_calls(searchable, [call_name]):
            args = call.get("args") if isinstance(call.get("args"), list) else []
            if any(var_re.search(str(arg or "")) for arg in args):
                return True
        return False

    def _constructor_uses_var(var_name: str) -> bool:
        var_re = re.compile(rf"\b{re.escape(var_name)}\b")
        for ctor in _iter_library_pin_constructor_calls(searchable):
            class_name = str(ctor.get("class_name") or "")
            args = ctor.get("args") if isinstance(ctor.get("args"), list) else []
            pin_positions = LIBRARY_PIN_CONSTRUCTOR_ARG_POSITIONS.get(class_name, [])
            if any(idx < len(args) and var_re.search(str(args[idx] or "")) for idx in pin_positions):
                return True
        return False

    def _is_numeric_analog_channel(var_name: str) -> bool:
        used_by_analog_read = _call_uses_var("analogRead", var_name)
        if not used_by_analog_read:
            return False
        used_by_digital_context = any(_call_uses_var(call, var_name) for call in digital_context_calls)
        return not used_by_digital_context and not _constructor_uses_var(var_name)

    def _identity(decl: dict[str, str]) -> str:
        param_id = str(decl.get("id") or "").strip()
        if param_id:
            return f"id:{param_id}"
        module = str(decl.get("module") or "").strip()
        name = str(decl.get("decl_name") or decl.get("name") or "").strip()
        return f"{module}:{name}"

    def _label(decl: dict[str, str]) -> str:
        module = str(decl.get("module") or "").strip()
        name = str(decl.get("decl_name") or decl.get("name") or "").strip()
        return f"{module}.{name}" if module else name

    seen: dict[str, dict[str, str]] = {}
    issues: list[str] = []
    for decl in pin_declarations:
        name = str(decl.get("decl_name") or decl.get("name") or "").strip()
        value = str(decl.get("value") or "").strip()
        physical_pin = _uno_physical_pin_key(value, numeric_analog_channel=_is_numeric_analog_channel(name))
        if physical_pin is None:
            continue

        prior = seen.get(physical_pin)
        if prior is None:
            seen[physical_pin] = decl
            continue

        if _identity(prior) == _identity(decl):
            continue

        line = str(decl.get("line") or "?").strip() or "?"
        prior_line = str(prior.get("line") or "?").strip() or "?"
        issues.append(
            f"Line {line}: pin parameter '{_label(decl)}' defaults to Arduino UNO physical pin {physical_pin}, "
            f"which is already assigned to '{_label(prior)}' on line {prior_line}; "
            "use a separate default pin or one shared named PARAM only when shared wiring is intentional"
        )

    return issues


def validate_pin_usage(code_text: str) -> list[str]:
    src = code_text or ""
    declarations = _extract_param_declarations(src)
    pin_declarations = [
        decl
        for decl in declarations
        if PIN_NAME_RE.search(str(decl.get("name") or "")) or PIN_NAME_RE.search(str(decl.get("decl_name") or ""))
    ]
    if not pin_declarations:
        return []

    searchable = _strip_comments_and_strings(src)
    setup_span = _find_function_span(searchable, "setup")
    setup_body = searchable[setup_span[0] : setup_span[1]] if setup_span else ""

    hardware_calls = [
        "pinMode",
        "digitalWrite",
        "digitalRead",
        "analogRead",
        "analogWrite",
        "tone",
        "noTone",
        "pulseIn",
        "shiftOut",
        "shiftIn",
        "attach",
        "attachInterrupt",
        "digitalPinToInterrupt",
    ]
    output_calls = ["digitalWrite", "analogWrite", "tone", "noTone", "shiftOut"]
    input_calls = ["digitalRead", "pulseIn", "attachInterrupt", "digitalPinToInterrupt"]

    def _call_uses_var(call_name: str, var_name: str, source: str) -> bool:
        var_re = re.compile(rf"\b{re.escape(var_name)}\b")
        for call in _iter_function_calls(source, [call_name]):
            args = call.get("args") if isinstance(call.get("args"), list) else []
            if any(var_re.search(str(arg or "")) for arg in args):
                return True
        return False

    def _pin_mode_values(var_name: str) -> list[str]:
        var_re = re.compile(rf"\b{re.escape(var_name)}\b")
        values: list[str] = []
        for call in _iter_function_calls(setup_body, ["pinMode"]):
            args = call.get("args") if isinstance(call.get("args"), list) else []
            if len(args) < 2 or not var_re.search(str(args[0] or "")):
                continue
            values.append(str(args[1] or "").strip().upper())
        return values

    def _constructor_uses_var(var_name: str) -> bool:
        var_re = re.compile(rf"\b{re.escape(var_name)}\b")
        for ctor in _iter_library_pin_constructor_calls(searchable):
            class_name = str(ctor.get("class_name") or "")
            args = ctor.get("args") if isinstance(ctor.get("args"), list) else []
            pin_positions = LIBRARY_PIN_CONSTRUCTOR_ARG_POSITIONS.get(class_name, [])
            if any(idx < len(args) and var_re.search(str(args[idx] or "")) for idx in pin_positions):
                return True
        return False

    issues: list[str] = []
    for decl in pin_declarations:
        name = str(decl.get("decl_name") or decl.get("name") or "").strip()
        if not name:
            continue

        used_in_hardware = any(_call_uses_var(call, name, searchable) for call in hardware_calls) or _constructor_uses_var(name)
        if not used_in_hardware:
            issues.append(f"Pin parameter '{name}' is declared as editable PARAM but is not used in any Arduino hardware call")
            continue

        used_as_output = any(_call_uses_var(call, name, searchable) for call in output_calls)
        has_pin_mode = _call_uses_var("pinMode", name, setup_body)
        pin_mode_values = _pin_mode_values(name)
        has_output_mode = any(mode == "OUTPUT" for mode in pin_mode_values)
        has_input_mode = any(mode in {"INPUT", "INPUT_PULLUP"} for mode in pin_mode_values)
        if used_as_output and not has_pin_mode:
            issues.append(f"Output pin parameter '{name}' is used for output but missing pinMode({name}, ...) in setup()")
        elif used_as_output and not has_output_mode:
            actual = ", ".join(pin_mode_values) if pin_mode_values else "(unknown)"
            issues.append(f"Output pin parameter '{name}' must use pinMode({name}, OUTPUT) in setup(); found mode(s): {actual}")

        used_as_input = any(_call_uses_var(call, name, searchable) for call in input_calls)
        if used_as_input and not has_pin_mode:
            issues.append(f"Input pin parameter '{name}' is read as digital input but missing pinMode({name}, INPUT or INPUT_PULLUP) in setup()")
        elif used_as_input and not has_input_mode:
            actual = ", ".join(pin_mode_values) if pin_mode_values else "(unknown)"
            issues.append(f"Input pin parameter '{name}' must use pinMode({name}, INPUT or INPUT_PULLUP) in setup(); found mode(s): {actual}")

    return issues


def _split_call_args(args_src: str) -> list[str]:
    args: list[str] = []
    current: list[str] = []
    depth = 0
    for ch in args_src or "":
        if ch in "([{":
            depth += 1
        elif ch in ")]}" and depth > 0:
            depth -= 1
        if ch == "," and depth == 0:
            args.append("".join(current).strip())
            current = []
            continue
        current.append(ch)
    tail = "".join(current).strip()
    if tail:
        args.append(tail)
    return args


def _iter_function_calls(source: str, call_names: list[str]) -> list[dict[str, object]]:
    if not source or not call_names:
        return []

    name_pattern = "|".join(re.escape(name) for name in call_names)
    call_re = re.compile(rf"\b(?P<name>{name_pattern})\s*\(")
    calls: list[dict[str, object]] = []
    for match in call_re.finditer(source):
        open_idx = match.end() - 1
        depth = 0
        close_idx = -1
        for idx in range(open_idx, len(source)):
            ch = source[idx]
            if ch == "(":
                depth += 1
            elif ch == ")":
                depth -= 1
                if depth == 0:
                    close_idx = idx
                    break
        if close_idx < 0:
            continue
        args_src = source[open_idx + 1 : close_idx]
        calls.append(
            {
                "name": match.group("name"),
                "start": match.start(),
                "args": _split_call_args(args_src),
            }
        )
    return calls


def _iter_library_pin_constructor_calls(source: str) -> list[dict[str, object]]:
    if not source:
        return []

    class_pattern = "|".join(re.escape(name) for name in LIBRARY_PIN_CONSTRUCTOR_ARG_POSITIONS)
    ctor_re = re.compile(
        rf"\b(?P<class_name>{class_pattern})\s+(?P<object_name>[A-Za-z_][A-Za-z0-9_]*)\s*\("
    )
    constructors: list[dict[str, object]] = []
    for match in ctor_re.finditer(source):
        open_idx = match.end() - 1
        depth = 0
        close_idx = -1
        for idx in range(open_idx, len(source)):
            ch = source[idx]
            if ch == "(":
                depth += 1
            elif ch == ")":
                depth -= 1
                if depth == 0:
                    close_idx = idx
                    break
        if close_idx < 0:
            continue
        constructors.append(
            {
                "class_name": match.group("class_name"),
                "object_name": match.group("object_name"),
                "start": match.start(),
                "args": _split_call_args(source[open_idx + 1 : close_idx]),
            }
        )
    return constructors


def _is_raw_pin_literal(value: str) -> bool:
    token = str(value or "").strip()
    token = re.sub(r"\s+", "", token)
    token = token.rstrip("uUlL")
    return bool(re.fullmatch(r"(?:A\d+|D?\d+)", token, flags=re.I))


def _line_number_at(source: str, offset: int) -> int:
    return source.count("\n", 0, max(0, offset)) + 1


def validate_hardware_pin_references(code_text: str) -> list[str]:
    searchable = _strip_comments_and_strings(code_text or "")
    pin_arg_positions = {
        "pinMode": [0],
        "digitalWrite": [0],
        "digitalRead": [0],
        "analogRead": [0],
        "analogWrite": [0],
        "tone": [0],
        "noTone": [0],
        "pulseIn": [0],
        "shiftOut": [0, 1],
        "shiftIn": [0, 1],
        "attach": [0],
    }
    issues: list[str] = []

    for call in _iter_function_calls(searchable, list(pin_arg_positions.keys())):
        call_name = str(call.get("name") or "")
        args = call.get("args") if isinstance(call.get("args"), list) else []
        for arg_idx in pin_arg_positions.get(call_name, []):
            if arg_idx >= len(args):
                continue
            arg = str(args[arg_idx] or "").strip()
            if not _is_raw_pin_literal(arg):
                continue
            line_no = _line_number_at(searchable, int(call.get("start") or 0))
            issues.append(
                f"Line {line_no}: {call_name} uses raw pin literal '{arg}'; declare a named pin variable with a same-line PARAM marker and use that variable"
            )

    for match in re.finditer(r"\bdigitalPinToInterrupt\s*\(\s*(?P<pin>A\d+|D?\d+)\s*\)", searchable, flags=re.I):
        pin = str(match.group("pin") or "").strip()
        line_no = _line_number_at(searchable, match.start())
        issues.append(
            f"Line {line_no}: digitalPinToInterrupt uses raw pin literal '{pin}'; declare a named interrupt pin variable with a same-line PARAM marker"
        )

    for ctor in _iter_library_pin_constructor_calls(searchable):
        class_name = str(ctor.get("class_name") or "")
        args = ctor.get("args") if isinstance(ctor.get("args"), list) else []
        pin_positions = LIBRARY_PIN_CONSTRUCTOR_ARG_POSITIONS.get(class_name, [])
        for arg_idx in pin_positions:
            if arg_idx >= len(args):
                continue
            arg = str(args[arg_idx] or "").strip()
            if not _is_raw_pin_literal(arg):
                continue
            line_no = _line_number_at(searchable, int(ctor.get("start") or 0))
            issues.append(
                f"Line {line_no}: {class_name} constructor uses raw pin literal '{arg}'; declare a named pin variable with a same-line PARAM marker and use that variable"
            )

    return issues


def _parse_integer_literal(value: str) -> int | None:
    token = str(value or "").strip()
    token = re.sub(r"\s+", "", token)
    token = re.sub(r"[uUlL]+$", "", token)
    if not re.fullmatch(r"[-+]?\d+", token):
        return None
    try:
        return int(token, 10)
    except ValueError:
        return None


def _extract_integer_declarations(code_text: str) -> dict[str, int]:
    cleaned = _strip_comments_and_strings(code_text or "")
    decl_re = re.compile(
        r"\b(?:const\s+)?(?:unsigned\s+long|unsigned\s+int|long|int|short|byte|uint8_t|uint16_t|uint32_t)\s+"
        r"(?P<name>[A-Za-z_][A-Za-z0-9_]*)\s*=\s*(?P<value>[^;]+);"
    )
    values: dict[str, int] = {}
    for match in decl_re.finditer(cleaned):
        parsed = _parse_integer_literal(match.group("value"))
        if parsed is not None:
            values[match.group("name")] = parsed
    return values


def _resolve_integer_argument(arg: str, declarations: dict[str, int]) -> int | None:
    parsed = _parse_integer_literal(arg)
    if parsed is not None:
        return parsed
    name = str(arg or "").strip()
    if re.fullmatch(r"[A-Za-z_][A-Za-z0-9_]*", name):
        return declarations.get(name)
    return None


def _extract_function_body_spans(src: str) -> dict[str, tuple[int, int]]:
    signature_re = re.compile(
        r"\b(?:void|bool|int|long|short|float|double|byte|char|unsigned\s+long|unsigned\s+int|uint8_t|uint16_t|uint32_t)\s+"
        r"(?P<name>[A-Za-z_][A-Za-z0-9_]*)\s*\([^;{}]*\)\s*\{"
    )
    spans: dict[str, tuple[int, int]] = {}
    for match in signature_re.finditer(src or ""):
        name = str(match.group("name") or "").strip()
        if not name or name in spans:
            continue

        body_start = match.end()
        depth = 1
        for idx in range(body_start, len(src)):
            ch = src[idx]
            if ch == "{":
                depth += 1
            elif ch == "}":
                depth -= 1
                if depth == 0:
                    spans[name] = (body_start, idx)
                    break
    return spans


def validate_blocking_delays(code_text: str, max_loop_delay_ms: int = 250) -> list[str]:
    src = code_text or ""
    searchable = _strip_comments_and_strings(src)
    function_spans = _extract_function_body_spans(searchable)
    loop_span = function_spans.get("loop")
    if not loop_span:
        return []

    declarations = _extract_integer_declarations(src)
    issues: list[str] = []

    callable_names = [name for name in function_spans if name not in {"setup", "loop"}]
    queued = ["loop"]
    visited: set[str] = set()
    while queued:
        func_name = queued.pop(0)
        if func_name in visited:
            continue
        visited.add(func_name)

        span = function_spans.get(func_name)
        if not span:
            continue

        body_src = searchable[span[0] : span[1]]
        scope = "loop()" if func_name == "loop" else f"helper '{func_name}' reachable from loop()"
        for match in re.finditer(r"\bdelay\s*\(\s*(?P<arg>[^()]+?)\s*\)", body_src):
            arg = str(match.group("arg") or "").strip()
            delay_ms = _resolve_integer_argument(arg, declarations)
            if delay_ms is None:
                continue

            line_no = searchable.count("\n", 0, span[0] + match.start()) + 1
            if delay_ms < 0:
                issues.append(f"Line {line_no}: delay({arg}) resolves to a negative value; use a non-negative interval")
            elif delay_ms > max_loop_delay_ms:
                issues.append(
                    f"Line {line_no}: delay({arg}) in {scope} blocks hardware responsiveness for {delay_ms} ms; "
                    f"use millis()-based scheduling or keep loop delay <= {max_loop_delay_ms} ms"
                )

        for call in _iter_function_calls(body_src, callable_names):
            callee = str(call.get("name") or "").strip()
            if callee and callee not in visited:
                queued.append(callee)

    return issues


def validate_dynamic_memory_usage(code_text: str) -> list[str]:
    searchable = _strip_comments_and_strings(code_text or "")
    checks = [
        ("Arduino String", r"\bString\b"),
        ("malloc/calloc/realloc/free", r"\b(?:malloc|calloc|realloc|free)\s*\("),
        ("new/delete", r"\b(?:new\s+[A-Za-z_][A-Za-z0-9_:<>]*|delete\s+)"),
        ("STL container", r"\bstd::(?:string|vector|map|set|list|deque|unordered_map|unordered_set)\b"),
    ]
    issues: list[str] = []
    for label, pattern in checks:
        match = re.search(pattern, searchable)
        if not match:
            continue
        line_no = searchable.count("\n", 0, match.start()) + 1
        issues.append(
            f"Line {line_no}: Arduino UNO code uses {label}; avoid dynamic heap allocation and use fixed-size buffers or static storage"
        )
    return issues


def validate_runtime_safety(code_text: str) -> list[str]:
    issues: list[str] = []
    issues.extend(validate_blocking_delays(code_text))
    issues.extend(validate_dynamic_memory_usage(code_text))
    return issues


def validate_arduino_static_quality(
    code_text: str,
    *,
    module_ids: list[str] | None = None,
    pin_definitions: dict[str, str] | None = None,
) -> list[str]:
    issues: list[str] = []
    issues.extend(validate_arduino_code(code_text))
    issues.extend(validate_uno_board_compatibility(code_text))
    issues.extend(validate_module_markers(code_text, module_ids))
    issues.extend(validate_module_marker_structure(code_text))
    issues.extend(validate_module_boundaries(code_text))
    issues.extend(validate_module_execution(code_text))
    issues.extend(validate_param_markers(code_text, module_ids))
    issues.extend(validate_pin_definitions_applied(code_text, pin_definitions))
    issues.extend(validate_uno_pin_values(code_text))
    issues.extend(validate_uno_pin_capabilities(code_text))
    issues.extend(validate_reserved_serial_pins(code_text))
    issues.extend(validate_pin_assignment_conflicts(code_text))
    issues.extend(validate_pin_usage(code_text))
    issues.extend(validate_hardware_pin_references(code_text))
    return issues


def check_requirement_coverage(req_text: str, code_text: str) -> list[str]:
    issues: list[str] = []
    source = req_text or ""
    m = re.search(r"用户需求\s*[:：]\s*(.+)$", source, re.S)
    if m:
        source = m.group(1).strip()
    rtxt = source.lower()
    ctxt = (code_text or "").lower()
    expectations: list[tuple[list[str], list[str], str]] = [
        (
            ["电机", "马达", "motor", "小车", "car"],
            ["motor", "analogwrite", "pwm", "servo", "digitalwrite", "forward", "backward"],
            "Requirement mentions motor/vehicle control, but code lacks motor control signals",
        ),
        (
            ["蜂鸣器", "报警", "告警", "buzzer", "beep", "alarm"],
            ["buzzer", "tone(", "notone(", "digitalwrite", "alarm"],
            "Requirement mentions alarm/buzzer behavior, but code lacks alert control logic",
        ),
        (
            ["按键", "按钮", "button", "key"],
            ["digitalread", "button", "key", "input_pullup"],
            "Requirement mentions button input, but code lacks input-reading logic",
        ),
        (
            ["串口", "日志", "serial", "print", "[diag]"],
            ["serial.begin", "serial.print", "serial.println", "[diag]"],
            "Requirement mentions serial/log output, but code lacks serial diagnostics",
        ),
        (
            ["显示", "数码管", "display", "lcd", "oled", "tm1637"],
            ["display", "lcd", "oled", "tm1637", "segment"],
            "Requirement mentions display output, but code lacks display logic",
        ),
    ]
    for req_keys, code_keys, message in expectations:
        if any(key in rtxt for key in req_keys) and not any(key in ctxt for key in code_keys):
            issues.append(message)
    return issues


def compile_arduino_uno(code_text: str, output_dir: str | Path, fqbn: str = "arduino:avr:uno") -> ArduinoCompileCheck:
    cli = shutil.which("arduino-cli")
    if not cli:
        return ArduinoCompileCheck(ok=False, log="arduino-cli is not available; compile check was skipped", cli_missing=True)

    base_dir = Path(output_dir).resolve()
    base_dir.mkdir(parents=True, exist_ok=True)
    try:
        with tempfile.TemporaryDirectory(prefix="arduino_compile_", dir=str(base_dir)) as temp_dir:
            sketch_dir = Path(temp_dir)
            sketch_path = sketch_dir / f"{sketch_dir.name}.ino"
            sketch_path.write_text(code_text, encoding="utf-8")
            proc = subprocess.run(
                [cli, "compile", "--fqbn", fqbn, str(sketch_dir)],
                capture_output=True,
                text=True,
                timeout=180,
            )
            msg = ((proc.stdout or "") + "\n" + (proc.stderr or "")).strip()
            return ArduinoCompileCheck(ok=proc.returncode == 0, log=msg[-6000:], cli_missing=False)
    except Exception as exc:
        return ArduinoCompileCheck(ok=False, log=f"arduino-cli compile check failed: {type(exc).__name__}: {exc}")


def build_arduino_repair_prompt(
    issues: list[str],
    code: str,
    *,
    user_requirements: str | None = None,
    module_specs: list[dict] | None = None,
    pin_definitions: dict[str, str] | None = None,
) -> str:
    issue_lines = "\n".join(f"- {issue}" for issue in issues)
    module_lines: list[str] = []
    for idx, item in enumerate(module_specs or [], start=1):
        if not isinstance(item, dict):
            continue
        module_id = str(item.get("id") or "").strip()
        if not module_id:
            continue
        title = str(item.get("title") or module_id).strip()
        module_type = str(item.get("moduleType") or item.get("module_type") or "custom").strip()
        subtitle = str(item.get("subtitle") or "").strip()
        module_lines.append(f"{idx}. id={module_id}; title={title}; type={module_type}; role={subtitle}")

    pin_lines = [f"- {key}: {value}" for key, value in (pin_definitions or {}).items() if str(key).strip() and str(value).strip()]
    context_parts = [
        "Repair context:",
        f"User requirements:\n{(user_requirements or '').strip() or '(not provided)'}",
        "Expected module ids and roles:\n" + ("\n".join(module_lines) if module_lines else "(not provided)"),
        "Explicit pin definitions:\n" + ("\n".join(pin_lines) if pin_lines else "(not provided)"),
        (
            "Marker contract:\n"
            "- Every expected module id must have exactly one editable block with standalone lines "
            "`// [MODULE:<module_id>:start]` and `// [MODULE:<module_id>:end]` around that module's helper function or code block.\n"
            "- MODULE blocks must be non-overlapping and never nested; setup() and loop() must stay outside all MODULE blocks.\n"
            "- setup() and loop() must remain complete Arduino functions and must not be fully wrapped by MODULE markers.\n"
            "- If a MODULE block defines a helper function, that function must be called from setup(), loop(), or another module, or registered as an interrupt callback.\n"
            "- Every tunable threshold, timing, gain, frequency, speed, duty, min/max, and hardware pin declaration must have a same-line [PARAM:...] marker.\n"
            "- Every PARAM default value must be a single literal on the declaration line, not an expression, function call, comment, extra statement, placeholder, or computed value.\n"
            "- Target exactly Arduino UNO bare hardware. Do not use ESP32/ESP8266 APIs, WiFi/Bluetooth headers, desktop C++ headers/console I/O, C stdio console I/O, custom main(), ledc*, INPUT_PULLDOWN, Serial1/Serial2, analogReadResolution, analogWriteResolution, or touchRead.\n"
            "- Every pin PARAM value must be valid for Arduino UNO: digital pins 0-13 / D0-D13, analog pins A0-A5, or LED_BUILTIN.\n"
            "- Arduino UNO analogRead sensor inputs must use A0-A5 by default; numeric analog channels 0-5 are acceptable only when explicitly provided by the user.\n"
            "- Arduino UNO analogWrite/PWM output must use D3, D5, D6, D9, D10, or D11; external interrupts through digitalPinToInterrupt must use D2 or D3.\n"
            "- Avoid D0/D1 for editable digital hardware pins and pin constructors because they are reserved for USB Serial/upload; use numeric 0/1 only as intended analogRead channels.\n"
            "- Do not assign the same physical Arduino UNO pin default to multiple distinct pin PARAMs unless shared wiring is intentional; prefer one shared named PARAM for shared bus pins.\n"
            "- Every hardware API pin argument must use a named pin variable with a same-line [PARAM:...] marker; do not call pinMode/digitalWrite/analogRead/tone/Servo.attach/TM1637Display constructors/LiquidCrystal constructors/SoftwareSerial constructors/Stepper constructors/etc. with raw literals such as 8, D3, or A0.\n"
            "- If using Servo, LiquidCrystal, SoftwareSerial, or Stepper, include #include <Servo.h> / #include <LiquidCrystal.h> / #include <SoftwareSerial.h> / #include <Stepper.h> and declare valid objects before use.\n"
            "- If using Wire.* or SPI.*, include #include <Wire.h> / #include <SPI.h> as real include lines, not commented-out text.\n"
            "- Pins used with output APIs (digitalWrite/analogWrite/tone/noTone/shiftOut) must be initialized in setup() with pinMode(<pin>, OUTPUT).\n"
            "- Pins used with digital input, pulse, or interrupt APIs must be initialized in setup() with pinMode(<pin>, INPUT) or pinMode(<pin>, INPUT_PULLUP); analogRead-only pins do not require pinMode.\n"
            "- PARAM metadata must include id, module, name, type; module must be one of the expected module ids."
        ),
        (
            "Runtime reliability contract:\n"
            "- loop() must stay responsive; use millis()-based scheduling for waits longer than 250 ms instead of long delay(...) calls.\n"
            "- Avoid Arduino String, malloc/calloc/realloc/free, new/delete, and STL containers on Arduino UNO; use fixed-size buffers or static storage."
        ),
    ]
    return (
        "Fix the Arduino sketch below so it is complete, directly compilable on Arduino UNO, "
        "and satisfies all listed issues. Output only the corrected .ino code, with no Markdown.\n\n"
        + "\n\n".join(context_parts)
        + "\n\n"
        f"Issues:\n{issue_lines}\n\n"
        "Current Arduino sketch:\n"
        f"{code}\n"
    )
