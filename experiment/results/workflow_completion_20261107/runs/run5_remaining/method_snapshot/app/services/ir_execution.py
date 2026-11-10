"""Bounded reference execution of validated scalar IR v2 for contract assertions.

This is not a C/Arduino emulator or a substitute for independent backend tests.
It follows ordered immediate state updates and then evaluates graph outputs.
No Python eval, generated native code, hardware access or LLM is involved.
"""
from functools import lru_cache
import math
import re
import struct

from pycparser import c_ast, c_parser
from app.services.ir_contract import validate_ir


def typed(value, typ):
    if typ == 'bool':
        return bool(value), typ
    if typ == 'i32':
        result = int(value)
        if not -(2**31) <= result < 2**31:
            raise ValueError('i32 overflow during contract execution')
        return result, typ
    result = float(value)
    if typ == 'f32':
        try:
            result = struct.unpack('f', struct.pack('f', result))[0]
        except OverflowError as exc:
            raise ValueError('f32 overflow during contract execution') from exc
    if not math.isfinite(result):
        raise ValueError('Non-finite value during contract execution')
    return result, typ


@lru_cache(maxsize=1024)
def parsed(code):
    return c_parser.CParser().parse('typedef int bool; void check(void){ double result = (' + code + '); }').ext[1].body.block_items[0].init


def evaluate(code, variables, variable_types):
    def promote(a, b):
        return 'f64' if 'f64' in (a, b) else ('f32' if 'f32' in (a, b) else a)

    def node_type(n):
        if isinstance(n, c_ast.ID):
            return 'bool' if n.name in ('true', 'false') else variable_types[n.name]
        if isinstance(n, c_ast.Constant):
            return 'i32' if n.type == 'int' else ('f32' if n.value.lower().endswith('f') else 'f64')
        if isinstance(n, c_ast.Cast):
            return {'int':'i32', 'float':'f32', 'double':'f64', 'bool':'bool'}[n.to_type.type.type.names[0]]
        if isinstance(n, c_ast.UnaryOp):
            return 'bool' if n.op == '!' else node_type(n.expr)
        if isinstance(n, c_ast.TernaryOp):
            return promote(node_type(n.iftrue), node_type(n.iffalse))
        if isinstance(n, c_ast.BinaryOp):
            return 'bool' if n.op in ('&&','||','==','!=','<','<=','>','>=') else promote(node_type(n.left), node_type(n.right))
        raise ValueError('Unsupported contract expression node: ' + type(n).__name__)

    def visit(n):
        typ = node_type(n)
        if isinstance(n, c_ast.ID):
            return n.name == 'true' if n.name in ('true','false') else variables(n.name)[0]
        if isinstance(n, c_ast.Constant):
            token = n.value
            if typ == 'i32':
                return typed(int(token, 16 if token.lower().startswith('0x') else (8 if len(token)>1 and token.startswith('0') else 10)),typ)[0]
            return typed(float(re.sub('[fFlL]$', '', token)), typ)[0]
        if isinstance(n, c_ast.Cast):
            return typed(visit(n.expr), typ)[0]
        if isinstance(n, c_ast.TernaryOp):
            return typed(visit(n.iftrue if visit(n.cond) else n.iffalse), typ)[0]
        if isinstance(n, c_ast.UnaryOp):
            a = visit(n.expr)
            result = {'!':lambda:not a, '+':lambda:+a, '-':lambda:-a, '~':lambda:~a}[n.op]()
            return typed(result, typ)[0]
        a = visit(n.left)
        if n.op == '&&':
            return bool(a and visit(n.right))
        if n.op == '||':
            return bool(a or visit(n.right))
        b = visit(n.right)
        operand_type = promote(node_type(n.left), node_type(n.right))
        a, b = typed(a, operand_type)[0], typed(b, operand_type)[0]
        if n.op in ('/','%') and b == 0:
            raise ValueError('Division by zero during contract execution')
        if n.op in ('<<','>>') and (not 0 <= b < 32 or a < 0):
            raise ValueError('Undefined or target-dependent shift during contract execution')
        if n.op in ('/','%') and operand_type == 'i32':
            quotient = (abs(a)//abs(b)) * (-1 if (a < 0) != (b < 0) else 1)
            typed(quotient, 'i32')  # INT_MIN / -1 is undefined for remainder too.
            result = quotient if n.op == '/' else a - quotient*b
        else:
            operations = {'+':lambda:a+b, '-':lambda:a-b, '*':lambda:a*b, '/':lambda:a/b,
                          '==':lambda:a==b, '!=':lambda:a!=b, '<':lambda:a<b, '<=':lambda:a<=b,
                          '>':lambda:a>b, '>=':lambda:a>=b, '&':lambda:a&b, '|':lambda:a|b,
                          '^':lambda:a^b, '<<':lambda:a<<b, '>>':lambda:a>>b}
            result = operations[n.op]()
        return typed(result, typ)[0]
    tree = parsed(code)
    return typed(visit(tree), node_type(tree))


class IRExecution:
    def __init__(self, ir):
        self.ir = validate_ir(ir)
        self.nodes = {n['id']:n for n in self.ir['nodes']}
        self.drivers = {tuple(sorted(e['dst'].items())):e['src'] for e in self.ir['edges']}
        self.state_types = {s['name']:s['type'] for s in self.ir['states']}
        self.states = {s['name']:typed(evaluate(s['initial'], lambda _:None, {})[0], s['type']) for s in self.ir['states']}

    def step(self, inputs):
        if not isinstance(inputs, dict) or set(inputs) != {p['name'] for p in self.ir['inputs']}:
            raise ValueError('Execution inputs must match the complete IR input inventory')
        values = {}
        for port in self.ir['inputs']:
            value = inputs[port['name']]
            valid = type(value) is bool if port['type']=='bool' else type(value) in (int,float) and math.isfinite(value)
            if port['type']=='i32':
                valid = type(value) is int
            if not valid:
                raise ValueError('Execution input type mismatch: ' + port['name'])
            values[port['name']] = typed(value, port['type'])

        def driver(endpoint):
            return self.drivers[tuple(sorted(endpoint.items()))]

        cache = None  # Updates inline graph expressions against current state.
        def source(endpoint):
            if endpoint['kind'] == 'input':
                return values[endpoint['name']]
            if endpoint['kind'] == 'state':
                return self.states[endpoint['name']]
            if cache is not None:
                return cache[endpoint['id']]
            return node_value(self.nodes[endpoint['id']])

        def node_value(node):
            ports = {p['name']:p for p in node['inports']}
            def argument(name):
                return typed(source(driver(dict(kind='node',id=node['id'],port=name)))[0], ports[name]['type'])
            value = evaluate(node['params']['Expression'], argument, {k:p['type'] for k,p in ports.items()})
            return typed(value[0], node['outports'][0]['type'])

        def update(item):
            if item['kind'] == 'sequence':
                for child in item['steps']:
                    update(child)
                return
            endpoints = {'arg'+str(i):e for i,e in enumerate(item['sources'])}
            def endpoint_type(e):
                if e['kind']=='input':return values[e['name']][1]
                if e['kind']=='state':return self.state_types[e['name']]
                return self.nodes[e['id']]['outports'][0]['type']
            value = evaluate(item.get('expression',item.get('condition')),
                             lambda n:source(endpoints[n]), {k:endpoint_type(e) for k,e in endpoints.items()})[0]
            if item['kind'] == 'assign':
                self.states[item['state']] = typed(value, self.state_types[item['state']])
            else:
                branch = item['then'] if value else item['else']
                if branch is not None:
                    update(branch)
        for item in self.ir['updates']:
            update(item)
        cache = {}
        for node in self.ir['nodes']:
            cache[node['id']] = node_value(node)
        return {p['name']:typed(source(driver(dict(kind='output',name=p['name'])))[0],p['type'])[0] for p in self.ir['outputs']}
