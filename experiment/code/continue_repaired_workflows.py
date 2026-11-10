"""Continue exactly 70 verified contracts; retain all prior baseline results."""
import argparse
import hashlib
import json
import os
import time
from collections import Counter
from concurrent.futures import ThreadPoolExecutor, wait, FIRST_COMPLETED
from pathlib import Path

import run_current_comparison as runner
from run_current_comparison import ROOT, save, digest, metrics
from resume_configured_audit import canonical_json_lines, retry_permission
from app.services.interface_planner import contract_digest
from method_snapshot import runtime_identity

BASE = ROOT/'experiment/output/configured_audit_v1_20261015'
REPAIRS = ROOT/'experiment/output/contract_planning_v5_20261016'
OUT = ROOT/'experiment/output/repaired_workflow_continuation_20261016'


class ContinuationCalls(runner.ResumableCalls):
    def __call__(self, prompt):
        cursor = self.cursor
        while cursor < len(self.entries) and 'error_type' in self.entries[cursor]:
            cursor += 1
        if cursor < len(self.entries):
            old = self.entries[cursor]['prompt']
            if old != prompt and canonical_json_lines(old) == canonical_json_lines(prompt):
                save(self.folder/f'json_order_replay_{cursor}.json',dict(original_sha256=hashlib.sha256(old.encode()).hexdigest(),
                    current_sha256=hashlib.sha256(prompt.encode()).hexdigest()))
                prompt = type(prompt)(old)
        return super().__call__(prompt)


def evaluate(case, config):
    folder = OUT/case['id']; path = folder/'result.json'
    if path.exists():
        previous = json.loads(path.read_text(encoding='utf-8'))
        if not previous.get('infrastructure_error'):
            return previous
    contract = json.loads((ROOT/case['contract_path']).read_text(encoding='utf-8'))
    assert contract_digest(contract) == case['contract_sha256']
    save(folder/'contract.json',contract)
    calls = ContinuationCalls(folder/'calls',config['model'])
    row = dict(case=case['id'], dataset=case['dataset'], arm='workflow', repeat=1,
        compile_ok=False, compile_status='not_generated', functional_status='not_generated_or_compile_failed',
        contract_reused=True, contract_sha256=case['contract_sha256'])
    stage = 'sysml_generation'; accepted = {}
    try:
        def validator(source):
            checked = runner.validate_candidate(source)
            if checked['ok']:
                accepted.update(source=source,ir=checked['ir'])
            return checked
        _, bundle, validation = runner.generate_validated_bundle(calls,
            runner.sysml_bundle_prompt(case['requirement'],contract=contract),
            max_repairs=2,validator=validator,contract=contract)
        save(folder/'model.json',bundle);save(folder/'model_validation.json',validation)
        source = bundle['system_sysml2']
        row.update(model_ok=True,model_attempts=len(validation['attempts']))
        if accepted.get('source') != source:
            raise ValueError('Accepted model source mismatch')
        stage = 'ir_to_code'
        with runner.TRANSLATOR, runner.JAVA:
            generated = runner.lower_verified(source,contract,accepted['ir'],folder/'lowered')
        row.update(backend_ok=True,provenance_status=generated.provenance_status)
        stage = 'hardware_adapter'
        code = runner.adapt_verified(generated,contract,folder/'lowered')
        receipt = json.loads((folder/'lowered/arduino_provenance.json').read_text(encoding='utf-8'))
        stage = 'arduino_compile'
        result, _ = runner.compile_candidate(case,folder/'workflow/attempt_0',code,receipt)
        row.update(result)
    except runner.SysMLValidationError as exc:
        save(folder/'model_validation.json',dict(ok=False,attempts=exc.attempts))
        last = exc.attempts[-1] if exc.attempts else {}
        row.update(failure_stage=last.get('failure_stage') or stage,model_ok=False,error=str(exc))
    except runner.BackendFailure as exc:
        row.update(failure_stage=stage,backend_ok=False,error=str(exc))
    except runner.api.HTTPException as exc:
        if exc.status_code in (400,422):
            row.update(failure_stage=stage,error=str(exc.detail)[:3000])
        else:
            row.update(failure_stage=stage,infrastructure_error=type(exc).__name__,functional_status='unverified')
    except (ValueError,TypeError,KeyError) as exc:
        row.update(failure_stage=stage,error=str(exc)[:3000])
    except Exception as exc:
        row.update(failure_stage=stage,infrastructure_error=type(exc).__name__,functional_status='unverified')
    row.update(metrics(calls.entries));save(path,row)
    return row


def summarize(config):
    rows = [json.loads(p.read_text(encoding='utf-8')) for p in OUT.glob('*/result.json')]
    expected = {c['id'] for c in config['cases']}
    assert len(rows)==len({r['case'] for r in rows}) and {r['case'] for r in rows} <= expected
    baseline = json.loads((BASE/'results.json').read_text(encoding='utf-8'))
    old = {r['case']:r for r in baseline if r['arm']=='workflow'}
    assert len(old)==296 and sum(r['compile_ok'] for r in old.values())==199
    assert not any(old[cid]['compile_ok'] for cid in expected)
    added = sum(r['compile_ok'] for r in rows)
    updated = dict(old)
    for row in rows:
        updated[row['case']] = row
    changed = [p for p,h in config['hashes'].items() if digest(ROOT/p)!=h]
    bad_artifacts = [r['case'] for r in rows if r.get('artifact') and digest(ROOT/r['artifact'])!=r['artifact_sha256']]
    infra = [r['case'] for r in rows if r.get('infrastructure_error')]
    summary = dict(scope='Cumulative development recovery: reuse 199 prior compiled tasks and continue 70 repaired contracts. Not a fresh single-version or equal-budget comparison.',
        planned=70,completed=len(rows),old_compile_passes=199,additional_compile_passes=added,
        cumulative_compile_passes=199+added,denominator=296,cumulative_compile_rate=(199+added)/296,
        complete={r['case'] for r in rows}==expected and not infra and not changed and not bad_artifacts,
        source_changes=changed,artifact_mismatches=bad_artifacts,infrastructure=infra,
        failure_stages=dict(Counter(r.get('failure_stage',r['compile_status']) for r in rows if not r['compile_ok'])),
        new_functional_status=dict(Counter(r['functional_status'] for r in rows)),
        cumulative_functional_status=dict(Counter(r['functional_status'] for r in updated.values())),
        continuation_usage={k:sum(r.get(k,0) or 0 for r in rows) for k in ('llm_calls','tokens','reported_cost_usd')})
    save(OUT/'summary.json',summary);save(OUT/'cumulative_workflow_results.json',list(updated.values()))
    lines=['# 修复后70题续跑与累计结果','',f'补跑完成：{len(rows)}/70；记录完整且核查通过：{summary["complete"]}。',
        f'原编译通过199题 + 本次新增{added}题 = **{199+added}/296（{(199+added)/296:.1%}）**。','',
        '复用原始接口验收契约，只运行后续模型生成（首稿+最多2次修复）、官方SysML验证、确定性后端、Arduino适配、目标板编译及已有独立功能测试。旧结果与接口规划原始调用均未重写。',
        '这是多轮修复后的累计开发结果，混合了历史成功与当前版本补跑，不能称为新版本独立首稿成功率；原直接生成/反馈组仅作历史参照。编译通过不等于功能正确。','',
        '续跑失败阶段：'+json.dumps(summary['failure_stages'],ensure_ascii=False),
        '本次功能状态：'+json.dumps(summary['new_functional_status'],ensure_ascii=False),
        '累计功能状态：'+json.dumps(summary['cumulative_functional_status'],ensure_ascii=False),
        '本次后续流程调用费用：'+json.dumps(summary['continuation_usage'],ensure_ascii=False),'',
        '无独立oracle的题目不计为功能已证明成功。接口开发阶段费用另见接口修复报告。尚未补跑的原27个失败题保留原结果。']
    (OUT/'REPORT_ZH.md').write_text('\n'.join(lines)+'\n',encoding='utf-8')
    return summary


def main(report_only=False):
    os.chdir(ROOT)
    os.environ['PATH']=str(runner.CLI.parent)+os.pathsep+os.environ['PATH']
    os.environ['WINDOWS_ARDUINO_CLI_PATH']=str(runner.CLI)
    repairs=json.loads((REPAIRS/'verification_summary.json').read_text(encoding='utf-8'))
    baseline=json.loads((BASE/'execution.json').read_text(encoding='utf-8'))
    lookup={c['id']:c for c in baseline['cases']}
    cases=[]
    for record in repairs['records']:
        if record['group']!='prior_failure' or not record['accepted']:
            continue
        path=REPAIRS/'verified_contracts'/record['case']/'contract.json'
        contract=json.loads(path.read_text(encoding='utf-8'))
        case=dict(lookup[record['case']]);case.update(requirement=contract['configuration_resolution']['effective_requirement'],
            contract_path=str(path.relative_to(ROOT)),contract_sha256=record['contract_sha256'],contract_file_sha256=digest(path))
        assert contract_digest(contract)==record['contract_sha256']
        cases.append(case)
    assert len(cases)==len({c['id'] for c in cases})==70
    files=[Path(__file__),ROOT/'app/main.py',*sorted((ROOT/'app/services').glob('*.py')),
           ROOT/'experiment/code/run_current_comparison.py',ROOT/'experiment/code/run_controlled_experiment.py',
           ROOT/'experiment/code/run_formal_experiment.py',ROOT/'experiment/code/run_contract_development.py',
           ROOT/'experiment/code/resume_configured_audit.py',*sorted((ROOT/'tools/prompts').glob('*.md')),
           *sorted((ROOT/'experiment/development/oracles').glob('*'))]
    files += [ROOT/c['contract_path'] for c in cases]
    config=dict(cases=cases,model=baseline['model'],temperature=.2,max_tokens=6000,max_model_repairs=2,workers=8,
        hashes={str(p.relative_to(ROOT)):digest(p) for p in files if p.is_file()},
        baseline_results_sha256=digest(BASE/'results.json'),repair_summary_sha256=digest(REPAIRS/'verification_summary.json'),
        runtime=runtime_identity())
    OUT.mkdir(parents=True,exist_ok=True)
    if (OUT/'execution.json').exists():
        assert json.loads((OUT/'execution.json').read_text(encoding='utf-8'))==config,'Continuation snapshot changed'
    else:
        save(OUT/'execution.json',config)
    if report_only:
        print(json.dumps(summarize(config),ensure_ascii=False));return
    read,write=Path.read_text,Path.write_text
    def storage_read(path,*args,**kwargs):
        return retry_permission(read,path,*args,**kwargs) if path.resolve().is_relative_to(OUT) else read(path,*args,**kwargs)
    def storage_write(path,*args,**kwargs):
        return retry_permission(write,path,*args,**kwargs) if path.resolve().is_relative_to(OUT) else write(path,*args,**kwargs)
    Path.read_text,Path.write_text=storage_read,storage_write
    remaining=iter(cases[8:]);stop=False
    with ThreadPoolExecutor(max_workers=8) as pool:
        pending={pool.submit(evaluate,c,config) for c in cases[:8]}
        while pending:
            done,pending=wait(pending,return_when=FIRST_COMPLETED)
            for future in done:
                row=future.result();stop |= bool(row.get('infrastructure_error'))
                status=summarize(config)
                print(json.dumps(dict(completed=status['completed'],added=status['additional_compile_passes'],infrastructure=bool(status['infrastructure'])),ensure_ascii=False),flush=True)
                if not stop and (case:=next(remaining,None)) is not None:
                    pending.add(pool.submit(evaluate,case,config))
    print(json.dumps(summarize(config),ensure_ascii=False),flush=True)


if __name__=='__main__':
    parser=argparse.ArgumentParser();parser.add_argument('--report-only',action='store_true')
    main(parser.parse_args().report_only)
