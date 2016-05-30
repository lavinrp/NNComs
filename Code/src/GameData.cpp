#include "GameData.h"



#pragma region Constructors / destructor
/*Default constructor for GameData
allocated radios and players*/
GameData::GameData() {
	radios = new vector<Radio*>();
	players = new vector<Player*>();
}

/*destructor for GameData
frees memory for the radios and players vectors, but
not for elements in the vectors*/
GameData::~GameData() {
	delete radios;
	delete players;
}
#pragma endregion

#pragma region getters / setters
/*returns the inGame status of the GameData*/
bool GameData::getInGame() {
	return inGame;
}

/*sets the ingame Status of the GameData*/
void GameData::setInGame(bool inGame) {
	this->inGame = inGame;
}

/*returns the vector of player pointers as a pointer*/
vector<Player*>* GameData::getPlayers() {
	return players;
}

/*returns the vector of radio pointers as a pointer*/
vector<Radio*>* GameData::getRadios() {
	return radios;
}
#pragma endregion
