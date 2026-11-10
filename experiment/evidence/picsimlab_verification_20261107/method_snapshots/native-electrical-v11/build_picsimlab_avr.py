"""Build the PICSimLab 0.9.3 NOGUI AVR boards for controlled batch fixtures.

Sources and dependencies live in the user's Linux cache, never in firmware.
"""
import hashlib
import json
import os
from pathlib import Path
import subprocess
import urllib.request
import zipfile

ROOT=Path(__file__).resolve().parents[2]
BASE=Path.home()/'.cache/llm4mdd-picsimlab-build'
OUT=ROOT/'experiment/output/picsimlab_verification_20261107'


def run(command,cwd,timeout=1200):
    print('RUN',str(cwd),command,flush=True)
    subprocess.run(command,cwd=cwd,check=True,timeout=timeout)


def main():
    os.nice(5)
    os.sched_setaffinity(0,set(sorted(os.sched_getaffinity(0))[:12]))
    BASE.mkdir(parents=True,exist_ok=True)
    p=BASE/'picsim.zip'
    if not p.exists():
        urllib.request.urlretrieve('https://codeload.github.com/lcgamboa/picsim/zip/refs/heads/master',p)
    if not (BASE/'picsim-master').exists():
        with zipfile.ZipFile(p) as z:z.extractall(BASE)
    provenance={}
    for name in ('picsim.zip','simavr.zip'):
        q=BASE/name
        provenance[name]=hashlib.sha256(q.read_bytes()).hexdigest()
    (OUT/'native_build_source_hashes.json').write_text(json.dumps(provenance,indent=2))
    picsim=BASE/'picsim-master'
    avr=BASE/'simavr-master'
    lx=BASE/'lxrad_NOGUI-main'
    for folder,target in ((picsim/'include/picsim',Path('..')/'include'),
                          (avr/'simavr/sim/simavr',Path('..')/'sim')):
        if not folder.exists():folder.symlink_to(target,target_is_directory=True)
    deps=BASE/'deps';deps.mkdir(exist_ok=True)
    for name,target in (('picsim',picsim),('simavr',avr),('lxrad_nogui',lx)):
        path=deps/name
        if not path.exists():path.symlink_to(target,target_is_directory=True)
    run(['make','-j8'],picsim)
    run(['make','build-simavr','-j8'],avr)
    run(['make','static','-j8'],lx)
    project=BASE/'picsimlab-0.9.3'
    src=project/'src'
    objects=['ppicsimlab.o']+[f'picsimlab{i}.o' for i in range(1,7)]
    objects += [str(p.relative_to(src).with_suffix('.o')) for p in (src/'lib').glob('*.cc')]
    objects += ['sim_backend/bsim_simavr.o','boards/board_Arduino_Uno.o','boards/board_Arduino_Mega.o']
    for group in ('devices','parts'):
        objects += [str(p.relative_to(src).with_suffix('.o')) for p in (src/group).glob('*.cc')
                    if p.name!='prog_psp.cc' and not p.name.startswith('exp_')]
    libs=f'{picsim}/src/libpicsim.a {lx}/lib/liblxrad_NOGUI.a {avr}/simavr/obj-x86_64-linux-gnu/libsimavr.a -lopenal -lminizip -pthread -ldl -lelf -lcurl -lz'
    makefile=src/'Makefile.NOGUI'
    text=makefile.read_text()
    assets=Path.home()/'.cache/llm4mdd-picsimlab-0.9.3/squashfs-root/usr/share/picsimlab'
    relative_assets='../../../llm4mdd-picsimlab-0.9.3/squashfs-root/usr/share/picsimlab/'
    text=text.replace('../share/picsimlab/',relative_assets).replace(str(assets)+'/',relative_assets)
    if '-D_NOTHREAD' in text:
        text=text.replace('-D_NOTHREAD','')
        # Match the threaded NOGUI release build; make does not track flag changes.
        for file in src.rglob('*.cc'):file.touch()
    makefile.write_text(text)
    run(['make','-f','Makefile.NOGUI','-j8',f'LIBPATH={deps}',f'OBJS={" ".join(objects)}',f'LIBS={libs}'],src)
    print('BUILT',src/'picsimlab_NOGUI',flush=True)


if __name__=='__main__':main()
