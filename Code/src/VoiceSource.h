#pragma once
#include "Point3D.h"

class VoiceSource : public Point3D
{
public:
	//constructors / destructor
	VoiceSource();
	VoiceSource(double x, double y, double z);
	virtual ~VoiceSource();

	//Member functions
	virtual double leftVolume(Point3D userPosition);
	virtual double rightVolume(Point3D userPosition);
	virtual short nextDistortion(double amplitude);

	//getters / setters
	double getVoiceLevel();
	void setVoiceLevel(double level);

protected:
	//the loudness of the voice source
	//used to calculate how far the voice will carry
	//and how loud it will sound
	double voiceLevel;
	//modifies the rate at which sound degrades over distance
	const double voiceFalloffModifier = 1;
};

