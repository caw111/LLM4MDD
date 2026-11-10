"""Assemble contract-owned SysML declarations and ordered action syntax.

The LLM supplies behavior, not package boilerplate or external interface declarations.
The result is still parsed and checked by the reference implementation, then lowered
from its semantic model. This module never emits executable IR or C.
"""
import re
import math
from decimal import Decimal
from app.services.behavior_contract import validate_sysml_contract
from app.services.ir_contract import identifier

TYPES = {'bool': 'Boolean', 'i32': 'Integer', 'f64': 'Real'}

REMAINDER_GUIDANCE = (
    'For integer parity, use "n == 2 * RealFunctions::floor(n / 2)" for even, '
    'or "n != 2 * RealFunctions::floor(n / 2)" for odd. '
    'For nonnegative integer n and positive integer d, the remainder is '
    '"n - d * RealFunctions::floor(n / d)". Preserve the original operands and domain; '
    'this floor formula is not C remainder for negative dividends. '
    'Do not add a timer or state for stateless arithmetic. Keep required timing/state for temporal behavior. '
    'These are repair examples, not automatic rewrites; complete validation remains required.'
)


class ExpressionSyntaxError(ValueError):
    """An expression-specific diagnostic, before parser cascade errors."""
    def __init__(self, message, path=None):
        self.path=path
        super().__init__((path+': ' if path else '')+message)


def fields(value, required, optional=()):
    if not isinstance(value, dict) or set(required)-value.keys() or value.keys()-set(required)-set(optional):
        raise ValueError('Model specification requires ' + str(required) + '; optional ' + str(optional))


def reference(value):
    if not isinstance(value, str) or len(value)>250:
        raise ValueError('Expected a scalar name or qualified part.scalar reference')
    for component in value.split('.'):
        identifier(component)
    return value


def expression(value, *, depth=0):
    if depth > 32:
        raise ValueError('Expression nesting must not exceed 32')
    if isinstance(value, str) and re.search(r'{\s*"if"\s*:', value):
        raise ExpressionSyntaxError('Invalid conditional expression '+repr(value[:300])+
            '. if condition then value else value is not the executable SysML expression syntax. '
            'Return {"if":"condition","then":"yesValue","else":"noValue"}; this renders (if condition ? yesValue else noValue). '
            'Example: if remaining > 0 then remaining - delta else 0 becomes '
            '{"if":"remaining > 0","then":"remaining - delta","else":0}. '
            'For a conditional nested in arithmetic, return the WHOLE expression as nested objects: '
            'elapsed + (if enabled then delta else 0) becomes '
            '{"+":["elapsed",{"if":"enabled","then":"delta","else":0}]}. '
            'Never put a JSON object inside an expression string. '
            'Preserve both values and the original condition; '
            'then/else action arrays are different from expression values.')
    from app.services.expression_normalization import normalize_expression
    try:
        converted = normalize_expression(value, lambda item: expression(item, depth=depth + 1))
    except ValueError as exc:
        raise ExpressionSyntaxError('Cannot normalize scalar expression: ' + str(exc)) from exc
    if converted is not value and converted != value:
        return expression(converted, depth=depth + 1)
    # JSON scalars are literal syntax, not a request to coerce model types.
    # In particular bool must be handled before int (bool subclasses int).
    if type(value) is bool:
        return 'true' if value else 'false'
    if type(value) is int:
        value = str(value)
    elif type(value) is float:
        if not math.isfinite(value):
            raise ExpressionSyntaxError('Numeric literal must be finite; NaN and Infinity are not model values')
        # Keep Real literals Real, including 1.0 and exponent-form JSON numbers.
        # Plain decimal syntax also avoids relying on parser exponent support.
        value = format(Decimal(repr(value)), 'f')
        if '.' not in value:
            value += '.0'
    if isinstance(value, dict):
        if set(value)=={'not'}:
            rendered='(not '+expression(value['not'],depth=depth+1)+')'
        elif set(value) in ({'floor'}, {'RealFunctions::floor'}):
            rendered='RealFunctions::floor('+expression(next(iter(value.values())),depth=depth+1)+')'
        elif len(value)==1 and next(iter(value)) in ('mod','%','remainder'):
            raise ExpressionSyntaxError('Unsupported executable remainder object '+repr(value)[:300]+'. '+REMAINDER_GUIDANCE)
        elif len(value)==1 and next(iter(value)) in ('and','or','xor','==','!=','<','<=','>','>=','+','-','*','/'):
            operator, operands = next(iter(value.items()))
            maximum = 256 if operator in ('and','or','xor') else 2
            if not isinstance(operands,list) or not 2 <= len(operands) <= maximum:
                raise ExpressionSyntaxError('Structured operator '+repr(operator)+' requires '+
                    ('2..256' if maximum==256 else 'exactly 2')+' expression operands; preserve every operand.')
            # 每个操作数保留括号与顺序，避免混合运算优先级改变原语义。
            rendered='('+(' '+operator+' ').join('('+expression(item,depth=depth+1)+')' for item in operands)+')'
        else:
            if set(value)!={'if','then','else'}:
                raise ExpressionSyntaxError('Unknown expression object '+repr(value)[:300]+
                    '. Use {"not": expression}, {"floor": numericExpression}, {"and": [left, right]}, a supported binary operator with two operands, '
                    'or {"if": condition, "then": value, "else": value}; action arrays belong only in statements.')
            condition, yes, no = (expression(value[key], depth=depth+1) for key in ('if','then','else'))
            rendered = '(if ' + condition + ' ? ' + yes + ' else ' + no + ')'
        if len(rendered) > 8000:
            raise ValueError('Expanded expression must not exceed 8000 characters')
        return rendered
    if not isinstance(value, str) or not value.strip() or len(value)>8000:
        raise ValueError('Expressions must be expression strings, JSON Boolean/finite numeric literals, or supported expression objects')
    if '%' in value:
        raise ExpressionSyntaxError('Unsupported executable remainder expression '+repr(value[:300])+
            '. Do not copy C % syntax into SysML or silently replace it with division. '+REMAINDER_GUIDANCE)
    if value.strip() in {'and','or','xor','not','==','!=','<','<=','>','>=','+','-','*','/'}:
        raise ExpressionSyntaxError('Operator '+repr(value)+' has no operands. Preserve the full condition/value; '
            'use a binary infix expression such as (count + 37 >= 100) or {">=": ["count + 37", 100]}.')
    function_style = False
    for match in re.finditer(r'(?:\b(?:and|or|xor)|==|!=|<=|>=|[<>+*/-])\s*\(', value):
        nesting = 1
        for char in value[match.end():]:
            if char == '(': nesting += 1
            elif char == ')': nesting -= 1
            elif char == ',' and nesting == 1: function_style = True
            if nesting == 0: break
    if function_style:
        raise ExpressionSyntaxError('Invalid function-style operator expression ' + repr(value[:300]) +
            '. SysML uses binary INFIX operators: (left and right), (left or right), (left xor right), (left >= right). '
            'Example: and(pressed, not(previous)) becomes (pressed and not previous). '
            'Example: >=(count + 37, 100) becomes (count + 37 >= 100), or {">=": ["count + 37", 100]}. '
            'Keep every operand, guard and state update; not(value) is already valid unary syntax. '
            'Correct all function-style Boolean occurrences in this complete candidate, then run full validation.')
    if re.search(r'[;{}#]|//|/\*|\*/',value):
        raise ValueError('An expression cannot contain declarations, statements or comments')
    if re.search(r'(?<!:):(?!:)|&&|\|\||!(?!=)', value) or ('?' in value and not re.search(r'\bif\b',value)):
        raise ValueError('Invalid expression ' + repr(value[:200]) + '. For conditionals return an expression object {"if":"condition","then":"yesValue","else":"noValue"}; the application renders SysML syntax. Use not/and/or, not !/&&/||.')
    return value.strip()


def scalar_type(value):
    if not isinstance(value,str) or value not in TYPES:
        raise ValueError('Use scalar type bool, i32 or f64 in SysML model specification; f32 is only supported by direct IR')
    return TYPES[value]


def sequence(statements, state_names, *, depth=0):
    if not isinstance(statements,list) or len(statements)>256 or depth>32:
        raise ValueError('An action branch needs 0..256 statements and nesting <=32')
    if not statements:
        # 显式空动作仍保留外层条件，由参考解析器和 IR 校验引用及类型。
        return ['first start;', 'then done;']
    lines=['first start;']
    for statement in statements:
        if isinstance(statement,dict) and 'assign' in statement:
            fields(statement,['assign','value'])
            target=reference(statement['assign'])
            if '.' not in target and target not in state_names:
                raise ValueError('Assignment target must be initialized internal state: '+target+
                                 '. For stateless behavior, put the input expression directly in outputs.')
            lines.append('then assign '+target+' := '+expression(statement['value'])+';')
        else:
            fields(statement,['if','then'],['else'])
            lines.append('then if '+expression(statement['if'])+' {')
            lines.extend('  '+line for line in sequence(statement['then'],state_names,depth=depth+1))
            lines.append('}')
            other=statement.get('else',[])
            if not isinstance(other,list):
                raise ValueError('else must be a statement list, or omitted')
            if other:
                lines[-1]+=' else {'
                lines.extend('  '+line for line in sequence(other,state_names,depth=depth+1))
                lines.append('}')
    return lines


def body(spec, inputs, outputs):
    fields(spec,['states','updates','outputs'],['calculations','parts','bindings','components'])
    from app.services.sysml_components import expand_components
    spec = expand_components(spec, inputs)
    if not isinstance(spec['outputs'],dict) or set(spec['outputs'])!={p['name'] for p in outputs}:
        raise ValueError('outputs must contain exactly these declared names, with one expression each: ' + ', '.join(p['name'] for p in outputs))
    names=set()
    lines=[]
    def declare(name):
        identifier(name)
        if name in names:
            raise ValueError('Duplicate declared name in model specification: '+name)
        names.add(name)
        return name
    for p in inputs:
        lines.append('in attribute '+declare(p['name'])+' : '+scalar_type(p['type'])+';')
    for p in outputs:
        declare(p['name'])
    states=spec['states']
    if not isinstance(states,list) or len(states)>128:
        raise ValueError('states must be a list with at most 128 initialized scalars')
    state_names=set()
    for s in states:
        fields(s,['name','type','initial'])
        name=declare(s['name']);state_names.add(name)
        lines.append('attribute '+name+' : '+scalar_type(s['type'])+' := '+expression(s['initial'])+';')
    calculations=spec.get('calculations',[])
    if not isinstance(calculations,list) or len(calculations)>256:
        raise ValueError('calculations must be a bounded list')
    for c in calculations:
        fields(c,['name','type','expression'])
        lines.append('attribute '+declare(c['name'])+' : '+scalar_type(c['type'])+' = '+expression(c['expression'])+';')
    parts=spec.get('parts',[])
    if not isinstance(parts,list) or len(parts)>128:
        raise ValueError('parts must be a bounded list')
    for part in parts:
        fields(part,['name','definition'])
        lines.append('part '+declare(part['name'])+' : '+identifier(part['definition'])+';')
    bindings=spec.get('bindings',[])
    if not isinstance(bindings,list) or len(bindings)>256:
        raise ValueError('bindings must be a bounded list')
    for b in bindings:
        fields(b,['left','right'])
        lines.append('bind '+reference(b['left'])+' = '+reference(b['right'])+';')
    if not isinstance(spec['updates'],list):
        raise ValueError('updates must be an ordered statement list')
    if spec['updates']:
        lines.append('perform action tick {')
        lines.extend('  '+line for line in sequence(spec['updates'],state_names))
        lines.append('}')
    for p in outputs:
        lines.append('out attribute '+p['name']+' : '+scalar_type(p['type'])+' = '+expression(spec['outputs'][p['name']])+';')
    return lines


def validate_expression_locations(controller):
    """Check original expressions before component expansion, retaining JSON pointers."""
    def check(value,path):
        try:expression(value)
        except ValueError as exc:
            # All failures inside expression() concern this expression slot,
            # including null/list values, C operators and resource bounds.
            # Preserve its original location instead of reporting BUNDLE_FORMAT.
            raise ExpressionSyntaxError(str(exc),path) from exc
    def actions(items,path,depth=0):
        if not isinstance(items,list) or depth>32:return
        for i,item in enumerate(items):
            if not isinstance(item,dict):continue
            at=path+'/'+str(i)
            if 'assign' in item and 'value' in item:check(item['value'],at+'/value')
            elif 'if' in item:
                check(item['if'],at+'/if')
                for branch in ('then','else'):actions(item.get(branch),at+'/'+branch,depth+1)
    def body_expressions(spec,path):
        if not isinstance(spec,dict):return
        for field,key in [('states','initial'),('calculations','expression')]:
            if isinstance(spec.get(field),list):
                for i,item in enumerate(spec[field]):
                    if isinstance(item,dict) and key in item:check(item[key],path+'/'+field+'/'+str(i)+'/'+key)
        actions(spec.get('updates'),path+'/updates')
        if isinstance(spec.get('outputs'),dict):
            for name,value in spec['outputs'].items():
                escaped=str(name).replace('~','~0').replace('/','~1')
                check(value,path+'/outputs/'+escaped)
    body_expressions(controller,'/controller')
    if isinstance(controller.get('definitions'),list):
        for i,definition in enumerate(controller['definitions']):
            if isinstance(definition,dict):body_expressions(definition.get('body'),'/controller/definitions/'+str(i)+'/body')


def assemble_sysml(controller, contract):
    validate_sysml_contract(contract)
    fields(controller,['states','updates','outputs'],['calculations','parts','bindings','definitions','components'])
    validate_expression_locations(controller)
    definitions=controller.get('definitions',[])
    if not isinstance(definitions,list) or len(definitions)>64:
        raise ValueError('definitions must be a list with at most 64 part definitions')
    lines=['package GeneratedModel {','  private import ScalarValues::*;']
    defined={'Controller','GeneratedModel','system'}
    for definition in definitions:
        fields(definition,['name','inputs','output_ports','body'])
        name=identifier(definition['name'])
        if name in defined:
            raise ValueError('Duplicate or reserved part definition: '+name)
        defined.add(name)
        for category in ('inputs','output_ports'):
            if not isinstance(definition[category],list) or len(definition[category])>128:
                raise ValueError('Part ports must be bounded lists')
            for p in definition[category]:
                fields(p,['name','type'])
        lines.append('  part def '+name+' {')
        lines.extend('    '+line for line in body(definition['body'],definition['inputs'],definition['output_ports']))
        lines.append('  }')
    root={k:v for k,v in controller.items() if k!='definitions'}
    lines.append('  part def Controller {')
    lines.extend('    '+line for line in body(root,
        [p for p in contract['interfaces'] if p['direction']=='input'],
        [p for p in contract['interfaces'] if p['direction']=='output']))
    lines+=['  }','  part system : Controller;','}']
    return '\n'.join(lines)+'\n'
