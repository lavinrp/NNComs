#include "Radio.h"


#pragma region Constructors / destructor

/*default constructor for Radio.
All values initialized to 1 or true
position is 0,0,0*/
Radio::Radio() : Radio(0, 0, 0) {

}

/*Constructor for Radio
frequency and volume set to 1.
on set to true

@param x: x position of radio
@param y: y position of radio
@param z: z position of radio
*/
Radio::Radio(double x, double y, double z) : Radio(x, y, z, 1){}

/*Constructor for Radio
position at 0,0,0 on set to true

@param frequency: frequency of the radio
*/
Radio::Radio(double frequency) : Radio(0, 0, 0, frequency) {}

/*Constructor for Radio
defaults to on with a volume of one

@param x: x position of radio
@param y: y position of radio
@param z: z position of radio
@param frequency: frequency of radio*/
Radio::Radio(double x, double y, double z, double frequency) : VoiceSource(x, y, z) {
	this->frequency = frequency;
	on = true;
	volume = 1;
}

Radio::~Radio() {
}
#pragma endregion

#pragma region getters / setters

/*returns the frequency value of this radio*/
double Radio::getFrequency() {
	return frequency;
}

/*sets the frequency value of this radio
@param newFrequency: the new frequency value of this radio*/
void Radio::setFrequency(double newFrequency) {
	frequency = newFrequency;
}

/*returns the volume of this radio*/
double Radio::getVolume() {
	return volume;
}

/*sets the volume of this radio
@param newVolume: the new volume for the radio*/
void Radio::setVolume(double newVolume) {
	volume = newVolume;
}

/*returns true if the radio is on. Returns false otherwise*/
bool Radio::isOn() {
	return on;
}

/*sets the on property for the radio
@param on: true sets the radio on. false sets the radio off*/
void Radio::setOn(bool on) {
	this->on = on;
}
#pragma endregion