#include "walls.h"
//#include <math.h>
#include <windows.h>		
#include <gl\gl.h>			
#include <gl\glu.h>
#include <fstream.h>
//#include "balls.h"

wallclass::wallclass()
{
	red = 1.0f;
	green = 1.0f;
	blue = 1.0f;
}

float *wallclass::templinex2 = 0;
float *wallclass::templiney2 = 0;

void wallclass::DrawLine()
{
	glVertex2f(linex, liney);
	glVertex2f(line2x, line2y);
}

void wallclass::WriteFile(char *file)
{
	ofstream fout(file, ios::app);
	fout << linex << " ";
	fout << line2x << " ";
	fout << liney << " ";
	fout << line2y << " ";
	fout << slope << " ";
	fout << yintercept << " ";
	fout << red << " ";
	fout << green << " ";
	fout << blue << "\n";
	fout.close();
}
