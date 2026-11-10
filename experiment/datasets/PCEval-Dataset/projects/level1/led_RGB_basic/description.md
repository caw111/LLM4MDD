# LED RGB

## Project Description
This is a simple Arduino project that turns on an RGB LED.

## Hardware Components
- Arduino Uno
- RGB LED
- 220Ω resistor * 3

## Circuit Description
The R, G, B pin of the RGB LED is connected to the pin4, pin3, pin2 pin of the Arduino Uno through a 220Ω current-limiting resistor. The common pin of the RGB LED is connected to 5V.

## Expected Behavior
When the Arduino powers up, it should:
1. Set pin4, pin3, pin2 pin mode to OUTPUT
2. Considering that the LED's common pin is connected to 5V, turn on only the blue segment at full brightness.

## Success Criteria
The test is successful when:
1. Only the blue light of the RGB LED is fully on.

## Notes for Implementation
- The LED should turn on immediately during setup and stay on
- No actions are required in the loop function
- The pin number used should match the circuit diagram

## Variants
This project may have different variants with the RGB LED connected to different pins. 

## Attributes
- To use 220Ω resistor, set the property name "value" to "220 ohm".