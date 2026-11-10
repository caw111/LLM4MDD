"""Freeze locally installed method/tool identities without exposing environment secrets."""
import hashlib
import importlib.metadata
from pathlib import Path
import platform
import shutil
import subprocess

ROOT = Path(__file__).resolve().parents[2]


def runtime_identity():
    files = [ROOT/'.tools/sysml/kernel/sysml/jupyter-sysml-kernel-0.62.0-all.jar',
             ROOT/'CodeGenerator2/Exe/ILTranslator/ILTranslator.exe']
    binaries = {str(p.relative_to(ROOT)): hashlib.sha256(p.read_bytes()).hexdigest() if p.is_file() else None for p in files}
    commands = {}
    for name, args in [('arduino-cli', ['version']), ('arduino-cli', ['core', 'list']), ('arduino-cli', ['lib', 'list']), ('java', ['-version']), ('g++', ['--version']), ('ar', ['--version'])]:
        executable = shutil.which(name)
        if name == 'java':
            from app.services.sysml import RUNTIME
            import os
            local = sorted((RUNTIME/'java').glob('*/bin/java.exe'))
            executable = os.environ.get('SYSML_JAVA') or (str(local[0]) if local else executable)
        key = name + ' ' + ' '.join(args)
        if not executable:
            commands[key] = 'missing'
            continue
        proc = subprocess.run([executable, *args], capture_output=True, text=True, encoding='utf-8', errors='replace', timeout=30)
        commands[key] = (proc.stdout + proc.stderr).strip()
    packages = {}
    for name in ('pycparser', 'openai', 'fastapi'):
        try:
            packages[name] = importlib.metadata.version(name)
        except importlib.metadata.PackageNotFoundError:
            packages[name] = None
    return {'python': platform.python_version(), 'packages': packages, 'binaries_sha256': binaries, 'tools': commands}
