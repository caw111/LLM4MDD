"""Extract fully specified public LED toggle obligations; no task IDs or oracle access."""
import copy
import re


def add_toggle_checks(contract, requirement):
    from app.services.interface_planner import public_match
    def single(pattern):
        found=list(re.finditer(pattern,requirement,re.I))
        match=public_match(pattern,requirement)
        if len(found)!=1 or match is None:return None
        start=max(requirement.rfind(c,0,match.start()) for c in '\n。；;.!?')+1
        if re.search(r'如果|若|当|\b(?:if|when)\b',requirement[start:match.start()],re.I):return None
        return match
    def amount(match):return int(next(g for g in match.groups() if g is not None))
    ports=contract['interfaces']
    lamps=[p for p in ports if p.get('binding',{}).get('kind')=='digital_output' and p['type']=='bool']
    clocks=[p for p in ports if p.get('binding',{}).get('kind')=='elapsed_ms']
    buttons=[p for p in ports if p.get('binding',{}).get('kind')=='digital_input']
    if len(lamps)!=1 or len(clocks)!=1 or len(buttons)>1:return contract
    interval=single(r'每\s*(\d+)\s*ms\s*翻转|toggles? every\s*(\d+)\s*ms')
    tolerance=single(r'(?:响应及翻转时间容差|时间容差|timing tolerance)\s*(\d+)\s*ms')
    if not interval or not tolerance:return contract
    evidence=[interval,tolerance]
    if buttons:
        initial=single(r'初始停止(?:且[^。；;\n]*熄灭)?|initially stopped')
        start=single(r'启动时从熄灭开始|starts? from off')
        toggle=single(r'按下沿切换运行/停止|press[ -]edge toggles? run/stop')
        stop=single(r'停止立即熄灭|stopping immediately turns (?:the )?(?:LED|output) off')
        reset=single(r'再启动重新计时|restarting resets timing')
        if not all((initial,start,toggle,stop,reset)):return contract
        evidence.extend([initial,start,toggle,stop,reset])
    else:
        initial=single(r'初始熄灭|initially off')
        first=single(r'(\d+)\s*ms\s*时首次点亮|first turns on after\s*(\d+)\s*ms')
        if not initial or not first or amount(first)!=amount(interval):return contract
        evidence.extend([initial,first])
    assertion=dict(delta=clocks[0]['name'],output=lamps[0]['name'],period_ms=2*amount(interval),
        pulse_ms=amount(interval),first_pulse='after_silence',active_value=True,inactive_value=False,
        tolerance_ms=amount(tolerance),evidence='; '.join(m[0] for m in evidence))
    if buttons:
        assertion.update(pause_input=buttons[0]['name'],initial_running=False,pause_policy='reset')
    result=copy.deepcopy(contract)
    checks=result.setdefault('periodic_checks',[])
    same=[c for c in checks if c['output']==assertion['output']]
    if any(any(c.get(k)!=v for k,v in assertion.items() if k!='evidence') for c in same):
        raise ValueError('Supplied periodic assertion conflicts with explicit toggle timing requirements')
    if not same:checks.append(assertion)
    startup=result.setdefault('startup_checks',[])
    if any(c['output']==assertion['output'] and c['expected'] is not False for c in startup):
        raise ValueError('Supplied startup assertion conflicts with explicit initially-off requirement')
    if not any(c['output']==assertion['output'] for c in startup):
        startup.append(dict(output=assertion['output'],expected=False,evidence=initial[0]))
    return result
