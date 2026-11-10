# Button RGB LED

## Project Description
This is a simple Arduino project that turns on an LED connected to a specific pin.

## Hardware Components
- Arduino Uno
- RGB LED
- 220Ω resistor * 3
- Push Button * 3
- 10kΩ resistor * 3

## Circuit Description
The R, G, B pin of the RGB LED is connected to the arduino digital pins through a 220Ω current-limiting resistor.
RGB LED has common cathode configuration. Push button uses pull-up resistor configuration. The three buttons correspond to R, G, and B respectively, and are connected to arduino pin4, pin3, and pin2 for input.

## Expected Behavior
When the Arduino powers up, it should:
1. Set LED pins to OUTPUT, and button pins to INPUT
2. Continuously read the button value
3. When a button is pressed, the corresponding color on the RGB LED lights up at full brightness.

## Success Criteria
The test is successful when:
1. The pressed button and the corresponding RGB LED pin state line up perfectly.

## Notes for Implementation
- Each button is configured with a pull-up resistor, resulting in a LOW signal when pressed.
- The RGB LED has a common cathode connected to GND, so a PWM value of 0 turns it off and 255 turns it fully on.
- When the button is not pressed, pin1.l is connected to pin1.r and pin2.l is connected to pin2.r. When the button is pressed, all the pins are connected together.

## Variants
This project may have different variants with the LED, button connected to different pins.

## Attributes
- To use 220Ω resistor, set the property name "value" to "220 ohm".
- To use 10kΩ resistor, set the property name "value" to "10000 ohm".