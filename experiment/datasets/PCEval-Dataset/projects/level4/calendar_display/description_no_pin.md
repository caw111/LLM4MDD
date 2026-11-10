# Calendar Display

## Project Description
This is a simple Arduino project that uses an RTC module to display the current date and day of the week on ili9341 LCD, and navigate days with buttons.

## Hardware Components
- Arduino Uno
- RTC module
- Ili9341 LCD Display
- Push Button * 2
- 10kΩ resistor * 2

## Circuit Description
RTC module, ili9341 LCD display, two push buttons are connected to arduino uno. For LCD display, RST, D/C, CS, MOSI, MISO, SCK pin of LCD display is connected to arduino digital pins. RTC module follows the standard wiring conventions defined by the I2C protocol. Button 1 and button 2 are connected to arduino digital pins. When the button is not pressed, it is connected to ground by 10kΩ resistor. Buttons use a pull-down resistor configuration.

## Expected Behavior
When the Arduino powers up, it should:
1. Initialize the RTC module and check if it can begin
2. Set text size as 3, and text color to white.
3. Continuously read the current time, and show YYYY-MM-DD to (10, 30) position.
4. Also show days of the week to (10, 80) position.

## Success Criteria
The test is successful when:
1. The correct date is displayed on the screen.
2. When a button is pressed, the displayed date changes correctly.

## Notes for Implementation
- Day of the week is one of the "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday".
- Button 1 corresponds to the previous date, and Button 2 to the next date.
- Use <Adafruit_GFX>, <Adafruit_ILI9341.h>, "RTClib.h" libraries.

## Attributes
- To use 10kΩ resistor, set the property name "value" to "10000 ohm".