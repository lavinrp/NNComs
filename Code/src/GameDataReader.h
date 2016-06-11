#pragma once
#include <string>
#include <windows.h>
#include <iostream>
#include <mutex>
#include <thread>
#include <map>

#include "GameData.h"

using namespace std;

//The number of values telling Game data reader how many player and radio doubles will be in each read
const unsigned int SIGNAL_VALUES_PER_READ = 2;

//Number of values used to flag initialization. 
const unsigned int INIT_FLAG = 1;

//buffer size for each read of GameDataReader
const unsigned int READER_BUFFER_SIZE = (MAX_RADIO_COUNT * DOUBLES_PER_RADIO +
	MAX_PLAYER_COUNT * DOUBLES_PER_PLAYER + SIGNAL_VALUES_PER_READ + INIT_FLAG) * 2;

//buffer size for initialization
const unsigned int INIT_BUFFER_SIZE = 1;

//buffer size for VoiceSourceCounts
const unsigned int VOICE_SOURCE_COUNT_BUFFER_SEIZE = 2;

/*VoiceSourceCounts
Stores the number of radios and players that will be in
a particular read of GameDataReader*/
struct VoiceSourceCounts {
	//number of radios in the next read
	INT64 radioCount;
	//number of players in the next read
	INT64 playerCount;
};

class GameDataReader
{
public:
	//Constructors / destructor

	/*Only constructor for GameDataReader
	initializes GameDataReader values. TS functions must be passed to allow for initialization
	of user
	@param ts3Functions: pointer functions from the ts3 plugin SDK
	@param serverConnectionHandlerID: id of the server the user is connected to*/
	GameDataReader(const TS3Functions ts3Functions, const uint64 serverConnectionHandlerID);
	virtual ~GameDataReader();

	//getters / setters
	bool isConnected();
	GameData* getGameData();

	//Member functions
	void begin();

protected:
	//reading functions
	void collectGameData();

	bool connectToPipe();

	void readFromPipe();

	void initializePlayer();

	VoiceSourceCounts readVoiceSourceCounts();

	void readPlayers(const INT64 playerCount);

	void readRadios(INT64 radioCount);

	//Member variables
	GameData* gameData;
	mutex gameDataMutex;

	//connection
	bool connectedStatus;
	void setConnectedStatus(bool status);
	//this mutex should only be used to lock use of connectedStatus
	mutex connectedStatusMutex;

	//Pipe things
	const LPTSTR pipeName = TEXT("\\\\.\\pipe\\gamePipe");
	bool continueDataCollection;
	HANDLE pipeHandle;

	//TS3 stuff
	TS3Functions ts3Functions;
	uint64 serverConnectionHandlerID;
};

