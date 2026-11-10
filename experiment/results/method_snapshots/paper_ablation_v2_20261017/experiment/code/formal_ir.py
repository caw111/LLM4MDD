"""Compatibility import: production owns the executable IR contract."""
import sys
from pathlib import Path

if __package__ in (None, ''):
    sys.path.insert(0, str(Path(__file__).resolve().parents[2]))

from app.services.ir_contract import (
    TYPES, RESERVED, identifier, fields, compatible, expression_type, validate_ir,
)
