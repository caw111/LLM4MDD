import unittest
from pathlib import Path

import app.main as main


ROOT = Path(__file__).resolve().parents[1]


class CodeSeedTests(unittest.TestCase):
    def test_backend_replaces_placeholder_code_seed(self) -> None:
        seed = main._clean_code_seed("// TODO: implement later", "decision", "decision", "决策模块")

        lowered = seed.lower()
        self.assertIn("module_id: decision", seed)
        self.assertNotIn("todo", lowered)
        self.assertNotIn("replace with", lowered)
        self.assertNotIn("to be implemented", lowered)

    def test_backend_keeps_concrete_code_seed(self) -> None:
        seed = "inputs: adc; logic: threshold compare; outputs: alarm_state"

        self.assertEqual(main._clean_code_seed(seed, "decision"), seed)

    def test_frontend_uses_configurable_model(self) -> None:
        app_source = (ROOT / "frontend-new" / "src" / "App.tsx").read_text(encoding="utf-8")

        self.assertIn("VITE_LLM_MODEL", app_source)
        self.assertNotIn('model: "gpt-4o-mini"', app_source)

    def test_incremental_arduino_prompt_keeps_uno_hard_constraints(self) -> None:
        prompt = (ROOT / "tools" / "prompts" / "incremental_c_to_arduino.prompt.md").read_text(encoding="utf-8")

        self.assertIn("Arduino UNO bare hardware", prompt)
        self.assertIn("named editable PARAM", prompt)
        self.assertIn("millis()-based scheduling", prompt)
        self.assertIn("D3/D5/D6/D9/D10/D11", prompt)
        self.assertIn("Do not leave loop() empty", prompt)
        self.assertIn("single literals", prompt)
        self.assertIn("never nested", prompt)
        self.assertIn(".attach", prompt)
        self.assertIn("LiquidCrystal constructors", prompt)
        self.assertIn("SoftwareSerial constructors", prompt)
        self.assertIn("Stepper constructors", prompt)
        self.assertIn("#include <Servo.h>", prompt)
        self.assertIn("#include <LiquidCrystal.h>", prompt)
        self.assertIn("#include <SoftwareSerial.h>", prompt)
        self.assertIn("#include <Stepper.h>", prompt)
        self.assertIn("#include <Wire.h>", prompt)
        self.assertIn("#include <SPI.h>", prompt)
        self.assertIn("D0/D1", prompt)
        self.assertIn("USB Serial/upload", prompt)

    def test_main_arduino_prompt_keeps_editability_constraints(self) -> None:
        prompt = (ROOT / "tools" / "prompts" / "c_to_arduino.prompt.md").read_text(encoding="utf-8")

        self.assertIn("Strict editability constraints", prompt)
        self.assertIn("non-overlapping", prompt)
        self.assertIn("single literal", prompt)
        self.assertIn("LED_BUILTIN", prompt)
        self.assertIn(".attach", prompt)
        self.assertIn("LiquidCrystal lcd", prompt)
        self.assertIn("SoftwareSerial serial", prompt)
        self.assertIn("Stepper motor", prompt)
        self.assertIn("#include <Servo.h>", prompt)
        self.assertIn("#include <LiquidCrystal.h>", prompt)
        self.assertIn("#include <SoftwareSerial.h>", prompt)
        self.assertIn("#include <Stepper.h>", prompt)
        self.assertIn("#include <Wire.h>", prompt)
        self.assertIn("#include <SPI.h>", prompt)
        self.assertIn("D0", prompt)
        self.assertIn("D1", prompt)


if __name__ == "__main__":
    unittest.main()
