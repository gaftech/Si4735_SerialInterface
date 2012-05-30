/*
 * SerialInterface.cpp
 *
 *  Created on: 29 mai 2012
 *      Author: gabriel
 */


// Arduino libs
#include <Arduino.h>
#include <WString.h>

// Avr libs

// Other external libs
#include <Si4735.h>

// Local libs
#include "SerialInterface.h"
#include "CmdBuffer.h"

SerialInterface::SerialInterface()
{
	_linepos = 0;
	_echo = true;
	_debug = true;
//	_buffer = CmdBuffer();
//	_driver = Si4735();
}

void SerialInterface::initSerial(long baud) {
	Serial.begin(baud);
}

void SerialInterface::initRadio(word freq) {
	_driver.begin(SI4735_MODE_FM);
	// Don't know why but a big delay seems necessary before first tuning after upload
	if (freq) {
		delay(500);
		tune(freq);
	}
}

void SerialInterface::update() {
	while (Serial.available()) {
		char inChar = readChar();
		if (inChar == '\r' || inChar == '\n') {
			clearBuf();
		} else if (! _buffer.add(inChar)){
			clearBuf();
			error();
		}
		if (_buffer.isComplete()) {
			bool ok = command(_buffer.getCmdType(), _buffer.getCmd(), _buffer.getArg());
			clearBuf();
			if (ok) {
				success();
			} else {
				error();
			}
		}
	}
}

bool SerialInterface::command(char cmdType, char cmd, const String &args) {
	if (_debug) {
		newline();
		Serial.print("Got command: ");
		Serial.print(args);
		Serial.print(cmdType);
		Serial.print(cmd);
		Serial.println();
	}
	bool success = true;
	switch (cmdType) {
	case '*':
		success = set(cmd, args);
		break;
	case '?':
		success = get(cmd, args);
		break;
	default:
		if (_debug) {
			newline();
			Serial.print("Invalid command type: ");
			Serial.println(cmdType);
		}
		success = false;
		break;
	}
	return success;
}

bool SerialInterface::get(char cmd, const String &args) {
	bool success = true;
	newline();
	switch (cmd) {
		case 'F':
			success = printFrequency();
			break;
		case 'R':
			success = printRSSI();
			break;
		case 'r':
			success = printRSQ();
			break;
		default:
			success = false;
			if (_debug) {
				Serial.print("Invalid get command: ");
				Serial.println(cmd);
			}
			break;
	}

	return success;
}

bool SerialInterface::set(char cmd, const String &args) {
	bool success = true;
	newline();
	switch (cmd)
	{
	/* Echo, Verbosity */
	case 'E':
		_echo = true;
		debugln("Echo ON");
		break;
	case 'e':
		_echo = false;
		debugln("Echo OFF");
		break;
	case 'D':
		_debug = true;
		debugln("Debug mode ON");
		break;
	case 'd':
		_debug = false;
		break;
	/* Tune */
	case 'F':
		success = tune(args.toInt());
		break;
	case '+':
		success = seekup();
		break;
	/* Audio */
	case 'm':
		_driver.mute();
		break;
	case 'M':
		_driver.unMute();
		break;
	default:
		if (_debug) {
			newline();
			Serial.print("Invalid set command: ");
			Serial.println(cmd);
		}
		success = false;
		break;
	}
	return success;
}

bool SerialInterface::tune(word freq) {
	if (freq < SI4735_MINFREQ || freq > SI4735_MAXFREQ) {
		if (_debug) {
			Serial.print("Invalid frequency: ");
			Serial.println(freq);
		}
		return 0;
	} else if (_debug) {
		Serial.print("Tuning... (x10 kHz): ");
		Serial.println(freq);
	}
	_driver.setFrequency(freq);
	if (_debug) {
		Serial.print("Tuned (x10 kHz): ");
		Serial.println(freq);
	}
	return 1;
}

bool SerialInterface::seekup() {
	_driver.seekUp();
	return 1;
}


bool SerialInterface::printFrequency() {
	word freq = _driver.getFrequency();
	Serial.println(freq);
	return freq >= SI4735_MINFREQ || freq <= SI4735_MAXFREQ;
}

bool SerialInterface::printRSSI() {
	_driver.getRSQ(&_RSQ);
	Serial.println(_RSQ.RSSI);
	return 1;
}

bool SerialInterface::printRSQ(){
	_driver.getRSQ(&_RSQ);
	if (_debug) {
		Serial.println(F("Signal quality metrics {"));
		Serial.print(F("RSSI = "));			Serial.print(_RSQ.RSSI);	Serial.println(" dBuV");
		Serial.print("SNR = ");				Serial.print(_RSQ.SNR);		Serial.println(" dB");
		Serial.print(F("Multipath = "));	Serial.print(_RSQ.MULT);	Serial.println(" %");
		if(_driver.getMode() == SI4735_MODE_FM) {
		  Serial.println((_RSQ.PILOT ? "Stereo" : "Mono"));
		  Serial.print(F("Stereo blend = "));	Serial.print(_RSQ.STBLEND);	Serial.println(" %");
		  Serial.print(F("freq Offset = "));	Serial.print(_RSQ.FREQOFF);	Serial.println(" kHz");
		}
		Serial.println("}");
	} else {
		Serial.print(F("RSSI:"));	Serial.println(_RSQ.RSSI);
		Serial.print(F("SNR:"));	Serial.println(_RSQ.SNR);
		Serial.print(F("MULT:"));	Serial.println(_RSQ.MULT);
		if(_driver.getMode() == SI4735_MODE_FM) {
			Serial.print(F("STEREO:"));		Serial.println(_RSQ.PILOT);
			Serial.print(F("STBLEND:"));	Serial.println(_RSQ.STBLEND);
			Serial.print(F("OFFSET:"));		Serial.println(_RSQ.FREQOFF);
		}
	}
	return 1;
}

void SerialInterface::clearBuf() {
	_buffer.reset();
	debugln("Buffer cleared");
}

char SerialInterface::readChar() {
	char c = (char) Serial.read();
	if (_echo) {
		Serial.print(c);
		_linepos++;
	}
	return c;
}

void SerialInterface::newline() {
	if (_linepos > 0) {
		Serial.println();
		_linepos = 0;
	}
}

void SerialInterface::debugln() {
	if (_debug) {
		newline();
		Serial.println();
	}
}

void SerialInterface::debugln(const char str[]) {
	if (_debug) {
		newline();
		Serial.println(str);
	}
}

void SerialInterface::debugln(const String &str) {
	if (_debug) {
		newline();
		Serial.println(str);
	}
}

void SerialInterface::error() {
	newline();
	Serial.println("ERROR");
}

void SerialInterface::success() {
	newline();
	Serial.println("OK");
}

