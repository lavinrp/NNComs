#pragma once
#include "Point3D.h"

class VoiceSource : public Point3D
{
public:
	//constructors / destructor
	VoiceSource();
	VoiceSource(double level);
	VoiceSource(double x, double y, double z, double level);
	VoiceSource(double x, double y, double z);
	virtual ~VoiceSource();

	//getters / setters
	double getVoiceLevel();
	void setVoiceLevel(double level);

protected:
	double voiceLevel;

};

