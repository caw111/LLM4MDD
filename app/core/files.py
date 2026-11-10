import os
import re
from pathlib import Path

from fastapi import HTTPException


def _safe_list_files(root: str) -> list[str]:
    base = Path(root).resolve()
    if not base.exists():
        return []
    out: list[str] = []
    for p in base.rglob("*"):
        if p.is_file():
            out.append(str(p.relative_to(base)).replace("\\", "/"))
    return sorted(out)


def _safe_read_under(root: str, rel: str) -> str:
    base = Path(root).resolve()
    target = (base / rel).resolve()
    if base not in target.parents and target != base:
        raise HTTPException(status_code=400, detail="Invalid path")
    if not target.exists() or not target.is_file():
        raise HTTPException(status_code=404, detail="File not found")
    return target.read_text(encoding="utf-8")


def _write_text(path: str, content: str) -> None:
    os.makedirs(os.path.dirname(path) or ".", exist_ok=True)
    with open(path, "w", encoding="utf-8") as handle:
        handle.write(content)


def _resolve_path(path_like: str) -> Path:
    raw = (path_like or "").strip().strip('"').strip("'")

    if os.name == "nt":
        # Support WSL-style absolute paths when backend runs on Windows.
        m_wsl = re.match(r"^/mnt/([A-Za-z])/(.*)$", raw)
        if m_wsl:
            drive = m_wsl.group(1).upper()
            rest = m_wsl.group(2).replace("/", "\\")
            return Path(f"{drive}:\\{rest}").resolve()

        # Recover from malformed path like C:\mnt\c\Users\...
        m_bad = re.match(r"^[A-Za-z]:[\\/]mnt[\\/]([A-Za-z])[\\/](.*)$", raw)
        if m_bad:
            drive = m_bad.group(1).upper()
            rest = m_bad.group(2).replace("/", "\\")
            return Path(f"{drive}:\\{rest}").resolve()

    path = Path(raw)
    if path.is_absolute():
        return path.resolve()
    return (Path.cwd() / path).resolve()
