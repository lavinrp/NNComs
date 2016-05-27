#pragma once

#include<vector>


#include "VoiceSource.h"
#include "Radio.h"

using namespace std;



class Player :
	public VoiceSource
{
public:
	//constructors / destructor
	Player();
	Player(double x, double y, double z);

	virtual ~Player();

	//getters / setters
	unsigned int getPrimaryRadio();
	void setPrimaryRadio();

	unsigned int getRadioCount();
	Radio getRadio(unsigned int position);

	//Member Functions
	void addRadio(Radio radio);
	void removeRadio(unsigned int position);
	void replaceRadio(unsigned int position, Radio replacement);


protected:
	//Index of the currently selected radio
	unsigned int primaryRadio;

	//radios held by the player
	vector<Radio> radios;

};

