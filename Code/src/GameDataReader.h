#pragma once
#include <string>
#include <windows.h>
#include <iostream>
#include <mutex>
#include <thread>
#include <unordered_map>
#include <vector>

#include "Player.h"
#include "Radio.h"

using namespace std;

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

	GameDataReader(const TS3Functions ts3Functions, const uint64 serverConnectionHandlerID);
	GameDataReader(const TS3Functions ts3Functions);


	virtual ~GameDataReader();

	//getters / setters
	bool isConnected();
	void setServerConnectionHandlerID(const uint64 serverConnectionHandlerID);

		//player
	Player* getPlayer(GameID gameID);

		//radio
	Radio* getRadio(unsigned int position);

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
	//gameData
	unordered_map <GameID, Player*> players;
	vector<Radio*> radios;

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

