#ifndef AD7730_h
#define AD7730_h

#include "Arduino.h"
#include "SPI.h"
#include "Math.h"

class AD7730
{
	public:
		AD7730(int resetPin, int readyPin, int chipSelect);
		void reset();//resets the AD7730 chip. Equivalent to a power down power up.
		void internalCalibrate();//performs internal zero and full scale calibrations of the chip
		void systemZeroCalibrate();//performs system zero scale calibration only of the system (sets the intercept to zero in the linear transfer function)
		void systemFullCalibrate();//performs full scale calibration of the system.
		void waitForReady();//pause until ready pin goes low
		void offsetDAC(float offset);// Sets the DAC tare offset value. Allowed range is -40 to 40 (mV) in increments of 2.5 mV
		void range(int range);//Sets the voltage range (gain) of the chip. Allowed values are 10, 20, 40, 80. 10mv range is most sensitive
		void vRef(float volts);//Sets the value of the high reference voltage. Allowed values are 2.5, 5 volts
		void channel(int chan);//Sets the input channel (1 or 2)
		void filter(int avg);//Sets the number of samples to be averaged by the smoothing filter. Default is 512. Max smoothing is 4095.
		void continuousConversionMode();//places the chip in continuous conversion mode (but does not begin continuous reading)
		void unipolar();//sets the chip in unipolar conversion mode
		void bipolar();//sets the chip in bipolar conversion mode (default)
		unsigned long read();//takes a single reading of the raw result from the AD7730 chip using the parameters set above.
		float readVoltage();//takes a single voltage reading from the chip using the parameters set above and converts to a voltage.
	private:
		int _resetPin;
		int _readyPin;
		int _CS;
};

#endif