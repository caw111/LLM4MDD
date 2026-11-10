# Button RTC multiple timezone

## Project Description
This is a Arduino project that different timestamps with a RTC module.

## Hardware Components
- Arduino Uno
- Push Button * 4
- 10kΩ resistor * 4
- RTC module

## Circuit Description
The RTC module is connected to the Arduino, following the standard wiring conventions defined by the I2C protocol. Button 1 to button 4 are connected to arduino digital pins through 10kΩ resistor. Buttons use a pull-down resistor configuration.

## Expected Behavior
When the Arduino powers up, it should:
1. Configure the button pin as an input
2. Initialize the serial monitor
3. Check if the RTC module is available and initialize it
4. Continuously retrieve the current time from the RTC
5. Check the state of buttons, and apply the corresponding timezone: Button 1 sets UTC+9, button 2 sets UTC+4, button 3 sets UTC-10, and button 4 sets UTC-5
6. Print the resulting timestamp

## Success Criteria
The test is successful when:
1. The UTC timestamp is printed when the button is not pressed
2. The UTC+9 timestamp is printed when the button 1 is pressed
3. The UTC+4 timestamp is printed when the button 2 is pressed
4. The UTC-10 timestamp is printed when the button 3 is pressed
5. The UTC-5 timestamp is printed when the button 4 is pressed

## Notes for Implementation
- The button input will read HIGH when pressed.
- When the button is not pressed, pin1.l is connected to pin1.r and pin2.l is connected to pin2.r. When the button is pressed, all the pins are connected together.
- Set the button ID corresponding to the button name, e.g., "push button1" for Button 1.
- The format of timestamp is YYYY-MM-DDTHH:MM:SS.

## Attributes
- To use 10kΩ resistor, set the property name "value" to "10000 ohm".