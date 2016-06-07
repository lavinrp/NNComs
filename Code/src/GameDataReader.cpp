#include "ts3_functions.h"
#include "plugin_definitions.h"
#include "GameDataReader.h"


#pragma region Constructors / destructor

//default constructor
GameDataReader::GameDataReader(const TS3Functions ts3Functions, const uint64 serverConnectionHandlerID){
	setConnectedStatus(false);
	continueDataCollection = true;
	this->ts3Functions = ts3Functions;
	this->serverConnectionHandlerID = serverConnectionHandlerID;
	gameData = new GameData();
}

GameDataReader::~GameDataReader() {
	delete gameData;
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

	//initialize pipe variables
	//TODO(Ryan Lavin): make buffer of correct type - 5/28/2016
	double buffer[READER_BUFFER_SIZE];
	DWORD bytesRead = 0;

	while (true) {
		//Bring data in from pipe
		bool readResult = ReadFile(
			pipeHandle,				//pipe
			buffer,					//Write location
			//TODO (Ryan Lavin): make the following line size of GameData - 5/28/2016
			sizeof(DWORD),			//number of bytes to read
			&bytesRead,				//bytes read
			NULL);					//Overlapped
	
		//store data
		if (readResult) {
			//TODO (Ryan Lavin): store data - 5/28/2016
		} else {
			//bad read halt reading
			return;
		}
	}
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
	ts3Functions.setClientSelfVariableAsInt(serverConnectionHandlerID, CLIENT_META_DATA, buffer[0]);
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
reads the radios passed through the pipe. Memory allocated for radios created here is released by GameData
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

		//create radio and set its values
		Radio* readRadio = new Radio(buffer[0], buffer[1], buffer[2]);
		readRadio->setVoiceLevel(buffer[3]);
		readRadio->setFrequency(buffer[4]);
		readRadio->setVolume(buffer[5]);
		readRadio->setOn(buffer[6]);

		//add fully created radio to game data
		gameData->addRadio(readRadio);
	}
}

/*readPlayers
reads the players pass through the pipe. Memory allocated for players created here is released by GameData.
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
		//create player and set its values
		Player* readPlayer = new Player(buffer[0], buffer[1], buffer[2]);
		readPlayer->setVoiceLevel(buffer[3]);
		//get radio at passed index
		Radio* primaryRadio = gameData->getRadio((unsigned int)buffer[4]);
		readPlayer->setRadio(primaryRadio);
		//store the players in game id
		readPlayer->setGameID(buffer[5]);

		//add fully created player to game data
		gameData->addPlayer(readPlayer);
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