#include "MAX1067.h"



MAX1067::MAX1067(uint32_t EOCpin)
{
	pinMode(SS, OUTPUT);
	digitalWrite(SS, HIGH); // so that we can set it low when we're ready
	pinMode(EOCpin, INPUT);
}

void MAX1067::readADC(uint32_t EOCpin, uint8_t sendByte){
	// This function looks at the opcode and decides which function to use for 
	// reading the ADC. 
	OPCODE_parser(sendByte);
	digitalWrite(SS, HIGH);
	if (SINGLE_CHANNEL){
		singleChannelNoScan(EOCpin, sendByte);
	}
	else if (!SINGLE_CHANNEL && EXTERNAL_CLK){
		Serial.println("Error! Scan modes ONLY work with the INTERNAL clock!");
		return;
	}
	else if (CHANNEL_3 && SCAN_0_N){
		readAllChannels(EOCpin, sendByte);
	}
}

void MAX1067::readAllChannels(uint32_t EOCpin, uint8_t sendByte)
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
	digitalWrite(SS, LOW);

	// This first transmission of the opcode gets sent no matter what to tell the 
	// ADC what we want, and how we want it done
	SPI.transfer(sendByte);

	while (digitalRead(EOCpin)) {
		yield(); 				//yield() is needed for ESP8266, otherwise delay(1) would work
	}
	
	inByte = SPI.transfer(sendByte);
	result = inByte;
	inByte = SPI.transfer(sendByte);
	result = result << 8;
	result = result | inByte;
	CH0_result = DropTwoLSB(result);
	Serial.print("Result CH0: ");
	Serial.println(CH0_result);
	
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
	digitalWrite(SS, HIGH);
}

void MAX1067::singleChannelNoScan(uint32_t EOCpin, uint8_t sendByte){
  uint8_t inByte = 0; // buffer to hold incoming data
  uint32_t result = 0;  // This is 4 bytes large
  uint32_t channel_result = 0; 

  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
  digitalWrite(SS, LOW);

  SPI.transfer(sendByte);

  if (INTERNAL_CLK){
    while (digitalRead(EOCpin)) {
      yield();        //yield() is needed for ESP8266, otherwise delay(1) would work
    }
  } 

  inByte = SPI.transfer(sendByte);
  result = inByte;
  inByte = SPI.transfer(sendByte);
  result = result << 8;
  result = result | inByte;
  channel_result = DropTwoLSB(result);
  Serial.print("Result: ");
  Serial.println(channel_result);

  SPI.endTransaction();
  digitalWrite(SS, HIGH);
}

uint32_t MAX1067::DropTwoLSB(uint32_t inputData)
{
	// Drop lowest two bits and realign D14 through D0. 
	unsigned int keepMask = 0xFFFC; //four bytes long = 32 bits
	return ((keepMask & inputData) >> 2);

}

uint8_t MAX1067::OPCODE_parser(uint8_t opcode)
{
	// Necessary to assert all these as false each time because
	// if a program used multiple opcodes in one program, these 
	// flags could get set in mutually exclusive ways.
	CHANNEL_0 = false;   
	CHANNEL_1 = false;   
	CHANNEL_2 = false;
	CHANNEL_3 = false;

	SINGLE_CHANNEL = false;   
	SCAN_0_N = false;   
	SCAN_2_N = false;
	SCAN_N_X4 = false;

	STAY_ON = false;   
	BOTH_OFF = false;   
	INTERNAL_REF_ON = false;
	STAY_OFF = false;

	EXTERNAL_CLK = false;   
	INTERNAL_CLK = false; 
	//Serial.print("Currently using an Op-Code that specifies: ");

	uint8_t channel_mask = B11100000 & opcode;
	switch (channel_mask){
		case B00000000:
		  //Serial.print("channel 0, ");
		  CHANNEL_0 = true;
		  break;
		case B00100000:
		  //Serial.print("channel 1, ");
		  CHANNEL_1 = true;
		  break;
		case B01000000:
		  //Serial.print("channel 2, ");
		  CHANNEL_2 = true;
		  break;
		case B01100000:
		  //Serial.print("channel 3, ");
		  CHANNEL_3 = true;
	}

	uint8_t scanmode_mask = B00011000 & opcode;
	switch (scanmode_mask){
		case B00000000:
		  //Serial.print("single channel no scan, ");
		  SINGLE_CHANNEL = true;
		  break;
		case B00001000:
		  //Serial.print("sequentially scan channels 0-N, ");
		  SCAN_0_N = true;
		  break;
		case B00010000:
		  //Serial.print("scan channels 2-N, ");
		  SCAN_2_N = true;
		  break;
		case B00011000:
		  //Serial.print("scan channel N four times, ");
		  SCAN_N_X4 = true;
		  break;
	}

	uint8_t powermode_mask = B00000110 & opcode;
	switch(powermode_mask){
		case B00000000:
			//Serial.println("Internal ref and buffer stay on, ");
			STAY_ON = true;
			break;
		case B00000010:
			//Serial.println("Internal ref and buffer off between conversions, ");
			BOTH_OFF = true;
			break;
		case B00000100:
			//Serial.println("Internal ref on, buffer off between conversions, ");
			INTERNAL_REF_ON = true;
			break;
		case B00000110:
			//Serial.println("Internal ref and buffer always off, ");
			STAY_OFF = true;
			break;
	}
	
	uint8_t clock_mask = B00000001 & opcode;
	switch (clock_mask) {
		case B00000000:
		  //Serial.println("external clock.");
		  EXTERNAL_CLK = true;
		  break;
		case B00000001:
		  //Serial.println("internal clock.");
		  INTERNAL_CLK = true;
		  break;
	}
	return opcode; //This lets me use this function within the call to readADC()
}
