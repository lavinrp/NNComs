#include "GameData.h"



#pragma region Constructors / destructor
/*Default constructor for GameData
allocated radios and players*/
GameData::GameData() {
	radios = new map<unsigned int, Radio*>();
	players = new map<unsigned int, Player*>();
}

/*destructor for GameData
frees memory for the radios and players vectors, including
each element in the vectors*/
GameData::~GameData() {
	//free radio memory
	for (map<unsigned int, Radio*>::iterator it = radios->begin(); it != radios->end(); it++) {
		delete it->second;
		it->second = nullptr;
	}
	delete radios;
	radios = nullptr;

	//free player memory
	for (map<unsigned int, Player*>::iterator it = players->begin(); it != players->end(); it++) {
		delete it->second;
		it->second = nullptr;
	}
	delete players;
	players = nullptr;
}
#pragma endregion

#pragma region getters / setters
/*returns the inGame status of the GameData*/
bool GameData::getInGame() {
	return inGame;
}

/*sets the in-game Status of the GameData*/
void GameData::setInGame(bool inGame) {
	this->inGame = inGame;
}

/*returns the vector of player pointers as a pointer*/
vector<Player*>* GameData::getPlayers() {
	//TODO(Ryan Lavin): fill stub -6/11/2016

	throw exception("Not Implemented");
}

/*returns the vector of radio pointers as a pointer*/
vector<Radio*>* GameData::getRadios() {
	//TODO(Ryan Lavin): fill stub -6/11/2016

	throw exception("Not Implemented");

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
	//TODO(Ryan Lavin): fill stub -6/11/2016
	throw exception("Not Implemented");
}
/*addRadio
adds the passed Radio pointer to the vector of radios
@param radio: radio to add to the GameData*/
void GameData::addRadio(Radio* radio) {
	//TODO(Ryan Lavin): fill stub -6/11/2016
	throw exception("Not Implemented");
}
#pragma endregion