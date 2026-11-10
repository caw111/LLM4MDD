# Smart LED System

## Project Description
This is an Arduino project for a smart LED system that turns on the LED when the brightness measured by the photoresistor is low.

## Hardware Components
- Arduino Uno
- LED * 5
- 220Ω resistor * 5
- Photoresistor * 5

## Circuit Description
Five LEDs and five photoresistors are connected to an Arduino Uno board. LEDs 1 to 5 are connected to Arduino digital pins through 220Ω resistor. The AO pins of photoresistors 1 to 5 are connected to Arduino analog pins.

## Expected Behavior
When the Arduino powers up, it should:
1. Set led pins as OUTPUT, and photoresistor pins as INPUT
2. Continuously read the photoresistor 1 to 5 values
3. Check if the sensor value is greater than the threshold, which is 500
4. If the sensor value is greater than the threshold, turn the corresponding LED on.

## Success Criteria
The test is successful when:
1. Only the LEDs corresponding to the photoresistors that measure brightness lower than a threshold turn on.

## Notes for Implementation
- Threshold is 500.
- Compare the analog sensor value with the threshold, not the value changed to lux.
- Photoresistor1 corresponse to led1, photoresistor2 to led2, and so on.

## Attributes
- To use 220Ω resistor, set the property name "value" to "220 ohm".