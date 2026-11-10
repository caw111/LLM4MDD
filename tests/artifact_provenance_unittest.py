import copy
from concurrent.futures import ThreadPoolExecutor
import json
from pathlib import Path
import tempfile
import unittest
from unittest.mock import patch

from fastapi.testclient import TestClient
import app.main as main
from app.services.artifact_provenance import ReceiptAuthority, ProvenanceError, object_digest, text_digest
from app.services.interface_planner import contract_digest


class AuthorityTests(unittest.TestCase):
    def test_concurrent_issuers_and_restart_share_one_key(self):
        with tempfile.TemporaryDirectory() as folder:
            path=Path(folder)/'private/issuer.key'
            def issue(n):return ReceiptAuthority(path).issue(dict(receipt_version=1,stage='test',n=n))
            with ThreadPoolExecutor(max_workers=8) as pool:receipts=list(pool.map(issue,range(24)))
            restarted=ReceiptAuthority(path)
            for n,receipt in enumerate(receipts):self.assertEqual(restarted.verify(receipt,'test')['n'],n)
            self.assertEqual(len(list(path.parent.iterdir())),1)
            broken=copy.deepcopy(receipts[0]);broken['claims']['n']=99
            with self.assertRaises(ProvenanceError):restarted.verify(broken,'test')
            other=ReceiptAuthority(Path(folder)/'other.key')
            with self.assertRaises(ProvenanceError):other.verify(receipts[0],'test')
            for bad in (None,{},dict(claims={},signature='中文'),dict(claims=[],signature='0'*64)):
                with self.subTest(bad=bad),self.assertRaises(ProvenanceError):restarted.verify(bad,'test')


class ProvenanceAPITests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.tmp=tempfile.TemporaryDirectory(prefix='provenance-tests-')
        cls.base=Path(cls.tmp.name)
        cls.contract=json.loads(Path('tools/examples/three_servos.contract.json').read_text(encoding='utf-8'))
        cls.source=Path('tools/sysml/three_servos.sysml').read_text(encoding='utf-8')
        with TestClient(main.app) as client,patch('app.main._call_llm',side_effect=AssertionError('No LLM in deterministic chain')):
            response=client.post('/compile/sysml2',json=dict(system_sysml2=cls.source,interface_contract=cls.contract,out_dir=str(cls.base/'model')))
            if response.status_code!=200:raise AssertionError(response.text)
            cls.model=response.json()
            response=client.post('/convert/il-to-c',json=dict(il_file_path=cls.model['files']['il'],
                interface_contract=cls.contract,provenance=cls.model['contents']['provenance'],out_c_dir=str(cls.base/'c')))
            if response.status_code!=200:raise AssertionError(response.text)
            cls.kernel=response.json()

    @classmethod
    def tearDownClass(cls):cls.tmp.cleanup()

    def il_payload(self,folder):
        return dict(il_file_path=self.model['files']['il'],out_c_dir=str(folder/'c'),
                    interface_contract=copy.deepcopy(self.contract),provenance=copy.deepcopy(self.model['contents']['provenance']))

    def adapter_payload(self,folder):
        contents=self.kernel['contents']
        return dict(c_code=contents['ProgramPage1.c'],compiled_kernel={'header':contents['define.h'],'source':contents['ProgramPage1.c']},
                    interface_contract=copy.deepcopy(self.contract),provenance=copy.deepcopy(self.kernel['provenance']),out_path=str(folder/'sketch.ino'))

    def test_verified_full_chain_retains_model_identity_and_compiles_on_mega(self):
        with tempfile.TemporaryDirectory() as folder,TestClient(main.app) as client,patch('app.main._call_llm',side_effect=AssertionError('No LLM')):
            base=Path(folder);response=client.post('/convert/c-to-arduino',json=self.adapter_payload(base))
            self.assertEqual(response.status_code,200,response.text)
            data=response.json();claims=data['provenance']['claims']
            self.assertEqual(data['provenance_status'],'verified_model_chain')
            self.assertEqual(claims['source_sha256'],text_digest(self.source))
            self.assertEqual(claims['ir_sha256'],object_digest(self.model['contents']['subsystem_ir']))
            self.assertEqual(claims['arduino_sha256'],text_digest(data['arduino_code']))
            self.assertEqual(claims['parent_receipt_sha256'],object_digest(self.kernel['provenance']))
            result=client.post('/arduino/compile',json=dict(sketch_path=data['saved_path'],fqbn='arduino:avr:mega',provenance=data['provenance']))
            self.assertEqual(result.status_code,200,result.text)
            self.assertTrue(result.json()['success'],result.text)
            self.assertEqual(result.json()['arduino_sha256'],claims['arduino_sha256'])
            self.assertEqual(result.json()['provenance_status'],'verified_model_chain')

    def test_javascript_json_roundtrip_preserves_frozen_contract_across_all_stages(self):
        import shutil
        import subprocess
        def browser_roundtrip(value):
            result=subprocess.run([shutil.which('node'),'-e',
                "let s='';process.stdin.setEncoding('utf8');process.stdin.on('data',c=>s+=c);process.stdin.on('end',()=>process.stdout.write(JSON.stringify(JSON.parse(s))));"],
                input=json.dumps(value,ensure_ascii=False),capture_output=True,text=True,encoding='utf-8',check=True)
            return json.loads(result.stdout)
        with tempfile.TemporaryDirectory() as folder,TestClient(main.app) as client,patch('app.main._call_llm',side_effect=AssertionError('No LLM')):
            base=Path(folder)
            prepared=client.post('/generate/interface-contract',json=dict(requirement='verified servo inventory',
                interface_contract=self.contract,out_path=str(base/'contract.json')))
            self.assertEqual(prepared.status_code,200,prepared.text)
            contract=browser_roundtrip(prepared.json())['interface_contract']
            self.assertIs(type(self.contract['interfaces'][2]['initial']),float)
            self.assertIs(type(contract['interfaces'][2]['initial']),int)
            self.assertEqual(contract_digest(contract),prepared.json()['contract_sha256'])
            model=client.post('/compile/sysml2',json=dict(system_sysml2=self.source,interface_contract=contract,out_dir=str(base/'model')))
            self.assertEqual(model.status_code,200,model.text)
            data=browser_roundtrip(model.json())
            self.assertEqual(data['contents']['contract_sha256'],prepared.json()['contract_sha256'])
            generated=client.post('/convert/il-to-c',json=dict(il_file_path=data['files']['il'],out_c_dir=str(base/'c'),
                interface_contract=contract,provenance=data['contents']['provenance']))
            self.assertEqual(generated.status_code,200,generated.text)
            data=browser_roundtrip(generated.json())
            result=client.post('/convert/c-to-arduino',json=dict(c_code=data['contents']['ProgramPage1.c'],
                compiled_kernel={'header':data['contents']['define.h'],'source':data['contents']['ProgramPage1.c']},
                interface_contract=contract,provenance=data['provenance'],out_path=str(base/'sketch.ino')))
            self.assertEqual(result.status_code,200,result.text)
            self.assertEqual(result.json()['contract_sha256'],prepared.json()['contract_sha256'])
            self.assertEqual(result.json()['provenance_status'],'verified_model_chain')

    def test_modified_final_sketch_rejects_and_compiler_uses_one_verified_read(self):
        import subprocess
        with tempfile.TemporaryDirectory() as folder,TestClient(main.app) as client:
            base=Path(folder);response=client.post('/convert/c-to-arduino',json=self.adapter_payload(base))
            self.assertEqual(response.status_code,200,response.text)
            data=response.json();path=Path(data['saved_path']);code=data['arduino_code']
            request=dict(sketch_path=str(path),provenance=data['provenance'],fqbn='arduino:avr:mega')
            path.write_text(code+'\n// replaced',encoding='utf-8')
            with patch('app.main._resolve_windows_arduino_cli',side_effect=AssertionError('Reject before tool lookup')):
                result=client.post('/arduino/compile',json=request)
                self.assertEqual(result.status_code,422,result.text)
            path.write_text(code,encoding='utf-8')
            def replace_after_read():
                path.write_text('void setup(){} void loop(){}',encoding='utf-8')
                return 'fixture-cli'
            def inspect_private_copy(args,**kwargs):
                private=Path(args[-1])
                self.assertNotEqual(private.resolve(),base.resolve())
                self.assertEqual((private/(private.name+'.ino')).read_text(encoding='utf-8'),code)
                return subprocess.CompletedProcess(args,0,stdout='fixture',stderr='')
            with patch('app.main._resolve_windows_arduino_cli',side_effect=replace_after_read),patch('app.main.subprocess.run',side_effect=inspect_private_copy):
                result=client.post('/arduino/compile',json=request)
            self.assertEqual(result.status_code,200,result.text)
            self.assertEqual(result.json()['arduino_sha256'],text_digest(code))

    def test_replaced_kernel_changed_contract_forged_or_wrong_stage_receipts_do_not_write(self):
        with tempfile.TemporaryDirectory() as folder,TestClient(main.app) as client,patch('app.main.generate_adapter',side_effect=AssertionError('Reject before adapter')):
            base=Path(folder);original=self.adapter_payload(base);variants=[]
            v=copy.deepcopy(original);v.pop('provenance');variants.append(v)
            v=copy.deepcopy(original);v['compiled_kernel']['source']+='\n// replaced';variants.append(v)
            v=copy.deepcopy(original);v['compiled_kernel']['header']+='\n// replaced';variants.append(v)
            v=copy.deepcopy(original);v['interface_contract']['interfaces'][0]['meaning']='different sensor';variants.append(v)
            v=copy.deepcopy(original);v['provenance']=self.model['contents']['provenance'];variants.append(v)
            v=copy.deepcopy(original);v['compiled_kernel']['source']+='\n// replaced'
            v['provenance']['claims']['kernel_source_sha256']=text_digest(v['compiled_kernel']['source']);variants.append(v)
            for index,payload in enumerate(variants):
                target=base/'sketch.ino';target.write_text('keep existing sketch',encoding='utf-8')
                with self.subTest(case=index):
                    result=client.post('/convert/c-to-arduino',json=payload)
                    self.assertEqual(result.status_code,422,result.text)
                    self.assertEqual(target.read_text(encoding='utf-8'),'keep existing sketch')

    def test_changed_il_and_contract_reject_before_backend_and_file_writes(self):
        with tempfile.TemporaryDirectory() as folder,TestClient(main.app) as client,patch('app.main._convert_il_to_c_unverified',side_effect=AssertionError('No backend')):
            base=Path(folder);original=self.il_payload(base);variants=[]
            changed=base/'changed.xml';changed.write_text(self.model['contents']['il_xml']+'\n<!-- changed -->',encoding='utf-8')
            v=copy.deepcopy(original);v['il_file_path']=str(changed);variants.append(v)
            v=copy.deepcopy(original);v['interface_contract']['interfaces'][0]['meaning']='another sensor';variants.append(v)
            v=copy.deepcopy(original);v.pop('provenance');variants.append(v)
            v=copy.deepcopy(original);v['provenance']=self.kernel['provenance'];variants.append(v)
            for field,value in [('optimize',True),('target','unapproved')]:
                v=copy.deepcopy(original);v[field]=value;variants.append(v)
            for payload in variants:
                result=client.post('/convert/il-to-c',json=payload)
                self.assertEqual(result.status_code,422,result.text)
                self.assertFalse((base/'c').exists())

    def test_original_il_race_cannot_change_the_private_backend_input(self):
        with tempfile.TemporaryDirectory() as folder,TestClient(main.app) as client:
            base=Path(folder);path=base/'input.xml';path.write_text(self.model['contents']['il_xml'],encoding='utf-8')
            payload=self.il_payload(base);payload['il_file_path']=str(path)
            real=main._run_codegen_step
            def change_original(name,args,**kwargs):
                actual=Path(args[args.index('-i')+1])
                self.assertNotEqual(actual.resolve(),path.resolve())
                self.assertEqual(text_digest(actual.read_text(encoding='utf-8')),self.model['contents']['provenance']['claims']['il_sha256'])
                path.write_text('changed after validation',encoding='utf-8')
                return real(name,args,**kwargs)
            (base/'c').mkdir();(base/'c/keep.txt').write_text('unrelated',encoding='utf-8')
            with patch('app.main._run_codegen_step',side_effect=change_original):response=client.post('/convert/il-to-c',json=payload)
            self.assertEqual(response.status_code,200,response.text)
            self.assertEqual(response.json()['contents']['ProgramPage1.c'],self.kernel['contents']['ProgramPage1.c'])
            self.assertEqual((base/'c/keep.txt').read_text(encoding='utf-8'),'unrelated')

    def test_standalone_backend_and_explicit_legacy_adapter_do_not_claim_provenance(self):
        with tempfile.TemporaryDirectory() as folder,TestClient(main.app) as client,patch('app.main._call_llm',side_effect=AssertionError('No LLM')):
            base=Path(folder)
            result=client.post('/convert/il-to-c',json=dict(il_file_path=self.model['files']['il'],out_c_dir=str(base/'c')))
            self.assertEqual(result.status_code,200,result.text)
            self.assertEqual(result.json()['provenance_status'],'unverified_legacy')
            self.assertIsNone(result.json()['provenance'])
            payload=self.adapter_payload(base);payload.pop('provenance');payload['allow_legacy_adapter']=True
            result=client.post('/convert/c-to-arduino',json=payload)
            self.assertEqual(result.status_code,200,result.text)
            self.assertEqual(result.json()['provenance_status'],'unverified_legacy')
            self.assertIsNone(result.json()['provenance'])

    def test_compatibility_endpoint_returns_canonical_kernel_and_receipt_with_custom_names(self):
        with tempfile.TemporaryDirectory() as folder,TestClient(main.app) as client,patch('app.main._call_llm',side_effect=AssertionError('No LLM')):
            base=Path(folder)
            response=client.post('/generate/c-from-step1',json=dict(step1_sysml2=self.source,interface_contract=self.contract,
                out_dir=str(base/'generated'),out_header_path=str(base/'custom.h'),out_source_path=str(base/'custom.c')))
            self.assertEqual(response.status_code,200,response.text)
            contents=response.json()['contents']
            self.assertIn('#include "custom.h"',contents['generated_model_c'])
            payload=self.adapter_payload(base)
            payload.update(compiled_kernel=contents['compiled_kernel'],provenance=contents['provenance'])
            result=client.post('/convert/c-to-arduino',json=payload)
            self.assertEqual(result.status_code,200,result.text)
            self.assertEqual(result.json()['provenance_status'],'verified_model_chain')


if __name__=='__main__':unittest.main()
