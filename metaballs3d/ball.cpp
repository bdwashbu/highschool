#include "balls.h"

ballclass::ballclass()
{
	X = 20;
	Y = 20;
	Z = 20;
	Radius = 4;
}

void ballclass::MoveBall(float scalar)
{
	if (X+scalar < 40)
		X+=scalar;
}