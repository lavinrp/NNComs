#pragma once
#include "VoiceSource.h"

class Player :
	public VoiceSource
{
public:
	//constructors / destructor
	Player();
	Player(double level, bool hasRadio);
	Player(double level);
	Player(double x, double y, double z, double level, bool hasRadio);
	Player(double x, double y, double z, double level);
	Player(double x, double y, double z);
	Player(double x, double y, double z, bool hasRadio);
	Player(bool hasRadio);

	virtual ~Player();

	//getters and setters
	bool getHasRadio() const;
	void setHasRadio(bool hasRadio);

protected:
	bool hasRadio;
};

