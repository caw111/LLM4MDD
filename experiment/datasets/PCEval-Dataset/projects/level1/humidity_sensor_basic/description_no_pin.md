# Humidity Sensor Basic

## Project Description
This is a simple Arduino project that uses dht22 sensor to measure the humidity.

## Hardware Components
- Arduino Uno
- DHT22 sensor

## Circuit Description
The DHT22 sensor is connected to an Arduino Uno. The data pin is connected to digital pin of the Arduino.

## Expected Behavior
When the Arduino powers up, it should:
1. Initialize serial monitor
2. Initialize and begin DHT sensor
3. Continuously read the DHT sensor's humidity value
4. Print humidity to the serial monitor in the following format: "Humidity: {humidity value}", for example: "Humidity: 40.0"

## Success Criteria
The test is successful when:
1. The correct humidity value is printed to the serial monitor.

## Notes for Implementation

