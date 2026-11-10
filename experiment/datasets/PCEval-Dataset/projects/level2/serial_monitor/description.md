# Serial Monitor

## Project Description
This is a simple Arduino project that read and write with serial monitor.

## Hardware Components
- Arduino Uno

## Circuit Description
There is no extra circuit.

## Expected Behavior
When the Arduino powers up, it should:
1. Initialize serial communication
2. Continuously read the serial monitor, and print back the value in the format: "Received: {value}".

## Success Criteria
The test is successful when:
1. The displayed serial message is same with the written value.

## Notes for Implementation
- In the loop function, read the serial monitor if it is available, and print it back to the serial monitor.

## Variants
This project dosen't have any variants.