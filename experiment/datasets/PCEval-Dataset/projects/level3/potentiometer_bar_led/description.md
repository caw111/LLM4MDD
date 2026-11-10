# Potentiometer Bar LED

## Project Description
This is a simple Arduino project that controls bar graph LED with a potentiometer.

## Hardware Components
- Arduino Uno
- Bar graph LED
- 220Ω resistor * 10
- Potentiometer

## Circuit Description
Anode pins 1~10 of bar graph LED are connected to pin2, pin3, pin4, pin5, pin6, pin7, pin8, pin9, pin10, pin11, respectively. The cathode pins 1 through 10 of the bar graph LED are connected to GND pin gnd1 through 220kΩ resistors. The SIG pin of the potentiometer is connected to the a0, the GND pin is connected to gnd1, and the VCC pin is connected to 5v.

## Expected Behavior
When the Arduino powers up, it should:
1. Set pin2, pin3, pin4, pin5, pin6, pin7, pin8, pin9, pin10, pin11 pin mode as OUTPUT
2. Set potentiometer signal pin a0 as INPUT
3. Continuously read the potentiometer value, which ranges from 0 to 1023, and map it to a range of 0 to 10
4. Turn on only the LEDs up to the mapped value, and turn off the rest.

## Success Criteria
The test is successful when:
1. Anode of the bar LED pins whose number is smaller than the mapped value receive HIGH signals, while greater pins receive LOW signals.

## Notes for Implementation
- Potentiometer value ranges from 0 to 1023.

## Variants
This project may have different variants with the LED, potentiometer connected to different pins or using LEDs of different colors.

## Attributes
- To use 220Ω resistor, set the property name "value" to "220 ohm".