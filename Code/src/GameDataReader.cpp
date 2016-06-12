#include "ts3_functions.h"
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
}

/*Constructor for game data reader. server connection handler 
defaults to 0 and must be set at a later time.
@param ts3Functions: pointer functions from the ts3 plugin SDK*/
GameDataReader::GameDataReader(const TS3Functions ts3Functions) : GameDataReader(ts3Functions, 0) {}

GameDataReader::~GameDataReader() {
	//free radio memory
	for (Radio* radio : radios) {
		delete radio;
	}
	//free player memory
	for (unordered_map<GameID, Player*>::iterator it = players.begin(); it != players.end(); it++) {
		delete it->second;
	}
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
		this_thread::sleep_for(0.01s);
	}
	//set and unlock connectedStatus
	connectedStatus = status;
	connectedStatusMutex.unlock();
}

/*getPlayer
finds the player with the passed gameID
@param gameID: in game ID of the player to find
@return: Return a pointer to the player with the passed gameID if one exists. Return nullptr otherwise.*/
Player* GameDataReader::getPlayer(GameID gameID) {
	unordered_map<GameID, Player*>::iterator playerIterator;
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
Radio* GameDataReader::getRadio(unsigned int position) {
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
Maintains game pipe connection.*/
void GameDataReader::readFromPipe() {

	//initialize this users in game ID and display in metadata
	initializePlayer();

	//find the number of each type of voice source to read
	VoiceSourceCounts voiceSourceCounts = readVoiceSourceCounts();

	//read and create players and radios
	readRadios(voiceSourceCounts.radioCount);
	readPlayers(voiceSourceCounts.playerCount);
}

/*initializePlayer
Sets current users Teamspeak metadata to the value passed by 
*/
void GameDataReader::initializePlayer() {
	//initialize pipe variables
	INT64 buffer[INIT_BUFFER_SIZE];
	DWORD bytesRead = 0;
	bool readResult = false;
	while (!readResult) {
		readResult = ReadFile(
			pipeHandle,				//pipe
			buffer,					//Write location
			sizeof(INT64),			//number of bytes to read
			&bytesRead,				//bytes read
			NULL);					//Overlapped
	}
	//set metadata to value read from pipe
	ts3Functions.setClientSelfVariableAsInt(serverConnectionHandlerID, CLIENT_META_DATA, (int)buffer[0]);
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
@param radioCount the number of radios that is expected to be sent by the pipe*/
void GameDataReader::readRadios(const INT64 radioCount) {
	for (unsigned int i = 0; i < radioCount; i++) {
		//read enough values to fill a radio
		double buffer[DOUBLES_PER_RADIO];
		DWORD bytesRead = 0;
		bool readResult = false;
		while (!readResult) {
			readResult = ReadFile(
				pipeHandle,								//pipe
				buffer,									//Write location
				DOUBLES_PER_RADIO * sizeof(double),		//number of bytes to read
				&bytesRead,								//bytes read
				NULL);									//Overlapped
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
		Radio* readRadio = getRadio(i);
		if (readRadio) {
			readRadio->setX(xpos);
			readRadio->setY(ypos);
			readRadio->setZ(zpos);
			readRadio->setFrequency(frequency);

		} else {
			//create radio and store it
			readRadio = new Radio(xpos, ypos, zpos, frequency);
			radios.push_back(readRadio);
		}
		readRadio->setVoiceLevel(voiceLevel);
		readRadio->setVolume(volume);
		readRadio->setOn(on);
		readRadio->setBroadcasting(broadcasting);
	}
}

/*readPlayers
reads the players pass through the pipe.
@param playerCount: number of players that is expected to be sent by the pipe*/
void GameDataReader::readPlayers(const INT64 playerCount) {
	for (unsigned int i = 0; i < playerCount; i++) {
		//read enough values to fill a player
		double buffer[DOUBLES_PER_PLAYER];
		DWORD bytesRead = 0;
		bool readResult = false;
		while (!readResult) {
			readResult = ReadFile(
				pipeHandle,								//pipe
				buffer,									//Write location
				DOUBLES_PER_PLAYER * sizeof(double),	//number of bytes to read
				&bytesRead,								//bytes read
				NULL);									//Overlapped
		}

		double xpos = buffer[0];
		double ypos = buffer[1];
		double zpos = buffer[2];
		double voiceLevel = buffer[3];
		unsigned int radioPosition = (unsigned int) buffer[4];
		GameID gameID = (GameID)buffer[5];

		//update or create player
		Player* readPlayer = getPlayer(gameID);
		if (readPlayer) {
			readPlayer->setX(xpos);
			readPlayer->setY(ypos);
			readPlayer->setZ(zpos);
			
		} else {
			//create and store player
			readPlayer = new Player(xpos, ypos, zpos);
			players.emplace(gameID, readPlayer);
		}
		readPlayer->setVoiceLevel(voiceLevel);
		Radio* selectedRadio = getRadio(radioPosition);
		readPlayer->setRadio(selectedRadio);
	}
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
	thread(collectGameData);
}
#pragma endregion