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

#include "Arduino.h"

#ifndef AFE4400_h
#define AFE4400_h



// Defines for Registers
#define CONTROL0       0x00

// timing registers
#define LED2STC        0x01
#define LED2ENDC       0x02
#define LED2LEDSTC     0x03
#define LED2LEDENDC    0x04
#define ALED2STC       0x05
#define ALED2ENDC      0x06
#define LED1STC        0x07
#define LED1ENDC       0x08
#define LED1LEDSTC     0x09
#define LED1LEDENDC    0x0A
#define ALED1STC       0x0B
#define ALED1ENDC      0x0C
#define LED2CONVST     0x0D
#define LED2CONVEND    0x0E
#define ALED2CONVST    0x0F
#define ALED2CONVEND   0x10
#define LED1CONVST     0x11
#define LED1CONVEND    0x12
#define ALED1CONVST    0x13
#define ALED1CONVEND   0x14
#define ADCRSTSTCT0    0x15
#define ADCRSTENDCT0   0x16
#define ADCRSTSTCT1    0x17
#define ADCRSTENDCT1   0x18
#define ADCRSTSTCT2    0x19
#define ADCRSTENDCT2   0x1A
#define ADCRSTSTCT3    0x1B
#define ADCRSTENDCT3   0x1C
#define PRPCOUNT       0x1D


#define CONTROL1       0x1E
#define SPARE1         0x1F
#define TIAGAIN        0x20
#define TIA_AMB_GAIN   0x21
#define LEDCNTRL       0x22
#define CONTROL2       0x23
#define SPARE2         0x24
#define SPARE3         0x25
#define SPARE4         0x26
#define RESERVED1      0x27
#define RESERVED2      0x28
#define ALARM          0x29
#define LED2VAL        0x2A
#define ALED2VAL       0x2B
#define LED1VAL        0x2C
#define ALED1VAL       0x2D
#define LED2_ALED2VAL  0x2E
#define LED1_ALED1VAL  0x2F
#define DIAG           0x30

// Class definition 

class AFE4400
{
public:
	int chipSelectPin;
	int nAFE_pwdn;
	int AFE_ADC_rdy;
	int nAFE_rst;
	bool debugSerial;

	int pulse;
	int oximetry;

	AFE4400();
	
	//
	// Basic Device control and readback functions
	//
	void enableDebugSerial(); 
	void disableDebugSerial();
	void sw_reset();
	void setLEDCurrent(uint32_t led1_current, uint32_t led2_current);
	void begin(int ss); 		
	void setDefaultTiming();
	void beginMeasure();
	int readPulseData();
	int readOxData();
	
	// May want to write code for diagnostic mode (enabled with bit 2 of CONTROL0)
	
	
	//  Low-level SPI control, to simplify overall coding
	//  using 32 bit ints to make passing data around easier
	//  but keep in mind that registers are only 24 bits wide
	void SPIWriteBit(byte regAddress, uint8_t bit, bool bit_high);
	uint32_t SPIReadReg(byte regAddress);
	void SPIWriteReg(byte regAddress, uint32_t regValue);
	
	void writeTimingData(byte regAddress, uint16_t timing_value);

	
private:
	void SPIEnableRead();
	void SPIDisableRead();
};

#endif