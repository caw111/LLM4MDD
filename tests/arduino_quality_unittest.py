import unittest

from app.services.arduino_quality import (
    build_arduino_repair_prompt,
    check_requirement_coverage,
    sanitize_arduino_code,
    validate_arduino_code,
    validate_arduino_static_quality,
    validate_hardware_pin_references,
    validate_module_boundaries,
    validate_module_execution,
    validate_module_marker_structure,
    validate_module_markers,
    validate_param_markers,
    validate_pin_assignment_conflicts,
    validate_pin_definitions_applied,
    validate_pin_usage,
    validate_reserved_serial_pins,
    validate_runtime_safety,
    validate_uno_board_compatibility,
    validate_uno_pin_capabilities,
    validate_uno_pin_values,
)


class ArduinoQualityTests(unittest.TestCase):
    def test_sanitize_strips_markdown_and_joins_param_marker(self) -> None:
        source = """```cpp
int threshold = 70;
// [PARAM:id=decision.threshold|module=decision|name=threshold|type=int|min=0|max=100]
void setup() {}
void loop() {}
```"""

        cleaned = sanitize_arduino_code(source)

        self.assertNotIn("```", cleaned)
        self.assertIn(
            "int threshold = 70; // [PARAM:id=decision.threshold|module=decision|name=threshold|type=int|min=0|max=100]",
            cleaned,
        )

    def test_sanitize_extracts_fenced_sketch_from_surrounding_text(self) -> None:
        source = """Here is the Arduino sketch:

```arduino
int sensorPin = A0; // [PARAM:id=input.sensorPin|module=input|name=sensorPin|type=int]
void setup() {}
void loop() {
  analogRead(sensorPin);
}
```

This code is ready to upload.
"""

        cleaned = sanitize_arduino_code(source)

        self.assertNotIn("Here is", cleaned)
        self.assertNotIn("```", cleaned)
        self.assertTrue(cleaned.startswith("int sensorPin = A0;"))
        self.assertIn("void loop()", cleaned)

    def test_sanitize_prefers_fenced_block_with_setup_and_loop(self) -> None:
        source = """```json
{"status":"draft"}
```

```cpp
void setup() {}
void loop() {}
```
"""

        cleaned = sanitize_arduino_code(source)

        self.assertNotIn("status", cleaned)
        self.assertEqual(cleaned, "void setup() {}\nvoid loop() {}")

    def test_validate_accepts_minimal_serial_sketch(self) -> None:
        code = """
void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.println("ok");
  delay(1000);
}
"""

        self.assertEqual(validate_arduino_code(code), [])

    def test_validate_ignores_commented_serial_calls(self) -> None:
        code_with_commented_begin = """
void setup() {
  // Serial.begin(9600);
}
void loop() {
  Serial.println("ok");
}
"""
        code_with_commented_print = """
void setup() {}
void loop() {
  // Serial.println("debug");
  int value = analogRead(A0);
}
"""

        begin_issues = validate_arduino_code(code_with_commented_begin)
        print_issues = validate_arduino_code(code_with_commented_print)

        self.assertTrue(any("Serial.begin is missing" in issue for issue in begin_issues))
        self.assertFalse(any("Serial.begin" in issue for issue in print_issues))

    def test_validate_ignores_commented_entrypoints_and_private_include(self) -> None:
        code = """
// #include "define.h"
// void setup() {}
// void loop() {}
int value = 0;
"""

        issues = validate_arduino_code(code)

        self.assertTrue(any("Missing setup()" in issue for issue in issues))
        self.assertTrue(any("Missing loop()" in issue for issue in issues))
        self.assertFalse(any("define.h" in issue for issue in issues))

    def test_validate_accepts_empty_setup_when_loop_has_logic(self) -> None:
        code = """
void setup() {}
void loop() {
  int value = analogRead(A0);
}
"""

        self.assertEqual(validate_arduino_code(code), [])

    def test_validate_accepts_liquidcrystal_with_include_and_constructor(self) -> None:
        code = """
#include <LiquidCrystal.h>
int rsPin = 12; // [PARAM:id=display.rsPin|module=display|name=rsPin|type=int]
int enPin = 11; // [PARAM:id=display.enPin|module=display|name=enPin|type=int]
int d4Pin = 5; // [PARAM:id=display.d4Pin|module=display|name=d4Pin|type=int]
int d5Pin = 4; // [PARAM:id=display.d5Pin|module=display|name=d5Pin|type=int]
int d6Pin = 3; // [PARAM:id=display.d6Pin|module=display|name=d6Pin|type=int]
int d7Pin = 2; // [PARAM:id=display.d7Pin|module=display|name=d7Pin|type=int]
LiquidCrystal lcd(rsPin, enPin, d4Pin, d5Pin, d6Pin, d7Pin);
void setup() {
  lcd.begin(16, 2);
}
void loop() {
  lcd.print("ok");
}
"""

        self.assertEqual(validate_arduino_code(code), [])

    def test_validate_accepts_softwareserial_with_include_and_constructor(self) -> None:
        code = """
#include <SoftwareSerial.h>
int gpsRxPin = 10; // [PARAM:id=gps.gpsRxPin|module=gps|name=gpsRxPin|type=int]
int gpsTxPin = 11; // [PARAM:id=gps.gpsTxPin|module=gps|name=gpsTxPin|type=int]
SoftwareSerial gpsSerial(gpsRxPin, gpsTxPin);
void setup() {
  gpsSerial.begin(9600);
}
void loop() {
  if (gpsSerial.available()) {
    int value = gpsSerial.read();
  }
}
"""

        self.assertEqual(validate_arduino_code(code), [])
        self.assertEqual(validate_pin_usage(code), [])
        self.assertEqual(validate_hardware_pin_references(code), [])

    def test_validate_accepts_stepper_with_include_and_named_constructor_pins(self) -> None:
        code = """
#include <Stepper.h>
int in1Pin = 8; // [PARAM:id=motor.in1Pin|module=motor|name=in1Pin|type=int]
int in2Pin = 10; // [PARAM:id=motor.in2Pin|module=motor|name=in2Pin|type=int]
int in3Pin = 9; // [PARAM:id=motor.in3Pin|module=motor|name=in3Pin|type=int]
int in4Pin = 11; // [PARAM:id=motor.in4Pin|module=motor|name=in4Pin|type=int]
int stepsPerRevolution = 2048; // [PARAM:id=motor.stepsPerRevolution|module=motor|name=stepsPerRevolution|type=int|min=1|max=4096]
Stepper motorStepper(stepsPerRevolution, in1Pin, in2Pin, in3Pin, in4Pin);
void setup() {
  motorStepper.setSpeed(10);
}
void loop() {
  motorStepper.step(1);
}
"""

        self.assertEqual(validate_arduino_code(code), [])
        self.assertEqual(validate_pin_usage(code), [])
        self.assertEqual(validate_hardware_pin_references(code), [])

    def test_validate_accepts_wire_and_spi_with_real_includes(self) -> None:
        code = """
#include <Wire.h>
#include <SPI.h>
void setup() {
  Wire.begin();
  SPI.begin();
}
void loop() {
  Wire.requestFrom(0x40, 1);
  byte value = SPI.transfer(0x00);
}
"""

        self.assertEqual(validate_arduino_code(code), [])

    def test_validate_does_not_treat_display_param_ids_as_tm1637_usage(self) -> None:
        code = """
int displayMode = 1; // [PARAM:id=display.displayMode|module=display|name=displayMode|type=int|min=0|max=3]
void setup() {}
void loop() {
  int mode = displayMode;
}
"""

        issues = validate_arduino_code(code)

        self.assertFalse(any("TM1637" in issue for issue in issues))

    def test_validate_rejects_common_library_usage_without_required_include(self) -> None:
        code = """
Servo gateServo;
LiquidCrystal lcd;
SoftwareSerial gpsSerial;
Stepper motorStepper;
void setup() {
  gateServo.attach(9);
}
void loop() {
  gateServo.write(90);
}
"""

        issues = validate_arduino_code(code)

        self.assertTrue(any("Servo usage requires #include <Servo.h>" in issue for issue in issues))
        self.assertTrue(any("LiquidCrystal usage requires #include <LiquidCrystal.h>" in issue for issue in issues))
        self.assertTrue(any("LiquidCrystal object construction is missing or malformed" in issue for issue in issues))
        self.assertTrue(any("SoftwareSerial usage requires #include <SoftwareSerial.h>" in issue for issue in issues))
        self.assertTrue(any("SoftwareSerial object construction is missing or malformed" in issue for issue in issues))
        self.assertTrue(any("Stepper usage requires #include <Stepper.h>" in issue for issue in issues))
        self.assertTrue(any("Stepper object construction is missing or malformed" in issue for issue in issues))

    def test_validate_rejects_common_library_usage_with_commented_out_includes(self) -> None:
        code = """
// #include <Servo.h>
/*
#include <LiquidCrystal.h>
*/
Servo gateServo;
int rsPin = 12; // [PARAM:id=display.rsPin|module=display|name=rsPin|type=int]
int enPin = 11; // [PARAM:id=display.enPin|module=display|name=enPin|type=int]
int d4Pin = 5; // [PARAM:id=display.d4Pin|module=display|name=d4Pin|type=int]
int d5Pin = 4; // [PARAM:id=display.d5Pin|module=display|name=d5Pin|type=int]
int d6Pin = 3; // [PARAM:id=display.d6Pin|module=display|name=d6Pin|type=int]
int d7Pin = 2; // [PARAM:id=display.d7Pin|module=display|name=d7Pin|type=int]
LiquidCrystal lcd(rsPin, enPin, d4Pin, d5Pin, d6Pin, d7Pin);
void setup() {
  gateServo.attach(9);
  lcd.begin(16, 2);
}
void loop() {
  gateServo.write(90);
}
"""

        issues = validate_arduino_code(code)

        self.assertTrue(any("Servo usage requires #include <Servo.h>" in issue for issue in issues))
        self.assertTrue(any("LiquidCrystal usage requires #include <LiquidCrystal.h>" in issue for issue in issues))

    def test_validate_rejects_wire_spi_usage_without_real_includes(self) -> None:
        code = """
// #include <Wire.h>
/*
#include <SPI.h>
*/
void setup() {
  Wire.begin();
  SPI.begin();
}
void loop() {
  Wire.requestFrom(0x40, 1);
  byte value = SPI.transfer(0x00);
}
"""

        issues = validate_arduino_code(code)

        self.assertTrue(any("Wire usage requires #include <Wire.h>" in issue for issue in issues))
        self.assertTrue(any("SPI usage requires #include <SPI.h>" in issue for issue in issues))

    def test_validate_rejects_empty_or_comment_only_loop(self) -> None:
        empty_loop_issues = validate_arduino_code(
            """
void setup() {}
void loop() {}
"""
        )
        comment_loop_issues = validate_arduino_code(
            """
void setup() {}
void loop() {
  // waiting for implementation
}
"""
        )

        self.assertTrue(any("loop() is empty" in issue for issue in empty_loop_issues))
        self.assertTrue(any("loop() is empty" in issue for issue in comment_loop_issues))

    def test_validate_rejects_incomplete_placeholder_sketch(self) -> None:
        issues = validate_arduino_code(
            """
void setup() {
  // TODO: replace with real implementation
}
"""
        )

        self.assertTrue(any("Missing loop()" in issue for issue in issues))
        self.assertTrue(any("Placeholder text" in issue for issue in issues))

    def test_requirement_coverage_flags_missing_buzzer_logic(self) -> None:
        issues = check_requirement_coverage(
            "Generate an alarm system with buzzer output when threshold is exceeded.",
            """
void setup() {}
void loop() {
  int value = analogRead(A0);
}
""",
        )

        self.assertTrue(any("alarm/buzzer" in issue for issue in issues))

    def test_validate_module_markers_accepts_complete_specs(self) -> None:
        code = """
// [MODULE:input_capture:start]
void readInput() {}
// [MODULE:input_capture:end]
// [MODULE:decision:start]
void decide() {}
// [MODULE:decision:end]
void setup() {}
void loop() {}
"""

        self.assertEqual(validate_module_markers(code, ["input_capture", "decision"]), [])

    def test_validate_module_markers_reports_missing_and_unknown_ids(self) -> None:
        code = """
// [MODULE:unexpected:start]
void unexpectedFn() {}
// [MODULE:unexpected:end]
void setup() {}
void loop() {}
"""

        issues = validate_module_markers(code, ["input_capture"])

        self.assertTrue(any("input_capture" in issue for issue in issues))
        self.assertTrue(any("unexpected" in issue for issue in issues))

    def test_validate_module_boundaries_rejects_entrypoints_and_nested_markers(self) -> None:
        code = """
// [MODULE:control:start]
void setup() {}
// [MODULE:inner:start]
void runInner() {}
// [MODULE:inner:end]
void loop() {}
// [MODULE:control:end]
"""

        issues = validate_module_boundaries(code)

        self.assertTrue(any("wraps Arduino setup()" in issue for issue in issues))
        self.assertTrue(any("wraps Arduino loop()" in issue for issue in issues))
        self.assertTrue(any("nested MODULE marker" in issue for issue in issues))

    def test_validate_module_marker_structure_rejects_malformed_and_duplicate_markers(self) -> None:
        code = """
void badInline() {} // [MODULE:inline:start]
// [MODULE:orphan:end]
// [MODULE:control:start]
void runControl() {}
// [MODULE:nested:start]
void runNested() {}
// [MODULE:nested:end]
// [MODULE:control:end]
// [MODULE:control:start]
void runControlAgain() {}
"""

        issues = validate_module_marker_structure(code)

        self.assertTrue(any("standalone line" in issue for issue in issues))
        self.assertTrue(any("no matching start marker" in issue for issue in issues))
        self.assertTrue(any("must not overlap or nest" in issue for issue in issues))
        self.assertTrue(any("multiple editable blocks" in issue for issue in issues))
        self.assertTrue(any("no matching end marker" in issue for issue in issues))

    def test_validate_module_execution_accepts_called_module_helpers(self) -> None:
        code = """
// [MODULE:input_capture:start]
void readInput() {}
// [MODULE:input_capture:end]
// [MODULE:decision:start]
void decide() {}
// [MODULE:decision:end]
void setup() {}
void loop() {
  readInput();
  decide();
}
"""

        self.assertEqual(validate_module_execution(code), [])

    def test_validate_module_execution_accepts_interrupt_callback_registration(self) -> None:
        code = """
int buttonPin = 2;
// [MODULE:input_capture:start]
void onButton() {}
// [MODULE:input_capture:end]
void setup() {
  attachInterrupt(digitalPinToInterrupt(buttonPin), onButton, FALLING);
}
void loop() {}
"""

        self.assertEqual(validate_module_execution(code), [])

    def test_validate_module_execution_reports_uncalled_module_helper(self) -> None:
        code = """
// [MODULE:input_capture:start]
void readInput() {}
// [MODULE:input_capture:end]
void setup() {}
void loop() {}
"""

        issues = validate_module_execution(code)

        self.assertTrue(any("readInput" in issue and "never called" in issue for issue in issues))

    def test_validate_param_markers_accepts_complete_marker(self) -> None:
        code = """
int threshold = 70; // [PARAM:id=decision.threshold|module=decision|name=threshold|zh_name=阈值|type=int|min=0|max=100]
int currentLevel = map(analogRead(A0), 0, 1023, 0, 100);
void setup() {}
void loop() {}
"""

        self.assertEqual(validate_param_markers(code), [])

    def test_validate_param_markers_accepts_numeric_range_metadata(self) -> None:
        code = """
unsigned long sampleIntervalMs = 100; // [PARAM:id=input.sampleIntervalMs|module=input|name=sampleIntervalMs|type=unsigned long|min=10|max=10000]
int micPin = A0; // [PARAM:id=input.micPin|module=input|name=micPin|type=int]
int ledPin = LED_BUILTIN; // [PARAM:id=alert.ledPin|module=alert|name=ledPin|type=int]
void setup() {}
void loop() {}
"""

        self.assertEqual(validate_param_markers(code), [])

    def test_validate_param_markers_rejects_non_literal_or_injected_defaults(self) -> None:
        code = """
int threshold = computeThreshold(); // [PARAM:id=decision.threshold|module=decision|name=threshold|type=int|min=0|max=100]
int intervalMs = 100; int hidden = 1; // [PARAM:id=timer.intervalMs|module=timer|name=intervalMs|type=int]
bool enabled = maybeEnabled(); // [PARAM:id=decision.enabled|module=decision|name=enabled|type=bool]
"""

        issues = validate_param_markers(code)

        self.assertTrue(any("computeThreshold()" in issue and "integer literal" in issue for issue in issues))
        self.assertTrue(any("extra code after the declaration" in issue for issue in issues))
        self.assertTrue(any("maybeEnabled()" in issue and "bool literal" in issue for issue in issues))

    def test_validate_param_markers_requires_marker_for_tunable_constants(self) -> None:
        code = """
int threshold = 70;
int buzzerPin = 8;
void setup() {}
void loop() {}
"""

        issues = validate_param_markers(code)

        self.assertTrue(any("threshold" in issue for issue in issues))
        self.assertTrue(any("buzzerPin" in issue for issue in issues))

    def test_validate_param_markers_rejects_broken_marker_layout_and_duplicates(self) -> None:
        code = """
int threshold = 70;
// [PARAM:id=decision.threshold|module=decision|name=threshold|type=int]
int otherThreshold = 80; // [PARAM:id=decision.threshold|module=decision|name=threshold|type=int]
int thresholdCopy = 90; // [PARAM:id=decision.threshold|module=decision|name=thresholdCopy|type=int]
"""

        issues = validate_param_markers(code)

        self.assertTrue(any("same line" in issue for issue in issues))
        self.assertTrue(any("does not match" in issue for issue in issues))
        self.assertTrue(any("duplicate PARAM id" in issue for issue in issues))

    def test_validate_param_markers_checks_type_and_numeric_ranges(self) -> None:
        code = """
int threshold = 70; // [PARAM:id=decision.threshold|module=decision|name=threshold|type=float|min=0|max=50]
int invertedRange = 10; // [PARAM:id=decision.invertedRange|module=decision|name=invertedRange|type=int|min=20|max=5]
int badMin = 10; // [PARAM:id=decision.badMin|module=decision|name=badMin|type=int|min=low|max=20]
"""

        issues = validate_param_markers(code)

        self.assertTrue(any("type 'float' does not match declaration type 'int'" in issue for issue in issues))
        self.assertTrue(any("default value 70 is above max 50" in issue for issue in issues))
        self.assertTrue(any("min 20 is greater than max 5" in issue for issue in issues))
        self.assertTrue(any("min value 'low' is not a numeric literal" in issue for issue in issues))

    def test_validate_param_markers_checks_module_membership_and_active_block(self) -> None:
        code = """
// [MODULE:alert:start]
int threshold = 70; // [PARAM:id=decision.threshold|module=decision|name=threshold|type=int]
// [MODULE:alert:end]
int orphanPin = 8; // [PARAM:id=orphan.orphanPin|module=orphan|name=orphanPin|type=int]
"""

        issues = validate_param_markers(code, ["alert", "decision"])

        self.assertTrue(any("does not match active MODULE block 'alert'" in issue for issue in issues))
        self.assertTrue(any("PARAM module 'orphan' is not one of expected module ids" in issue for issue in issues))

    def test_repair_prompt_includes_module_and_pin_context(self) -> None:
        prompt = build_arduino_repair_prompt(
            ["Missing editable MODULE markers for module_id(s): decision"],
            "void setup() {}\nvoid loop() {}",
            user_requirements="蜂鸣器超过阈值后告警",
            module_specs=[
                {"id": "decision", "title": "决策模块", "moduleType": "decision", "subtitle": "判断阈值"},
                {"id": "alert", "title": "告警模块", "moduleType": "alert", "subtitle": "驱动蜂鸣器"},
            ],
            pin_definitions={"alert": "buzzerPin=8"},
        )

        self.assertIn("id=decision", prompt)
        self.assertIn("id=alert", prompt)
        self.assertIn("buzzerPin=8", prompt)
        self.assertIn("Marker contract", prompt)
        self.assertIn("[PARAM:", prompt)
        self.assertIn("Runtime reliability contract", prompt)
        self.assertIn("millis()-based scheduling", prompt)
        self.assertIn("raw literals", prompt)
        self.assertIn("INPUT_PULLUP", prompt)
        self.assertIn("A0-A5", prompt)
        self.assertIn("PWM", prompt)
        self.assertIn("analogRead", prompt)
        self.assertIn("ESP32", prompt)
        self.assertIn("custom main", prompt)
        self.assertIn("stdio", prompt)
        self.assertIn("single literal", prompt)
        self.assertIn("non-overlapping", prompt)
        self.assertIn("never nested", prompt)
        self.assertIn("Servo.attach", prompt)
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

    def test_validate_uno_board_compatibility_accepts_standard_uno_code(self) -> None:
        code = """
#include <Servo.h>
Servo gateServo;
void setup() {
  Serial.begin(9600);
  pinMode(2, INPUT_PULLUP);
}
void loop() {
  gateServo.write(90);
}
"""

        self.assertEqual(validate_uno_board_compatibility(code), [])

    def test_validate_uno_board_compatibility_rejects_non_uno_headers_and_apis(self) -> None:
        code = """
#include <WiFi.h>
#include <BluetoothSerial.h>
#include <iostream>
#include <stdio.h>
using namespace std;
int main() {
  std::cout << "desktop";
  return 0;
}
void setup() {
  Serial1.begin(9600);
  printf("hello");
  scanf("%d", 0);
  pinMode(2, INPUT_PULLDOWN);
  ledcWrite(0, 128);
  analogReadResolution(12);
  touchRead(4);
  ESP.restart();
}
void loop() {}
"""

        issues = validate_uno_board_compatibility(code)

        self.assertTrue(any("WiFi.h" in issue for issue in issues))
        self.assertTrue(any("BluetoothSerial.h" in issue for issue in issues))
        self.assertTrue(any("Serial1" in issue for issue in issues))
        self.assertTrue(any("INPUT_PULLDOWN" in issue for issue in issues))
        self.assertTrue(any("ledc" in issue for issue in issues))
        self.assertTrue(any("analogReadResolution" in issue for issue in issues))
        self.assertTrue(any("touchRead" in issue for issue in issues))
        self.assertTrue(any("ESP platform APIs" in issue for issue in issues))
        self.assertTrue(any("iostream" in issue for issue in issues))
        self.assertTrue(any("main()" in issue for issue in issues))
        self.assertTrue(any("namespace" in issue for issue in issues))
        self.assertTrue(any("console I/O" in issue for issue in issues))
        self.assertTrue(any("stdio.h" in issue for issue in issues))
        self.assertTrue(any("stdio output" in issue for issue in issues))
        self.assertTrue(any("stdio input" in issue for issue in issues))

    def test_validate_arduino_static_quality_aggregates_all_static_checks(self) -> None:
        code = """
#include <WiFi.h>
int motorPwmPin = D8; // [PARAM:id=motor.motorPwmPin|module=motor|name=motorPwmPin|type=int]
int buzzerPin = 9; // [PARAM:id=alert.buzzerPin|module=alert|name=buzzerPin|type=int]
void setup() {
  pinMode(motorPwmPin, OUTPUT);
  pinMode(8, OUTPUT);
}
void loop() {
  analogWrite(motorPwmPin, 128);
  digitalWrite(8, HIGH);
}
"""

        issues = validate_arduino_static_quality(
            code,
            module_ids=["motor", "alert"],
            pin_definitions={"alert": "buzzerPin=8"},
        )

        self.assertTrue(any("WiFi.h" in issue for issue in issues))
        self.assertTrue(any("Missing editable MODULE markers" in issue for issue in issues))
        self.assertTrue(any("buzzerPin" in issue and "expects value '8'" in issue for issue in issues))
        self.assertTrue(any("analogWrite" in issue and "motorPwmPin" in issue for issue in issues))
        self.assertTrue(any("digitalWrite uses raw pin literal '8'" in issue for issue in issues))

    def test_validate_pin_definitions_accepts_module_pin_mapping(self) -> None:
        code = """
// [MODULE:alert:start]
int buzzerPin = 8; // [PARAM:id=alert.buzzerPin|module=alert|name=buzzerPin|zh_name=蜂鸣器引脚|type=int]
void driveAlert() { tone(buzzerPin, 1000); }
// [MODULE:alert:end]
void setup() {}
void loop() {}
"""

        self.assertEqual(validate_pin_definitions_applied(code, {"alert": "buzzerPin=8"}), [])

    def test_validate_pin_definitions_accepts_direct_api_and_digital_prefix_equivalence(self) -> None:
        code = """
int MIC_PIN = A1; // [PARAM:id=input.MIC_PIN|module=input|name=MIC_PIN|type=int]
int CLK = 4; // [PARAM:id=display.CLK|module=display|name=CLK|type=int]
void setup() {}
void loop() {}
"""

        issues = validate_pin_definitions_applied(code, {"MIC_PIN": "A1", "display": "CLK=D4"})

        self.assertEqual(issues, [])

    def test_validate_pin_definitions_reports_missing_or_wrong_values(self) -> None:
        code = """
int buzzerPin = 9; // [PARAM:id=alert.buzzerPin|module=alert|name=buzzerPin|type=int]
void setup() {}
void loop() {}
"""

        issues = validate_pin_definitions_applied(code, {"alert": "buzzerPin=8, ledPin=D3"})

        self.assertTrue(any("buzzerPin" in issue and "expects value '8'" in issue for issue in issues))
        self.assertTrue(any("ledPin=D3" in issue for issue in issues))

    def test_validate_uno_pin_values_accepts_uno_pin_literals(self) -> None:
        code = """
int micPin = A5; // [PARAM:id=input.micPin|module=input|name=micPin|type=int]
int buzzerPin = D13; // [PARAM:id=alert.buzzerPin|module=alert|name=buzzerPin|type=int]
int statusPin = LED_BUILTIN; // [PARAM:id=alert.statusPin|module=alert|name=statusPin|type=int]
void setup() {}
void loop() {}
"""

        self.assertEqual(validate_uno_pin_values(code), [])

    def test_validate_uno_pin_values_reports_out_of_range_pins(self) -> None:
        code = """
int micPin = A8; // [PARAM:id=input.micPin|module=input|name=micPin|type=int]
int relayPin = D22; // [PARAM:id=actuator.relayPin|module=actuator|name=relayPin|type=int]
int placeholderPin = DEFAULT_PIN; // [PARAM:id=actuator.placeholderPin|module=actuator|name=placeholderPin|type=int]
void setup() {}
void loop() {}
"""

        issues = validate_uno_pin_values(code)

        self.assertTrue(any("micPin" in issue and "A0-A5" in issue for issue in issues))
        self.assertTrue(any("relayPin" in issue and "D0-D13" in issue for issue in issues))
        self.assertTrue(any("placeholderPin" in issue and "UNO literal" in issue for issue in issues))

    def test_validate_reserved_serial_pins_rejects_d0_d1_for_digital_hardware(self) -> None:
        code = """
int buttonPin = D0; // [PARAM:id=input.buttonPin|module=input|name=buttonPin|type=int]
int ledPin = 1; // [PARAM:id=alert.ledPin|module=alert|name=ledPin|type=int]
void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
}
void loop() {
  if (digitalRead(buttonPin) == LOW) {
    digitalWrite(ledPin, HIGH);
  }
}
"""

        issues = validate_reserved_serial_pins(code)

        self.assertTrue(any("buttonPin" in issue and "D0" in issue and "USB Serial/upload" in issue for issue in issues))
        self.assertTrue(any("ledPin" in issue and "D1" in issue and "USB Serial/upload" in issue for issue in issues))

    def test_validate_reserved_serial_pins_allows_numeric_analog_channels(self) -> None:
        code = """
int auxAnalogPin = 0; // [PARAM:id=input.auxAnalogPin|module=input|name=auxAnalogPin|type=int]
int backupAnalogPin = 1; // [PARAM:id=input.backupAnalogPin|module=input|name=backupAnalogPin|type=int]
void setup() {}
void loop() {
  int aux = analogRead(auxAnalogPin);
  int backup = analogRead(backupAnalogPin);
}
"""

        self.assertEqual(validate_reserved_serial_pins(code), [])

    def test_validate_reserved_serial_pins_rejects_d0_d1_in_pin_constructors(self) -> None:
        code = """
#include <SoftwareSerial.h>
int gpsRxPin = 0; // [PARAM:id=gps.gpsRxPin|module=gps|name=gpsRxPin|type=int]
int gpsTxPin = D1; // [PARAM:id=gps.gpsTxPin|module=gps|name=gpsTxPin|type=int]
SoftwareSerial gpsSerial(gpsRxPin, gpsTxPin);
void setup() {
  gpsSerial.begin(9600);
}
void loop() {
  if (gpsSerial.available()) {
    int value = gpsSerial.read();
  }
}
"""

        issues = validate_reserved_serial_pins(code)

        self.assertTrue(any("gpsRxPin" in issue and "D0" in issue for issue in issues))
        self.assertTrue(any("gpsTxPin" in issue and "D1" in issue for issue in issues))

    def test_validate_pin_assignment_conflicts_reports_duplicate_physical_pins(self) -> None:
        code = """
int buzzerPin = 8; // [PARAM:id=alert.buzzerPin|module=alert|name=buzzerPin|type=int]
int relayPin = D8; // [PARAM:id=relay.relayPin|module=relay|name=relayPin|type=int]
int ledPin = LED_BUILTIN; // [PARAM:id=status.ledPin|module=status|name=ledPin|type=int]
int indicatorPin = D13; // [PARAM:id=indicator.indicatorPin|module=indicator|name=indicatorPin|type=int]
void setup() {
  pinMode(buzzerPin, OUTPUT);
  pinMode(relayPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(indicatorPin, OUTPUT);
}
void loop() {
  digitalWrite(buzzerPin, HIGH);
  digitalWrite(relayPin, LOW);
  digitalWrite(ledPin, HIGH);
  digitalWrite(indicatorPin, LOW);
}
"""

        issues = validate_pin_assignment_conflicts(code)

        self.assertTrue(any("relay.relayPin" in issue and "physical pin D8" in issue for issue in issues))
        self.assertTrue(any("indicator.indicatorPin" in issue and "physical pin D13" in issue for issue in issues))

    def test_validate_pin_assignment_conflicts_treats_numeric_analog_channels_as_analog_read_pins(self) -> None:
        code = """
int auxAnalogPin = 0; // [PARAM:id=input.auxAnalogPin|module=input|name=auxAnalogPin|type=int]
int serialRxPin = D0; // [PARAM:id=gps.serialRxPin|module=gps|name=serialRxPin|type=int]
int backupAnalogPin = A0; // [PARAM:id=input.backupAnalogPin|module=input|name=backupAnalogPin|type=int]
void setup() {
  pinMode(serialRxPin, INPUT_PULLUP);
}
void loop() {
  int aux = analogRead(auxAnalogPin);
  int backup = analogRead(backupAnalogPin);
  if (digitalRead(serialRxPin) == LOW) {}
}
"""

        issues = validate_pin_assignment_conflicts(code)

        self.assertFalse(any("serialRxPin" in issue and "auxAnalogPin" in issue for issue in issues))
        self.assertTrue(any("backupAnalogPin" in issue and "physical pin A0" in issue for issue in issues))

    def test_validate_arduino_static_quality_aggregates_pin_assignment_conflicts(self) -> None:
        code = """
int buzzerPin = 8; // [PARAM:id=alert.buzzerPin|module=alert|name=buzzerPin|type=int]
int relayPin = D8; // [PARAM:id=relay.relayPin|module=relay|name=relayPin|type=int]
void setup() {
  pinMode(buzzerPin, OUTPUT);
  pinMode(relayPin, OUTPUT);
}
void loop() {
  digitalWrite(buzzerPin, HIGH);
  digitalWrite(relayPin, LOW);
}
"""

        issues = validate_arduino_static_quality(code)

        self.assertTrue(any("physical pin D8" in issue for issue in issues))

    def test_validate_uno_pin_capabilities_accepts_pwm_and_interrupt_pins(self) -> None:
        code = """
int micPin = A3; // [PARAM:id=input.micPin|module=input|name=micPin|type=int]
int auxAnalogPin = 2; // [PARAM:id=input.auxAnalogPin|module=input|name=auxAnalogPin|type=int]
int motorPwmPin = D5; // [PARAM:id=motor.motorPwmPin|module=motor|name=motorPwmPin|type=int]
int buttonPin = 2; // [PARAM:id=input.buttonPin|module=input|name=buttonPin|type=int]
void onButton() {}
void setup() {
  pinMode(motorPwmPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(buttonPin), onButton, FALLING);
}
void loop() {
  int mic = analogRead(micPin);
  int aux = analogRead(auxAnalogPin);
  analogWrite(motorPwmPin, 128);
}
"""

        self.assertEqual(validate_uno_pin_capabilities(code), [])

    def test_validate_uno_pin_capabilities_reports_non_pwm_and_interrupt_pins(self) -> None:
        code = """
int micPin = D8; // [PARAM:id=input.micPin|module=input|name=micPin|type=int]
int motorPwmPin = D8; // [PARAM:id=motor.motorPwmPin|module=motor|name=motorPwmPin|type=int]
int buttonPin = D4; // [PARAM:id=input.buttonPin|module=input|name=buttonPin|type=int]
void onButton() {}
void setup() {
  pinMode(motorPwmPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(buttonPin), onButton, FALLING);
}
void loop() {
  int sample = analogRead(micPin);
  analogWrite(motorPwmPin, 128);
}
"""

        issues = validate_uno_pin_capabilities(code)

        self.assertTrue(any("analogRead" in issue and "micPin" in issue and "A0-A5" in issue for issue in issues))
        self.assertTrue(any("analogWrite" in issue and "motorPwmPin" in issue and "D3" in issue for issue in issues))
        self.assertTrue(any("digitalPinToInterrupt" in issue and "buttonPin" in issue and "D2 or D3" in issue for issue in issues))

    def test_validate_pin_usage_accepts_input_and_output_pins(self) -> None:
        code = """
int micPin = A1; // [PARAM:id=input.micPin|module=input|name=micPin|type=int]
int buzzerPin = 8; // [PARAM:id=alert.buzzerPin|module=alert|name=buzzerPin|type=int]
void setup() {
  pinMode(buzzerPin, OUTPUT);
}
void loop() {
  int sample = analogRead(micPin);
  if (sample > 700) {
    tone(buzzerPin, 1000);
  } else {
    noTone(buzzerPin);
  }
}
"""

        self.assertEqual(validate_pin_usage(code), [])

    def test_validate_pin_usage_reports_unused_pin_param(self) -> None:
        code = """
int buzzerPin = 8; // [PARAM:id=alert.buzzerPin|module=alert|name=buzzerPin|type=int]
void setup() {}
void loop() {}
"""

        issues = validate_pin_usage(code)

        self.assertTrue(any("not used in any Arduino hardware call" in issue for issue in issues))

    def test_validate_pin_usage_requires_pinmode_for_output_pin(self) -> None:
        code = """
int buzzerPin = 8; // [PARAM:id=alert.buzzerPin|module=alert|name=buzzerPin|type=int]
void setup() {}
void loop() {
  digitalWrite(buzzerPin, HIGH);
}
"""

        issues = validate_pin_usage(code)

        self.assertTrue(any("missing pinMode(buzzerPin" in issue for issue in issues))

    def test_validate_pin_usage_requires_output_mode_for_output_pin(self) -> None:
        code = """
int buzzerPin = 8; // [PARAM:id=alert.buzzerPin|module=alert|name=buzzerPin|type=int]
void setup() {
  pinMode(buzzerPin, INPUT);
}
void loop() {
  tone(buzzerPin, 1000);
}
"""

        issues = validate_pin_usage(code)

        self.assertTrue(any("pinMode(buzzerPin, OUTPUT)" in issue and "INPUT" in issue for issue in issues))

    def test_validate_pin_usage_accepts_interrupt_pin_with_input_mode(self) -> None:
        code = """
int buttonPin = 2; // [PARAM:id=input.buttonPin|module=input|name=buttonPin|type=int]
volatile bool pressed = false;
void onButton() {
  pressed = true;
}
void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(buttonPin), onButton, FALLING);
}
void loop() {}
"""

        self.assertEqual(validate_pin_usage(code), [])

    def test_validate_pin_usage_accepts_servo_attach_pin_without_pinmode(self) -> None:
        code = """
#include <Servo.h>
Servo gateServo;
int servoPin = 9; // [PARAM:id=actuator.servoPin|module=actuator|name=servoPin|type=int]
void setup() {
  gateServo.attach(servoPin);
}
void loop() {
  gateServo.write(90);
}
"""

        self.assertEqual(validate_pin_usage(code), [])

    def test_validate_pin_usage_accepts_library_constructor_pin_params(self) -> None:
        code = """
#include <TM1637Display.h>
int CLK = 2; // [PARAM:id=display.CLK|module=display|name=CLK|type=int]
int DIO = 3; // [PARAM:id=display.DIO|module=display|name=DIO|type=int]
TM1637Display display(CLK, DIO);
void setup() {
  display.setBrightness(2);
}
void loop() {
  display.showNumberDec(42);
}
"""

        self.assertEqual(validate_pin_usage(code), [])
        self.assertEqual(validate_hardware_pin_references(code), [])

    def test_validate_pin_usage_requires_pinmode_for_digital_input_pin(self) -> None:
        code = """
int buttonPin = 2; // [PARAM:id=input.buttonPin|module=input|name=buttonPin|type=int]
void setup() {}
void loop() {
  if (digitalRead(buttonPin) == LOW) {
  }
}
"""

        issues = validate_pin_usage(code)

        self.assertTrue(any("missing pinMode(buttonPin, INPUT or INPUT_PULLUP)" in issue for issue in issues))

    def test_validate_pin_usage_requires_input_mode_for_digital_input_pin(self) -> None:
        code = """
int buttonPin = 2; // [PARAM:id=input.buttonPin|module=input|name=buttonPin|type=int]
void setup() {
  pinMode(buttonPin, OUTPUT);
}
void loop() {
  if (digitalRead(buttonPin) == LOW) {
  }
}
"""

        issues = validate_pin_usage(code)

        self.assertTrue(any("pinMode(buttonPin, INPUT or INPUT_PULLUP)" in issue and "OUTPUT" in issue for issue in issues))

    def test_validate_hardware_pin_references_accepts_named_pin_params(self) -> None:
        code = """
int micPin = A0; // [PARAM:id=input.micPin|module=input|name=micPin|type=int]
int buzzerPin = 8; // [PARAM:id=alert.buzzerPin|module=alert|name=buzzerPin|type=int]
void setup() {
  pinMode(buzzerPin, OUTPUT);
}
void loop() {
  int sample = analogRead(micPin);
  if (sample > 700) {
    tone(buzzerPin, 1000);
  }
}
"""

        self.assertEqual(validate_hardware_pin_references(code), [])

    def test_validate_hardware_pin_references_reports_raw_pin_literals(self) -> None:
        code = """
void onPulse() {}
void setup() {
  pinMode(8, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(2), onPulse, CHANGE);
}
void loop() {
  int sample = analogRead(A0);
  digitalWrite(8, sample > 700 ? HIGH : LOW);
}
"""

        issues = validate_hardware_pin_references(code)

        self.assertTrue(any("pinMode uses raw pin literal '8'" in issue for issue in issues))
        self.assertTrue(any("analogRead uses raw pin literal 'A0'" in issue for issue in issues))
        self.assertTrue(any("digitalWrite uses raw pin literal '8'" in issue for issue in issues))
        self.assertTrue(any("digitalPinToInterrupt uses raw pin literal '2'" in issue for issue in issues))

    def test_validate_hardware_pin_references_reports_raw_servo_attach_pin(self) -> None:
        code = """
#include <Servo.h>
Servo gateServo;
void setup() {
  gateServo.attach(9);
}
void loop() {
  gateServo.write(90);
}
"""

        issues = validate_hardware_pin_references(code)

        self.assertTrue(any("attach uses raw pin literal '9'" in issue for issue in issues))

    def test_validate_hardware_pin_references_reports_raw_library_constructor_pins(self) -> None:
        code = """
#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
void setup() {
  lcd.begin(16, 2);
}
void loop() {
  lcd.print("ok");
}
"""

        issues = validate_hardware_pin_references(code)

        self.assertTrue(any("LiquidCrystal constructor uses raw pin literal '12'" in issue for issue in issues))
        self.assertTrue(any("LiquidCrystal constructor uses raw pin literal '2'" in issue for issue in issues))

    def test_validate_hardware_pin_references_reports_raw_softwareserial_constructor_pins(self) -> None:
        code = """
#include <SoftwareSerial.h>
SoftwareSerial gpsSerial(10, 11);
void setup() {
  gpsSerial.begin(9600);
}
void loop() {
  if (gpsSerial.available()) {
    int value = gpsSerial.read();
  }
}
"""

        issues = validate_hardware_pin_references(code)

        self.assertTrue(any("SoftwareSerial constructor uses raw pin literal '10'" in issue for issue in issues))
        self.assertTrue(any("SoftwareSerial constructor uses raw pin literal '11'" in issue for issue in issues))

    def test_validate_hardware_pin_references_reports_raw_stepper_constructor_pins(self) -> None:
        code = """
#include <Stepper.h>
Stepper motorStepper(2048, 8, 10, 9, 11);
void setup() {
  motorStepper.setSpeed(10);
}
void loop() {
  motorStepper.step(1);
}
"""

        issues = validate_hardware_pin_references(code)

        self.assertFalse(any("2048" in issue for issue in issues))
        self.assertTrue(any("Stepper constructor uses raw pin literal '8'" in issue for issue in issues))
        self.assertTrue(any("Stepper constructor uses raw pin literal '11'" in issue for issue in issues))

    def test_validate_runtime_safety_reports_long_loop_delay(self) -> None:
        code = """
unsigned long sampleIntervalMs = 1000; // [PARAM:id=input.sampleIntervalMs|module=input|name=sampleIntervalMs|type=unsigned long]
void setup() {
  delay(1000);
}
void loop() {
  delay(sampleIntervalMs);
}
"""

        issues = validate_runtime_safety(code)

        self.assertTrue(any("delay(sampleIntervalMs)" in issue for issue in issues))
        self.assertFalse(any("Line 4" in issue for issue in issues))

    def test_validate_runtime_safety_reports_long_delay_in_loop_helper(self) -> None:
        code = """
void runAlarm() {
  delay(1000);
}
void setup() {}
void loop() {
  runAlarm();
}
"""

        issues = validate_runtime_safety(code)

        self.assertTrue(any("delay(1000)" in issue for issue in issues))
        self.assertTrue(any("helper 'runAlarm' reachable from loop()" in issue for issue in issues))

    def test_validate_runtime_safety_allows_short_loop_delay(self) -> None:
        code = """
void setup() {}
void loop() {
  delay(20);
}
"""

        self.assertEqual(validate_runtime_safety(code), [])

    def test_validate_runtime_safety_reports_dynamic_memory(self) -> None:
        code = """
void setup() {}
void loop() {
  String message = "alarm";
  char *buf = (char *)malloc(16);
  int *count = new int(0);
}
"""

        issues = validate_runtime_safety(code)

        self.assertTrue(any("Arduino String" in issue for issue in issues))
        self.assertTrue(any("malloc/calloc/realloc/free" in issue for issue in issues))
        self.assertTrue(any("new/delete" in issue for issue in issues))


if __name__ == "__main__":
    unittest.main()
