#include "star.h"
#include <stdlib.h>
#include <time.h>
#include <windows.h>		
#include <gl\gl.h>			

starclass::starclass()
{
	Init();
}

void starclass::Init()
{
	X = (rand()%1200-600)/10.0f;
	Y = (rand()%1200-600)/10.0f;
	Z = (rand()%1200-600)/10.0f;
	Size = (rand()%25+10)/10.0f;
}

void starclass::DrawStar()
{
	glColor4f(1.0f, 1.0f, 1.0f, Size/3.5f);
	glPointSize(Size);
	glBegin(GL_POINTS);
		glVertex3f(X,Y,Z);
	glEnd();
}