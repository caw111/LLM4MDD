# Parking Spot Monitor

## Project Description
This is an parking spot monitor project made with Arduino that detects vehicle presence using an ultrasonic sensor and shows the status with an RGB LED.

## Hardware Components
- Arduino Uno
- 220Ω resistor * 3
- RGB LED
- Ultrasonic Distance Sensor

## Circuit Description
An ultrasonic distance sensor and a RGB led is connected to arduino uno. R, G, B pin of RGB led are connected to arduino digital pins through 220Ω resistor. RGB LED uses common cathode configuration. For ultrasonic distance sensor, TRIG and ECHO pin is connected to arduino digital pins.

## Expected Behavior
When the Arduino powers up, it should:
1. Set RGB LED pins and TRIG pin as OUTPUT, and ECHO as INPUT
2. Continuously triger and read the distance sensor value
3. Calculate the distance
4. If distance is smaller than 50, turn on the red light of RGB led
5. If distance is greater than or equal to 50, turn on the green light of RGB led

## Success Criteria
The test is successful when:
1. When the distance is smaller than 50, red light on the RGB led turned on.
2. When the distance is greater than or equal to 50, green light on the RGB led turned on.

## Notes for Implementation
- To convert the distance sensor value to centimeters, divide it by 58.

## Attributes
- To use 220Ω resistor, set the property name "value" to "220 ohm".