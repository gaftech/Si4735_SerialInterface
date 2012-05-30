/*
 * Serial input buffer.
 * (c) Gabriel Fournier 2012
 */

#ifndef CmdBuffer_h
#define CmdBuffer_h

// arduino libs
#include "Arduino.h"
#include "WString.h"

#define MAX_ARGS_DEFAULT 10

class CmdBuffer
{
	public:
		// Constructs empty buffer
		CmdBuffer();
		// Simple getters
		char getCmd();
		char getCmdType();
		String getArg();
		// Add a received character to the buffer. Returns success status.
		bool add(char c);
		// Indicates if a command type character is valid
		bool isValidCmdType(char c);
		// Indicates if the buffer got required chargs
		bool isComplete();
		// Reset command and args
		void reset();
		
	private:
		// The command letter
		char cmd;
		// The command type ('*' => setter, '?' => getter)
		char cmdType;
		// Arguments
		String cmdArg;
		// Max argument length
		int maxArgs;
		
		
};



#endif
