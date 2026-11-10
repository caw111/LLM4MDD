# Clock

## Project Description
This is a Arduino project that show current time using 4-digit 7-segment display.

## Hardware Components
- Arduino Uno
- 4-digit 7-segment display
- RTC module

## Circuit Description
Circuit is designed to use a 4-digit 7-segment display and a RTC module.
RTC module follows the standard wiring conventions defined by the I2C protocol.
In the 4-digit 7-segment display, digit pins dig1 to dig4 and segment pins A ~ G are connected to arduino digital pins. Colon pin is also connected to arduino digital pin. 4-digit 7-segment display has common anode configuration.

## Expected Behavior
When the Arduino powers up, it should:
1. Check if the RTC module is available and initialize it
2. Set segment pins and digit pins mode as OUTPUT
3. Turn on the colon led
4. Continuously retrieve the current time from the RTC
5. Display current time to 4-digit 7-segment display, in the following format: "HH:MM"

## Success Criteria
The test is successful when:
1. The correct time is shown to the 4-digit 7-segment display

## Notes for Implementation
- The 7-segment display is common anode type, so the segment turns on when the pin reads LOW.
- Digits are activated when digit pins are HIGH. 
- When using a 4-digit 7-segment display, be sure to specify '4-digit' at the beginning of the component type.