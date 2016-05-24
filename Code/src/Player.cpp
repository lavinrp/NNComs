#include "Player.h"

#pragma region Constructors / destructor

/*Default constructor
position: 0,0,0
voiceLevel: 0
hasRadio: false*/
Player::Player() {
	this->hasRadio = false;
}

/*constructor
voicLevel defaults to 0
@param x: x position of the player
@param y: y position of the player
@param z: z position of the player
@param hasRadio: true if the player has a radio*/
Player::Player(double x, double y, double z, bool hasRadio) : 
	VoiceSource(x, y, z) {
	this->hasRadio = hasRadio;
}

/*constructor
voiceLevel defaults to 0
HasRadio defaults to false
@param x: x position of the player
@param y: y position of the player
@param z: z position of the player*/
Player::Player(double x, double y, double z) :
	VoiceSource(x, y, z){
	this->hasRadio = false;
}

/*constructor
position defaults to 0,0,0
voiceLevel defaults to 0
@param x: x position of the player
@param y: y position of the player
@param z: z position of the player
@param level: voice level of the player
@param hasRadio: true if the player has a radio*/
Player::Player(bool hasRadio) {
	this->hasRadio = hasRadio;
}

Player::~Player() {
}
#pragma endregion

#pragma region getters and settere

/*@return: true if the player has a radio. false otherwise*/
bool Player::getHasRadio() const{
	return hasRadio;
}

/*sets the hasRadio property to the passed value
@param hasRadio: true if the player has a radio. false otherwise*/
void Player::setHasRadio(bool hasRadio) {
	this->hasRadio = hasRadio;
}

#pragma endregion
