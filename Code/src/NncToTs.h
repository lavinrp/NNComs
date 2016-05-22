#pragma once

typedef unsigned __int16 anyID;

void getNncSoundData(anyID clientID, int& sources, float* leftVolumes, float* rightVolumes, short* distortions);
bool isNncMuted(anyID clientID);
