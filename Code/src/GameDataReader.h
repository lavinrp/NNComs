#pragma once
#include <string>
#include <windows.h>
#include <iostream>
#include <mutex>
#include<thread>

using namespace std;

//buffer size for each read of GameDataReader
const unsigned int READER_BUFFER_SIZE = 128;//TODO: change this to 2 X the size of the GameData

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

protected:
	//connection
	bool connectedStatus;
	void setConnectedStatus(bool status);
	//this mutex should only be used to lock use of connectedStatus
	mutex connectedStatusMutex;

	//Pipe things
	const LPTSTR pipeName = TEXT("\\\\.\\pipe\\gamePipe");
	HANDLE pipeHandle;
};

