# Distance Sensor

## Project Description
This is a simple Arduino project that uses Ultrasonic distance sensor to measure the distance.

## Hardware Components
- Arduino Uno
- Ultrasonic Distance Sensor

## Circuit Description
The ultrasonic distance sensor is connected to the Arduino. The TRIG pin and the ECHO pin is connected to arduino digital pins.

## Expected Behavior
When the Arduino powers up, it should:
1. Initialize serial communication
2. Continuously trigger and read the Ultrasonic distance sensor
3. Change the value to the centimeter and print in the format: "Distance in CM: {value}"

## Success Criteria
The test is successful when:
1. Correct distance value in centimeter is printed in the serial monitor.

## Notes for Implementation
- To convert the distance sensor value to centimeters, divide it by 58.

## Variants
There may be variants with init distance value and ECHO, TRIG, GND pin number.