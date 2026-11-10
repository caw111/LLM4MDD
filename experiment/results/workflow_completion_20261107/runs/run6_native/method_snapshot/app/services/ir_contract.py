"""Canonical executable IR v2 contract, shared by production and experiments.

Version 1 is intentionally not auto-upgraded: legacy actor graphs do not carry
the execution/state semantics needed to establish equivalence with v2.
"""
import copy,re
from graphlib import TopologicalSorter,CycleError
from pycparser import c_ast,c_parser

TYPES={'i32','f32','f64','bool'}
RESERVED=set('auto break case char const continue default do double else enum extern float for goto if inline int long register restrict return short signed sizeof static struct switch typedef union unsigned void volatile while bool true false'.split())

def identifier(value):
    if not isinstance(value,str) or not re.fullmatch(r'[A-Za-z][A-Za-z0-9_]*',value) or value in RESERVED or value.startswith(('ProgramPage1','sysml_')):
        raise ValueError('Invalid or reserved IR identifier: '+str(value))
    return value

def fields(obj,required,optional=()):
    if not isinstance(obj,dict) or not set(required)<=obj.keys() or obj.keys()-set(required)-set(optional):
        raise ValueError('Expected fields '+str(required)+'; got '+str(list(obj) if isinstance(obj,dict) else type(obj).__name__))

def compatible(actual,expected):return actual==expected or (actual=='i32' and expected in ('f32','f64')) or (actual=='f32' and expected=='f64')

def expression_type(code,variables):
    if not isinstance(code,str) or len(code)>12000 or re.search(r'[;{}#"\[\]\\]|/\*|//',code):raise ValueError('Only scalar expressions are allowed')
    tree=c_parser.CParser().parse('typedef int bool; void check(void){ double result = ('+code+'); }')
    expr=tree.ext[1].body.block_items[0].init
    def visit(n):
        if isinstance(n,c_ast.ID):
            if n.name in ('true','false'):return 'bool'
            if n.name not in variables:raise ValueError('Unknown expression name '+n.name)
            return variables[n.name]
        if isinstance(n,c_ast.Constant):
            if n.type not in ('int','float','double'):raise ValueError('Unsupported literal '+n.type)
            return {'int':'i32','float':'f32','double':'f64'}[n.type]
        if isinstance(n,c_ast.UnaryOp):
            t=visit(n.expr)
            if n.op=='!' and t=='bool':return 'bool'
            if n.op in ('+','-') and t!='bool':return t
            if n.op=='~' and t=='i32':return t
        if isinstance(n,c_ast.BinaryOp):
            a,b=visit(n.left),visit(n.right)
            if n.op in ('&&','||') and a==b=='bool':return 'bool'
            if n.op in ('==','!=') and (a==b or a!='bool' and b!='bool'):return 'bool'
            if n.op in ('<','>','<=','>=') and a!='bool' and b!='bool':return 'bool'
            if n.op in ('&','|','^','<<','>>','%') and a==b=='i32':return 'i32'
            if n.op in ('+','-','*','/') and a!='bool' and b!='bool':return 'f64' if 'f64' in (a,b) else ('f32' if 'f32' in (a,b) else 'i32')
        if isinstance(n,c_ast.TernaryOp):
            if visit(n.cond)!='bool':raise ValueError('Conditional guard must be bool')
            a,b=visit(n.iftrue),visit(n.iffalse)
            if a==b:return a
            if a!='bool' and b!='bool':return 'f64' if 'f64' in (a,b) else 'f32'
        if isinstance(n,c_ast.Cast) and isinstance(n.to_type.type,c_ast.TypeDecl) and isinstance(n.to_type.type.type,c_ast.IdentifierType):
            names=n.to_type.type.type.names
            if len(names)==1 and names[0] in ('int','double','float','bool'):
                visit(n.expr);return {'int':'i32','double':'f64','float':'f32','bool':'bool'}[names[0]]
        raise ValueError('Unsupported expression operation '+type(n).__name__)
    return visit(expr)

def _validate_ir(value):
    ir=copy.deepcopy(value)
    if not isinstance(ir, dict) or ir.get('version') != 2:
        raise ValueError('Executable IR requires version=2. Legacy v1 must be explicitly migrated; use /compile/sysml2 to rebuild from the source model.')
    fields(ir,['version','name','page','inputs','outputs','nodes','edges','states','updates','execution'])
    if ir['version']!=2 or ir['page']!='ProgramPage1' or ir['execution']!='step-actions-then-outputs':raise ValueError('Use IR version 2 and step-actions-then-outputs execution')
    identifier(ir['name'])
    def ports(items,state=False):
        if not isinstance(items,list) or len(items)>128:raise ValueError('Expected bounded port list')
        result={}
        for p in items:
            fields(p,['name','type','initial'] if state else ['name','type'])
            name=identifier(p['name'])
            if name in result or p['type'] not in TYPES:raise ValueError('Duplicate name or unknown scalar type')
            result[name]=p['type']
            if state and not compatible(expression_type(p['initial'],{}),p['type']):raise ValueError('State initializer type mismatch')
        return result
    inputs,outputs,states=ports(ir['inputs']),ports(ir['outputs']),ports(ir['states'],True)
    if inputs.keys() & (outputs.keys() | states.keys()) or not outputs:
        raise ValueError('Inputs must not alias outputs/state, and outputs must be non-empty')
    for name in outputs.keys() & states.keys():
        if outputs[name] != states[name]:
            raise ValueError('An exposed state output must have the same type as its state')
    if not isinstance(ir['nodes'],list) or len(ir['nodes'])>256:raise ValueError('Invalid node list')
    nodes={};ins={};outs={}
    for node in ir['nodes']:
        fields(node,['id','type','inports','outports','params']);identifier(node['id'])
        if node['id'] in nodes or node['type']!='Expression':raise ValueError('Only unique Expression nodes are supported')
        fields(node['params'],['Expression'])
        ins[node['id']]=ports(node['inports']);outs[node['id']]=ports(node['outports'])
        if len(outs[node['id']])!=1:raise ValueError('Expression node must have one output')
        if not compatible(expression_type(node['params']['Expression'],ins[node['id']]),next(iter(outs[node['id']].values()))):raise ValueError('Node expression type mismatch')
        nodes[node['id']]=node
    def endpoint(ref,destination=False):
        if not isinstance(ref,dict):raise ValueError('Endpoint must be an object')
        kind=ref.get('kind')
        if kind=='node':
            fields(ref,['kind','id','port']);table=ins if destination else outs
            if ref['id'] not in table or ref['port'] not in table[ref['id']]:raise ValueError('Unknown node port')
            return table[ref['id']][ref['port']]
        fields(ref,['kind','name'])
        table={'output':outputs} if destination else {'input':inputs,'state':states}
        if kind not in table or ref['name'] not in table[kind]:raise ValueError('Unknown endpoint')
        return table[kind][ref['name']]
    drivers={};dependencies={nid:set() for nid in nodes}
    if not isinstance(ir['edges'],list) or len(ir['edges'])>4096:raise ValueError('Invalid edges')
    for edge in ir['edges']:
        fields(edge,['src','dst']);a=endpoint(edge['src']);b=endpoint(edge['dst'],True)
        if not compatible(a,b):raise ValueError('Edge type mismatch')
        dst=edge['dst'];key=tuple(sorted(dst.items()))
        if key in drivers:raise ValueError('Multiple drivers')
        drivers[key]=edge['src']
        if dst['kind']=='node' and edge['src']['kind']=='node':dependencies[dst['id']].add(edge['src']['id'])
    expected=[{'kind':'output','name':name} for name in outputs]+[{'kind':'node','id':nid,'port':name} for nid,ps in ins.items() for name in ps]
    if any(tuple(sorted(p.items())) not in drivers for p in expected):raise ValueError('Missing driver')
    try:ir['nodes']=[nodes[nid] for nid in TopologicalSorter(dependencies).static_order()]
    except CycleError as exc:raise ValueError('Cyclic IR graph') from exc
    def update(u,depth=0):
        if depth>32:raise ValueError('Update nesting too deep')
        kind=u.get('kind') if isinstance(u,dict) else None
        if kind=='sequence':
            fields(u,['kind','steps'])
            if not isinstance(u['steps'],list) or len(u['steps'])>256:raise ValueError('Invalid update sequence')
            for step in u['steps']:update(step,depth+1)
            return
        fields(u,['kind','state','expression','sources'] if kind=='assign' else ['kind','condition','sources','then','else'])
        if kind not in ('assign','if'):raise ValueError('Unknown update kind')
        if not isinstance(u['sources'], list) or len(u['sources']) > 128:
            raise ValueError('Update sources must be a bounded endpoint list')
        variables={'arg'+str(i):endpoint(ref) for i,ref in enumerate(u['sources'])}
        typ=expression_type(u['expression'] if kind=='assign' else u['condition'],variables)
        if kind=='assign':
            if u['state'] not in states or not compatible(typ,states[u['state']]):raise ValueError('Invalid state assignment')
        else:
            if typ!='bool':raise ValueError('Update condition must be Boolean')
            update(u['then'],depth+1)
            if u['else'] is not None:update(u['else'],depth+1)
    if not isinstance(ir['updates'],list) or len(ir['updates'])>256:raise ValueError('Invalid updates')
    for u in ir['updates']:update(u)
    return ir

def validate_ir(value):
    """Return an independent, topologically ordered v2 graph or ValueError."""
    try:
        return _validate_ir(value)
    except (KeyError, TypeError, RecursionError, c_parser.ParseError) as exc:
        raise ValueError('Invalid executable IR: ' + str(exc)) from exc
