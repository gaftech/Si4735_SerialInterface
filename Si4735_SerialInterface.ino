/*
* Si4735 Test
* (c) Gabriel Fournier 2012
*
*/

//Seen in csdexter SerialExample:
//Due to a bug in Arduino, these need to be included here too/first
#include <SPI.h>
#include <Wire.h>

#include "SerialInterface.h"

// Our serial interface
SerialInterface Siface;

void setup()
{
	Siface.initSerial(9600);
	Siface.debugln("Starting program");
	Siface.initRadio(9160);
	Siface.debugln("Program initialized");
}


void loop()
{ 
	Siface.update();
}


