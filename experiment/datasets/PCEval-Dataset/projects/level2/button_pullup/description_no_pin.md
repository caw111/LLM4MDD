# Button Pullup

## Project Description
This is a simple Arduino project that prints whether the button is pressed.

## Hardware Components
- Arduino Uno
- 10kΩ resistor
- Push Button

## Circuit Description
Circuit is designed to use a pull-up resistor. The button is connected to 5V through a 10kΩ resistor. Also, the button is connected to ground.

## Expected Behavior
When the Arduino powers up, it should:
1. Initialize serial communication
2. Configure the specified button pin as INPUT
3. Continuously check the button value, and print "Button pressed" if the value is LOW, or print "Button not pressed" if the value is HIGH.

## Success Criteria
The test is successful when:
1. "Button pressed" is printed when the button is pressed
2. "Button not pressed" is printed when the button is not pressed

## Notes for Implementation
- The pin number used should match the circuit diagram
- When the button is not pressed, pin1.l is connected to pin1.r and pin2.l is connected to pin2.r. When the button is pressed, all the pins are connected together.

## Variants
This project may have different variants with the pushbutton connected to different pins.

## Attributes
- To use 10kΩ resistor, set the property name "value" to "10000 ohm".