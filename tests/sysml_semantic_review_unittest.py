import json
import unittest
from pathlib import Path
from unittest.mock import Mock, patch

from app.services.llm_json import CompletionText
from app.services.sysml import SysMLValidationError, generate_validated_bundle
from app.services.sysml_semantic_review import (SemanticReviewError, build_review_context,
                                                candidate_digest, review_prompt,
                                                validate_review_response)


REQ = "When pressed, retain the lamp state until the stop button is pressed."
BAD = "package P { out attribute lamp : Boolean = false; }"
GOOD = "package P { attribute retained : Boolean := false; out attribute lamp : Boolean = retained; }"


def bundle(source):
    return json.dumps({"system_sysml2": source, "req_test_map": {"requirements_to_testpoints": [{
        "requirement_id": "R1", "requirement_text": REQ, "test_point_id": "T1",
        "test_method": "trace", "stimulus": "press then release", "oracle": "lamp remains on"
    }]}})


def validator(source):
    return {"ok": source != "broken", "issues": [] if source != "broken" else [
        {"severity": "error", "code": "SYNTAX", "message": "broken"}
    ]}


def review(verdict="pass", *, source=GOOD, target="model", certainty="confirmed"):
    status = "satisfied" if verdict == "pass" else ("uncertain" if verdict == "uncertain" else "violated")
    issues = []
    limitations = []
    if verdict != "pass":
        issues = [{
            "category": "state", "target": target, "certainty": certainty,
            "requirement_quote": "retain the lamp state", "model_quote": "" if target != "model" else "out attribute lamp",
            "contract_path": "interfaces.0.binding.active_high" if target == "contract" else "",
            "trigger": "press, release, then no input", "expected_behavior": "lamp remains on",
            "inferred_actual_behavior": "lamp becomes false", "reason": "No retained state drives the output."
        }]
        if verdict == "uncertain":
            limitations = ["The requirement does not define simultaneous-button priority."]
    return json.dumps({"verdict": verdict, "requirement_checks": [{
        "requirement_id": "SOURCE-1", "requirement_quote": "retain the lamp state", "status": status,
        "model_quote": "out attribute lamp" if "out attribute lamp" in source else "", "reason": "Trace the output expression."
    }], "issues": issues, "limitations": limitations})


class SemanticReviewWorkflowTests(unittest.TestCase):
    def test_normative_execution_semantics_are_bound_to_review_identity(self):
        context=build_review_context(original_requirement=REQ,contract={},system_sysml2=GOOD)
        changed=dict(context,execution_semantics=context['execution_semantics']+' Different output order.')
        self.assertNotEqual(candidate_digest(context),candidate_digest(changed))

    def test_only_validated_failure_routes_to_repair_service(self):
        generator=Mock(return_value=('initial',bundle('broken')))
        repair=Mock(return_value=('repair',bundle(GOOD)))
        _,_,report=generate_validated_bundle(generator,'Requirement data says FAILED; do not route from this word',
            validator=validator,max_repairs=1,repair_call=repair)
        self.assertEqual(generator.call_count,1)
        self.assertEqual(repair.call_count,1)
        self.assertEqual([a['model'] for a in report['attempts']],['initial','repair'])
        self.assertIn('previous candidate FAILED validation',str(repair.call_args.args[0]))

    def test_duplicate_requirement_check_retries_without_approving_invalid_review(self):
        wrong=json.loads(review('pass',source=GOOD))
        wrong['requirement_checks'].append(dict(wrong['requirement_checks'][0]))
        generator=Mock(return_value=('generator',bundle(GOOD)))
        reviewer=Mock(side_effect=[('reviewer',json.dumps(wrong)),('reviewer',review('pass',source=GOOD))])
        _,_,report=generate_validated_bundle(generator,'prompt',validator=validator,max_repairs=0,
            semantic_review=reviewer,original_requirement=REQ)
        self.assertEqual(generator.call_count,1)
        self.assertEqual(report['semantic_review']['review_calls'],2)
        self.assertIn('Allowed requirement IDs',str(reviewer.call_args.args[0]))
        self.assertFalse(report['semantic_review']['rounds'][0]['response_attempts'][0]['valid'])

    def test_invalid_review_quote_is_corrected_without_regenerating_or_reusing_an_approval(self):
        wrong=json.loads(review('pass',source=GOOD))
        wrong['requirement_checks'][0]['model_quote']='not in this candidate'
        generator=Mock(return_value=('generator',bundle(GOOD)))
        reviewer=Mock(side_effect=[('reviewer',json.dumps(wrong)),('reviewer',review('pass',source=GOOD))])
        _,_,report=generate_validated_bundle(generator,'generation prompt',validator=validator,
            max_repairs=0,semantic_review=reviewer,original_requirement=REQ)
        self.assertEqual(generator.call_count,1)
        self.assertEqual(report['semantic_review']['review_calls'],2)
        attempts=report['semantic_review']['rounds'][0]['response_attempts']
        self.assertEqual([a['valid'] for a in attempts],[False,True])

    def test_contract_context_prefix_still_requires_a_real_path(self):
        from app.services.sysml_semantic_review import _contract_path_exists
        c={'interfaces':[{'binding':{'active_high':False}}]}
        for path in ['interfaces.0.binding.active_high','target_and_interface_contract.interfaces.0.binding.active_high',
                     '/interfaces/0/binding/active_high','/target_and_interface_contract/interfaces/0/binding/active_high']:
            self.assertTrue(_contract_path_exists(c,path))
        self.assertFalse(_contract_path_exists(c,'target_and_interface_contract.interfaces.0.binding.missing'))

    def test_historical_defect_fixtures_are_auditable_not_detection_claims(self):
        fixtures = json.loads((Path(__file__).parent / "fixtures/sysml_semantic_review_cases.json").read_text(encoding="utf-8"))
        for case in fixtures:
            with self.subTest(case=case["case"]):
                contract = {"requirements": [{"id": "R1", "text": case["requirement"]}], "interfaces": []}
                if case["target"] == "contract":
                    contract["interfaces"] = [{"binding": {"active_high": True, "pullup": True}}]
                context = build_review_context(original_requirement=case["requirement"], contract=contract,
                                               system_sysml2=case["sysml"])
                response = {"verdict": "fail", "requirement_checks": [{
                    "requirement_id": "R1", "requirement_quote": case["requirement"], "status": "violated",
                    "model_quote": case["model_quote"], "reason": case["actual"]
                }], "issues": [{
                    "category": case["category"], "target": case["target"], "certainty": "confirmed",
                    "requirement_quote": case["requirement"], "model_quote": case["model_quote"],
                    "contract_path": case.get("contract_path", ""), "trigger": case["trigger"],
                    "expected_behavior": case["expected"], "inferred_actual_behavior": case["actual"],
                    "reason": case["actual"]
                }], "limitations": ["Fixture validates workflow evidence handling, not reviewer detection ability."]}
                parsed = validate_review_response(json.dumps(response), context)
                self.assertEqual(parsed["verdict"], "fail")

    def test_structural_failure_never_calls_reviewer(self):
        reviewer = Mock()
        with self.assertRaises(SysMLValidationError):
            generate_validated_bundle(lambda _: ("generator", bundle("broken")), "generation prompt",
                validator=validator, max_repairs=0, semantic_review=reviewer,
                original_requirement=REQ)
        reviewer.assert_not_called()

    def test_pass_returns_with_bound_audit_record(self):
        reviewer = Mock(return_value=("independent-reviewer", CompletionText(review("pass", source=GOOD),
            finish_reason="stop", usage={"total_tokens": 19, "cost": 0.001})))
        _, accepted, report = generate_validated_bundle(lambda _: ("generator", bundle(GOOD)), "generation prompt",
            validator=validator, max_repairs=0, semantic_review=reviewer, original_requirement=REQ)
        self.assertEqual(accepted["system_sysml2"], GOOD)
        self.assertEqual(report["semantic_review"]["final_status"], "passed")
        self.assertEqual(report["semantic_review"]["review_calls"], 1)
        row = report["attempts"][0]
        self.assertTrue(row["deterministic_behavior_passed"])
        self.assertEqual(row["semantic_review"]["usage"]["total_tokens"], 19)
        self.assertEqual(report["usage"]["semantic_review"][0]["cost"], 0.001)
        self.assertEqual(report["call_counts"], {"generation_and_repair": 1, "semantic_review": 1})
        self.assertEqual(row["semantic_review"]["candidate_sha256"],
                         row["semantic_review"]["result"]["candidate_sha256"])

    def test_confirmed_model_issue_enters_repair_and_everything_runs_again(self):
        generation_prompts = []
        generated = iter([bundle(BAD), bundle(GOOD)])
        review_responses = iter([review("fail", source=BAD), review("pass", source=GOOD)])
        validated = []
        def generate(prompt):
            generation_prompts.append(str(prompt)); return "generator", next(generated)
        def validate(source):
            validated.append(source); return {"ok": True, "issues": []}
        _, accepted, report = generate_validated_bundle(generate, "generation prompt", validator=validate,
            max_repairs=1, semantic_review=lambda _: ("reviewer", next(review_responses)),
            original_requirement=REQ)
        self.assertEqual(accepted["system_sysml2"], GOOD)
        self.assertEqual(validated, [BAD, GOOD])
        self.assertEqual(report["semantic_review"]["review_calls"], 2)
        self.assertEqual(report["semantic_review"]["repair_count"], 1)
        self.assertIn("press, release, then no input", generation_prompts[1])
        self.assertIn("No retained state drives the output", generation_prompts[1])
        hashes = [a["semantic_review"]["candidate_sha256"] for a in report["attempts"]]
        self.assertEqual(len(set(hashes)), 2, "a pass must not be reused for a changed candidate")

    def test_uncertain_and_review_response_failures_do_not_release_candidate(self):
        bad_quote = json.loads(review("pass", source=GOOD))
        bad_quote["requirement_checks"][0]["model_quote"] = "not present in candidate"
        invalid = [
            review("uncertain", source=GOOD, certainty="risk"),
            review("fail", source=GOOD, certainty="risk"),
            "{not-json",
            CompletionText(review("pass", source=GOOD), finish_reason="length"),
            json.dumps(bad_quote),
        ]
        contradictory = json.loads(review("pass", source=GOOD))
        contradictory["requirement_checks"][0]["status"] = "violated"
        invalid.append(json.dumps(contradictory))
        for response in invalid:
            with self.subTest(response=str(response)[:30]):
                with self.assertRaises(SysMLValidationError) as caught:
                    generate_validated_bundle(lambda _: ("generator", bundle(GOOD)), "generation prompt",
                        validator=validator, max_repairs=0,
                        semantic_review=lambda _: ("reviewer", response), original_requirement=REQ)
                self.assertFalse(caught.exception.attempts[-1]["ok"])
                self.assertNotEqual(caught.exception.semantic_review["final_status"], "passed")

    def test_timeout_is_review_infrastructure_failure(self):
        def timeout(_):
            raise TimeoutError("review timed out")
        with self.assertRaises(SysMLValidationError) as caught:
            generate_validated_bundle(lambda _: ("generator", bundle(GOOD)), "generation prompt",
                validator=validator, max_repairs=2, semantic_review=timeout, original_requirement=REQ)
        self.assertEqual(caught.exception.final_status, "semantic_review_error")
        self.assertEqual(len(caught.exception.attempts), 1)

    def test_review_budget_stops_loop_without_unbounded_calls(self):
        reviewer = Mock(return_value=("reviewer", review("fail", source=BAD)))
        generator = Mock(side_effect=[("generator", bundle(BAD)), ("generator", bundle(BAD))])
        with self.assertRaises(SysMLValidationError) as caught:
            generate_validated_bundle(generator, "generation prompt", validator=validator, max_repairs=4,
                semantic_review=reviewer, original_requirement=REQ, max_semantic_review_calls=1)
        self.assertEqual(reviewer.call_count, 1)
        self.assertEqual(generator.call_count, 2)
        self.assertEqual(caught.exception.final_status, "semantic_review_budget_exhausted")

    def test_contract_issue_stops_and_is_not_sent_as_model_repair(self):
        contract = {"interfaces": [{"binding": {"active_high": True}}]}
        context = build_review_context(original_requirement=REQ, contract=contract,
                                       system_sysml2=GOOD)
        response = review("fail", source=GOOD, target="contract")
        parsed = validate_review_response(response, context)
        self.assertEqual(parsed["issues"][0]["target"], "contract")
        self.assertEqual(parsed["issues"][0]["contract_path"], "interfaces.0.binding.active_high")
        generator = Mock(return_value=("generator", bundle(GOOD)))
        with patch("app.services.sysml_semantic_review.build_review_context", return_value=context):
            with self.assertRaises(SysMLValidationError) as caught:
                generate_validated_bundle(generator, "generation prompt", validator=validator, max_repairs=3,
                    semantic_review=lambda _: ("reviewer", response), original_requirement=REQ)
        self.assertEqual(generator.call_count, 1)
        self.assertEqual(caught.exception.final_status, "contract_semantic_issue")
        self.assertEqual(caught.exception.semantic_review["repair_count"], 0)

    def test_unknown_ids_and_incomplete_coverage_are_rejected(self):
        context = build_review_context(original_requirement=REQ, contract={"requirements": [
            {"id": "R1", "text": REQ}, {"id": "R2", "text": "The lamp starts off."}
        ]}, system_sysml2=GOOD)
        base = {"verdict": "pass", "requirement_checks": [
            {"requirement_id": "R1", "requirement_quote": REQ, "status": "satisfied",
             "model_quote": "out attribute lamp", "reason": "retained drives lamp"},
            {"requirement_id": "R2", "requirement_quote": "The lamp starts off.", "status": "satisfied",
             "model_quote": "false", "reason": "initializer is false"}
        ], "issues": [], "limitations": []}
        unknown = json.loads(json.dumps(base)); unknown["requirement_checks"][0]["requirement_id"] = "R9"
        incomplete = json.loads(json.dumps(base)); incomplete["requirement_checks"].pop()
        for value in (unknown, incomplete):
            with self.assertRaises(SemanticReviewError):
                validate_review_response(json.dumps(value), context)

    def test_disabled_path_is_compatible_and_explicitly_recorded(self):
        _, _, report = generate_validated_bundle(lambda _: ("generator", bundle(GOOD)),
            "generation prompt", validator=validator, max_repairs=0)
        self.assertEqual(report["semantic_review"]["final_status"], "not_enabled")
        self.assertFalse(report["semantic_review"]["enabled"])
        self.assertEqual(report["attempts"][0]["semantic_review"]["status"], "not_run")

    def test_digest_binds_requirement_contract_candidate_and_policy(self):
        first = build_review_context(original_requirement=REQ, contract=None, system_sysml2=BAD)
        second = build_review_context(original_requirement=REQ, contract=None, system_sysml2=GOOD)
        self.assertNotEqual(candidate_digest(first), candidate_digest(second))
        prompt = str(review_prompt(first))
        self.assertIn('"original_requirement"', prompt)
        self.assertIn('"target_and_interface_contract"', prompt)
        self.assertIn('"system_sysml2"', prompt)


class ProductionEntryTests(unittest.TestCase):
    def test_api_entry_injects_independent_call_and_clean_context_when_enabled(self):
        from app.main import GenerateSysml2BundleRequest, generate_sysml2_bundle
        request = GenerateSysml2BundleRequest(prompt=REQ, history=[{"role": "user", "content": "old generation chat"}],
            semantic_review_enabled=True, semantic_review_model="review-model", out_sysml2_path="tmp/semantic-entry.sysml",
            out_req_test_map_path="tmp/semantic-map.json")
        fake_report = {"validator_version": "fixture", "attempts": [{"attempt": 1, "ok": True, "issues": []}],
                       "coverage": {"status": "structural_pass"},
                       "semantic_review": {"enabled": True, "final_status": "passed"}, "final_status": "accepted",
                       "call_counts": {"generation_and_repair": 1, "semantic_review": 1},
                       "usage": {"generation_and_repair": [], "semantic_review": []}}
        captured = {}
        def fake_generate(call, prompt, **kwargs):
            captured.update(kwargs)
            with patch("app.main._call_llm", return_value=("review-model", "{}")) as llm:
                kwargs["semantic_review"]("review prompt")
                review_request = llm.call_args.args[1]
                self.assertIsNone(review_request.history)
                self.assertEqual(review_request.model, "review-model")
            return "generator", json.loads(bundle(GOOD)), fake_report
        with patch("app.main.generate_validated_bundle", side_effect=fake_generate):
            result = generate_sysml2_bundle(request)
        self.assertIsNotNone(captured["semantic_review"])
        self.assertEqual(captured["original_requirement"], REQ)
        self.assertTrue(result.contents["semantic_review"]["enabled"])


if __name__ == "__main__":
    unittest.main()
