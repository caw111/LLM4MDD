# Servo Motor Control

## Project Description
This is a simple Arduino project that uses servo motor.

## Hardware Components
- Arduino Uno
- Servo Motor

## Circuit Description
The servo motor is connected to the Arduino. The PWM is connected to arduino digital pin.

## Expected Behavior
When the Arduino powers up, it should:
1. Attach servo motor to digital pin
2. Move the servo from 0 to 180 degrees in 1-degree steps
3. When the moving is finished, print the current servo position to the serial monitor by reading the servo position.
4. Move the servo from 180 to 0 degrees in 1-degree steps
5. When the moving is finished, print the current servo position to the serial monitor by reading the servo position.
6. Repeat 3~6 process.

## Success Criteria
The test is successful when:
1. The servo motor moves correctly.
2. Correct position is printed to the serial monitor.

## Notes for Implementation
- Use Servo library to use the servo motor.
- Print only the number when printing the angle.

## Variants
There may be variants with GND pin, PWM pin.