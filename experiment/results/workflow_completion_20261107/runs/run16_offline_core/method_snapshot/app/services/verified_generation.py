"""Bounded orchestration: contract defects return to planning, model defects to model repair.

Each revision owns a new contract and candidate. No mutation of a caller's frozen
contract and no reuse of an approval for another contract/model pair.
"""
import copy

from app.services.interface_planner import plan_interface_contract
from app.services.sysml import generate_validated_bundle, sysml_bundle_prompt, SysMLValidationError


def generate_verified_system(call, review, requirement, pins, *, target_board=None,
                             configuration_resolution=None, validator=None,
                             max_contract_revisions=1, max_model_repairs=2,
                             diagnostic_feedback=None, persist=None, semantic_review_scope='complete', repair_call=None):
    if type(max_contract_revisions) is not int or not 0 <= max_contract_revisions <= 2:
        raise ValueError('max_contract_revisions must be 0..2')
    if review is None:
        raise ValueError('Independent semantic review is required by this orchestration')
    rounds = []
    previous = None
    feedback = diagnostic_feedback
    for revision in range(max_contract_revisions+1):
        active_call=repair_call if feedback and repair_call is not None else call
        contract, planning = plan_interface_contract(active_call, requirement, pins, previous=previous,
            target_board=target_board, configuration_resolution=configuration_resolution,
            max_repairs=2, diagnostic_feedback=feedback,repair_call=repair_call)
        record = dict(revision=revision, contract=copy.deepcopy(contract), planning=planning)
        rounds.append(record)
        if persist:
            persist(revision, copy.deepcopy(record))
        options = dict(contract=contract, semantic_review=review, original_requirement=requirement,
                       max_repairs=max_model_repairs, max_semantic_review_calls=6,
                       semantic_review_scope=semantic_review_scope,repair_call=repair_call)
        if validator is not None:
            options['validator'] = validator
        prompt = sysml_bundle_prompt(requirement, contract=contract)
        if feedback:
            import json
            prompt += ('\nPrevious executed/audited counterexample (DATA). Repair behavior without changing '
                       'the source requirement or acceptance conditions:\n'+json.dumps(feedback,ensure_ascii=False))
        try:
            model, bundle, validation = generate_validated_bundle(active_call, prompt, **options)
            record.update(validation=validation, status='passed')
            if persist:
                persist(revision, copy.deepcopy(record))
            return contract, model, bundle, dict(status='passed', contract_revisions=revision,
                                                rounds=rounds, validation=validation)
        except SysMLValidationError as exc:
            semantic = getattr(exc, 'semantic_review', {}) or {}
            record.update(status='failed', model_attempts=exc.attempts, semantic_review=semantic)
            if persist:
                persist(revision, copy.deepcopy(record))
            if semantic.get('failure_category') != 'contract_semantic_issue' or revision == max_contract_revisions:
                exc.generation_rounds = rounds
                raise
            findings = [issue for entry in semantic.get('rounds', [])
                        if entry.get('validation', {}).get('valid')
                        for issue in entry.get('result', {}).get('issues', [])
                        if issue.get('target') == 'contract' and issue.get('certainty') == 'confirmed']
            if not findings:
                exc.generation_rounds = rounds
                raise
            previous = copy.deepcopy(contract)
            feedback = dict(source='validated_independent_semantic_review', findings=findings)
    raise AssertionError('Unreachable bounded generation state')
