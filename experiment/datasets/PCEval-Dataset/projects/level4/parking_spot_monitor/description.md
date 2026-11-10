# Parking Spot Monitor

## Project Description
This is an parking spot monitor project made with Arduino that detects vehicle presence using an ultrasonic sensor and shows the status with an RGB LED.

## Hardware Components
- Arduino Uno
- 220Ω resistor * 3
- RGB LED
- Ultrasonic Distance Sensor

## Circuit Description
An ultrasonic distance sensor and a RGB led is connected to arduino uno. R, G, B pin of RGB led are connected to arduino pin4, pin3, pin2, through 220Ω resistor, respectively. COM pin of RGB led is connected to arduino gnd2. For ultrasonic distance sensor, GND and VCC pin are connected to arduino gnd2 and 5v, respectively. TRIG and ECHO pin is connected to pin8 and pin9.

## Expected Behavior
When the Arduino powers up, it should:
1. Set pin8, pin2 ~ pin4 as OUTPUT, and pin9 as INPUT
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