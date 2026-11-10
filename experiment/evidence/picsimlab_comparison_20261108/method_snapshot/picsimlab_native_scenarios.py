"""Interpret untimed descriptive benchmark states as bounded eventual states.

The source's explicit timestamps and every other executable plan remain intact.
The original translated plan is retained as an audit artifact.
"""
from bisect import bisect_left,bisect_right
import json
import math

from picsimlab_assertions import pin


def prepare(entry):
    plan=json.loads(json.dumps(entry['plan']))
    entries=plan.get('native_test_entries',[])
    if plan.get('origin')!='supplied_EmbedBench_descriptive_tests_and_original_reference' or not entries:
        return plan
    if any("'time':" in text or '"time":' in text for text in entries):return plan
    if not plan['checks'] or any(c['kind'] not in ('gpio','pwm','servo','display') for c in plan['checks']):return plan
    times=sorted({c['at_ms'] for c in plan['checks']})
    translated={t:1000+index*8000 for index,t in enumerate(times)}
    groups={t:[] for t in times}
    steps=[]
    for step in plan.get('input_steps',[]):
        if step['at_ms']==0:steps.append(step);continue
        index=min(len(times)-1,bisect_left(times,step['at_ms']))
        groups[times[index]].append(step)
    windows={}
    for index,old in enumerate(times):
        target=translated[old];source_steps=groups[old]
        if source_steps:
            old_begin=min(s['at_ms'] for s in source_steps)
            new_begin=0 if index==0 else translated[times[index-1]]+500
            for step in source_steps:steps.append(dict(step,at_ms=new_begin+step['at_ms']-old_begin))
            begin=new_begin+20
        else:begin=0 if index==0 else translated[times[index-1]]+20
        windows[old]=(begin,target,index)
    plan['input_steps']=sorted(steps,key=lambda x:x['at_ms'])
    for check in plan['checks']:
        old=check['at_ms'];begin,end,index=windows[old]
        check.update(at_ms=end,native_eventual_group=index,native_eventual_from_ms=begin,native_eventual_to_ms=end)
    plan['horizon_ms']=max(translated.values())+1000
    plan['timing_interpretation']=dict(policy='untimed_descriptions_eventual_states_v1',
        observation_budget_per_action_ms=8000,original_horizon_ms=entry['plan']['horizon_ms'],
        reason='Source descriptive entries give no timestamps. The earlier fixed timestamps were inferred, not source deadlines. Preserve expected states and order; observe physical completion within a disclosed finite window.')
    return plan


def replace_results(plan,trace,board,result):
    if not plan.get('timing_interpretation'):return
    signals={}
    for event in trace:
        kind=event['type']
        if kind in ('gpio','pwm','servo','display'):
            key=(kind,event.get('name') if kind=='display' else event['pin'])
            signals.setdefault(key,[]).append(event)
    for events in signals.values():events.sort(key=lambda e:e['at_ms'])
    def value(check,t):
        kind=check['kind'];name=check.get('name') if kind=='display' else pin(check['pin'],board)
        if kind=='display':
            parts=[]
            for item in name.split(','):
                events=signals.get(('display',item.strip()),[]);index=bisect_right(events,t,key=lambda e:e['at_ms'])
                if not index:return None
                parts.append(events[index-1]['text'])
            return ''.join(parts).strip()
        events=signals.get((kind,name),[]);index=bisect_right(events,t,key=lambda e:e['at_ms'])
        if kind=='gpio':
            measured=signals.get(('pwm',name),[]);j=bisect_right(measured,t,key=lambda e:e['at_ms'])
            if j:return int(measured[j-1]['value']>0)
        if not index:return None
        event=events[index-1]
        return event.get('angle') if kind=='servo' else event['value']
    def matches(check,actual):
        if actual is None:return False
        expected=check.get('expected')
        if check.get('numeric'):
            try:actual=float(actual);expected=float(expected) if expected is not None else None
            except (ValueError,TypeError):return False
        if expected is not None:
            if isinstance(expected,(int,float)):
                try:return abs(float(actual)-expected)<=check.get('tolerance',0)
                except (ValueError,TypeError):return False
            return str(actual).strip()==str(expected).strip()
        return ('min' not in check or actual>=check['min']) and ('max' not in check or actual<=check['max'])
    groups={}
    for row in result['checks']:groups.setdefault(row['test']['native_eventual_group'],[]).append(row)
    for rows in groups.values():
        begin=rows[0]['test']['native_eventual_from_ms'];end=rows[0]['test']['native_eventual_to_ms'];witness=None
        for t in range(math.ceil(begin),math.floor(end)+1,20):
            actual=[value(row['test'],t) for row in rows]
            if all(matches(row['test'],v) for row,v in zip(rows,actual)):
                witness=(t,actual);break
        for index,row in enumerate(rows):
            row['original_point_actual']=row['actual'];row['passed']=witness is not None
            row['actual']=dict(witness_at_ms=witness[0],value=witness[1][index]) if witness else None
            row['timing_interpretation']='combined_eventual_state'
            if witness:row.pop('unverified',None)
            else:row['unverified']='untimed_source_completion_not_observed_within_disclosed_finite_window'
