#pragma once
#include "VoiceSource.h"

//The number of doubles that need to be passed by a game to create a full Radio
const unsigned int DOUBLES_PER_RADIO = DOUBLES_PER_VOICE_SOURCE + 4;

class Radio :
	public VoiceSource
{
public:
	//Constructors / destructor
	Radio();
	Radio(double x, double y, double z);
	Radio(double frequency);
	Radio(double x, double y, double z, double frequency);
	virtual ~Radio();

	//getters / setters
	double getFrequency();
	void setFrequency(double newFrequency);
	double getVolume();
	void setVolume(double newVolume);
	bool isOn();
	void setOn(bool on);
	bool isBroadcasting();
	void setBroadcasting(bool broadcasting);

	virtual short nextDistortion(double amplitude);

protected:
	//member variables
	double frequency;
	double volume;
	bool on;
	bool broadcasting;
};

