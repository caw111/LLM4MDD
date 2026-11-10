"""服务端签发的模型产物链；凭据不能由客户端重新计算摘要来伪造。

本地服务的签名密钥存于 git 忽略的 .tools 目录，跨重启和 worker 共用。
信任边界是服务进程及其文件系统，不是针对本机代码执行者的安全沙箱。
"""
import hashlib
import hmac
import json
import os
from pathlib import Path
import secrets
import tempfile


class ProvenanceError(ValueError):
    pass


def text_digest(value):
    return hashlib.sha256(value.replace('\r\n','\n').encode('utf-8')).hexdigest()


def object_digest(value):
    return hashlib.sha256(canonical(value)).hexdigest()


def canonical(value):
    return json.dumps(value,ensure_ascii=False,sort_keys=True,separators=(',',':'),allow_nan=False).encode('utf-8')


class ReceiptAuthority:
    def __init__(self,key_path):
        self.key_path=Path(key_path)

    def _key(self):
        if not self.key_path.exists():
            self.key_path.parent.mkdir(parents=True,exist_ok=True)
            # 完整写入后用排他硬链接发布，避免多个 worker 读取半个密钥或互相覆盖。
            fd,name=tempfile.mkstemp(prefix='.issuer-',dir=self.key_path.parent)
            try:
                with os.fdopen(fd,'wb') as handle:
                    handle.write(secrets.token_bytes(32))
                    handle.flush();os.fsync(handle.fileno())
                try:os.link(name,self.key_path)
                except FileExistsError:pass
            finally:
                Path(name).unlink(missing_ok=True)
        key=self.key_path.read_bytes()
        if len(key)!=32:raise RuntimeError('Artifact issuer key is invalid; restore it or recompile with a new issuer.')
        return key

    def issue(self,claims):
        payload=json.loads(canonical(claims))
        return {'claims':payload,'signature':hmac.new(self._key(),canonical(payload),hashlib.sha256).hexdigest()}

    def verify(self,receipt,stage,expected=None):
        if not isinstance(receipt,dict) or set(receipt)!={'claims','signature'}:
            raise ProvenanceError('Missing server-issued '+stage+' provenance. Recompile the validated model.')
        claims=receipt['claims'];signature=receipt['signature']
        if not isinstance(claims,dict) or not isinstance(signature,str) or len(signature)!=64 or any(c not in '0123456789abcdef' for c in signature):
            raise ProvenanceError('Malformed artifact provenance.')
        digest=hmac.new(self._key(),canonical(claims),hashlib.sha256).hexdigest()
        if not hmac.compare_digest(digest,signature):
            raise ProvenanceError('Artifact provenance signature does not match this server. Recompile the model.')
        if claims.get('receipt_version')!=1 or claims.get('stage')!=stage:
            raise ProvenanceError('Artifact provenance has the wrong stage or version.')
        for name,value in (expected or {}).items():
            if claims.get(name)!=value:
                raise ProvenanceError('Artifact provenance mismatch: '+name+'. Recompile using the frozen model and contract.')
        return dict(claims)

    def extend(self,receipt,previous_stage,stage,**values):
        claims=self.verify(receipt,previous_stage)
        claims.update(values,stage=stage,parent_receipt_sha256=object_digest(receipt))
        return self.issue(claims)


AUTHORITY=ReceiptAuthority(Path(__file__).resolve().parents[2]/'.tools/provenance/issuer.key')


def issue_model(source,ir,contract,il_xml,validator_version):
    # 调用方必须先完成真实 SysML 解析；这里再次使用公共 IR/契约入口。
    from app.services.ir_contract import validate_ir
    from app.services.behavior_contract import check_contract
    from app.services.interface_planner import contract_digest
    canonical_ir=validate_ir(ir)
    if contract is None or check_contract(canonical_ir,contract)['status']=='failed':
        raise ProvenanceError('A validated frozen interface contract is required to issue model provenance.')
    return AUTHORITY.issue(dict(receipt_version=1,stage='sysml_ir',
        source_sha256=text_digest(source),ir_sha256=object_digest(canonical_ir),
        contract_sha256=contract_digest(contract),il_sha256=text_digest(il_xml),validator_version=validator_version))
