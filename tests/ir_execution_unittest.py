import unittest
from pathlib import Path
import shutil
import subprocess
import tempfile

from app.services.ir_execution import IRExecution, evaluate


def expression_ir(expression, typ='i32'):
    return dict(version=2,name='Expression',page='ProgramPage1',execution='step-actions-then-outputs',
                inputs=[dict(name='a',type=typ),dict(name='b',type=typ)],outputs=[dict(name='result',type=typ)],
                states=[],updates=[],nodes=[dict(id='op',type='Expression',inports=[dict(name='x',type=typ),dict(name='y',type=typ)],
                    outports=[dict(name='result',type=typ)],params={'Expression':expression})],
                edges=[dict(src=dict(kind='input',name=n),dst=dict(kind='node',id='op',port=p)) for n,p in [('a','x'),('b','y')]]+
                      [dict(src=dict(kind='node',id='op',port='result'),dst=dict(kind='output',name='result'))])


class ExecutionTests(unittest.TestCase):
    def test_expression_execution_matches_native_c_for_defined_cases(self):
        cases=[('i32','a/b',-7,3),('i32','a%b',-7,3),('i32','(a<<2)^b',13,7),
               ('i32','a>b ? a-b : b-a',4,9),('f32','a+b',16777216.0,1.0),
               ('f64','a+b',16777216.0,1.0),('f64','a>0.0 ? b/(a-1.0) : 0.0',0.0,2.0),
               ('f64','((int)a)-((a < (double)((int)a)) ? 1 : 0)',-1.5,0.0),
               ('f64','(float)(a+b)',16777216.0,1.0),('f32','a+0.1f',1.0,0.0)]
        cpp=['#include <iostream>','#include <iomanip>','int main(){std::cout<<std::setprecision(17);']
        expected=[]
        for typ,expr,a,b in cases:
            ctype={'i32':'int','f32':'float','f64':'double'}[typ]
            cpp.append('{ '+ctype+' a='+repr(a)+', b='+repr(b)+'; std::cout<<('+expr+')<<"\\n"; }')
            values={'a':(a,typ),'b':(b,typ)}
            expected.append(evaluate(expr,values.__getitem__,{'a':typ,'b':typ})[0])
        cpp.append('}')
        with tempfile.TemporaryDirectory() as folder:
            source=Path(folder)/'expressions.cpp';binary=Path(folder)/'expressions.exe'
            source.write_text('\n'.join(cpp),encoding='utf-8')
            build=subprocess.run([shutil.which('g++'),'-std=c++17',str(source),'-o',str(binary)],capture_output=True,text=True,timeout=30)
            self.assertEqual(build.returncode,0,build.stderr)
            run=subprocess.run([str(binary)],capture_output=True,text=True,timeout=10)
            self.assertEqual(run.returncode,0,run.stderr)
            self.assertEqual([float(line) for line in run.stdout.splitlines()],expected)

    def test_integer_operations_match_signed_c_and_detect_undefined_values(self):
        for code,a,b,want in [('x/y',-7,3,-2),('x%y',-7,3,-1),('x%y',7,-3,1),('x^y',10,3,9),
                              ('x<<y',3,4,48),('x>>y',48,4,3),('x&y',10,3,2),('x|y',10,3,11)]:
            with self.subTest(code=code):self.assertEqual(IRExecution(expression_ir(code)).step(dict(a=a,b=b))['result'],want)
        for code,a,b in [('x+y',2147483647,1),('x/y',-2147483648,-1),('x%y',-2147483648,-1),
                         ('x/y',1,0),('x<<y',1,32),('x>>y',-1,1)]:
            with self.subTest(code=code),self.assertRaises(ValueError):IRExecution(expression_ir(code)).step(dict(a=a,b=b))

    def test_f32_rounding_and_f64_are_distinct(self):
        self.assertEqual(IRExecution(expression_ir('x+y','f32')).step(dict(a=16777216.0,b=1.0))['result'],16777216.0)
        self.assertEqual(IRExecution(expression_ir('x+y','f64')).step(dict(a=16777216.0,b=1.0))['result'],16777217.0)

    def test_branch_local_expressions_remain_lazy(self):
        variables={'enabled':(False,'bool'),'sample':(1.0,'f64')}
        types={k:v[1] for k,v in variables.items()}
        for code,expected in [('enabled ? sample/(sample-1.0) : 0.0',0.0),
                              ('enabled && (sample/(sample-1.0)>0.0)',False),
                              ('!enabled || (sample/(sample-1.0)>0.0)',True)]:
            self.assertEqual(evaluate(code,variables.__getitem__,types)[0],expected)

    def test_immediate_updates_recompute_graph_from_new_state(self):
        ir=expression_ir('x+y')
        ir['states']=[dict(name='count',type='i32',initial='0')]
        ir['edges'][0]['src']=dict(kind='state',name='count')
        ir['updates']=[dict(kind='sequence',steps=[
            dict(kind='assign',state='count',expression='arg0+1',sources=[dict(kind='state',name='count')]),
            dict(kind='if',condition='arg0==6',sources=[dict(kind='node',id='op',port='result')],
                 then=dict(kind='assign',state='count',expression='10',sources=[]),**{'else':None})])]
        engine=IRExecution(ir)
        self.assertEqual(engine.step(dict(a=0,b=5))['result'],15)
        self.assertEqual(engine.step(dict(a=0,b=5))['result'],16)
        self.assertEqual(IRExecution(ir).step(dict(a=0,b=5))['result'],15)

    def test_all_graph_nodes_execute_after_updates_even_if_unconnected(self):
        ir=expression_ir('x/y')
        ir['edges'][-1]['src']=dict(kind='input',name='a')
        with self.assertRaisesRegex(ValueError,'zero'):IRExecution(ir).step(dict(a=1,b=0))

    def test_invalid_ir_and_bad_input_inventory_cannot_execute(self):
        ir=expression_ir('x+y');ir['version']=1
        with self.assertRaises(ValueError):IRExecution(ir)
        engine=IRExecution(expression_ir('x+y'))
        for sample in [dict(a=True,b=1),dict(a=1),dict(a=1,b=2,c=3),dict(a=1.0,b=2)]:
            with self.assertRaises(ValueError):engine.step(sample)


if __name__=='__main__':unittest.main()
