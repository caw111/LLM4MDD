# Potentiometer Servo Motor

## Project Description
This is a simple Arduino project that controls servo motor with a potentiometer.

## Hardware Components
- Arduino Uno
- Servo motor
- Potentiometer

## Circuit Description
The SIG pin of the potentiometer is connected to the a0. The PWN pin of servo motor is connected to the pin9. GND pins are connected to gnd1, and VCC pins are connected to 5v. 

## Expected Behavior
When the Arduino powers up, it should:
1. Generate Servo object
2. Set potentiometer signal pin a0 as INPUT
3. Continuously read the potentiometer value, which ranges from 0 to 1023, and map it to a range of 0 to 180
4. Write the mapped value to the servo motor
5. Continuously read the servo value and print it to the serial monitor 

## Success Criteria
The test is successful when:
1. The servo motor rotates correctly according to the mapped value from the potentiometer.

## Notes for Implementation
- Potentiometer value ranges from 0 to 1023.
- Servo motor value ranges from 0 to 180.

## Variants
This project may have different variants with the servo motor, potentiometer connected to different pins.