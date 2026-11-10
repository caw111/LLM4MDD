# Traffic Light

## Project Description
This project uses three LEDs to create a traffic light. When a pedestrian button is pressed, the traffic light changes to yellow and then red.

## Hardware Components
- Arduino Uno
- Push Button
- 10kΩ resistor
- 220Ω resistor * 3
- LED * 3

## Circuit Description
LED1 is red and connected to Arduino pin8 through a 220-ohm resistor. LED2 is yellow and connected to pin9 through a 220-ohm resistor. LED3 is green and connected to pin10 through a 220-ohm resistor. The cathode of each LED is connected to gnd2.
The input from the push button goes into Arduino pin2. When the button is not pressed, it is connected to gnd2 through a 10k ohm resistor; when pressed, it is connected to the 5V pin.

## Expected Behavior
When the Arduino powers up, it should:
1. Configure the button pin as INPUT.
2. Configure three LED pins as OUTPUT.
3. Continuously monitor the button state.
4. If it is not pressed, turn the green LED3 on.
5. If it is pressed, turn the green LED3 off, and turn the yellow LED2 on for 1000ms, and then turn the red LED1 on for 2000ms. Lastly, turn the green LED3 on again.

## Success Criteria
The test is successful when:
1. The green LED turns on when the button is not pressed.
2. The yellow LED and the red LED turns on sequentially when the button is pressed.

## Notes for Implementation
- The button input will read HIGH when pressed.
- When the button is not pressed, pin1.l is connected to pin1.r and pin2.l is connected to pin2.r. When the button is pressed, all the pins are connected together.
- Set the LED ID corresponding to the led name, e.g., "led1" for LED1.

## Attributes
- To use 220Ω resistor, set the property name "value" to "220 ohm".
- To specify color of the led, set the property name "color" to "red", "green" or "yellow".