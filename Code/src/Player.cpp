#include "Player.h"

//TODO(Ryan Lavin): make this match the UML - 5/23/2016

#pragma region Constructors / destructor

/*Default constructor
position: 0,0,0
voiceLevel: 0*/
Player::Player() : Player(0, 0, 0) {}


/*constructor
voiceLevel defaults to 0
@param x: x position of the player
@param y: y position of the player
@param z: z position of the player*/
Player::Player(double x, double y, double z) :
	VoiceSource(x, y, z) {
	primaryRadioLocation = 0;
}

Player::~Player() {
}

#pragma endregion

#pragma region Member Functions

/*addRadio
Adds a radio to the players radios vector.
@param radio: pointer to the players new radio.*/
void Player::addRadio(Radio* radio) {
	radios.push_back(radio);
}

/*removeRadio
Removes the radio at the specified position if it exists. Does nothing if there
is no radio at the specified position
@param position: position in the radios array to erase*/
void Player::removeRadio(unsigned int position) {
	if (radios.size() > position) { //if vector of size 2 can delete up to position 1 
		radios.erase(radios.begin() + position);
	}
}

/*replaceRadio
replaces the radio pointer at the specified position in Player.Radios with a passed radio pointer
@param position: position of the radio to replace
@param replacement: radio pointer to replace the previous */
void Player::replaceRadio(unsigned int position, Radio* replacement) {
	radios[position] = replacement;
}

#pragma endregion

#pragma region getters and settere

/*getPrimaryRadio
returns the index of the pointer to the players primary radio. If radios is empty
return a nullptr
*/
Radio* Player::getPrimaryRadio() {
	if (radios.size() > 0) {
		return radios[primaryRadioLocation];
	} else { 
		return nullptr; 
	}
}

/*selectPrimaryRadio
selects the radio at the passed position as the primary radio.
Position wraps back to beginning of radios if thee position does not exist.
If radios is empty position is set to 0

@param position: position of the new primary radio inside the Radios vector*/
void Player::selectPrimaryRadio(unsigned int position) {
	if (radios.size()) {
		primaryRadioLocation = position % radios.size();
	} else {
		primaryRadioLocation = 0;
	}
}

/*getRadioCount
returns the number of radios stored by the player*/
unsigned int Player::getRadioCount() {
	return radios.size();
}

/*getRadio
returns the radio at the specified position

@param position: position of the radio to retrieve
@return: the radio at the specified position in Player.Radios. If the requested position does
not exist returns nullptr*/
Radio* Player::getRadio(unsigned int position) {
	//ensure the position exists in radios before returning the value at position
	if (position < radios.size() && radios.size()) {
		return radios[position];
	} else {
		return nullptr;
	}
}

#pragma endregion

