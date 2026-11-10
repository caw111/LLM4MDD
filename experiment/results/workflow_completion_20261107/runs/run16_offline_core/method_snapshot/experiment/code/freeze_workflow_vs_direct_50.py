"""Freeze the revised 25-internal/25-EmbedBench two-arm pilot manifest.

Selection uses only source, stratum and case ID.  No model/network calls or
historical outcomes are consulted.
"""
from __future__ import annotations

import copy
import hashlib
import json
import random
import sys
from collections import Counter, defaultdict
from datetime import datetime, timezone
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT))
sys.path.insert(0, str(ROOT / "experiment/code"))

from app.services.configuration_policy import VERSION, resolve_configuration
from freeze_new_workflow_50 import circuits_in
from run_contract_development import INTERPRETATIONS


SEED = 42
SOURCE = ROOT / "experiment/formal/manifest.json"
TARGET = ROOT / "experiment/formal/workflow_vs_direct_50_seed42_manifest.json"


def digest(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main() -> None:
    source = json.loads(SOURCE.read_text(encoding="utf-8"))
    cases = source["cases"]
    assert len(cases) == len({case["id"] for case in cases}) == 296
    by_dataset: dict[str, list[dict]] = defaultdict(list)
    for case in cases:
        by_dataset[case["dataset"]].append(case)

    rng = random.Random(SEED)
    internal_levels = ("simple", "medium", "complex", "advanced")
    # All four internal strata contain 30 cases.  A proportional sample of 25
    # has one tied remainder; choose the extra stratum with the seeded RNG.
    extra_level = rng.choice(sorted(internal_levels))
    internal_quotas = {level: 6 + int(level == extra_level) for level in internal_levels}
    selected: list[dict] = []
    for level in internal_levels:
        pool = sorted((case for case in by_dataset["internal"] if case["stratum"] == level), key=lambda case: case["id"])
        assert len(pool) == 30
        selected.extend(rng.sample(pool, internal_quotas[level]))
    embed_pool = sorted(by_dataset["EmbedBench"], key=lambda case: case["id"])
    assert len(embed_pool) == 126 and {case["stratum"] for case in embed_pool} == {"Mega"}
    selected.extend(rng.sample(embed_pool, 25))

    assert len(selected) == len({case["id"] for case in selected}) == 50
    assert Counter(case["dataset"] for case in selected) == Counter(internal=25, EmbedBench=25)
    assert not any(case["dataset"] == "PCEval" for case in selected)

    frozen = []
    for position, original in enumerate(selected, 1):
        input_path = ROOT / original["source"]
        assert input_path.is_file() and digest(input_path) == original["source_sha256"]
        interpretation = INTERPRETATIONS.get(original["id"], "")
        configured_input = original["requirement"] + ("\n" + interpretation if interpretation else "")
        resolution = resolve_configuration(configured_input, {}, board=original["fqbn"])
        circuits = circuits_in(original["requirement"])
        row = copy.deepcopy(original)
        row.update(
            selection_position=position,
            case_id=original["id"],
            source_name=original["dataset"],
            difficulty=original["stratum"],
            original_requirement=original["requirement"],
            effective_requirement=resolution["effective_requirement"],
            target_board=original["fqbn"],
            original_circuit=(circuits[0] if len(circuits) == 1 else circuits),
            input_file=original["source"],
            input_file_sha256=digest(input_path),
            public_interpretation=interpretation,
            configuration_resolution=resolution,
        )
        frozen.append(row)

    manifest = {
        "experiment": "workflow_vs_direct_50_pilot",
        "created_utc": datetime.now(timezone.utc).isoformat(),
        "selection_seed": SEED,
        "selection_algorithm": (
            "Python random.Random(42); ID-sorted pools; random.sample without replacement. "
            "Internal proportional allocation is 6 per equal-sized stratum plus one seeded tied-remainder stratum; "
            "EmbedBench is sampled as one pool because its manifest has only the Mega stratum."
        ),
        "selection_inputs_only": ["case_id", "dataset/source", "stratum/difficulty"],
        "excluded_datasets": ["PCEval"],
        "source_manifest": str(SOURCE.relative_to(ROOT)),
        "source_manifest_sha256": digest(SOURCE),
        "source_population": {key: len(value) for key, value in sorted(by_dataset.items())},
        "source_strata": {key: dict(sorted(Counter(case["stratum"] for case in value).items())) for key, value in sorted(by_dataset.items())},
        "quotas": {"internal": internal_quotas, "EmbedBench": {"Mega": 25}, "PCEval": {}},
        "configuration_policy": VERSION,
        "planned_scope": 50,
        "repetitions": 1,
        "arms": ["new_workflow_semantic_review", "direct_generation"],
        "cases": frozen,
    }
    encoded = json.dumps(manifest, ensure_ascii=False, indent=2) + "\n"
    if TARGET.exists():
        existing = json.loads(TARGET.read_text(encoding="utf-8"))
        comparable = dict(existing)
        comparable.pop("created_utc", None)
        intended = dict(manifest)
        intended.pop("created_utc", None)
        if comparable != intended:
            raise RuntimeError("Refusing to overwrite a different frozen manifest: " + str(TARGET))
    else:
        TARGET.write_text(encoded, encoding="utf-8")
    print(json.dumps({
        "manifest": str(TARGET.relative_to(ROOT)),
        "manifest_sha256": digest(TARGET),
        "total": len(frozen),
        "unique": len({case["case_id"] for case in frozen}),
        "sources": dict(Counter(case["source_name"] for case in frozen)),
        "internal_strata": dict(Counter(case["difficulty"] for case in frozen if case["source_name"] == "internal")),
        "embedbench_strata": dict(Counter(case["difficulty"] for case in frozen if case["source_name"] == "EmbedBench")),
        "pceval": sum(case["source_name"] == "PCEval" for case in frozen),
    }, ensure_ascii=False, indent=2))


if __name__ == "__main__":
    main()
