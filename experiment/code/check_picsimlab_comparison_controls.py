"""Check known good/bad frozen sources under the common comparison profiles."""
import collections,json,os
import review_picsimlab_comparison as review
OUT=review.OUT/'controls'
def main():
 os.nice(15);review.OUT=OUT;manifest=review.load(OUT/'manifest.json');rows=[]
 for case in manifest['cases']:
  row=review.review_case(case)
  if row:rows.append(row);review.save(OUT/'cases'/case['id']/'basic_review.json',row)
 differences=[]
 for row in rows:
  old=manifest['expected'][row['original_case']];expected='passed' if old=='scenarios_passed' else 'failed' if old=='failed' else 'unverified'
  if row['status']!=expected:differences.append(dict(case=row['original_case'],expected=expected,actual=row['status'],failed=[dict(id=c['id'],kind=c['test']['kind'],actual=c['actual'],test=c['test']) for c in row.get('failed_checks',[])],extra=row.get('extra_core_checks')))
 result=dict(total=len(rows),counts=dict(collections.Counter(r['status'] for r in rows)),differences=differences)
 review.save(OUT/'basic_control_check.json',result);print(json.dumps(result,ensure_ascii=False,indent=2),flush=True)
if __name__=='__main__':main()
