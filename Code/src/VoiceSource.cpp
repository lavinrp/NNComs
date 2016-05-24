#include "VoiceSource.h"


#pragma region constructors / destructors
/*default constructor. Creates default Point3D. sets voiceLevel to 0.*/
VoiceSource::VoiceSource() : Point3D() {
	voiceLevel = 0;
}

/*Voice Source constructor
@param x: x position of the source
@param y: y position of the source
@param z: z position of the source*/
VoiceSource::VoiceSource(double x, double y, double z) : Point3D(x, y, z) {
	voiceLevel = 0;
}

VoiceSource::~VoiceSource() {
}

#pragma endregion

#pragma region getters / setters
double VoiceSource::getVoiceLevel() {
	return voiceLevel;
}

void VoiceSource::setVoiceLevel(double level) {
	voiceLevel = level;
}


#pragma endregion
