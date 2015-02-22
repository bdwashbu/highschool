#include "holes.h"
#include <windows.h>
#include <gl\gl.h>			
#include <gl\glu.h>
#include <fstream.h>
#include <math.h>	
		
float *holeclass::SetPointer(holeclass *hole, float mousex, float mousey, int count)
{
	bool go = false;
	for (int i = 0; i < 20; i++)
	{
		if (hole[i].X == 0.0)
			break;
	}
	if (count == 1)
		return &hole[i].X;
	else
		return &hole[i].Y;
}

void holeclass::DrawHole()
{
	glPointSize(13.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_POINTS);
		glVertex2f(X,Y);
	glEnd();
	glPointSize(10.0f);
	glColor3f(0.0f, 0.0, 0.0f);
	glBegin(GL_POINTS);
		glVertex2f(X,Y);
	glEnd();	
}

float *holeclass::SetxRightPointer(float mousex, float mousey, float *templinex, float *templiney, int choice)
{
	if (choice == 1)
	{
	if (sqrt((mousey - Y)*(mousey - Y)+(mousex - X)*(mousex - X)) < 7 && &X != templinex)
	{
		*templinex = 0.0f;
		return &X;
	}
	else
		return 0;
	}
	else
	{
	if (sqrt((mousey - Y)*(mousey - Y)+(mousex - X)*(mousex - X)) < 7 && &Y != templiney)
	{
		*templiney = 0.0f;
		return &Y;
	}
	else
		return 0;
	}

}

void holeclass::WriteFile(char *file)
{
	ofstream fout(file, ios::app);
	fout << X << " ";
	fout << Y << "\n";
	fout.close();
}
