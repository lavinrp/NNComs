#pragma once

#include<vector>


#include "VoiceSource.h"
#include "Radio.h"

using namespace std;

//The number of doubles that need to be passed by a game to create a full Player
const unsigned int DOUBLES_PER_PLAYER = DOUBLES_PER_VOICE_SOURCE + 1;

class Player :
	public VoiceSource {
public:
	//constructors / destructor
	Player();
	Player(double x, double y, double z);

	virtual ~Player();

	//getters / setters
	Radio* getRadio();
	void setRadio(Radio* radio);

protected:
	//currently selected radio
	Radio* radio;
};

