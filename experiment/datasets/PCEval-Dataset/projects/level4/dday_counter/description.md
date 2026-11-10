# Serial RTC sensor

## Project Description
This is a simple Arduino project that compare RTC sensor time and serial input.

## Hardware Components
- Arduino Uno
- ds1307 Real Time Clock module

## Circuit Description
The RTC sensor is connected to the Arduino. The VCC pin is connected to 5V, the GND pin to gnd2, the SDA pin to A4, and the SCL pin to A5.

## Expected Behavior
When the Arduino powers up, it should:
1. Initialize serial communication
2. Check if the RTC module can begin
3. Read current time once with RTC module
4. Continuously read the serial input, which is a target datetime in ISO format
5. Calculate how much day is left until the target time, and print to the serial monitor in the following format: "D-{days}"

## Success Criteria
The test is successful when:
1. Correct day left to the target datetime is printed in the serial monitor.

## Notes for Implementation
- Get current time with RTC sensor only once, in the setup
- Use RTClib to generate RTC_DS1307 object and read the value.
- RTC module has I2C interface.

## Variants
There may be variants with GND pin.

## Test Inputs
Test input values are: "2025-04-11T09:38:00", "2025-06-11T09:38:00", "2025-03-03T09:38:00"