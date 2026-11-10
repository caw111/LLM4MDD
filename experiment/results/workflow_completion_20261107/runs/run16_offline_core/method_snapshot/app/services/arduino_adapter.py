"""Generate hardware glue while retaining the deterministic model implementation."""
import re
import json


def compiled_kernel_text(header: str, source: str) -> str:
    if not header.strip() or not re.search(r'void\s+ProgramPage1\s*\(', source):
        raise ValueError('Expected define.h and ProgramPage1.c from ILTranslator.')
    if re.search(r'\bmain\s*\(', source):
        raise ValueError('Pass ProgramPage1.c rather than the host main.c.')
    source = re.sub(r'^\s*#\s*include\s+"define\.h"\s*$', '', source, flags=re.M)
    # IL i32 must remain 32-bit on AVR, where native int is 16-bit.
    kernel = re.sub(r'\bint\b', 'int32_t', header + '\n' + source)
    return '#include <stdint.h>\n#include <stdbool.h>\n' + kernel.strip() + '\n'


def adapter_issues(code: str, *, header=None, contract=None) -> list[str]:
    tokens = re.sub(r'//[^\n]*|/\*[\s\S]*?\*/|"(?:\\.|[^"\\])*"', ' ', code)
    issues = []
    if re.search(r'\b(?:sysml_state_\w*|sysml_value_\w*)\b', tokens):
        issues.append('Do not access internal model state or compiler temporaries.')
    if re.search(r'\b(?:void|int|bool|double|float|int32_t)\s+ProgramPage1\w*\s*\(', tokens):
        issues.append('Do not redeclare or redefine supplied model functions.')
    if re.search(r'#\s*(?:define|undef)\s+(?:ProgramPage1\w*|sysml_\w*|int32_t|bool|double)\b', tokens):
        issues.append('Do not redefine model symbols or scalar types with macros.')
    if re.search(r'\b(?:ProgramPage1_state|main\s*\()', tokens):
        issues.append('Do not redefine the state type or provide main().')
    without_init = re.sub(r'\bProgramPage1_Init\s*\(\s*&\s*ProgramPage1_instance\s*\)', '', tokens)
    if re.search(r'\bProgramPage1_instance\b', without_init):
        issues.append('The model instance is opaque and already allocated. Use it only in the Init call; put outputs in separate local variables.')
    for function in ('setup', 'loop'):
        if not re.search(r'\bvoid\s+' + function + r'\s*\(\s*(?:void)?\s*\)', tokens):
            issues.append('Provide Arduino ' + function + '().')
    if not re.search(r'\bProgramPage1_Init\s*\(\s*&\s*ProgramPage1_instance\s*\)', tokens):
        issues.append('Call ProgramPage1_Init(&ProgramPage1_instance) in setup().')
    if not re.search(r'\bProgramPage1\s*\(', tokens):
        issues.append('Call ProgramPage1 with sampled inputs and output pointers in loop().')
    if contract is not None and header:
        signature = re.search(r'void\s+ProgramPage1\s*\(([^)]*)\)', header)
        names = [re.findall(r'\b[A-Za-z_]\w*\b', arg)[-1] for arg in signature[1].split(',')] if signature else []
        inputs = {p['name'] for p in contract['interfaces'] if p['direction'] == 'input'}
        # Conservative direct-literal check; arbitrary C++ dataflow still needs independent tests.
        for call in re.finditer(r'\bProgramPage1\s*\(([^()]*)\)', tokens):
            for name, argument in zip(names, call[1].split(',')):
                if name in inputs and re.fullmatch(r'\s*(?:true|false|[-+]?\d+(?:\.\d*)?[fFlLuU]*)\s*', argument):
                    issues.append('Sampled input ' + name + ' cannot be replaced with a constant argument.')
    return issues


def adapter_repair_guidance(adapter, issues, repeated=False):
    """Identify concrete declarations without silently deleting generated code."""
    lines = []
    for number, line in enumerate(adapter.splitlines(), 1):
        if re.search(r'\b(?:void|int|bool|double|float|int32_t)\s+ProgramPage1\w*\s*\(', line):
            lines.append(f'Line {number}: {line.strip()}')
    guidance = '\n'.join(issues)
    if lines:
        guidance += ('\nThese lines redeclare immutable model functions. Remove these declarations from your response; '
                     'the generated kernel already provides them:\n' + '\n'.join(lines) +
                     '\nKeep the CALLS ProgramPage1_Init(&ProgramPage1_instance) in setup and ProgramPage1(...) in loop. '
                     'A call has no return type in front. Do not remove hardware sampling or output application.')
    if repeated:
        guidance += ('\nNO PROGRESS: the same structural errors occurred twice. Apply only the indicated local edits. '
                     'Do not copy the rejected declarations again. Return the entire corrected adapter.')
    return guidance


def adapter_api_description(header):
    """Describe argument order without inviting the model to copy C prototypes."""
    signature = re.search(r'void\s+ProgramPage1\s*\(([^)]*)\)', header)
    args = signature[1].strip() if signature else ''
    parameters = [] if args in ('', 'void') else args.split(',')
    lines = ['Init call in setup: ProgramPage1_Init(&ProgramPage1_instance);',
             'Step function name: ProgramPage1. Arguments in exact order:']
    for number, parameter in enumerate(parameters, 1):
        parameter = re.sub(r'\bint\b', 'int32_t', parameter.strip())
        name = re.findall(r'\b[A-Za-z_]\w*\b', parameter)[-1]
        lines.append(f'{number}. {parameter}: ' + ('output; pass &local_' + name if '*' in parameter else 'input; pass sampled value'))
    return '\n'.join(lines)


def generate_adapter(call, header, source, requirements, pins, *, max_repairs=2, contract=None):
    if contract is not None:
        from app.services.behavior_contract import validate_contract
        validate_contract(contract)
    kernel = compiled_kernel_text(header, source)
    if contract is not None and any('binding' in p for p in contract['interfaces']):
        from app.services.hardware_adapter import generate_hardware_adapter
        adapter = generate_hardware_adapter(header, contract)
        return 'deterministic-hardware-adapter', '#ifdef ARDUINO\n#include <Arduino.h>\n#endif\n// BEGIN COMPILED SYSML MODEL\n' + kernel + '// END COMPILED SYSML MODEL\n\n' + adapter
    prompt = (
        'Generate ONLY Arduino hardware adapter code, without markdown. The compiled model is immutable and is prepended automatically. '
        'Your response is appended to that kernel, not compiled as a separate translation unit. '
        'Do not copy, modify, redeclare or replace it, and do not reimplement its calculations. '
        'Use int32_t for Integer inputs/outputs, double for Real, bool for Boolean. '
        'setup() initializes hardware and calls ProgramPage1_Init(&ProgramPage1_instance) once. '
        'loop() samples hardware, calls ProgramPage1 once per control period, and applies its outputs. '
        'Keep thresholds, report due decisions, timers and state transitions in the model. '
        'For a periodic report, supply measured values and elapsed time, then format serial text only when '
        'the model report-trigger output is true. Never synthesize a report timer or constant sensor reading '
        'to compensate for a missing model output. Preserve units, initial values and sampling periods. '
        'ProgramPage1_state and ProgramPage1_instance already exist: never declare either or access instance fields. '
        'Pass &ProgramPage1_instance ONLY to ProgramPage1_Init. Use separate ordinary local variables for outputs '
        '(e.g. bool sound = false; then pass &sound to the output argument, never &ProgramPage1_instance.sound). '
        'Never access sysml_state_* or sysml_value_* symbols. Include device libraries as needed. '
        'Preserve all requirements. Keep unspecified pins and calibration values explicit and configurable.\n'
        'Never override a model output with a constant hardware command, even to compensate for a model defect. '
        'Already-provided API (use calls only; no prototypes, extern declarations, model headers or typedefs):\n' + adapter_api_description(header) + '\nRequirements:\n' + requirements + '\nPins:\n' + str(pins)
    )
    if contract is not None:
        prompt += '\nIndependent hardware interface contract:\n' + json.dumps(contract, ensure_ascii=False)
    current = prompt
    previous_issues = None
    for _ in range(max_repairs + 1):
        model, adapter = call(current)
        adapter = re.sub(r'^\s*```(?:cpp|c|arduino)?\s*\n?|\n?```\s*$', '', adapter.strip())
        issues = adapter_issues(adapter, header=header, contract=contract)
        if not issues:
            return model, '#ifdef ARDUINO\n#include <Arduino.h>\n#endif\n// BEGIN COMPILED SYSML MODEL\n' + kernel + '// END COMPILED SYSML MODEL\n\n' + adapter
        current = prompt + '\nPrevious adapter:\n' + adapter + '\nFix:\n' + adapter_repair_guidance(adapter, issues, issues == previous_issues)
        previous_issues = issues
    raise ValueError('Arduino adapter failed structural checks: ' + '; '.join(issues))
