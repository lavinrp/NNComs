#define _USE_MATH_DEFINES
#include <math.h>
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

#pragma region Member Functions
/*leftVolume
Calculates and returns the volume of this VoiceSource for the left ear
@param userPosition: the location of the user of this ts client
@return: volume modifier for left ear
*/
double VoiceSource::leftVolume(Point3D userPosition) {
	//E = (PI * A * B * D) / (2* c^2)
	//E = ans
	//A = voiceLevel
	//B = voiceFalloffModifier
	//D = Angle
	//C = distance
	double dist = this->distance(userPosition);
	double temp = M_PI * voiceLevel * voiceFalloffModifier * this->angle2D(userPosition);
	return temp / (2 * dist * dist);
}

/*rightVolume
Calculates and returns the volume of this VoiceSource for the right ear
@param userPosition: the location of the user of this ts client
@return: volume modifier for right ear
*/
double VoiceSource::rightVolume(Point3D userPosition) {
	//E = (A * B *(2 * pi - D)) / (2 * pi * C^2)
	//E = ans
	//A = voiceLevel
	//B = voiceFalloffModifier
	//D = Angle
	//C = distance
	double angle = this->angle2D(userPosition);
	double dist = this->distance(userPosition);
	double temp = (voiceLevel * voiceFalloffModifier * ((2 * M_PI) - angle));
	return temp / (2 * M_PI * dist * dist);
}

/*nextDistortion
Calculates and returns the volume of this VoiceSource for the left ear
@param amplitude: Amplitude modifier for distortion. Larger amplitude -> larger distortion
@reutrn: scaled value of next distortion*/
short VoiceSource::nextDistortion(double amplitude) {
	return 0;
}
#pragma endregion

#pragma region getters / setters

/*getVoiceLevel
returns the loudness of the voice source
@return: voiceLevel*/
double VoiceSource::getVoiceLevel() {
	return voiceLevel;
}

/*setVoiceLevel
sets the loudness of this voice source
@param level: the new loudness level of the voice*/
void VoiceSource::setVoiceLevel(double level) {
	voiceLevel = level;
}
#pragma endregion
