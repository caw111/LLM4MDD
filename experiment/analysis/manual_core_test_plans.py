"""Additional requirement-derived checks, never overwrite the retained suite."""
import copy
from datetime import datetime
import json
from pathlib import Path


def make_plan(case,frozen):
    plan=copy.deepcopy(frozen)
    checks=[]
    def sample(kind,t,key,value,tolerance=None):
        c=dict(kind=kind,id=f'{kind}_{key}_{t}',at_ms=t,expected=value,
               requirement_quote='Original task primary behavior, independently checked after manual sketch correction')
        c['name' if kind=='display' else 'pin']=key
        if tolerance is not None:c['tolerance']=tolerance
        checks.append(c)
    root=Path(__file__).resolve().parents[2]/'experiment/output'
    if case in ('embedbench_41','embedbench_102'):
        return json.loads((root/'core_repair_offline_round1'/case/'full_core_plan.json').read_text(encoding='utf-8'))
    if case in ('embedbench_33','embedbench_35','embedbench_38'):
        origin='core_repair_offline_round1' if case=='embedbench_38' else 'core_repair_offline_round2'
        return json.loads((root/origin/case/'test_plan.json').read_text(encoding='utf-8'))
    if case in ('advanced_003','advanced_005','advanced_007','advanced_008','advanced_011','advanced_013','advanced_014',
                'advanced_015','advanced_017','advanced_019','advanced_023','advanced_026',
                'advanced_027','complex_006','complex_007','complex_012',
                'complex_018','complex_019','complex_021','complex_022',
                'complex_026','complex_027','embedbench_47','embedbench_75','embedbench_84','embedbench_85'):
        plan.update(horizon_ms=12000,input_steps=[],serial_inputs=[])

        def input_values(kind,pin,values):
            plan['input_steps'].extend(dict(kind=kind,pin=pin,at_ms=t,value=value) for t,value in values)

        def buttons(pin,starts):
            input_values('digital',pin,[(0,1)]+[(t,state) for start in starts for t,state in
                         ((start,0),(start+8,1),(start+15,0),(start+240,1))])

        def serial(key,start,stop,pattern,count=1):
            checks.append(dict(kind='serial',id=key,from_ms=start,to_ms=stop,regex=pattern,
                               min_count=count,max_count=count,requirement_quote='Required serial output for the scenario'))

        if case=='advanced_003':
            input_values('digital',2,[(0,0),(1000,1),(2300,0),(3200,1),(4200,0),(5400,1),(6800,0)])
            input_values('digital',3,[(0,0),(1000,1)])
            input_values('analog','A0',[(0,800),(4100,100)])
            plan['serial_inputs']=[dict(at_ms=t,text=data) for t,data in
                ((250,'STATUS\n'),(1800,'ARM\n'),(3500,'STATUS'),(3800,'DISARM\n'),
                 (4300,'STATUS\n'),(4700,'RESET\n'),(6300,'STATUS\n'))]
            for t,count,alarm in ((1250,0,0),(2100,1,1),(2550,1,0),(3500,2,1),
                                  (4000,2,0),(5000,0,0),(5850,1,1),(7100,1,0)):
                sample('display',t,'tm:4:5',str(count))
                sample('gpio',t,13,alarm)
            for t,freq in ((2100,1000),(2550,0),(3300,1000),(4150,0),(6100,1000)):
                sample('tone',t,6,freq)
            for t,label,count in ((250,'DISARMED',0),(3500,'ARMED',2),(4300,'DISARMED',2),(6300,'NIGHT',1)):
                serial(f'mode_{t}',t,t+250,f'^mode: {label}$')
                serial(f'event_count_{t}',t,t+250,f'^events: {count}$')
        elif case=='advanced_005':
            plan['horizon_ms']=18500
            input_values('temperature',7,[(0,24),(4000,35),(8000,24)])
            input_values('humidity',7,[(0,53),(4000,48)])
            input_values('temperature',8,[(0,21),(8000,34),(14000,20)])
            input_values('humidity',8,[(0,65),(8000,56)])
            input_values('analog','A0',[(0,300),(4000,900)])
            input_values('analog','A1',[(0,500),(8200,950)])
            buttons(2,[6500])
            buttons(3,[9500])
            for t,value in ((250,'24'),(2450,'53'),(4550,'900'),(6250,'21'),(6750,'35'),(9750,'48'),(18100,'20')):
                sample('display',t,'tm:4:5',value)
            for t,a,b in ((300,0,0),(4500,1,0),(8700,0,1),(14500,0,0)):
                sample('gpio',t,12,a)
                sample('gpio',t,13,b)
            labels=('living_room_temperature_c','living_room_humidity_percent','living_room_light_adc',
                    'bedroom_temperature_c','bedroom_humidity_percent','bedroom_light_adc')
            for t,values in ((5000,(35,48,900,21,65,500)),(10000,(24,48,900,34,56,950)),(15000,(24,48,900,20,56,950))):
                for label,value in zip(labels,values):
                    serial(f'{label}_{t}',t,t+250,rf'^{label}{value}(?:\.0+)?$')
            serial('living_room_alert',4000,4250,r'^living_room_alarm1$')
            serial('bedroom_alert',8200,8450,r'^bedroom_alarm1$')
        elif case=='advanced_007':
            plan['horizon_ms']=18500
            input_values('analog','A1',[(0,8),(2000,22),(7500,14),(10000,25),(14000,8)])
            input_values('digital',2,[(0,0),(1000,1),(12300,0),(15500,1),(17500,0)])
            for t,value in ((250,0),(2400,1),(7800,1),(14400,0)):
                sample('gpio',t,10,value)
            for t,freq in ((9800,0),(10950,0),(11250,1000),(11950,1000),(12550,0),(17000,0)):
                sample('tone',t,6,freq)
            for t,value in ((11250,1),(11750,0),(12550,0)):
                sample('gpio',t,13,value)
            for t,value in ((4500,'10'),(5500,'10'),(6400,'5'),(13500,'12'),(14500,'0')):
                sample('display',t,'tm:4:5',value)
            for t in (2000,3000,4000,5000):
                serial(f'warning_{t}',t,t+200,r'^critical cooling loss:')
            serial('no_warning_while_closed',12400,15300,r'^critical cooling loss:',0)
            plan['experimental_condition']='The unspecified cooling band is 6–8 Celsius. The normally closed door contact grounds D2 when shut.'
        elif case=='advanced_008':
            plan['horizon_ms']=16000
            buttons(2,[350,2050])
            buttons(3,[950,1550])
            buttons(7,[3000,4350,6300])
            for t,value in ((250,'2'),(650,'3'),(1250,'2'),(2350,'3'),(3250,'3'),
                            (4550,'2'),(5500,'2'),(6575,'2'),(8200,'2'),(10100,'3'),(13100,'0')):
                sample('display',t,'tm:4:5',value)
            for t,value in ((7100,0),(8050,1000),(8250,0),(10050,1000),(10200,0),(10350,1000),
                            (13050,1000),(13350,1000),(13650,1000),(14100,0)):
                sample('tone',t,6,value)
            sample('gpio',12500,13,0)
            sample('gpio',13400,13,1)
            serial('completed',12950,13200,r'^process complete$')
            for i,value in enumerate((3000,2000,3000),1):
                checks.append(dict(kind='serial',id=f'stage_{i}_duration',prefix=f'stage{i}_elapsed_ms',
                    expected=value,tolerance=20,from_ms=12950,to_ms=13200,min_count=1,max_count=1,
                    requirement_quote='Report each stage elapsed time excluding pauses.'))
        elif case=='advanced_011':
            input_values('analog','A1',[(0,61),(2700,105),(5100,61)])
            input_values('analog','A0',[(0,100),(3600,730),(5100,100)])
            input_values('digital',2,[(0,0),(6400,1),(8600,0)])
            for t,red,green,tone in ((250,0,1,0),(2950,0,0,0),(4200,1,0,1000),(4700,1,0,0),
                                     (5700,0,1,0),(7200,1,0,1000),(9300,0,1,0)):
                sample('gpio',t,13,red)
                sample('gpio',t,12,green)
                sample('tone',t,6,tone)
            for t,value in ((250,'0'),(3100,'51'),(4300,'730'),(6700,'2'),(8900,'29')):
                sample('display',t,'tm:4:5',value)
            serial('normal_and_attention_silent',0,3500,r'^fire alert$',0)
            serial('both_sensors_trigger',3600,3850,r'^fire alert$')
            serial('flame_alone_triggers',6400,6650,r'^fire alert$')
            serial('recovered_silent',8800,12000,r'^fire alert$',0)
        elif case=='advanced_013':
            plan['horizon_ms']=67000
            input_values('digital',2,[(0,1)])
            input_values('digital',3,[(0,1)])
            input_values('digital',7,[(0,0),(1000,1),(2600,0)])
            plan['serial_inputs']=[dict(at_ms=t,text=text) for t,text in ((500,'QUERY\n'),(4100,'RESET\n'),(4500,'QUERY'))]
            for t,value in ((250,2),(1300,3),(3000,2),(65000,2)):
                sample('display',t,'tm:4:5',str(value))
                sample('gpio',t,13,int(value==3))
            sample('tone',1150,6,1000)
            sample('tone',1450,6,0)
            for t in (500,4500):
                for level,value in enumerate((1,1,0),1):
                    serial(f'query_{t}_{level}',t,t+250,f'^level{level}_occupied{value}$')
            serial('timer_reset_cancels_original_deadline',0,64000,r'^restock needed$',0)
            serial('vacancy_after_new_deadline',64100,64500,r'^restock needed$')
            serial('no_duplicate_vacancy',64500,67000,r'^restock needed$',0)
        elif case=='advanced_014':
            plan['horizon_ms']=19000
            input_values('analog','A0',[(0,700),(10200,100)])
            input_values('digital',2,[(0,0),(9000,1),(12400,0)])
            buttons(3,[16200])
            for t,value in ((250,'1'),(8000,'1'),(9250,'2'),(10450,'3'),(14000,'3'),
                            (14700,'4'),(16500,'4')):
                sample('display',t,'tm:4:5',value)
            for t,value in ((13900,0),(15100,1000),(15600,0),(16100,1000),(16600,0),(17100,0)):
                sample('tone',t,6,value)
                sample('gpio',t,13,int(value!=0))
            serial('fault_stagnant_high_water',8000,8350,r'^wash fault1$')
            serial('no_duplicate_fault',8400,19000,r'^wash fault',0)
        elif case=='advanced_015':
            plan['ultrasonic']=[dict(trigger_pin=8,echo_pin=9)]
            input_values('distance',9,[(0,15),(5100,42),(7200,19),(9100,None)])
            input_values('digital',2,[(0,0),(600,1),(4400,0),(5500,1),(6100,0),(7800,1),(8200,0)])
            plan['serial_inputs']=[dict(at_ms=t,text='STATUS\n') for t in (250,4800,6500,8500,10500)]
            for t,valve,alarm in ((300,0,0),(850,1,0),(3100,1,0),(3900,0,0),(4500,0,0),
                                 (5800,0,1),(8000,1,0),(9500,0,1)):
                sample('gpio',t,10,valve)
                sample('gpio',t,13,alarm)
            for t,value in ((250,'75'),(2300,'1'),(4550,'75'),(7550,'1'),(8550,'65')):
                sample('display',t,'tm:4:5',value)
            for t,uses,percent in ((250,0,75),(4800,1,75),(6500,1,7.5),(8500,2,65),(10500,2,0)):
                serial(f'uses_{t}',t,t+250,f'^today_uses{uses}$')
                checks.append(dict(kind='serial',id=f'level_{t}',prefix='remaining_percent',
                    expected=percent,tolerance=.3,from_ms=t,to_ms=t+250,min_count=1,max_count=1,
                    requirement_quote='Return the current water level percentage on request.'))
        elif case=='advanced_017':
            plan['horizon_ms']=15700
            buttons(2,[500,4100,10300])
            buttons(3,[1300,2600,11900])
            buttons(7,[5800,13400])
            buttons(8,[14900])
            input_values('digital',9,[(0,1),(1900,0),(2200,1),(3500,0),(3800,1),(4800,0),(5100,1),
                                      (6800,0),(10100,1),(11200,0),(11500,1),(12700,0),(13000,1),
                                      (14200,0),(14500,1)])
            for t,value in ((250,0),(650,1),(1600,1),(2300,0),(2800,2),(4300,1),(6000,3),
                            (10500,1),(12100,2),(13600,3),(15100,4)):
                sample('display',t,'tm:4:5',str(value))
                sample('gpio',t,13,int(value!=0))
            sample('tone',650,6,1000)
            sample('tone',900,6,0)
            for t,player,wins in ((500,1,1),(2600,2,1),(4100,1,2),(5800,3,1),(10300,1,1),
                                  (11900,2,1),(13400,3,1),(14900,4,1)):
                serial(f'player_{t}',t,t+250,rf'^player_id{player}$')
                serial(f'timestamp_{t}',t,t+250,rf'^timestamp_ms{t//100}[0-9]{{2}}$')
                serial(f'wins_{t}',t,t+250,rf'^player wins: {wins}$')
            serial('ignored_when_locked',1000,1800,r'^player_id',0)
            serial('no_clear_on_tap',1900,9500,r'^stats cleared$',0)
            serial('clear_after_three_seconds',9800,10100,r'^stats cleared$')
        elif case=='advanced_019':
            plan['ultrasonic']=[dict(trigger_pin=8,echo_pin=9),dict(trigger_pin=10,echo_pin=11)]
            input_values('distance',9,[(0,100),(1100,38),(4800,20),(7100,None),(9500,65)])
            input_values('distance',11,[(0,100),(3000,25),(4800,12),(9500,90)])
            for t,level,red in ((250,0,0),(1450,0,0),(3300,1,1),(3750,1,0),
                                (5300,2,1),(5700,2,0),(7550,0,0),(10000,0,0)):
                sample('display',t,'tm:4:5',str(level))
                sample('gpio',t,12,red)
                sample('gpio',t,13,int(level==0))
                sample('tone',t,6,1000 if level else 0)
            checks.append(dict(kind='serial',id='event_duration',prefix='danger_event_duration_ms',
                expected=4100,tolerance=250,from_ms=7100,to_ms=7500,min_count=1,max_count=1,
                requirement_quote='Report the most recent hazard duration when the event ends.'))
            serial('no_second_event',7600,12000,r'^danger_event_duration_ms',0)
        elif case=='advanced_023':
            input_values('analog','A1',[(0,10),(2300,22),(4800,10)])
            input_values('digital',2,[(0,0),(1300,1),(1600,0),(6100,1),(6350,0)])
            input_values('digital',3,[(0,0),(3500,1),(5500,0),(8100,1),(8950,0)])
            plan['serial_inputs']=[dict(at_ms=t,text=message) for t,message in
                                   ((500,'GPS:31.25,121.75\n'),(5700,'GPS:12.5,-10.125\n'),(7600,'GPS:100,0\n'))]
            for t,value in ((250,'4'),(2600,'2'),(4200,'10'),(6500,'4'),(8350,'4'),(10250,'5')):
                sample('display',t,'tm:4:5',value)
            for t,value in ((300,0),(1500,0),(2500,1000),(5300,1000),(5900,0),(8500,1000),(9300,0)):
                sample('tone',t,6,value)
                sample('gpio',t,13,int(value!=0))
            for t,label,coordinates in ((1300,'shock',r'31\.2500,121\.7500'),(2300,'temperature high',r'31\.2500,121\.7500'),
                                         (3500,'box opened',r'31\.2500,121\.7500'),(6100,'shock',r'12\.5000,-10\.1250'),
                                         (8100,'box opened',r'12\.5000,-10\.1250')):
                serial(f'event_{t}',t,t+250,rf'^{label} GPS: {coordinates}$')
            serial('no_event_during_stable_measurements',3700,4700,r' GPS:',0)
        elif case=='advanced_026':
            plan['horizon_ms']=17500
            input_values('digital',2,[(0,0),(900,1),(1500,0),(10200,1),(10900,0),(15800,1),(16200,0)])
            input_values('digital',3,[(0,0),(3000,1),(3900,0),(8200,1),(8800,0)])
            input_values('analog','A0',[(0,200),(7400,800),(9700,200)])
            for t,state in ((250,0),(1150,255),(3200,255),(6600,255),(7150,0),(8450,0),(10450,255),(14250,0),(16050,255)):
                sample('pwm',t,9,state)
            for t,count in ((250,0),(1200,0),(3300,1),(7300,1),(10450,2),(16500,2)):
                sample('display',t,'tm:4:5',str(count))
            for t,value in ((5000,4100),(10000,6300),(15000,10200)):
                checks.append(dict(kind='serial',id=f'elapsed_{t}',prefix='illuminated_on_time_ms',
                    expected=value,tolerance=120,from_ms=t,to_ms=t+200,min_count=1,max_count=1,
                    requirement_quote='Report only time during which the hall light was enabled.'))
        elif case=='advanced_027':
            plan['horizon_ms']=26000
            buttons(2,[4200,7300])
            input_values('analog','A0',[(0,900),(2100,100),(5200,900),(9200,500)])
            input_values('analog','A1',[(0,100),(2100,900),(5200,100),(9200,510)])
            input_values('analog','A2',[(0,820),(2400,100),(3300,820),(11000,500),(23500,900)])
            for t,value in ((250,45),(2350,135),(5750,135),(7550,45),(9450,90)):
                sample('servo',t,9,value,1)
            for t,value in ((250,'40'),(2800,'0'),(3800,'1'),(4400,'40'),(22200,'0'),(24400,'43')):
                sample('display',t,'tm:4:5',value)
            for t,value in ((3000,0),(20750,0),(21300,1000),(23750,0)):sample('tone',t,6,value)
            serial('no_transient_low_warning',0,20500,r'^low charge$',0)
            serial('persistent_low_warning',21000,21250,r'^low charge$')
            serial('no_duplicate_warning',21300,26000,r'^low charge$',0)
        elif case=='complex_006':
            input_values('analog','A0',[(0,819),(4000,512),(7000,1023)])
            input_values('temperature',7,[(0,32),(8000,None),(10000,25)])
            for t,value in ((300,'32'),(2400,'40'),(4300,'32'),(6300,'25'),(10300,'50'),(11300,'50')):
                sample('display',t,'tm:4:5',value)
            for t,active,freq in ((300,0,0),(4300,1,1000),(4600,1,0),(5200,1,1000),(7250,0,0),(8500,0,0)):
                sample('gpio',t,13,active)
                sample('tone',t,6,freq)
            plan['experimental_condition']='The requirement leaves threshold range and alarm cadence open. This implementation uses 0–50 Celsius and repeats a 400 ms pulse each second.'
        elif case=='complex_007':
            buttons(2,[7200])
            plan['serial_inputs']=[dict(at_ms=t,text=command+'\n') for t,command in
                ((500,'ON'),(1500,'PWM:177'),(2500,'STATUS'),(3000,'BLINK'),(5200,'BUZZ'),(6400,'OFF'),
                 (8200,'PWM:999'),(9000,'PWM:0'))]
            for t,state in ((250,0),(750,1),(3200,1),(3750,0),(4200,1),(6600,0)):
                sample('gpio',t,13,state)
            for t,value in ((1750,177),(7500,177),(8500,177),(9250,0)):
                sample('pwm',t,9,value)
            sample('tone',5350,6,1000)
            sample('tone',5600,6,0)
            for t,state in ((2500,1),(7200,0)):
                serial(f'status_led_{t}',t,t+250,f'^LED_STATE{state}$')
                serial(f'status_pwm_{t}',t,t+250,'^PWM_VALUE177$')
        elif case=='complex_012':
            input_values('analog','A0',[(0,1023),(6200,512)])
            buttons(2,[1000,4200,7600])
            for t,value in ((300,'1000'),(4500,'1000'),(6400,'550')):
                sample('display',t,'tm:4:5',value)
            for t,state in ((300,0),(1250,1),(1750,0),(2250,1),(2750,0),(4500,0),(6800,0),(7750,1),(8000,0)):
                sample('gpio',t,13,state)
            plan['experimental_condition']='The requirement leaves the blink period range open. This implementation displays the full cycle in milliseconds, from 100 to 1000.'
        elif case=='complex_018':
            input_values('analog','A1',[(0,41),(4100,68),(6900,82),(8700,41)])
            plan['serial_inputs']=[dict(at_ms=t,text=text) for t,text in
                ((600,'SET_FAN_MIN:64\n'),(1600,'SET_THRESHOLD:25\n'),(5600,'SET_THRESHOLD:35'),
                 (7300,'SET_FAN_MIN:130\n'),(9300,'SET_FAN_MIN:999\n'),(10300,'RESET\n'))]
            for t,value in ((250,0),(900,64),(2100,64),(4500,269),(6000,64),(7100,191),(7650,255),
                            (9000,130),(9600,130),(10600,0)):
                sample('pwm',t,5,min(255,value),3)
            for t,value in ((250,0),(4500,1),(6000,0),(7200,1),(9000,0)):sample('gpio',t,13,value)
            serial('first_overheat',4100,4350,r'^overheat_temperature')
            serial('later_overheat',6900,7150,r'^overheat_temperature')
            serial('clear_after_cooldown',8700,12000,r'^overheat_temperature',0)
        elif case=='complex_019':
            plan['horizon_ms']=28000
            buttons(2,[500,1300,26000])
            buttons(3,[2300,8200])
            for t,value in ((300,'0'),(800,'10'),(1600,'20'),(2650,'20'),(3500,'19'),(8600,'14'),
                            (21500,'1'),(22500,'0'),(26400,'10')):
                sample('display',t,'tm:4:5',value)
            for t,value in ((22300,0),(22400,1000),(24300,1000),(24400,0),(26500,0)):
                sample('tone',t,6,value)
        elif case=='complex_021':
            input_values('temperature',7,[(0,25),(4000,None),(8000,27)])
            input_values('humidity',7,[(0,63),(4000,None),(8000,51)])
            for t,value in ((250,'25'),(2250,'63'),(4250,'0000'),(6250,'0000'),(8250,'27'),(10250,'51')):
                sample('display',t,'tm:4:5',value)
            for t,value in ((250,0),(4250,1),(4750,0),(5250,1),(8250,0)):
                sample('gpio',t,13,value)
            serial('error_after_invalid_reading',4000,4250,'^sensor error$')
            serial('no_error_after_recovery',8000,12000,'^sensor error$',0)
        elif case=='complex_022':
            buttons(2,[600,2400,7900])
            buttons(3,[1300,4700])
            for t,value in ((250,'0'),(850,'1'),(2700,'2'),(8100,'0')):
                sample('display',t,'tm:4:5',value)
            for t,value in ((250,0),(850,64),(1600,80),(2100,80),(2940,127),(3440,255),(3940,127),
                            (5125,127),(5500,255),(5875,127),(8100,0)):
                sample('pwm',t,9,value,4)
        elif case=='complex_026':
            plan['horizon_ms']=14200
            buttons(2,[3300,9700])
            input_values('temperature',7,[(0,21),(4000,28),(8000,25)])
            input_values('humidity',7,[(0,62),(4000,47),(8000,54)])
            plan['serial_inputs']=[dict(at_ms=t,text=text) for t,text in
                                   ((250,'TEMP\n'),(2050,'HUMI'),(6050,'BOTH\n'),(11050,'TEMP\n'))]
            for t,value in ((600,'21'),(2300,'62'),(4300,'47'),(6300,'28'),(8300,'54'),
                            (10300,'25'),(11300,'25'),(13500,'25')):
                sample('display',t,'tm:4:5',value)
            serial('silent_initial',0,3200,r'^(TEMP|HUMI)',0)
            serial('enabled_reports_temperature',3300,3600,r'^TEMP21(?:\.0+)?$')
            serial('enabled_reports_humidity',3300,3600,r'^HUMI62(?:\.0+)?$')
            serial('updated_temperature',8000,8800,r'^TEMP25(?:\.0+)?$')
            serial('updated_humidity',8000,8800,r'^HUMI54(?:\.0+)?$')
            serial('silent_after_toggle',10000,14000,r'^(TEMP|HUMI)',0)
        elif case=='complex_027':
            plan.update(horizon_ms=26500,ultrasonic=[dict(trigger_pin=8,echo_pin=9)])
            input_values('temperature',7,[(0,24),(5400,28),(11400,26)])
            input_values('distance',9,[(0,35),(2500,10),(5400,18),(8500,14),(11400,None),
                                        (14500,12),(17500,20),(20500,12),(23500,15)])
            for t,active in ((250,0),(3250,0),(6250,0),(9250,1),(12250,0),(15250,0),(21250,0),(24250,0)):
                sample('gpio',t,13,active)
            for n,(temperature,distance) in enumerate(((24,35),(24,10),(28,18),(28,14),(26,None),
                                                       (26,12),(26,20),(26,12),(26,15))):
                t=n*3000
                serial(f'temperature_{t}',t,t+200,rf'^Temperature{temperature}(?:\.0+)?$')
                pattern=rf'^Distance{distance}(?:\.0+)?$' if distance is not None else r'(?i)^Distancenan$'
                serial(f'distance_{t}',t,t+200,pattern)
            serial('persistent_on_third',9000,9250,'^persistent object detected$')
            serial('no_false_persistent',12000,26000,'^persistent object detected$',0)
        elif case=='embedbench_47':
            inputs=(0,85,170,255,341,425,511,597,682,767,852,937,1023)
            plan['horizon_ms']=13500
            input_values('analog','A3',[(n*1000,v) for n,v in enumerate(inputs)])
            input_values('analog','A4',[(n*1000,v) for n,v in enumerate(reversed(inputs))])
            for n,pair in enumerate(zip(inputs,reversed(inputs))):
                for group,adc in enumerate(pair):
                    h=adc*6/1023
                    x=255*(1-abs(h%2-1))
                    rgb=((255,x,0),(x,255,0),(0,255,x),(0,x,255),(x,0,255),(255,0,x))[int(h)%6]
                    for i,value in enumerate(rgb):
                        sample('pwm',n*1000+200,2+3*group+i,int(value),1)
        elif case=='embedbench_75':
            input_values('digital',2,[(0,1),(500,0),(600,1),(1500,0),(1800,1),(4000,0),(4250,1),
                                      (6800,0),(7050,1)])
            input_values('analog','A3',[(0,1023),(2800,512),(6100,0),(8500,1023)])
            input_values('analog','A4',[(0,512),(5000,1023),(7500,0)])
            for t,count in ((250,0),(800,0),(1600,0),(1750,10),(3050,5),(4500,5),
                            (5250,10),(6450,10),(7150,0),(8750,10)):
                for i,pin in enumerate(range(3,13)):sample('gpio',t,pin,int(i<count))
        elif case=='embedbench_84':
            buttons(2,[500,1700,2900,4100,8300])
            buttons(3,[7100])
            input_values('analog','A3',[(0,1023),(5300,512)])
            input_values('analog','A4',[(0,512),(5300,1023)])
            input_values('analog','A5',[(0,256),(5300,0)])
            for t,values in ((250,(0,0,0)),(800,(255,0,0)),(2000,(0,127,0)),(3200,(0,0,63)),
                             (4400,(255,127,63)),(5500,(127,255,0)),(7400,(0,0,0)),(8600,(127,0,0))):
                for pin,value in zip((4,5,6),values):sample('pwm',t,pin,value,1)
        else:
            buttons(2,[500,1400,2300,9000,9950,11000])
            buttons(3,[3200,4100,5000,5900,6800,11000])
            input_values('analog','A3',[(0,1023),(7000,512),(8100,0),(8800,768)])
            for t,mask,level in ((250,1,255),(800,2,255),(1700,4,255),(2600,1,255),(3500,0,255),
                                 (4400,3,255),(5300,6,255),(6200,5,255),(7150,3,127),(8350,3,0),
                                 (9300,0,191),(10250,1,191),(11300,0,191)):
                for i,pin in enumerate((4,5,6)):sample('pwm',t,pin,level if mask&(1<<i) else 0,1)
    elif case=='medium_010':
        plan['input_steps']=[dict(event,pin='A0') if event['kind']=='analog' and event['pin']=='A1' else event
                             for event in plan['input_steps']]
        plan.pop('reference_cpp',None)
        plan['experimental_condition']='The original requirement connects the potentiometer to A0. The retained plan stimulates A1.'
        return plan
    elif case=='complex_024':
        checks=copy.deepcopy(plan['checks'])
        presses=[event['at_ms'] for event in plan['input_steps'] if event['kind']=='digital' and event['value']==0]
        for index,at in enumerate(presses,1):
            adc={event['pin']:event['value'] for event in sorted(plan['input_steps'],key=lambda event:event['at_ms'])
                 if event['kind']=='analog' and event['at_ms']<=at}
            for label,value,tolerance in (('light',adc['A0'],0),('temp',adc['A1']*500/1023,.02),('count',index,0)):
                checks.append(dict(kind='serial',id=f'{label}_{index}',prefix=label,expected=value,tolerance=tolerance,
                    from_ms=at,to_ms=at+250,min_count=1,max_count=1,
                    requirement_quote='Report the current light, LM35 temperature and sample count on each button press'))
    elif case=='embedbench_10':
        plan.update(horizon_ms=13000,input_steps=[])
        for at,adc in ((0,0),(2000,512),(4000,256),(8000,1023),(10000,512)):
            plan['input_steps'].append(dict(at_ms=at,pin='A3',value=adc,kind='analog'))
        for at,value in ((0,1),(6000,0),(6400,1),(11000,0),(11400,1)):
            plan['input_steps'].append(dict(at_ms=at,pin=12,value=value,kind='digital'))
        for at,count,reverse in ((500,0,False),(2500,5,False),(4500,2,False),(6700,2,True),
                                 (8500,10,True),(10500,5,True),(11800,5,False)):
            for index,pin in enumerate(range(2,12)):sample('gpio',at,pin,int(count>(index if reverse else 9-index)))
    elif case in ('embedbench_11','embedbench_13','embedbench_14','embedbench_67','embedbench_125'):
        plan.update(horizon_ms=16500,input_steps=[])
        if case in ('embedbench_11','embedbench_13','embedbench_14'):
            display_fixture='embedbench_11' if case=='embedbench_13' else case
            native=json.loads((root/'behavior_all296_20261023/embed_native'/display_fixture/'native_plan.json').read_text(encoding='utf-8'))
            plan['shift_registers']=native['shift_registers']
        if case=='embedbench_11':
            for n in range(8):
                sample('display',n*2000+250,'sevseg1',('0','A','P')[n%3])
                sample('display',n*2000+1750,'sevseg1',('0','A','P')[n%3])
        elif case=='embedbench_13':
            plan['horizon_ms']=60500
            for pin in (2,3,4):sample('gpio',250,pin,0)
            sample('display',250,'sevseg1','0')
            times=[n*2000+offset for n in range(31) for offset in (250,450)]
            checks.extend([dict(kind='display_gpio_count',id='display_matches_every_count',name='sevseg1',
                pins=[2,3,4],at_ms=times,requirement_quote='Display the actual number of selected LEDs.'),
                dict(kind='distinct_gpio_patterns',id='select_varied_patterns',pins=[2,3,4],at_ms=times,min=5,
                     requirement_quote='Select varied subsets across successive intervals.')])
        elif case=='embedbench_14':
            sample('display',250,'sevseg1',' ')
            for n in range(1,16):sample('display',n*1000+250,'sevseg1',str(n*2%10))
        elif case=='embedbench_67':
            plan['input_steps']=[dict(at_ms=0,kind='digital',pin=2,value=1)]
            for start in (1000,4500,8100):
                for at,value in ((start,0),(start+6,1),(start+13,0),(start+500,1)):
                    plan['input_steps'].append(dict(at_ms=at,kind='digital',pin=2,value=value))
            for t,a,b in ((250,0,0),(1550,0,92),(2300,0,180),(3900,0,180),(5050,92,88),(5800,180,0),
                          (8650,88,92),(9400,0,180)):
                sample('servo',t,3,a,5)
                sample('servo',t,4,b,5)
        else:
            plan['input_steps']=[dict(at_ms=0,kind='digital',pin=5,value=1)]
            for start in (1000,1700,4300,8000,11300,14200):
                plan['input_steps'] += [dict(at_ms=start,kind='digital',pin=5,value=0),
                                        dict(at_ms=start+350,kind='digital',pin=5,value=1)]
            for t,mode in ((250,0),(1250,1),(1900,1),(4200,1),(4450,2),(4900,2),(5650,2),
                           (8250,3),(11550,0),(14450,1)):
                for pin in (2,3,4):sample('gpio',t,pin,int(mode==pin-1))
                if mode!=2:
                    pins=(6,7,8,9,10,11,12,13,44)
                    for i,pin in enumerate(pins):sample('pwm',t,pin,192 if mode==3 else (255 if mode==1 and i%4==0 else 0),1)
            sample('pwm',4450,7,255,2)
            sample('pwm',4900,7,255,2)
            sample('pwm',4450,6,224,8)
            sample('pwm',4900,6,109,8)
            sample('pwm',5650,6,79,8)
    elif case=='embedbench_24':
        plan.update(horizon_ms=6500,input_steps=[])
        for n,(minimum,maximum,position) in enumerate(((0,0,0),(1023,0,512),(0,1023,1023),
                                                       (512,512,0),(512,512,1023),(512,512,512))):
            t=n*1000
            for pin,value in zip(('A3','A4','A5'),(minimum,maximum,position)):
                plan['input_steps'].append(dict(at_ms=t,pin=pin,value=value,kind='analog'))
            low=minimum*90//1023
            high=90+maximum*90//1023
            sample('servo',t+200,2,low+position*(high-low)/1023,1.1)
    elif case=='embedbench_26':
        plan.update(horizon_ms=41000,input_steps=[])
        for phase in range(21):
            at=phase*2000+250
            step=(phase-1)%18
            index=(step if step<=9 else 18-step) if phase else -1
            for pin in range(2,13):
                sample('gpio',at,pin,int(index==pin-2))
    elif case in ('embedbench_28','embedbench_29','embedbench_30','embedbench_34','embedbench_50','embedbench_54'):
        plan.update(horizon_ms=24500,input_steps=[])
        if case=='embedbench_28':
            for n in range(12):
                phase=n%4
                count=(0,3,6,10)[phase]
                for t in (n*2000+150,n*2000+1700):
                    for i in range(3):sample('servo',t,2+i,180 if phase>i else 0,1)
                    for i,pin in enumerate((*range(5,14),44)):sample('gpio',t,pin,int(i<count))
        elif case in ('embedbench_29','embedbench_30'):
            for n,values in enumerate(((0,0,0),(1023,0,1023),(0,1023,0),(800,700,800),(700,200,512),
                                        (500,500,1023),(1023,1023,1023),(0,1023,1023),(1023,0,512))):
                t=n*2000
                for pin,value in zip(('A3','A4','A5'),values):
                    plan['input_steps'].append(dict(at_ms=t,pin=pin,kind='analog',value=value))
                if case=='embedbench_29':
                    pattern=[]
                    for size,adc in zip((3,3,4),values):pattern.extend(int(i<adc*size//1023) for i in range(size))
                else:
                    first,last=sorted(value*9//1023 for value in values[:2])
                    count=(last-first+1)*values[2]//1023
                    pattern=[int(first<=i<first+count) for i in range(10)]
                for i,state in enumerate(pattern):sample('gpio',t+200,2+i,state)
        elif case=='embedbench_34':
            native=json.loads((root/'behavior_all296_20261023/embed_native'/case/'native_plan.json').read_text(encoding='utf-8'))
            plan['shift_registers']=native['shift_registers']
            for n in range(12):
                digit=n%10
                t=n*2000+250
                sample('display',t,'sevseg1',str(digit))
                for i in range(3):
                    sample('servo',t,2+i,0 if n==0 or digit==9 else 45*(digit//3+i),1)
        elif case=='embedbench_50':
            plan['horizon_ms']=36500
            for n in range(19):
                t=n*2000+250
                index=(n-1)%8 if n else -1
                for i,pin in enumerate((8,9,10,11,12,13,44,45)):
                    sample('gpio',t,pin,int(i==index))
                for pin,on in zip(range(2,8),(0<=index<4,index>=4,False,index>=4,index>=4,0<=index<4)):
                    sample('gpio',t,pin,int(on))
        else:
            rgb=[0,0,0]
            for t,adc in ((0,0),(700,512),(2500,1023),(5200,256),(8000,768),
                          (10200,512),(12500,0),(15200,512),(18500,1023),(20200,0),(23500,256)):
                plan['input_steps'].append(dict(at_ms=t,pin='A3',value=adc,kind='analog'))
                rgb[t//5000%3]=adc*255//1023
                for pin,value in zip((2,3,4),rgb):sample('pwm',t+200,pin,value,1)
                for i,pin in enumerate((*range(5,14),44)):sample('gpio',t+200,pin,int(i<adc*10//1023))
    elif case in ('embedbench_76','embedbench_77','embedbench_78','embedbench_79','embedbench_82','embedbench_83','embedbench_88','medium_020'):
        plan.update(horizon_ms=14000,input_steps=[])
        if case in ('embedbench_76','embedbench_77','embedbench_78','embedbench_79','embedbench_88'):
            native=json.loads((root/'behavior_all296_20261023/embed_native'/case/'native_plan.json').read_text(encoding='utf-8'))
            plan['shift_registers']=native['shift_registers']
        def input_series(kind,pin,sequence):
            plan['input_steps'] += [dict(at_ms=t,pin=pin,value=value,kind=kind) for t,value in sequence]
        def press(pin,start,length):
            input_series('digital',pin,[(start,0),(start+length,1)])
        def digits(t,value):
            for name,digit in zip(('sevseg1','sevseg2'),f'{value:02}'):sample('display',t,name,digit)
        input_series('digital',2,[(0,1)])
        if case=='embedbench_76':
            input_series('digital',3,[(0,1)])
            input_series('digital',4,[(0,1)])
            sample('display',300,'sevseg1','0')
            sample('servo',300,5,0,1)
            sample('servo',300,6,0,1)
            for n,(pin,value) in enumerate(((2,1),(3,3),(4,6),(3,8),(4,0),(4,3),(3,5)),1):
                t=n*1000
                press(pin,t,250)
                sample('display',t+300,'sevseg1',str(value))
                sample('servo',t+300,5,90 if value>=5 else 0,1)
                sample('servo',t+300,6,180 if value>=5 else 90,1)
        elif case=='embedbench_77':
            for n in range(13):
                if n:press(2,n*1000,220)
                sample('display',n*1000+350,'sevseg1',str(n%10))
                for pin in (3,4):sample('servo',n*1000+350,pin,(n%10%2)*90,1)
        elif case=='embedbench_78':
            input_series('digital',3,[(0,1)])
            input_series('analog','A3',[(0,1023),(5500,0)])
            input_series('analog','A4',[(0,1023),(5500,0)])
            for pin,t,length in ((2,500,230),(3,1400,230),(2,2500,2100),(3,6200,475),(2,8300,475)):
                press(pin,t,length)
            for t,value in ((300,99),(850,0),(1750,99),(2800,0),(3800,1),(4800,2),
                            (5850,2),(7000,97),(9100,2)):
                digits(t,value)
        elif case=='embedbench_79':
            for pin in (3,4):input_series('digital',pin,[(0,1)])
            input_series('analog','A3',[(0,1023),(5500,0)])
            for pin,t,length in ((3,500,250),(2,1400,250),(2,2500,2100),(4,5100,250),
                                 (3,6200,475),(2,8300,475)):
                press(pin,t,length)
            for t,value in ((300,0),(850,99),(1750,0),(2800,1),(3800,2),(4800,3),
                            (5400,0),(7000,95),(9100,0)):
                digits(t,value)
        elif case=='embedbench_82':
            input_series('digital',3,[(0,1)])
            input_series('digital',4,[(0,1)])
            for pin,t in ((2,500),(3,1000),(3,3500),(4,6600),(3,9400),(2,12000)):
                press(pin,t,230)
            for t,a,b,color in ((250,0,0,0),(850,90,0,1),(1300,90,0,1),(3200,90,0,1),
                                (3850,90,90,2),(6950,0,0,3),(9750,0,90,2),(12350,90,90,1)):
                sample('servo',t,8,a,1)
                sample('servo',t,9,b,1)
                for i in range(3):sample('gpio',t,5+i,int(color==i+1))
        elif case=='embedbench_83':
            input_series('digital',3,[(0,1)])
            for pin,t in ((2,500),(3,3500),(3,6500),(2,9500),(2,12500)):
                press(pin,t,240)
            for t,a,b,color in ((250,0,0,0),(850,90,0,1),(2400,90,0,1),(2750,90,0,0),
                                (3850,0,0,2),(5900,0,0,0),(6850,0,180,3),(8900,0,180,0),
                                (9850,0,0,2),(12850,90,0,1)):
                sample('servo',t,7,a,1)
                sample('servo',t,8,b,1)
                for i in range(3):sample('gpio',t,4+i,int(color==i+1))
        elif case=='embedbench_88':
            plan['horizon_ms']=54000
            input_series('digital',3,[(0,1)])
            digits(300,0)
            for pin in (4,5,6):sample('gpio',300,pin,0)
            press(3,500,220)
            digits(800,99)
            for n in range(101):
                t=1300+n*500
                press(2,t,230)
                if n in (0,1,33,34,66,67,99,100):
                    value=n%100
                    digits(t+200,value)
                    for i,active in enumerate((value<=33,34<=value<=66,value>=67)):
                        sample('gpio',t+200,4+i,int(active))
        else:
            input_series('analog','A0',[(0,180),(3200,700),(9200,512)])
            for t in (2200,4700,6800,8600):
                press(2,t,260)
            for t,value in ((300,'0'),(1700,'1'),(2600,'180'),(3450,'700'),(5050,'1234'),
                            (7250,'7'),(9050,'700'),(9550,'512')):
                sample('display',t,'tm:4:5',value)
    elif case in ('pceval_clock','pceval_calendar_display','pceval_accelerometer','pceval_gyroscope',
                  'pceval_exercise_counter','medium_019'):
        plan.update(horizon_ms=12500,input_steps=[],serial_inputs=[])
        def serial_check(key,start,end,pattern,count=1,maximum=None):
            check=dict(kind='serial',id=key,from_ms=start,to_ms=end,regex=pattern,min_count=count,
                       requirement_quote='Observe the live input and the task-specific response.')
            if maximum is not None:check['max_count']=maximum
            checks.append(check)
        if case=='pceval_clock':
            plan['rtc_epoch_s']=int(datetime.fromisoformat('2024-11-24T23:59:57+00:00').timestamp())
            for t,value in ((250,'2359'),(1500,'2359'),(3500,'0000'),(6700,'0000')):
                checks.append(dict(kind='multiplex_display',id=f'clock_{t}',digit_pins=[2,3,4,5],
                    segment_pins=list(range(6,13)),from_ms=t,to_ms=t+150,expected=value,
                    requirement_quote='Decode the four multiplexed common-anode digits before and after midnight.'))
                sample('gpio',t,13,0)
        elif case=='pceval_calendar_display':
            plan['rtc_epoch_s']=int(datetime.fromisoformat('2024-02-29T12:00:00+00:00').timestamp())
            for pin,times in ((2,(800,3200)),(3,(2000,4500,5700))):
                plan['input_steps'].append(dict(at_ms=0,kind='digital',pin=pin,value=0))
                for t in times:plan['input_steps'] += [dict(at_ms=t,kind='digital',pin=pin,value=1),
                                                      dict(at_ms=t+220,kind='digital',pin=pin,value=0)]
            for t,date,day in ((0,'2024-02-29','Thursday'),(800,'2024-03-01','Friday'),(2000,'2024-02-29','Thursday'),
                               (3200,'2024-03-01','Friday'),(4500,'2024-02-29','Thursday'),(5700,'2024-02-28','Wednesday')):
                for y,value in ((30,date),(80,day)):
                    checks.append(dict(kind='lcd',id=f'calendar_{t}_{y}',regex=f'^{value}$',from_ms=t,to_ms=t+250,
                        display_properties=dict(initialized=True,background=0,x=10,y=y,color=65535,size=3),
                        requirement_quote='Show the real date and corresponding weekday, including leap-day navigation.'))
        elif case in ('pceval_accelerometer','pceval_gyroscope'):
            plan['imu_samples']=[]
            serial_check('ready',0,500,r'^MPU6050 ready!$')
            accel=case=='pceval_accelerometer'
            for t,a,g in ((0,(2.45,1.96,3.43),(1.13,2.97,-2.01)),
                          (2000,(-6.31,4.22,9.81),(-0.25,0.32,3.14)),
                          (4000,(0.0,-2.75,1.50),(2.25,-1.11,0.0))):
                plan['imu_samples'].append(dict(at_ms=t,acceleration=a,gyro=g))
                x,y,z=a if accel else g
                unit=r'm/s\^2' if accel else 'rad/s'
                pattern=rf'^X: {x:.2f}, Y: {y:.2f}, Z: {z:.2f} {unit}$'.replace('.',r'\.')
                serial_check(f'axes_{t}',t+100,t+1500,pattern,count=2)
            checks.append(dict(kind='interval',id='half_second_reports',source='serial',regex=r'^X:',
                expected_ms=525,tolerance_ms=30,min_count=10,from_ms=0,to_ms=8000,
                requirement_quote='Use the required 500 ms loop delay with up to 55 ms for the serial frame and loop overhead.'))
        elif case=='pceval_exercise_counter':
            plan['seven_segments']=[dict(name='sevseg1',pins=list(range(2,9)),common_anode=False)]
            plan['imu_samples']=[]
            for t,raw,count in ((0,0,0),(600,15000,0),(1200,15001,1),(1900,25000,1),(2500,2000,1),
                                (3100,20000,1),(3700,1999,1),(4300,-32768,2),(5000,-10000,2),(5700,0,2),(6200,-18000,3)):
                plan['imu_samples'].append(dict(at_ms=t,raw_y=raw))
                sample('display',t+200,'sevseg1',str(count))
        else:
            plan['ultrasonic']=[dict(trigger_pin=8,echo_pin=9)]
            plan['input_steps']=[dict(at_ms=0,kind='digital',pin=2,value=1)]
            for t in (2900,7200,9800):
                plan['input_steps'] += [dict(at_ms=t,kind='digital',pin=2,value=0),
                                        dict(at_ms=t+270,kind='digital',pin=2,value=1)]
            for t,v in ((0,24),(3900,31),(7700,20)):
                plan['input_steps'].append(dict(at_ms=t,kind='temperature',pin=7,value=v))
            for t,v in ((0,45),(4500,18),(5400,None),(6200,31)):
                plan['input_steps'].append(dict(at_ms=t,kind='distance',pin=9,value=v))
            serial_check('initial_temperature',1900,2400,r'^Temperature: 24\.0 C$')
            for key,start,end,distance in (('switch_to_distance',2900,3300,45),('changing_distance',4500,5200,18),
                                           ('switch_to_distance_again',9800,10400,31)):
                checks.append(dict(kind='serial',id=key,from_ms=start,to_ms=end,expected=distance,tolerance=.3,
                    regex=r'^Distance: [0-9]+\.[0-9] cm$',min_count=1,
                    requirement_quote='Show the measured distance in centimeters, allowing the echo-scale approximation.'))
            serial_check('invalid_echo',5400,6100,r'^Distance: no echo / out of range$')
            serial_check('switch_to_temperature',7200,7600,r'^Temperature: 31\.0 C$')
            serial_check('temperature_remains_live',8000,8500,r'^Temperature: 20\.0 C$')
    elif case in ('pceval_RTC_module_basic','pceval_button_RTC_timezone','pceval_multiple_timezone',
                  'pceval_alarm','pceval_dday_counter'):
        plan.update(horizon_ms=17000,input_steps=[],serial_inputs=[])
        epoch='2024-11-24T20:48:54+00:00'
        if case=='pceval_RTC_module_basic':
            epoch='2024-11-24T04:48:00+00:00'
            scenarios=[(0,800,r'^Current time: 2024/11/24 \(Sunday\) 0?4:48:0{1,2}$')]
        elif case=='pceval_alarm':
            epoch='2024-11-24T04:48:57+00:00'
            scenarios=[]
            for t in (250,1700,2950):sample('tone',t,8,0)
            for t in (3250,4500,7000,15000):
                checks.append(dict(kind='tone',id=f'alarm_due_{t}',pin=8,at_ms=t,min=31,
                    requirement_quote='The configured buzzer sounds only after the RTC reaches the alarm time.'))
        elif case=='pceval_dday_counter':
            epoch='2024-11-24T04:48:00+00:00'
            scenarios=[]
            for t,date,days,suffix in ((500,'2025-04-11T09:38:00',138,'\n'),
                                       (3000,'2025-06-11T09:38:00',199,'\r\n'),
                                       (5500,'2025-03-03T09:38:00',99,''),
                                       (13000,'2024-11-25T04:49:00',1,'\n')):
                plan['serial_inputs'].append(dict(at_ms=t,text=date+suffix))
                scenarios.append((t,t+250,f'^D-{days}$'))
        else:
            pins=(2,) if case=='pceval_button_RTC_timezone' else (2,3,4,5)
            for i,pin in enumerate(pins):
                t=2000+i*3000
                plan['input_steps'] += [dict(at_ms=at,kind='digital',pin=pin,value=value)
                                        for at,value in ((0,0),(t,1),(t+2000,0))]
            scenarios=[(0,1500,r'^2024-11-24T20:48:[0-9]{2}$'),
                       (2300,3800,r'^2024-11-25T05:48:[0-9]{2}$'),
                       (14200,16500,r'^2024-11-24T20:49:[0-9]{2}$')]
            if case=='pceval_multiple_timezone':
                scenarios += [(5000,5850,r'^2024-11-25T00:48:[0-9]{2}$'),
                              (8300,9900,r'^2024-11-24T10:49:[0-9]{2}$'),
                              (11300,12900,r'^2024-11-24T15:49:[0-9]{2}$')]
        plan['rtc_epoch_s']=int(datetime.fromisoformat(epoch).timestamp())
        for t,end,pattern in scenarios:
            checks.append(dict(kind='serial',id=f'rtc_report_{t}',regex=pattern,from_ms=t,to_ms=end,min_count=1,
                requirement_quote='Report the expected time or date computation from the independently configured RTC.'))
    elif case in ('medium_018','pceval_button_LCD_display','pceval_dht22_LCD_display',
                  'pceval_serial_LCD_display','pceval_piano_keyboard'):
        plan.update(horizon_ms=12000,input_steps=[],serial_inputs=[])
        def stimuli(kind,pin,events):
            plan['input_steps'].extend(dict(at_ms=t,pin=pin,value=value,kind=kind) for t,value in events)
        def lcd(key,t,end,pattern,y=0,rst=8,count=1):
            checks.append(dict(kind='lcd',id=key,regex=pattern,from_ms=t,to_ms=end,min_count=count,
                max_count=count,display_properties=dict(initialized=True,rotation=1,background=0,
                    x=0,y=y,color=65535,size=2,cs=10,dc=9,rst=rst),
                requirement_quote='Check displayed content with the required LCD setup and text settings.'))
        if case=='medium_018':
            stimuli('digital',2,[(0,1),(5100,0),(5450,1)])
            stimuli('digital',3,[(0,1),(1600,0),(1900,1),(8300,0),(8600,1)])
            stimuli('analog','A0',[(0,1023),(2400,512),(3200,0),(4100,1023),(7900,256)])
            for t,value in ((180,18),(1000,100),(1900,255),(2650,128),(3450,0),(4350,255),(8900,64)):
                sample('pwm',t,9,value,4)
            checks.append(dict(kind='relation',id='auto_fade_after_selection',source='pwm',pin=9,
                at_ms=[5800,6400,7000],relation='decreasing',tolerance=3,
                requirement_quote='D2 selects gradual brightness independent of the potentiometer.'))
        elif case=='pceval_button_LCD_display':
            stimuli('digital',2,[(0,0),(1000,1),(3000,0),(5000,1),(5800,0)])
            for t,label in ((0,'Not pressed'),(1000,'Pressed'),(3000,'Not pressed'),(5000,'Pressed'),(5800,'Not pressed')):
                lcd(f'button_{t}',t,t+250,f'^{label}!$')
            lcd('no_redraw_while_held',1300,2900,r'^Pressed!$',count=0)
        elif case=='pceval_dht22_LCD_display':
            stimuli('temperature',2,[(0,23),(4200,28),(8200,None),(10100,20)])
            stimuli('humidity',2,[(0,41),(4200,72),(8200,None),(10100,56)])
            plan['horizon_ms']=14000
            for t,end,humidity,temperature in ((0,2400,41,23),(4200,6500,72,28),(10100,12500,56,20)):
                lcd(f'humidity_{t}',t,end,rf'^Humidity: {humidity}(?:\.0+)?$')
                lcd(f'temperature_{t}',t,end,rf'^Temperature: {temperature}(?:\.0+)?$',y=100)
        elif case=='pceval_serial_LCD_display':
            for t,data,suffix in ((400,'Hello Arduino','\n'),(2500,'A12','\r\n'),(4600,'Mixed Case 987','')):
                plan['serial_inputs'].append(dict(at_ms=t,text=data+suffix))
                lcd(f'display_{t}',t,t+250,f'^{data}$',rst=-1)
                checks.append(dict(kind='serial',id=f'echo_{t}',regex=f'^{data}$',from_ms=t,to_ms=t+250,
                    min_count=1,max_count=1,requirement_quote='Echo each original input exactly once.'))
        else:
            for i,pin in enumerate(range(2,6)):
                t=1000+i*1800
                stimuli('digital',pin,[(0,0),(t,1),(t+850,0)])
                for j,buzzer in enumerate(range(9,13)):
                    sample('tone',t+200,buzzer,(262,294,330,349)[i] if j==i else 0)
                    sample('tone',t+1100,buzzer,0)
    elif case=='pceval_4_digit_7_segment_display':
        plan.update(horizon_ms=8500,input_steps=[])
        for n in range(8):
            digit=n%4
            mask=(0x06,0x5b,0x4f,0x66)[digit]
            for t in (n*1000+150,n*1000+850):
                for bit in range(7):sample('gpio',t,2+bit,1-((mask>>bit)&1))
                sample('gpio',t,9,1)
                for i in range(4):sample('gpio',t,10+i,int(i==digit))
    elif case=='pceval_LCD_display_basic':
        plan.update(horizon_ms=1500,input_steps=[])
        checks.append(dict(kind='lcd',id='startup_hello',regex=r'^Hello World!$',from_ms=0,to_ms=1000,
            min_count=1,display_properties=dict(initialized=True,rotation=1,background=0,
                 x=0,y=0,color=65535,size=2,cs=10,dc=9,rst=8),
            requirement_quote='Display Hello World in white at (0, 0) on a black landscape screen, with size 2.'))
    elif case in ('pceval_multiplexer_photoresistor','pceval_multiplexer_potentiometer'):
        channels=6 if case=='pceval_multiplexer_photoresistor' else 5
        plan.update(horizon_ms=3200,input_steps=[])
        samples=[]
        for t,values in ((0,(0,203,405,607,809,1023)),(1500,(901,712,523,334,145,0))):
            for channel,value in enumerate(values[:channels]):
                samples.append(dict(at_ms=t,channel=channel,value=value))
                checks.append(dict(kind='serial',id=f'channel_{channel}_{t}',from_ms=t+50,to_ms=t+1400,
                    regex=rf'^Sensor {channel}: {value}$',min_count=3,
                    requirement_quote='Read and repeatedly report the independent ADC value of each connected input channel.'))
        checks.append(dict(kind='serial',id='no_unwired_channels',from_ms=0,to_ms=3200,
            regex=rf'^Sensor (?:[{channels}-9]|1[0-5]):',min_count=0,max_count=0,
            requirement_quote='Only scan the wired analog multiplexer channels.'))
        plan['analog_multiplexers']=[dict(signal_pin='A0',select_pins=[2,3,4,5],samples=samples)]
    elif case=='pceval_photoresistor_bar_led':
        plan.update(horizon_ms=8500,input_steps=[])
        for n,value in enumerate((1023,900,700,512,256,0,1023)):
            t=n*1100
            plan['input_steps'].append(dict(at_ms=t,pin='A0',value=value,kind='analog'))
            count=(1023-value)*10//1023
            for pin in range(2,12):sample('gpio',t+200,pin,int(pin-2<count))
    elif case in ('pceval_binary_led','pceval_serial_bar_led'):
        plan.update(horizon_ms=10000,input_steps=[],serial_inputs=[])
        values=(0,1,10,21,31,0) if case=='pceval_binary_led' else (0,6,3,9,10,0)
        pins=range(2,7) if case=='pceval_binary_led' else range(2,12)
        for n,value in enumerate(values):
            t=n*1400+250
            plan['serial_inputs'].append(dict(at_ms=t,text=f'{value}\n'))
            for pin in pins:
                expected=(value>>(pin-2))&1 if case=='pceval_binary_led' else int(pin-2<value)
                sample('gpio',t+200,pin,expected)
                sample('gpio',t+1000,pin,expected)
    elif case in ('embedbench_43','embedbench_51','embedbench_53','embedbench_57','embedbench_58','embedbench_59'):
        plan.update(horizon_ms=32500,input_steps=[])
        if case in ('embedbench_43','embedbench_57','embedbench_58','embedbench_59'):
            native=json.loads((root/'behavior_all296_20261023/embed_native'/case/'native_plan.json').read_text(encoding='utf-8'))
            plan['shift_registers']=native['shift_registers']
        if case=='embedbench_43':
            for pin,steps in (('A3',((0,1023),(2700,512),(10500,0),(18500,1023))),
                              ('A4',((0,0),(6500,1023))),('A5',((0,0),(6500,1023),(14500,0)))):
                plan['input_steps'].extend(dict(at_ms=t,pin=pin,value=adc,kind='analog') for t,adc in steps)
            for t,digit,pwm in ((250,0,255),(2250,1,255),(2950,1,127),(4300,2,127),(6300,3,127),(9750,3,127),
                                (10300,4,127),(10750,4,0),(14300,5,0),(16750,5,0),(17300,6,0),
                                (18750,6,255),(20300,7,255),(23300,8,255),(26300,9,255),(29300,0,255)):
                sample('display',t,'sevseg1',str(digit))
                for i,pin in enumerate(range(2,12)):sample('pwm',t,pin,pwm if i<digit else 0,1)
        elif case=='embedbench_51':
            for n in range(16):
                phase=n%5
                t=n*2000+250
                active=(1,16,256,17,511)[phase]
                for i,pin in enumerate(range(2,11)):sample('pwm',t,pin,255 if active&(1<<i) else 0)
                for i,pin in enumerate((11,12,13,44,45)):sample('gpio',t,pin,int(i<=phase))
        elif case=='embedbench_53':
            for n in range(16):
                t=n*2000+250
                active=(0,1,3,7,6,4)[n%6]
                for i in range(3):
                    on=bool(active&(1<<i))
                    sample('pwm',t,2+i,255 if on else 0)
                    sample('servo',t,5+i,180 if on else 0,1)
                for i,pin in enumerate((8,9,10,11,12,13,44,45)):
                    sample('gpio',t,pin,int(i<bin(active).count('1')*8//3))
        elif case=='embedbench_57':
            plan['horizon_ms']=204500
            for n in (*range(15),45,89,90,98,99,100,101):
                t=n*2000+250
                number=99-n%100
                for name,digit in zip(('sevseg1','sevseg2'),f'{number:02}'):sample('display',t,name,digit)
                active=(1,16,256,5,24,384)[n%100%6]
                for i,pin in enumerate(range(2,11)):sample('gpio',t,pin,int(bool(active&(1<<i))))
        else:
            plan['horizon_ms']=42500
            for n in range(21):
                t=n*2000+250
                step=n%10
                digit=9-step
                sample('display',t,'sevseg1',str(digit))
                if case=='embedbench_58':
                    color=0 if step==0 else (1,2,4,3,6,5,7)[(step-1)%7]
                    rgbs=((2,3,4),)
                    sample('servo',t,5,step*20,1)
                else:
                    color=(1,2,6,5,4,3,7)[n%7]
                    rgbs=((2,3,4),(5,6,7),(8,9,10))
                    for pin in (11,12,13):sample('servo',t,pin,(step//3)*60,1)
                for rgb in rgbs:
                    for i,pin in enumerate(rgb):sample('pwm',t,pin,255 if color&(1<<i) else 0)
    elif case=='embedbench_44':
        plan.update(horizon_ms=10500,input_steps=[])
        for at,adc in ((0,0),(2000,365),(4000,725),(6000,1023),(8000,201)):
            plan['input_steps'].append(dict(at_ms=at,pin='A3',value=adc,kind='analog'))
            value=adc*99//1023
            for name,digit in zip(('sevseg1','sevseg2'),f'{value:02}'):sample('display',at+200,name,digit)
            for index,pin in enumerate(range(2,12)):sample('gpio',at+200,pin,int(index<value//10))
    elif case=='embedbench_52':
        plan.update(horizon_ms=17500,input_steps=[])
        for n in range(9):
            at=n*2000+350
            step=n%7
            for pin in (8,9):sample('servo',at,pin,step*30,1)
            for pin in range(2,8):sample('gpio',at,pin,int((pin==2 and step>0) or (pin==7 and step>3)))
            for index,pin in enumerate((10,11,12,13,44,45)):sample('gpio',at,pin,int(index<step))
    elif case=='embedbench_55':
        plan.update(horizon_ms=15000,input_steps=[])
        for index,(a,b) in enumerate(((0,0),(512,512),(340,682),(341,681),(1023,1023),(682,0))):
            at=index*2300
            plan['input_steps'] += [dict(at_ms=at,pin='A3',value=a,kind='analog'),dict(at_ms=at,pin='A4',value=b,kind='analog')]
            for when in (at+200,at+2100):
                for rgb,adc in zip(((2,3,4),(5,6,7)),(a,b)):
                    for channel,pin in enumerate(rgb):sample('gpio',when,pin,int(channel==((adc>=341)+(adc>=682))))
                for i,pin in enumerate((10,11,12,13,44,45,8,9)):sample('gpio',when,pin,int(i<(a+b)*4//1023))
    elif case=='embedbench_56':
        plan.update(horizon_ms=204500,input_steps=[])
        for n in (*range(16),49,98,99,100,101):
            at=n*2000+250
            for name,digit in zip(('sevseg1','sevseg2'),f'{n%100:02}'):sample('display',at,name,digit)
            color=(1,2,4,3,6,5,7)[n%7]
            for rgb in ((2,3,4),(5,6,7)):
                for channel,pin in enumerate(rgb):sample('gpio',at,pin,int(bool(color&(1<<channel))))
    elif case in ('embedbench_90','embedbench_94','embedbench_95','embedbench_96',
                  'embedbench_105','embedbench_106','embedbench_108'):
        plan['input_steps']=[]
        if case in ('embedbench_105','embedbench_106'):
            native=json.loads((root/'behavior_all296_20261023/embed_native'/case/'native_plan.json').read_text(encoding='utf-8'))
            plan['shift_registers']=native['shift_registers']
        if case=='embedbench_90':
            plan['horizon_ms']=38500
            for tick in range(19):
                t=tick*2000+300
                step=tick%15
                for pin,active in ((2,0<step<=6),(3,7<=step<=13),(4,step==14)):
                    sample('gpio',t,pin,int(active))
                sample('servo',t,5,min(step,6)*30,1)
                second=0 if tick<7 or step>=14 else (180 if step<=7 else (13-step)*30)
                sample('servo',t,6,second,1)
        elif case=='embedbench_94':
            plan['horizon_ms']=6500
            for t,adc in ((0,(0,0,0)),(1000,(114,512,0)),(2000,(228,1023,1023)),(5000,(0,0,0))):
                for pin,value in zip(('A3','A4','A5'),adc):
                    plan['input_steps'].append(dict(at_ms=t,pin=pin,kind='analog',value=value))
                sample('pwm',t+200,2,adc[1]*255//1023,1)
            for t,angle in ((300,0),(1100,10),(1350,20),(2250,22),(3100,31),(4500,40),(5200,20),(5600,0)):
                sample('servo',t,3,angle,2)
        elif case=='embedbench_95':
            plan['horizon_ms']=46500
            for tick in range(24):
                t=tick*2000+250
                for pin,on in ((2,tick and tick%2),(3,tick and not tick%2)):
                    sample('gpio',t,pin,int(on))
                for i,pin in enumerate(range(4,14)):
                    sample('gpio',t,pin,int(i<tick%11))
        elif case=='embedbench_96':
            plan['horizon_ms']=18500
            for tick in range(10):
                t=tick*2000+250
                step=(tick-1)%4 if tick else -1
                for i in range(3):sample('gpio',t,2+i,int(i==step))
                for i,pin in enumerate((*range(5,14),44)):
                    sample('gpio',t,pin,int(step==3 or (i==0 and tick>0)))
        elif case=='embedbench_105':
            plan['horizon_ms']=18500
            for n,adc in enumerate((0,351,352,692,693,1023,80)):
                t=n*2600
                plan['input_steps'].append(dict(at_ms=t,pin='A3',kind='analog',value=adc))
                value=adc*99//1023
                for at in (t+220,t+2300):
                    for name,digit in zip(('sevseg1','sevseg2'),f'{value:02}'):
                        sample('display',at,name,digit)
                    for pin,on in ((2,1),(3,value>=34),(4,value>=67)):
                        sample('gpio',at,pin,int(on))
        elif case=='embedbench_106':
            plan['horizon_ms']=204500
            for tick in (*range(12),20,25,49,60,89,90,98,99,100,101):
                t=tick*2000+250
                number=tick%100
                for name,digit in zip(('sevseg1','sevseg2'),f'{number:02}'):
                    sample('display',t,name,digit)
                for pin,on in ((2,tick and tick%2),(3,tick and not tick%2)):
                    sample('gpio',t,pin,int(on))
                for i,pin in enumerate(range(4,14)):
                    sample('gpio',t,pin,int(i<=number//10))
        else:
            plan['horizon_ms']=32500
            for tick in range(17):
                t=tick*2000+250
                sample('gpio',t,2,tick%2)
                color=(1,2,4,3,6,5,7)[tick%7]
                for rgb in ((3,4,5),(6,7,8),(9,10,11)):
                    for channel,pin in enumerate(rgb):
                        sample('gpio',t,pin,int(bool(color&(1<<channel))))
    elif case in ('embedbench_42','embedbench_62','embedbench_81','embedbench_86','embedbench_97','embedbench_98'):
        plan.update(horizon_ms=30000,input_steps=[])
        if case in ('embedbench_42','embedbench_62','embedbench_81'):
            native=json.loads((root/'behavior_all296_20261023/embed_native'/case/'native_plan.json').read_text(encoding='utf-8'))
            plan['shift_registers']=native['shift_registers']
        if case in ('embedbench_42','embedbench_62'):
            plan['horizon_ms']=204500
            for tick in (*range(12),18,19,20,35,79,80,89,90,98,99,100,101):
                t=tick*2000+250
                number=tick%100
                for name,digit in zip(('sevseg1','sevseg2'),f'{number:02}'):sample('display',t,name,digit)
                if case=='embedbench_42':
                    units=number%10
                    for i,pin in enumerate((12,13,44)):
                        sample('servo',t,pin,min(180,units*18+i*60) if units else 0,1)
                    pins=range(2,12)
                else:
                    for i,pin in enumerate(range(2,8)):
                        color=0 if number<=5 else (1 if number<=9 else 2)
                        sample('pwm',t,pin,255 if tick and i%3==color else 0)
                    pins=(8,9,10,11,12,13,44,45)
                for i,pin in enumerate(pins):sample('gpio',t,pin,int(i<number//10))
        elif case in ('embedbench_81','embedbench_86'):
            pins=(2,3,4) if case=='embedbench_81' else (2,)
            for pin in pins:plan['input_steps'].append(dict(at_ms=0,kind='digital',pin=pin,value=1))
            presses=((2,)+19*(3,)+(3,2,3,4)) if case=='embedbench_81' else 10*(2,)
            value=0
            for n,pin in enumerate((0,)+presses):
                t=n*1000
                if pin:
                    for at,state in ((t,0),(t+8,1),(t+15,0),(t+280,1)):
                        plan['input_steps'].append(dict(at_ms=at,kind='digital',pin=pin,value=state))
                    value=(value+1 if pin==2 else value+5 if pin==3 else 0) if case=='embedbench_81' else (value+1)%8
                    if value>99:value=0
                if case=='embedbench_81':
                    for name,digit in zip(('sevseg1','sevseg2'),f'{value:02}'):sample('display',t+145,name,digit)
                    bar_pins=(*range(5,14),44)
                    fill=value//10
                else:
                    color=(0,1,2,3,6,5,7,0)[value]
                    other=(0,4,6,5,3,6,3,7)[value]
                    for group,mask in enumerate((color,other)):
                        for i in range(3):sample('pwm',t+230,3+3*group+i,255 if mask&(1<<i) else 0)
                    bar_pins=(9,10,11,12,13,44,45)
                    fill=value
                for i,led in enumerate(bar_pins):sample('gpio',t+230,led,int(i<fill))
            if case=='embedbench_86':
                plan['input_steps'] += [dict(at_ms=450,kind='digital',pin=2,value=0),
                                        dict(at_ms=550,kind='digital',pin=2,value=1)]
                for pin in range(3,9):sample('pwm',700,pin,0)
        elif case=='embedbench_97':
            for tick in range(14):
                t=tick*2000+250
                stage=tick%4
                for i in range(3):
                    sample('servo',t,5+i,(i+1)*60 if stage>i else 0,1)
                    sample('gpio',t,2+i,int(stage>i))
                for i,pin in enumerate((8,9,10,11,12,13,44,45)):
                    sample('gpio',t,pin,int(i<(0,2,5,8)[stage]))
        else:
            plan['horizon_ms']=84500
            for tick in range(42):
                t=tick*2000+250
                angle=(tick-1)%19*10 if tick else 0
                sample('servo',t,44,angle,1)
                sample('servo',t,45,180-angle if tick else 0,1)
                for pin in (2,3):sample('gpio',t,pin,int(tick>0 and angle<=90))
                for i,pin in enumerate(range(4,14)):sample('gpio',t,pin,int(i<angle//18))
    elif case in ('embedbench_103','embedbench_109','embedbench_113','embedbench_114','embedbench_116','embedbench_122'):
        plan.update(horizon_ms=31000,input_steps=[])
        if case in ('embedbench_103','embedbench_116'):
            native=json.loads((root/'behavior_all296_20261023/embed_native'/case/'native_plan.json').read_text(encoding='utf-8'))
            plan['shift_registers']=native['shift_registers']
        if case in ('embedbench_103','embedbench_109'):
            for t in range(250,30500,500):
                if case=='embedbench_103':
                    x=t%12000
                    angle=max(0,min(180,x*.045,(10000-x)*.045))
                    sample('servo',t,4,angle,2)
                    sample('servo',t,5,180-angle,2)
                    sample('display',t,'sevseg1',str(t//2000%10))
                    for i,pin in enumerate((2,3)):sample('gpio',t,pin,int(t//2000%2!=i))
                else:
                    x=t%8000
                    angle=max(0,min(180,x*.09,(6000-x)*.09))
                    for pin in (8,9):sample('servo',t,pin,angle,2)
                    step=(t+2000)//4000
                    for i in range(3):
                        sample('gpio',t,2+i,int(step>0 and (step-1)%3==i))
                        sample('gpio',t,5+i,int(step%3==i))
        elif case in ('embedbench_113','embedbench_114'):
            for n in range(15):
                t=n*2000+250
                if case=='embedbench_113':
                    phase=n%4
                    sample('gpio',t,2,phase%2)
                    mask=(1,34,140,511)[phase]
                    for i,pin in enumerate(range(3,12)):sample('gpio',t,pin,int(bool(mask&(1<<i))))
                    for i,pin in enumerate((12,13,44,45)):sample('gpio',t,pin,int(i<=phase))
                else:
                    phase=(n-1)%3+1 if n else 0
                    led1=phase in (1,3)
                    led2=phase in (2,3)
                    for i,on in enumerate((led1,led2,phase==1,phase==3,phase==3,False,led2,phase==3)):
                        sample('gpio',t,2+i,int(on))
                    for i,pin in enumerate((10,11,12,13,44,45)):
                        sample('gpio',t,pin,int(led1 if i<3 else led2))
        elif case=='embedbench_116':
            plan['horizon_ms']=204500
            for n in (*range(16),41,49,98,99,100,101):
                t=n*2000+250
                for name,digit in zip(('sevseg1','sevseg2'),f'{n%100:02}'):sample('display',t,name,digit)
                sample('gpio',t,2,n%2)
                color=(1,2,4,3,6,5,7)[n%7]
                for i,pin in enumerate((3,4,5)):sample('gpio',t,pin,int(bool(color&(1<<i))))
        else:
            for pin in (5,6,7):plan['input_steps'].append(dict(at_ms=0,kind='digital',pin=pin,value=1))
            for t,pin,length in ((500,5,90),(1100,5,300),(2400,6,300),(3700,7,300),(5000,5,300),
                                 (6300,6,300),(7600,5,300),(8900,7,300),(10200,5,300)):
                plan['input_steps'] += [dict(at_ms=t,kind='digital',pin=pin,value=0),
                                        dict(at_ms=t+length,kind='digital',pin=pin,value=1)]
            for t,bits in ((250,0),(850,0),(1450,1),(2750,3),(4050,7),(5350,6),(6650,4),
                           (7950,5),(9250,1),(10550,0)):
                for i,pin in enumerate((2,3,4)):sample('gpio',t,pin,(bits>>i)&1)
                for i,pin in enumerate((8,9,10,11,12,13,44,45),1):sample('gpio',t,pin,int(i==bits))
    elif case in ('embedbench_60','embedbench_115'):
        native=json.loads((root/'behavior_all296_20261023/embed_native'/case/'native_plan.json').read_text(encoding='utf-8'))
        plan.update(shift_registers=native['shift_registers'],horizon_ms=24500,input_steps=[])
        if case=='embedbench_60':
            for n,values in enumerate(((0,0,0),(1023,0,0),(0,1023,1023),(512,256,768),(0,0,0))):
                at=n*2000
                for index,(pin,value) in enumerate(zip(('A3','A4','A5'),values)):
                    plan['input_steps'].append(dict(at_ms=at,pin=pin,value=value,kind='analog'))
                    for output in (2+index,5+index):sample('pwm',at+200,output,value*255//1023,1)
                for name,value in zip(('sevseg1','sevseg2'),(values[0],values[2])):
                    sample('display',at+200,name,str(value*9//1023))
        else:
            for n in range(12):
                at=n*2000+250
                phase=n%10
                digit=9-phase
                color=(phase-1)%3 if 0<n<10 else phase%3
                for name in ('sevseg1','sevseg2'):sample('display',at,name,str(digit))
                for index in range(3):
                    lit=int(n>0 and digit>0 and color==index)
                    sample('gpio',at,2+index,lit)
                    for pin in (5+index,8+index):sample('gpio',at,pin,1 if digit==0 else lit)
    elif case=='pceval_parking_spot_monitor':
        plan.update(horizon_ms=11500,input_steps=[],ultrasonic=[dict(trigger_pin=8,echo_pin=9)])
        for at,distance in ((0,100),(2000,25),(4000,50),(6000,49),(8000,120)):
            plan['input_steps'].append(dict(at_ms=at,pin=9,value=distance,kind='distance'))
            for pin,value in ((4,int(distance<50)),(3,int(distance>=50)),(2,0)):
                sample('gpio',at+200,pin,value)
    elif case=='pceval_potentiometer_bar_led':
        plan.update(horizon_ms=11500,input_steps=[])
        for at,adc in ((0,0),(2000,102),(4000,103),(6000,512),(8000,1023)):
            plan['input_steps'].append(dict(at_ms=at,pin='A0',value=adc,kind='analog'))
            for index,pin in enumerate(range(2,12)):sample('gpio',at+200,pin,int(index<adc*10//1023))
    elif case=='advanced_006':
        plan.update(horizon_ms=17000,input_steps=[])
        for at,pin,value,kind in ((0,2,1,'digital'),(0,'A0',256,'analog'),(0,'A1',768,'analog'),(0,'A2',768,'analog'),
            (1500,'A2',128,'analog'),(3500,'A2',768,'analog'),(5000,2,0,'digital'),(5400,2,1,'digital'),
            (7500,2,0,'digital'),(7900,2,1,'digital')):
            plan['input_steps'].append(dict(at_ms=at,pin=pin,value=value,kind=kind))
        for at,value in ((500,1),(2200,0),(4200,1),(6000,0),(8500,1),(12000,1),(13500,0)):sample('gpio',at,10,value)
        sample('tone',2200,6,1000,1);sample('tone',4200,6,0)
        plan['experimental_condition']='Preserve the original implementation\'s ADC thresholds: dry soil <512, no rain >=512, sufficient water >=512. Manual mode entry and forced-run presses are separate; pressing during a forced run cancels and returns to automatic.'
    elif case=='pceval_button_pullup':
        plan.update(horizon_ms=9500,input_steps=[dict(at_ms=t,pin=4,value=v,kind='digital') for t,v in ((0,1),(2000,0),(4000,1),(6000,0))])
        for at,pressed in ((0,False),(2000,True),(4000,False),(6000,True)):
            for offset in (0,1000):
                checks.append(dict(kind='serial',id=f'pullup_{at}_{offset}',from_ms=at+offset+100,to_ms=at+offset+500,
                    regex='^Button '+('pressed' if pressed else 'not pressed')+'$',min_count=1,baud=9600,
                    requirement_quote='Continuously report the externally pulled-up button state'))
    elif case=='embedbench_3':
        plan.update(horizon_ms=12000,input_steps=[dict(at_ms=0,pin=p,value=1,kind='digital') for p in (5,6,7)])
        for at,pin in ((2500,5),(6000,6),(10000,7)):
            plan['input_steps'] += [dict(at_ms=at,pin=pin,value=0,kind='digital'),dict(at_ms=at+400,pin=pin,value=1,kind='digital')]
        for at,color in ((500,1),(2300,2),(3500,2),(5500,2),(8500,4),(11000,0)):
            for bit,pin in enumerate((2,3,4)):sample('pwm',at,pin,255 if color&(1<<bit) else 0)
    elif case=='embedbench_9':
        plan.update(horizon_ms=16000,input_steps=[dict(at_ms=0,pin=12,value=1,kind='digital')])
        for n in range(7):
            at=1000+n*2000;count=(n+1)%6*2
            plan['input_steps'] += [dict(at_ms=at,pin=12,value=0,kind='digital'),dict(at_ms=at+400,pin=12,value=1,kind='digital')]
            for i,pin in enumerate(range(2,12)):sample('gpio',at+700,pin,int(count>9-i))
    elif case=='embedbench_15':
        plan.update(horizon_ms=22500,input_steps=[])
        for n in range(22):sample('display',n*1000+400,'sevseg1,sevseg2',str(n*6%100))
    elif case=='embedbench_27':
        plan.update(horizon_ms=45500,input_steps=[])
        for n in range(22):
            step=n%20;count=step if step<=10 else 20-step
            for pin in (2,3,4):sample('servo',n*2000+400,pin,count*18,1)
            for i,pin in enumerate((5,6,7,8,9,10,11,12,13,44)):sample('gpio',n*2000+400,pin,int(i<count))
    elif case=='embedbench_36':
        plan.update(horizon_ms=11500,input_steps=[])
        for n,(a,b) in enumerate(((0,0),(1023,0),(0,1023),(1023,1023),(512,512))):
            at=n*2000
            plan['input_steps'] += [dict(at_ms=at,pin='A3',value=a,kind='analog'),dict(at_ms=at,pin='A4',value=b,kind='analog')]
            sample('display',at+500,'sevseg1',str((a*9//1023+b*9//1023)%10))
    elif case=='embedbench_40':
        plan.update(horizon_ms=45500,input_steps=[])
        for n in range(22):
            count=n%10;sample('display',n*2000+400,'sevseg1',str(count))
            for i,pin in enumerate(range(2,12)):sample('gpio',n*2000+400,pin,int(i<count))
    elif case=='embedbench_48':
        plan.update(horizon_ms=15000,input_steps=[])
        for at,a,b in ((0,0,0),(3000,512,256),(7000,1023,1023)):
            plan['input_steps'] += [dict(at_ms=at,pin='A3',value=a,kind='analog'),dict(at_ms=at,pin='A4',value=b,kind='analog')]
        for at,a,b in ((500,0,0),(2500,0,0),(4500,512,256),(6500,512,256),(8500,1023,1023),(10500,1023,1023),(12500,1023,1023)):
            for red,green,blue in ((2,3,4),(5,6,7),(8,9,10)):
                sample('pwm',at,red,int(a*255/1023),1);sample('pwm',at,green,int(b*255/1023),1);sample('gpio',at,blue,0)
            sample('servo',at,11,a*180/1023,1);sample('servo',at,12,b*180/1023,1);sample('servo',at,13,180 if at//2000%2 else 0,1)
    elif case=='embedbench_80':
        plan.update(horizon_ms=26500,input_steps=[dict(at_ms=0,pin=2,value=1,kind='digital')])
        for n in range(11):
            at=1000+n*2200;count=(n+1)%10
            plan['input_steps'] += [dict(at_ms=at,pin=2,value=0,kind='digital'),dict(at_ms=at+400,pin=2,value=1,kind='digital')]
            sample('display',at+700,'sevseg1',str(count))
            for i,pin in enumerate(range(3,13)):sample('gpio',at+700,pin,int(i<count))
    elif case=='embedbench_89':
        plan.update(horizon_ms=22500,input_steps=[dict(at_ms=0,pin=p,value=1,kind='digital') for p in (2,3,4)])
        for n in range(16):
            at=1000+n*1200;count=(n+1)%16;pin=2+n%3
            plan['input_steps'] += [dict(at_ms=at,pin=pin,value=0,kind='digital'),dict(at_ms=at+400,pin=pin,value=1,kind='digital')]
            sample('display',at+700,'sevseg1',str(count%10))
            colors=(0,0) if not count else ((1,4) if count<5 else ((2,3) if count<10 else (5,6)))
            for pins,color in zip(((5,6,7),(8,9,10)),colors):
                for bit,p in enumerate(pins):sample('pwm',at+700,p,255 if color&(1<<bit) else 0)
    elif case=='embedbench_91':
        plan.update(horizon_ms=15000,input_steps=[])
        for n in range(7):
            at=n*2000+400;sample('gpio',at,2,int(n%2==0));sample('gpio',at,3,n%2)
            for pin,v in zip((4,5,6),(0,90,180) if n%2==0 else (180,0,90)):sample('servo',at,pin,v,1)
    elif case=='embedbench_93':
        plan.update(horizon_ms=11500,input_steps=[dict(at_ms=t,pin='A3',value=v,kind='analog') for t,v in ((0,0),(1000,512),(3500,1023),(6500,256))])
        for at,angle,direction in ((500,0,0),(1500,90,1),(3200,90,0),(4000,180,1),(5800,180,0),(7000,45,-1),(9000,45,0)):
            sample('servo',at,4,angle,1);sample('gpio',at,2,int(direction==1));sample('gpio',at,3,int(direction==-1))
    elif case=='embedbench_99':
        plan.update(horizon_ms=12500,input_steps=[dict(at_ms=t,pin='A3',value=v,kind='analog') for t,v in ((0,0),(2000,512),(4000,1023),(10000,256))])
        for at,duty,count in ((500,0,0),(2500,127,5),(4500,255,10),(6500,0,10),(8500,255,10),(11000,63,2)):
            sample('pwm',at,2,duty,1)
            for i,pin in enumerate(range(3,13)):sample('gpio',at,pin,int(i<count))
    elif case=='embedbench_100':
        plan.update(horizon_ms=9500,input_steps=[])
        for at,a,b,c in ((0,0,0,0),(2000,256,512,512),(4000,1023,128,1023),(6000,512,1023,0)):
            for p,v in zip(('A3','A4','A5'),(a,b,c)):plan['input_steps'].append(dict(at_ms=at,pin=p,value=v,kind='analog'))
            sample('pwm',at+500,2,int(a*255/1023),1);sample('pwm',at+500,3,int(b*255/1023),1)
            for i,pin in enumerate(range(4,14)):sample('gpio',at+500,pin,int(i<c*10//1023))
    elif case=='simple_020':
        plan.update(horizon_ms=5500)
        plan['input_steps']=[dict(at_ms=t,pin=2,value=v,kind='digital') for t,v in ((0,1),(900,0),(1250,1),(2700,0),(3050,1))]
        plan['experimental_condition']='Disclosed ground-connected INPUT_PULLUP button; the retained legacy trace used the opposite wiring variant.'
        for t,value in ((500,'0000'),(1100,'1111'),(1700,'0000'),(2900,'1111'),(3800,'0000')):
            sample('display',t,'tm:4:5',value)
    elif case in ('pceval_humidity_sensor','pceval_temperature_sensor'):
        humidity=case=='pceval_humidity_sensor';word='Humidity' if humidity else 'Temperature'
        values=(30,66,45,80) if humidity else (25,35,26,45)
        threshold=50 if humidity else 30
        plan.update(horizon_ms=18500,input_steps=[],checks=[])
        for at,v in zip((0,4500,9000,13500),values):
            plan['input_steps'].append(dict(at_ms=at,pin=2,value=v,kind='humidity' if humidity else 'temperature'))
            checks.append(dict(kind='serial',id=f'comparison_{at}',from_ms=at+500,to_ms=at+3000,
                regex='^'+word+' is '+('above' if v>threshold else 'below')+' the threshold$',min_count=1,baud=9600,
                requirement_quote='Report the correct comparison for the current sensor value'))
    elif case=='pceval_button_pulldown':
        plan.update(horizon_ms=9000,input_steps=[])
        for at,value in ((0,0),(2000,1),(4000,0),(6000,1)):
            plan['input_steps'].append(dict(at_ms=at,pin=4,value=value,kind='digital'))
            for offset in (0,1000):
                checks.append(dict(kind='serial',id=f'state_{at}_{offset}',from_ms=at+offset+100,to_ms=at+offset+500,
                    regex='^Button '+('pressed' if value else 'not pressed')+'$',min_count=1,baud=9600,
                    requirement_quote='Continuously report the supplied externally pulled-down button state'))
    elif case=='pceval_button_duration':
        plan.update(horizon_ms=16000,input_steps=[dict(at_ms=t,pin=4,value=v,kind='digital') for t,v in ((0,1),(500,0),(3500,1),(5500,0),(12500,1))])
        for at,value in ((3500,3),(12500,7)):
            checks.append(dict(kind='serial',id=f'duration_{value}',from_ms=at,to_ms=at+500,
                prefix='Button pressed duration: ',expected=value,tolerance=.1,min_count=1,baud=9600,
                requirement_quote='Print the pressed duration in seconds'))
    elif case=='pceval_serial_RGB_led':
        plan.update(horizon_ms=9500,input_steps=[],serial_inputs=[])
        for at,text,mask in ((1000,'Red',1),(2500,'Red, Green',3),(4000,'Green, Blue',6),(5500,'Blue',4),(7000,'Red, Blue',5)):
            plan['serial_inputs'].append(dict(at_ms=at,text=text+'\n'))
            for bit,pin in enumerate((4,3,2)):
                sample('gpio',at+200,pin,int(not(mask&(1<<bit))))
                sample('gpio',at+1000,pin,int(not(mask&(1<<bit))))
    elif case=='pceval_serial_monitor':
        plan.update(horizon_ms=6500,input_steps=[],serial_inputs=[dict(at_ms=1000,text='hello\n'),dict(at_ms=2500,text='42\n-17.25\n'),dict(at_ms=4000,text='idle value')])
        for at,text in ((1000,'hello'),(2500,'42'),(2500,'-17.25'),(4000,'idle value')):
            checks.append(dict(kind='serial',id=f'echo_{at}_{text}',from_ms=at,to_ms=at+500,
                contains='Received: '+text,min_count=1,baud=9600,requirement_quote='Echo the actual received value'))
    elif case=='simple_013':
        plan.update(horizon_ms=11000,input_steps=[dict(at_ms=t,pin=7,value=v,kind='temperature') for t,v in ((0,21),(3500,33),(7000,18))])
        for at,value in ((1000,21),(4500,33),(8500,18)):
            checks.append(dict(kind='serial',id=f'temperature_{at}',from_ms=at,to_ms=at+1500,expected=value,
                tolerance=.1,min_count=1,requirement_quote='Read the D7 DHT11 temperature and report once per second'))
        checks.append(dict(kind='interval',id='one_second_reporting',source='serial',from_ms=1000,to_ms=10000,
            expected_ms=1000,tolerance_ms=100,min_count=6,requirement_quote='Report every second'))
    elif case=='medium_014':
        plan.update(horizon_ms=7000,input_steps=[],serial_inputs=[dict(at_ms=t,text='BUZZ\n') for t in (1000,3000,5000)])
        for at in (1000,3000,5000):
            sample('tone',at+200,6,1000,1);sample('tone',at+700,6,0)
    elif case=='medium_016':
        plan.update(horizon_ms=16000,input_steps=[dict(at_ms=0,pin=2,value=1,kind='digital')])
        for at in (1000,6000,10000):plan['input_steps'] += [dict(at_ms=at,pin=2,value=0,kind='digital'),dict(at_ms=at+400,pin=2,value=1,kind='digital')]
        sample('gpio',500,13,0)
        checks.append(dict(kind='edges',id='flashing_mode',pin=13,from_ms=1700,to_ms=5500,min_count=4,
            requirement_quote='The selected flashing mode must repeatedly turn the LED on and off'))
        for t,v in ((6700,1),(8500,1),(10700,0),(14500,0)):sample('gpio',t,13,v)
    elif case=='medium_025':
        plan.update(horizon_ms=9000,input_steps=[dict(at_ms=t,pin='A0',value=v,kind='analog') for t,v in ((0,0),(2000,256),(4000,512),(6000,1023))])
        for t,v in ((500,0),(2500,63),(4500,127),(6500,255)):
            sample('pwm',t,9,v,1);sample('display',t,'tm:4:5',str(v))
    elif case=='pceval_7_segment_display_serial':
        plan.update(horizon_ms=11500,input_steps=[],serial_inputs=[])
        for at,value in ((1000,5),(3000,0),(5000,9),(7000,2),(9000,8)):
            plan['serial_inputs'].append(dict(at_ms=at,text=str(value)+'\n'))
            sample('display',at+200,'sevseg1',str(value));sample('display',at+1500,'sevseg1',str(value))
    elif case in ('embedbench_64','embedbench_65'):
        analog=case=='embedbench_65'
        buttons=(2,) if analog else (2,3)
        plan['horizon_ms']=27000
        plan['input_steps']=[dict(at_ms=0,pin=p,value=1,kind='digital') for p in buttons]
        colors=[0,1,2,4,3,6,5,7,0,7,5]
        if analog:colors=colors[:9]
        for n,color in enumerate(colors):
            at=0 if n==0 else 1000+(n-1)*2500
            if n:
                pin=2 if n<=8 else 3
                plan['input_steps'] += [dict(at_ms=at,pin=pin,value=0,kind='digital'),dict(at_ms=at+500,pin=pin,value=1,kind='digital')]
            for bit,pin in enumerate((3,4,5) if analog else (4,5,6)):
                sample('pwm' if analog else 'gpio',at+800,pin,(255 if analog else 1) if color&(1<<bit) else 0)
    elif case=='embedbench_66':
        plan['horizon_ms']=17000
        plan['input_steps']=[dict(at_ms=0,pin=2,value=1,kind='digital')]
        states=[(0,0,0),(90,45,135),(180,90,0),(45,135,90),(0,0,0),(0,0,0),(90,45,135)]
        for index,values in enumerate(states):
            at=0 if index==0 else 1000+(index-1)*2500
            if index:plan['input_steps'] += [dict(at_ms=at,pin=2,value=0,kind='digital'),dict(at_ms=at+400,pin=2,value=1,kind='digital')]
            for pin,value in zip((3,4,5),values):sample('servo',at+700,pin,value,1)
    elif case=='embedbench_68':
        plan['horizon_ms']=11500
        plan['input_steps']=[dict(at_ms=0,pin=p,value=1,kind='digital') for p in (2,3,4)]
        plan['input_steps'] += [dict(at_ms=0,pin=p,value=v,kind='analog') for p,v in (('A3',256),('A4',512),('A5',1023))]
        for pin,at in ((2,1000),(3,3500),(4,6000)):
            plan['input_steps'] += [dict(at_ms=at,pin=pin,value=0,kind='digital'),dict(at_ms=at+700,pin=pin,value=1,kind='digital')]
        plan['input_steps'].append(dict(at_ms=8500,pin='A3',value=1023,kind='analog'))
        for t,values in ((500,(0,0,0)),(1500,(63,0,0)),(2500,(63,0,0)),(4000,(63,127,0)),(5000,(63,127,0)),(6500,(63,127,255)),(9000,(63,127,255))):
            for pin,v in zip((5,6,7),values):sample('pwm',t,pin,v,1)
    elif case=='embedbench_69':
        plan['horizon_ms']=12500
        plan['input_steps']=[dict(at_ms=0,pin=2,value=1,kind='digital'),dict(at_ms=0,pin='A3',value=256,kind='analog')]
        for at in (1000,4000,7000,10000):plan['input_steps'] += [dict(at_ms=at,pin=2,value=0,kind='digital'),dict(at_ms=at+400,pin=2,value=1,kind='digital')]
        plan['input_steps'] += [dict(at_ms=2500,pin='A3',value=512,kind='analog'),dict(at_ms=5500,pin='A3',value=1023,kind='analog')]
        for t,values in ((500,(0,0,0)),(1500,(63,0,0)),(3000,(127,0,0)),(4500,(0,127,0)),(6000,(0,255,0)),(7500,(0,0,255)),(10500,(255,0,0))):
            for pin,v in zip((3,4,5),values):sample('pwm',t,pin,v,1)
    elif case=='embedbench_70':
        plan['horizon_ms']=12500
        plan['input_steps']=[dict(at_ms=0,pin=p,value=1,kind='digital') for p in (2,3,4)]
        plan['input_steps'] += [dict(at_ms=0,pin=p,value=v,kind='analog') for p,v in (('A3',256),('A4',512),('A5',1023))]
        for at,pin in ((1000,2),(4000,3),(7000,4),(10000,2)):
            plan['input_steps'] += [dict(at_ms=at,pin=pin,value=0,kind='digital'),dict(at_ms=at+400,pin=pin,value=1,kind='digital')]
        for t,v in ((500,0),(1500,45),(3000,45),(4500,90),(6000,90),(7500,180),(9000,180),(10500,45)):sample('servo',t,5,v,1)
    elif case=='embedbench_71':
        plan['horizon_ms']=11500
        plan['input_steps']=[dict(at_ms=0,pin=p,value=1,kind='digital') for p in (2,3)]
        plan['input_steps'] += [dict(at_ms=0,pin=p,value=v,kind='analog') for p,v in (('A3',512),('A4',1023))]
        for at,pin in ((1000,2),(3500,3),(8500,2)):
            plan['input_steps'] += [dict(at_ms=at,pin=pin,value=0,kind='digital'),dict(at_ms=at+500,pin=pin,value=1,kind='digital')]
        plan['input_steps'].append(dict(at_ms=6000,pin='A3',value=0,kind='analog'))
        for t,values in ((500,(0,0)),(1700,(90,0)),(4200,(90,180)),(7000,(90,180)),(9200,(0,180))):
            for pin,v in zip((4,5),values):sample('servo',t,pin,v,1)
    elif case=='embedbench_73':
        plan['horizon_ms']=11500
        plan['input_steps']=[dict(at_ms=0,pin=p,value=1,kind='digital') for p in (2,3,4)]
        for at,pin in ((1000,2),(4000,3),(7000,4)):
            plan['input_steps'] += [dict(at_ms=at,pin=pin,value=0,kind='digital'),dict(at_ms=at+400,pin=pin,value=1,kind='digital')]
        for t,angle,count in ((500,0,0),(1700,30,3),(3200,30,3),(4700,60,6),(6200,60,6),(7700,90,10),(10000,90,10)):
            sample('servo',t,45,angle,1)
            for i,pin in enumerate((5,6,7,8,9,10,11,12,13,44)):sample('gpio',t,pin,int(i<count))
    elif case=='embedbench_87':
        plan.update(horizon_ms=27000,input_steps=[dict(at_ms=0,pin=p,value=1,kind='digital') for p in (2,3,4)])
        sequence=[2]*9+[3,4]
        levels=[0,0,0]
        for n,pin in enumerate(sequence):
            at=1000+n*2200
            for dt,v in ((0,0),(2,1),(4,0),(6,1),(8,0),(500,1)):
                plan['input_steps'].append(dict(at_ms=at+dt,pin=pin,value=v,kind='digital'))
            channel=pin-2
            levels[channel]=0 if levels[channel]>=255 else min(255,levels[channel]+32)
            for out,v in zip((5,6,7),levels):sample('pwm',at+300,out,v)
            count=8 if levels[channel]==255 else levels[channel]//32
            for i,out in enumerate((8,9,10,11,12,13,44,45)):sample('gpio',at+300,out,int(i<count))
    elif case=='medium_003':
        plan['horizon_ms']=7000
        plan['input_steps']=[dict(at_ms=t,pin=pin,value=v,kind='analog') for t,pin,v in ((0,'A0',1023),(0,'A1',30),(1000,'A1',61),(2000,'A1',62),(3500,'A1',95),(5000,'A1',30))]
        for t,v in ((500,0),(1500,0),(2700,255),(4200,255),(5700,0)):sample('pwm',t,5,v)
        plan['experimental_condition']='LM35 input values are 10-bit ADC counts, converted using the declared 5V reference, not already-converted Celsius.'
    elif case=='embedbench_101':
        plan['horizon_ms']=81000
        for n in range(20):
            t=n*4000+400
            sample('display',t,'sevseg1',str(9-n%10))
        for n in range(40):
            sample('gpio',n*2000+400,2,int(n%2==0))
            sample('gpio',n*2000+400,3,int(n%2==1))
    elif case=='embedbench_111':
        plan['horizon_ms']=12500
        plan['input_steps']=[dict(at_ms=t,kind='analog',pin='A3',value=v)
            for t,v in ((0,100),(2000,341),(4000,342),(6000,682),(8000,683),(10000,1023))]
        for t,selected in ((500,0),(2500,0),(4500,1),(6500,1),(8500,2),(10500,2)):
            for i in range(3):
                sample('pwm',t,5+i,255 if i==selected else 0)
                sample('gpio',t,2+i,int(i==selected))
    elif case=='embedbench_20':
        plan['horizon_ms']=79000
        for step in range(39):
            n=step%36;sample('servo',step*2000+400,2,(n if n<=18 else 36-n)*10,1)
    elif case=='embedbench_21':
        plan['horizon_ms']=27000
        values=[(0,0,0),(0,45,90),(45,90,135),(90,135,180),(135,180,0),(180,0,45)]
        for step in range(13):
            index=step if step<6 else (step-1)%5+1
            for pin,v in zip((2,3,4),values[index]):sample('servo',step*2000+400,pin,v,1)
    elif case=='embedbench_37':
        plan['horizon_ms']=16500
        plan['input_steps']=[dict(at_ms=t,kind='analog',pin=p,value=v) for t,p,v in ((0,'A3',0),(0,'A4',0),(3000,'A3',512),(3000,'A4',512),(6000,'A3',1023),(6000,'A4',1023),(10000,'A3',0),(10000,'A4',0))]
        for t,adc in ((2500,0),(5500,512),(8500,1023),(12500,0)):
            sample('display',t,'sevseg1',str(adc*9//1023))
            sample('servo',t,2,adc*90/1023,1)
            sample('servo',t,3,90+adc*90/1023,1)
            sample('servo',t,4,180,1)
    elif case=='embedbench_118':
        plan['horizon_ms']=11000
        plan['input_steps']=[dict(at_ms=t,kind='digital',pin=p,value=v) for t,p,v in ((0,3,1),(0,4,1),(500,3,0),(800,3,1),(3500,3,0),(3800,3,1),(6500,4,0),(6800,4,1))]
        for t,angle in ((200,90),(2800,120),(5800,150),(8800,120)):sample('servo',t,5,angle,1)
        for t,v in ((200,0),(900,1),(2300,1),(2900,0),(3900,1),(5300,1),(5900,0),(6900,1),(8300,1),(8900,0)):sample('gpio',t,2,v)
    elif case=='embedbench_119':
        plan['horizon_ms']=14000
        plan['input_steps']=[dict(at_ms=t,kind='digital',pin=p,value=v) for t,p,v in ((0,3,1),(0,4,1),(500,3,0),(800,3,1),(3500,4,0),(3800,4,1),(8500,4,0),(8800,4,1))]
        for t,a,b,led in ((200,0,0,0),(900,90,0,1),(2900,90,0,1),(3900,0,0,1),(5900,0,0,0),(7900,0,0,1),(8900,0,180,0),(11500,0,180,0)):
            sample('servo',t,5,a,1);sample('servo',t,6,b,1);sample('gpio',t,2,led)
    elif case=='embedbench_12':
        plan['horizon_ms']=15500
        plan['input_steps']=[dict(at_ms=0,kind='digital',pin=2,value=1)]
        for i in range(6):
            t=1000+i*2200
            for dt,value in ((0,0),(2,1),(4,0),(6,1),(8,0),(500,1)):
                plan['input_steps'].append(dict(at_ms=t+dt,kind='digital',pin=2,value=value))
            sample('display',t+250,'sevseg1',str((i+1)*3%12))
            sample('display',t+1200,'sevseg1',str((i+1)*3%12))
        sample('display',300,'sevseg1','0')
    elif case=='embedbench_123':
        plan['horizon_ms']=69000
        plan['input_steps']=[dict(at_ms=0,kind='digital',pin=4,value=1)]
        for i in range(33):
            t=1000+i*2000
            plan['input_steps'] += [dict(at_ms=t,kind='digital',pin=4,value=0),dict(at_ms=t+500,kind='digital',pin=4,value=1)]
            count=(i+1)%11;overflow=(i+1)//11%4
            for index,pin in enumerate((5,6,7,8,9,10,11,12,13,44)):sample('gpio',t+300,pin,int(index<count))
            sample('gpio',t+300,2,overflow&1);sample('gpio',t+300,3,(overflow>>1)&1)
    else:return None
    plan['checks']=checks
    plan.pop('reference_cpp',None)
    plan['coverage']='targeted_core_regression'
    plan['test_origin']='manual_requirement_derived_supplement; retained original test plan is unchanged'
    return plan
