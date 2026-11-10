# Serial RGB LED

## Project Description
This is a simple Arduino project that control RGB LED with serial monitor.

## Hardware Components
- Arduino Uno
- RGB LED
- 220Ω resistor * 3

## Circuit Description
The R, G, B pin of the RGB LED is connected to the arduino digital pins through a 220Ω current-limiting resistor. RGB LED has common anode configuration.

## Expected Behavior
When the Arduino powers up, it should:
1. Set LED pins to OUTPUT
2. Initialize serial monitor
3. Continuously check if the serial monitor is available, and read until '\n' appears. 
4. Check if the input contains "Red", "Green", or "Blue", and fully turn on the corresponding color on the RGB LED if it does.

## Success Criteria
The test is successful when:
1. The input colors and the corresponding RGB LED pin state line up perfectly.

## Notes for Implementation
- The common pin of the RGB LED is an anode. 
- The input is given as a comma-separated list, such as "Red, Green".

## Variants
This project may have different variants with the LED conntected to different pins.

## Test Inputs
Test input values are: "Red", "Red, Green", "Green, Blue"