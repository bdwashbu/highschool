#include "walls.h"
#include <math.h>
#include <windows.h>		
#include <gl\gl.h>			
#include <gl\glu.h>
#include <fstream.h>

wallclass::wallclass()
{
	red = 1.0f;
	green = 1.0f;
	blue = 1.0f;
}

bool wallclass::gridlock = true;
float wallclass::distance = 10.0f;
float *wallclass::templinex2 = 0;
float *wallclass::templiney2 = 0;
int wallclass::currentwall = -1;

void wallclass::DrawLine(int i)
{
if ((linex != 0.0f || liney != 0.0f) && (line2y != 0.0f || line2x != 0.0f))
{
glColor4f(red, green, blue, 1.0f);
		if (i == currentwall)
		{
		glColor3f(0.0f, 0.0f, 0.7f);
		}
glVertex2f(linex, liney);
glVertex2f(line2x, line2y);
}
}

void wallclass::WriteFile(char *file)
{
	ofstream fout(file, ios::app);
	fout << linex << " ";
	fout << line2x << " ";
	fout << liney << " ";
	fout << line2y << " ";
	if (linex > line2x)
	{
		fout << linex << " ";
		fout << line2x << " ";
	}
	else
	{
		fout << line2x << " ";
		fout << linex << " ";
	}
	if (liney > line2y)
	{
		fout << liney << " ";
		fout << line2y << " ";
	}
	else
	{
		fout << line2y << " ";
		fout << liney << " ";
	}
	fout << slope << " ";
	fout << yintercept << " ";
	fout << red << " ";
	fout << green << " ";
	fout << blue << "\n";
	fout.close();
}

void wallclass::Equation()
{
	FindSlope();
	yintercept = liney - (linex*slope);
}

void wallclass::FindSlope()
{
	if (line2x != linex && line2y != liney)
	{
		slope = (line2y - liney)/(line2x - linex);
	//	if (slope > 480.0f)
		//	slope = 480.0f;
	//	if (slope < -480.0f)
	//		slope = -480.0f;
	}
	else if (line2x == linex)
		slope = 250.0f;
	else
		slope = 0.0015f;
}

float *wallclass::SetPointer(wallclass *wall, float mousex, float mousey, int count)
{
	for (int i = 0; i < 100; i++)
	{
		if (wall[i].linex == 0.0)
			break;
		else if (wall[i].liney == 0.0)
			break;
		else if (wall[i].line2x == 0.0)
			break;
		else if (wall[i].line2y == 0.0)
			break;
	}
	if (wall[i].linex == 0.0f && count == 1)
		return &wall[i].linex;
	else if (wall[i].liney == 0.0f && count == 2)
		return &wall[i].liney;
	else if (wall[i].line2x == 0.0f && count == 1)
		return &wall[i].line2x;
	else if (wall[i].line2y == 0.0f && count == 2)
		return &wall[i].line2y;
	else
	{
		if (count == 1)
			return &wall[100].line2x;
		else
			return &wall[100].line2y;
	}
}

float *wallclass::SetxRightPointer(float mousex, float mousey, float *templinex, float *templiney)
{
	if (sqrt((mousey - liney)*(mousey - liney)+(mousex - linex)*(mousex - linex)) < 5 && &linex != templinex)
	{
		*templinex = 0.0f;
		return &linex;
	}
	else if (sqrt((mousey - line2y)*(mousey - line2y)+(mousex - line2x)*(mousex - line2x)) < 5 && &line2x != templinex)
	{
		*templinex = 0.0f;
		return &line2x;
	}
	else
		return 0;
}

float *wallclass::SetyRightPointer(float mousex, float mousey, float *templinex, float *templiney)
{
	if (sqrt((mousey - liney)*(mousey - liney)+(mousex - linex)*(mousex - linex)) < 5 && &liney != templiney)
	{
		*templiney = 0.0f;
		return &liney;
	}
	else if (sqrt((mousey - line2y)*(mousey - line2y)+(mousex - line2x)*(mousex - line2x)) < 5 && &line2y != templiney)
	{
		*templiney = 0.0f;
		return &line2y;
	}
	else
		return 0;
}

void wallclass::GridLock(float *templinex, float *templiney)
{
	if (sqrt((*templiney - liney)*(*templiney - liney)+(*templinex - linex)*(*templinex - linex)) < distance && &liney != templiney)
		{
			templinex2 = &linex;
			templiney2 = &liney;
			distance = sqrtf((*templiney - liney)*(*templiney - liney)+(*templinex - linex)*(*templinex - linex));
		//	*templinex = *templinex2;
		//	*templiney = *templiney2;
		}
	else if (sqrt((*templiney - line2y)*(*templiney - line2y)+(*templinex - line2x)*(*templinex - line2x)) < distance && &line2y != templiney)
		{ 
			templinex2 = &line2x;
			templiney2 = &line2y;
			distance = sqrtf((*templiney - line2y)*(*templiney - line2y)+(*templinex - line2x)*(*templinex - line2x));
		//	*templinex = *templinex2;
		//	*templiney = *templiney2;
		}
}

bool wallclass::SelectWall(float mousex, float mousey, float *maxdistance)
{
	float longestx;
	float shortestx;
	float longesty;
	float shortesty;


	if (linex > line2x)
	{
		longestx = linex;
		shortestx = line2x;
	}
	else
	{
		longestx = line2x;
		shortestx = linex;
	}
	if (liney > line2y)
	{
		longesty = liney;
		shortesty = line2y;
	}
	else
	{
		longesty = line2y;
		shortesty = liney;
	}

	float tempyintercept = (mousey) - (-1.0f/slope)*(mousex);

	float nearestxpoint = (tempyintercept - yintercept)/(slope - (-1.0f/slope));
	float nearestypoint = (-1.0f/slope)*(mousex)+tempyintercept;

	float distance = sqrtf((mousey - nearestypoint)*(mousey - nearestypoint)+(mousex - nearestxpoint)*(mousex - nearestxpoint));
	if (distance < *maxdistance && distance > 0.0f && mousex > shortestx-5 && mousex < longestx+5 && mousey < longesty+5 && mousey > shortesty-5)
	{
		*maxdistance = distance;
		return true;
	}
	else
		return false; 
}

void wallclass::DrawColors()
{
	glBegin(GL_LINES);
		glVertex2i(510, 260);
		glVertex2i(620, 260);
		glVertex2f(510+(110*red), 265);
		glVertex2f(510+(110*red), 255);
		glVertex2i(510, 230);
		glVertex2i(620, 230);
		glVertex2f(510+(110*green), 235);
		glVertex2f(510+(110*green), 225);
		glVertex2i(510, 200);
		glVertex2i(620, 200);
		glVertex2f(510+(110*blue), 205);
		glVertex2f(510+(110*blue), 195);
	glEnd();

	glPushAttrib(GL_LINE_BIT);
	glLineWidth(7.0);
	glColor3f(red,green,blue);
	glBegin(GL_LINES);
		glVertex2i(510, 170);
		glVertex2i(620, 170);
	glEnd();
		glColor3f(1.0f, 1.0f, 1.0f);
		glPopAttrib();

}

					
