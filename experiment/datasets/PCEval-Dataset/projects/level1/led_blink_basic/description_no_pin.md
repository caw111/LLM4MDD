# Blink LED

## Project Description
This is a simple Arduino project that makes an LED connected to a specific pin blink on and off at regular intervals.

## Hardware Components
- Arduino Uno
- LED
- 220Ω resistor
- Breadboard

## Circuit Description
The LED is connected to pin of the Arduino Uno through a 220Ω current-limiting resistor. The anode of the LED is connected to arduino digital pin via the resistor. The cathode of the LED is connected to ground.

## Expected Behavior
When the Arduino powers up, it should:
1. Initialize serial communication
2. Send a message "LED Blink test starts" to the serial monitor
3. Begin blinking the LED with the following pattern:
   - Turn ON for 1 second
   - Turn OFF for 1 second
   - Repeat indefinitely

## Success Criteria
The test is successful when:
1. The correct serial message is displayed
2. The LED turns on and off at regular 1-second intervals
3. The blinking pattern continues indefinitely

## Attributes
- To use 220Ω resistor, set the property name "value" to "220 ohm".