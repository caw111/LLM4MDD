"""Bounded public periodic-output assertions, independent of controller generation.

The reference uses elapsed active time and a mathematical pulse train. It never
reads controller state names, model constants, or independent hardware test data.
"""
import math


def validate_periodic_checks(contract):
    checks = contract.get('periodic_checks', [])
    if not isinstance(checks, list) or len(checks) > 16:
        raise ValueError('periodic_checks must be a bounded list')
    ports = {p['name']: p for p in contract['interfaces']}
    step = contract['control_period_ms']
    if checks and (type(step) is not int or not 1 <= step <= 2147483647):
        raise ValueError('periodic_checks require an integer i32 millisecond control period')
    required = {'delta','output','period_ms','pulse_ms','first_pulse','active_value','inactive_value','tolerance_ms','evidence'}
    optional = {'pause_input','initial_running','pause_policy'}
    for check in checks:
        if not isinstance(check,dict) or required - check.keys() or check.keys() - required - optional:
            raise ValueError('periodic_checks require '+str(sorted(required))+'; optional '+str(sorted(optional)))
        for field, direction, typ in [('delta','input','i32'),('output','output',None),('pause_input','input','bool')]:
            if field not in check: continue
            name=check[field]
            if not isinstance(name,str) or name not in ports or ports[name]['direction']!=direction or (typ and ports[name]['type']!=typ):
                raise ValueError('Invalid periodic_checks '+field+' port')
        if ports[check['delta']]['unit']!='ms' or ports[check['delta']]['initial']!=0:
            raise ValueError('periodic_checks delta requires i32 ms with initial 0')
        for field in ('period_ms','pulse_ms','tolerance_ms'):
            if type(check[field]) is not int or not 0<=check[field]<=2147483647:
                raise ValueError('periodic_checks '+field+' must be a nonnegative i32')
        period,width,tolerance=(check[k] for k in ('period_ms','pulse_ms','tolerance_ms'))
        if not 0 < width < period or 2*tolerance+step>=min(width,period-width):
            raise ValueError('periodic_checks need observable active and inactive windows beyond timing tolerance and control step')
        if check['first_pulse'] not in ('immediate','after_period','after_silence'):
            raise ValueError('periodic_checks first_pulse must be immediate, after_period or after_silence')
        typ=ports[check['output']]['type']
        for field in ('active_value','inactive_value'):
            value=check[field]
            valid=type(value) is bool if typ=='bool' else type(value) in (int,float) and math.isfinite(value)
            if typ=='i32':valid=type(value) is int and -(2**31)<=value<2**31
            if not valid:raise ValueError('periodic_checks value has wrong output type/range')
        from app.services.ir_execution import typed
        if typed(check['active_value'],typ)[0]==typed(check['inactive_value'],typ)[0]:
            raise ValueError('periodic_checks must distinguish active and inactive outputs')
        if not isinstance(check['evidence'],str) or not check['evidence'].strip():
            raise ValueError('periodic_checks require explicit source evidence')
        if 'pause_input' in check:
            if type(check.get('initial_running')) is not bool or check.get('pause_policy') not in ('preserve','reset'):
                raise ValueError('periodic_checks pause requires initial_running and preserve/reset policy')
            if ports[check['pause_input']]['initial'] is not False:
                raise ValueError('periodic_checks press-edge probes require initially released normalized input')
        elif optional & check.keys():
            raise ValueError('periodic_checks pause options require pause_input')
    return checks


def reference_trace(check, step, *, paused=False, jitter=False, pause_offset_ms=None):
    period,width=check['period_ms'],check['pulse_ms']
    initial_running=check.get('initial_running',True)
    start_offset=0 if initial_running else 3*step
    first={'immediate':0,'after_period':period,'after_silence':period-width}[check['first_pulse']]
    align=lambda t: max(step,((t+step-1)//step)*step)
    presses=[] if initial_running else [(start_offset,start_offset+3*step)]
    if paused:
        offset=width//2 if pause_offset_ms is None else pause_offset_ms
        if type(offset) is not int or not 0<=offset<width:
            raise ValueError('Pause probe offset must fall inside the active pulse')
        at=align(start_offset+first+offset)
        resume=align(at+period//3+7*step)
        second=align(resume+period+period//2)
        last=align(second+period//4+7*step)
        presses += [(at,at+3*step),(resume,resume+3*step),(second,second+3*step),(last,last+3*step)]
    # Probe eight further cycles so short finite pulse enumerations fail; this is
    # still bounded evidence, not a proof of arbitrary unbounded execution.
    horizon=(presses[-1][1] if presses else start_offset)+8*period+width
    if horizon//step+2 > 100000:
        raise ValueError('Public periodic assertion exceeds the 100000-step verification budget; assertion is unverified, not passed')
    trace=[];edges=[];now=0;elapsed=0;running=initial_running;previous=False;index=0
    # Do not invent a scheduling stall longer than the stated response bound.
    # With a zero/sub-step tolerance only the nominal control schedule is used.
    jitter_multiple=max(1,min(3,check['tolerance_ms']//step))
    def active():return running and elapsed>=first and (elapsed-first)%period<width
    while now<=horizon:
        dt=0 if index==0 else step*([1,jitter_multiple,1,jitter_multiple][(index-1)%4] if jitter else 1)
        now+=dt
        if running:
            before=elapsed
            elapsed+=dt
            for offset in (first,first+width):
                n=max(0,(before-offset)//period+1)
                boundary=offset+n*period
                while boundary<=elapsed:
                    edges.append(now-dt+boundary-before)
                    boundary+=period
        pressed=any(a<=now<b for a,b in presses)
        before_toggle=active()
        if pressed and not previous:
            running=not running
            if not running and check.get('pause_policy')=='reset':elapsed=0
        previous=pressed
        on=active()
        if on!=before_toggle:edges.append(now)
        trace.append((now,dt,pressed,check['active_value'] if on else check['inactive_value']))
        index+=1
    return trace,edges


def check_periodic(ir, contract):
    from app.services.ir_execution import IRExecution, typed
    report={'status':'not_requested','checked':0,'scope':'Bounded public pulse, phase and pause/resume traces; no hardware or unbounded proof.'}
    issues=[]
    checks=contract.get('periodic_checks',[])
    if not checks:return report,issues
    report['status']='passed'
    try:
        for check in checks:
            output_type=next(p['type'] for p in contract['interfaces'] if p['name']==check['output'])
            probes=[(False,False,None),(False,True,None)]
            if 'pause_input' in check:
                # A half-pulse remainder can be entirely hidden by two edge
                # tolerance windows plus sampling. Vary the pause phase from
                # public pulse parameters, not an independent test's timings.
                offsets=sorted({check['pulse_ms']//4,check['pulse_ms']//2,3*check['pulse_ms']//4})
                probes += [(True,jitter,offset) for offset in offsets for jitter in (False,True)]
            for paused,jitter,pause_offset in probes:
                trace,edges=reference_trace(check,contract['control_period_ms'],paused=paused,jitter=jitter,pause_offset_ms=pause_offset)
                # A sample may differ within the explicit tolerance of a
                # mathematical edge. No implementation-specific tolerance.
                engine=IRExecution(ir);history=[]
                for at,dt,pressed,expected in trace:
                    expected=typed(expected,output_type)[0]
                    inputs={p['name']:p['initial'] for p in contract['interfaces'] if p['direction']=='input'}
                    inputs[check['delta']]=dt
                    if 'pause_input' in check:inputs[check['pause_input']]=pressed
                    actual=engine.step(inputs)[check['output']]
                    history.append(dict(at_ms=at,inputs=inputs,expected=expected,actual=actual))
                    if check['tolerance_ms'] and any(abs(at-edge)<=check['tolerance_ms'] for edge in edges):continue
                    report['checked']+=1
                    if actual!=expected:
                        report.update(status='failed',counterexample=dict(check=check,paused=paused,jitter=jitter,pause_offset_ms=pause_offset,history=history[-8:]))
                        import json
                        issues.append(dict(severity='error',code='CONTRACT_PERIODIC',stage='requirements',
                            message='Public period/pulse/pause assertion violated: '+json.dumps(report['counterexample'],ensure_ascii=False)))
                        return report,issues
    except (ValueError,ArithmeticError) as exc:
        report['status']='execution_error'
        issues.append(dict(severity='error',code='CONTRACT_EXECUTION',stage='requirements',message=str(exc)))
    return report,issues
