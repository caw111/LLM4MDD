"""Publish completed frozen-group native simulation results and integrity evidence."""
import collections,hashlib,json,os,shutil
from pathlib import Path
import run_picsimlab_comparison_groups as run
OUT=run.OUT;ROOT=run.ROOT
LABELS={'historical_direct':'旧版直接生成','historical_direct_feedback':'旧版直接生成＋编译反馈','direct_gpt61':'GPT-6.1直接生成','ablation_direct':'同轮直接生成首稿','ablation_direct_feedback':'同轮直接生成＋编译反馈','ablation_workflow':'同轮完整工作流（未追加人工修复）','ablation_no_contract_feedback':'关闭接口契约反馈修复','ablation_no_model_feedback':'关闭模型反馈修复','ablation_blind_resampling':'无诊断重新采样','ablation_llm_backend':'LLM替换确定性后端及适配器'}
def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
def rate(n,d):return f'{n}/{d}（{n/d*100:.2f}%）'
def main():
 os.nice(15);manifest=run.load(OUT/'manifest.json');summary=run.load(OUT/'basic_function_summary.json');records=run.load(OUT/'basic_function_records.json');assert len(records)==2960 and len({(r['group'],r['case']) for r in records})==2960
 assert all(r['status'] in ('passed','failed','unverified') for r in records)
 integrity=[];index={}
 for case in manifest['cases']:
  folder=OUT/'cases'/case['id'];assert sha(folder/'source.ino')==case['source_sha256']
  c=run.load(folder/'compilation.json');entry=dict(case=case['id'],source_unchanged=True,compile_ok=c['compile_ok']);index[case['id']]=case
  if c['compile_ok']:
   assert sha(folder/'firmware/sketch.ino.hex')==c['hex']['sha256'];assert (folder/'case_execution.json').exists();entry['firmware_unchanged']=True
   result=run.load(folder/'case_execution.corrected_timing.json' if (folder/'case_execution.corrected_timing.json').exists() else folder/'case_execution.json')
   for p in result['plans']:
    assert p.get('source_sha256')==case['source_sha256'] and p.get('firmware_sha256')==c['hex']['sha256']
   entry['simulation_plans']=len(result['plans'])
  integrity.append(entry)
 run.save(OUT/'integrity_verification.json',integrity)
 for group in manifest['groups']:
  rows=[r for r in records if r['group']==group];assert len(rows)==296
  assert collections.Counter(r['dataset'] for r in rows)==dict(internal=120,PCEval=50,EmbedBench=126)
 for r in records:
  r['method_label']=LABELS[r['group']]
  if r.get('manual_requirement_review'):r['decision_reason']=r['manual_requirement_review']['reason']
  elif not r['execution_key']:r['decision_reason']='该组没有生成可执行源码'
  elif not r['compile_ok']:
   c=run.load(OUT/'cases'/r['execution_key']/'compilation.json');r['decision_reason']='目标板编译未通过；'+c.get('diagnostic_tail',c.get('error',''))[-600:]
  elif r['status']=='passed':r['decision_reason']='已覆盖基本功能场景通过，严格附加断言及资格说明保留在逐题记录'
  else:
   b=run.load(OUT/'cases'/r['execution_key']/'basic_review.json');failed=b.get('failed_checks',[]);extra=[e for e in b.get('extra_core_checks',[]) if e['status']!='passed'];r['decision_reason']='；'.join(c['id']+'：'+str(c['actual'])[:130] for c in failed[:3]) or str(extra)[:400] or '缺少充分的功能通过证据'
 run.save(OUT/'all_case_results.json',records)
 summary.update(final=True,scope='10 frozen groups x 296 tasks; no source repair/regeneration',compilation_unique_passed=sum(c['compile_ok'] for c in integrity),compilation_unique_failed=sum(not c['compile_ok'] for c in integrity),source_integrity_verified=True,labels=LABELS,manual_decision_count=len(run.load(OUT/'manual_function_decisions.json')),previous_repaired_snapshot_separate=True)
 run.save(OUT/'final_summary.json',summary)
 lines=['# 直接生成与消融组：PICSimLab最终结果','','全部10组×296题均已完成判定。基本功能通过率分母为296，未生成源码和编译失败不计通过；未确认项单列。生成代码保持冻结，未修复、未重新生成。','', '| 方法 | 目标板编译通过 | 基本功能通过 | 不通过 | 未确认 |','|---|---:|---:|---:|---:|']
 for group in manifest['groups']:
  s=summary['groups'][group]['all'];lines.append(f'| {LABELS[group]} | {rate(s["compile_passed"],296)} | {rate(s["passed"],296)} | {s["failed"]} | {s["unverified"]} |')
 lines+=['','## 三个数据集分别的结果','','| 方法 | 数据集 | 编译通过 | 基本功能通过 | 不通过 | 未确认 |','|---|---|---:|---:|---:|---:|']
 for group in manifest['groups']:
  for d in ['internal','PCEval','EmbedBench']:
   s=summary['groups'][group][d];n=s['total'];lines.append(f'| {LABELS[group]} | {d} | {rate(s["compile_passed"],n)} | {rate(s["passed"],n)} | {s["failed"]} | {s["unverified"]} |')
 lines+=['','## 统计口径与限制','','- 执行基于校准后的PICSimLab无界面原生AVR引擎，实际运行Uno/Mega固件，采集引脚、PWM、串口和屏幕像素；传感器及外设使用理想化模型。','- 相同题目、源码、板型和输入配置复用仿真证据。1565份不同源码中1412份通过本轮编译，均已执行仿真。2960条组别记录不代表2960次独立的不同程序运行。','- 采用基本核心功能口径，结合仿真断言与需求/源码语义复核；不代表所有严格格式、响应上限、接口边界和未覆盖场景全部满足。250ms显示响应、500ms启动、部分严格串口格式等限制未全满足的情况保留在逐题说明。','- 4道无时间戳题目统一将首次操作移到上电500ms之后，每阶段给8秒观察预算，21份可编译版本已补测，旧结果保留。','- 距离量化、相同输入下合法ADC分箱/灯数取整、HSV平滑的小幅取整误差和物理更新延迟单独审查，不将理想物理输入值直接当成程序实测整数。','- 42个上一轮已有二元结论的控制程序，在共同口径下全部复现原结论。原先未确认advanced_009在共同接线配置下获得通过证据；本表不据此声称重新全测之前296题修复后快照。','- 本表完整工作流是同轮消融的原始冻结代码，区别于此前经过多轮人工修复的283/296快照。论文同轮消融应比较同轮7组；3个历史/跨模型直接生成组作为补充，不将修复后快照替代原始消融对照。','- 本轮补齐LedControl1.0.6、ShiftRegister74HC5951.3.1、Tone1.0.0库并复测相关代码，因此本轮标准化编译数与部分历史数值有小幅差异。','','[全部2960条逐题结果](ALL_CASE_RESULTS.md) · [机器可读逐题结果](all_case_results.json) · [完整汇总JSON](final_summary.json) · [人工语义复核](manual_function_decisions.json) · [源码/固件完整性](integrity_verification.json)','']
 (OUT/'FINAL_RESULTS.md').write_text('\n'.join(lines),encoding='utf8')
 detail=['# 全部2960条逐题结果','','| 组别 | 题号 | 数据集 | 编译 | 基本功能 | 判定依据 |','|---|---|---|---|---|---|']
 names={'passed':'通过','failed':'不通过','unverified':'未确认'}
 for r in records:
  key=r['execution_key'];cid=f'[{r["case"]}](cases/{key}/source.ino)' if key else r['case'];reason=r['decision_reason'].replace('|','/').replace('\n',' ')[:600]
  detail.append(f'| {LABELS[r["group"]]} | {cid} | {r["dataset"]} | {"通过" if r["compile_ok"] else "未通过/未生成"} | {names[r["status"]]} | {reason} |')
 (OUT/'ALL_CASE_RESULTS.md').write_text('\n'.join(detail)+'\n',encoding='utf8')
 snapshot=OUT/'method_snapshot';methods={}
 for p in (ROOT/'experiment/code').glob('*picsimlab*'):
  if p.is_file():shutil.copyfile(p,snapshot/('final_'+p.name));methods[p.name]=sha(p)
 engine=simpath=Path.home()/'.cache/llm4mdd-picsimlab-build/engines/native-electrical-v14/picsimlab_NOGUI';methods['native_engine_sha256']=sha(engine);run.save(OUT/'method_integrity.json',methods)
 print(json.dumps({g:summary['groups'][g]['all'] for g in manifest['groups']},ensure_ascii=False),flush=True)
if __name__=='__main__':main()
