# Piano Keyboard

## Project Description
This is an Arduino project that creates a piano keyboard using four push buttons and four buzzers corresponding to the notes Do, Re, Mi, and Fa.

## Hardware Components
- Arduino Uno
- Push Button * 4
- 10kΩ resistor * 4
- Piezoelectric Buzzer * 4

## Circuit Description
Four piezo buzzers and four push buttons are connected to an Arduino Uno. Button 1 and Buzzer 1 correspond to the note Do, Button 2 and Buzzer 2 to Re, Button 3 and Buzzer 3 to Mi, and Button 4 and Buzzer 4 to Fa. Buttons 1 through 4 are connected to Arduino pins 2, 3, 4, and 5, respectively. When the button is not pressed, the input pin is connected to gnd3 through a 10kΩ resistor. When the button is pressed, the input pin is connected to 5v. 
The positive pins of the buzzers are connected to Arduino pins 9, 10, 11, and 12, respectively, and their negative pins are connected to gnd3.

## Expected Behavior
When the Arduino powers up, it should:
1. Configure button pins as INPUT
2. Configure buzzer pins as OUTPUT
3. Continuously monitor the button state, and if it reads HIGH, play a tone on the corresponding buzzer
4. If no button is pressed, keep all buzzers silent

## Success Criteria
The test is successful when:
1. The buzzer plays a tone when the corresponding button is pressed.

## Notes for Implementation
- The button input will read HIGH when it is pressed.
- When the button is not pressed, pin1.l is connected to pin1.r and pin2.l is connected to pin2.r. When the button is pressed, all the pins are connected together.
- The frequencies of Do, Re, Mi, and Fa are 262 Hz, 294 Hz, 330 Hz, and 349 Hz, respectively.
- Only one button is pressed at a time.

## Attributes
- To use 10kΩ resistor, set the property name "value" to "10000 ohm".