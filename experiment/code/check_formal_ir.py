import sys,json,copy
from pathlib import Path
ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT))
from app.services.ir_contract import validate_ir
from app.services.ir_prompt import IR_EXAMPLE
assert validate_ir(IR_EXAMPLE)==IR_EXAMPLE
for name,modify in [
 ('missing_driver',lambda x:x['edges'].pop()),
 ('duplicate_driver',lambda x:x['edges'].append(copy.deepcopy(x['edges'][0]))),
 ('unknown_port',lambda x:x['edges'][0]['src'].update(name='missing')),
 ('bad_expression',lambda x:x['nodes'][0]['params'].update(Expression='system(1)')),
 ('type_mismatch',lambda x:x['nodes'][0]['outports'][0].update(type='bool'))]:
 value=copy.deepcopy(IR_EXAMPLE);modify(value)
 try:validate_ir(value)
 except Exception:pass
 else:raise AssertionError(name)
from app.services.sysml_compiler import compile_sysml
assert validate_ir(compile_sysml((ROOT/'tools/sysml/counter.sysml').read_text(encoding='utf-8')))['states']
print('IR graph/type and compiler-produced stateful IR checks passed')
