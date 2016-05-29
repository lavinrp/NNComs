#include "GameDataReader.h"


#pragma region Constructors / destructor

//default constructor
GameDataReader::GameDataReader(){
	
}

GameDataReader::~GameDataReader() {}
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
	//initialize pipe variables
	//TODO(Ryan Lavin): make buffer of correct type - 5/28/2016
	DWORD buffer[READER_BUFFER_SIZE];
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
 
/*collectGameData
Searches for and maintains connection to game pipe.
Stores data from game pipe.
Thread safe.*/
void GameDataReader::collectGameData() {

	while (true) {
		//Connect to the game pipe
		//will go on indefinitely until connection is established or an error occurs
		bool connection = connectToPipe();
		setConnectedStatus(connection);

		//read from pipe untill bad read
		readFromPipe();
		//set connectedStatus and begin connection process again after bad read
		setConnectedStatus(false);
	}
}

/*begin
starts thread in charge of pipe connection and reading*/
void GameDataReader::begin() {
	thread (collectGameData);
}
#pragma endregion