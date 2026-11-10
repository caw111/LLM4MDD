# Binary LED

## Project Description
This is an arduino project that uses 5 LEDs to display the binary form of a number received over serial.

## Hardware Components
- Arduino Uno
- 220Ω resistor * 5
- LED * 5

## Circuit Description
Five LEDs are connected to Arduino Uno. LED 1 to 5 are connected to arduino pin2 to pin6 through 220Ω resistor, respectively. Cathode pin of each LEDs are connected to gnd2.

## Expected Behavior
When the Arduino powers up, it should:
1. Set pin2, pin3, pin4, pin5, pin6 pin mode as OUTPUT
3. Continuously read the serial monitor, which ranges from 0 to 31
4. With LED1 as the LSB and LED5 as the MSB, light up the LEDs that represent 1s in the binary form of the input number

## Success Criteria
The test is successful when:
1. When a number is entered via serial input, LEDs corresponding to the bits set to 1 in its binary representation are turned on.

## Notes for Implementation
- Serial input ranges from 0 to 31.

## Attributes
- To use 220Ω resistor, set the property name "value" to "220 ohm".