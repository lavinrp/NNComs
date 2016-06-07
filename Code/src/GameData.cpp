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

/*getRadio
returns the radio stored in GameData at the given position
@param position: position of the radio to return
@return the radio at the past position if it exists. nullptr otherwise*/
Radio* GameData::getRadio(unsigned int position) {
	if (radios->size() > position) {
		return (*radios)[position];
	} else {
		return nullptr;
	}
}
#pragma endregion

#pragma region Member Functions
/*addPlayer
adds the passed Player pointer to the vector of players
@param player: player to add to the GameData*/
void GameData::addPlayer(Player* player) {
	players->push_back(player);
}
/*addRadio
adds the passed Radio pointer to the vector of radios
@param radio: radio to add to the GameData*/
void GameData::addRadio(Radio* radio) {
	radios->push_back(radio);
}
#pragma endregion