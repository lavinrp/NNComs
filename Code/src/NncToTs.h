#pragma once

typedef unsigned __int16 anyID;


class NncToTs
{
public:
	//constructors / destructor
	NncToTs();
	NncToTs(anyID clientID);
	~NncToTs();

	//Member functions
	void getNncSoundData();
	bool isNncMuted();

	//getters and setters

	//client
	anyID getClientID();
	//left volume
	float* getLeftVolumes();
	float getLeftVolume(unsigned int position);
	//right volume
	float* getRightVolumes();
	float getRightVolume(unsigned int position);
	//Distortions
	short* getDistortions();
	short getDistortion(unsigned int position);
	//Sources
	int getSources();



private:
	anyID clientID;
	float* leftVolumes;
	float* rightVolumes;
	short* distortions;
	int sources;




};



