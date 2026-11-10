# Button Controlled LED

## Project Description
This is a Arduino project that controls an LED with a pushbutton. The LED will turn on only while the button is pressed and turn off when released.

## Hardware Components
- Arduino Uno
- Push Button
- 220Ω resistor
- LED

## Circuit Description
A push button is connected to arduino digital pin. A LED is connected to a digital pin through a 220Ω current-limiting resistor.

## Expected Behavior
When the Arduino powers up, it should:
1. Configure the button pin as INPUT
2. Configure the LED pin as OUTPUT
3. Continuously monitor the button state
4. Turn the LED ON when the button is pressed (reads LOW)
5. Turn the LED OFF when the button is released (reads HIGH)

## Success Criteria
The test is successful when:
1. The LED turns on immediately when the button is pressed
2. The LED remains on only while the button is held down
3. The LED turns off immediately when the button is released

## Notes for Implementation
- The button input will read LOW when pressed if using a pull-up resistor configuration
- Ensure proper current-limiting resistor for the LED based on its specifications
- When the button is not pressed, pin1.l is connected to pin1.r and pin2.l is connected to pin2.r. When the button is pressed, all the pins are connected together.

## Attributes
- To use 220Ω resistor, set the property name "value" to "220 ohm".