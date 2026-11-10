"""Shared prompt for the direct IR v2 generation path."""
import json

IR_EXAMPLE={'version':2,'name':'Gate','page':'ProgramPage1','inputs':[{'name':'enabled','type':'bool'},{'name':'value','type':'f64'}],
 'outputs':[{'name':'selected','type':'f64'}], 'nodes':[{'id':'choose','type':'Expression','inports':[{'name':'arg0','type':'bool'},{'name':'arg1','type':'f64'}],
 'outports':[{'name':'result','type':'f64'}],'params':{'Expression':'arg0 ? arg1 : 0.0'}}],
 'edges':[{'src':{'kind':'input','name':'enabled'},'dst':{'kind':'node','id':'choose','port':'arg0'}},{'src':{'kind':'input','name':'value'},'dst':{'kind':'node','id':'choose','port':'arg1'}},{'src':{'kind':'node','id':'choose','port':'result'},'dst':{'kind':'output','name':'selected'}}],
 'states':[],'updates':[],'execution':'step-actions-then-outputs'}
IR_V2_PROMPT='''Generate only a JSON scalar computational IR implementing ALL requirements. Hardware samples/time are inputs, actuator commands are outputs; hardware I/O belongs to a later adapter. Never drop behavior or replace computations with fabricated constant outputs. Use the exact version-2 schema illustrated below. Types: i32 (32-bit integer), f32, f64, bool. Expressions are C scalar arithmetic, comparisons, Boolean operations and ternary conditionals; no calls, pointers, strings, arrays or statements. Only inport names are visible inside an Expression. Each input port/output has exactly one driver; no algebraic cycles. Expression nodes have exactly one outport. Inputs, outputs and states have distinct nonreserved identifiers.
Persistent state uses states [{"name":"count","type":"i32","initial":"0"}]. State is a source endpoint {"kind":"state","name":"count"}.
Updates happen in order BEFORE outputs. An assignment: {"kind":"assign","state":"count","expression":"arg0 + 1","sources":[{"kind":"state","name":"count"}]}. A conditional: {"kind":"if","condition":"arg0","sources":[{"kind":"input","name":"enabled"}],"then":ASSIGNMENT,"else":null}. A sequence: {"kind":"sequence","steps":[ASSIGNMENT,...]}. arg0, arg1 etc refer in order to sources. Each update reads state as modified by preceding updates. Initializers must be constant. Put all controller computations in this model; the later adapter only samples inputs and applies outputs. No extra schema fields.
Example:\n'''+json.dumps(IR_EXAMPLE)+'\nRequirements and supplied circuit:\n'
