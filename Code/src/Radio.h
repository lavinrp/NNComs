#pragma once
#include "VoiceSource.h"
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


protected:
	//member variables
	double frequency;
	double volume;
	bool on;
};

