# Temperature Sensor Basic

## Project Description
This is a simple Arduino project that uses dht22 sensor to measure the temperature.

## Hardware Components
- Arduino Uno
- DHT22 sensor

## Circuit Description
The DHT22 sensor is connected to an Arduino Uno. The data pin is connected to digital pin on the Arduino.

## Expected Behavior
When the Arduino powers up, it should:
1. Initialize serial monitor
2. Initialize and begin DHT sensor
3. Continuously read the DHT sensor's temperature value
4. Print temperature to the serial monitor in the following format: "Current temperature: {temperature value}", for example: "Current temperature: 40"

## Success Criteria
The test is successful when:
1. Correct temperature value in celsius is printed in the serial monitor.

## Notes for Implementation