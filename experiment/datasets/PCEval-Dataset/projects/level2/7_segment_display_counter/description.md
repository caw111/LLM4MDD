# One Digit 7-segment Display Counter

## Project Description
This is an Arduino project that show a counter with digit 7-segment display.

## Hardware Components
- Arduino Uno
- 330Ω resistor * 8
- 7-segment display

## Circuit Description
Circuit is designed to use a 7-segment display. The 7-segment display is a common anode type, so the common pin is connected to 5V. The A to G and DP pins are connected to Arduino digital pins 2 to 9 through 330-ohm resistors.

## Expected Behavior
When the Arduino powers up, it should:
1. Set 2~9 pin as an output.
2. Taking into account that the display is common anode, set the corresponding pins to HIGH or LOW so that the elapsed time in seconds is shown on the 7-segment display.

## Success Criteria
The test is successful when:
1. The numbers 1, 2, and 3 appear according to the elapsed seconds.

## Notes for Implementation
- The 7-segment display is a common anode type.
- Display 1 between 1–2 seconds, 2 between 2–3 seconds, and 3 between 3–4 seconds.

## Variants
This project may have different variants with the segment pins connected to the different pins.