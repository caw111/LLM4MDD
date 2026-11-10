# Multiplexer with Photoresistor

## Project Description
This is an arduino project that reads 6 photoresistor sensor value with 16 channel analog multiplexer.

## Hardware Components
- Arduino Uno
- Photoresistor * 6
- Analog Multiplexer

## Circuit Description
Six photoresistors are connected to an Arduino Uno through an analog multiplexer.
The AO pins of photoresistors 1 through 6 are connected to channels I0 to I5 of the multiplexer, respectively.
The VCC pins of the photoresistors are connected to the Arduino's 5V pin, and the GND pins are connected to gnd2.
The SIG pin of the multiplexer is connected to the Arduino's A0 pin and is used to read values.
The S0 to S3 pins of the multiplexer are connected to Arduino digital pins 2 to 5.
The EN and GND pins of the multiplexer are connected to gnd2, and the VCC pin is connected to the 5V pin.

## Expected Behavior
When the Arduino powers up, it should:
1. Set Arduino pins 2 to 5 as output pins.
2. Initialize the Serial Monitor for communication.
3. Continuously read the sensor values from each channel (0 to 5).
4. Print each sensor value to the Serial Monitor in the format: "Sensor {channel}: {light value}" for example: "Sensor 0: 544"

## Success Criteria
The test is successful when:
1. The light value of each photoresistor (1 to 6) is printed to the Serial Monitor in the correct format.

## Notes for Implementation
- No other component is connected to I6~I15 pin of the anaolog multiplexer.
