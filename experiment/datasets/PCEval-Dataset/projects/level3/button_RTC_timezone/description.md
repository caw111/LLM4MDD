# Button RTC timezone

## Project Description
This is a Arduino project that prints UTC and UTC+9 timestamp with a RTC module.

## Hardware Components
- Arduino Uno
- Push Button
- 10kΩ resistor
- RTC module

## Circuit Description
The RTC module is connected to the Arduino. The VCC pin is connected to 5V, the GND pin to gnd3, the SDA pin to A4, and the SCL pin to A5. Push button is connected to pin2 for input, and reads high when it is pressed.

## Expected Behavior
When the Arduino powers up, it should:
1. Configure the button pin as an input
2. Initialize the serial monitor
3. Check if the RTC module is available and initialize it
4. Continuously retrieve the current time from the RTC
5. Check the button state, and if it is pressed, add 9 hours to adjust the timezone
6. Print the resulting timestamp

## Success Criteria
The test is successful when:
1. The UTC timestamp is printed when the button is not pressed
2. The UTC+9 timestamp is printed when the button is pressed

## Notes for Implementation
- The button input will read HIGH when pressed.
- When the button is not pressed, pin1.l is connected to pin1.r and pin2.l is connected to pin2.r. When the button is pressed, all the pins are connected together.
- The format of timestamp is YYYY-MM-DDTHH:MM:SS.

## Attributes
- To use 10kΩ resistor, set the property name "value" to "10000 ohm".