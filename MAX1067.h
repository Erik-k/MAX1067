/*
	This library lets the user communicate over SPI with the MAX1067 
	14-bit ADC. 
	Created by Erik Knechtel, Winter 2017
	www.knek-tek.me
*/


#ifndef MAX1067_h
#define MAX1067_h

#define FALSE 0
#define TRUE 1

#include <Arduino.h>
#include <SPI.h> 
// On the ESP8266: MOSI=D13, MISO=D12, SCLK=D14, SDA=D2
// On the UNO: MOSI = 11, MISO = 12, SCK = 13, SS = 10, EOC chosen as 9 for convenience

// The SPI pins are defined in pins_arduino.h which is different for each board. 

class MAX1067
{
public:
	boolean CHANNEL_0 = false;   
	boolean CHANNEL_1 = false;   
	boolean CHANNEL_2 = false;
	boolean CHANNEL_3 = false;

	boolean SINGLE_CHANNEL = false;   
	boolean SCAN_0_N = false;   
	boolean SCAN_2_N = false;
	boolean SCAN_N_X4 = false;

	boolean STAY_ON = false;   
	boolean BOTH_OFF = false;   
	boolean INTERNAL_REF_ON = false;
	boolean STAY_OFF = false;

	boolean EXTERNAL_CLK = false;   
	boolean INTERNAL_CLK = false; 
	
	MAX1067(uint32_t EOCpin); // Constructor which initializes pins
	void readADC(uint32_t EOCpin, uint8_t sendByte);
	void readAllChannels(uint32_t EOCpin, uint8_t sendByte);						  // Function to read ADC
	void singleChannelNoScan(uint32_t EOCpin, uint8_t sendByte);
	uint32_t DropTwoLSB(uint32_t inputData);			  // Cleans ADC data
	uint8_t OPCODE_parser(uint8_t opcode);							  // Nice for double checking bit values	
	
private:

};

#endif
