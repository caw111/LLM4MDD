# LCD Display with DHT22 sensor

## Project Description
This is an Arduino project that shows dht22 sensor value to the ili9341 lcd display.

## Hardware Components
- Arduino Uno
- Ili9341 LCD Display
- DHT22 sensor

## Circuit Description
Ili9341 LCD Display and dht22 sensor are connected to the Arudino Uno board. RST, D/C, CS, MOSI, MISO, SCK pin of LCD display is connected to arduino pin 8 to 13, respectively. Each GND pins of LCD display and DHT sensor are connected to arduino pin gnd1. Each VCC pins of LCD display and DHT sensor are connected to arduino pin 5v. SDA pin of DHT sensor is connected to arduino pin2.

## Expected Behavior
When the Arduino powers up, it should:
1. Declare Adafruit_ILI9341 object
2. Begin display and change it to landscape mode (rotation 1)
3. Set the text color to white, and size to 2
4. Initialize and begin the dht22 sensor
5. Continuously read the humidity and temperature value of dht22 sensor
6. If the sensor value is changed, clear the display with a black background and print the humidity at position (0, 0), in the following format: "Humidity: {humidity}", and print the temperature at position (0, 100) in the following format: "Temperature: {temperature}"

## Success Criteria
The test is successful when:
1. Sensor value is successfully printed to the display, with correct position, color, and size.

## Notes for Implementation
- Use the following libraries: <SPI.h>, <Adafruit_GFX.h>, and <Adafruit_ILI9341.h>.
- Wait for 100 milliseconds at the end of every loop iteration.
