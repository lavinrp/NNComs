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
@param x: x position of the player
@param y: y position of the player
@param z: z position of the player
@param level: voice level of the player
@param hasRadio: true if the player has a radio*/
Player::Player(double x, double y, double z, double level, bool hasRadio) : 
	VoiceSource(x, y, z, level) {
	this->hasRadio = hasRadio;
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
hasRadio defaults to false
@param x: x position of the player
@param y: y position of the player
@param z: z position of the player
@param level: voice level of the player*/
Player::Player(double x, double y, double z, double level) :
	VoiceSource(x, y, z, level) {
	this->hasRadio = false;
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
@param level: voice level of the player
@param hasRadio: true if the player has a radio*/
Player::Player(double level, bool hasRadio) :
	VoiceSource (level){
	this->hasRadio = hasRadio;
}

/*constructor
position defaults to 0,0,0
hasRadio defaults to false
@param x: x position of the player
@param y: y position of the player
@param z: z position of the player
@param level: voice level of the player
@param hasRadio: true if the player has a radio*/
Player::Player(double level) :
	VoiceSource (level){
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
bool Player::getHasRadio() {
	return hasRadio;
}

void Player::setHasRadio(bool hasRadio) {
	this->hasRadio = hasRadio;
}

#pragma endregion
