#include <windows.h>		
#include <gl\gl.h>			
#include <gl\glu.h>
#include <fstream.h>


#include "grid.h"

bool gridclass::circle = true;
bool gridclass::torus = false;

void gridclass::AssignGrid(int a, int b, float xwidth, float ylength)
{
	leftx = a*xwidth+1;
	bottomy = b*ylength+1;
}

void gridclass::RenderGrid()
{
//	glBegin(GL_POINTS);
		glColor3f(red, green, blue);
		glVertex2i(leftx,bottomy);
//	glEnd();
}