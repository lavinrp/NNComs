#pragma once

#ifndef NNC_EXPORT
#define NNC_EXPORT __declspec(dllexport)
#endif // !NNC_EXPORT

#include "VoiceSource.h"

//The number of doubles that need to be passed by a game to create a full Radio
const unsigned int DOUBLES_PER_RADIO = DOUBLES_PER_VOICE_SOURCE + 4;

class NNC_EXPORT Radio :
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


	//Do not initialize. Let undefined start value add to randomness.
	//determines the position of the square wave distortion.
	unsigned long long currentDistortion;
};

