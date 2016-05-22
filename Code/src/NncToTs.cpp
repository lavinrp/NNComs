#include "NncToTs.h"


/*getNNCPlayerData
returns all required player data for the given client ID from NNComs
@param clientID: id of the client whose voice will be changed
@param sources: The number of audio sources to emulate. The size of the leftVolumes, rightVolumes and distortions arrays
@param leftVolumes: array of volumes to be applied to the left headphone channel for each emulated audio source
@param rightVolumes: array of volumes to be applied to the right headphone channel for each emulated audio source
@param distortions: array of audio distortions to apply to each emulated audio source*/
void getNncSoundData(anyID clientID, int& sources, float* leftVolumes, float* rightVolumes, short* distortions) {
	//TODO: fill getNncSoundData stub

	sources = 1;
	/*leftVolumes = new float[sources];
	rightVolumes = new float[sources];
	distortions = new short[sources];*/

	leftVolumes[0] = 1;
	rightVolumes[0] = 0.2;
	distortions[0] = 0;

	return;
}

/*isNncMuted
Mutes the client if it should be muted. Returns true if the client is muted. Returns false otherwise
@param clientID: the ID of the client whose muted status is being checked
@return: true if the client is muted by NNC false otherwise*/
bool isNncMuted(anyID clientID) {
	//TODO: fill isNncMuted stub
	return false;
}
