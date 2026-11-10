"""Small native-firmware timing probe for the Linux PICSimLab NOGUI build."""
import json
import os
from pathlib import Path
import re
import socket
import subprocess
import time

ROOT = Path(__file__).resolve().parents[2]
OUT = ROOT / 'experiment/output/picsimlab_verification_20261107'
BASE = Path.home() / '.cache/llm4mdd-picsimlab-0.9.3'


class Remote:
    def __init__(self):
        self.sock = socket.create_connection(('127.0.0.1', 5000), 2)
        self.sock.settimeout(10)
        self.log = []
        self.receive()

    def receive(self):
        data = b''
        while not data.rstrip().endswith(b'>'):
            chunk = self.sock.recv(65536)
            if not chunk:
                raise ConnectionError('PICSimLab closed the connection')
            data += chunk
        return data.decode(errors='replace')

    def command(self, command):
        self.sock.sendall((command + '\n').encode())
        reply = self.receive()
        self.log.append(dict(command=command, reply=reply))
        if 'ERROR' in reply:
            raise RuntimeError(f'{command}: {reply}')
        return reply

    def millis(self, address):
        reply = self.command(f'dumpr {address:x} 4')
        match = re.search(r'^[0-9A-Fa-f]+: ((?:[0-9A-Fa-f]{2} ?){4})', reply, re.M)
        if not match:
            raise ValueError(reply)
        return int.from_bytes(bytes.fromhex(match[1]), 'little')


def main():
    assert os.name == 'posix'
    os.nice(15)
    os.sched_setaffinity(0, {min(os.sched_getaffinity(0))})
    env = dict(os.environ, DISPLAY='', WAYLAND_DISPLAY='', PULSE_SERVER='',
               ALSOFT_DRIVERS='null', APPDIR=str(BASE / 'squashfs-root'),
               LD_PRELOAD=str(BASE / 'profile_compat.so'),
               LD_LIBRARY_PATH=f'{BASE}/squashfs-root/usr/lib:{BASE}/squashfs-root/usr/lib/picsimlab')
    empty = OUT / 'probe_empty.pcf'
    empty.write_text('version,0,0,0:0.9.3\nscale,0,0,0:1.0\nspare_on,0,0,0:1\nosc_on,0,0,0:0\n', encoding='ascii')
    remote = None
    with (OUT / 'timing_simulator.log').open('wb') as log:
        app = subprocess.Popen([str(BASE / 'squashfs-root/AppRun'), 'Arduino_Mega', 'atmega2560',
                                str(OUT / 'cases/embedbench_1/firmware/sketch.ino.hex'), str(empty)],
                               cwd=BASE, env=env, stdout=log, stderr=log, start_new_session=True)
        try:
            deadline = time.monotonic() + 20
            while time.monotonic() < deadline:
                if app.poll() is not None:
                    raise RuntimeError(f'PICSimLab exited with {app.returncode}')
                try:
                    remote = Remote()
                    break
                except OSError:
                    time.sleep(.2)
            assert remote is not None, 'Remote interface did not start'
            remote.command('sim stop')
            info = remote.command('info')
            assert 'atmega2560' in info, info
            remote.command('version')
            remote.command('pinsl')
            remote.command('splist')
            remote.command('spdel all')
            remote.command('reset')
            address = json.loads((OUT / 'cases/embedbench_1/compilation.json').read_text())['millis_ram_address']
            samples = []
            started = time.monotonic()
            remote.command('sim start')
            for _ in range(8):
                time.sleep(.5)
                samples.append(dict(wall_seconds=time.monotonic() - started,
                                    firmware_ms=remote.millis(address)))
            remote.command('sim stop')
            result = dict(status='probe_complete', engine='PICSimLab 0.9.3 Linux64_NOGUI',
                          samples=samples, board_info=info,
                          cpu_affinity=sorted(os.sched_getaffinity(0)), nice=os.getpriority(os.PRIO_PROCESS, 0))
            (OUT / 'timing_probe.json').write_text(json.dumps(result, indent=2))
            print(json.dumps(result), flush=True)
        finally:
            if remote:
                (OUT / 'timing_remote.json').write_text(json.dumps(remote.log, indent=2))
                remote.sock.close()
            app.terminate()
            try:
                app.wait(timeout=5)
            except subprocess.TimeoutExpired:
                app.kill()
                app.wait()


if __name__ == '__main__':
    main()
