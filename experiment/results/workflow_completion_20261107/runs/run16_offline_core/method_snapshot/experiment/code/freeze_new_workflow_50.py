"""Freeze the unbiased 50-case manifest for the new-workflow quick pilot.

This script is deterministic and performs no model or network calls.
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

from app.services.configuration_policy import VERSION, resolve_configuration
from run_contract_development import INTERPRETATIONS


SEED = 42
SOURCE = ROOT / "experiment/formal/manifest.json"
TARGET = ROOT / "experiment/formal/new_workflow_50_seed42_manifest.json"


def sha256(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def circuits_in(requirement: str) -> list[dict]:
    decoder = json.JSONDecoder()
    found: list[dict] = []
    for offset, char in enumerate(requirement):
        if char != "{":
            continue
        try:
            value, _ = decoder.raw_decode(requirement[offset:])
        except ValueError:
            continue
        if isinstance(value, dict) and isinstance(value.get("parts"), list) and isinstance(value.get("connections"), list):
            if value not in found:
                found.append(value)
    return found


def proportional_quotas(counts: dict[str, int], total: int) -> dict[str, int]:
    population = sum(counts.values())
    exact = {key: total * value / population for key, value in counts.items()}
    quotas = {key: int(value) for key, value in exact.items()}
    remaining = total - sum(quotas.values())
    order = sorted(counts, key=lambda key: (-(exact[key] - quotas[key]), key))
    for key in order[:remaining]:
        quotas[key] += 1
    return quotas


def main() -> None:
    source = json.loads(SOURCE.read_text(encoding="utf-8"))
    cases = source["cases"]
    assert len(cases) == 296
    assert len({case["id"] for case in cases}) == 296

    by_dataset: dict[str, list[dict]] = defaultdict(list)
    for case in cases:
        by_dataset[case["dataset"]].append(case)
    assert {key: len(value) for key, value in by_dataset.items()} == {
        "internal": 120,
        "EmbedBench": 126,
        "PCEval": 50,
    }

    rng = random.Random(SEED)
    selected: list[dict] = []
    quotas: dict[str, dict[str, int]] = {}

    internal_quotas = {level: 5 for level in ("simple", "medium", "complex", "advanced")}
    quotas["internal"] = internal_quotas
    for level in ("simple", "medium", "complex", "advanced"):
        pool = sorted((case for case in by_dataset["internal"] if case["stratum"] == level), key=lambda case: case["id"])
        assert len(pool) == 30
        selected.extend(rng.sample(pool, internal_quotas[level]))

    embed_counts = dict(Counter(case["stratum"] for case in by_dataset["EmbedBench"]))
    assert embed_counts == {"Mega": 126}
    quotas["EmbedBench"] = {"Mega": 20}
    selected.extend(rng.sample(sorted(by_dataset["EmbedBench"], key=lambda case: case["id"]), 20))

    pceval_counts = dict(Counter(case["stratum"] for case in by_dataset["PCEval"]))
    pceval_quotas = proportional_quotas(pceval_counts, 10)
    quotas["PCEval"] = pceval_quotas
    for level in sorted(pceval_counts):
        pool = sorted((case for case in by_dataset["PCEval"] if case["stratum"] == level), key=lambda case: case["id"])
        selected.extend(rng.sample(pool, pceval_quotas[level]))

    assert len(selected) == 50
    assert len({case["id"] for case in selected}) == 50
    assert Counter(case["dataset"] for case in selected) == Counter(internal=20, EmbedBench=20, PCEval=10)
    assert Counter(case["stratum"] for case in selected if case["dataset"] == "internal") == Counter(internal_quotas)
    assert Counter(case["stratum"] for case in selected if case["dataset"] == "PCEval") == Counter(pceval_quotas)

    frozen = []
    for position, original in enumerate(selected, 1):
        source_path = ROOT / original["source"]
        assert source_path.is_file()
        assert sha256(source_path) == original["source_sha256"]
        public_interpretation = INTERPRETATIONS.get(original["id"], "")
        configured_input = original["requirement"] + ("\n" + public_interpretation if public_interpretation else "")
        resolution = resolve_configuration(configured_input, {}, board=original["fqbn"])
        circuits = circuits_in(original["requirement"])
        item = copy.deepcopy(original)
        item.update(
            selection_position=position,
            case_id=original["id"],
            source_name=original["dataset"],
            difficulty=original["stratum"],
            original_requirement=original["requirement"],
            effective_requirement=resolution["effective_requirement"],
            target_board=original["fqbn"],
            original_circuit=(circuits[0] if len(circuits) == 1 else circuits),
            input_file=original["source"],
            input_file_sha256=sha256(source_path),
            public_interpretation=public_interpretation,
            configuration_resolution=resolution,
        )
        frozen.append(item)

    manifest = {
        "experiment": "new_workflow_50_quick_pilot",
        "created_utc": datetime.now(timezone.utc).isoformat(),
        "selection_seed": SEED,
        "selection_algorithm": "Python random.Random(42); ID-sorted pools; random.sample without replacement; sequential group order internal(simple,medium,complex,advanced), EmbedBench, PCEval(sorted strata); PCEval quotas use Hamilton largest remainder with stratum-name tie break.",
        "selection_inputs_only": ["case_id", "dataset/source", "stratum/difficulty"],
        "source_manifest": str(SOURCE.relative_to(ROOT)),
        "source_manifest_sha256": sha256(SOURCE),
        "source_population": {key: len(value) for key, value in by_dataset.items()},
        "source_strata": {key: dict(sorted(Counter(case["stratum"] for case in value).items())) for key, value in by_dataset.items()},
        "quotas": quotas,
        "configuration_policy": VERSION,
        "planned_scope": 50,
        "repetitions": 1,
        "arms": ["new_workflow_semantic_review"],
        "cases": frozen,
    }
    encoded = json.dumps(manifest, ensure_ascii=False, indent=2) + "\n"
    if TARGET.exists() and TARGET.read_text(encoding="utf-8") != encoded:
        raise RuntimeError(f"Refusing to overwrite a different frozen manifest: {TARGET}")
    TARGET.write_text(encoded, encoding="utf-8")
    print(json.dumps({
        "manifest": str(TARGET.relative_to(ROOT)),
        "manifest_sha256": sha256(TARGET),
        "total": len(frozen),
        "unique_case_ids": len({case["case_id"] for case in frozen}),
        "datasets": dict(Counter(case["source_name"] for case in frozen)),
        "internal_strata": dict(Counter(case["difficulty"] for case in frozen if case["source_name"] == "internal")),
        "pceval_strata": dict(Counter(case["difficulty"] for case in frozen if case["source_name"] == "PCEval")),
        "embedbench_strata": dict(Counter(case["difficulty"] for case in frozen if case["source_name"] == "EmbedBench")),
    }, ensure_ascii=False, indent=2))


if __name__ == "__main__":
    main()
