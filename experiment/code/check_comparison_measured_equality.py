import copy,os
import run_picsimlab_comparison_groups as run
sim=run.sim
os.nice(15);sim.OUT=run.OUT;sim.VERSION=run.VERSION;sim.APP=sim.BASE/'engines'/run.VERSION/'picsimlab_NOGUI';sim.RUN=run.BUILD/'distance_equality_check'
case=next(c for c in run.load(run.OUT/'manifest.json')['cases'] if c['id']=='pceval_distance_sensor__d22efb249a06a302');case=dict(case,_variant='equality')
p=copy.deepcopy(run.load(run.OUT/'test_profiles.json')['pceval_distance_sensor']['entries'][0]['plan']);p['serial_inputs'][1]['text']='198\n'
p['checks']=[dict(id='relation_'+str(i),kind='serial',from_ms=1200+i*3000,to_ms=2200+i*3000,contains='comparison_result_code'+str(v),min_count=1,requirement_quote='根据实测整数厘米与串口目标进行大小比较') for i,v in enumerate([2,1,0])]
row=sim.run_plan(case,dict(name='measured_integer_comparison_three_branches',plan=p),1500,0);run.save(run.OUT/'distance_equality_check.json',row);print(row['status'],flush=True)
