import os
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

from fastapi import HTTPException

import app.main as main


VALID_SKETCH = """const int LED_PIN = 13; // [PARAM:id=blink.ledPin|module=blink|name=LED_PIN|type=int]
bool ledState = false;

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  runBlink();
}

// [MODULE:blink:start]
void runBlink() {
  ledState = !ledState;
  digitalWrite(LED_PIN, ledState ? HIGH : LOW);
}
// [MODULE:blink:end]
"""


class ArduinoUpdateModuleCodeTests(unittest.TestCase):
    def _write_temp_sketch(self, tmp_dir: str, content: str = VALID_SKETCH) -> Path:
        sketch_path = Path(tmp_dir) / "arduino_output.ino"
        sketch_path.write_text(content, encoding="utf-8")
        return sketch_path

    def test_update_module_code_rejects_static_quality_issue_before_save(self) -> None:
        with tempfile.TemporaryDirectory() as tmp_dir:
            sketch_path = self._write_temp_sketch(tmp_dir)
            original = sketch_path.read_text(encoding="utf-8")

            with patch.dict(os.environ, {"ARDUINO_STATIC_CHECK_STRICT": "true", "ARDUINO_RUNTIME_STRICT": "false"}):
                req = main.ArduinoUpdateModuleCodeRequest(
                    sketch_path=str(sketch_path),
                    module_id="blink",
                    code="""void runBlink() {
  digitalWrite(7, HIGH);
}""",
                )
                with self.assertRaises(HTTPException) as raised:
                    main.update_arduino_module_code(req)

            self.assertEqual(raised.exception.status_code, 422)
            self.assertIsInstance(raised.exception.detail, dict)
            self.assertTrue(
                any("raw pin literal" in issue for issue in raised.exception.detail.get("issues", []))
            )
            self.assertEqual(sketch_path.read_text(encoding="utf-8"), original)

    def test_update_module_code_saves_valid_code_and_preserves_backslashes(self) -> None:
        with tempfile.TemporaryDirectory() as tmp_dir:
            sketch_path = self._write_temp_sketch(tmp_dir)
            replacement = """void runBlink() {
  ledState = !ledState;
  digitalWrite(LED_PIN, ledState ? HIGH : LOW);
  Serial.println("\\n");
}"""

            with patch.dict(os.environ, {"ARDUINO_STATIC_CHECK_STRICT": "true", "ARDUINO_RUNTIME_STRICT": "false"}):
                response = main.update_arduino_module_code(
                    main.ArduinoUpdateModuleCodeRequest(
                        sketch_path=str(sketch_path),
                        module_id="blink",
                        code=replacement,
                    )
                )

            saved = sketch_path.read_text(encoding="utf-8")
            self.assertTrue(response.success)
            self.assertEqual(response.module_id, "blink")
            self.assertEqual(response.module_code_map["blink"], replacement)
            self.assertIn('Serial.println("\\n");', saved)

    def test_update_params_rejects_invalid_uno_pin_value_before_save(self) -> None:
        with tempfile.TemporaryDirectory() as tmp_dir:
            sketch_path = self._write_temp_sketch(tmp_dir)
            original = sketch_path.read_text(encoding="utf-8")

            with patch.dict(os.environ, {"ARDUINO_STATIC_CHECK_STRICT": "true", "ARDUINO_RUNTIME_STRICT": "false"}):
                req = main.ArduinoUpdateParamsRequest(
                    sketch_path=str(sketch_path),
                    updates=[main.ArduinoParamUpdateItem(param_id="blink.ledPin", value="99")],
                )
                with self.assertRaises(HTTPException) as raised:
                    main.update_arduino_params(req)

            self.assertEqual(raised.exception.status_code, 422)
            self.assertIsInstance(raised.exception.detail, dict)
            self.assertTrue(
                any("invalid Arduino UNO pin value" in issue for issue in raised.exception.detail.get("issues", []))
            )
            self.assertEqual(sketch_path.read_text(encoding="utf-8"), original)

    def test_update_params_rejects_code_injection_value_before_save(self) -> None:
        with tempfile.TemporaryDirectory() as tmp_dir:
            sketch_path = self._write_temp_sketch(tmp_dir)
            original = sketch_path.read_text(encoding="utf-8")

            req = main.ArduinoUpdateParamsRequest(
                sketch_path=str(sketch_path),
                updates=[main.ArduinoParamUpdateItem(param_id="blink.ledPin", value="12; int injected = 1")],
            )
            with self.assertRaises(HTTPException) as raised:
                main.update_arduino_params(req)

            self.assertEqual(raised.exception.status_code, 400)
            self.assertIsInstance(raised.exception.detail, dict)
            self.assertIn("single literal", raised.exception.detail.get("issue", ""))
            self.assertEqual(sketch_path.read_text(encoding="utf-8"), original)

    def test_compiled_kernel_edit_is_rejected_before_save(self) -> None:
        sketch = "// BEGIN COMPILED SYSML MODEL\n" + VALID_SKETCH + "// END COMPILED SYSML MODEL\n"
        with tempfile.TemporaryDirectory() as tmp_dir:
            path = self._write_temp_sketch(tmp_dir, sketch)
            for updates in (False, True):
                with self.assertRaises(HTTPException) as raised:
                    if updates:
                        main.update_arduino_params(main.ArduinoUpdateParamsRequest(
                            sketch_path=str(path), updates=[main.ArduinoParamUpdateItem(param_id="blink.ledPin", value="12")]))
                    else:
                        main.update_arduino_module_code(main.ArduinoUpdateModuleCodeRequest(
                            sketch_path=str(path), module_id="blink", code="void runBlink() {}"))
                self.assertEqual(raised.exception.status_code, 422)
                self.assertEqual(path.read_text(encoding="utf-8"), sketch)

    def test_update_params_saves_valid_value(self) -> None:
        with tempfile.TemporaryDirectory() as tmp_dir:
            sketch_path = self._write_temp_sketch(tmp_dir)

            with patch.dict(os.environ, {"ARDUINO_STATIC_CHECK_STRICT": "true", "ARDUINO_RUNTIME_STRICT": "false"}):
                response = main.update_arduino_params(
                    main.ArduinoUpdateParamsRequest(
                        sketch_path=str(sketch_path),
                        updates=[main.ArduinoParamUpdateItem(param_id="blink.ledPin", value="12")],
                    )
                )

            saved = sketch_path.read_text(encoding="utf-8")
            param_values = {param["param_id"]: param["value"] for param in response.params}
            self.assertTrue(response.success)
            self.assertEqual(param_values["blink.ledPin"], "12")
            self.assertIn("const int LED_PIN = 12;", saved)


if __name__ == "__main__":
    unittest.main()
