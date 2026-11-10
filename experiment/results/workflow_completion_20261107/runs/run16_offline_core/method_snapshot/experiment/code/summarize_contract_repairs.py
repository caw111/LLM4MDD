"""Audit saved development-round successes against the final interface validators."""
import hashlib
import json
import re
from pathlib import Path

from run_current_comparison import ROOT, save
from app.services.interface_planner import plan_interface_contract, ContractPlanningError
from app.services.llm_json import CompletionText


def main():
    out = ROOT/'experiment/output/contract_planning_v5_20261016'
    rounds = [ROOT/f'experiment/output/contract_planning_v{v}_20261016/real_model' for v in (2,3,4,5)]
    configs = [json.loads((p/'execution.json').read_text(encoding='utf-8')) for p in rounds]
    reports = [json.loads((p/'summary.json').read_text(encoding='utf-8')) for p in rounds]
    assert all(r['complete'] for r in reports)
    final_hashes = {p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in configs[-1]['hashes']}
    changed = [p for p,h in configs[-1]['hashes'].items() if final_hashes[p]!=h]
    save(out/'final_method_snapshot.json',dict(hashes=final_hashes,
        changes_after_last_generation_round=changed,
        scope='Final-source offline revalidation; generation-round execution snapshots are preserved verbatim.'))
    records = []
    for case in configs[0]['cases']:
        result = dict(case=case['id'], group=case['group'], dataset=case['dataset'], accepted=False)
        for folder in reversed(rounds):
            path = folder/case['id']/'planning.json'
            if not path.exists():
                continue
            report = json.loads(path.read_text(encoding='utf-8'))
            for attempt in reversed(report['attempts']):
                if not attempt.get('ok'):
                    result.setdefault('error', attempt['error'])
                    continue
                metadata = attempt.get('completion', {})
                raw = CompletionText(attempt['response'], finish_reason=metadata.get('finish_reason'), refusal=metadata.get('refusal'))
                try:
                    contract, checked = plan_interface_contract(lambda _: ('saved-model-response',raw),
                        case['requirement'], {}, target_board=case['board'], max_repairs=0)
                    target = out/'verified_contracts'/case['id']; target.mkdir(parents=True, exist_ok=True)
                    save(target/'contract.json', contract)
                    result.update(accepted=True, source=str(path.relative_to(ROOT)),
                                  response_sha256=hashlib.sha256(attempt['response'].encode()).hexdigest(),
                                  contract_sha256=checked['contract_sha256'], construction=checked['attempts'][0]['construction'])
                    result.pop('error',None)
                    break
                except ContractPlanningError as exc:
                    result['error'] = exc.attempts[-1]['error']
            if result['accepted']:
                break
        records.append(result)
    assert len(records)==len({r['case'] for r in records})==99
    summary = dict(scope='Development repair verification: successes regenerated across several method revisions, then revalidated under final source. Not independent sampling, not an end-to-end or functional success rate.',
        complete=True, method_hashes=final_hashes, model=configs[-1]['model'],
        changes_after_last_generation_round=changed,
        groups={g:dict(n=sum(r['group']==g for r in records),accepted=sum(r['group']==g and r['accepted'] for r in records)) for g in ('prior_failure','control')},
        usage={k:sum(r['usage'][k] for r in reports) for k in reports[0]['usage']},
        development_rounds=[dict(path=str(p.relative_to(ROOT)),groups=r['groups']) for p,r in zip(rounds,reports)], records=records)
    log = (out/'regression_tests.log').read_text(encoding='utf-8') if (out/'regression_tests.log').exists() else ''
    count = re.search(r'Ran (\d+) tests',log)
    summary['tests'] = dict(complete=bool(count and log.rstrip().endswith('OK')), n=int(count[1]) if count else None)
    summary['complete'] = summary['tests']['complete']
    save(out/'verification_summary.json',summary)
    n = summary['groups']['prior_failure']['accepted']
    lines = ['# 接口契约规划修复与验证', '',
        '保留原始888条实验记录；本报告是针对失败题的开发修复验证，不能替代新版本的完整三组实验。', '',
        '## 修复内容', '',
        '- 补齐分组按钮、反向传感器接线、风扇/继电器/增氧泵，以及“CLK 连接 D2”等写法；避免漏端点和分配虚构的第二个器件。',
        '- 将电路转为逐器件信号与目录操作事实；区分条形灯 A1..A10 端子与 MCU 模拟引脚，保留真实段序及4/6/8段等部分接线。',
        '- 以源器件ID或唯一匹配引脚恢复电气字段。接地按钮恢复低电平有效/上拉，共阳RGB采用已知反向电气映射。',
        '- 澄清设备请求与程序端口的区别，给出每种操作的允许字段及目标板PWM能力；共享被动LED负载每个物理GPIO只注册一次。',
        '- 规范无效果的数字选项、移位寄存器位号等价拼写，改进重复DHT通道定位；保留有语义差别的取值、未知字段和校准错误检查。',
        '- 阻止用普通GPIO覆盖I2C/SPI引脚来冒充RTC/IMU/LCD驱动；没有实现的设备能力继续失败。', '',
        '## 验证结果', '',
        f'- 原先87个接口失败题中，最终有 **{n}/87** 个保存的重新生成提案通过当前完整接口验收；另有 **12/12** 个按来源选取的控制题通过。',
        '- 所有通过提案均在最终源码下再次验收，来源响应、散列、构造决策和契约保存于 verified_contracts 与 verification_summary.json。',
        '- 多轮开发复用了同一批已见失败题，反复采样与版本修复不可分离，不能将该数值称为独立首稿成功率或端到端提升。',
        f'- 最终回归测试：{summary["tests"]}。包括官方SysML编译、IR/IL/C/Arduino转换、主机行为断言及Uno/Mega目标板编译；条形灯集成夹具验证接线链路，不验证所有原始任务行为。',
        f'- 本轮四个接口验证阶段合计费用约 ${summary["usage"]["reported_cost_usd"]:.4f}，tokens {summary["usage"]["tokens"]:,}；不含原888条实验费用。', '',
        '原296题历史提案的独立离线复核见 offline/offline_revalidation.json；它不调用LLM，不是新生成实验。一条旧LCD提案被新驱动边界正确拒绝，未为提高数字放松校验。', '',
        '## 仍未通过的原失败题', '', '| 题目 | 最后诊断 |', '|---|---|']
    for r in records:
        if not r['accepted']:
            lines.append('| '+r['case']+' | '+r.get('error','No accepted proposal').replace('\n',' ').replace('|','/').replace('\r',' ')+' |')
    lines += ['', '这些问题包括真实驱动/协议缺口、未指定输出目标和无法满足的需求/接线。不能把接口验收或目标板编译当作功能正确；正式效果仍需冻结最终版本，在相同配置与预算下重新对照评测。']
    (out/'REPORT_ZH.md').write_text('\n'.join(lines)+'\n',encoding='utf-8')
    print(json.dumps({k:summary[k] for k in ('groups','usage','tests')},ensure_ascii=False))


if __name__=='__main__':
    main()
