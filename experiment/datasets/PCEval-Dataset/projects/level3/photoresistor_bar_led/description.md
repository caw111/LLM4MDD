# Photoresitor Bar LED

## Project Description
This is an Arduino project that lights up the bar LED according to the brightness level.

## Hardware Components
- Arduino Uno
- Bar graph LED
- 220Ω resistor * 10
- Photoresistor

## Circuit Description
Anode pins 1~10 of bar graph LED are connected to pin2, pin3, pin4, pin5, pin6, pin7, pin8, pin9, pin10, pin11, respectively. The cathode pins 1 through 10 of the bar graph LED are connected to GND pin gnd2 through 220kΩ resistors. The AO pin of the photoresistor is connected to the arduino a0 pin, the GND pin is connected to gnd2, and the VCC pin is connected to 5v.

## Expected Behavior
When the Arduino powers up, it should:
1. Set pin2, pin3, pin4, pin5, pin6, pin7, pin8, pin9, pin10, pin11 pin mode as OUTPUT
2. Set photoresistor signal pin A0 as INPUT
3. Continuously read the photoresistor value, which ranges from 1023 to 0, and map it to a range of 0 to 10
4. Turn on only the LEDs up to the mapped value, and turn off the rest.

## Success Criteria
The test is successful when:
1. Anode of the bar LED pins whose number is smaller than the mapped value receive HIGH signals, while greater pins receive LOW signals.

## Notes for Implementation
- Photoresistor's analog value ranges from 1023 to 0. The smaller the analog value, the brighter the light.

## Attributes
- To use 220Ω resistor, set the property name "value" to "220 ohm".