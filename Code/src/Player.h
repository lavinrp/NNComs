#pragma once
#include "VoiceSource.h"

class Player :
	public VoiceSource
{
public:
	//constructors / destructor
	Player();
	Player(double x, double y, double z);

	virtual ~Player();
};

