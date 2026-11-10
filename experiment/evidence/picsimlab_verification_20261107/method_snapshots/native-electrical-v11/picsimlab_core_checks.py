"""Requirement-derived checks on measured electrical and display observations."""
import bisect
import math

KINDS={'sample_relation','pwm_ramp_repeat','display_led_parity','buzzer_bursts','display_refresh','display_activity'}


def evaluate(check,trace,board):
    import picsimlab_assertions as a
    c=check;kind=c['kind'];p=a.pin(c.get('pin',0),board)
    def value(source,t):
        events=[e for e in trace if e['type']==source and (e.get('pin')==p if source!='display' else e.get('name')==c['name']) and e.get('at_ms',0)<=t]
        if not events:return None
        e=max(events,key=lambda e:(e.get('at_ms',0),e.get('sequence',0)))
        return e['text'].strip() if source=='display' else e['angle'] if source=='servo' else e['value']
    actual=None;ok=False
    if kind in ('sample_relation','pwm_ramp_repeat'):
        samples=[value(c.get('source','pwm'),t) for t in c['at_ms']]
        try:values=list(map(float,samples))
        except (ValueError,TypeError):values=[]
        actual=dict(samples=samples,times_ms=c['at_ms'])
        if len(values)==len(samples) and values:
            if kind=='sample_relation':
                differences=[b-a for a,b in zip(values,values[1:])];relation=c['relation']
                ok=all(x>=c.get('min_gap',1) for x in differences) if relation=='increasing' else all(x<=-c.get('min_gap',1) for x in differences) if relation=='decreasing' else all(x>=-c.get('tolerance',0) for x in differences) and len(set(values))>=c.get('min_distinct',2) if relation=='nondecreasing' else all(x<=c.get('tolerance',0) for x in differences) and len(set(values))>=c.get('min_distinct',2) if relation=='nonincreasing' else max(values)-min(values)<=c.get('tolerance',0) if relation=='equal' else len(set(values))>=c.get('min_distinct',2)
            else:
                phase=0;cycles=0
                for v in values:
                    if phase==0 and v<=10:phase=1
                    elif phase==1 and v>=245:phase=2
                    elif phase==2 and v<=10:cycles+=1;phase=1
                actual.update(distinct_levels=len(set(values)),completed_ramps=cycles,minimum=min(values),maximum=max(values))
                ok=cycles>=2 and len(set(values))>=40
    elif kind=='display_led_parity':
        pairs=[]
        for t in c['at_ms']:
            try:pairs.append((int(float(value('display',t))),int(value('gpio',t))))
            except (ValueError,TypeError):pairs.append((None,None))
        mapping={parity:{level for number,level in pairs if number is not None and number%2==parity} for parity in (0,1)}
        ok=all(n is not None and v is not None for n,v in pairs) and len(mapping[0])==len(mapping[1])==1 and mapping[0]!=mapping[1]
        actual=dict(number_and_led=pairs,parity_levels={str(k):sorted(v) for k,v in mapping.items()})
    elif kind=='buzzer_bursts':
        edges=sorted(e['at_ms'] for e in trace if e['type']=='gpio' and e['pin']==p and c['from_ms']<=e['at_ms']<=c['to_ms'])
        starts=[];last=-1e9
        for t in edges:
            if t-last>c.get('separation_ms',100):starts.append(t)
            last=t
        intervals=[b-a for a,b in zip(starts,starts[1:])]
        actual=dict(starts_ms=starts,intervals_ms=intervals)
        ok=len(starts)>=c.get('min_count',1) and ('period_ms' not in c or intervals and all(abs(dt-c['period_ms'])<=c.get('tolerance_ms',50) for dt in intervals))
    elif kind=='display_refresh':
        events=sorted((e for e in trace if e['type']=='display' and e['name']==c['name'] and c['from_ms']<=e['at_ms']<=c['to_ms']),key=lambda e:e['at_ms'])
        changes=[];old=None
        for e in events:
            if e['text']!=old:
                try:float(e['text'].strip())
                except ValueError:continue
                changes.append(e['at_ms']);old=e['text']
        intervals=[b-a for a,b in zip(changes,changes[1:])]
        actual=dict(changes_ms=changes,intervals_ms=intervals)
        ok=len(changes)>=c.get('min_count',3) and all(abs(dt-c['period_ms'])<=c.get('tolerance_ms',100) for dt in intervals)
    elif kind=='display_activity':
        frames=[e for e in trace if e['type']=='display' and e['name']==c['name'] and c.get('from_ms',0)<=e['at_ms']<=c['to_ms'] and e['text'].strip(' ?-')]
        distinct={e['text'] for e in frames}
        actual=dict(frames=len(frames),distinct_texts=sorted(distinct),examples=frames[:3])
        ok=len(distinct)>=c.get('min_distinct',1)
    return dict(id=c['id'],requirement_quote=c['requirement_quote'],test=c,actual=actual,passed=bool(ok))
