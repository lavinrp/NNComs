#pragma once
#include "Point3D.h"

class VoiceSource
{
public:
	//constructors / destructor
	VoiceSource();
	VoiceSource(const Point3D& position);
	virtual ~VoiceSource();

	//getters / setters
	Point3D getPosition();
	void setPosition(const Point3D& position);
private:
	Point3D position;


};

