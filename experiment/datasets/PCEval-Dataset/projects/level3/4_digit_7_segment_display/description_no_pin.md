# 4-digit 7-segment Display

## Project Description
This is an Arduino project that show numbers with 4-digit 7-segment display.

## Hardware Components
- Arduino Uno
- 4-digit 7-segment display

## Circuit Description
Circuit is designed to use a 4-digit 7-segment display. The 4-digit 7-segment display is a common anode type. The A to G and DP, and DIG1 to DIG4 pins are connected to arduino digital pins.

## Expected Behavior
When the Arduino powers up, it should:
1. Set segment pins and digit pins as an output.
2. Taking into account that the display is common anode, display the number 1 on digit1, 2 on digit2, 3 on digit3, and 4 on digit4, in order, with an 1000ms delay between each step.

## Success Criteria
The test is successful when:
1. The 4-digit 7-segment display shows 1, 2, 3, and 4 in order.

## Notes for Implementation
- The 4-digit 7-segment display is a common anode type.

## Variants
This project may have different variants with the segment pins connected to the different pins.