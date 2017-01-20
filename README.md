# MAX1067
An Arduino library for the Maxim MAX1067 14-bit ADC. 
https://www.maximintegrated.com/en/products/analog/data-converters/analog-to-digital-converters/MAX1067.html
This chip is a Multichannel, 14-Bit, 200ksps Analog-to-Digital Converter which can handle both 5V and 3.3V supplies, and is able to handle different voltages on its analog and digital sides. For example, you can interface a 0-5V analog signal with a 3.3V digital device using this chip. 

Handles opcodes for choosing the channel (0-3), scan method (single channel no scan, scan 0-N), power-down modes, and clock choice. So this lets you choose one channel to scan, or scan through all four channels. I have not added the functionality to scan from channel 2 to N or scan one channel four times.

SPI speed is set at 1MHz but that can be changed in SPI.beginTransaction(Speed, Bit order, SPI mode). 

For more information including hookup guide and other scan methods please read the datasheet for this chip, linked above. 