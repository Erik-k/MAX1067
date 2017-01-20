// Sample arduino program to read the MAX1067 from an Arduino Uno.
// This program will read the ADC once per second and output formatted
// results over serial.
// Erik Knechtel January 2017

#include "MAX1067.h"

// initialize the adc item with the EOC pin. 
// Pins on the UNO: MOSI = 11, MISO = 12, SCK = 13, SS = 10, EOC chosen as 9 for convenience
MAX1067 adc = MAX1067(9);

// Opcodes. See page 15 of device datasheet.
// Example opcode: 
// MSB: [011]   [01]      [00]       [1]
//       ch3,  scan 0-3, stay on, internal clk
const byte scan_all_channels = B01101001;

// MSB: [000]   [00]      [00]       [0]
//       ch0,  no scan, stay on, external clk
const byte single_channel_opcode = B00000000;

int i = 0;

void setup() {
  Serial.begin(115200);
  SPI.begin();
}

void loop() {
  Serial.println();
  Serial.println(i);

  // readADC(EOCpin, opcode)
  adc.readADC(9, scan_all_channels);
  
  i++;
  delay(1000);
}


