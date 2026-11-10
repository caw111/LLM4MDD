# One Digit 7-segment Display

## Project Description
This is an Arduino project that show number 5 with digit 7-segment display.

## Hardware Components
- Arduino Uno
- 330Ω resistor * 8
- 7-segment display

## Circuit Description
Circuit is designed to use a 7-segment display. The 7-segment display is a common anode type. The A to G and DP pins are connected to Arduino through 330-ohm resistors.

## Expected Behavior
When the Arduino powers up, it should:
1. Set segment pins as an output.
2. Taking into account that the display is common anode, set the corresponding pins to HIGH or LOW so that the number 5 appears on the 7-segment display.

## Success Criteria
The test is successful when:
1. The number 5 appears on the 7-segment display.

## Notes for Implementation
- The 7-segment display is a common anode type.

## Variants
This project may have different variants with the segment pins connected to the different pins.

## Attributes
- To use 330Ω resistor, set the property name "value" to "330 ohm".