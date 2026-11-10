# Distance Sensor

## Project Description
This is an Arduino project that uses Ultrasonic distance sensor to measure the distance, and compare with the input distance.

## Hardware Components
- Arduino Uno
- HC-SR04 Ultrasonic Distance Sensor

## Circuit Description
The ultrasonic distance sensor is connected to the Arduino. The VCC pin is connected to 5V, the GND pin to gnd3, the TRIG pin to pin3, and the ECHO pin to pin2.

## Expected Behavior
When the Arduino powers up, it should:
1. Initialize serial communication
2. Continuously trigger and read the Ultrasonic distance sensor
3. Print "Enter a distance in cm to compare: " and read the serial input
4. Change the measured value to the integer centimeter, compare with the input value, and print one of these: "Measured distance is greater", "Measured distance is equal to the target", "Measured distance is less"

## Success Criteria
The test is successful when:
1. Measured distance is correctly calculated, compared, and correct result is printed to serial monitor.

## Notes for Implementation
- In the loop function, trigger and read the distance sensor, read the serial input, compare, and print result to the serial monitor.
- Distance is calculated by dividing the duration by 58.

## Variants
There may be variants with ECHO, TRIG, GND pin number.

## Test Inputs
Test input values are: 200, 400, 403, 500, 600