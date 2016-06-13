#pragma once

#include <tuple>

#include "GameDataReader.h"
#include "VoiceSource.h"
#include "Player.h"
#include "Radio.h"

typedef unsigned __int16 anyID;


class NncToTs
{
public:
	//constructors / destructor
	NncToTs();
	NncToTs(anyID clientID);
	NncToTs(anyID clientID, GameDataReader* gameDataReader);
	NncToTs(anyID clientID, GameID gameID, GameDataReader* gameDataReader);
	~NncToTs();

	//Member functions
	void getNncSoundData();
	bool isNncMuted();

	//getters and setters

	//client
	anyID getClientID();
	//gameID
	GameID getClientGameID();
	void setClientGameID(GameID clientGameID);
	//left volume
	float* getLeftVolumes();
	float getLeftVolume(unsigned int position);
	//right volume
	float* getRightVolumes();
	float getRightVolume(unsigned int position);
	//Distortions
	short* getDistortions();
	short getDistortion(unsigned int position);
	//Sources
	int getSources();
	//gameDataReader
	void setGameDataReader(GameDataReader* gameDataReader);
	GameDataReader* getGameDataReader();
	//max audible distance
	const double maxAudibleDistance = 500;



private:

	//functions
	void getAudibleSources(Player* selfPlayer, Player* otherPlayer, vector<VoiceSource*>& audibleSources);

	//member variables
	GameID clientGameID;
	anyID clientID;
	float* leftVolumes;
	float* rightVolumes;
	short* distortions;
	int sources;

	//variables for calculations
	GameDataReader* gameDataReader;


};
