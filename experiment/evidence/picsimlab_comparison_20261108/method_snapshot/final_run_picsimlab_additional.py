"""Run explicitly saved additional input scenarios without replacing primary records."""
import argparse,json,os,sys,hashlib
import run_picsimlab_all as sim

def main():
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--case',required=True);p.add_argument('--plans',required=True);p.add_argument('--version',default='native-electrical-v11');p.add_argument('--label',default='additional');p.add_argument('--resolve-configuration',action='store_true');args=p.parse_args()
 os.nice(15)
 sim.VERSION=args.version;sim.APP=sim.BASE/'engines'/sim.VERSION/'picsimlab_NOGUI';sim.RUN=sim.BASE/'additional_workers'/args.label/args.case/sim.VERSION
 case=next(c for c in sim.load(sim.OUT/'manifest.json')['cases'] if c['id']==args.case)
 case=dict(case,_variant=args.label)
 plans=sim.load(sim.OUT/args.plans)
 if args.resolve_configuration:
  sys.path.insert(0,str(sim.ROOT))
  from app.services.configuration_policy import resolve_configuration
  resolution=resolve_configuration(case['requirement'],board=case['fqbn'])
  case['configuration_resolution']=resolution;case['effective_requirement']=resolution['effective_requirement']
  policy=sim.ROOT/'app/services/configuration_policy.py'
  sim.save(sim.OUT/'cases'/case['id']/'reconstructed_bench_configuration.json',dict(resolution=resolution,policy_sha256=hashlib.sha256(policy.read_bytes()).hexdigest(),basis='Recomputed from the original requirement using the existing published configuration-defaults-v3 policy. Not claimed to be a recovered historical configuration file.',candidate_code_modified=False))
  replacements={}
  for device in resolution['devices']:
   if not device['kind'].startswith('tm1637'):continue
   before=device['original'];after=device['resolved']
   if all(k in before and before[k] is not None for k in ('clk_pin','dio_pin')):
    old='tm:'+':'.join(str(sim.assertions.pin(before[k],case['fqbn'])) for k in ('clk_pin','dio_pin'))
    new='tm:'+':'.join(str(sim.assertions.pin(after[k],case['fqbn'])) for k in ('clk_pin','dio_pin'))
    replacements[old]=new
  def remap(value):
   if isinstance(value,dict):return {k:remap(v) for k,v in value.items()}
   if isinstance(value,list):return [remap(v) for v in value]
   return replacements.get(value,value) if isinstance(value,str) else value
  plans=remap(plans)
  for entry in plans:entry['plan'].setdefault('assumptions',[]).append('Conditional bench: original pin conflicts are resolved by the existing configuration policy; see reconstructed_bench_configuration.json. Raw-wiring results remain separate.')
 rows=[sim.run_plan(case,e,0,i) for i,e in enumerate(plans)]
 sim.save(sim.OUT/'additional_results'/f'{args.case}_{args.label}_{sim.VERSION}.json',dict(case=case['id'],version=sim.VERSION,label=args.label,plans=rows))
 print(json.dumps(dict(case=case['id'],plans=[dict(name=r['plan_name'],status=r['status'],failed=[dict(id=c['id'],actual=c['actual'],test=c['test']) for c in r.get('checks',[]) if not c['passed']]) for r in rows]),ensure_ascii=False,indent=2),flush=True)
if __name__=='__main__':main()
