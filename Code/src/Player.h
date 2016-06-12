#pragma once

#include<vector>


#include "VoiceSource.h"
#include "Radio.h"


using namespace std;

//type of players unique game ID
typedef unsigned long long GameID;
//Holds ID for player in TS
typedef unsigned __int16 anyID;

//The number of doubles that need to be passed by a game to create a full Player
const unsigned int DOUBLES_PER_PLAYER = DOUBLES_PER_VOICE_SOURCE + 2;

class Player :
	public VoiceSource {
public:
	//constructors / destructor
	Player();
	Player(double x, double y, double z);
	//No explicit copy constructor needed as a shallow copy is preferred here

	virtual ~Player();

	//getters / setters
	Radio* getRadio();
	void setRadio(Radio* radio);

	anyID getTsID();
	void setTsID(anyID id);

protected:
	//currently selected radio
	Radio* radio;
	//TS identification number
	anyID tsID;
};

