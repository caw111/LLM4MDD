"""Summarize reference-program checks; these are not model evaluation scores."""
import json
from collections import Counter
from pathlib import Path

ROOT=Path(__file__).resolve().parents[2]
AUDIT=ROOT/'experiment/formal/oracle_audit'

def classify(result, log):
    if result.get('status')=='passed':return 'passed'
    if 'Invalid scenario step key' in log:return 'unsupported_scenario_command'
    if 'missing required parameter' in log:return 'invalid_scenario_parameter'
    if 'Pin not found' in log:return 'invalid_pin'
    if 'Failed to read comparison file' in log:return 'missing_reference_asset'
    if 'No such file or directory' in log:return 'missing_compile_dependency'
    if result.get('status') in ('infrastructure_auth_or_quota','blocked_missing_token','infrastructure_error','infrastructure_wall_timeout'):return result['status']
    if result.get('returncode')==42:return 'reference_scenario_timeout'
    if not result.get('compile_ok'):return 'reference_compile_failure'
    return 'reference_scenario_failure'

def main():
    manifest=json.loads((ROOT/'experiment/formal/manifest.json').read_text(encoding='utf-8'))
    rows=[]
    for case in manifest['cases']:
        if case['dataset']!='PCEval':continue
        folder=AUDIT/case['id'];path=folder/'result.json'
        if not path.exists():
            rows.append({'case':case['id'],'split':case['split'],'category':'not_run'});continue
        result=json.loads(path.read_text(encoding='utf-8'))
        logpath=folder/('simulation.log' if result.get('compile_ok') else 'compile.log')
        log=logpath.read_text(encoding='utf-8') if logpath.exists() else ''
        rows.append({'case':case['id'],'split':case['split'],'category':classify(result,log),'recorded_status':result['status']})
    counts=Counter(r['category'] for r in rows)
    (AUDIT/'classified_summary.json').write_text(json.dumps({'counts':dict(counts),'cases':rows},ensure_ascii=False,indent=2),encoding='utf-8')
    lines=['# PCEval 原始测试审计','',
        '范围：全部 50 个案例，运行数据集原始参考程序和测试；不是 LLM 生成实验。',
        '已核验 Wokwi 凭据可用。数据集原文件及预期输出未修改；补齐了运行目录截图资源和缺失传感器库。',
        '参考程序通过是测试可用性的必要条件，不能单独证明测试覆盖完整。失败案例保留在覆盖率分母中，不能作为模型失败或直接从主结果中静默删除。','',
        '| 分类 | 数量 |','|---|---:|']
    lines += [f'| {k} | {v} |' for k,v in sorted(counts.items())]
    lines += ['', '| 案例 | 分区 | 审计结果 |','|---|---|---|']
    lines += [f"| {r['case']} | {r['split']} | {r['category']} |" for r in rows]
    lines += ['', '每个案例目录包含 result.json、compile.log 和适用时的 simulation.log。复测前记录保存在 before_environment_fix。',
        '后续：核对不支持命令的原意及参考测试矛盾，记录测试迁移版本；完成其余数据集 oracle、六组统一执行器与重复实验后，才能开展正式比较。']
    (AUDIT/'REPORT_ZH.md').write_text('\n'.join(lines)+'\n',encoding='utf-8')
    print(json.dumps(dict(counts),ensure_ascii=False))

if __name__=='__main__':
    assert classify({'status':'passed'},'')=='passed'
    assert classify({'compile_ok':True},'Invalid scenario step key: wait-pin')=='unsupported_scenario_command'
    assert classify({'compile_ok':True,'returncode':42},'Timeout')=='reference_scenario_timeout'
    main()
