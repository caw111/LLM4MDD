# Alarm

## Project Description
This is an Arduino project that triggers the buzzer when the specified time is reached.

## Hardware Components
- Arduino Uno
- Piezoelectric Buzzer
- RTC module

## Circuit Description
The RTC module and piezo buzzer is connected to the Arduino. RTC module follows the standard wiring conventions defined by the I2C protocol. For piezo buzzer, the positive pin is connected to the arduino digital pin.

## Expected Behavior
When the Arduino powers up, it should:
1. Set the buzzer pin mode as OUTPUT
2. Initialize the RTC module, and check if it can begin
3. Continuously read the current time with RTC module
4. Tone the buzzer if the current time is on or after '2024-11-24T04:49:00'

## Success Criteria
The test is successful when:
1. Buzzer plays tone only when the current time is on or after the specified time.

## Notes for Implementation
- Specified time is "2024-11-24T04:49:00".
- Use RTClib to generate RTC_DS1307 object and read the value.
- RTC module has I2C interface.
