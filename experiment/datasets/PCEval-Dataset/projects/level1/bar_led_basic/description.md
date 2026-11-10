# Bar LED

## Project Description
This is a simple Arduino project that turns on an bar graph LED.

## Hardware Components
- Arduino Uno
- Bar graph LED
- 220Ω resistor * 10

## Circuit Description
Anode pins 1~10 of bar graph LED are connected to pin2, pin3, pin4, pin5, pin6, pin7, pin8, pin9, pin10, pin11, respectively. The cathode pins 1 through 10 of the bar graph LED are connected to GND pin gnd1 through 220kΩ resistors.

## Expected Behavior
When the Arduino powers up, it should:
1. Set pin2, pin3, pin4, pin5, pin6, pin7, pin8, pin9, pin10, pin11 pin mode as OUTPUT
2. Turn on the bar graph LEDs only up to the threshold, which is 6

## Success Criteria
The test is successful when:
1. Pins 1 to 6 of the bar graph LED anodes receive HIGH signals, while pins 7 to 10 receive LOW signals.

## Notes for Implementation
- The LED should turn on immediately during setup and stay on
- No actions are required in the loop function
- The pin number used should match the circuit diagram

## Variants
This project may have different variants with the bar graph LED connected to different pins or using bar graph LED of different colors.

## Attributes
- To use 220Ω resistor, set the property name "value" to "220 ohm".