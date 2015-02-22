#include "ball.h"
#include <stdlib.h>

void ballclass::SetAttributes()
{
	X = 160;
	Y = 120;
	movex = float(rand()%30)-15;
	movey = float(rand()%30)-15;
	size = int(rand()%2)+2;
}


void ballclass::BallMove(float scalar)
{
	if (X+(movex*scalar) > 290 || X+(movex*scalar) < 30)
		movex*=-1;
	else
		X+=(movex*scalar);
	if (Y+(movey*scalar) > 210 || Y+(movey*scalar) < 30)
		movey*=-1;
	else
		Y+=(movey*scalar);
}
