"""Run firmware compilation inside WSL, with no Windows compiler children."""
import hashlib
import argparse
import json
import os
from pathlib import Path
import shutil
import subprocess
import tarfile
import time

ROOT = Path(__file__).resolve().parents[2]
OUT = ROOT / 'experiment/output/picsimlab_verification_20261107'
BASE = Path.home() / '.cache/llm4mdd-arduino'


def status(stage, **details):
    value = dict(stage=stage, updated_at=time.strftime('%Y-%m-%dT%H:%M:%S%z'),
                 pid=os.getpid(), nice=os.getpriority(os.PRIO_PROCESS, 0),
                 cpu_affinity=sorted(os.sched_getaffinity(0)), **details)
    temporary = OUT / 'background_status.json.tmp'
    temporary.write_text(json.dumps(value, indent=2), encoding='utf8')
    temporary.replace(OUT / 'background_status.json')
    print(json.dumps(value), flush=True)


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--prepare-only',action='store_true')
    parser.add_argument('--case',action='append',default=[])
    args=parser.parse_args()
    if os.name != 'posix' or not Path('/proc/sys/fs/binfmt_misc/WSLInterop').exists():
        raise SystemExit('This launcher requires WSL; Windows executables are prohibited.')
    os.nice(15)
    os.sched_setaffinity(0, {min(os.sched_getaffinity(0))})
    BASE.mkdir(parents=True, exist_ok=True)
    status('preparing_linux_toolchain')
    archive = ROOT / '.tools/arduino/arduino-cli_linux64.tar.gz'
    expected = '396b4dd7d405ce116b7c95c0941784b005d989c52adfde51d983f66c4b0f9531'
    assert hashlib.sha256(archive.read_bytes()).hexdigest() == expected
    cli = BASE / 'arduino-cli'
    if not cli.exists():
        with tarfile.open(archive) as package:
            item = next(m for m in package.getmembers() if m.name == 'arduino-cli')
            package.extract(item, BASE, filter='data')
    data = BASE / 'data'
    data.mkdir(exist_ok=True)
    windows_data = Path('/mnt/c/Users/27872/AppData/Local/Arduino15')
    for name in ('package_index.json', 'package_index.json.sig', 'library_index.json', 'library_index.json.sig'):
        if not (data / name).exists():
            shutil.copyfile(windows_data / name, data / name)
    config = BASE / 'arduino-cli.yaml'
    config.write_text(json.dumps({'directories': {
        'data': str(data), 'downloads': str(BASE / 'downloads'),
        'user': '/mnt/c/Users/27872/Documents/Arduino'}}), encoding='utf8')
    os.environ['ARDUINO_CONFIG_FILE'] = str(config)
    core = data / 'packages/arduino/hardware/avr/1.8.6'
    if not core.exists():
        status('installing_linux_avr_core')
        with (OUT / 'linux_core_install.log').open('ab') as log:
            result = subprocess.run([str(cli), '--config-file', str(config), 'core', 'install',
                                     'arduino:avr@1.8.6'], stdout=log, stderr=log, timeout=900)
        if result.returncode:
            raise RuntimeError('Native AVR core installation failed; see linux_core_install.log')
    nm = data / 'packages/arduino/tools/avr-gcc/7.3.0-atmel3.6.1-arduino7/bin/avr-nm'
    assert nm.is_file(), str(nm)
    os.environ['PICSIMLAB_ARDUINO_CLI'] = str(cli)
    os.environ['PICSIMLAB_AVR_NM'] = str(nm)
    os.environ['PICSIMLAB_BUILD_CACHE'] = str(BASE / 'build_workers')
    if args.prepare_only:
        status('linux_toolchain_ready')
        return
    status('compiling_firmware')
    command=['python3', str(ROOT / 'experiment/code/compile_picsimlab_firmware.py'),'--workers','1']
    for case in args.case:
        command.extend(['--case',case])
    result = subprocess.run(command, timeout=12 * 3600)
    status('compilation_finished', exit_code=result.returncode)


if __name__ == '__main__':
    try:
        main()
    except Exception as exc:
        status('blocked', error_type=type(exc).__name__, error=str(exc))
        raise
