"""Translate equivalent scalar spellings to the existing expression JSON dialect."""
import copy
import re

TOKEN = re.compile(r'\s*(RealFunctions::floor|[A-Za-z][A-Za-z0-9_]*(?:\.[A-Za-z][A-Za-z0-9_]*)*|(?:\d+\.\d*|\.\d+|\d+)(?:[eE][+-]?\d+)?|&&|\|\||==|!=|<=|>=|[()+\-*/<>,?:!])')
PRECEDENCE = {'or': 1, '||': 1, 'xor': 2, 'and': 3, '&&': 3, '==': 4, '!=': 4,
              '<': 5, '<=': 5, '>': 5, '>=': 5, '+': 6, '-': 6, '*': 7, '/': 7}


def needs_normalization(text):
    if re.search(r'\bif\b[\s\S]*\bthen\b', text):
        return True
    if re.search(r'&&|\|\||!(?!=)|(?<!:):(?!:)', text):
        return True
    for match in re.finditer(r'\b(?:and|or|xor)\s*\(', text):
        before = text[:match.start()].rstrip()
        if not before or before[-1] in '(,?:=><!+-*/&|' or re.search(r'\b(?:and|or|xor|not|if|else)$', before):
            return True
        depth = 1
        for char in text[match.end():]:
            if char == '(': depth += 1
            elif char == ')': depth -= 1
            elif char == ',' and depth == 1: return True
            if depth == 0: break
    return False


class ScalarParser:
    def __init__(self, text, render):
        self.tokens, self.position, self.render = [], 0, render
        cursor = 0
        while cursor < len(text):
            match = TOKEN.match(text, cursor)
            if not match:
                if not text[cursor:].strip(): break
                raise ValueError('Unsupported token in scalar expression at character ' + str(cursor))
            self.tokens.append(match[1]); cursor = match.end()
        if len(self.tokens) > 2048:
            raise ValueError('Scalar normalization is limited to 2048 tokens')

    def peek(self):
        return self.tokens[self.position] if self.position < len(self.tokens) else None

    def take(self, expected=None):
        token = self.peek()
        if token is None or (expected is not None and token != expected):
            raise ValueError('Expected ' + str(expected or 'operand') + ', got ' + str(token))
        self.position += 1
        return token

    def parse(self, minimum=0, depth=0):
        if depth > 32:
            raise ValueError('Scalar normalization nesting must not exceed 32')
        token = self.take()
        if token in ('not', '!', '+', '-'):
            operand = self.parse(8, depth + 1)
            if token in ('not', '!'): value = {'not': operand}
            elif type(operand) in (int, float): value = operand if token == '+' else -operand
            else: value = {token: [0, operand]}
        elif token == '(':
            value = self.parse(0, depth + 1); self.take(')')
        elif token == 'if':
            condition = self.parse(1, depth + 1)
            separator = self.take()
            if separator not in ('?', 'then'):
                raise ValueError('Conditional requires ? or then before the true value')
            yes = self.parse(0, depth + 1); self.take('else')
            value = {'if': condition, 'then': yes, 'else': self.parse(0, depth + 1)}
        elif token in ('and', 'or', 'xor', 'RealFunctions::floor'):
            self.take('('); arguments = [self.parse(0, depth + 1)]
            while self.peek() == ',':
                self.take(','); arguments.append(self.parse(0, depth + 1))
                if len(arguments) > 256: raise ValueError('Too many scalar function operands')
            self.take(')')
            if token == 'RealFunctions::floor':
                if len(arguments) != 1: raise ValueError('floor requires exactly one operand')
                value = 'RealFunctions::floor(' + self.render(arguments[0]) + ')'
            else:
                if len(arguments) < 2: raise ValueError(token + ' requires at least two operands')
                value = {token: arguments}
        elif token in ('true', 'false'):
            value = token == 'true'
        elif re.fullmatch(r'(?:\d+\.\d*|\.\d+|\d+)(?:[eE][+-]?\d+)?', token):
            value = float(token) if re.search(r'[.eE]', token) else int(token)
        elif re.fullmatch(r'[A-Za-z][A-Za-z0-9_]*(?:\.[A-Za-z][A-Za-z0-9_]*)*', token) and token not in ('then', 'else'):
            value = token
        else:
            raise ValueError('Expected a scalar operand, got ' + token)
        while self.peek() in PRECEDENCE and PRECEDENCE[self.peek()] >= minimum:
            operator = self.take()
            value = { {'&&': 'and', '||': 'or'}.get(operator, operator): [value, self.parse(PRECEDENCE[operator] + 1, depth + 1)] }
        if minimum == 0 and self.peek() == '?':
            self.take('?'); yes = self.parse(0, depth + 1); self.take(':')
            value = {'if': value, 'then': yes, 'else': self.parse(0, depth + 1)}
        return value


def normalize_expression(value, render):
    if isinstance(value, dict) and set(value) == {'if'} and isinstance(value['if'], dict):
        nested = value['if']
        condition_keys = set(nested) - {'then', 'else'}
        if {'then', 'else'} <= nested.keys() and len(condition_keys) == 1 and next(iter(condition_keys)) in PRECEDENCE:
            key = next(iter(condition_keys))
            return {'if': {key: nested[key]}, 'then': nested['then'], 'else': nested['else']}
    if not isinstance(value, str) or not needs_normalization(value):
        return value
    if len(value) > 8000:
        raise ValueError('Expression must not exceed 8000 characters')
    parser = ScalarParser(value, render)
    result = parser.parse()
    if parser.peek() is not None:
        raise ValueError('Unexpected scalar token ' + parser.peek())
    return result


def normalize_controller(controller, render):
    result, events = copy.deepcopy(controller), []
    def expr(value, path, depth=0):
        if depth > 32: raise ValueError('Expression normalization nesting must not exceed 32')
        converted = normalize_expression(value, render)
        if converted != value:
            events.append(dict(path=path, original=value, normalized=converted, rule='equivalent_scalar_syntax'))
        if isinstance(converted, dict):
            return {key: expr(item, path + '/' + key, depth + 1) for key, item in converted.items()}
        if isinstance(converted, list):
            return [expr(item, path + '/' + str(index), depth + 1) for index, item in enumerate(converted)]
        return converted
    def actions(items, path, depth=0):
        if not isinstance(items, list) or depth > 32: return
        for index, item in enumerate(items):
            if not isinstance(item, dict): continue
            at = path + '/' + str(index)
            if 'assign' in item and 'value' in item: item['value'] = expr(item['value'], at + '/value')
            if 'if' in item:
                item['if'] = expr(item['if'], at + '/if')
                for branch in ('then', 'else'): actions(item.get(branch), at + '/' + branch, depth + 1)
    def body(spec, path):
        if not isinstance(spec, dict): return
        for field, key in [('states', 'initial'), ('calculations', 'expression'), ('components', 'enabled')]:
            if isinstance(spec.get(field), list):
                for index, item in enumerate(spec[field]):
                    if isinstance(item, dict) and key in item: item[key] = expr(item[key], path + '/' + field + '/' + str(index) + '/' + key)
        actions(spec.get('updates'), path + '/updates')
        if isinstance(spec.get('outputs'), dict):
            for name, value in spec['outputs'].items():
                spec['outputs'][name] = expr(value, path + '/outputs/' + str(name).replace('~', '~0').replace('/', '~1'))
    body(result, '/controller')
    if isinstance(result, dict) and isinstance(result.get('definitions'), list):
        for index, definition in enumerate(result['definitions']):
            if isinstance(definition, dict): body(definition.get('body'), '/controller/definitions/' + str(index) + '/body')
    return result, events
