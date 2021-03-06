#pragma once

#ifndef NNC_EXPORT
#define NNC_EXPORT __declspec(dllexport)
#endif // !NNC_EXPORT

#include <string>
#include <windows.h>
#include <iostream>
#include <mutex>
#include <thread>
#include <unordered_map>
#include <vector>
#include <memory>

#include "../include/teamspeak/public_definitions.h"
#include "../include/ts3_functions.h"

#include "UniversalConsts.h"
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
struct NNC_EXPORT VoiceSourceCounts {
	//number of radios in the next read
	UINT64 radioCount;
	//number of players in the next read
	UINT64 playerCount;
};

class NNC_EXPORT GameDataReader
{
public:
	//Constructors / destructor

	GameDataReader(const TS3Functions ts3Functions, const uint64 serverConnectionHandlerID);
	GameDataReader(const TS3Functions ts3Functions);


	virtual ~GameDataReader();

	//getters / setters
	bool isConnected();
	void setServerConnectionHandlerID(const uint64 serverConnectionHandlerID);

	void setContinueDataCollection(bool continueCollection);
	bool getContinueDataCollection();

	mutex gameDataMutex;

		//player
	shared_ptr<Player> getPlayer(GameID gameID);

		//radio
	shared_ptr<Radio> getRadio(unsigned int position);

		//TS
	void setPttStatus(bool status);


	int getSelfGameID();

	//Member functions
	void begin();

protected:
	//reading functions
	void collectGameData();

	bool connectToPipe();

	void readFromPipe();

	bool initializePlayer();

	VoiceSourceCounts readVoiceSourceCounts();

	bool readPlayers(const INT64 playerCount);

	bool readRadios(INT64 radioCount);

	//Member variables
	//gameData
	unordered_map <GameID, shared_ptr<Player>> players;
	vector<shared_ptr<Radio>> radios;
	int selfGameID;

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
	bool pttLastValue;
};

