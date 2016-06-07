#pragma once

#include <vector>

#include "Radio.h"
#include "Player.h"

const unsigned int MAX_RADIO_COUNT = 500;
const unsigned int MAX_PLAYER_COUNT = 100;
const unsigned int MAX_RADIOS_PER_PLAYER = 1;

class GameData
{
public:
	//constructors
	GameData();
	virtual ~GameData();

	//getters / setters
	bool getInGame();
	void setInGame(bool inGame);

	vector<Player*>* getPlayers();
	vector<Radio*>* getRadios();
	Radio* getRadio(unsigned int position);

	//member functions
	void addPlayer(Player* player);
	void addRadio(Radio* radio);

private:
	bool inGame;

	vector<Radio*>* radios;
	vector<Player*>* players;
};

