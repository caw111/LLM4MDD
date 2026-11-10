import sys,json
from elftools.elf.elffile import ELFFile
from pathlib import Path
p=Path(__file__).resolve().parents[2]/'experiment/output/picsimlab_verification_20261107/cases'/sys.argv[1]/'firmware/sketch.ino.elf'
with p.open('rb') as f:
 d=ELFFile(f).get_dwarf_info()
 for cu in d.iter_CUs():
  for die in cu.iter_DIEs():
   if die.tag not in ('DW_TAG_subprogram','DW_TAG_inlined_subroutine'):continue
   a=die.attributes; name=a.get('DW_AT_name');origin=die.get_DIE_from_attribute('DW_AT_abstract_origin') if 'DW_AT_abstract_origin' in a else None
   if origin:name=origin.attributes.get('DW_AT_name')
   if name and name.value in (b'loop',b'setup',b'main'):
    print(die.tag,name.value,{k:(v.form,repr(v.value)) for k,v in a.items()})
