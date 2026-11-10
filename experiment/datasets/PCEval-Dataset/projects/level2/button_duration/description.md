# Button Duration

## Project Description
This is an Arduino project that determines how long a switch is pressed

## Hardware Components
- Arduino Uno
- 10kΩ resistor
- Push Button

## Circuit Description
Circuit is designed to use a pull-up resistor. When the button is not pressed, the pin4 pin is connected to 5V through a 10kΩ resistor. When the button is pressed, the pin4 pin is connected to gnd2.

## Expected Behavior
When the Arduino powers up, it should:
1. Initialize serial communication
2. Configure the specified button pin as INPUT
3. Continuously monitor the button state and measure how long it is pressed
4. Print the duration in second, in the following format: "Button pressed duration: {duration}s"

## Success Criteria
The test is successful when:
1. The pressed duration in seconds is correctly printed in the given format

## Notes for Implementation
- Include a short delay in the loop to handle switch bouncing
- When the button is not pressed, pin1.l is connected to pin1.r and pin2.l is connected to pin2.r. When the button is pressed, all the pins are connected together.

## Variants
This project may have different variants with the pushbutton connected to different pins.

## Attributes
- To use 10kΩ resistor, set the property name "value" to "10000 ohm".