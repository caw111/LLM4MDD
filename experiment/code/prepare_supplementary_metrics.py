"""Read frozen evidence on the Windows side, avoiding WSL per-file overhead."""
import hashlib
import json
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
BASE = ROOT / 'experiment/output/picsimlab_comparison_20261108'
OUT = ROOT / 'experiment/output/additional_metrics_20261109'


def load(path):
    return json.loads(path.read_text(encoding='utf-8-sig'))


def main():
    manifest = load(BASE/'manifest.json')
    groups = [g for g in manifest['groups'] if g not in ('historical_direct', 'historical_direct_feedback')]
    records = [r for r in load(BASE/'all_case_results.json') if r['group'] in groups]
    bykey = {m['id']: m for m in manifest['cases']}
    keys = sorted({r['execution_key'] for r in records if r.get('execution_key')})
    sources = {}
    for i, key in enumerate(keys):
        directory = BASE/'cases'/key
        raw = (directory/'source.ino').read_bytes()
        digest = hashlib.sha256(raw).hexdigest()
        assert digest == bykey[key]['source_sha256'], (key, 'source hash mismatch')
        log = (directory/'compile.log').read_text(encoding='utf-8', errors='replace')
        flash = re.search(r'Sketch uses (\d+) bytes[\s\S]*?Maximum is (\d+) bytes', log)
        sram = re.search(r'Global variables use (\d+) bytes[\s\S]*?Maximum is (\d+) bytes', log)
        memory = None
        if flash and sram:
            f, fc, s, sc = int(flash[1]), int(flash[2]), int(sram[1]), int(sram[2])
            memory = {'flash_bytes': f, 'flash_capacity': fc, 'flash_percent': 100*f/fc,
                      'sram_static_bytes': s, 'sram_capacity': sc, 'sram_static_percent': 100*s/sc}
        sources[key] = {'source': raw.decode('utf-8'), 'meta': bykey[key],
                        'compilation': load(directory/'compilation.json'), 'memory': memory}
        if (i+1) % 200 == 0:
            print(json.dumps({'prepared_unique': i+1}), flush=True)
    result = {'groups': groups, 'records': records, 'profiles': load(BASE/'test_profiles.json'), 'sources': sources}
    OUT.mkdir(parents=True, exist_ok=True)
    (OUT/'input_bundle.json').write_text(json.dumps(result, ensure_ascii=False), encoding='utf-8')
    print(json.dumps({'prepared': True, 'records': len(records), 'unique': len(sources)}), flush=True)


if __name__ == '__main__':
    main()
