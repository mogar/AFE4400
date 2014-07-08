/*
 Arduino Library for Texas Instruments AFE4400 - Pulse-Oximetry Analog Front End
 go to http://www.ti.com/product/afe4400 for datasheet
 
 See https://github.com/mogar/AFE4400 for more info
 
 
The MIT License (MIT)

Copyright (c) 2014 Morgan Redfield

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 
 */ 

#include <Arduino.h>
#include <AFE4400.h>
#include <SPI.h>

AFE4400::AFE4400() {
  debugSerial = 0;
}

void AFE4400::sw_reset() {
   // write to control0 reg bit 3 1	
  // soft reset
  SPIWriteBit(CONTROL0, 3, true);

   // wait for bit 3 of control0 to be 0
  delay(10);
}



//
//  begin()
//  Initial SPI setup, soft reset of device
//
void AFE4400::begin(int ss) {
  slaveSelectPin = ss;
  pinMode(slaveSelectPin, OUTPUT);
  SPI.begin();
  SPI.setDataMode(SPI_MODE0);	//CPHA = CPOL = 0    MODE = 0
  delay(1000);
    
  sw_reset();
  
  if (debugSerial) {Serial.println("Soft Reset\n");}
 }
 
 void AFE4400::enableDebugSerial() {
   debugSerial = 1;
 }
 
 void AFE4400::disableDebugSerial() {
   debugSerial = 0;
 }

 void AFE4400::setLEDCurrent(uint32_t led1_current, uint32_t led2_current) {
	// read the reg
	uint32_t current_val = SPIReadReg(LEDCNTRL);
	
	// set led 1 value
	led1_current = led1_current & 0xFF;
	current_val = current_val & ~(0xFF << 8);
	current_val = current_val | (led1_current << 8);
	
	// set led 2 value
	led2_current = led2_current & 0xFF;
	current_val = current_val & ~(0xFF);
	current_val = current_val | (led2_current);

	// write reg
	SPIWriteReg(LEDCNTRL, current_val);
 }
 
 void AFE4400::setDefaultTiming() {
   // set pulse repetition frequency to 500Hz, duty cycle to 25%

  // setup the timer module to default values
  writeTimingData(LED2STC, 6000);
  writeTimingData(LED2ENDC, 7999);
  writeTimingData(LED2LEDSTC, 6000);
  writeTimingData(LED2LEDENDC, 7998);
  writeTimingData(ALED2STC, 0);
  writeTimingData(ALED2ENDC, 1998);
  writeTimingData(LED1STC, 2000);
  writeTimingData(LED1ENDC, 3998);
  writeTimingData(LED1LEDSTC, 2000);
  writeTimingData(LED1LEDENDC, 3999);
  writeTimingData(ALED1STC, 4000);
  writeTimingData(ALED1ENDC, 5998);
  writeTimingData(LED2CONVST, 2);
  writeTimingData(LED2CONVEND, 1999);
  writeTimingData(ALED2CONVST, 2002);
  writeTimingData(ALED2CONVEND, 3999);
  writeTimingData(LED1CONVST, 4002);
  writeTimingData(LED1CONVEND, 5999);
  writeTimingData(ALED1CONVST, 6002);
  writeTimingData(ALED1CONVEND, 6002);
  writeTimingData(ALED1CONVEND, 7999);
  writeTimingData(ADCRSTSTCT0, 0);
  writeTimingData(ADCRSTENDCT0, 2);
  writeTimingData(ADCRSTSTCT1, 2000);
  writeTimingData(ADCRSTENDCT1, 2002);
  writeTimingData(ADCRSTSTCT2, 4000);
  writeTimingData(ADCRSTENDCT2, 4002);
  writeTimingData(ADCRSTSTCT3, 6000);
  writeTimingData(ADCRSTENDCT3, 6002);
  writeTimingData(PRPCOUNT, 7999);
 }
 
//
//  beginMeasure()
//  turn on Measurement mode - required after reset
//  be sure to set LED current before calling
//  be sure to set timing parameters before calling (e.g. with setDefaultTiming)
// 
void AFE4400::beginMeasure() {
  // setup the LED driver (bit 11 of CONTROL2)
  SPIWriteBit(CONTROL2, 11, false); // set it for H-Bridge mode

  // turn on LED current (bit 17 of LEDCNTRL)
  SPIWriteBit(LEDCNTRL, 17, true);

  // enable the timer module (bit D8 of CONTROL1)
  SPIWriteBit(CONTROL1, 8, true);
  
  if (debugSerial) {Serial.print("Now beginning pulse and oximeter readings");}
  
  // TODO: setup a timer or an interrupt to do SPI periodically
  
}

int AFE4400::readPulseData(){
  return pulse;
}

int AFE4400::readOxData(){
  return oximetry;
}

// Basic SPI routines to simplify code
// read and write one register
void AFE4400::SPIWriteBit(byte regAddress, uint8_t bit, bool bit_high) {
	// read the reg
	uint32_t current_val = SPIReadReg(regAddress);
	
	// check to see if we need to change the bit
	if (bit_high & !(current_val & 1 << bit)) {
		// set bit correct in reg
		current_val = current_val | (bit_high << bit);
		
		// write reg
		SPIWriteReg(regAddress, current_val);
	} else if (!bit_high & (current_val & 1 << bit)) {
		// set bit correct in reg
		current_val = current_val & ~(bit_high << bit);
		
		// write reg
		SPIWriteReg(regAddress, current_val);
	}
	
}


uint32_t AFE4400::SPIReadReg(byte regAddress){
  byte temp_byte = 0;
  uint32_t reg_value = 0;
  
  
  // enable reading from registers
  SPIEnableRead();
  
  digitalWrite(slaveSelectPin, LOW);
  
  // set address
  SPI.transfer(regAddress);
  // get first byte
  temp_byte = SPI.transfer(0x00);
  reg_value = temp_byte << 16;
  // get second byte
  temp_byte = SPI.transfer(0x00);
  reg_value = temp_byte << 8;
  // get last byte
  temp_byte = SPI.transfer(0x00);
  
  digitalWrite(slaveSelectPin, HIGH);
  
  // disable reading from registers
  SPIDisableRead();

  return reg_value;
}

void AFE4400::SPIEnableRead() {
	SPIWriteReg(CONTROL0, 1);
	delay(1);
}

void AFE4400::SPIDisableRead() {
	SPIWriteReg(CONTROL0, 0);
	delay(1);
}

void AFE4400::SPIWriteReg(byte regAddress, uint32_t regValue){
  byte temp_byte = 0;
  
  digitalWrite(slaveSelectPin, LOW);
  
  SPI.transfer(regAddress);  // write instruction
  temp_byte = regValue >> 16;
  SPI.transfer(regValue);
  temp_byte = regValue >> 8;
  SPI.transfer(regValue);
  temp_byte = regValue;
  SPI.transfer(regValue);
  
  digitalWrite(slaveSelectPin, HIGH);
}

void AFE4400::writeTimingData(byte regAddress, uint16_t timing_value) {
	if (regAddress < LED2STC || regAddress > PRPCOUNT) {
		if (debugSerial) {Serial.print("Error: timing address out of range"); }
		return;
	}
	uint32_t reg_val = timing_value;
	
	// write the value
	SPIWriteReg(regAddress, reg_val);
}

