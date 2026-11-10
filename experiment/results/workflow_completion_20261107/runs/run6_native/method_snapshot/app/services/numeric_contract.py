"""Bounded, source-backed numeric obligations; never read benchmark/oracle data.

These checks exercise public properties of IR, not a full behavioral proof.
The narrow prose recognizers require all relevant parameters and pin roles.
"""
import copy
import json
import re


FIELDS = {
    'counter_edges': {'kind','output','increment','decrement','reset','delta','minimum','maximum',
                      'initial','min_repeat_ms','response_ms','evidence'},
    'mapped_parity': {'kind','output','input','even_input','odd_input','even_output','odd_output',
                      'adc_max','display_max','pwm_max','pwm_tolerance','response_ms','evidence'},
}


def validate_numeric_checks(contract):
    checks=contract.get('numeric_checks',[])
    if not isinstance(checks,list) or len(checks)>16:
        raise ValueError('numeric_checks must be a bounded list')
    ports={p['name']:p for p in contract['interfaces']}
    step=contract['control_period_ms'];seen=set()
    if checks and (type(step) is not int or not 1<=step<=2147483647):
        raise ValueError('numeric_checks require an integer i32 control period')
    for check in checks:
        if not isinstance(check,dict) or not isinstance(check.get('kind'),str) or check['kind'] not in FIELDS:
            raise ValueError('Unknown numeric_checks kind')
        kind=check['kind']
        if set(check)!=FIELDS[kind]:raise ValueError('numeric_checks '+kind+' requires '+str(sorted(FIELDS[kind])))
        if not isinstance(check['evidence'],str) or not check['evidence'].strip():
            raise ValueError('numeric_checks require explicit source evidence')
        def port(field,direction,types,unit=None):
            name=check[field];p=ports.get(name) if isinstance(name,str) else None
            if not p or p['direction']!=direction or p['type'] not in types or (unit and p['unit']!=unit):
                actual={k:p.get(k) for k in ('name','direction','type','unit')} if p else name
                guidance=(' Expose the real raw ADC input with unit adc_count and analog_input scale=1, offset=0; '
                          'compute display mapping, parity and PWM mapping in SysML, preserving this physical pin.'
                          if unit=='adc_count' else '')
                raise ValueError('numeric_checks.'+kind+'.'+field+' requires interface direction='+direction+
                    ', type in '+str(types)+(', unit='+unit if unit else '')+
                    '; actual='+json.dumps(actual,ensure_ascii=False)+'.'+guidance)
            return p
        def integer(field,low,high):
            value=check[field]
            if type(value) is not int or not low<=value<=high:
                raise ValueError('Invalid numeric_checks '+field+' integer range')
        port('output','output',('i32',))
        identity=(kind,check['output'])
        if identity in seen:raise ValueError('Duplicate numeric_checks output obligation')
        seen.add(identity)
        integer('response_ms',step,10000)
        if kind=='counter_edges':
            for name in ('increment','decrement','reset'):
                p=port(name,'input',('bool',),'1')
                if p['initial'] is not False:raise ValueError('counter_edges probes need initially released normalized buttons')
            if len({check[k] for k in ('increment','decrement','reset','delta')})!=4:
                raise ValueError('counter_edges needs distinct button and time inputs')
            port('delta','input',('i32',),'ms')
            for field in ('minimum','maximum','initial'):integer(field,-2147483647,2147483646)
            if not check['minimum']<=0<=check['maximum'] or not check['minimum']<check['maximum']:
                raise ValueError('counter_edges needs a range containing reset value zero')
            if not check['minimum']<=check['initial']<=check['maximum']:
                raise ValueError('counter_edges initial must be in range')
            # Reaching a wrap from the specified initial state must be bounded.
            if check['maximum']-check['minimum']>1000:raise ValueError('counter_edges range exceeds the public probe budget')
            integer('min_repeat_ms',1,2147483647)
            if check['response_ms']>=check['min_repeat_ms']:
                raise ValueError('counter_edges needs a response window shorter than the earliest repeat')
        else:
            for field in ('input','even_input','odd_input'):
                p=port(field,'input',('i32','f32','f64'),'adc_count')
                b=p.get('binding',{})
                if b.get('kind')!='analog_input' or b.get('scale',1)!=1 or b.get('offset',0)!=0:
                    raise ValueError('numeric_checks '+field+' requires raw ADC samples with analog_input, scale=1, offset=0; '
                                     'actual binding='+json.dumps(b,ensure_ascii=False)+'; keep task mapping in SysML')
            for field in ('even_output','odd_output'):
                p=port(field,'output',('i32','f32','f64'),'pwm_count');b=p.get('binding',{})
                if b.get('kind')!='pwm_output' or b.get('scale',1)!=1 or b.get('offset',0)!=0:
                    raise ValueError('numeric_checks '+field+' requires model PWM counts with pwm_output, scale=1, offset=0; '
                                     'actual binding='+json.dumps(b,ensure_ascii=False)+'; keep parity and mapping in SysML')
            if len({check[k] for k in ('input','even_input','odd_input')})!=3 or check['even_output']==check['odd_output']:
                raise ValueError('mapped_parity requires distinct sampled inputs and actuator outputs')
            integer('adc_max',1,65535);integer('display_max',1,9999);integer('pwm_max',1,255)
            integer('pwm_tolerance',0,check['pwm_max']//4)
    return checks


def add_numeric_checks(contract,requirement):
    from app.services.interface_planner import public_match
    from app.services.hardware_adapter import physical_pin
    def one(pattern):
        matches=list(re.finditer(pattern,requirement,re.I));m=public_match(pattern,requirement)
        if len(matches)!=1 or m is None:return None
        start=max(requirement.rfind(c,0,m.start()) for c in '\n。；;.!?')+1
        if re.search(r'如果|若|当|\b(?:if|when)\b',requirement[start:m.start()],re.I):return None
        return m
    ports=contract['interfaces'];result=copy.deepcopy(contract)
    displays=[p for p in ports if p.get('binding',{}).get('kind') in ('shift595_number','tm1637_number')]
    if len(displays)!=1:return result
    output=displays[0]['name'];board=contract.get('board','arduino:avr:uno')
    def bound(pin,kind):
        found=[p for p in ports if p.get('binding',{}).get('kind')==kind and
               physical_pin(p['binding']['pin'],board)==physical_pin(pin,board)]
        if len(found)!=1:raise ValueError('Public numeric requirement needs one '+kind+' interface on '+pin)
        return found[0]['name']
    def add(check):
        checks=result.setdefault('numeric_checks',[])
        if not isinstance(checks,list):raise ValueError('numeric_checks must be a bounded list')
        same=[c for c in checks if isinstance(c,dict) and c.get('kind')==check['kind'] and c.get('output')==output]
        if any(any(c.get(k)!=v for k,v in check.items() if k!='evidence') for c in same):
            raise ValueError('Supplied numeric_checks contradict the explicit public '+check['kind']+' requirement')
        if not same:checks.append(check)
    pins=one(r'(D\d+)加一[、，,](D\d+)减一[、，,](D\d+)清零')
    wrap=one(r'上溢(\d+)回(\d+)[，,]下溢(\d+)回(\d+)')
    initial=one(r'初始(\d+)[，,；;]')
    rate=one(r'重复间隔(\d+)\+floor\(ADC\*\d+/\d+\)毫秒')
    tolerance=one(r'响应及重复时间容差(\d+)ms')
    immediate=one(r'按下立即执行一步');reset=one(r'清零优先并重置重复计时')
    simultaneous=one(r'加减同时按下时不步进')
    polarity=one(r'均INPUT_PULLUP低电平按下')
    clocks=[p for p in ports if p.get('binding',{}).get('kind')=='elapsed_ms']
    if all((pins,wrap,initial,rate,tolerance,immediate,reset,simultaneous,polarity)) and len(clocks)==1:
        hi,lo,lo2,hi2=map(int,wrap.groups())
        if (hi,lo)!=(hi2,lo2):raise ValueError('Contradictory public counter wrap endpoints')
        for pin in pins.groups():
            name=bound(pin,'digital_input');binding=next(p['binding'] for p in ports if p['name']==name)
            if binding.get('pullup') is not True or binding.get('active_high',True) is not False:
                raise ValueError('Public pressed input '+name+' on '+pin+' requires pullup=true and active_high=false')
        add(dict(kind='counter_edges',output=output,increment=bound(pins[1],'digital_input'),
                 decrement=bound(pins[2],'digital_input'),reset=bound(pins[3],'digital_input'),delta=clocks[0]['name'],
                 minimum=lo,maximum=hi,initial=int(initial[1]),min_repeat_ms=int(rate[1]),response_ms=int(tolerance[1]),
                 evidence='; '.join(m[0] for m in (pins,wrap,initial,rate,tolerance,immediate,reset,simultaneous,polarity))))
    mapping=one(r'(A\d+)映射floor\(ADC\*(\d+)/(\d+)\)得到显示整数0\.\.(\d+)')
    pwm=one(r'(A\d+)[、，,](A\d+)分别控制(D\d+)[、，,](D\d+)的PWM[，,]原始ADC0\.\.(\d+)线性映射0\.\.(\d+)[，,]容差(\d+)计数')
    parity=one(r'偶数只允许(D\d+)按(A\d+)设定亮度输出[，,](D\d+)为0[；;]奇数相反[，,](D\d+)为0[、，,](D\d+)按(A\d+)亮度输出')
    response=one(r'变化(\d+)ms内反映')
    if all((mapping,pwm,parity,response)):
        if (mapping[2]!=mapping[4] or mapping[3]!=pwm[5] or
                (parity[1],parity[2],parity[3],parity[4],parity[5],parity[6])!=
                (pwm[3],pwm[1],pwm[4],pwm[3],pwm[4],pwm[2])):
            raise ValueError('Contradictory public ADC mapping/parity pin roles')
        add(dict(kind='mapped_parity',output=output,input=bound(mapping[1],'analog_input'),
                 even_input=bound(pwm[1],'analog_input'),odd_input=bound(pwm[2],'analog_input'),
                 even_output=bound(pwm[3],'pwm_output'),odd_output=bound(pwm[4],'pwm_output'),
                 adc_max=int(mapping[3]),display_max=int(mapping[2]),pwm_max=int(pwm[6]),
                 pwm_tolerance=int(pwm[7]),response_ms=int(response[1]),
                 evidence='; '.join(m[0] for m in (mapping,pwm,parity,response))))
    validate_numeric_checks(result)
    return result


def check_numeric(ir,contract):
    checks=validate_numeric_checks(contract)
    report=dict(status='not_requested' if not checks else 'passed',checked=0,
                scope='Public short-edge counter and sampled mapping/parity assertions only; no full hold timing or hardware proof.')
    issues=[]
    if not checks:return report,issues
    from app.services.ir_execution import IRExecution
    initial={p['name']:p['initial'] for p in contract['interfaces'] if p['direction']=='input'}
    step=contract['control_period_ms'];template=IRExecution(ir)
    def reject(check,history):
        report.update(status='failed',counterexample=dict(check=check,history=history[-8:]))
        issues.append(dict(severity='error',stage='requirements',code='CONTRACT_NUMERIC',
                           message='Public numeric assertion violated: '+json.dumps(report['counterexample'],ensure_ascii=False)))
    try:
        for check in checks:
            frames=check['response_ms']//step
            if check['kind']=='counter_edges':
                engine=copy.deepcopy(template);expected=check['initial'];history=[]
                actual=engine.step(initial)[check['output']]
                history.append(dict(inputs=initial.copy(),expected=expected,actual=actual));report['checked']+=1
                if actual!=expected:reject(check,history);return report,issues
                down=check['initial']-check['minimum']+1
                # Reach lower wrap, cross upper wrap, vary ordinary directions,
                # then check simultaneous inputs and reset priority.
                actions=[(-1,False)]*down+[(1,False),(1,False),(-1,False),(2,False),
                        (1,False),(1,True),(-1,False),(-1,True),(1,False),(0,True)]
                if len(actions)*2*frames>100000:raise ValueError('Public counter probe exceeds 100000-step budget')
                for direction,reset in actions:
                    if reset:expected=0
                    elif direction in (-1,1):
                        expected+=direction
                        if expected<check['minimum']:expected=check['maximum']
                        if expected>check['maximum']:expected=check['minimum']
                    for pressed in (True,False):
                        stimulus={**initial,check['delta']:step,
                                  check['increment']:pressed and direction in (1,2),
                                  check['decrement']:pressed and direction in (-1,2),check['reset']:pressed and reset}
                        for _ in range(frames):actual=engine.step(stimulus)[check['output']]
                        history.append(dict(inputs=stimulus,expected=expected,actual=actual));report['checked']+=1
                        if actual!=expected:reject(check,history);return report,issues
            else:
                maximum=check['adc_max'];limit=check['display_max'];points={0,1,maximum,maximum//2,maximum//3}
                for n in range(min(limit,32)+1):
                    edge=(n*maximum+limit-1)//limit
                    points.update(x for x in (edge-1,edge,edge+1) if 0<=x<=maximum)
                levels=((0,maximum),(maximum,maximum//3),(maximum//5,maximum))
                if len(points)*len(levels)*frames>100000:raise ValueError('Public numeric probe exceeds 100000-step budget')
                engine=copy.deepcopy(template);engine.step(initial)
                elapsed={p['name']:step for p in contract['interfaces'] if p['direction']=='input'
                         and p.get('binding',{}).get('kind')=='elapsed_ms'}
                for raw in sorted(points):
                    number=raw*limit//maximum
                    for a,b in levels:
                        stimulus={**initial,**elapsed,check['input']:raw,check['even_input']:a,check['odd_input']:b}
                        for _ in range(frames):actual=engine.step(stimulus)
                        expected={check['output']:number,check['even_output']:a*check['pwm_max']//maximum if number%2==0 else 0,
                                  check['odd_output']:b*check['pwm_max']//maximum if number%2 else 0}
                        observed={k:actual[k] for k in expected};report['checked']+=1
                        wrong=observed[check['output']]!=number
                        for field in ('even_output','odd_output'):
                            name=check[field];physical=max(0,min(255,int(observed[name])))
                            tolerance=check['pwm_tolerance'] if expected[name]!=0 else 0
                            wrong|=abs(physical-expected[name])>tolerance
                        if wrong:
                            reject(check,[dict(inputs=stimulus,expected=expected,actual=observed)])
                            return report,issues
    except (ValueError,ArithmeticError) as exc:
        report['status']='execution_error'
        issues.append(dict(severity='error',code='CONTRACT_EXECUTION',stage='requirements',message=str(exc)))
    return report,issues
