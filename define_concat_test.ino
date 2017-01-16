#include "MAX1067.h"

MAX1067 adc = MAX1067(1, 2, 3, 4, 5);

const byte MAX1067_init = B01101001;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  byte myOpcode = MAX1067_init;
  Serial.println(adc.getChannel(myOpcode));

  Serial.println();
  if (myOpcode & B00000000){
    Serial.println("Ext clock.");
  }
  else if (myOpcode & B00000001){
    Serial.print("Int clock.");
  }
  
}

void loop() {
  // put your main code here, to run repeatedly:

}

