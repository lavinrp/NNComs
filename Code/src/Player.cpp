#include "Player.h"

//TODO(Ryan Lavin): make this match the UML - 5/23/2016

#pragma region Constructors / destructor

/*Default constructor
position: 0,0,0
voiceLevel: 0*/
Player::Player() {}


/*constructor
voiceLevel defaults to 0
@param x: x position of the player
@param y: y position of the player
@param z: z position of the player*/
Player::Player(double x, double y, double z) :
	VoiceSource(x, y, z){
}

Player::~Player() {
}
#pragma endregion

#pragma region Member Functions


#pragma endregion

#pragma region getters and settere



#pragma endregion
