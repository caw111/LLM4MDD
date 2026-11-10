"""Compare optimized and original pixel decoding on real and repeated framebuffers."""
import gzip,importlib.util,json,os,tempfile,time
from pathlib import Path
import picsimlab_raster as new
ROOT=Path(__file__).resolve().parents[2];OUT=ROOT/'experiment/output/picsimlab_comparison_20261108';OLD=ROOT/'experiment/output/picsimlab_verification_20261107'
def main():
 os.nice(15);spec=importlib.util.spec_from_file_location('raster_original',OUT/'method_snapshot/picsimlab_raster_before_speedup.py');old=importlib.util.module_from_spec(spec);spec.loader.exec_module(old)
 selected=[]
 for cid in ['pceval_button_LCD_display','pceval_calendar_display','pceval_serial_LCD_display']:
  path=OLD/'cases'/cid/'simulation/native-electrical-v14/binary_review_00/lcd_frames.bin.gz';n=0
  for meta,img,pixels in new.read_frames(path):
   if not meta['initialized'] or all(a==b for a,b in img.getextrema()):continue
   selected.append((meta,pixels.tobytes()));n+=1
   if n>=3:break
 with tempfile.TemporaryDirectory(prefix='picsim-raster-equivalence-') as work:
  path=Path(work)/'frames.gz'
  with gzip.open(path,'wb') as f:
   meta=selected[0][0];raw=bytes(240*320*4)
   for i in range(3):f.write(f'L {i} {meta["mac"]} 1 10 9 8\n'.encode());f.write(raw)
   for meta,raw in selected:
    for i in range(3):
     f.write(f'L {round(meta["at_ms"]*16000)+i} {meta["mac"]} {int(meta["initialized"])} {meta["cs"]} {meta["dc"]} {meta["rst"]}\n'.encode());f.write(raw)
  start=time.monotonic();a=old.observe(path);old_seconds=time.monotonic()-start
  start=time.monotonic();b=new.observe(path);new_seconds=time.monotonic()-start
  assert a==b,'Pixel decoding changed'
  new.self_check()
  result=dict(equal=True,frames=3+len(selected)*3,decoded_events=len(a),old_seconds=old_seconds,new_seconds=new_seconds,speedup=old_seconds/new_seconds)
  (OUT/'raster_speed_equivalence.json').write_text(json.dumps(result,indent=2));print(json.dumps(result),flush=True)
if __name__=='__main__':main()
