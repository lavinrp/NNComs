#include "ts3_functions.h"
#include "teamspeak/public_errors.h"
#include "plugin_definitions.h"
#include "GameDataReader.h"


#pragma region Constructors / destructor


/*
initializes GameDataReader values.TS functions must be passed to allow for initialization
of user
@param ts3Functions: pointer functions from the ts3 plugin SDK
@param serverConnectionHandlerID : id of the server the user is connected to*/
GameDataReader::GameDataReader(const TS3Functions ts3Functions, const uint64 serverConnectionHandlerID){
	setConnectedStatus(false);
	continueDataCollection = true;
	this->ts3Functions = ts3Functions;
	this->serverConnectionHandlerID = serverConnectionHandlerID;
	pttLastValue = false;
}

/*Constructor for game data reader. server connection handler 
defaults to 0 and must be set at a later time.
@param ts3Functions: pointer functions from the ts3 plugin SDK*/
GameDataReader::GameDataReader(const TS3Functions ts3Functions) : GameDataReader(ts3Functions, 0) {}

GameDataReader::~GameDataReader() {
	//radio memory automatically freed
	//player memory automatically freed
}
#pragma endregion

#pragma region Getters / setters

/*Returns true if a pipe has connected to the specified location.
Returns false otherwise*/
bool GameDataReader::isConnected() {
	return connectedStatus;
}

/*setConnectedStatus

!!!!!!!PROTECTED FUNCTION!!!!!!!!

Thread safe way to set connectedStatus
@param status: set to true if the connected to a pipe
set to false otherwise*/
void GameDataReader::setConnectedStatus(bool status) {
	//wait until the connected status can be modified
	while (!connectedStatusMutex.try_lock()) {
		this_thread::sleep_for(THREAD_WAIT);
	}
	//set and unlock connectedStatus
	connectedStatus = status;
	connectedStatusMutex.unlock();
}

/*getPlayer
finds the player with the passed gameID
@param gameID: in game ID of the player to find
@return: Return a shared_ptr to the player with the passed gameID if one exists. Return nullptr otherwise.*/
shared_ptr<Player> GameDataReader::getPlayer(GameID gameID) {
	unordered_map<GameID, shared_ptr<Player>>::iterator playerIterator;
	playerIterator = players.find(gameID);

	if ( playerIterator != players.end() ) {
		return playerIterator->second;
	} else {
		return nullptr;
	}
}

/*getRadio
returns the requested radio
@param position: the position of the radio to return
@return: Pointer to the radio at the passed position in GameDataReaders radio vector. nullptr if
the requested radio does not exist.*/
shared_ptr<Radio> GameDataReader::getRadio(unsigned int position) {
	if (radios.size() > position) {
		return radios[position];
	} else {
		return nullptr;
	}
}

/*setServerConnectionHandlerID
sets the serverConnectionHandlerID
@param serverConnectionHandlerID: new serverConnectionHandlerID*/
void GameDataReader::setServerConnectionHandlerID(const uint64 serverConnectionHandlerID) {
	this->serverConnectionHandlerID = serverConnectionHandlerID;
}

/*getSelfGameID
returns the gameID of the selfClient*/
int GameDataReader::getSelfGameID() {
	return selfGameID;
}

/*setPttStatus
Toggles the TS INPUT_ACTIVE property. Should be used to turn on self client microphone
in conjunction with in game radio broadcasts. Does nothing if value is set to previous value.
sets pttLastValue to the passed status.
@param status: true to toggle talking on. False to toggle talking off.*/
void GameDataReader::setPttStatus(bool status) {
	if (status != pttLastValue) {
		//get myID
		anyID myID;
		if (ts3Functions.getClientID(serverConnectionHandlerID, &myID) != ERROR_ok) {
			//handle error
			ts3Functions.logMessage("Error querying own client id", LogLevel_ERROR, "Plugin", serverConnectionHandlerID);
			return;
		}

		//set players talking status to the passed value
		unsigned int error;
		bool shouldTalk;
		shouldTalk = status; 
		if ((error = ts3Functions.setClientSelfVariableAsInt(myID, CLIENT_INPUT_DEACTIVATED,
			shouldTalk ? INPUT_ACTIVE : INPUT_DEACTIVATED))
			!= ERROR_ok) {
			//handle error
			char* errorMsg;
			if (ts3Functions.getErrorMessage(error, &errorMsg) != ERROR_ok) {
				ts3Functions.logMessage(errorMsg, LogLevel_ERROR, "Plugin", serverConnectionHandlerID);

				ts3Functions.freeMemory(errorMsg);
			}
			return;
		}

		//update other users
		if (ts3Functions.flushClientSelfUpdates(myID, NULL) != ERROR_ok) {
			//handle error
			char* errorMsg;
			if (ts3Functions.getErrorMessage(error, &errorMsg) != ERROR_ok) {
				ts3Functions.logMessage(errorMsg, LogLevel_ERROR, "Plugin", serverConnectionHandlerID);
				ts3Functions.freeMemory(errorMsg);
			}
		}

		pttLastValue = status;
	}
}
#pragma endregion

#pragma region Member Functions

/*connectToPipe
connects pipeHandle to the pipe with a name defined by pipeName
@return: true if the connection is successful. Returns false otherwise.*/
bool GameDataReader::connectToPipe() {

	//create file
	pipeHandle = CreateFile(
		pipeName,
		GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	while (true) {
		//pipe found break connection loop
		if (pipeHandle != INVALID_HANDLE_VALUE) {
			break;
		}
		//unexpected pipe error
		if (GetLastError() != ERROR_PIPE_BUSY) {
			return false;
		}
	}

	//If pipe is somehow still invalid throw error
	if (pipeHandle == INVALID_HANDLE_VALUE) {
		return false;
	}
	return true;
}

/*readFromPipe
Reads data from game pipe.
Stores game pipe data.
Only exits on bad read.*/
void GameDataReader::readFromPipe() {

	//initialize this users in game ID and display in metadata
	bool goodData = initializePlayer();

	//continually read from pipe
	while (goodData) {
		//find the number of each type of voice source to read
		VoiceSourceCounts voiceSourceCounts = readVoiceSourceCounts();

		//read and create players and radios
		while (!gameDataMutex.try_lock() ){
			this_thread::sleep_for(THREAD_WAIT);
		}
		bool radioSuccess = readRadios(voiceSourceCounts.radioCount);
		bool playerSuccess = readPlayers(voiceSourceCounts.playerCount);
		gameDataMutex.unlock();

		goodData = radioSuccess && playerSuccess;
	}
}

/*initializePlayer
Sets current users Teamspeak metadata to the value passed by
@return: true if data successfully set. False otherwise.
*/
bool GameDataReader::initializePlayer() {
	//initialize pipe variables
	INT64 buffer[INIT_BUFFER_SIZE];
	DWORD bytesRead = 0;
	bool readResult = false;
		readResult = ReadFile(
			pipeHandle,				//pipe
			buffer,					//Write location
			sizeof(INT64),			//number of bytes to read
			&bytesRead,				//bytes read
			NULL);					//Overlapped

		//return on bad read
		if (!readResult) {
			return false;
		}

	//set metadata to value read from pipe
	ts3Functions.setClientSelfVariableAsInt(serverConnectionHandlerID, CLIENT_META_DATA, (int)buffer[0]);
	ts3Functions.flushClientSelfUpdates(serverConnectionHandlerID, NULL);
	selfGameID = (int)buffer[0];

	//good read. All data set.
	return true;
}

/*readVoiceSourceCounts
reads data from the pipe to determine how many of each VoiceSource will be in the next read
@return: VoiceSourceCounts struct holding the number of each voiceSource that will
be in the next read*/
VoiceSourceCounts GameDataReader::readVoiceSourceCounts() {
	//read enough values to fill VoiceSourceCounts
	INT64 buffer[VOICE_SOURCE_COUNT_BUFFER_SEIZE];
	DWORD bytesRead = 0;
	bool readResult = false;
	while (!readResult) {
		readResult = ReadFile(
			pipeHandle,					//pipe
			buffer,						//Write location
			sizeof(VoiceSourceCounts),	//number of bytes to read
			&bytesRead,					//bytes read
			NULL);						//Overlapped
	}

	//Create and return a VoiceSourceCounts with the data from the pipe
	VoiceSourceCounts voiceSourceCounts;
	voiceSourceCounts.playerCount = buffer[0];
	voiceSourceCounts.radioCount = buffer[1];

	return voiceSourceCounts;
}

/*readRadio
reads the radios passed through the pipe.
@param radioCount the number of radios that is expected to be sent by the pipe
@return: true if all reads and operations successful. False otherwise. */
bool GameDataReader::readRadios(const INT64 radioCount) {
	for (unsigned int i = 0; i < radioCount; i++) {
		//read enough values to fill a radio
		double buffer[DOUBLES_PER_RADIO];
		DWORD bytesRead = 0;
		bool readResult = false;
		readResult = ReadFile(
			pipeHandle,								//pipe
			buffer,									//Write location
			DOUBLES_PER_RADIO * sizeof(double),		//number of bytes to read
			&bytesRead,								//bytes read
			NULL);									//Overlapped

		//return with false on bad read
		if (!readResult) {
			return false;
		}

		//values from pipe
		double xpos = buffer[0];
		double ypos = buffer[1];
		double zpos = buffer[2];
		double voiceLevel = buffer[3];
		double frequency = buffer[4];
		double volume = buffer[5];
		bool on = (bool)buffer[6];
		bool broadcasting = (bool)buffer[7];

		//update or create radio
		shared_ptr<Radio> readRadio = getRadio(i);
		if (readRadio) {
			readRadio->setX(xpos);
			readRadio->setY(ypos);
			readRadio->setZ(zpos);
			readRadio->setFrequency(frequency);

		} else {
			//create radio and store it
			readRadio = make_shared<Radio>(xpos, ypos, zpos, frequency);
			radios.push_back(readRadio);
		}
		readRadio->setVoiceLevel(voiceLevel);
		readRadio->setVolume(volume);
		readRadio->setOn(on);
		readRadio->setBroadcasting(broadcasting);
	}

	//crop radios vector to fit new number of radios
	if (radioCount < radios.size()) {
		//erase radios at position radioCount to end
		radios.erase(radios.begin() + radioCount, radios.end());
	}

	//return true if all reads and operations successful
	return true;
}

/*readPlayers
reads the players pass through the pipe.
@param playerCount: number of players that is expected to be sent by the pipe
@return: true if successful with all reads. False otherwise.*/
bool GameDataReader::readPlayers(const INT64 playerCount) {
	for (unsigned int i = 0; i < playerCount; i++) {
		//read enough values to fill a player
		double buffer[DOUBLES_PER_PLAYER];
		DWORD bytesRead = 0;
		bool readResult = false;
		readResult = ReadFile(
			pipeHandle,								//pipe
			buffer,									//Write location
			DOUBLES_PER_PLAYER * sizeof(double),	//number of bytes to read
			&bytesRead,								//bytes read
			NULL);									//Overlapped

		//return if bad read
		if (!readResult) {
			return false;
		}

		double xpos = buffer[0];
		double ypos = buffer[1];
		double zpos = buffer[2];
		double voiceLevel = buffer[3];
		unsigned int radioPosition = (unsigned int) buffer[4];
		GameID gameID = (GameID)buffer[5];

		//update or create player
		shared_ptr<Player> readPlayer = getPlayer(gameID);
		if (readPlayer) {
			readPlayer->setX(xpos);
			readPlayer->setY(ypos);
			readPlayer->setZ(zpos);
			
		} else {
			//create and store player
			readPlayer = make_shared<Player>(xpos, ypos, zpos);
			players.emplace(gameID, readPlayer);
		}
		readPlayer->setVoiceLevel(voiceLevel);
		shared_ptr<Radio> selectedRadio = getRadio(radioPosition);
		readPlayer->setRadio(selectedRadio);
	}

	//Handel radio push to talk for self player
	shared_ptr<Radio> selfPlayersRadio = players[selfGameID]->getRadio();
	if (selfPlayersRadio) {
		setPttStatus(selfPlayersRadio->isBroadcasting());
	} else {
		setPttStatus(false);
	}


	//return true if all reads good
	return true;
}

/*collectGameData
Searches for and maintains connection to game pipe.
Stores data from game pipe.
Thread safe.*/
void GameDataReader::collectGameData() {

	while (continueDataCollection) {
		//Connect to the game pipe
		//will go on indefinitely until connection is established or an error occurs
		bool connection = connectToPipe();
		setConnectedStatus(connection);

		//read from pipe until bad read
		readFromPipe();
		//set connectedStatus and begin connection process again after bad read
		setConnectedStatus(false);
	}
}

/*begin
starts thread in charge of pipe connection and reading*/
void GameDataReader::begin() {
		thread readingThread(&GameDataReader::collectGameData, this);
		readingThread.detach();
}
#pragma endregion