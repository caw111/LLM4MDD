# Gyroscope

## Project Description
This is an Arduino project that uses MPU6050 6-axis accelerometer & gyro sensor to measure the rotation.

## Hardware Components
- Arduino Uno
- MPU6050 6-axis Accel & Gyro Sensor

## Circuit Description
The MPU6050 sensor is connected to the Arduino. The VCC pin is connected to 3.3V, the GND pin to gnd3, the SCL pin to A5, and the SDA pin to A4.

## Expected Behavior
When the Arduino powers up, it should:
1. Initialize serial communication
2. Check if the MPU sensor can be initialized successfully, and print 'MPU6050 ready!' to the serial monitor
3. Continuously read the gryoscope data from the MPU6050 sensor
4. Print the X, Y, and Z axis rotation values in rad/s to the serial monitor, for example: 'X: 1.13, Y: 2.97, Z: -2.01 rad/s'

## Success Criteria
The test is successful when:
1. Correct X, Y, Z gyroscope result is printed to the serial monitor.

## Notes for Implementation
- Use Adafruit_MPU6050.h, Adafruit_Sensor.h, Wire.h libraries.
- Delay 500ms in every loop.
- MPU6050 has I2C interface.

## Variants
There may be variants with GND connected to different pin.