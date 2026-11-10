def evaluate(plan,trace,board):
    by={k:[e for e in trace if e['type']==k] for k in {e['type'] for e in trace}}
    execution=by['execution'][-1]
    state_checks=any(c['kind'] in ('state_witness','state_invariant','event_window') for c in plan['checks'])
    ordered=[e for e in trace if e['type'] in ('gpio','servo')]
    ordered.sort(key=lambda e:(e['at_ms'],e.get('sequence',0)))
    anchor_events=sorted(ordered+by.get('serial',[]),key=lambda e:(e['at_ms'],e.get('sequence',0)))
    ordered_metadata=bool(by.get('reset_state')) and all('sequence' in e for e in ordered)
    snapshots=[]
    if state_checks and ordered_metadata:
        state={('gpio',p):0 for p in range(70 if 'mega' in board else 20)}
        snapshots=[(0,dict(state))]
        for e in ordered:
            key=(e['type'],e['pin'])
            value=int(e['value']>0) if e['type']=='gpio' else e['angle'] if e.get('active') and e.get('operation')=='write' else None
            if state.get(key)!=value:
                state[key]=value;snapshots.append((e['at_ms'],dict(state)))
    def state_match(conditions,state):
        return all(within(condition,state.get((condition['source'],pin(condition['pin'],board)))) for condition in conditions)
    def command_anchor(definition,after=-1):
        previous=None;found=[]
        for i,e in enumerate(anchor_events):
            if e['type']!=definition['source']:continue
            if e['type']=='serial':
                if i>after and matching(definition,e):found.append(i)
                continue
            if e['pin']!=pin(definition['pin'],board):continue
            if e['type']=='servo' and (not e.get('active') or e.get('operation')!='write'):continue
            value=e['angle'] if e['type']=='servo' else int(e['value']>0)
            if previous is not None and abs(value-previous)<=definition.get('tolerance',0):continue
            previous=value
            if i>after and within(definition,value):found.append(i)
        n=definition.get('occurrence',1)
        return found[n-1] if len(found)>=n else None
    indexed={}
    for kind,events in by.items():
        events.sort(key=lambda e:e.get('at_ms',0))
        for event in events:
            if 'pin' in event:indexed.setdefault((kind,event['pin']),[]).append(event)
    def last(events,t):
        index=bisect.bisect_right(events,t,key=lambda e:e.get('at_ms',0))
        return events[index-1] if index else None
    def selected(kind,p=None):return by.get(kind,[]) if p is None else indexed.get((kind,p),[])
    def scalar(source,c,t):
        p=pin(c.get('pin',0),board)
        if source in ('gpio','pwm'):
            e=last(selected('gpio',p),t)
            if e is None:return None
            return (e['value'] if e['pwm'] else (255 if e['value'] else 0)) if source=='pwm' else int(e['value']>0)
        if source=='servo':
            e=last(selected('servo',p),t);return e['angle'] if e and e['active'] else None
        if source=='display':
            texts=[]
            for name in c['name'].split(','):
                e=last([e for e in by.get('display',[]) if e['name']==name.strip()],t)
                if not e:return None
                texts.append(e['text'])
            return ''.join(texts).strip()
        if source=='tone':
            e=last(selected('tone',p),t)
            if e and e['frequency'] and t<e['until_ms']:return e['frequency']
            events=selected('gpio',p);level=0;rises=[]
            for e in events:
                if e.get('pwm'):continue
                if e['value'] and not level and abs(e.get('at_us',e['at_ms']*1000)-t*1000)<=3000:rises.append(e.get('at_us',e['at_ms']*1000))
                level=int(e['value']>0)
            return (len(rises)-1)*1000000/(rises[-1]-rises[0]) if len(rises)>=2 and rises[-1]>rises[0] else 0
        if source=='pin_mode':return next((e['value'] for e in by.get('pin_mode',[]) if e['pin']==p),None)
        raise ValueError(source)
    def within(c,actual):
        if actual is None:return False
        if c.get('numeric'):
            try:actual=float(actual)
            except (ValueError,TypeError):return False
        if 'expected' in c:
            expected=c['expected']
            if c.get('numeric'):
                try:expected=float(expected)
                except (ValueError,TypeError):return False
            if isinstance(expected,(int,float)):
                try:return math.isfinite(float(actual)) and abs(float(actual)-expected)<=c.get('tolerance',0)
                except (ValueError,TypeError):return False
            return str(actual).strip()==str(expected).strip()
        return ('min' not in c or actual>=c['min']) and ('max' not in c or actual<=c['max'])
    def matching(c,event):
        if 'baud' in c and event.get('baud')!=c['baud']:return False
        if c.get('kind')=='lcd' and any(event.get(k)!=v for k,v in c.get('display_properties',{}).items()):return False
        text=event.get('text','')
        if 'prefix' in c and c['prefix'] not in text:return False
        if 'contains' in c and c['contains'] not in text:return False
        if 'regex' in c and not re.search(c['regex'],text):return False
        if 'expected' in c:
            if isinstance(c['expected'],(int,float)):return any(abs(v-c['expected'])<=c.get('tolerance',0) for v in pilot10.numbers(text))
            return str(c['expected']) in text
        return True
    def edges(c):
        p=pin(c['pin'],board);result=[];level=0
        for e in selected('gpio',p):
            current=int(e['value']>0)
            if current!=level:
                if c.get('edge','both')=='both' or c.get('edge')==('rising' if current else 'falling'):result.append(e)
            level=current
        # tone() represents a timer-driven carrier; reconstruct its ideal host waveform.
        segments=[];active=None
        for e in selected('tone',p):
            t=e['at_ms'];freq=e['frequency'];end=min(e['until_ms'],plan['horizon_ms'])
            if active and active[2]==freq and t<=active[1]:active[1]=end;continue
            if active:
                active[1]=min(active[1],t)
                if active[1]>active[0]:segments.append(active)
            active=[t,end,freq] if freq else None
        if active and active[1]>active[0]:segments.append(active)
        lo=c.get('from_ms',0);hi=c.get('to_ms',plan['horizon_ms'])
        for a,b,freq in segments:
            step=500.0/freq;start=max(0,int(math.ceil((lo-a)/step)));end=int(math.floor((min(b,hi)-a)/step))
            if end-start>500000:raise ValueError('tone edge observation budget exceeded')
            for n in range(start,end+1):
                value=1 if n%2==0 else 0
                if c.get('edge','both')=='both' or c.get('edge')==('rising' if value else 'falling'):result.append(dict(at_ms=a+n*step,value=value))
        result.sort(key=lambda e:e['at_ms'])
        return result
    checks=[]
    for original in plan['checks']:
        c=dict(original)
        if c.get('origin')=='setup_complete':
            if not execution.get('setup_returned') or 'setup_at_ms' not in execution:
                checks.append(dict(id=c['id'],requirement_quote=c['requirement_quote'],test=c,
                                   actual=None,passed=False,unverified='setup_anchor_unavailable'))
                continue
            offset=execution['setup_at_ms']
            if c['kind'] in ('serial','lcd','edges','interval'):
                c.setdefault('from_ms',0);c.setdefault('to_ms',plan['horizon_ms'])
            for key in ('at_ms','from_ms','to_ms'):
                if key in c:
                    c[key]=[t+offset for t in c[key]] if isinstance(c[key],list) else c[key]+offset
            end=max(([c['at_ms']] if not isinstance(c.get('at_ms'),list) else c['at_ms']) if 'at_ms' in c else [c.get('to_ms',offset)])
            if end>execution['virtual_ms']:
                checks.append(dict(id=c['id'],requirement_quote=c['requirement_quote'],test=c,
                                   actual=None,passed=False,unverified='setup_relative_window_not_observed'))
                continue
        observation_end=max(c['at_ms']) if isinstance(c.get('at_ms'),list) else c.get('at_ms',c.get('to_ms',0))
        if observation_end>execution['virtual_ms']:
            checks.append(dict(id=c['id'],requirement_quote=c['requirement_quote'],test=c,
                actual=None,passed=False,unverified='requested_window_not_observed'))
            continue
        kind=c['kind'];ok=False;actual=None
        if kind=='event_order':
            if any('sequence' not in e for e in anchor_events):
                checks.append(dict(id=c['id'],requirement_quote=c['requirement_quote'],test=c,actual=None,
                    passed=False,unverified='event_order_metadata_unavailable'))
                continue
            first=command_anchor(c['first']);second=command_anchor(c['second'])
            actual=dict(first=anchor_events[first] if first is not None else None,second=anchor_events[second] if second is not None else None)
            ok=first is not None and second is not None and first<second
            if first is not None and second is not None:
                gap=anchor_events[second]['at_ms']-anchor_events[first]['at_ms'];actual['gap_ms']=gap
                if c.get('allow_same_ms') and gap==0:ok=True
                ok=ok and gap>=c.get('min_gap_ms',0) and gap<=c.get('max_gap_ms',float('inf'))
        elif kind=='event_window':
            if not ordered_metadata:
                checks.append(dict(id=c['id'],requirement_quote=c['requirement_quote'],test=c,actual=None,
                    passed=False,unverified='ordered_state_metadata_unavailable'))
                continue
            start=command_anchor(c['start']);end=command_anchor(c['end'],start) if start is not None else None
            if start is None or end is None:actual=dict(reason='required_phase_anchor_missing')
            else:
                lo=anchor_events[start]['at_ms']+c.get('start_delay_ms',0)
                hi=anchor_events[end]['at_ms']-c.get('end_margin_ms',0)
                actual=dict(start_at_ms=anchor_events[start]['at_ms'],end_at_ms=anchor_events[end]['at_ms'],observed_from_ms=lo,observed_to_ms=hi)
                if lo<=hi:
                    frames={t:s for t,s in snapshots};times=sorted(frames)
                    probes=sorted({lo,hi}|{t for t in times if lo<=t<=hi})
                    violations=[t for t in probes if not state_match(c['conditions'],frames[times[max(0,bisect.bisect_right(times,t)-1)]])]
                    actual.update(observations=len(probes),violation_times_ms=violations[:20]);ok=not violations
                else:actual['reason']='phase_window_empty_or_reversed'
        elif kind in ('state_witness','state_invariant'):
            lo,hi=c.get('from_ms',0),c.get('to_ms',plan['horizon_ms'])
            if not ordered_metadata or hi>execution['virtual_ms']:
                checks.append(dict(id=c['id'],requirement_quote=c['requirement_quote'],test=c,actual=None,
                    passed=False,unverified='ordered_state_metadata_or_window_unavailable'))
                continue
            times=[t for t,_ in snapshots]
            start=max(0,bisect.bisect_left(times,lo)-1)
            observations=[(lo,snapshots[start][1])]+[(t,s) for t,s in snapshots[start+1:] if lo<=t<=hi]
            if kind=='state_witness':
                matches=[t for t,s in observations if state_match(c['conditions'],s)]
                actual=dict(witness_at_ms=matches[0] if matches else None);ok=bool(matches)
            else:
                # Cross-pin writes within a millisecond are observed as one settled API state.
                settled={t:s for t,s in observations};settled[hi]=snapshots[max(0,bisect.bisect_right(times,hi)-1)][1]
                for t in list(settled):
                    probe=t+c.get('settle_ms',0)
                    if probe<=hi:settled[probe]=snapshots[max(0,bisect.bisect_right(times,probe)-1)][1]
                guarded_since=None;violations=[];count=0
                for t,s in sorted(settled.items()):
                    if not state_match(c.get('when',[]),s):guarded_since=None;continue
                    if guarded_since is None:guarded_since=t
                    if t-guarded_since<c.get('settle_ms',0):continue
                    count+=1
                    if not state_match(c['conditions'],s):violations.append(t)
                actual=dict(observations=count,violation_times_ms=violations[:20])
                ok=count>=c.get('min_observations',1) and not violations
        elif kind=='gpio_count':
            levels=[scalar('gpio',dict(c,pin=p),c['at_ms']) for p in c['pins']]
            actual=None if any(v is None for v in levels) else sum(v==int(c.get('active_high',True)) for v in levels)
            ok=within(c,actual)
        elif kind=='display_gpio_count':
            actual=[]
            for t in c['at_ms']:
                levels=[scalar('gpio',dict(c,pin=p),t) for p in c['pins']]
                actual.append((scalar('display',c,t),None if any(v is None for v in levels) else sum(levels)))
            ok=bool(actual) and all(count is not None and digit==str(count) for digit,count in actual)
        elif kind=='distinct_gpio_patterns':
            patterns={tuple(scalar('gpio',dict(c,pin=p),t) for p in c['pins']) for t in c['at_ms']}
            actual=len({pattern for pattern in patterns if None not in pattern})
            ok=within(c,actual)
        elif kind=='segment_frame':
            frames=[e for e in by.get('display',[]) if e.get('name')==c['name'] and
                    c.get('from_ms',0)<=e['at_ms']<=c.get('to_ms',plan['horizon_ms']) and e.get('segments')==c['segments'] and e.get('enabled',True)]
            actual=dict(count=len(frames),examples=frames[:3]);ok=len(frames)>=c.get('min_count',1)
        elif kind=='multiplex_display':
            glyphs={mask:str(i) for i,mask in enumerate((63,6,91,79,102,109,125,7,127,111))}
            slots=[collections.Counter() for _ in c['digit_pins']]
            for t in range(c['from_ms'],c['to_ms']+1):
                active=[i for i,p in enumerate(c['digit_pins']) if scalar('gpio',dict(c,pin=p),t)==int(c.get('digit_active_high',True))]
                if len(active)!=1:continue
                segments=[scalar('gpio',dict(c,pin=p),t) for p in c['segment_pins']]
                if None in segments:continue
                bits=sum(1<<i for i,value in enumerate(segments) if value==int(not c.get('common_anode',True)))
                slots[active[0]][glyphs.get(bits,'?')]+=1
            actual=''.join(slot.most_common(1)[0][0] if slot else '?' for slot in slots)
            ok=actual==c['expected'] and all(slot[char]>=3 for slot,char in zip(slots,actual))
            if not ok and any(not slot or slot.most_common(1)[0][0]=='?' or
                              slot.most_common(1)[0][1]<0.95*sum(slot.values()) for slot in slots):
                checks.append(dict(id=c['id'],requirement_quote=c['requirement_quote'],test=c,
                    actual=actual,passed=False,unverified='multiplex_scan_not_decoded_reliably'))
                continue
        elif kind=='command_sequence':
            events=selected(c['source'],pin(c['pin'],board))
            if c['source']=='servo':
                events=[e for e in events if e.get('active')]
                if any('operation' not in e for e in events):
                    checks.append(dict(id=c['id'],requirement_quote=c['requirement_quote'],test=c,actual=None,
                        passed=False,unverified='command_operation_metadata_missing'))
                    continue
                events=[e for e in events if e['operation']=='write']
            values=[]
            for e in events:
                if not c.get('from_ms',0)<=e['at_ms']<=c.get('to_ms',plan['horizon_ms']):continue
                value=e['angle'] if c['source']=='servo' else e['value']
                if not values or abs(value-values[-1]['value'])>c.get('tolerance',0):
                    values.append(dict(value=value,at_ms=e['at_ms']))
            expected=c['values'];prefix=values[:len(expected)]
            ok=len(prefix)==len(expected) and all(abs(v['value']-x)<=c.get('tolerance',0) for v,x in zip(prefix,expected))
            intervals=[b['at_ms']-a['at_ms'] for a,b in zip(prefix,prefix[1:])]
            for field,compare in [('min_intervals_ms',lambda a,b:a>=b),('max_intervals_ms',lambda a,b:a<=b)]:
                if field in c:ok=ok and all(bound is None or compare(dt,bound) for dt,bound in zip(intervals,c[field]))
            actual=dict(commands=prefix,intervals_ms=intervals)
        elif kind=='setup':
            actual=dict(returned=execution.get('setup_returned'),at_ms=execution.get('setup_at_ms'))
            ok=actual['returned'] is True and actual['at_ms'] is not None and actual['at_ms']<=c['max_ms']
        elif kind in ('gpio','pwm','servo','display','tone','pin_mode'):
            actual=scalar(kind,c,c.get('at_ms',plan['horizon_ms']));ok=within(c,actual)
        elif kind in ('serial','lcd'):
            entries=[e for e in by.get(kind,[]) if c.get('from_ms',0)<=e['at_ms']<=c.get('to_ms',plan['horizon_ms']) and matching(c,e)]
            actual=dict(count=len(entries),examples=entries[:3]);ok=len(entries)>=c.get('min_count',1) and ('max_count' not in c or len(entries)<=c['max_count'])
        elif kind=='edges':
            events=[e for e in edges(c) if c.get('from_ms',0)<=e['at_ms']<=c.get('to_ms',plan['horizon_ms'])]
            actual=dict(count=len(events),times_ms=[e['at_ms'] for e in events[:20]])
            ok=len(events)>=c.get('min_count',0) and ('max_count' not in c or len(events)<=c['max_count'])
        elif kind=='interval':
            events=edges(c) if c['source']=='gpio' else [e for e in by.get('serial',[]) if matching(c,e)]
            events=[e for e in events if c.get('from_ms',0)<=e['at_ms']<=c.get('to_ms',plan['horizon_ms'])]
            intervals=[b['at_ms']-a['at_ms'] for a,b in zip(events,events[1:])]
            actual=dict(count=len(events),intervals_ms=intervals[:40]);ok=len(events)>=c.get('min_count',3) and bool(intervals) and all(abs(dt-c['expected_ms'])<=c.get('tolerance_ms',0) for dt in intervals)
        elif kind=='relation':
            actual=[scalar(c['source'],c,t) for t in c['at_ms']]
            try:
                values=list(map(float,actual));pairs=list(zip(values,values[1:]));tol=c.get('tolerance',0)
                ok=bool(pairs) and all(math.isfinite(v) for v in values) and all({'increasing':b>a+tol,'decreasing':b<a-tol,'equal':abs(a-b)<=tol,'different':abs(a-b)>tol}[c['relation']] for a,b in pairs)
            except (ValueError,TypeError):ok=False
        else:raise ValueError('Unsupported check kind '+kind)
        checks.append(dict(id=c['id'],requirement_quote=c['requirement_quote'],test=c,actual=actual,passed=bool(ok)))
    state=('failed' if any(not c['passed'] and not c.get('unverified') for c in checks) else
           'unverified_window' if any(c.get('unverified') for c in checks) else
           'passed' if checks else 'unverified_no_checks')
    # Missing high-level frames can be an observation limitation for a bit-banged driver.
    for c in checks:
        if c['test']['kind']=='servo' and c['actual'] is None:
            p=pin(c['test']['pin'],board)
            if selected('gpio',p):state='unverified_observer'
        if c['test']['kind']=='display' and c['actual'] is None and c['test'].get('name','').startswith('tm:'):
            pins=[int(v) for v in c['test']['name'].split(':')[1:]]
            if any(e['pin'] in pins for e in by.get('gpio',[])):state='unverified_observer'
    if execution['status'] not in ('completed','capture-end'):state='unverified_runtime'
    return dict(status=state,checks=checks,failed_assertions=sum(not c['passed'] for c in checks),execution=execution)


