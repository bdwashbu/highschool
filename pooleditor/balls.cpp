#include "balls.h"
//#include "walls.h"
#include <windows.h>
#include <gl\gl.h>			
#include <gl\glu.h>
#include <fstream.h>
#include <math.h>		

ballclass::ballclass()
{
}

void ballclass::DrawBall()
{
	glVertex2f(X,Y);
}

float *ballclass::SetPointer(ballclass *ball, float mousex, float mousey, int count)
{
	bool go = false;
	for (int i = 0; i < 20; i++)
	{
		if (ball[i].X == 0.0)
			break;
	}
	if (count == 1)
		return &ball[i].X;
	else
		return &ball[i].Y;
}

void ballclass::WriteFile(char *file)
{
	ofstream fout(file, ios::app);
	fout << X << " ";
	fout << Y << "\n";
	fout.close();
}

float *ballclass::SetxRightPointer(float mousex, float mousey, float *templinex, float *templiney, int choice)
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

//float *ballclass::SetxRightPointer(float mousex, float mousey, float *templinex, float *templiney)
//{
//	if (sqrt((mousey - Y)*(mousey - Y)+(mousex - X)*(mousex - X)) < 5 && &X != templinex)
//	{
//		*templinex = 0.0f;
//		return &X;
//	}
//	else
//		return 0;
//}
