#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "grid.h"
//#include "fmod.h"

class blockclass
{
public:
	blockclass();
	void DrawPiece(float scalar, float time);
	virtual void HandleInput(bool &left, bool &right, bool &down, bool &up, gridclass &grid, FSOUND_SAMPLE *flip, FSOUND_SAMPLE *stomp, float scalar) = 0;
	bool DetectCollison(float scalar, gridclass &grid, float tempspeed, bool &space);
	virtual void Restart(bool nextpiece) = 0;
	void AssignColor(void);
	void MoveLeft(gridclass &grid, bool &left, float scalar);
	void MoveRight(gridclass &grid, bool &right, float scalar);
	void MoveDown(gridclass &grid, FSOUND_SAMPLE *stomp, bool &down);
protected:
	bool hit;
	int offsetx[4]; int offsety[4];
	int X, Rotation;
	int AbsoluteX;
	float Speed;
	float timer;
	float Y;
	float Red, Green, Blue;
	float BlockLife;
	float blurlife;
	float blurypos;
};

class stick:public blockclass
{
public:
	void Restart(bool nextpiece);
	void HandleInput(bool &left, bool &right, bool &down, bool &up, gridclass &grid, FSOUND_SAMPLE *flip, FSOUND_SAMPLE *stomp, float scalar);
};

class square:public blockclass
{
public:
	void Restart(bool nextpiece);
	void HandleInput(bool &left, bool &right, bool &down, bool &up, gridclass &grid, FSOUND_SAMPLE *flip, FSOUND_SAMPLE *stomp, float scalar);
};

class curveright:public blockclass
{
public:
	void Restart(bool nextpiece);
	void HandleInput(bool &left, bool &right, bool &down, bool &up, gridclass &grid, FSOUND_SAMPLE *flip, FSOUND_SAMPLE *stomp, float scalar);
};

class curveleft:public blockclass
{
public:
	void Restart(bool nextpiece);
	void HandleInput(bool &left, bool &right, bool &down, bool &up, gridclass &grid, FSOUND_SAMPLE *flip, FSOUND_SAMPLE *stomp, float scalar);
};

class longcurveright:public blockclass
{
public:
	void Restart(bool nextpiece);
	void HandleInput(bool &left, bool &right, bool &down, bool &up, gridclass &grid, FSOUND_SAMPLE *flip, FSOUND_SAMPLE *stomp, float scalar);
};

class longcurveleft:public blockclass
{
public:
	void Restart(bool nextpiece);
	void HandleInput(bool &left, bool &right, bool &down, bool &up, gridclass &grid, FSOUND_SAMPLE *flip, FSOUND_SAMPLE *stomp, float scalar);
};

class goodpiece:public blockclass
{
public:
	void Restart(bool nextpiece);
	void HandleInput(bool &left, bool &right, bool &down, bool &up, gridclass &grid, FSOUND_SAMPLE *flip, FSOUND_SAMPLE *stomp, float scalar);
};


#endif