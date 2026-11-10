# Button Servo Motor

## Project Description
This is a simple Arduino project that controls servo motor with a pushbutton.

## Hardware Components
- Arduino Uno
- Servo Motor
- Push Button
- 10kΩ resistor

## Circuit Description
Button is connected to Arduino with 10kΩ pull up resistor. PWM pin of the servo motor is connected to arduino digital pin.

## Expected Behavior
When the Arduino powers up, it should:
1. Initialize serial monitor as baud rate 9600
2. Attaches the servo to the servo object
3. Set button pin mode as INPUT
4. Continuously print the angle of the servo motor to serial monitor
5. Continuously read the button value, and set the servo motor to 180 degrees when the button is pressed, and to 90 degrees when it is not

## Success Criteria
The test is successful when:
1. The pressed button and the corresponding servo motor angle line up perfectly.

## Notes for Implementation
- Use the Servo library to create and control a servo object.
- When the button is not pressed, pin1.l is connected to pin1.r and pin2.l is connected to pin2.r. When the button is pressed, all the pins are connected together.

## Variants
This project may have different variants with the servo motor, button, GND connected to different pins, and the angle of the servo motor.

## Attributes
- To use 10kΩ resistor, set the property name "value" to "10000 ohm".