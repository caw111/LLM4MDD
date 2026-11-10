"""Executable boundary checks from explicitly disclosed acceptance conditions."""
import re


def check_public_button_contract(contract, requirement):
    match = re.search(r'\nOperational acceptance profile operational-acceptance-v[123] ',requirement)
    if not match:
        return
    source = requirement[:match.start()]
    # Explicit circuitry/polarity needs its own extracted facts, never a default.
    if not re.search(r'按钮|button', source, re.I) or re.search(
            r'"connections"|active.high|active.low|下拉|上拉|INPUT_PULL|pressed.*(?:HIGH|LOW)', source, re.I):
        return
    errors = []
    for index, port in enumerate(contract.get('interfaces', [])):
        binding = port.get('binding', {})
        if binding.get('kind') != 'digital_input':
            continue
        if binding.get('active_high') is not False or binding.get('pullup') is not True:
            errors.append(dict(path=f'interfaces.{index}.binding',
                stimuli=[dict(level='HIGH', expected_pressed=False),
                         dict(level='LOW', expected_pressed=True)],
                actual=binding, expected=dict(active_high=False, pullup=True)))
    if errors:
        import json
        raise ValueError('BUTTON_ELECTRICAL_REGRESSION: '+json.dumps(errors, ensure_ascii=False))


def check_startup(execution, limit_ms=500):
    if not execution or 'setup_at_ms' not in execution:
        return dict(status='unverified', reason='No execution evidence for startup')
    passed = execution.get('setup_returned') is True and (limit_ms is None or execution['setup_at_ms'] <= limit_ms)
    return dict(status='passed' if passed else 'failed', expected_max_ms=limit_ms,
                actual_ms=execution['setup_at_ms'], setup_returned=execution.get('setup_returned'))
