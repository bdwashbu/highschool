#include "holes.h"
#include <windows.h>
#include <gl\gl.h>			
#include <gl\glu.h>
#include <fstream.h>	
		
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
	glColor4f(1.0f, 1.0f, 1.0f,1.0f);
	glPointSize(13);
	glBegin(GL_POINTS);
		glVertex2f(X,Y);
	glEnd();
	glPointSize(10);
	glColor3f(0.0f, 0.0, 0.0f);
	glBegin(GL_POINTS);
		glVertex2f(X,Y);
	glEnd();	
}

void holeclass::WriteFile(char *file)
{
	ofstream fout(file, ios::app);
	fout << X << " ";
	fout << Y << "\n";
	fout.close();
}
