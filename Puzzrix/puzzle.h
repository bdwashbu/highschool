#ifndef _H_PUZZLE_H_
#define _H_PUZZLE_H_

#include "grid.h"

class puzzleclass
{
public:
	puzzleclass();
	void Restart(void);
	void DrawPiece(void);
	void HandleInput(bool &left, bool &right, bool &down, bool &up, gridclass &grid, FSOUND_SAMPLE *flip);
	bool DetectCollison(float scalar, gridclass &grid);
private:
	unsigned int position;
	float Speed;
	int X, X2;
	float Y, Y2;
	bool hit, hit2;
	float Red, Blue, Green, Red2, Blue2, Green2;
	float timer, timer2;
};

#endif