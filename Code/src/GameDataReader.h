#pragma once
#include <string>
#include <windows.h>
#include <iostream>
#include <mutex>
#include <thread>

#include "GameData.h"

using namespace std;

//The number of values telling Game data reader how many player and radio doubles will be in each read
const unsigned int SIGNAL_VALUES_PER_READ = 2;

//signals if this read will be an initialization 0 is false anything else is true
const unsigned int INIT_FLAG = 1;

//buffer size for each read of GameDataReader
const unsigned int READER_BUFFER_SIZE = (MAX_RADIO_COUNT * DOUBLES_PER_RADIO +
	MAX_PLAYER_COUNT * DOUBLES_PER_PLAYER + SIGNAL_VALUES_PER_READ + INIT_FLAG) * 2;

class GameDataReader
{
public:
	//Constructors / destructor
	GameDataReader();
	virtual ~GameDataReader();

	//getters / setters
	bool isConnected();

	//Member functions
	void begin();

	void collectGameData();

	bool connectToPipe();

	void readFromPipe();

	void initializePlayers();

protected:
	//connection
	bool connectedStatus;
	void setConnectedStatus(bool status);
	//this mutex should only be used to lock use of connectedStatus
	mutex connectedStatusMutex;

	//Pipe things
	const LPTSTR pipeName = TEXT("\\\\.\\pipe\\gamePipe");
	bool continueDataCollection;
	HANDLE pipeHandle;
};

