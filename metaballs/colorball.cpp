#include "colorball.h"
#include <stdlib.h>
#include <time.h>

void colorballclass::SetAttributes()
{
	X = 160;
	Y = 120;
	movex = float(rand()%60)-30;
	movey = float(rand()%60)-30;

}


void colorballclass::BallMove(float scalar)
{
	if (X+(movex*scalar) > 280 || X+(movex*scalar) < 40)
		movex*=-1;
	else
		X+=(movex*scalar);
	if (Y+(movey*scalar) > 200 || Y+(movey*scalar) < 40)
		movey*=-1;
	else
		Y+=(movey*scalar);
}
