import argparse
import json
import os
import sys
from pathlib import Path
from dataclasses import dataclass
from typing import Any, Dict, List, Optional, Tuple
import xml.etree.ElementTree as ET

if __package__ in (None, ''):
    sys.path.insert(0, str(Path(__file__).resolve().parents[1]))


def _require(condition: bool, message: str) -> None:
    if not condition:
        raise ValueError(message)


def _as_list(value: Any) -> List[Any]:
    if value is None:
        return []
    if isinstance(value, list):
        return value
    raise ValueError(f"Expected list, got {type(value).__name__}")


def _as_str(value: Any, *, field: str) -> str:
    _require(isinstance(value, str) and value.strip() != "", f"Field '{field}' must be a non-empty string")
    return value


def _as_optional_str(value: Any) -> Optional[str]:
    if value is None:
        return None
    if isinstance(value, str):
        return value
    raise ValueError(f"Expected string or null, got {type(value).__name__}")


def _as_dict(value: Any, *, field: str) -> Dict[str, Any]:
    _require(isinstance(value, dict), f"Field '{field}' must be an object")
    return value


def _array_size_to_str(array_size: Any) -> str:
    if array_size is None:
        return ""
    if isinstance(array_size, str):
        return array_size
    if isinstance(array_size, list):
        parts: List[str] = []
        for item in array_size:
            if isinstance(item, int):
                parts.append(str(item))
            elif isinstance(item, str) and item.strip().isdigit():
                parts.append(item.strip())
            else:
                raise ValueError(f"Invalid array_size item: {item!r}")
        return ",".join(parts)
    raise ValueError(f"Invalid array_size: {array_size!r}")


def _normalize_port_name_for_codegen(name: str) -> str:
    # CodeGenerator2 TBConverter uppercases the first letter for actor in/out port names.
    # If we keep names aligned here, templates that reference port names (notably Expression)
    # can stay consistent end-to-end.
    if not name:
        return name
    c0 = name[0]
    if "a" <= c0 <= "z":
        return c0.upper() + name[1:]
    return name


def _replace_word_tokens(text: str, mapping: Dict[str, str]) -> str:
    # Token replacement similar to CodeGenerator2's template logic:
    # replace only when surrounded by non "word" chars.
    def _is_word(ch: str) -> bool:
        return ch.isalnum() or ch == "_"

    out = text
    for src, dst in mapping.items():
        if src == dst or not src:
            continue
        start = 0
        while True:
            pos = out.find(src, start)
            if pos < 0:
                break
            left_ok = pos == 0 or not _is_word(out[pos - 1])
            right_pos = pos + len(src)
            right_ok = right_pos >= len(out) or not _is_word(out[right_pos])
            if left_ok and right_ok:
                out = out[:pos] + dst + out[right_pos:]
                start = pos + len(dst)
            else:
                start = pos + len(src)
    return out


def _prettify_xml(elem: ET.Element) -> None:
    # In-place indentation for ElementTree (py3.9+ compatible).
    def _indent(e: ET.Element, level: int = 0) -> None:
        i = "\n" + level * "  "
        if len(e):
            if not e.text or not e.text.strip():
                e.text = i + "  "
            for child in e:
                _indent(child, level + 1)
            if not e.tail or not e.tail.strip():
                e.tail = i
        else:
            if level and (not e.tail or not e.tail.strip()):
                e.tail = i

    _indent(elem, 0)


@dataclass(frozen=True)
class Endpoint:
    kind: str  # "input" | "output" | "node"
    name: Optional[str] = None  # for input/output
    node_id: Optional[str] = None  # for node
    port: Optional[str] = None  # for node port


@dataclass
class IdGen:
    actor_id: int = 1000
    port_id: int = 2000
    link_interface_id: int = 3000
    relation_id: int = 4000
    property_id: int = 5000

    def next_actor(self) -> int:
        self.actor_id += 1
        return self.actor_id

    def next_port(self) -> int:
        self.port_id += 1
        return self.port_id

    def next_link_interface(self) -> int:
        self.link_interface_id += 1
        return self.link_interface_id

    def next_relation(self) -> int:
        self.relation_id += 1
        return self.relation_id

    def next_property(self) -> int:
        self.property_id += 1
        return self.property_id


def _parse_endpoint(obj: Any, *, role: str) -> Endpoint:
    d = _as_dict(obj, field=role)
    kind = _as_str(d.get("kind"), field=f"{role}.kind")
    _require(kind in ("input", "output", "node"), f"{role}.kind must be one of input/output/node")
    if kind in ("input", "output"):
        name = _as_str(d.get("name"), field=f"{role}.name")
        return Endpoint(kind=kind, name=name)
    node_id = _as_str(d.get("id"), field=f"{role}.id")
    port = _as_str(d.get("port"), field=f"{role}.port")
    return Endpoint(kind=kind, node_id=node_id, port=port)


def ir_to_tblink(ir: Dict[str, Any], *, legacy_v1: bool = False) -> ET.ElementTree:
    if not legacy_v1:
        from app.services.ir_contract import validate_ir
        ir = validate_ir(ir)
    else:
        _require(ir.get('version') == 1, 'Legacy export requires explicit version=1')
    version = ir.get("version", 1)
    _require(version in (1, 2), f"Unsupported IR version: {version}")
    if version == 2:
        _require(ir.get("states") == [] and ir.get("updates") == []
                 and ir.get("execution") == "step-actions-then-outputs",
                 "TBLink export supports only stateless IR; use IL for stateful models")

    model_name = _as_str(ir.get("name", "Subsystem"), field="name")
    page_name = _as_str(ir.get("page", "ProgramPage1"), field="page")

    inputs = _as_list(ir.get("inputs"))
    outputs = _as_list(ir.get("outputs"))
    nodes = _as_list(ir.get("nodes"))
    edges = _as_list(ir.get("edges"))

    _require(len(edges) > 0, "IR must contain at least one edge")

    node_by_id: Dict[str, Dict[str, Any]] = {}
    for n in nodes:
        nd = _as_dict(n, field="nodes[]")
        node_id = _as_str(nd.get("id"), field="nodes[].id")
        _require(node_id not in node_by_id, f"Duplicate node id: {node_id}")
        node_by_id[node_id] = nd

    def _check_port_list(node_id: str, field: str) -> List[Dict[str, Any]]:
        port_list = _as_list(node_by_id[node_id].get(field))
        out: List[Dict[str, Any]] = []
        for p in port_list:
            pd = _as_dict(p, field=f"nodes[{node_id}].{field}[]")
            _as_str(pd.get("name"), field=f"nodes[{node_id}].{field}[].name")
            _as_str(pd.get("type", "f32"), field=f"nodes[{node_id}].{field}[].type")
            out.append(pd)
        return out

    node_inports: Dict[str, List[Dict[str, Any]]] = {nid: _check_port_list(nid, "inports") for nid in node_by_id}
    node_outports: Dict[str, List[Dict[str, Any]]] = {nid: _check_port_list(nid, "outports") for nid in node_by_id}

    input_defs: Dict[str, Dict[str, Any]] = {}
    for inp in inputs:
        d = _as_dict(inp, field="inputs[]")
        name = _as_str(d.get("name"), field="inputs[].name")
        _require(name not in input_defs, f"Duplicate input name: {name}")
        input_defs[name] = d

    output_defs: Dict[str, Dict[str, Any]] = {}
    for outp in outputs:
        d = _as_dict(outp, field="outputs[]")
        name = _as_str(d.get("name"), field="outputs[].name")
        _require(name not in output_defs, f"Duplicate output name: {name}")
        output_defs[name] = d

    id_gen = IdGen()

    root = ET.Element("Root")
    xml_page = ET.SubElement(
        root,
        "ModelPage",
        {
            "Id": "1",
            "ModelPageName": page_name,
            "ModelPageType": "TypeProgramModelPage",
        },
    )

    # Map endpoint -> LinkInterfaceId used by CodeGenerator2 TBConverter connectivity.
    # Keys:
    # ("input", name) -> link_id of the single Outport port
    # ("output", name) -> link_id of the single Inport port
    # ("node_out", node_id, port) -> link_id
    # ("node_in", node_id, port) -> link_id
    link_id_map: Dict[Tuple[str, str, str], int] = {}

    def _mk_link_interface(parent: ET.Element, *, link_id: int, link_type: str) -> None:
        ET.SubElement(
            parent,
            "LinkInterface",
            {
                "Id": str(link_id),
                "LinkDirection": "TypeAll",
                "LinkType": link_type,  # TypeIn / TypeOut
                "LinkRelationType": "TypeDataRelation",
            },
        )

    def _mk_port(
        parent: ET.Element,
        *,
        port_type: str,
        name: str,
        data_type: str,
        value: str = "",
        array_size: str = "",
        link_type: str,
    ) -> int:
        port_id = id_gen.next_port()
        link_id = id_gen.next_link_interface()
        xml_port = ET.SubElement(
            parent,
            "Port",
            {
                "Id": str(port_id),
                "PortType": port_type,  # TypeInport/TypeOutport
                "Name": name,
                "DataType": data_type,
                "Value": value,
                "ArraySize": array_size,
            },
        )
        _mk_link_interface(xml_port, link_id=link_id, link_type=link_type)
        return link_id

    def _mk_property(parent: ET.Element, *, name: str, value: str) -> None:
        prop_id = id_gen.next_property()
        ET.SubElement(
            parent,
            "Property",
            {
                "Id": str(prop_id),
                "Name": name,
                "Value": value,
            },
        )

    # Function inports/outports are represented as special actors with type "Inport"/"Outport".
    x_cursor = 50
    y_cursor = 50
    y_step = 80

    for in_name, in_def in input_defs.items():
        actor_id = id_gen.next_actor()
        xml_actor = ET.SubElement(
            xml_page,
            "Actor",
            {
                "Id": str(actor_id),
                "Type": "Inport",
                "Name": in_name,
                "X": str(x_cursor),
                "Y": str(y_cursor),
                "Width": "80",
                "Height": "40",
            },
        )
        dtype = _as_str(in_def.get("type", "f32"), field=f"inputs[{in_name}].type")
        asz = _array_size_to_str(in_def.get("array_size"))
        link_id = _mk_port(
            xml_actor,
            port_type="TypeOutport",
            name="Out",
            data_type=dtype,
            array_size=asz,
            link_type="TypeOut",
        )
        link_id_map[("input", in_name, "Out")] = link_id
        y_cursor += y_step

    y_cursor = 50
    x_nodes = 250
    for node_id, nd in node_by_id.items():
        actor_id = id_gen.next_actor()
        node_type = _as_str(nd.get("type"), field=f"nodes[{node_id}].type")
        xml_actor = ET.SubElement(
            xml_page,
            "Actor",
            {
                "Id": str(actor_id),
                "Type": node_type,
                "Name": node_id,
                "X": str(x_nodes),
                "Y": str(y_cursor),
                "Width": "120",
                "Height": "60",
            },
        )

        # Keep a per-node map from IR port name -> normalized TBLink port name.
        node_port_name_map: Dict[str, str] = {}

        for p in node_inports[node_id]:
            pname_raw = _as_str(p.get("name"), field=f"nodes[{node_id}].inports[].name")
            pname = _normalize_port_name_for_codegen(pname_raw)
            node_port_name_map[pname_raw] = pname
            ptype = _as_str(p.get("type", "f32"), field=f"nodes[{node_id}].inports[{pname}].type")
            asz = _array_size_to_str(p.get("array_size"))
            link_id = _mk_port(
                xml_actor,
                port_type="TypeInport",
                name=pname,
                data_type=ptype,
                array_size=asz,
                link_type="TypeIn",
            )
            # Accept both raw and normalized names in edges.
            link_id_map[("node_in", node_id, pname_raw)] = link_id
            link_id_map[("node_in", node_id, pname)] = link_id

        for p in node_outports[node_id]:
            pname_raw = _as_str(p.get("name"), field=f"nodes[{node_id}].outports[].name")
            pname = _normalize_port_name_for_codegen(pname_raw)
            node_port_name_map[pname_raw] = pname
            ptype = _as_str(p.get("type", "f32"), field=f"nodes[{node_id}].outports[{pname}].type")
            asz = _array_size_to_str(p.get("array_size"))
            link_id = _mk_port(
                xml_actor,
                port_type="TypeOutport",
                name=pname,
                data_type=ptype,
                array_size=asz,
                link_type="TypeOut",
            )
            link_id_map[("node_out", node_id, pname_raw)] = link_id
            link_id_map[("node_out", node_id, pname)] = link_id

        params = nd.get("params", {})
        if params is not None:
            params_d = _as_dict(params, field=f"nodes[{node_id}].params")
            for k, v in params_d.items():
                if v is None:
                    continue
                key = _as_str(k, field=f"nodes[{node_id}].params key")
                val_raw = "" if v is None else str(v)
                # If this is an Expression node, rewrite identifiers to match normalized port names.
                if node_type == "Expression" and key.lower() == "expression" and node_port_name_map:
                    val_raw = _replace_word_tokens(val_raw, node_port_name_map)
                _mk_property(xml_actor, name=key, value=val_raw)

        y_cursor += y_step

    x_outputs = 520
    y_cursor = 50
    for out_name, out_def in output_defs.items():
        actor_id = id_gen.next_actor()
        xml_actor = ET.SubElement(
            xml_page,
            "Actor",
            {
                "Id": str(actor_id),
                "Type": "Outport",
                "Name": out_name,
                "X": str(x_outputs),
                "Y": str(y_cursor),
                "Width": "80",
                "Height": "40",
            },
        )
        dtype = _as_str(out_def.get("type", "f32"), field=f"outputs[{out_name}].type")
        asz = _array_size_to_str(out_def.get("array_size"))
        link_id = _mk_port(
            xml_actor,
            port_type="TypeInport",
            name="In",
            data_type=dtype,
            array_size=asz,
            link_type="TypeIn",
        )
        link_id_map[("output", out_name, "In")] = link_id
        y_cursor += y_step

    def _resolve_src(ep: Endpoint) -> int:
        if ep.kind == "input":
            return link_id_map[("input", ep.name or "", "Out")]
        _require(ep.kind == "node", "src must be input or node")
        return link_id_map[("node_out", ep.node_id or "", ep.port or "")]

    def _resolve_dst(ep: Endpoint) -> int:
        if ep.kind == "output":
            return link_id_map[("output", ep.name or "", "In")]
        _require(ep.kind == "node", "dst must be output or node")
        return link_id_map[("node_in", ep.node_id or "", ep.port or "")]

    # Relations
    for edge in edges:
        ed = _as_dict(edge, field="edges[]")
        src = _parse_endpoint(ed.get("src"), role="edges[].src")
        dst = _parse_endpoint(ed.get("dst"), role="edges[].dst")

        # Validate endpoint existence
        if src.kind == "input":
            _require(src.name in input_defs, f"Edge src input not found: {src.name}")
        else:
            _require(src.node_id in node_by_id, f"Edge src node not found: {src.node_id}")
            _require(
                any(p["name"] == src.port for p in node_outports[src.node_id]),
                f"Edge src outport not found: {src.node_id}.{src.port}",
            )

        if dst.kind == "output":
            _require(dst.name in output_defs, f"Edge dst output not found: {dst.name}")
        else:
            _require(dst.node_id in node_by_id, f"Edge dst node not found: {dst.node_id}")
            _require(
                any(p["name"] == dst.port for p in node_inports[dst.node_id]),
                f"Edge dst inport not found: {dst.node_id}.{dst.port}",
            )

        src_link = _resolve_src(src)
        dst_link = _resolve_dst(dst)
        start = min(src_link, dst_link)
        end = max(src_link, dst_link)
        rel_id = id_gen.next_relation()
        ET.SubElement(
            xml_page,
            "Relation",
            {
                "Id": str(rel_id),
                "RelationType": "TypeData",
                "StartLinkInterface": str(start),
                "EndLinkInterface": str(end),
            },
        )

    tree = ET.ElementTree(root)
    _prettify_xml(root)
    return tree


def main() -> int:
    parser = argparse.ArgumentParser(description="Convert stateless executable IR v2 to TBLink XML.")
    parser.add_argument("--legacy-v1", action="store_true", help="Explicitly export an archived v1 actor graph; not the executable v2 pipeline")
    parser.add_argument("--in", dest="in_path", required=True, help="Input subsystem_ir.json")
    parser.add_argument("--out", dest="out_path", required=True, help="Output TBLink xml path")
    args = parser.parse_args()

    with open(args.in_path, "r", encoding="utf-8") as handle:
        ir = json.load(handle)

    tree = ir_to_tblink(ir, legacy_v1=args.legacy_v1)

    out_dir = os.path.dirname(os.path.abspath(args.out_path))
    if out_dir:
        os.makedirs(out_dir, exist_ok=True)
    tree.write(args.out_path, encoding="utf-8", xml_declaration=True)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
