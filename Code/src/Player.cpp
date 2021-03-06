#include "Player.h"

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
	radio = shared_ptr<Radio>();
	tsID = -1; //TODO(Ryan Lavin): determine if this is a good default value. -6/11/2016
}

Player::~Player() {
}

#pragma endregion

#pragma region getters and settere

/*getRadio
returns the players equipped radio
*/
shared_ptr<Radio> Player::getRadio() {
	return radio;
}

/*setRadio
Sets the players currently equipped radio
@param radio: pointer to the players radio. Set to null shared_ptr if player has no radio*/
void Player::setRadio(shared_ptr<Radio> radio) {
	this->radio = radio;
}

/*getTsID
returns the players TS ID
@returns: the TS ID of the player*/
anyID Player::getTsID() {
	return this->tsID;
}

/*setTsID
stores the players TS ID
@param id: players new TS ID*/
void Player::setTsID(anyID id) {
	this -> tsID = id;
}

#pragma endregion
