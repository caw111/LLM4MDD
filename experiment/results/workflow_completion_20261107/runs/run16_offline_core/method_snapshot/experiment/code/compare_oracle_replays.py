"""Compare complete archived generations rescored by the same independent oracle.

Preserves raw scores and audits both artifact chains before comparing measurements.
"""
import argparse
import json
from pathlib import Path

from summarize_oracle_replay import combine, read, sha, ROOT


def compare(before, after):
    replays = [read(folder/'report.json') for folder in (before, after)]
    originals = [ROOT/report['original'] for report in replays]
    executions = [read(folder/'execution.json') for folder in originals]
    if replays[0]['oracle_source_hashes'] != replays[1]['oracle_source_hashes']:
        raise ValueError('Both runs must be replayed using identical oracle sources')
    if replays[0]['runtime'] != replays[1]['runtime']:
        raise ValueError('Replay tool identities differ')
    for key in ('cases', 'repetitions', 'public_interpretations'):
        if executions[0][key] != executions[1][key]:
            raise ValueError('Generation task or acceptance identity differs: '+key)
    combined = [combine(original, replay) for original, replay in zip(originals, (before, after))]
    return dict(
        before=combined[0], after=combined[1],
        common_oracle_source_hashes=replays[0]['oracle_source_hashes'],
        common_runtime=replays[0]['runtime'],
        original_scores_retained=True,
        new_llm_calls=sum(report['new_llm_calls'] for report in replays),
        scope='Separate stochastic generation runs on identical development tasks, each rescored by the same oracle. '
              'Before/after differences are descriptive, not a causal ablation. Within-run paired intervals are retained. '
              'Host output traces do not prove physical smoothness or whole-corpus non-inferiority.',
        comparison_script_sha256=sha(Path(__file__)),
    )


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    for name in ('before', 'after', 'out'):
        parser.add_argument('--'+name, required=True)
    args = parser.parse_args()
    result = compare(ROOT/args.before, ROOT/args.after)
    with (ROOT/args.out).open('x', encoding='utf-8') as stream:
        json.dump(result, stream, ensure_ascii=False, indent=2)
    print(json.dumps({name: result[name]['arms'] for name in ('before', 'after')}, ensure_ascii=False))
