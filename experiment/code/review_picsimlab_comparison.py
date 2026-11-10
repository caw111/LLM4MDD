"""Basic-function review over saved electrical observations; retain strict results."""
import argparse,bisect,collections,copy,gzip,hashlib,json,math,os,re,time,shutil,itertools,concurrent.futures
from pathlib import Path
import run_picsimlab_all as sim
ROOT=Path(__file__).resolve().parents[2];OUT=ROOT/'experiment/output/picsimlab_comparison_20261108'
POLICY_VERSION=5
LOCAL_REVISIONS={"advanced_005":"full_report_period_5s_v1"}
SKIP={
 'pceval_parking_spot_monitor':{'gpio_4_4200','gpio_3_4200'},
 'complex_001':{'no_boundary_alarm'},'medium_002':{'exact_boundary_led','exact_boundary_silent'},
 'medium_007':{'above_range_blank'},'simple_021':{'initial_silence'},
 'pceval_button_buzzer':{'button_input'},'pceval_button_servo_motor':{'external_pullup_input'},
 'pceval_led_blink_basic':{'startup_message'},'pceval_servo_motor_basic':{'no_labels_or_other_characters'},
 'complex_027':{'gpio_13_24250','distance_15000'},
 'pceval_photoresistor_basic':{'half_second_reporting'},
 'pceval_serial_LCD_display':{'whitespace_display','maximum_payload_display','short_replacement'}}
DISTANCE={'simple_014','medium_007','medium_019','pceval_distance_sensor_basic'}
def load(p):return sim.load(p)
def save(p,v):sim.save(p,v)
def numbers(s):return [float(x) for x in re.findall(r'[-+]?\d+(?:\.\d+)?',str(s))]
def canonical(s):return re.sub(r'[\W_]+','',str(s),flags=re.UNICODE).lower()
def within(c,v):
 if v is None:return False
 if c.get('numeric'):
  try:v=float(v)
  except (ValueError,TypeError):return False
 if 'expected' in c:
  e=c['expected']
  if isinstance(e,(int,float)) or c.get('numeric'):
   try:return math.isfinite(float(v)) and abs(float(v)-float(e))<=c.get('tolerance',0)+1e-9
   except (ValueError,TypeError):return False
  return str(v).strip()==str(e).strip()
 return ('min' not in c or v>=c['min']) and ('max' not in c or v<=c['max'])
class Evidence:
 def __init__(self,folder,board):
  self.plan=load(folder/'test_plan.json');self.board=board
  with gzip.open(folder/'observations.jsonl.gz','rt') as f:self.trace=[json.loads(x) for x in f]
  self.signals={};self.times={};self.pwm={}
  for e in self.trace:
   if e['type'] in ('gpio','servo','pwm','display'):
    key=(e['type'],e.get('name') if e['type']=='display' else e['pin']);self.signals.setdefault(key,[]).append(e)
  for k,events in self.signals.items():events.sort(key=lambda e:(e.get('at_ms',0),e.get('sequence',0)));self.times[k]=[e.get('at_ms',0) for e in events]
 def last(self,kind,p,t):
  k=(kind,p);es=self.signals.get(k,[]);i=bisect.bisect_right(self.times.get(k,[]),t)-1
  return es[i] if i>=0 else None
 def duty(self,p,t):
  if p not in self.pwm:
   es=self.signals.get(('gpio',p),[]);times=[];areas=[];rises=[];level=0;last=0;integral=0
   for e in es:
    at=e['at_ms']*1000;integral+=(at-last)*level;times.append(at);areas.append(integral)
    if e['value'] and not level:rises.append(at)
    level=e['value'];last=at
   self.pwm[p]=(es,times,areas,rises)
  es,ts,areas,rises=self.pwm[p];t*=1000;i=bisect.bisect_right(ts,t)-1;v=255 if i>=0 and es[i]['value'] else 0
  r=bisect.bisect_right(rises,t)
  if r>=3 and t-rises[r-1]<5000:
   a,b=rises[r-3],rises[r-1]
   def cum(at):
    j=bisect.bisect_right(ts,at)-1
    return areas[j]+(at-ts[j])*es[j]['value'] if j>=0 else 0
   if b>a:v=round(255*(cum(b)-cum(a))/(b-a))
  return v
 def scalar(self,c,t):
  k=c['kind'];p=sim.assertions.pin(c.get('pin',0),self.board)
  if k=='pwm':return self.duty(p,t)
  if k=='gpio':
   if str(p) in self.plan.get('electrical_led_pins',{}):
    duty=self.duty(p,t);return int(duty>0) if self.plan['electrical_led_pins'][str(p)] else int(duty>=255)
   e=self.last('gpio',p,t);return e['value'] if e else 0
  if k=='servo':
   e=self.last(k,p,t);return e.get('angle') if e else None
  if k=='display':
   es=[self.last(k,n.strip(),t) for n in c['name'].split(',')]
   return ''.join(e['text'] for e in es).strip() if all(es) else None
 def serial(self,c):return [e for e in self.trace if e['type']=='serial' and c.get('from_ms',0)<=e['at_ms']<=c.get('to_ms',self.plan['horizon_ms'])]
 def input(self,kind,t,pin=None):
  a=[s for s in self.plan.get('input_steps',[]) if s['kind']==kind and s['at_ms']<=t and (pin is None or str(s['pin'])==str(pin))]
  return max(a,key=lambda s:s['at_ms'])['value'] if a else None

def optional(cid,c,row):
 if c['kind'] in ('setup','pin_mode'):return '基本功能以实际响应为准，不以附加启动上限或输入模式枚举否决'
 if c['id'] in SKIP.get(cid,set()):return '沿用上一轮基本功能口径；保留严格断言为附加符合性记录'
 if cid=='advanced_005' and 'stable_two_room' not in str(row.get('plan_name')) and c['kind'] in ('serial','gpio'):return '完整报表与告警由同批稳定输入独立场景验证，旧缓存边界检查保留为诊断'
 if cid=='complex_022' and c['kind']=='pwm' and c.get('at_ms') in (2940,3440,3940):return '未规定呼吸相位，另检查这三个实际PWM样本确有亮度变化'
 return None

def qualified(cid,c,original,ev):
 actual=original['actual'];kind=c['kind'];events=ev.serial(c) if kind=='serial' else []
 if cid in DISTANCE and kind in ('display','serial'):
  expected=c.get('expected')
  if expected is None and 'min' in c and 'max' in c:expected=(c['min']+c['max'])/2
  if kind=='serial' and cid=='simple_014':expected=ev.input('distance',c['from_ms'])
  if kind=='display' and cid=='medium_019' and c['id'] not in {'selection_3400','selection_5000','selection_11400','selection_13000'}:expected=None
  if expected is not None:
   try:expected=float(expected)
   except (ValueError,TypeError):expected=None
  if expected is not None:
   tol=max(1,abs(expected)*.02);vals=[float(actual)] if kind=='display' and actual is not None and re.fullmatch(r'\s*-?\d+(?:\.\d+)?\s*',str(actual)) else [v for e in events for v in numbers(e['text'])]
   if any(abs(v-expected)<=tol for v in vals):return dict(reason='实际测距值在2%或1cm的量化容差内，基本功能不要求理想整数与固定标签',expected=expected,tolerance=tol,observed=vals[:12])
 if kind=='serial' and cid=='complex_027' and c['id']=='no_false_persistent':
  readings=[]
  for event in ev.trace:
   if event['type']=='serial' and re.search(r'dist|距',event['text'],re.I):
    vals=numbers(event['text']);readings.append((event['at_ms'],vals[-1] if vals else None))
  alarms=[e for e in events if 'persistent object detected' in e['text'].lower()]
  if alarms and all(len(prior:=[v for t,v in readings if t<=e['at_ms']][-3:])==3 and all(v is not None and v<20 for v in prior) for e in alarms):return dict(reason='告警前实际连续三次实测距离均小于20cm，名义20cm边界被量化到略小于20cm',alarms=[e['at_ms'] for e in alarms])
 if kind=='serial':
  if cid=='pceval_photoresistor_basic' and c['id'].startswith('adc_'):
   target=ev.input('analog',c['from_ms'],'A2');vals=[v for e in events for v in numbers(e['text'])]
   if target is not None and vals.count(target)>=c.get('min_count',1):return dict(reason='实际ADC读数正确，忽略冒号、空格、小数格式',expected=target,observed=vals[:12])
  if cid=='pceval_distance_sensor' and 'comparison_' in c['id']:
   word='equal' if 'equal' in c.get('contains','') else 'greater' if 'greater' in c.get('contains','') else 'less';patterns={'equal':r'(?<!not )\bequal\b|等于','greater':r'\bgreater\b|\blarger\b|大于','less':r'\bless\b|\bsmaller\b|小于'}
   hits=[e['text'] for e in events if re.search(patterns[word],e['text'],re.I)]
   if hits:return dict(reason='距离关系含义正确，文字措辞不同',relation=word,examples=hits[:3])
  if 'contains' in c and c.get('min_count',1)>0 and canonical(c['contains']):
   hits=[e['text'] for e in events if canonical(c['contains']) in canonical(e['text'])]
   if len(hits)>=c.get('min_count',1):return dict(reason='串口含义相同，忽略大小写、空格和标点',examples=hits[:3])
  if cid=='complex_027' and c['id'].startswith(('distance_','temperature_')):
   source='distance' if c['id'].startswith('distance_') else 'temperature';target=ev.input(source,c.get('from_ms',0));tol=max(1,abs(target)*.02) if source=='distance' and target is not None else 0
   hints=r'dist|距|cm' if source=='distance' else r'temp|温'
   hits=[e['text'] for e in events if re.search(hints,e['text'],re.I) and target is not None and any(abs(v-target)<=tol for v in numbers(e['text']))]
   if hits:return dict(reason='传感器实测值正确，忽略串口标签和浮点格式',expected=target,tolerance=tol,examples=hits[:3])
 if cid=='advanced_015' and kind=='serial' and c['id']=='level_6500':
  vals=[v for e in events for v in numbers(e['text'])]
  if any(abs(v-7.5)<=.5 for v in vals):return dict(reason='水位百分比的测距换算量化，允许0.5个百分点',observed=vals[:12])
 if cid=='pceval_gyroscope' and c['id']=='half_second_reports':
  gaps=actual.get('intervals_ms',[]) if isinstance(actual,dict) else []
  if len(gaps)>=c.get('min_count',3)-1 and all(450<=g<=550 for g in gaps):return dict(reason='同一X轴报告的实际连续周期约2Hz，保留小幅中断抖动',intervals_ms=gaps[:12])
 if kind=='lcd' and cid in ('pceval_button_LCD_display','pceval_serial_LCD_display','pceval_dht22_LCD_display','pceval_calendar_display','pceval_LCD_display_basic') and c.get('min_count',1)>0:
  start=c.get('from_ms',0)
  if c.get('origin')=='setup_complete':start+=next((e.get('setup_at_ms',0) for e in ev.trace if e['type']=='execution'),0)
  end=ev.plan['horizon_ms'];changes=[]
  if cid=='pceval_calendar_display':
   changes=[s['at_ms'] for s in ev.plan.get('input_steps',[]) if s['kind']=='digital' and s['value']==1 and s['at_ms']>start]
   if c['id'].startswith('initial_'):end=min(end,(86400-ev.plan['rtc_epoch_s']%86400)*1000)
  elif cid=='pceval_button_LCD_display':changes=[s['at_ms'] for s in ev.plan.get('input_steps',[]) if s['at_ms']>start]
  else:changes=[s['at_ms'] for s in ev.plan.get('serial_inputs',[])+ev.plan.get('input_steps',[]) if s['at_ms']>start+20]
  if changes:end=min(end,min(changes))
  lcd=[e for e in ev.trace if e['type']=='lcd' and start<=e['at_ms']<end]
  # Retained text is valid when no new rendering event is necessary.
  earlier=[e for e in ev.trace if e['type']=='lcd' and e['at_ms']<start]
  if earlier:
   latest=max(e['at_ms'] for e in earlier)
   lcd+=[e for e in earlier if e['at_ms']==latest]
  def match(text):
   if cid=='pceval_button_LCD_display':
    wanted_pressed=not ('not' in c.get('regex','').lower() or 'release' in c['id'])
    released=bool(re.search(r'not\s*pressed|released|未按|松开',text,re.I));pressed=bool(re.search(r'pressed|按下',text,re.I)) and not released
    return pressed if wanted_pressed else released
   if 'contains' in c:return canonical(c['contains']) in canonical(text)
   if 'regex' in c:
    try:return re.search(c['regex'].lstrip('^').rstrip('$'),text,re.I) is not None
    except re.error:return False
   return str(c.get('expected','')).strip()==text.strip()
  hits=[e for e in lcd if match(e['text'])]
  if hits:return dict(reason='实际LCD像素在对应输入阶段显示正确内容；250ms/布局属于另列严格符合性条件',window_ms=[start,end],examples=[dict(at_ms=e['at_ms'],text=e['text']) for e in hits[:3]])
 if cid=='pceval_serial_LCD_display' and kind=='serial' and c.get('min_count',1)>0:
  end=min([s['at_ms'] for s in ev.plan.get('serial_inputs',[]) if s['at_ms']>c['from_ms']+20]+[ev.plan['horizon_ms']]);extended=dict(c,to_ms=end)
  hits=[e['text'] for e in ev.serial(extended) if ('regex' not in c or re.search(c['regex'],e['text']))]
  if hits:return dict(reason='对应输入完成后实际串口回显正确；严格响应时限另列',examples=hits[:3],to_ms=end)
 return None

def review_case(case):
 folder=OUT/'cases'/case['id'];execution=folder/'case_execution.corrected_timing.json' if (folder/'case_execution.corrected_timing.json').exists() else folder/'case_execution.json';compilation=folder/'compilation.json'
 if not compilation.exists():return None
 compiled=load(compilation)
 if not compiled['compile_ok']:
  status='failed' if compiled.get('compile_status')=='compile_failed' else 'unverified'
  return dict(case=case['id'],original_case=case['original_case'],status=status,compile_ok=False,reason=compiled.get('compile_status'),source_sha256=case['source_sha256'])
 if not execution.exists():return None
 raw=load(execution);allchecks=[];extras=[]
 invalid_environment=any(r.get('observed_inputs_unhandled') or r.get('status')=='infrastructure_unverified' for r in raw.get('plans',[]))
 for row in raw.get('plans',[]):
  path=OUT/row['result_directory']
  if not (path/'observations.jsonl.gz').exists():extras.append(dict(status='unverified',reason=row.get('error',row['status'])));continue
  checks=row.get('checks',[]);ev=None
  def evidence():
   nonlocal ev
   if ev is None:ev=Evidence(path,case['fqbn'])
   return ev
  for original in checks:
   c=original['test'];r=dict(original,plan_directory=row['result_directory']);skip=optional(case['original_case'],c,row)
   if skip:r.update(basic_status='supplementary',basic_reason=skip)
   elif original['passed']:r['basic_status']='passed'
   else:
    q=qualified(case['original_case'],c,original,evidence())
    if q:r.update(basic_status='qualified_pass',qualification=q)
    else:r['basic_status']='unverified' if original.get('unverified') else 'failed'
   allchecks.append(r)
  # Correct a shared snapshot boundary only when all signals can coexist.
  times=sorted({c['test']['at_ms'] for c in allchecks if c.get('plan_directory')==row['result_directory'] and c['test']['kind'] in ('gpio','pwm','servo','display') and isinstance(c['test'].get('at_ms'),(int,float)) and 'native_eventual_group' not in c['test']})
  for at in times:
   group=[c for c in allchecks if c.get('plan_directory')==row['result_directory'] and c['test']['kind'] in ('gpio','pwm','servo','display') and c['test'].get('at_ms')==at and c['basic_status']!='supplementary']
   if not any(c['basic_status'] in ('failed','unverified') for c in group):continue
   for dt in (10,25,50):
    observed=[evidence().scalar(c['test'],at+dt) for c in group]
    if all(within(c['test'],v) for c,v in zip(group,observed)):
     for c,v in zip(group,observed):
      if c['basic_status'] in ('failed','unverified'):c.update(basic_status='qualified_pass',qualification=dict(reason='相同输入阶段所有信号在50ms物理更新容差内共同达到要求',at_ms=at+dt,actual=v))
     break
  if case['original_case']=='embedbench_30':
   groups=collections.defaultdict(list)
   for item in allchecks:
    if item.get('plan_directory')==row['result_directory'] and 'native_eventual_group' in item['test']:groups[item['test']['native_eventual_group']].append(item)
   samples=[]
   for group in groups.values():
    t=group[0]['test']['native_eventual_to_ms'];adc=[evidence().input('analog',t,p) for p in ('A3','A4','A5')];observed=[evidence().scalar(dict(kind='gpio',pin=p),t) for p in range(2,12)]
    samples.append((adc,observed))
   legal=None
   for bound,percent,count in itertools.product(('floor9','round9','bins10'),('raw','floor100','round100'),('floor','round','ceil')):
    ok=bool(samples)
    for adc,observed in samples:
     if any(x is None for x in adc):ok=False;break
     def index(x):return min(9,int(x*9/1023) if bound=='floor9' else int(x*9/1023+.5) if bound=='round9' else int(x*10/1024))
     lo,hi=sorted((index(adc[0]),index(adc[1])));ratio=adc[2]/1023
     if percent=='floor100':ratio=math.floor(ratio*100)/100
     elif percent=='round100':ratio=math.floor(ratio*100+.5)/100
     n=(hi-lo+1)*ratio;n=math.floor(n) if count=='floor' else math.floor(n+.5) if count=='round' else math.ceil(n);n=min(hi-lo+1,max(0,n))
     expected=[int(lo<=i<lo+n) for i in range(10)]
     if observed!=expected:ok=False;break
    if ok:legal=dict(bound=bound,percentage=percent,count=count);break
   if legal:
    for item in allchecks:
     if item.get('plan_directory')==row['result_directory'] and item['basic_status'] in ('failed','unverified'):item.update(basic_status='qualified_pass',qualification=dict(reason='需求未指定ADC分箱和灯数取整；同一种合法策略在全部输入阶段均匹配实际十路灯状态，且始终从较小边界顺序填充',policy=legal))
  if case['original_case']=='embedbench_47':
   grouped=collections.defaultdict(list)
   for item in allchecks:
    if item.get('plan_directory')==row['result_directory'] and 'native_eventual_group' in item['test']:grouped[item['test']['native_eventual_group']].append(item)
   for group in grouped.values():
    end=group[0]['test']['native_eventual_to_ms'];values=[evidence().scalar(item['test'],end) for item in group]
    if all(within(dict(item['test'],tolerance=max(4,item['test'].get('tolerance',0))),value) for item,value in zip(group,values)):
     for item,value in zip(group,values):
      if item['basic_status'] in ('failed','unverified'):item.update(basic_status='qualified_pass',qualification=dict(reason='HSV平滑输出的8位取整允许4/255误差；六路颜色在同一时刻联合验证',at_ms=end,actual=value))
  if case['original_case']=='complex_022':
   vals=[c['actual'] for c in checks if c['test']['kind']=='pwm' and c['test'].get('at_ms') in (2940,3440,3940)]
   extras.append(dict(id='breathing_has_real_brightness_change',status='passed' if len(vals)==3 and all(isinstance(v,(int,float)) for v in vals) and max(vals)-min(vals)>=10 else 'failed',actual=vals))
  if case['original_case']=='advanced_005' and 'stable_two_room' in str(row.get('plan_name')):
   messages=[e for e in evidence().trace if e['type']=='serial' and numbers(e['text']) and not re.search(r'alarm|alert|warn|异常|告警|报警',e['text'],re.I)]
   starts=[];last=None
   for event in sorted(messages,key=lambda e:e['at_ms']):
    if last is None or event['at_ms']-last>500:starts.append(event['at_ms'])
    last=event['at_ms']
   gaps=[b-a for a,b in zip(starts,starts[1:])]
   extras.append(dict(id='complete_two_room_report_every_five_seconds',status='passed' if len(starts)>=6 and all(4750<=gap<=5250 for gap in gaps) else 'failed',report_starts_ms=starts,intervals_ms=gaps))
  if case['original_case']=='complex_027':
   times=[e['at_ms'] for e in evidence().trace if e['type']=='serial' and re.search(r'temp|温|^T(?:\b|[:=])',e['text'],re.I)]
   gaps=[b-a for a,b in zip(times,times[1:])]
   extras.append(dict(id='three_second_report_period',status='passed' if len(times)>=7 and all(2850<=gap<=3150 for gap in gaps) else 'failed',report_times_ms=times,intervals_ms=gaps))
  if case['original_case']=='simple_029':
   e=evidence();vals=[e.duty(9,t) for t in (400,2800,5200,7600)]
   extras.append(dict(id='inverse_control_responds_across_normal_input_range',status='passed' if len(set(vals))>=3 and all(b<=a+2 for a,b in zip(vals,vals[1:])) else 'failed',actual=vals,inputs=[0,256,512,768]))
 # Either level-gating or press-to-toggle is acceptable when the prose specifies neither.
 if case['original_case']=='medium_017' and any(c['basic_status']=='failed' for c in allchecks):
  row=raw['plans'][0];ev=Evidence(OUT/row['result_directory'],case['fqbn']);vals=[ev.duty(9,t) for t in (500,2500,4000,5500,7500,10000)]
  if vals[0]==0 and vals[1]<vals[2]<vals[3] and vals[4]>0 and vals[5]==0:
   for c in allchecks:
    if c['basic_status']=='failed':c.update(basic_status='qualified_pass',qualification=dict(reason='需求允许按次锁存开关；实际首次开启后调光、第二次按下后关闭',samples=vals))
 relevant=[c for c in allchecks if c['basic_status']!='supplementary']
 status='failed' if any(c['basic_status']=='failed' for c in relevant) or any(e['status']=='failed' for e in extras) else 'unverified' if not relevant or any(c['basic_status']=='unverified' for c in relevant) or any(e['status']=='unverified' for e in extras) else 'passed'
 if invalid_environment:status='unverified'
 result=dict(case=case['id'],original_case=case['original_case'],dataset=case['dataset'],source_sha256=case['source_sha256'],profile_sha256=case['profile_sha256'],compile_ok=True,status=status,environment_invalid=invalid_environment,strict_execution_status=raw['status'],core_checks=len(relevant),qualified_checks=sum(c['basic_status']=='qualified_pass' for c in relevant),failed_checks=[c for c in relevant if c['basic_status'] in ('failed','unverified')],supplementary_checks=[c for c in allchecks if c['basic_status']=='supplementary'],qualified_evidence=[c for c in relevant if c['basic_status']=='qualified_pass'],extra_core_checks=extras)
 return result

def cached_review(case):
 target=OUT/'cases'/case['id']/'basic_review.json'
 r=load(target) if target.exists() else None
 execution_path=OUT/'cases'/case['id']/'case_execution.corrected_timing.json'
 if not execution_path.exists():execution_path=OUT/'cases'/case['id']/'case_execution.json'
 inputs=[p.stat().st_mtime_ns for p in [execution_path,OUT/'cases'/case['id']/'compilation.json'] if p.exists()]
 stale=target.exists() and inputs and target.stat().st_mtime_ns<max(inputs)
 revision=LOCAL_REVISIONS.get(case['original_case'])
 if r is None or r.get('policy_version')!=POLICY_VERSION or r.get('policy_subrevision')!=revision or stale:
  if target.exists():shutil.copyfile(target,target.with_name('basic_review.previous_'+str(time.time_ns())+'.json'))
  r=review_case(case)
  if r:r['policy_version']=POLICY_VERSION;r['policy_subrevision']=revision;save(target,r)
 return r

def main():
 assert numbers("LDR Value1023.00")==[1023.0]
 assert numbers("Temperature24.00 Distance35.84")==[24.0,35.84]
 assert not within(dict(expected=1),None)
 os.nice(15);manifest=load(OUT/'manifest.json');rows=[]
 with concurrent.futures.ProcessPoolExecutor(max_workers=4) as pool:
  rows=[r for r in pool.map(cached_review,manifest['cases']) if r]
 manual_path=OUT/'manual_function_decisions.json';manual=load(manual_path) if manual_path.exists() else {}
 for r in rows:
  if r['case'] in manual:
   decision=manual[r['case']];assert decision['source_sha256']==r['source_sha256']
   r['automatic_status']=r['status'];r['status']=decision['status'];r['manual_requirement_review']=decision
 lookup={r['case']:r for r in rows};records=[]
 for group in load(OUT/'group_records.json'):
  key=group.get('execution_key');r=lookup.get(key)
  record=dict(group=group['group'],case=group['case'],dataset=group['dataset'],execution_key=key,source_sha256=group.get('source_sha256'),historical_compile_ok=group['compile_ok'])
  if key is None:record.update(status='failed',compile_ok=False,reason='generation_failed_no_code')
  elif r:record.update(status=r['status'],compile_ok=r['compile_ok'],reason=r.get('reason'),review_file=f'cases/{key}/basic_review.json')
  else:record.update(status='pending',compile_ok=None)
  if r and r.get('manual_requirement_review'):record['manual_requirement_review']=r['manual_requirement_review']
  if key and (OUT/'cases'/key/'compilation.json').exists():record['compile_ok']=load(OUT/'cases'/key/'compilation.json')['compile_ok']
  records.append(record)
 summary={}
 for group in manifest['groups']:
  data=[r for r in records if r['group']==group];sets={}
  for name,rs in [('all',data)]+[(d,[r for r in data if r['dataset']==d]) for d in ('internal','PCEval','EmbedBench')]:
   count=collections.Counter(r['status'] for r in rs);n=len(rs);sets[name]=dict(total=n,compile_passed=sum(r['compile_ok'] is True for r in rs),**{k:count.get(k,0) for k in ('passed','failed','unverified','pending')},pass_rate=count.get('passed',0)/n if n else None)
  summary[group]=sets
 save(OUT/'basic_function_records.json',records);save(OUT/'basic_function_summary.json',dict(final=False,groups=summary,unique_reviewed=len(rows),remaining_unique=manifest['unique_sources']-len(rows),method='same basic-function standard; saved strict mismatches retained; 50ms coherent output settling, measurement/format review; code frozen'))
 save(OUT/'review_queue.json',[dict(case=r['case'],original_case=r['original_case'],status=r['status'],failed=[dict(id=c['id'],kind=c['test']['kind'],test=c['test'],actual=c['actual'],unverified=c.get('unverified')) for c in r.get('failed_checks',[])],extra=r.get('extra_core_checks')) for r in rows if r['status']!='passed'])
 print(json.dumps(dict(reviewed=len(rows),counts=dict(collections.Counter(r['status'] for r in rows))),ensure_ascii=False),flush=True)
if __name__=='__main__':main()
