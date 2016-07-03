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
//TODO (Ryan Lavin): I think this comment says this function is from the wrong perspective. Ensure that userPosition actually is the position of this user - 6/13/2016

/*leftVolume
Calculates and returns the volume of this VoiceSource for the left ear
@param userPosition: the location of the user of this ts client
@return: volume modifier for left ear
*/
double VoiceSource::leftVolume(const Point3D& userPosition) {
	//E = (PI * A * B * D) / (2* c^2)
	//E = ans
	//A = voiceLevel
	//B = voiceFalloffModifier
	//D = Angle
	//C = distance
	double dist = this->distance(userPosition);
	//only calculate audio change if the point3Ds are not at the same location
	if (dist) {
		double temp = M_PI * voiceLevel * voiceFalloffModifier * this->angle2D(userPosition);
		return temp / (2 * dist * dist);
	}
	return 1;
}

//TODO (Ryan Lavin): I think this comment says this function is from the wrong perspective. Ensure that userPosition actually is the position of this user - 6/13/2016

/*rightVolume
Calculates and returns the volume of this VoiceSource for the right ear
@param userPosition: the location of the user of this ts client
@return: volume modifier for right ear
*/
double VoiceSource::rightVolume(const Point3D& userPosition) {
	//E = (A * B *(2 * pi - D)) / (2 * pi * C^2)
	//E = ans
	//A = voiceLevel
	//B = voiceFalloffModifier
	//D = Angle
	//C = distance
	double dist = this->distance(userPosition);
	
	//only calculate audio change if the point3Ds are not at the same location
	if (dist) {
		double angle = this->angle2D(userPosition);
		double temp = (voiceLevel * voiceFalloffModifier * ((2 * M_PI) - angle));
		return temp / (2 * M_PI * dist * dist);
	}
	return 1;
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
