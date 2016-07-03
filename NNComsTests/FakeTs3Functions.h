#pragma once
#include "../Code/include/teamspeak/clientlib_publicdefinitions.h"
#include "../Code/include/plugin_definitions.h"
#include "../Code/include/ts3_functions.h"
#include "../Code//include/teamspeak/public_errors.h"




unsigned int setClientSelfVariableAsInt(uint64 serverConnectionHandlerID,
	size_t flag, int value) {
	return 0;

}

unsigned int flushClientSelfUpdates(uint64 serverConnectionHandlerID, const char* returnCode) {
	return 0;
}


unsigned int getClientID(uint64 serverConnectionHandlerID, anyID* result) {
	return 1;
}

unsigned int getErrorMessage (unsigned int errorCode, char** error) {
	return ERROR_ok;
}

unsigned int logMessage(const char* logMessage, enum LogLevel severity,
	const char* channel, uint64 logID) {
	return 0;
}


unsigned int freeMemory(void* pointer) {
	return 0;
}

TS3Functions GetTs3Functions() {
	TS3Functions fakeTs3Functions;

	fakeTs3Functions.setClientSelfVariableAsInt = setClientSelfVariableAsInt;
	fakeTs3Functions.flushClientSelfUpdates = flushClientSelfUpdates;
	fakeTs3Functions.getClientID = getClientID;
	fakeTs3Functions.getErrorMessage = getErrorMessage;
	fakeTs3Functions.logMessage = logMessage;
	fakeTs3Functions.freeMemory = freeMemory;

	return fakeTs3Functions;
}