#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_servo_motor_basic\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Servo.h>

Servo myServo;

#line 5 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_servo_motor_basic\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 16 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_servo_motor_basic\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 5 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_servo_motor_basic\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  Serial.begin(9600);

  // 1. Attach servo motor to pin 9
  myServo.attach(9);

  // Optional: ensure servo starts at 0 before sweeping
  myServo.write(0);
  delay(300);
}

void loop() {
  // Repeat 3~6 process
  for (;;) {
    // 2. Move the servo from 0 to 180 degrees in 1-degree steps
    for (int pos = 0; pos <= 180; pos++) {
      myServo.write(pos);
      delay(15);
    }

    // 3. Print the current servo position to the serial monitor by reading the servo position.
    // Servo position corresponds to the last commanded angle.
    Serial.println(180);

    // 4. Move the servo from 180 to 0 degrees in 1-degree steps
    for (int pos = 180; pos >= 0; pos--) {
      myServo.write(pos);
      delay(15);
    }

    // 5. Print the current servo position to the serial monitor by reading the servo position.
    Serial.println(0);

    // 6. Repeat process
    delay(200);
  }
}
