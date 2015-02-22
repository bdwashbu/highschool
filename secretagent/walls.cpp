#include "walls.h"
#include <math.h>
#include <windows.h>		
#include <gl\gl.h>			
#include <gl\glu.h>
#include <fstream.h>

lines::lines()
{
	//linex = 0.0f;
	//line2x = 0.0f;
	//liney = 0.0f;
	//line2y = 0.0f;
	//slope = 0.0;
	closed = true;
	red = 1.0f;
	green = 1.0f;
	blue = 1.0f;
	//moving = false;
	//door = false;
}

bool lines::gridlock = true;
float lines::distance = 10.0f;
float *lines::templinex2 = 0;
float *lines::templiney2 = 0;
int lines::currentwall = -1;
float lines::xoffset = 0.0f;
float lines::yoffset = 0.0f;

void lines::DrawLine(int i)
{

if ((linex != 0.0f || liney != 0.0f) && (line2y != 0.0f || line2x != 0.0f))
{
if (linex == line2x)
	line2x+=0.01f;

if (liney == line2y)
	line2y+=0.01f;

glColor4f(red, green, blue, 1.0f);
		if (i == currentwall)
		{
			glShadeModel(GL_SMOOTH);
			if (movelinex != 0.0 || moveliney != 0.0)
			{
			glBegin(GL_LINES);
				glColor3f(0.0f, 0.0f, 0.7f);
				glVertex2f(savelinex, saveliney);
				glColor3f(0.7f, 0.0f, 0.0f);
				glVertex2f(movelinex, moveliney);
			glEnd();
			}
			if (moveline2x != 0.0 || moveline2y != 0.0)
			{
			glBegin(GL_LINES);
				glColor3f(0.0f, 0.0f, 0.7f);
				glVertex2f(saveline2x, saveline2y);
				glColor3f(0.7f, 0.0f, 0.0f);
				glVertex2f(moveline2x, moveline2y);
			glEnd();
			}
			glShadeModel(GL_FLAT);
			glColor3f(0.0f, 0.0f, 0.7f);
		}
	glBegin(GL_LINES);
	glVertex2f(linex, liney);
	glVertex2f(line2x, line2y);
	glEnd();

}

if (moving != true && closed == true)
{
	savelinex = linex;
	saveline2x = line2x;
	saveliney = liney;
	saveline2y = line2y;
}
if (door == true && movelinex == 0.0f && moveliney == 0.0f)
{
	movelinex = linex+1;
	moveliney = liney+1;
}
if (door == true && moveline2x == 0.0f && moveline2y == 0.0f)
{
	moveline2x = line2x+1;
	moveline2y = line2y+1;
}


}

void lines::WriteFile(char *file)
{
	ofstream fout(file, ios::app);
	fout << linex << " ";
	fout << line2x << " ";
	fout << liney << " ";
	fout << line2y << " ";
	fout << slope << " ";
	fout << yintercept << " ";
	if (door)
		fout << 1 << " ";
	else
		fout << 0 << " ";
	fout << movelinex << " ";
	fout << moveliney << " ";
	fout << moveline2x << " ";
	fout << moveline2y << " ";
	fout << red << " ";
	fout << green << " ";
	fout << blue << "\n";
	fout.close();
}

void lines::Equation()
{
	FindSlope();
	yintercept = liney - (linex*slope);
}

void lines::FindSlope()
{
	if (line2x != linex && line2y != liney)
		slope = (line2y - liney)/(line2x - linex);
	else if (line2x == linex)
		slope = 0.01f;
	else
		slope = 1000.0f;
}

float *lines::SetPointer(lines *line, float mousex, float mousey, int count)
{
	for (int i = 0; i < 100; i++)
	{
		if (line[i].linex == 0.0)
			break;
		else if (line[i].liney == 0.0)
			break;
		else if (line[i].line2x == 0.0)
			break;
		else if (line[i].line2y == 0.0)
			break;
	}
	if (line[i].linex == 0.0f && count == 1)
		return &line[i].linex;
	else if (line[i].liney == 0.0f && count == 2)
		return &line[i].liney;
	else if (line[i].line2x == 0.0f && count == 1)
		return &line[i].line2x;
	else if (line[i].line2y == 0.0f && count == 2)
		return &line[i].line2y;
	else
	{
		if (count == 1)
			return &line[100].line2x;
		else
			return &line[100].line2y;
	}
}

float *lines::SetxRightPointer(float mousex, float mousey, float *templinex, float *templiney)
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

float *lines::SetyRightPointer(float mousex, float mousey, float *templinex, float *templiney)
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

void lines::GridLock(float *templinex, float *templiney)
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

void lines::WallMove(float scalar)
{
	float templinex, templiney, templine2x, templine2y;
	if (closed)
	{
	templinex = linex; templiney = liney; templine2x = line2x; templine2y = line2y;
	linex += (movelinex-savelinex)/10*scalar;
	liney += (moveliney-saveliney)/10*scalar;
	line2x += (moveline2x-saveline2x)/10*scalar;
	line2y += (moveline2y-saveline2y)/10*scalar;
	Equation();
	if (sqrt((moveline2y - line2y)*(moveline2y - line2y)+(moveline2x - line2x)*(moveline2x - line2x)) >= 
		sqrt((moveline2y - templine2y)*(moveline2y - templine2y)+(moveline2x - templine2x)*(moveline2x - templine2x)))
	{
		line2x = moveline2x+0.01f; line2y = moveline2y+0.01f; linex = movelinex+0.01f;liney = moveliney+0.01f; 
		moving = false;
		closed = false;
	}
	}
	else
	{
	templinex = linex; templiney = liney; templine2x = line2x; templine2y = line2y;
	linex += (savelinex-movelinex)/10*scalar;
	liney += (saveliney-moveliney)/10*scalar;
	line2x += (saveline2x-moveline2x)/10*scalar;
	line2y += (saveline2y-moveline2y)/10*scalar;
	Equation();
	if (sqrt((saveline2y - line2y)*(saveline2y - line2y)+(saveline2x - line2x)*(saveline2x - line2x)) >=
		sqrt((saveline2y - templine2y)*(saveline2y - templine2y)+(saveline2x - templine2x)*(saveline2x - templine2x)))
	{
		linex = savelinex+0.01f;liney = saveliney+0.01f; line2x = saveline2x+0.01f; line2y = saveline2y+0.01f;
		moving = false;
		closed = true;
	}
	}
}

bool lines::SelectWall(float mousex, float mousey, float *maxdistance)
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

	float tempyintercept = (mousey+yoffset) - (-1.0f/slope)*(mousex+xoffset);

	float nearestxpoint = (tempyintercept - yintercept)/(slope - (-1.0f/slope));
	float nearestypoint = (-1.0f/slope)*(mousex+xoffset)+tempyintercept;

	float distance = sqrtf((mousey + yoffset - nearestypoint)*(mousey + yoffset - nearestypoint)+(mousex + xoffset - nearestxpoint)*(mousex + xoffset - nearestxpoint));
	if (distance < *maxdistance && distance > 0.0f && mousex+xoffset > shortestx-5 && mousex+xoffset < longestx+5 && mousey+yoffset < longesty+5 && mousey+yoffset > shortesty-5)
	{
		*maxdistance = distance;
		return true;
	}
	else
		return false; 
}

void lines::DrawColors()
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

void lines::DrawMiniMap(float agentX, float agentY)
{
	glBegin(GL_LINES);
		glVertex2i(495, 90);
		glVertex2i(570, 90);
		glVertex2i(570,90);
		glVertex2i(570, 15);
		glVertex2i(570,15);
		glVertex2i(495,15);
		glVertex2i(495,15);
		glVertex2i(495,90);
	glEnd();
	glPushMatrix();
	glTranslatef(495+xoffset/12.9f,15+yoffset/11.2f,0.0);
	glBegin(GL_LINES);
		glVertex2f(18.75f, 56.25f);
		glVertex2f(56.25f, 56.25f);
		glVertex2f(56.25f,56.25f);
		glVertex2f(56.25f, 18.75f);
		glVertex2f(56.25f,18.75f);
		glVertex2f(18.75f,18.75f);
		glVertex2f(18.75f,18.75f);
		glVertex2f(18.75f,56.25f);
	glEnd();
	glPopMatrix();
	//map area border
	glColor3f(1.0f,1.0f,1.0f);
	glBegin(GL_LINES);
		glVertex2i(485,15);
		glVertex2i(485,420);
		glVertex2i(485,420);
		glVertex2i(15,420);
		glVertex2i(15,15);
		glVertex2i(15,420);
		glVertex2i(15,15);
		glVertex2i(485,15);
	glEnd();

	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_POINTS);
		glVertex2f(513+(agentX/13),34 + (agentY/11.2f));
	glEnd();
	glColor3f(1.0f, 1.0f, 1.0f);
}

