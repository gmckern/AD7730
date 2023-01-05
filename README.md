# AD7730-for-Arduino
Easy to use Arduino library for the AD7730 analog to digital converter / amplifier

Hi there! This library is designed to simplify the programming of the AD7730 chip from an Arduino board.
However, the AD7730 is not the simplest chip to work with as it was not specifically designed for hobbyists.

While the programming side of the project can be simplified considerably with the help of a library, configuring the hardware properly will take some time and patience. The following post is very helpful in terms of getting the pins connected properly:

https://forum.arduino.cc/t/arduino-and-the-ad7730/320036

Here is a summary of the pin configuration:
  AD7730 PIN	ARDUINO PIN
1	  SCLK		  Arduino D13
2	  MCLK IN		Crystal + (if using an external clock source connect to this pin only)
3	  MCLK OUT	Crystal - (don't connect this pin if using an external clock source)
4	  POL			  5v
5	  ~SYNC		  5v
6	  ~RESET		Arduino D8
7	  VBIAS	
8	  AGND		  GND (use Arduino analog ground)
9	  AVDD		  5v
10	AIN1+		  Load Cell + (Green)
11	AIN1-		  Load Cell – (White)
12	AIN2+	
13	AIN2-	
14	REFIN+		5v
15	REFIN-		GND
16	ACX	
17	~ACX	
18	~STANDBY	5v
19	~CS			  Arduino D10
20	~RDY		  Arduino D9
21	DOUT		  Arduino D12
22	DIN			  Arduino D11
23	DVDD		  5v
24	DGND		  GND (Important! use Arduino digital ground pin)

***Important Note***
The AD7730 chip communicates via SPI. SPI is not reliable over long wires. 
Even using off the shelf Dupont wires for the SPI pins may be unreliable. 
Try to keep your SPI wires under 3 inches or so. 
Also, make sure to connect the digital ground pin on the AD7730 (pin 24) to the ground pin on the digital side of the Arduino.

