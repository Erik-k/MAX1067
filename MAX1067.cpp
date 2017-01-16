#include "MAX1067.h"

MAX1067::MAX1067(int CSpin, int MOSI, int MISO, int SCLK, int EOCpin)
{
	pinMode(CS, OUTPUT);
	digitalWrite(CS, HIGH); // so that we can set it low when we're ready
	pinMode(EOC, INPUT);
}

unsigned int MAX1067::readADC(byte sendByte)
{
	// Currently just works with a scan of all four channels:
	// MSB: [011]   [01]      [00]       [1]
	//       ch3   scan 0-3  stay on  internal clk
	//const byte MAX1067_init = B01101001; // 0x69
	
	uint8_t inByte = 0; // buffer to hold incoming data
	uint32_t result = 0;  // This is 4 bytes large
	uint32_t CH0_result = 0; 
	uint32_t CH1_result = 0;
	uint32_t CH2_result = 0;
	uint32_t CH3_result = 0;
	
	SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
	digitalWrite(CS, LOW);

	// This first transmission of the opcode gets sent no matter what to tell the 
	// ADC what we want, and how we want it done
	SPI.transfer(sendByte);

	if (sendByte & B00000001) {		// EOC only gets used for internal clock 
		while (digitalRead(EOC)) {
			yield(); 				//yield() is needed for ESP8266, otherwise delay(1) would work
		}
	}
	
	if (sendByte & B00000000) {		// Channel 0, single channel no scan
		inByte = SPI.transfer(sendByte);
		result = inByte;
		inByte = SPI.transfer(sendByte);
		result = result << 8;
		result = result | inByte;
		CH0_result = DropTwoLSB(result);
		
		Serial.print("Result CH0: ");
		Serial.println(CH0_result);
	}
	
	if (sendByte & B00011000) {		// Channel 0, scan four times
		
		
	}
	
	inByte = SPI.transfer(sendByte);
	result = inByte;
	inByte = SPI.transfer(sendByte);
	result = result << 8;
	result = result | inByte;
	CH1_result = DropTwoLSB(result);
	Serial.print("Result CH1: ");
	Serial.println(CH1_result);

	inByte = SPI.transfer(sendByte);
	result = inByte;
	inByte = SPI.transfer(sendByte);
	result = result << 8;
	result = result | inByte;
	CH2_result = DropTwoLSB(result);
	Serial.print("Result CH2: ");
	Serial.println(CH2_result);

	inByte = SPI.transfer(sendByte);
	result = inByte;
	inByte = SPI.transfer(sendByte);
	result = result << 8;
	result = result | inByte;
	CH3_result = DropTwoLSB(result);
	Serial.print("Result CH3: ");
	Serial.println(CH3_result);

	SPI.endTransaction();
	digitalWrite(CS, HIGH);

	return (result);
}

max1067_channel_t MAX1067::getChannel(byte opcode)
{
  /* Read the opcode and returns channel choice */
  return (max1067_channel_t)(opcode & 0b11100000);
}


unsigned int MAX1067::DropTwoLSB(unsigned int inputData)
{
	// Drop lowest two bits and realign D14 through D0. 
	unsigned int keepMask = 0xFFFC; //four bytes long = 32 bits
	return ((keepMask & inputData) >> 2);

}

byte MAX1067::OPCODE_parser(byte opcode)
{
	Serial.print("Currently using an Op-Code that specifies: ");

	byte channel_mask = B11100000 & opcode;
	switch (channel_mask){
		case B00000000:
		  Serial.print("channel 0, ");
		  break;
		case B00100000:
		  Serial.print("channel 1, ");
		  break;
		case B01000000:
		  Serial.print("channel 2, ");
		  break;
		case B01100000:
		  Serial.print("channel 3, ");
	}

	byte scanmode_mask = B00011000 & opcode;
	switch (scanmode_mask){
		case B00000000:
		  Serial.print("single channel no scan, ");
		  break;
		case B00001000:
		  Serial.print("sequentially scan channels 0-N, ");
		  break;
		case B00010000:
		  Serial.print("scan channels 2-N, ");
		  break;
		case B00011000:
		  Serial.print("scan channel N four times, ");
		  break;
	}

	byte powermode_mask = B00000110 & opcode;
	switch(powermode_mask){
		case B00000000:
			Serial.println("Internal ref and buffer stay on, ");
			break;
		case B00000010:
			Serial.println("Internal ref and buffer off between conversions, ");
			break;
		case B00000100:
			Serial.println("Internal ref on, buffer off between conversions, ");
			break;
		case B00000110:
			Serial.println("Internal ref and buffer always off, ");
			break;
	}
	
	byte clock_mask = B00000001 & opcode;
	switch (clock_mask) {
		case B00000000:
		  Serial.println("external clock.");
		  break;
		case B00000001:
		  Serial.println("internal clock.");
		  break;
	}
	return opcode; //This lets me use this function within the call to readADC()
}
