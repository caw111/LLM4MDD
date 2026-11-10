# Photoresistor

## Project Description
This is a simple Arduino project that uses 4 pin photoresistoer sensor module to sense the light.

## Hardware Components
- Arduino Uno
- Photoresistor sensor module

## Circuit Description
The photoresistor sensor module is connected to the Arduino. The AO pin is connected to arduino digital pin.

## Expected Behavior
When the Arduino powers up, it should:
1. Initialize serial communication
2. Continuously read the photoresistor sensor module, and print the value in the format: "LDR Value: {value}".

## Success Criteria
The test is successful when:
1. Correct LDR value is printed in the serial monitor.

## Notes for Implementation
- In the loop function, read the photoresistor, and print it to the serial monitor.
- Delay 500ms in every loop.

## Variants
There may be variants with init lux value and pin number.