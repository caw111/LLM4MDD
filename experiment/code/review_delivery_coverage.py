"""Apply the frozen comparison rubric to the 296 delivered sketches."""
import argparse
import collections
import hashlib
import statistics

import review_requirement_implementation as review


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--workers', type=int, default=32)
    args = parser.parse_args()
    earlier = review.BASE / 'semantic_review'
    out = review.BASE / 'delivery_requirement_coverage'
    out.mkdir(parents=True, exist_ok=True)
    cache = earlier / 'cache'
    delivery = review.BASE.parent / 'picsimlab_verification_20261107'
    rows = review.load(delivery / 'final_all296_cases_after_34_review.json')
    bundle = review.load(review.BASE / 'input_bundle.json')
    frozen = review.load(earlier / 'audited_checklists.json')
    old_summary = review.load(earlier / 'summary.json')
    checklists = frozen['checklists']
    tasks = {case: review.task_data(p) for case, p in bundle['profiles'].items()}
    assert len(rows) == len(checklists) == 296
    assert {row['case'] for row in rows} == set(checklists)
    assert sum(row['basic_core_status'] == 'scenarios_passed' for row in rows) == 283
    assert all(row['compile_ok'] for row in rows)
    assert review.digest(tasks) == frozen['task_input_sha256']
    assert review.digest(frozen) == old_summary['checklist_sha256']
    assert review.digest(review.REVIEW) == old_summary['review_rubric_sha256']
    assert review.digest(review.PATH_REVIEW) == old_summary['path_review_rubric_sha256']
    prior = review.load(earlier / 'source_reviews.json')
    by_hash = {(x['meta']['original_case'], x['meta']['source_sha256']): k for k, x in bundle['sources'].items()}
    reused, sources = {}, {}
    for row in rows:
        case, sha = row['case'], row['source_sha256']
        raw = (delivery / 'cases' / case / 'source.ino').read_bytes()
        assert hashlib.sha256(raw).hexdigest() == sha, case
        previous_key = by_hash.get((case, sha))
        if previous_key in prior:
            reused[case] = {**prior[previous_key], 'reused_execution_key': previous_key}
        else:
            sources[case] = raw.decode('utf-8')
    key = review.credential()
    review.verify_evidence_matching()
    controls = {**review.calibrate(cache, key), **review.calibrate_timing(cache, key)}
    assert all(c['actual'] == c['expected'] for c in controls.values())
    primary, errors = review.parallel({c: lambda c=c, source=s: review.audit_job(
        cache, key, tasks[c], checklists[c]['items'], source) for c, s in sources.items()}, args.workers, 'delivery-primary')
    review.save(out / 'primary_errors.json', errors)
    if errors:
        raise SystemExit('Delivery primary review incomplete. Completed requests are cached.')
    review.save(out / 'source_reviews.primary.json', primary)
    jobs = {}
    for c, s in sources.items():
        decided = {i['id'] for i in primary[c]['items'] if i['status'] != 'U'}
        items = [i for i in checklists[c]['items'] if i['id'] in decided]
        if items:
            jobs[c] = lambda c=c, s=s, items=items: review.audit_job(cache, key, tasks[c], items, s, review.PATH_REVIEW)
    verification, errors = review.parallel(jobs, args.workers, 'delivery-path-review')
    review.save(out / 'verification_errors.json', errors)
    if errors:
        raise SystemExit('Delivery path review incomplete. Completed requests are cached.')
    review.save(out / 'source_reviews.verification.json', verification)
    fresh, conflicts = review.combine_reviews(primary, verification)
    combined = {**reused, **fresh}
    # The shared task-level ambiguity applies regardless of the candidate source.
    if 'embedbench_1' in fresh:
        for i in fresh['embedbench_1']['items']:
            if i['id'] == 'R005':
                i.update(status='U', reason='Shared adjudication: the required interval is ambiguous between cycle restart and next LED activation.', evidence=[], uncertainty='shared reference ambiguity')
    result = []
    for row in rows:
        case = row['case']
        items = checklists[case]['items']
        checked = combined[case]['items']
        assert [i['id'] for i in checked] == [i['id'] for i in items], case
        counts = collections.Counter(i['status'] for i in checked)
        assert len(items) == counts['P'] + counts['F'] + counts['U']
        result.append({'case':case, 'dataset':row['dataset'], 'source_sha256':row['source_sha256'],
                       'core_passed':row['basic_core_status'] == 'scenarios_passed', 'N':len(items),
                       **{status:counts[status] for status in ('P','F','U')}})
    metrics = []
    for dataset in ('all', 'internal', 'PCEval', 'EmbedBench'):
        group = [r for r in result if dataset == 'all' or r['dataset'] == dataset]
        p = statistics.mean(r['P'] / r['N'] for r in group)
        metrics.append({'dataset':dataset, 'tasks':len(group), 'core_passed':sum(r['core_passed'] for r in group),
                        'confirmed_coverage_percent':100*p, 'not_passed_percent':100*(1-p),
                        **{s:sum(r[s] for r in group) for s in ('N','P','F','U')}})
    summary = {'method':'LLM4MDD delivery snapshot (283/296 PICSimLab core passes)',
               'reused_identical_sources':len(reused), 'new_sources_reviewed':len(fresh),
               'new_review_disagreements':conflicts, 'checklist_sha256':review.digest(frozen),
               'model':review.MODEL, 'provider':review.PROVIDER, 'summaries':metrics}
    review.save(out / 'source_reviews.json', combined)
    review.save(out / 'project_verdicts.json', result)
    review.save(out / 'summary.json', summary)
    print(summary, flush=True)


if __name__ == '__main__':
    main()
