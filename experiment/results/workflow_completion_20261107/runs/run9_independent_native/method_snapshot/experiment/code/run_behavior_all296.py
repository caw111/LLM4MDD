"""Run unchanged artifacts only after independent test-design/reference readiness gates."""
import collections
import concurrent.futures
import hashlib
import json
from pathlib import Path
import time

import behavior_full_host as host
from behavior_full_protocol import GENERATOR_MODEL, REVIEWER_MODEL, PROTOCOL
from design_behavior_tests import call,validate,preserve_seed,review_prompt
from run_all_groups_host_smoke import LABELS, EXPECTED

ROOT,OUT=host.ROOT,host.OUT
PILOT=ROOT/'experiment/output/behavior_pilot10_20261023'
MANIFEST=ROOT/'experiment/output/all_groups_host_smoke_20261023/frozen_manifest.json'


def signature():
    files={p.name:host.sha(p) for p in host.FIXTURE.iterdir()}
    files['host_python']=host.sha(host.__file__)
    files['runner_python']=host.sha(__file__)
    return hashlib.sha256(json.dumps(files,sort_keys=True).encode()).hexdigest()


def candidate(row,case,plan,folder):
    if host.sha(row['source'])!=row['source_sha256']:
        return dict(status='unverified_source_changed',evidence=str(folder))
    cpp=Path(row['source']).parent.parent/'build/sketch/sketch.ino.cpp'
    return host.execute(cpp,row['board'],case,plan,folder)


def one(case,design,rows,fixture_hash):
    folder=OUT/'cases'/case
    record=folder/'case_result.json'
    if record.exists():
        cached=json.loads(record.read_text(encoding='utf-8'))
        if cached.get('fixture_signature')==fixture_hash:return cached
    result=dict(case=case,dataset=rows[0]['dataset'],fixture_signature=fixture_hash,results={},coverage=design.get('coverage','partial'))
    if design['status']!='design_review_approved':
        result.update(readiness='unverified_test_design',design_status=design['status'])
        host.save(record,result);return result
    requirement=design['requirement'];plan=design['plan'];coverage=design['coverage']
    board=rows[0]['board'] if rows[0].get('board') else next(r['board'] for r in rows if r.get('board'))
    folder.mkdir(parents=True,exist_ok=True)
    for attempt in range(3):
        host.case_files(case,plan,board)
        ref=folder/f'reference_{attempt}.cpp';ref.write_text(plan['reference_cpp'],encoding='utf-8')
        preflight=host.execute(ref,board,case,plan,folder/f'reference_{attempt}',True)
        host.save(folder/f'preflight_{attempt}.json',preflight)
        if preflight['status']=='passed':
            mutant=folder/'no_output_mutant.cpp';mutant.write_text('#include <Arduino.h>\nvoid setup(){}\nvoid loop(){}\n')
            sensitivity=host.execute(mutant,board,case,plan,folder/'no_output_mutant',True)
            if sensitivity['status']!='failed':
                # Doing nothing can satisfy an explicitly all-OFF requirement;
                # an all-HIGH implementation is then an independently wrong control.
                mutant=folder/'all_high_mutant.cpp'
                count=70 if 'mega' in board else 20
                mutant.write_text('#include <Arduino.h>\nvoid setup(){for(int p=0;p<'+str(count)+';++p){pinMode(p,OUTPUT);digitalWrite(p,HIGH);}}\nvoid loop(){}\n',encoding='utf-8')
                sensitivity=host.execute(mutant,board,case,plan,folder/'all_high_mutant',True)
            if sensitivity['status']=='failed':
                host.save(folder/'tested_plan.json',dict(plan=plan,coverage=coverage,requirement=requirement,design_source=str(OUT/'test_designs_v2'/case/'accepted.json')))
                result.update(readiness='ready',coverage=coverage,preflight_evidence=preflight['evidence'],mutant_evidence=sensitivity['evidence'])
                break
            preflight=dict(status='unverified_test_sensitivity',explanation='No-output mutant was not rejected')
        result.update(readiness='unverified_reference',preflight_status=preflight['status'])
        if attempt==2:break
        # Only reference diagnostics enter repairs. Candidate sources/results are never sent.
        failures=[c for c in preflight.get('checks',[]) if not c['passed']][:12]
        log=(folder/f'reference_{attempt}/build.stderr.txt')
        diagnostic=log.read_text(errors='replace')[-10000:] if log.exists() else ''
        feedback=dict(preflight_status=preflight['status'],failed_reference_checks=failures,build_diagnostics=diagnostic,
                      execution=preflight.get('execution'),explanation=preflight.get('explanation'))
        if 'raw-i2c' in str(feedback) or 'imu-physical-input' in str(feedback) or 'lcd-shape' in str(feedback):break
        try:
            draft=call(folder/'preflight_repairs','draft',attempt,GENERATOR_MODEL,PROTOCOL+'\nREQUIREMENT:\n'+requirement+'\nPrevious approved test:\n'+json.dumps(plan,ensure_ascii=False)+'\nReference preflight failed. Repair the reference/test while preserving the original requirement; do not weaken tests just to fit incorrect reference code.\n'+json.dumps(feedback,ensure_ascii=False))
            validate(draft,requirement)
            seed=OUT/'test_designs_v2'/case/'frozen_seed.json'
            if seed.exists():preserve_seed(draft,json.loads(seed.read_text(encoding='utf-8'))['plan'])
            if not draft.get('supported'):break
            review=call(folder/'preflight_repairs','review',attempt,REVIEWER_MODEL,review_prompt(requirement,draft))
            host.save(folder/f'preflight_repair_review_{attempt}.json',review)
            if not review.get('approved'):continue
            coverage='full' if draft['coverage']=='full' and review.get('coverage')=='full' else 'partial'
            plan=draft
        except Exception as exc:
            result['repair_error']=type(exc).__name__;break
    if result['readiness']!='ready':host.save(record,result);return result
    jobs={}
    for row in rows:
        if not row['compile_ok']:continue
        cpp=Path(row['source']).parent.parent/'build/sketch/sketch.ino.cpp'
        key=hashlib.sha256(json.dumps([row['source_sha256'],row['board'],host.sha(cpp)]).encode()).hexdigest()
        jobs.setdefault(key,row)
    with concurrent.futures.ThreadPoolExecutor(max_workers=3) as pool:
        futures={pool.submit(candidate,row,case,plan,folder/'executions'/key):key for key,row in jobs.items()}
        for f in concurrent.futures.as_completed(futures):
            key=futures[f]
            try:r=f.result()
            except Exception as exc:r=dict(status='unverified_harness',error_type=type(exc).__name__)
            result['results'][key]=r
    for row in rows:
        if row['compile_ok']:
            cpp=Path(row['source']).parent.parent/'build/sketch/sketch.ino.cpp'
            key=hashlib.sha256(json.dumps([row['source_sha256'],row['board'],host.sha(cpp)]).encode()).hexdigest()
            result.setdefault('row_keys',{})[row['group']]=key
    host.save(record,result)
    return result


def report(manifest,completed,started,final=False):
    pilot_rows=json.loads((PILOT/'mapped_results.json').read_text())
    pilot_map={(r['group'],r['case']):r for r in pilot_rows}
    mapped=[]
    for row in manifest:
        item=dict(group=row['group'],case=row['case'],dataset=row['dataset'],compile_ok=row['compile_ok'])
        if not row['compile_ok']:item['behavior_status']='compile_failed_not_run'
        elif (row['group'],row['case']) in pilot_map:
            p=pilot_map[row['group'],row['case']];item.update(behavior_status=p['behavior_status'],evidence=p.get('evidence'),test_origin='frozen_pilot10',coverage='pilot_feature_checks')
        elif row['case'] not in completed:item['behavior_status']='pending'
        else:
            case=completed[row['case']]
            if case['readiness']!='ready':item.update(behavior_status=case['readiness'],evidence=str(OUT/'cases'/row['case']),test_origin='requirement_only_model_assisted')
            else:
                r=case['results'][case['row_keys'][row['group']]]
                state=r['status']
                if state=='passed' and case['coverage']!='full':state='partial_checks_passed_not_full'
                item.update(behavior_status=state,evidence=r.get('evidence'),coverage=case['coverage'],test_origin='requirement_only_reviewed_reference_validated')
        mapped.append(item)
    groups={}
    for group in EXPECTED:
        selected=[r for r in mapped if r['group']==group]
        datasets={}
        for d,total in [('internal',120),('PCEval',50),('EmbedBench',126)]:
            subset=[r for r in selected if r['dataset']==d];assert len(subset)==total
            count=collections.Counter(r['behavior_status'] for r in subset)
            datasets[d]=dict(total=total,compiled=sum(r['compile_ok'] for r in subset),counts=dict(count),
                             confirmed_passes=count['passed'],confirmed_pass_fraction=count['passed']/total,
                             tested_behavior_failures=count['failed'],pending=count['pending'])
        count=collections.Counter(r['behavior_status'] for r in selected)
        groups[group]=dict(label=LABELS[group],total=296,compiled=sum(r['compile_ok'] for r in selected),counts=dict(count),datasets=datasets)
    summary=dict(final=final,case_readiness_audits_completed=len(completed)+10,planned_cases=296,elapsed_seconds=round(time.time()-started,1),
                 groups=groups,scope='Host API functional feature tests with explicit expected outputs; no hardware/SimulIDE; partial and unverified results remain separate.')
    host.save(OUT/'mapped_results.json',mapped);host.save(OUT/'summary.json',summary)
    lines=['# 全量296题：按三个数据集分列的行为测试','',
           f'测试就绪审计已完成 {len(completed)+10}/296 题；本批处理完成：{final}。',
           '原代码未重新生成或修复。每题测试仅依赖有效需求/电路；候选代码不进入测试生成或修复提示。',
           '通过表示已审查且参考程序通过、无输出错误程序被拒绝的有限行为测试通过；不是实物运行或全输入正确性证明。部分检查通过、测试未确认、原编译失败、待执行均分开。','']
    for dataset,total in [('internal',120),('PCEval',50),('EmbedBench',126)]:
        lines += ['## '+{'internal':'本地数据集','PCEval':'PCEval','EmbedBench':'EmbedBench'}[dataset]+f'（{total}题）','',
                  '| 组别 | 编译通过 | 行为通过 | 行为未通过 | 仅部分检查通过 | 其他未确认 | 待执行 | 已确认通过/全部题目 |',
                  '|---|---:|---:|---:|---:|---:|---:|---:|']
        for group,g in groups.items():
            v=g['datasets'][dataset];c=v['counts'];unknown=sum(n for k,n in c.items() if k not in ('passed','failed','partial_checks_passed_not_full','pending','compile_failed_not_run'))
            lines.append(f"| {g['label']} | {v['compiled']} | {c.get('passed',0)} | {c.get('failed',0)} | {c.get('partial_checks_passed_not_full',0)} | {unknown} | {c.get('pending',0)} | {v['confirmed_pass_fraction']:.1%} |")
        lines.append('')
    lines+=['## 方法及限制','',
            '沿用冻结的10题人工试跑记录；其余题为仅基于需求生成的结构化测试，模型在独立上下文中审查。每个测试先实际执行正常参考实现并逐项比较预期，再执行无输出错误程序检验区分能力，通过后才运行各组候选。参考执行失败的测试不用于给候选判错。','',
            '完整覆盖指有限测试对明确功能要点的覆盖，不是形式化证明；模型辅助设计及审查可能共同遗漏缺陷。缺省内部接线约定：独立LED为引脚阳极/GND阴极，未给电路的按钮接GND并采用上拉；显式给出的电路优先。未明确阈值、命令格式等不得臆造后强制判错；不能完整测定的要点保留partial或未确认。','',
            '记录是主机Arduino API命令及部分独立逻辑器件解码，不覆盖AVR资源/中断、真实I2C/SPI、电气波形、舵机物理运动或LCD像素表现。原始EmbedBench描述测试存在部分冲突，没有直接拿矛盾描述当作真值。','',
            '各数据集“已确认通过/全部题目”是保守已确认比例；存在部分/未确认项时不能声称该值为整个数据集的精确功能正确率。累计workflow267/280与单轮消融属于不同实验协议。','',
            '详见mapped_results.json、每题tested_plan.json、参考/错误程序预检、候选trace.jsonl、逐项result.json，以及模型原始请求/响应及审查记录。']
    (OUT/'REPORT_ZH.md').write_text('\n'.join(lines)+'\n',encoding='utf-8')
    return summary


def main():
    host.fixture();fixture_hash=signature()
    manifest=json.loads(MANIFEST.read_text())
    by_case=collections.defaultdict(list)
    for r in manifest:by_case[r['case']].append(r)
    pilot_cases={r['case'] for r in json.loads((PILOT/'mapped_results.json').read_text())}
    pending=set(by_case)-pilot_cases;completed={};inflight={};started=time.time()
    host.save(OUT/'execution_protocol.json',dict(manifest=str(MANIFEST),manifest_sha256=host.sha(MANIFEST),fixture_signature=fixture_hash,
                                               generator=GENERATOR_MODEL,reviewer=REVIEWER_MODEL,candidate_feedback_to_test_design=False))
    with concurrent.futures.ThreadPoolExecutor(max_workers=8) as pool:
        while pending or inflight:
            for case in sorted(pending):
                if len(inflight)>=16:break
                path=OUT/'test_designs_v2'/case/'accepted.json'
                if path.exists():
                    design=json.loads(path.read_text(encoding='utf-8'));inflight[pool.submit(one,case,design,by_case[case],fixture_hash)]=case
            pending-=set(inflight.values())
            done=[f for f in inflight if f.done()]
            for f in done:
                case=inflight.pop(f)
                try:completed[case]=f.result()
                except Exception as exc:
                    completed[case]=dict(case=case,readiness='unverified_harness',error_type=type(exc).__name__,error=str(exc)[:500])
                    host.save(OUT/'cases'/case/'case_result.json',completed[case])
            progress=OUT/'design_progress_v2.json'
            if progress.exists() and json.loads(progress.read_text()).get('completed')==286:
                for case in list(pending):
                    if not (OUT/'test_designs_v2'/case/'accepted.json').exists():
                        completed[case]=dict(case=case,readiness='unverified_test_design',reason='design pass completed without an accepted executable plan');pending.remove(case)
            s=report(manifest,completed,started)
            print(json.dumps(dict(case_audits=len(completed)+10,pending_cases=len(pending),running=len(inflight),ready=sum(r['readiness']=='ready' for r in completed.values()),seconds=s['elapsed_seconds'])),flush=True)
            if pending or inflight:time.sleep(10)
    report(manifest,completed,started,True)
    print('All 296 case readiness/execution records finalized with separate dataset summaries.',flush=True)


if __name__=='__main__':main()
