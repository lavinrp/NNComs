#include "NncToTs.h"


#pragma region Constructors / destructor

/*Default constructor for Nnc
Sets all pointers to nullptr
This should really never be used*/
NncToTs::NncToTs() {
	this->clientID = 0;
	this->leftVolumes = nullptr;
	this->rightVolumes = nullptr;
	this->distortions = nullptr;
}

/*Constructor for NncToTs. Retrieves all relevant data for passed clientID.
@param clientID: id of the client to retrieve data for*/
NncToTs::NncToTs(anyID clientID) {
	this->clientID = clientID;
	this->getNncSoundData();
}

/*NncToTs Destructor. Frees all memory allocated in NncToTs. Will call delete
on nullptr if default constructor was used and getnncSoundData was never called*/
NncToTs::~NncToTs() {
	delete[] leftVolumes;
	delete[] rightVolumes;
	delete[] distortions;
}
#pragma endregion

#pragma region Functions

/*getNNCPlayerData
returns all required player data for the given client ID from NNComs
@param clientID: id of the client whose voice will be changed
@param sources: The number of audio sources to emulate. The size of the leftVolumes, rightVolumes and distortions arrays
@param leftVolumes: array of volumes to be applied to the left headphone channel for each emulated audio source
@param rightVolumes: array of volumes to be applied to the right headphone channel for each emulated audio source
@param distortions: array of audio distortions to apply to each emulated audio source*/
void NncToTs::getNncSoundData() {

	//Find number of audio sources
	sources = 1;

	//Find left and right volumes for each source
	leftVolumes = new float[sources];
	leftVolumes[0] = 0.3f;

	rightVolumes = new float[sources];
	rightVolumes[0] = 0;

	//Find distortions of each source
	distortions = new short[sources];
	//TODO(Ryan Lavin): if this array is not initialized it creates cool static. Use this. - 5/22/2016
	distortions[0] = 0;
}

/*isNncMuted
Mutes the client if it should be muted. Returns true if the client is muted. Returns false otherwise
@param clientID: the ID of the client whose muted status is being checked
@return: true if the client is muted by NNC false otherwise*/
bool NncToTs::isNncMuted() {
	//TODO: fill isNncMuted stub
	return false;
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

#pragma endregion