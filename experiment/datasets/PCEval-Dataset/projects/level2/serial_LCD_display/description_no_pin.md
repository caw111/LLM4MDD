# LCD Display with Serial Monitor

## Project Description
This is an Arduino project that shows serial input to the ili9341 lcd display.

## Hardware Components
- Arduino Uno
- Ili9341 LCD Display

## Circuit Description
Ili9341 LCD Display is connected to the Arudino Uno board. D/C, CS, MOSI, MISO, SCK pin of LCD display is connected to arduino digital pins.

## Expected Behavior
When the Arduino powers up, it should:
1. Declare Adafruit_ILI9341 object
2. Begin display and change it to landscape mode (rotation 1)
3. Initialize the serial monitor
4. Continuously read the serial monitor
5. If there is input from the Serial Monitor, clear the display with a black background and print the input string at position (0, 0) in white with text size 2
6. Also print the input string to the serial monitor

## Success Criteria
The test is successful when:
1. Input string is successfully printed to the display, with correct position, color, and size.
2. Input string is successfully printed to the serial monitor.

## Notes for Implementation
- Use the following libraries: <SPI.h>, <Adafruit_GFX.h>, and <Adafruit_ILI9341.h>.
