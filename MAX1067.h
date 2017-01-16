/*
	This library lets the user communicate over SPI with the MAX1067 
	14-bit ADC. 
	Created by Erik Knechtel, Winter 2017
	www.knek-tek.me
*/


#ifndef MAX1067_h
#define MAX1067_h

#include <Arduino.h>
#include <SPI.h> 
// On the ESP8266: defines MOSI=D13, MISO=D12, SCLK=D14, SDA=D2
// On the UNO: MOSI = 11, MISO = 12, SCK = 13, SS = 10, EOC chosen as 9 for convenience

//Should pin defines be left open to the user, and have the constructor take 
// the pins in as values? Need cross platform compatibility. HOWEVER the 
// libraries for each chip will have their own defines. See how other libraries
// handle cross compatibility. 

const int EOC = 1;
const int CS = 1; 

typedef enum
{
	CHANNEL_0           = 0b000,   
	CHANNEL_1           = 0b001,   
	CHANNEL_2           = 0b010,
	CHANNEL_3			= 0b011
} max1067_channel_t;

typedef enum
{
	SINGLE_CHANNEL      = 0b00,   
	SCAN_0_N   		    = 0b01,   
	SCAN_2_N            = 0b10,
	SCAN_N_X4		    = 0b11
} max1067_scan_t;

typedef enum
{
	STAY_ON             = 0b00,   
	BOTH_OFF            = 0b01,   
	INTERNAL_REF_ON     = 0b10,
	STAY_OFF		    = 0b11
} max1067_powermode_t;

typedef enum
{
	EXTERNAL_CLK        = 0b0,   
	INTERNAL_CLK        = 0b1 
} max1067_clock_t;


class MAX1067
{
public:
    MAX1067(int CSpin, int MOSI, int MISO, int SCLK, int EOCpin); // Constructor which initializes pins
	unsigned int readADC(byte sendByte);						  // Function to read ADC
	unsigned int DropTwoLSB(unsigned int inputData);			  // Cleans ADC data
	byte OPCODE_parser(byte opcode);							  // Nice for double checking bit values
	max1067_channel_t getChannel(byte opcode);					// For parsing opcodes
	
	
private:
	

};

#endif