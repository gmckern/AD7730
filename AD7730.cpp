#include "Arduino.h"
#include "AD7730.h"
#include "SPI.h"
#include "Math.h"

AD7730::AD7730(int resetPin, int readyPin, int chipSelect){
	SPI.begin();
	SPI.setDataMode(SPI_MODE3);
	SPI.setBitOrder(MSBFIRST); 
	SPI.setClockDivider(SPI_CLOCK_DIV128);
	
	pinMode(resetPin, OUTPUT);
	pinMode(readyPin, INPUT);	
	pinMode(chipSelect, OUTPUT);
	pinMode(MISO, INPUT);
	pinMode(MOSI, OUTPUT);
	digitalWrite(resetPin, HIGH);
	digitalWrite(chipSelect, HIGH);

	_resetPin = resetPin;
	_readyPin = readyPin;
	_CS = chipSelect;
}


void AD7730::reset(){
	//perform a hardware reset using the reset pin on the chip
	digitalWrite(_resetPin, LOW);
	delay(200);
	digitalWrite(_resetPin, HIGH);
	delay(200);
}

void AD7730::internalCalibrate(){
	
	digitalWrite(_CS, LOW);
	SPI.transfer(0x12);//set next operation as single read of Mode register (2 byte register)
	int byte1 = SPI.transfer(0x00);//read first byte
	int byte2 = SPI.transfer(0x00);//read second byte
	
	byte1 = (byte1 & 0x1f) | (0x4 << 5);//update byte1 to write internal zero scale calibration to chip 

	SPI.transfer(0x02);//set next operation as single write to Mode register
	SPI.transfer(byte1);//write first byte
	SPI.transfer(byte2);//write updated second byte
	digitalWrite(_CS, HIGH);

	
	waitForReady();//wait for internal zero scale calibration to complete
	
	byte1 = (byte1 & 0x1f) | (0x5 << 5);//update byte1 to write internal full scale calibration to chip 
	
	digitalWrite(_CS, LOW);
	SPI.transfer(0x02);//set next operation as single write to Mode register
	SPI.transfer(byte1);//write first byte
	SPI.transfer(byte2);//write updated second byte
	digitalWrite(_CS, HIGH);
	
	waitForReady();//wait for internal full scale calibration to complete
	
}

void AD7730::systemZeroCalibrate(){
	
	digitalWrite(_CS, LOW);
	SPI.transfer(0x12);//set next operation as single read of Mode register (2 byte register)
	int byte1 = SPI.transfer(0x00);//read first byte
	int byte2 = SPI.transfer(0x00);//read second byte
	
	byte1 = (byte1 & 0x1f) | (0x6 << 5);//update byte1 to write system zero scale calibration to chip 

	SPI.transfer(0x02);//set next operation as single write to Mode register
	SPI.transfer(byte1);//write first byte
	SPI.transfer(byte2);//write updated second byte
	digitalWrite(_CS, HIGH);
	
	waitForReady();//wait for system zero scale calibration to complete
}

void AD7730::systemFullCalibrate(){
	
	digitalWrite(_CS, LOW);
	SPI.transfer(0x12);//set next operation as single read of Mode register (2 byte register)
	int byte1 = SPI.transfer(0x00);//read first byte
	int byte2 = SPI.transfer(0x00);//read second byte
	
	byte1 = (byte1 & 0x1f) | (0x7 << 5);//update byte1 to write system zero scale calibration to chip 

	SPI.transfer(0x02);//set next operation as single write to Mode register
	SPI.transfer(byte1);//write first byte
	SPI.transfer(byte2);//write updated second byte
	digitalWrite(_CS, HIGH);
	
	waitForReady();//wait for system zero scale calibration to complete
}

void AD7730::waitForReady(){
	while(digitalRead(_readyPin)){
		//keep waiting as long as ready pin remains HIGH
	}
}
	

void AD7730::offsetDAC(float offset){
	int index = (int)(offset/2.5);
	int sign = 0;
	if(index < 0){
		sign = 1;
		index *= -1;
	}
	int toSend = (sign << 5 | index);//format offset setting as byte to send to DAC register
	
	digitalWrite(_CS, LOW);
	SPI.transfer(0x04);//set next operation as single write to the DAC register
	SPI.transfer(toSend);//write data to DAC register
	digitalWrite(_CS, HIGH);
}

void AD7730::range(int range){
	// format range setting is 2 bits to write to Mode register
	int bits;
	if(range <= 10){
		bits = 0;
	}else if(range <= 20){
		bits = 1;
	}else if(range <= 40){
		bits = 2;
	}else{
		bits = 3;
	}
	
	digitalWrite(_CS, LOW);
	SPI.transfer(0x12);//set next operation as single read of Mode register (16 bit register)
	int byte1 = SPI.transfer(0xff);//read first byte
	int byte2 = SPI.transfer(0xff);//read second byte
		
	byte2 = (0xcf & byte2) | (bits << 4);//replace current range bits with specified bits
	
	SPI.transfer(0x02);//set next operation as single write to Mode register
	SPI.transfer(byte1);//write first byte
	SPI.transfer(byte2);//write updated second byte
	digitalWrite(_CS, HIGH);
}

void AD7730::vRef(float volts){
	int bit;
	if(volts <= 2.5){
		bit = 0;
	}else{
		bit = 1;
	}
	
	digitalWrite(_CS, LOW);
	SPI.transfer(0x12);//set next operation as single read of Mode register (16 bit register)
	int byte1 = SPI.transfer(0x00);//read first byte
	int byte2 = SPI.transfer(0x00);//read second byte
		
	byte2 = (0x7f & byte2) | (bit << 7);//replace current range bits with specified bits
	
	SPI.transfer(0x02);//set next operation as single write to Mode register
	SPI.transfer(byte1);//write first byte
	SPI.transfer(byte2);//write updated second byte
	digitalWrite(_CS, HIGH);
}

void AD7730::channel(int chan){
	int bits;
	if(chan==1){
		bits = 0;
	}else{
		bits = 1;
	}
	
	digitalWrite(_CS, LOW);
	SPI.transfer(0x12);//set next operation as single read of Mode register (16 bit register)
	int byte1 = SPI.transfer(0x00);//read first byte
	int byte2 = SPI.transfer(0x00);//read second byte
		
	byte2 = (0xfc & byte2) | bits;//replace current channel bits with specified bits
	
	SPI.transfer(0x02);//set next operation as single write to Mode register
	SPI.transfer(byte1);//write first byte
	SPI.transfer(byte2);//write updated second byte
	digitalWrite(_CS, HIGH);
}

void AD7730::filter(int avg){
	
	digitalWrite(_CS, LOW);
	SPI.transfer(0x13);//set next operation as single read of Filter register (24 bit register)
	int byte1 = SPI.transfer(0x00);//read first byte
	int byte2 = SPI.transfer(0x00);//read second byte
	int byte3 = SPI.transfer(0x00);//read third byte
	
	//trim input parameter to 12 bits
	if(avg > 4095){
		avg = 4095;
	}
	byte1 = avg >> 4;//fill byte1 with 8 most signicant bits of avg
	byte2 = (byte2 & 0xf) | avg << 8;//4 least significant bits of avg spill over to byte2
	
	SPI.transfer(0x03);//set next operation as single write to Filter register
	SPI.transfer(byte1);
	SPI.transfer(byte2);
	SPI.transfer(byte3);
	digitalWrite(_CS, HIGH);
}

void AD7730::unipolar(){
	
	digitalWrite(_CS, LOW);
	SPI.transfer(0x12);//set next operation as single read of Mode register (16 bit register)
	int byte1 = SPI.transfer(0x00);//read first byte
	int byte2 = SPI.transfer(0x00);//read second byte
		
	byte1 = (byte1 & 0xef) | 0x10;//configure unipolar conversion mode
	
	SPI.transfer(0x02);//set next operation as single write to Mode register
	SPI.transfer(byte1);//write first byte
	SPI.transfer(byte2);//write second byte
	digitalWrite(_CS, HIGH);
}

void AD7730::bipolar(){
	
	digitalWrite(_CS, LOW);
	SPI.transfer(0x12);//set next operation as single read of Mode register (16 bit register)
	int byte1 = SPI.transfer(0x00);//read first byte
	int byte2 = SPI.transfer(0x00);//read second byte
		
	byte1 = (byte1 & 0xef);//configure bipolar conversion mode
	
	SPI.transfer(0x02);//set next operation as single write to Mode register
	SPI.transfer(byte1);//write first byte
	SPI.transfer(byte2);//write second byte
	digitalWrite(_CS, HIGH);
}

void AD7730::continuousConversionMode(){
	
	digitalWrite(_CS, LOW);
	SPI.transfer(0x12);//set next operation as single read of Mode register (16 bit register)
	int byte1 = SPI.transfer(0x00);//read first byte
	int byte2 = SPI.transfer(0x00);//read second byte
		
	byte1 = (byte1 & 0x1f) | (0x01 << 5);//configure for continuous conversion mode
	
	SPI.transfer(0x02);//set next operation as single write to Mode register
	SPI.transfer(byte1);//write first byte
	SPI.transfer(byte2);//write second byte
	digitalWrite(_CS, HIGH);
}

unsigned long AD7730::read(){
	//use when chip is in continuous conversion mode. This function takes a single reading so should be used in a loop.
	unsigned long result;
	
	digitalWrite(_CS, LOW);
	SPI.transfer(0x11);//set next operation as single read from Data register (24 bit register assuming the chip is used in 24 bit mode)
	digitalWrite(_CS, HIGH);
	
	waitForReady();
	
	digitalWrite(_CS, LOW);
	int byte1 = SPI.transfer(0xff);
	int byte2 = SPI.transfer(0xff);
	int byte3 = SPI.transfer(0xff);
	digitalWrite(_CS, HIGH);
	
	result = byte1*256L*256L + byte2*256L + byte3;
	return result;
}

float AD7730::readVoltage(){
	
	digitalWrite(_CS, LOW);
	SPI.transfer(0x12);//set next operation as single read of Mode register (16 bit register)
	int byte1 = SPI.transfer(0x00);//read first byte
	int byte2 = SPI.transfer(0x00);//read second byte
	digitalWrite(_CS, HIGH);
	
	int raw = read();
	int unipolar = (byte1 & 0xe0) >> 4;//mask out and shift polarity bit; 0 is bipolar, 1 is unipolar
	float mV;
	
	int range = (byte2 & 0xc0) >> 4;//mask out and shift range bits
	if(range == 0){
		range = 10;
	}else if(range == 1){
		range = 20;
	}else if(range == 2){
		range = 40;
	}else{
		range = 80;
	}
	
	if(!unipolar){
		mV = (float)range*((float)raw - (float)8388608)/(float)8388608;
		return mV;
	}else{
		mV = (float)range*(float)raw/(float)16777216;
		return mV;
	}
}