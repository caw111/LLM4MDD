# Temperature Sensor

## Project Description
This is a simple Arduino project that reads temperature sensor value and check if it is above the threshold.

## Hardware Components
- Arduino Uno
- DHT22 sensor

## Circuit Description
The DHT22 sensor is connected to an Arduino Uno. The VCC pin is connected to the 5V pin on the Arduino, and the GND pin is connected to the GND2 pin. The data pin is connected to digital pin 2 on the Arduino.

## Expected Behavior
When the Arduino powers up, it should:
1. Declare the threshold value, which is 30.0
2. Initialize serial communication
3. Continuously read the DHT sensor's temperature value
4. Compare the temperature sensor value with the threshold, and print one of these: 'Temperature is below the threshold', 'Temperature is above the threshold'

## Success Criteria
The test is successful when:
1. The correct compare result is printed to the serial monitor.

## Notes for Implementation
- Serial input is always a float value.
- Threshold is 30.0.
