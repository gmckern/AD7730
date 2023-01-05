#include <AD7730.h>

AD7730 adc(8,9,10); //create new instance with reset pin = 8, ready pin = 9, chip select pin = 10

void setup() {
  Serial.begin(9600);
  adc.reset();          //reset the AD7730 chip
  adc.filter(512);      //set digital filter to 512 samples (This setting provides good filtering of 50 hz and 60 hz noise)
  adc.range(10);        //set voltage range to 10 millivolts (this sets the gain of the internal amplifier)
  adc.offsetDAC(0);     //use this function to correct for a wheatstone bridge circuit whose resistors aren't perfectly balanced
  adc.unipolar();       //set chip to unipolar mode
  adc.internalCalibrate();          //run internal calibration of chip before reading voltages
  adc.continuousConversionMode();   //place chip in continuous conversion mode
}

long result;            //declare variable to hold conversion results. Since the AD7730 is a 24bit ADC, the 16bit int data type will not work for this.

void loop() {
  result = adc.read();              //take a single conversion result from the chip
  Serial.println(result);           //print conversion result to the serial monitor
}
