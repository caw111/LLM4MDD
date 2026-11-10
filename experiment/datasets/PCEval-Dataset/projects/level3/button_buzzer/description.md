# Button Buzzer

## Project Description
This is a simple Arduino project that uses pushbutton to control piezoelectric buzzer.

## Hardware Components
- Arduino Uno
- Piezoelectric buzzer
- Push Button
- 10kΩ resistor

## Circuit Description
Piezo buzzer and button are connected to the Arduino. The positive pin of buzzer is connected to pin9, the negative pin to gnd3. The circuit is designed so that the input reads LOW when the push button is pressed and HIGH when it is not pressed. A 10kΩ resistor is connected on the side connected to 5V.

## Expected Behavior
When the Arduino powers up, it should:
1. Set button pin to INPUT, and buzzer pin to OUTPUT
2. Continuously read the button value
3. If the button is pressed, play 262Hz tone

## Success Criteria
The test is successful when:
1. Piezo buzzer runs when the button is pressed.
2. The piezo buzzer doesn’t sound when the button is not pressed.

## Notes for Implementation
- The button reads LOW when pressed and HIGH when not pressed.
- When the button is not pressed, pin1.l is connected to pin1.r and pin2.l is connected to pin2.r. When the button is pressed, all the pins are connected together.

## Variants
There may be variants with buzzer pin number, button pin number, and GND pin number.

## Attributes
- To use 10kΩ resistor, set the property name "value" to "10000 ohm".