# Exercise Counter

## Project Description
This is a motion counter that detects movement with an MPU6050 gyroscope and displays the count on a 1-digit 7-segment display, made with arduino uno.

## Hardware Components
- Arduino Uno
- 7-segment display
- 220Ω resistor * 7
- MPU6050 6-axis Accel & Gyro Sensor

## Circuit Description
A 7-segment display and a mpu6050 sensor is connected to arduino uno. Pin A to G is connected to arduino digtial pins through 220Ω resistor, respectively. 7-segment display has common cathode configuration. MPU6050 sensor follows the standard wiring conventions defined by the I2C protocol.

## Expected Behavior
When the Arduino powers up, it should:
1. Set pins for 7-segments as OUTPUT
2. Check the connection of mpu6050
3. Display number 0 on the 7-segment display
4. Continuously read the Y rotation value of mpu6050
5. The count increases when the Y-axis rotation value exceeds 15000, and it does not increase again until the absolute value drops below 2000
6. Continuously display the count on the 7-segment display

## Success Criteria
The test is successful when:
1. The count increases only when the absolute Y-axis rotation value rises above 15000 after previously dropping below 2000.
2. The current count is correctly displayed on the 7-segment display.

## Notes for Implementation
- Use MPU6050.h, Wire.h libraries.
- MPU6050 has I2C interface.

## Attributes
- To use 220Ω resistor, set the property name "value" to "220 ohm".