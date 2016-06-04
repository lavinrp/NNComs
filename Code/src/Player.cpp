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
	radio = nullptr;
}

Player::~Player() {
}

#pragma endregion

#pragma region getters and settere

/*getRadio
returns the players equipped radio
*/
Radio* Player::getRadio() {
	return radio;
}

/*setRadio
Sets the players currently equipped radio
@param radio: pointer to the players radio. Set to nullptr if player has no radio*/
void Player::setRadio(Radio* radio) {
	this->radio = radio;
}

/*getID
returns the players TS ID
@returns: the TS ID of the player*/
anyID Player::getID() {
	return this->id;
}

/*setID
stores the players TS ID
@param id: players new TS ID*/
void Player::setID(anyID id) {
	this -> id = id;
}

#pragma endregion

