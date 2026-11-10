# Clock

## Project Description
This is a Arduino project that show current time using 4-digit 7-segment display.

## Hardware Components
- Arduino Uno
- 4-digit 7-segment display
- RTC module

## Circuit Description
Circuit is designed to use a 4-digit 7-segment display and a RTC module. 
For RTC module, the VCC pin is connected to arduino 5V, the GND pin to gnd3, the SDA pin to A4, and the SCL pin to A5.
In the 4-digit 7-segment display, digit pins dig1 ~ dig4 are connected to arduino pin2 ~ pin5, respectively. Segment pins A ~ G are connected to arduino pin6 ~ pin12. Colon pin is connected to pin13. The common pin of display is connected to 5v.

## Expected Behavior
When the Arduino powers up, it should:
1. Check if the RTC module is available and initialize it
2. Set pin 2 ~ 13 mode as OUTPUT
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