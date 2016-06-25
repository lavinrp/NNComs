#include "NncToTs.h"


#pragma region Constructors / destructor

/*Default constructor for Nnc
Sets all pointers to nullptr
This should really never be used*/
NncToTs::NncToTs() : NncToTs (0){}

/*Constructor for NncToTs.
@param clientID: id of the client whose data will be retrieved*/
NncToTs::NncToTs(anyID clientID) {
	this->clientID = clientID;
	this->leftVolumes = nullptr;
	this->rightVolumes = nullptr;
	this->distortions = nullptr;
}

/*Constructor for NncToTs
Stores clientID and gameDataReader
@param clientID: id of the client whose data will be retrieved
@param GameDataReader: pointer to the plugin's gameDataReader.
*/
NncToTs::NncToTs(anyID clientID, GameDataReader* gameDataReader) : NncToTs(clientID){
	setGameDataReader(gameDataReader);
}

/*Constructor for NncToTs
sets ClientID, ClientGameID, and gameDataReader
@param clientID: id of the client whose data will be retrieved
@param gameID: the gameID of the client
@param GameDataReader: pointer to the plugin's gameDataReader.*/
NncToTs::NncToTs(anyID clientID, GameID gameID, GameDataReader* gameDataReader) : NncToTs(clientID, gameDataReader) {
	setClientGameID(gameID);
}

/*NncToTs Destructor. Frees all memory allocated in NncToTs. Will call delete
on nullptr if getnncSoundData was never called*/
NncToTs::~NncToTs() {
	delete[] leftVolumes;
	delete[] rightVolumes;
	delete[] distortions;
}
#pragma endregion

#pragma region Functions

/*getAudibleSources
Stores all voice sources that are audible to the selfPlayer
@param selfPlayer: current usersPlayer. Distances are compared to this player
@param otherPlayer: player whose voice will be checked for audibility
@param audibleSources: reference to a list of VoiceSources. This vector will be modified to hold
all audible voice sources and their distances.*/
void NncToTs::getAudibleSources(shared_ptr<Player> selfPlayer, shared_ptr<Player> otherPlayer, vector<shared_ptr<VoiceSource>>& audibleSources) {
	//get audible sources
	//check distance to the other player
	double distanceToPlayer = selfPlayer->distance(*otherPlayer);
	if (distanceToPlayer < maxAudibleDistance) {
		audibleSources.push_back(otherPlayer);
	}
	//get every nearby radio listening to the channel the other player is broadcasting on
	shared_ptr<Radio> activeRadio = otherPlayer->getRadio();
	if (activeRadio) {
		if (activeRadio->isBroadcasting()) {
			bool endOfRadios = false;
			unsigned int radioNum = 0;
			while (!endOfRadios) {
				shared_ptr<Radio> selectedRaio = gameDataReader->getRadio(radioNum); 
				if (selectedRaio) {
					double distanceToRadio = selfPlayer->distance(*selectedRaio);
					if (selectedRaio->getFrequency() == activeRadio->getFrequency() && distanceToRadio < maxAudibleDistance) {
						audibleSources.push_back(selectedRaio);
					}
					radioNum++;
				} else {
					endOfRadios = true;
				}
			}
		}
	}
}

/*getNNCPlayerData
stores all required player data for the given client ID from NNComs
@param clientID: id of the client whose voice will be changed
@param sources: The number of audio sources to emulate. The size of the leftVolumes, rightVolumes and distortions arrays
@param leftVolumes: array of volumes to be applied to the left headphone channel for each emulated audio source
@param rightVolumes: array of volumes to be applied to the right headphone channel for each emulated audio source
@param distortions: array of audio distortions to apply to each emulated audio source*/
void NncToTs::getNncSoundData() {
	//ensure that game data reader does not change values while they are being used in calculations
	while (!gameDataReader->gameDataMutex.try_lock()) {
		this_thread::sleep_for(THREAD_WAIT);
	}

	shared_ptr<Player> otherPlayer = gameDataReader->getPlayer(clientGameID);
	shared_ptr<Player> selfPlayer = gameDataReader->getPlayer(gameDataReader->getSelfGameID());

	//Find number of audible sources and their distances from the selfPlayer
	vector<shared_ptr<VoiceSource>> audibleSources;
	getAudibleSources(selfPlayer, otherPlayer, audibleSources);
	sources = (int)audibleSources.size();

	//only calculate sound modifications if there are any audible sources
	if (sources) {
		//Find left and right volumes for each source
		leftVolumes = new float[sources];
		rightVolumes = new float[sources];
		for (int i = 0; i < sources; i++) {
			//TODO (Ryan Lavin): the comments of left and right volume indicate that the following statements are backwards. I don't believe them. Check this -6/13/2016
			leftVolumes[i] = (float)selfPlayer->leftVolume(*audibleSources[i]);
			rightVolumes[i] = (float)selfPlayer->rightVolume(*audibleSources[i]);
		}

		//Find distortions of each source
		distortions = new short[sources];
		//TODO(Ryan Lavin): if this array is not initialized it creates cool static. Use this. - 5/22/2016
		for (int i = 0; i < sources; ++i) {
			distortions[i] = audibleSources[i]->nextDistortion(1);
		}
	}

	//allow game data reader to continue reading game data
	gameDataReader->gameDataMutex.unlock();
}

#pragma endregion

#pragma region Getters / Setters
anyID NncToTs::getClientID() {
	return this->clientID;
}

float* NncToTs::getLeftVolumes() {
	return this->leftVolumes;
}

float NncToTs::getLeftVolume(unsigned int position) {
	return this->leftVolumes[position];
}

float* NncToTs::getRightVolumes() {
	return this->rightVolumes;
}

float NncToTs::getRightVolume(unsigned int position) {
	return this->rightVolumes[position];
}

short* NncToTs::getDistortions() {
	return this->distortions;
}

short NncToTs::getDistortion(unsigned int position) {
	return this->distortions[position];
}

int NncToTs::getSources() {
	return this->sources;
}

/*setGameDataReader
determines where nnc to TS will get game data from
*/
void NncToTs::setGameDataReader(GameDataReader* gameDataReader) {
	this->gameDataReader = gameDataReader;
}

GameDataReader* NncToTs::getGameDataReader() {
	return this->gameDataReader;
}

GameID NncToTs::getClientGameID() {
	return clientGameID;
}

void NncToTs::setClientGameID(GameID clientGameID) {
	this->clientGameID = clientGameID;
}

#pragma endregion