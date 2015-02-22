#include "line.h"
#include <windows.h>		
#include <gl\gl.h>			

lineclass::lineclass()
{
	Alive = false;
	Life = 0.0f;
}

void lineclass::DrawLine(float scalar)
{
	if (Alive) {
	glColor4f(1.0f, 1.0f, 1.0f, Life);
	glBegin(GL_LINES);
		glVertex2f(X,Y);
		glVertex2f(X2, Y2);
	glEnd();
	Life-=((rand()%25+50.0f)/100.0f)*scalar;
	if (Life < 0.0)
		Alive = false;
	}
}

bool lineclass::SetLine(float x, float y, float x2, float y2)
{
	if (Alive == false) {
		X = x;
		Y = y;
		X2 = x2;
		Y2 = y2;
		Alive = true;
		Life = 1.0f;
		return true;
	}
	return false;
}
