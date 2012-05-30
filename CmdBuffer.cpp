#include "Arduino.h"  
#include "WString.h"

#include "CmdBuffer.h"

CmdBuffer::CmdBuffer() {
	maxArgs = MAX_ARGS_DEFAULT;
	reset();
}

void CmdBuffer::reset() {
	cmd = 0;
	cmdType = 0;
	cmdArg = "";
}

char CmdBuffer::getCmd() {
	return cmd;
}

char CmdBuffer::getCmdType() {
	return cmdType;
}

String CmdBuffer::getArg() {
	return cmdArg;
}

bool CmdBuffer::add(char c) {
	if (cmd > 0)
		// buffer already complete
		return 0;
	if (cmdType > 0)
		cmd = c;
	else if (isValidCmdType(c))
		cmdType = c;
	else if (cmdArg.length() >= maxArgs)
		return 0;
	else
		cmdArg += c;
	return 1;
}

bool CmdBuffer::isValidCmdType(char c) {
	String valid = "?*";
	return valid.indexOf(c) > -1;
}

bool CmdBuffer::isComplete() {
	return cmd > 0;
}
