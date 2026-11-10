# LCD Display with Button

## Project Description
This is an Arduino project that shows button state to the ili9341 lcd display.

## Hardware Components
- Arduino Uno
- Ili9341 LCD Display
- Push Button
- 10kΩ resistor

## Circuit Description
Ili9341 LCD Display and push button are connected to the Arudino Uno board. RST, D/C, CS, MOSI, MISO, SCK pin of LCD display is connected to arduino pin 8 to 13, respectively. GND pin of LCD display is connected to arduino pin gnd1. VCC pin of LCD display is connected to arduino pin 5v. The push button input is connected to pin 2. It is configured in a cathode-style circuit so that the input reads HIGH when pressed and LOW when released.

## Expected Behavior
When the Arduino powers up, it should:
1. Declare Adafruit_ILI9341 object
2. Begin display and change it to landscape mode, and set the text color to white, and size to 2
3. Set button pin as INPUT, and countinuosly read the button state
4. If the button is newly pressed, clear the display with a black background and print the test "Pressed! at position (0, 0)
5. ELse, clear the display with a black background and print the test "Not pressed! at position (0, 0)

## Success Criteria
The test is successful when:
1. Button state is successfully printed to the display, with correct position, color, and size.

## Notes for Implementation
- Use the following libraries: <SPI.h>, <Adafruit_GFX.h>, and <Adafruit_ILI9341.h>.
- When the button is not pressed, pin1.l is connected to pin1.r and pin2.l is connected to pin2.r. When the button is pressed, all the pins are connected together.

## Attributes
- To use 10kΩ resistor, set the property name "value" to "10000 ohm".