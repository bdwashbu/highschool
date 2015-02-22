#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "fmod.h"
#include "texture.h"

class playerclass
{
public:
	playerclass();
	~playerclass();
	int Score;
	float BonusTimer;
	int Level;
	int highscore[5];
	char InputString[3];
	int InputCount;
	bool Inputting;
	int InputPlace;
	char highscorename[5][3];
	float Red, Green, Blue;
	float timeelapsed;
	float FloatScore, TotalSpeed;
	bool Paused, GameOver;
	float untilstart; bool playedfightsound;
	int Levelgap, NewLevel;
	bool MonitorLevel(float scalar, textureclass *texture);
	int calculatefps(double scalar);
private:
	FSOUND_SAMPLE *newlevel;
};

#endif