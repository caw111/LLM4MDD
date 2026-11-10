"""Read frozen evidence; never run candidates, call models, or change verdicts."""
import collections
import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
OUT = ROOT / 'experiment/analysis/error_origin_audit_20261029'
DATA = ROOT / 'experiment/output'


def read(p, default=None):
    return json.loads(p.read_text(encoding='utf-8')) if p.exists() else default


def digest(p):
    return hashlib.sha256(p.read_bytes()).hexdigest()


def save(p, value):
    p.parent.mkdir(parents=True, exist_ok=True)
    p.write_text(json.dumps(value, ensure_ascii=False, indent=2), encoding='utf-8')


def aggregate(rows):
    assert len({(x['group'], x['case']) for x in rows}) == len(rows)
    result = {}
    for group in sorted({x['group'] for x in rows}):
        rr = [x for x in rows if x['group'] == group]
        result[group] = dict(total=len(rr), compiled=sum(x['compile_ok'] for x in rr),
            behavior=dict(collections.Counter(x['behavior_status'] for x in rr)),
            legacy=dict(collections.Counter(x['legacy_status'] for x in rr)),
            generation_failure_stages=dict(collections.Counter(
                x.get('failure_stage') or x.get('compile_status') or 'unresolved'
                for x in rr if not x['compile_ok'])),
            datasets={ds: dict(total=sum(x['dataset'] == ds for x in rr),
                behavior=dict(collections.Counter(x['behavior_status'] for x in rr if x['dataset'] == ds)))
                for ds in ['internal', 'PCEval', 'EmbedBench']})
    return result


def main():
    inputs = {name: DATA / path for name, path in {
        'manifest': 'all_groups_host_smoke_20261023/frozen_manifest.json',
        'behavior': 'behavior_all296_20261023/mapped_results.json',
        'legacy': 'behavior_all296_20261023/legacy16/mapped_results.json',
        'static': 'code_review_all_20261023/results.json',
        'requirements': 'llm_review_handoff_20261023/original_requirements.json',
    }.items()}
    hashes = {str(p): digest(p) for p in inputs.values()}
    manifest = read(inputs['manifest'])
    behavior = {(x['group'], x['case']): x for x in read(inputs['behavior'])}
    legacy = {(x['group'], x['case']): x for x in read(inputs['legacy'])}
    static = {(x['group'], x['case']): x for x in read(inputs['static'])}
    requirements = {x['id']: x for x in read(inputs['requirements'])}
    rows, dossiers, source_hashes = [], {}, {}
    cache = {}
    for frozen in manifest:
        key = frozen['group'], frozen['case']
        b, leg, rev = behavior.get(key, {}), legacy.get(key, {}), static.get(key, {})
        row = dict(frozen, behavior_status=b.get('behavior_status', 'missing'),
            legacy_status=leg.get('status', 'not_in_legacy16'),
            review_status=rev.get('review_status', 'not_in_this_static_review'),
            attribution='not_yet_localized', evidence=b.get('evidence'),
            legacy_evidence=leg.get('evidence'), static_issues=rev.get('issues', []))
        if frozen.get('source'):
            source = Path(frozen['source'])
            actual = digest(source)
            assert actual == frozen['source_sha256'], str(source)
            source_hashes[str(source)] = actual
            if str(source) not in cache:
                base = next((q for q in source.parents if (q / 'contract.json').exists()), None)
                artifacts = {}
                if base:
                    for name in ['contract.json', 'model.json', 'model_validation.json', 'contract_planning.json']:
                        p = base / name
                        if p.exists():
                            artifacts[name] = dict(path=str(p), sha256=digest(p), content=read(p))
                cache[str(source)] = dict(source=str(source), source_sha256=actual,
                    code=source.read_text(encoding='utf-8'), artifacts=artifacts)
            row['upstream_artifacts'] = {n: dict(path=a['path'], sha256=a['sha256'])
                for n, a in cache[str(source)]['artifacts'].items()}
        # Failure-stage metadata belongs to this experimental arm, never another method.
        roots = []
        g = frozen['group']
        arms = {'historical_direct': 'direct', 'historical_direct_feedback': 'direct_feedback',
                'historical_workflow_initial': 'workflow', 'workflow_267': 'workflow'}
        if g in arms and not frozen['compile_ok']:
            roots.append((DATA / 'configured_audit_v1_20261015' / frozen['case'] / 'repeat_1', arms[g]))
        if g.startswith('ablation_'):
            roots.append((DATA / 'paper_ablation_v2_20261017/jobs' / frozen['case'] / 'repeat_1',
                          g.removeprefix('ablation_')))
        for base, arm in roots:
            p = base / 'results.json'
            values = read(p, [])
            values = values if isinstance(values, list) else values.get('results', [])
            hit = next((v for v in values if v.get('arm') == arm), None)
            if hit and hit.get('compile_ok') == frozen['compile_ok']:
                row.update(failure_stage=hit.get('failure_stage'), compile_status=hit.get('compile_status'),
                           failure_error=hit.get('error'), stage_metadata=str(p))
                if not frozen.get('source'):
                    row['upstream_artifacts'] = {n: dict(path=str(base / n), sha256=digest(base / n))
                        for n in ['contract_planning.json', 'model_validation.json'] if (base / n).exists()}
                break
        if row['behavior_status'] == 'failed' or row['legacy_status'] == 'failed' or row['static_issues']:
            ident = frozen.get('source_sha256') or ':'.join(key)
            dossier_key = frozen['case'] + '__' + ident[:20].replace(':', '_')
            row['dossier'] = str(OUT / 'dossiers' / (dossier_key + '.json'))
            if dossier_key not in dossiers:
                src = cache.get(frozen.get('source'), {})
                dossiers[dossier_key] = dict(case=frozen['case'], dataset=frozen['dataset'],
                    original_requirement=requirements[frozen['case']]['requirement'],
                    **src, observations=[])
            ev = read(Path(b['evidence']) / 'result.json', {}) if b.get('evidence') else {}
            dossiers[dossier_key]['observations'].append(dict(group=g,
                behavior_status=row['behavior_status'], legacy_status=row['legacy_status'],
                evidence=row['evidence'], legacy_evidence=row['legacy_evidence'],
                failed_checks=[z for z in ev.get('checks', []) if z.get('passed') is False],
                static_issues=row['static_issues']))
        rows.append(row)
    assert len(rows) == 3848 and len(behavior) == 3848
    assert all(digest(Path(p)) == h for p, h in hashes.items())
    for name, dossier in dossiers.items():
        save(OUT / 'dossiers' / (name + '.json'), dossier)
    save(OUT / 'inventory.json', rows)
    save(OUT / 'summary.json', dict(groups=aggregate(rows), input_hashes=hashes,
        source_files_checked=len(source_hashes), source_sha256=source_hashes,
        dossier_count=len(dossiers), scope='All 13 frozen arms; no new generation or execution',
        attribution_policy='Static labels are clues, not runtime proof; absent localization stays unresolved.'))
    print(json.dumps(dict(rows=len(rows), dossiers=len(dossiers), sources=len(source_hashes)), ensure_ascii=False))


if __name__ == '__main__':
    main()
