# AD7730-for-Arduino
Easy to use Arduino library for the AD7730 analog to digital converter / amplifier

Hi there! This library is designed to simplify the programming of the AD7730 chip from an Arduino board.
However, the AD7730 is not the simplest chip to work with as it was not specifically designed for hobbyists.

While the programming side of the project can be simplified considerably with the help of a library, configuring the hardware properly will take some time and patience. The following post is very helpful in terms of getting the pins connected properly:

https://forum.arduino.cc/t/arduino-and-the-ad7730/320036

| AD7730 pin | Mnemonic | Arduino Pin | Notes                                                       |
| ---------- | -------- | ----------- | ---                                                         |
| 1          | SCLK     | Arduino D13 |                                                             |
| 2	         | MCLK IN  | Crystal +   | External clock source connects to this pin only             |
| 3	         | MCLK OUT | Crystal -   | Leave pin unconnected when using an external clock source   |
| 4	         | POL      | 5V or 3.3V  | Logic High (should Match DVDD)                              |
| 5 | ~SYNC	| 5V or 3.3V | Logic High (should Match DVDD)  |
| 6	| ~RESET | Arduino D8 | |
| 7	| VBIAS	| | |
| 8	| AGND | GND | |
| 9	| AVDD | 5v | |
| 10 | AIN1+ | Bridge Circuit + | |
| 11 | AIN1- | Bridge Circuit - | |
| 12 | AIN2+ | | |
| 13 | AIN2- | | |	
| 14 | REFIN+ | 5v | |
| 15 | REFIN- | GND | |
| 16 | ACX | | |
| 17 | ~ACX	| | |
| 18 | ~STANDBY | 5V or 3.3V | Logic High (should Match DVDD)  |
| 19 | ~CS | Arduino D10 | |
| 20 | ~RDY | Arduino D9 | |
| 21 | DOUT | Arduino D12 | Keep this wire as short as possible |
| 22 | DIN | Arduino D11 | Keep this wire as short as possible |
| 23 | DVDD | 5V or 3.3V | Digital voltage should match the board (3.3V for Arduino DUE) |
| 24 | DGND | GND | Must be connected to the Arduino DIGITAL GND pin! |

***Important Note***
The AD7730 chip communicates via SPI. SPI is not reliable over long wires. 
Even using off the shelf Dupont wires for the SPI pins may be unreliable. 
Try to keep your SPI wires under 3 inches or so. 
Also, make sure to connect the digital ground pin on the AD7730 (pin 24) to the ground pin on the digital side of the Arduino.
According to the AD7730 datasheet, the digital supply voltage can be 5V nominal, or 3V nominal. Running this chip with the Arduino UNO is simple as both digital and analog HIGH will be 5V. However, it should be possible to run this chip with the Arduino DUE board (which operates at 3.3V) In that case you would supply the digital side of the AD7730 chip with 3.3V and the analog side with 5V. All digital communication and logic pins would operate at 3.3V rather than 5V. 

