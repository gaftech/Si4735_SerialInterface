/*
 * SerialInterface.h
 *
 *  Created on: 29 mai 2012
 *      Author: gabriel
 */

#ifndef SERIALINTERFACE_H_
#define SERIALINTERFACE_H_

// Ext libs

// Local libs
#include "CmdBuffer.h"
#include "Si4735.h"

// Constants
#define SI4735_MINFREQ 6000		// *10 kHz
#define SI4735_MAXFREQ 12800	// *10 kHz

class SerialInterface
{
	// private attributes
	private:
		CmdBuffer _buffer;
		bool _echo;
		bool _debug;
		int _linepos;
		Si4735 _driver;
		byte _response[16];
		Si4735_RX_Metrics _RSQ;

	// public methods
	public:
		SerialInterface();
		void initSerial(long baud);
		void initRadio(word freq);
		/* Input buffer */
		void update();
		/* Response */
		void debugln();
		void debugln(const char str[]);
		void debugln(const String &);
		void error();
		void success();

	// private methods
	private:
		/* buffer / response */
		char readChar();
		void clearBuf();
		void newline();
		/* command wrappers */
		bool command(char, char, const String &);
		bool get(char, const String &);
		bool set(char, const String &);
		/* radio setters */
		bool tune(word);
		bool seekup();
		/* radio getters */
		bool printFrequency();
		bool printRSSI();
		bool printRSQ();


};

#endif /* SERIALINTERFACE_H_ */
