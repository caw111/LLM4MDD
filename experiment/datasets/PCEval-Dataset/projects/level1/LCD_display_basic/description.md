# LCD Display Basic

## Project Description
This is a simple Arduino project that shows text to the ili9341 lcd display.

## Hardware Components
- Arduino Uno
- Ili9341 LCD Display

## Circuit Description
Ili9341 LCD Display is connected to the Arudino Uno board. D/C, CS, MOSI, MISO, SCK pin of LCD display is connected to arduino pin 9 to 13, respectively. GND pin of LCD display is connected to arduino pin gnd1. VCC pin of LCD display is connected to arduino pin 5v.

## Expected Behavior
When the Arduino powers up, it should:
1. Declare Adafruit_ILI9341 object
2. Begin display and change it to landscape mode
3. Fill the display with black color
4. Display 'Hello World!' at position (0, 0) in white color with text size 2.

## Success Criteria
The test is successful when:
1. Text "Hello World!" is successfully printed to the display, with correct position, color, and size.

## Notes for Implementation
- Use the following libraries: <SPI.h>, <Adafruit_GFX.h>, and <Adafruit_ILI9341.h>.
