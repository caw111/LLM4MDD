# Serial Bar LED

## Project Description
This is a simple Arduino project that controls bar graph LED with serial monitor.

## Hardware Components
- Arduino Uno
- bar graph LED
- 220Ω resistor * 10

## Circuit Description
Anode pins 1~10 of bar graph LED are connected to arduino digital pins. The cathode pins 1 through 10 of the bar graph LED are connected to ground through 220kΩ resistors.

## Expected Behavior
When the Arduino powers up, it should:
1. Set digital pin modes as OUTPUT
2. Configure serial monitor
3. Continuously read the serial input, which is the threshold
4. Turn on the LEDs only up to the threshold

## Success Criteria
The test is successful when:
1. Anode of the bar LED pins whose number is smaller than the threshold receive HIGH signals, while greater pins receive LOW signals.

## Notes for Implementation
- Serial input is an integer.

## Variants
This project may have different variants with the LED connected to different pins or using LEDs of different colors.

## Test Inputs
Test input values are: 6, 3, 9

## Attributes
- To use 220Ω resistor, set the property name "value" to "220 ohm".