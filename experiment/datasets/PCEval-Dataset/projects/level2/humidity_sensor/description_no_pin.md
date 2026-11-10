# Humidity Sensor

## Project Description
This is a simple Arduino project that reads humidity sensor value and check if it is above the threshold.

## Hardware Components
- Arduino Uno
- DHT22 sensor

## Circuit Description
The DHT22 sensor is connected to an Arduino Uno. The data pin is connected to digital pin on the Arduino.

## Expected Behavior
When the Arduino powers up, it should:
1. Declare the threshold value, which is 50.0
2. Initialize serial communication
3. Continuously read the DHT sensor's humidity value
4. Compare the humidity sensor value with the threshold, and print one of these: 'Humidity is below the threshold', 'Humidity is above the threshold'

## Success Criteria
The test is successful when:
1. The correct compare result is printed to the serial monitor.

## Notes for Implementation
- Serial input is always a float value.
- Threshold is 50.0.
