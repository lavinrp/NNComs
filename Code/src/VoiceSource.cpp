#include "VoiceSource.h"


#pragma region constructors / destructors
VoiceSource::VoiceSource() {
	position = Point3D();
}

VoiceSource::VoiceSource(const Point3D& position) {
	this->position = position;
}

VoiceSource::~VoiceSource() {
}
#pragma endregion

#pragma region getters / setters

Point3D VoiceSource::getPosition() {
	return Point3D();
}

void VoiceSource::setPosition(const Point3D& position) {
	this->position = position;
}
#pragma endregion
