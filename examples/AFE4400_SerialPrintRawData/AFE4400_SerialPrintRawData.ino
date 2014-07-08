/*

Example Code for use with AFE4400 library
 Arduino Library for Texas Instruments AFE4400 - Pulse-Oximetry Analog Front End
 go to http://www.ti.com/product/afe4400 for datasheet
 See http://mogar.github.com/AFE4400 for more info
 
Pin Connections:
Arduino    <----->  AFE4400
SS   (10)
MOSI   ()
MISO   ()
CLK    ()
GND
5V 
 
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
 
#include <SPI.h>
#include "AFE4400.h"

AFE4400 pulseOx;

void setup() {
	Serial.begin(115200);
	
	// enable debug print statements
	pulseOx.enableDebugSerial();
	
	// begin communication with the AFE4400
	pulseOx.begin(10);
	
	// setup AFE4400 for default measurements
	pulseOx.setLEDCurrent(128, 128); // half current for each LED by default
	pulseOx.setDefaultTiming();
	
	// begin measuring
	// note that after this, all pulse oximetry measurements will
	// automatically be printed to the Serial interface
	pulseOx.beginMeasure();
	
}


void loop() {
	// do nothing
	// the pulseOx class will automatically handle reading the AFE4400
	// at the correct times. After each read, the data will be printed
	// to the serial interface (because we have debut turned on)
}
