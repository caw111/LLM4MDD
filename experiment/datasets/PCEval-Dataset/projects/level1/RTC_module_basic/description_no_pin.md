# RTC module

## Project Description
This is a simple Arduino project that uses RTC module to get current time.

## Hardware Components
- Arduino Uno
- RTC module

## Circuit Description
The RTC module is connected to the Arduino. It follows the standard wiring conventions defined by the I2C protocol.

## Expected Behavior
When the Arduino powers up, it should:
1. Initialize serial communication
2. Check if the RTC module can begin
3. Read current time with RTC module
4. Print to the serial monitor in the following format: "Current time: YYYY/M/D (Day of the week) H:M:S"

## Success Criteria
The test is successful when:
1. Correct day and time is printed in the serial monitor.

## Notes for Implementation
- No actions are required in the loop function.
- Day of the week is one of the "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday".
- Use RTClib to generate RTC_DS1307 object and read the value.
- RTC module has I2C interface.

## Variants
There may be variants with GND pin.

## Test Inputs
Test init value is: "2024-11-24T04:48:00"