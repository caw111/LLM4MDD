"""Add offline checks where saved trace-review entries had no executable assertions.
Expectations come from requirements and supplied wiring; source/HEX remain frozen.
"""
import copy,json,hashlib
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
OUT=ROOT/'experiment/output/picsimlab_verification_20261107'
manifest=json.loads((OUT/'manifest.json').read_text())
CASES={c['id']:c for c in manifest['cases']}
def write(cid,plan,explanation,assumptions=(),coverage='core_scenarios'):
 p=OUT/'cases'/cid/'offline_supplementary_plans.json'
 plan.update(supported=True,coverage=coverage,coverage_explanation=explanation,assumptions=list(assumptions))
 p.write_text(json.dumps([dict(name='offline_requirement_checks_20261108',design_basis='original_requirement_and_supplied_circuit',plan=plan)],ensure_ascii=False,indent=2))
def original(cid):return copy.deepcopy(json.loads((OUT/'cases'/cid/'test_plans.json').read_text())[0]['plan'])
def check(cid,kind,**kw):return dict(id='core_'+kind+'_'+hashlib.sha256(json.dumps(kw,sort_keys=True).encode()).hexdigest()[:10],kind=kind,requirement_quote=CASES[cid]['requirement'].split('Given circuit')[0],**kw)
def add(plan,cid,kind,**kw):plan['checks'].append(check(cid,kind,**kw))
def display(plan,cid,t,n,tolerance=0):add(plan,cid,'display',name='tm:4:5',at_ms=t,expected=n,numeric=True,tolerance=tolerance)
def pwm(plan,cid,p,t,n,tolerance=3):add(plan,cid,'pwm',pin=p,at_ms=t,expected=n,tolerance=tolerance)
def gpio(plan,cid,p,t,n):add(plan,cid,'gpio',pin=p,at_ms=t,expected=n)
def pulse(plan,p,t,high=False,duration=350):
 plan['input_steps'].extend([dict(at_ms=t,pin=p,kind='digital',value=int(high)),dict(at_ms=t+duration,pin=p,kind='digital',value=int(not high))])
def input_(plan,p,t,v,kind='analog'):plan['input_steps'].append(dict(at_ms=t,pin=p,kind=kind,value=v))
def blank(h):return dict(horizon_ms=h,input_steps=[],checks=[],ultrasonic=[],serial_inputs=[],seven_segments=[],shift_registers=[])

def main():
 for cid in ('simple_004','simple_029'):
  p=blank(11500);values=[0,60,128,200,255,768,1023,200,0]
  for i,v in enumerate(values):
   input_(p,'A0',i*1200,v)
  add(p,cid,'sample_relation',source='pwm',pin=9,at_ms=[1000+i*1200 for i in range(7)],relation='nonincreasing' if cid.endswith('029') else 'nondecreasing',min_distinct=4,tolerance=2)
  add(p,cid,'sample_relation',source='pwm',pin=9,at_ms=[1000,10600],relation='equal',tolerance=2)
  write(cid,p,'Monotonic brightness response, at least four distinguishable levels, and repeated-input stability.',('PWM duty is measured over complete hardware carrier periods.','For the photoresistor bench, larger A0 means stronger light.','The prose specifies no linear scaling formula; saturation and nonlinear transfer are not alone counted as core failures.'))
 cid='simple_026';p=blank(30000);add(p,cid,'pwm_ramp_repeat',source='pwm',pin=9,at_ms=list(range(100,30000,20)))
 write(cid,p,'Two complete increasing brightness ramps with at least 40 distinct measured PWM levels.',('Thirty seconds is a finite observation budget; the requirement does not specify fade speed.',))
 cid='medium_009';p=blank(17000)
 input_(p,7,0,40,'humidity')
 for t,v in [(0,10),(3500,20),(7000,30),(10500,15),(14000,25)]:
  input_(p,7,t,v,'temperature');display(p,cid,t+2700,v)
 write(cid,p,'DHT11 integer temperatures are held long enough to verify rising/falling display updates.',('Unspecified TM1637 wiring uses CLK D4, DIO D5.','Snapshots allow the specified two-second refresh plus DHT transfer and display I/O.'))
 cid='medium_017';p=blank(11000);input_(p,2,0,0,'digital')
 for t,v in [(0,0),(1800,256),(3300,900),(5000,1023),(8000,512),(9500,128)]:input_(p,'A0',t,v)
 pulse(p,2,1000,True);pulse(p,2,4300,True);pulse(p,2,7000,True)
 for t,v in [(600,0),(2800,64),(4100,224),(6500,0),(9000,128),(10500,32)]:pwm(p,cid,9,t,v,4)
 write(cid,p,'Button toggles enable, disabled LED stays off despite input change, re-enabled PWM follows A0.',('External pull-down button is active HIGH, as in the saved test interface.',))
 cid='medium_022';p=blank(13000);p['ultrasonic']=[dict(trigger_pin=8,echo_pin=9)]
 for i,v in enumerate([30,7,20,4,50]):
  input_(p,9,i*2500,v,'distance');gpio(p,cid,13,i*2500+1800,int(v<10));display(p,cid,i*2500+1800,v,1)
 write(cid,p,'Distance display and below-ten-centimeter warning at values away from the threshold.',('Unspecified wiring uses HC-SR04 D8/D9, TM1637 D4/D5.','One centimeter allows pulseIn integer quantization.'))
 cid='medium_024';p=blank(13000);input_(p,2,0,0,'digital');times=[500]
 display(p,cid,500,0)
 for i,t in enumerate(range(1500,12000,1700)):
  pulse(p,2,t,True);times.append(t+800);display(p,cid,t+800,i+1)
 add(p,cid,'display_led_parity',name='tm:4:5',pin=13,at_ms=times)
 write(cid,p,'One increment per press and consistent opposite LED levels for odd and even counts.',('Unspecified TM1637 wiring uses D4/D5; saved active-HIGH button interface is retained.','The prose does not specify which parity lights the LED.'))
 cid='medium_029';p=blank(26000);input_(p,2,0,0,'digital');pulse(p,2,10000,True);pulse(p,2,16000,True)
 add(p,cid,'buzzer_bursts',pin=6,from_ms=1000,to_ms=9500,min_count=3,period_ms=3000,tolerance_ms=100)
 add(p,cid,'edges',pin=6,edge='both',from_ms=11000,to_ms=15500,max_count=0)
 add(p,cid,'buzzer_bursts',pin=6,from_ms=16500,to_ms=25500,min_count=2,period_ms=3000,tolerance_ms=100)
 write(cid,p,'Three-second beep cycles, silence while paused, and resumed periodic beeping.',('Saved external pull-down button interface is retained.','The initial phase and beep duration are not prescribed.'))
 cid='pceval_distance_sensor_basic';p=original(cid)
 for i,v in enumerate([34,80,120]):add(p,cid,'serial',from_ms=i*4000+1000,to_ms=min(i*4000+3900,12900),prefix='Distance in CM',expected=v,tolerance=1,min_count=1)
 write(cid,p,'Three actual echo pulse widths must produce corresponding centimeter reports.',('The core criterion accepts missing punctuation between the distance label and its numeric value; exact format remains a separate conformance observation.',))
 cid='complex_005';p=original(cid)
 add(p,cid,'sample_relation',source='display',name='tm:4:5',at_ms=[1000,2300],relation='different')
 add(p,cid,'sample_relation',source='display',name='tm:4:5',at_ms=[3200,4500,6000],relation='equal')
 add(p,cid,'sample_relation',source='display',name='tm:4:5',at_ms=[7500,9000,11000],relation='different',min_distinct=3)
 add(p,cid,'sample_relation',source='pwm',pin=9,at_ms=[1000,2300],relation='different')
 write(cid,p,'Automatic updates, frozen display across input changes, and resumed updates.',('Saved active-HIGH button interface is retained.','No particular light mapping formula or manual LED behavior is imposed.'))
 cid='complex_017';p=original(cid)
 gpio(p,cid,13,7000,1);add(p,cid,'edges',pin=6,edge='both',from_ms=5500,to_ms=9500,max_count=0)
 add(p,cid,'serial',from_ms=5500,to_ms=9500,expected=12,tolerance=1,min_count=1)
 add(p,cid,'buzzer_bursts',pin=6,from_ms=1000,to_ms=3500,min_count=1)
 add(p,cid,'buzzer_bursts',pin=6,from_ms=12000,to_ms=17500,min_count=1)
 write(cid,p,'Audible output before mute and after resume, mute indicator, and continued distance reports.',('Saved active-HIGH button interface is retained.','The distance-to-beep-frequency curve is unspecified; this scenario verifies the mute/resume core.'))
 cid='advanced_002';p=original(cid)
 gpio(p,cid,10,3500,1)
 for t in [5500,6200,7000,7800,8500,20500,21500,22500]:add(p,cid,'tone',pin=6,at_ms=t,min=100)
 add(p,cid,'sample_relation',source='display',name='tm:4:5',at_ms=[4500,13500,19500,23500],relation='different',min_distinct=2)
 write(cid,p,'Approach guidance, sustained close-distance warning, and changing display.',('Thresholds other than the specified 5 cm warning are not prescribed.','Only these core paths are checked; occupancy count semantics remain unspecified.'),coverage='partial_core')
 cid='advanced_012';p=original(cid)
 # Endpoints exercise automatic response; the assignment of curtain angle to open/closed is unspecified.
 add(p,cid,'sample_relation',source='servo',pin=9,at_ms=[3000,6500],relation='different')
 add(p,cid,'sample_relation',source='servo',pin=9,at_ms=[6500,9500],relation='equal',tolerance=3)
 gpio(p,cid,10,3000,1)
 add(p,cid,'sample_relation',source='display',name='tm:4:5',at_ms=[3000,6500,9500],relation='different')
 write(cid,p,'Automatic bright/dark response, high-temperature override, and display response.',('Increasing LDR A0 represents increasing light.','The physical open/closed angle is unspecified; only relative behavior is checked.'),coverage='partial_core')
 # EmbedBench: pin bindings are taken from supplied circuit, not candidate code.
 for cid in ['embedbench_107','embedbench_63']:
  p=original(cid);name=p['shift_registers'][0]['displays'][0]['name'];p['horizon_ms']=23000
  for i in range(11):
   n=i%10;t=i*2000+500
   add(p,cid,'display',name=name,at_ms=t,expected=str(n),numeric=True)
   bar=list(range(4,14)) if cid.endswith('107') else list(range(5,14))+[44]
   add(p,cid,'gpio_count',pins=bar,at_ms=t,expected=n)
   if cid.endswith('107'):
    if i:
     gpio(p,cid,2,t,int(n%2==0));gpio(p,cid,3,t,int(n%2==1))
   else:
    color=(1,0,0) if n<=3 else (0,1,0) if n<=6 else (0,0,1)
    for pin,v in zip((2,3,4),color):gpio(p,cid,pin,t,v)
  write(cid,p,'Complete 0–9 cycle, wraparound, shift-register display, bar count and parity/range LEDs.')
 cid='embedbench_124';p=original(cid);name=p['shift_registers'][0]['displays'][0]['name']
 for t,n in [(500,0),(2200,9)]+[(t+800,i%10) for i,t in enumerate(range(3000,17000,1200))]:
  add(p,cid,'display',name=name,at_ms=t,expected=n,numeric=True)
  if t>500:gpio(p,cid,2,t,int(n%2==0))
 write(cid,p,'Decrement wraparound, repeated increment, wrap to zero and LED parity.')
 cid='embedbench_112';p=original(cid)
 for t,v1,v2 in [(1000,0,0),(3500,400,0),(6200,400,800),(8500,1023,800),(11500,1023,1023),(14500,0,1023)]:
  for pin,adc in [(2,v1),(3,v2),(5,v1),(6,v2),(8,v1),(9,v2)]:pwm(p,cid,pin,t,round(adc*255/1023))
  gpio(p,cid,4,t,int(v1==v2==1023))
  if v1==v2==1023:
   for pin in [7,10]:pwm(p,cid,pin,t,255)
 write(cid,p,'Independent potentiometer-to-brightness/color mapping, maximum-state white and indicator.')
 cid='embedbench_61';p=original(cid);name=p['shift_registers'][0]['displays'][0]['name']
 for t,v1,v2 in [(1000,0,0),(3500,1,339),(7000,512,0),(10500,512,1023),(14000,0,1023)]:
  red=int(v1*255/1023);blue=int(v2*255/1023)
  for pin,value in [(2,red),(3,red),(4,blue)]:pwm(p,cid,pin,t,value)
  # Only unambiguous endpoint brightness 0 is exact; intermediate scaling rounding unspecified.
  add(p,cid,'display',name=name,at_ms=t,expected=(2*red+blue)/3/28.33,numeric=True,tolerance=1)
 write(cid,p,'Both analog controls, PWM components, average-brightness digit with rounding allowance.')
 cid='embedbench_49';p=original(cid)
 for t,adc in [(2800,200),(5900,500),(9200,850),(13000,0)]:
  for pin in [8,9,10]:add(p,cid,'servo',pin=pin,at_ms=t,expected=round(adc*180/1023),tolerance=3)
  # Component behavior outside its stated ramp interval is not specified; check each ramp interior only.
  component,lo,hi=('R',0,341) if adc<=341 else ('G',342,682) if adc<=682 else ('B',683,1023)
  offsets={'R':0,'G':1,'B':2}
  for base in [2,5]:pwm(p,cid,base+offsets[component],t,round((adc-lo)*255/(hi-lo)),4)
 write(cid,p,'Three servo mappings and each explicitly specified RGB component ramp; undefined other components unconstrained.')
 cid='embedbench_110';p=original(cid)
 colors=[(1,0,0),(0,1,0),(0,0,1),(1,1,0),(0,1,1),(1,0,1),(1,1,1)]
 for i in range(8):
  t=i*2000+1200;gpio(p,cid,2,t,i%2)
  for base in [3,6]:
   for offset,v in enumerate(colors[i%7]):pwm(p,cid,base+offset,t,255*v)
  if i:
   for pin,n in [(9,180 if i%2 else 0),(10,0 if i%2 else 180),(11,135 if i%2 else 90)]:add(p,cid,'servo',pin=pin,at_ms=t,expected=n,tolerance=3)
 write(cid,p,'Two-second coordinated light/color cycle and servo endpoint alternation.',('The detailed servo-specific initial angles take precedence over the contradictory all-servos-zero introductory sentence.','Endpoint checks allow 1.2 seconds after a scan begins; strict scan trajectory remains separately inspectable.'))
 cid='embedbench_120';p=original(cid)
 for t,adc in [(1000,512),(3000,512),(18000,850),(21000,850)]:pwm(p,cid,2,t,round(adc*255/1023))
 add(p,cid,'sample_relation',source='pwm',pin=2,at_ms=list(range(5000,15000,250)),relation='different',min_distinct=10)
 for t in range(5000,15000,250):add(p,cid,'pwm',pin=2,at_ms=t,min=round(220*255/1023)-3,max=round(820*255/1023)+3)
 write(cid,p,'Manual mapping, automatic brightness variation within two-pot bounds, and return to manual.',('Automatic transition duration is retained for separate trajectory review; the core scenario verifies mode switching and bounded variation.',))
 # Remaining advanced questions have incomplete interfaces; execute explicit core
 # paths and disclose the missing settings rather than treating a trace as a pass.
 for cid in ['advanced_020','advanced_021','advanced_022','advanced_025','advanced_028','advanced_029']:
  p=blank(40000)
  assumptions=['Raw stated TM1637 D4/D5 wiring is retained. Several original questions assign that same pin to another device; no silent rewiring is performed.','These scenarios cover explicit observable core paths; unspecified thresholds, configuration UI and command grammar prevent complete conformance coverage.']
  if cid=='advanced_020':
   for pin in [2,3,4]:input_(p,pin,0,0,'digital')
   input_(p,'A0',0,0);pulse(p,3,2000,True)
   add(p,cid,'serial',from_ms=2000,to_ms=29000,contains='movement complete',min_count=1)
   add(p,cid,'display_activity',name='tm:4:5',from_ms=0,to_ms=29000,min_distinct=2)
   input_(p,'A0',30000,1023);pulse(p,4,32000,True);add(p,cid,'buzzer_bursts',pin=6,from_ms=31000,to_ms=39000,min_count=1)
   assumptions.append('A0=0 and 1023 exercise empty and maximal load; the load calibration and overload threshold are unspecified.')
  elif cid=='advanced_021':
   input_(p,2,0,0,'digital');input_(p,'A0',0,0);input_(p,'A2',0,1023);input_(p,'A1',0,20)
   input_(p,'A1',8000,164);input_(p,'A0',8000,1023);input_(p,'A2',16000,0)
   pulse(p,2,20000,True);pulse(p,2,26000,True)
   gpio(p,cid,10,5000,1);gpio(p,cid,10,13000,0);gpio(p,cid,5,22000,0)
   add(p,cid,'buzzer_bursts',pin=6,from_ms=18000,to_ms=25000,min_count=1)
   add(p,cid,'display_activity',name='tm:4:5',from_ms=0,to_ms=39000,min_distinct=2)
   assumptions.append('LM35 bench temperatures are approximately 10 and 80 C; A2 low means low water. Exact application setpoints were not supplied.')
  elif cid=='advanced_022':
   p['horizon_ms']=155000
   for pin,v in [(2,0),(3,0),(7,1)]:input_(p,pin,0,v,'digital')
   p['serial_inputs']=[dict(at_ms=1000,text='1 2\n')]
   input_(p,3,64000,1,'digital');input_(p,2,65000,1,'digital');pulse(p,7,148000,False)
   add(p,cid,'edges',pin=12,edge='both',from_ms=3000,to_ms=58000,min_count=3)
   gpio(p,cid,13,80000,1)
   add(p,cid,'serial',from_ms=120000,to_ms=147000,contains='overtime use',min_count=1)
   gpio(p,cid,13,153000,0)
   assumptions.append('The undisclosed reservation grammar is exercised as two whitespace-delimited minute values, start=1 and end=2; parser incompatibility is an interface uncertainty, not alone proof of semantic failure.')
  elif cid=='advanced_025':
   p['horizon_ms']=65000
   for pin in [2,3,4]:input_(p,pin,0,0,'digital')
   input_(p,'A1',0,20);pulse(p,2,1000,True);pulse(p,3,2500,True);pulse(p,4,4000,True);input_(p,'A1',8000,164)
   add(p,cid,'display_activity',name='tm:4:5',from_ms=0,to_ms=64000,min_distinct=3)
   add(p,cid,'serial',from_ms=1000,to_ms=64000,min_count=3)
   add(p,cid,'buzzer_bursts',pin=6,from_ms=8000,to_ms=64000,min_count=1)
   assumptions.append('Button roles, target defaults and stage durations are undisclosed; a 65-second exploratory budget cannot prove absence of a longer valid process.')
  elif cid=='advanced_028':
   p['horizon_ms']=20000
   for pin in [2,3,4,7]:input_(p,pin,0,0,'digital')
   pulse(p,2,2000,True);pulse(p,3,5000,True);pulse(p,7,10000,True);pulse(p,7,15000,True)
   add(p,cid,'buzzer_bursts',pin=6,from_ms=3000,to_ms=9000,min_count=1)
   add(p,cid,'serial',from_ms=10000,to_ms=19000,min_count=2)
   add(p,cid,'display_activity',name='tm:4:5',from_ms=0,to_ms=19000,min_distinct=2)
   gpio(p,cid,13,18500,0)
  else:
   input_(p,'A0',0,41);input_(p,'A1',0,20);input_(p,2,0,0,'digital')
   input_(p,'A0',8000,164);input_(p,'A1',14000,0);input_(p,2,20000,1,'digital');input_(p,2,32000,0,'digital')
   add(p,cid,'serial',from_ms=10000,to_ms=19000,min_count=2)
   add(p,cid,'buzzer_bursts',pin=6,from_ms=10000,to_ms=39000,min_count=1)
   add(p,cid,'display_activity',name='tm:4:5',from_ms=0,to_ms=39000,min_distinct=2)
   assumptions.append('High-temperature and long-open-door thresholds are not specified, so alarm timing remains conditional on the implementation interface.')
  write(cid,p,'Executed exploratory core scenario with explicitly disclosed interface uncertainty.',assumptions,coverage='partial_core_interface_uncertain')
 print('Wrote 27 requirement-derived supplementary plans.',flush=True)
if __name__=='__main__':main()
