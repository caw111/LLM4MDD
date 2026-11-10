"""Infrastructure-only continuation of the frozen development method."""
import json
from datetime import datetime, timezone
import formal_resume as recovery
import run_development_repair as development
out = recovery.runner.ROOT / 'experiment/output/development_repair_v2_20261008'
# Keep new memoized reference results out of the historical formal experiment.
recovery.cache_root = out / 'reference_validation_cache'
original_same = recovery.same_diagnostics

def same_diagnostics(left, right):
    suffix = '\nTargeted repair guidance:\n'
    if suffix in left and suffix in right:
        a, a_guidance = left.rsplit(suffix, 1)
        b, b_guidance = right.rsplit(suffix, 1)
        return a_guidance == b_guidance and original_same(a, b)
    return original_same(left, right)

recovery.same_diagnostics = same_diagnostics
if __name__ == '__main__':
    marker = recovery.MARKER
    assert same_diagnostics('p'+marker+'[{"message":"a"},{"message":"b"}]\nTargeted repair guidance:\nx',
                            'p'+marker+'[{"message":"b"},{"message":"a"}]\nTargeted repair guidance:\nx')
    assert not same_diagnostics('p'+marker+'[{"message":"a"}]\nTargeted repair guidance:\nx',
                                'p'+marker+'[{"message":"b"}]\nTargeted repair guidance:\nx')
    recovery.runner.save(out/'transport_recovery.json', {
        'created_utc': datetime.now(timezone.utc).isoformat(),
        'policy': 'Reuse exact successful calls. Retry only transport/service transient errors up to twice. Preserve model/prompt/repair budgets; do not retry permission errors.',
        'recovery_sha256': recovery.runner.digest(recovery.runner.ROOT/'experiment/code/formal_resume.py'),
        'wrapper_sha256': recovery.runner.digest(recovery.runner.ROOT/'experiment/code/resume_development_repair.py')})
    development.main()
