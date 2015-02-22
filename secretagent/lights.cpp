#include "lights.h"
#include <math.h>
#include <windows.h>		
#include <gl\gl.h>			
#include <gl\glu.h>
#include <fstream.h>
#include <string.h>


int lightmaps::currentlight = 0;

lightmaps::lightmaps()
{
	radius = 150.0f;
	X = 250;
	Y = 250;
	red = 1.0f;
	green = 1.0f;
	blue = 1.0f;
}

void lightmaps::DrawLOS(lines *line, int angle)
{	
	float shortestx, shortesty, longestx, longesty;
	float shortestx2, shortesty2, longestx2, longesty2;
	float losx, losy;
	float losslope;
	float savedist = radius;
	//theta = angle

	FindShortest(&shortestx, &shortesty,&longestx, &longesty, X, X+(cosf(angle/57.29f) - sinf(angle/57.29f)*radius), Y, Y+(sinf(angle/57.29f) + cosf(angle/57.29f)*radius));

	losslope = (Y-(Y+sinf(angle/57.29f) + cosf(angle/57.29f)*radius)) / (X-(X+cosf(angle/57.29f) - sinf(angle/57.29f)*radius));
	
	float tempyintercept = Y - losslope*X;

	for (int i = 0; i < 100; i++)
	{
	FindShortest(&shortestx2, &shortesty2,&longestx2, &longesty2, line[i].linex, line[i].line2x, line[i].liney, line[i].line2y);
	float nearestxpoint = (line[i].yintercept - tempyintercept)/(losslope - line[i].slope);
	float nearestypoint = losslope*nearestxpoint+tempyintercept;

	if (nearestxpoint > shortestx && nearestxpoint < longestx && nearestypoint > shortesty && nearestypoint < longesty
		&& nearestxpoint > shortestx2 && nearestxpoint < longestx2 && nearestypoint > shortesty2 && nearestypoint < longesty2
		&& sqrt((nearestypoint - Y)*(nearestypoint - Y) + (nearestxpoint - X)*(nearestxpoint - X)) < savedist)
	{
		losx = nearestxpoint;
		losy = nearestypoint;
		savedist = sqrtf((nearestypoint - Y)*(nearestypoint - Y) + (nearestxpoint - X)*(nearestxpoint - X));
	}
	else if (savedist == radius)
	{
		losx = X+(cosf(angle/57.29f) - sinf(angle/57.29f)*radius);
		losy = Y+(sinf(angle/57.29f) + cosf(angle/57.29f)*radius);
	}
	}

		glPushAttrib(GL_LINE_BIT);
		glLineWidth(4.0f);
		glBegin(GL_LINES);
			glColor3f(red, green, blue);
			glVertex2f(X,Y);
			glVertex2f(losx, losy);
		glEnd();
		glPopAttrib();



}

void lightmaps::FindShortest(float *shortestx = 0, float *shortesty = 0, float *longestx = 0, float *longesty = 0, float linex = 0, float line2x = 0, float liney = 0, float line2y = 0)
{
	if (linex > line2x)
	{
		*longestx = linex;
		*shortestx = line2x;
	}
	else
	{
		*longestx = line2x;
		*shortestx = linex;
	}
	if (liney > line2y)
	{
		*longesty = liney;
		*shortesty = line2y;
	}
	else
	{
		*longesty = line2y;
		*shortesty = liney;
	}
}

void lightmaps::SaveLights(char *newname, char *filename, int i)
{
	ofstream fout2(newname, ios::app);
	fout2 << filename; fout2 << i << "\n";
	fout2 << radius << " ";
	fout2 << X << " ";
	fout2 << Y << "\n";
	fout2.close();
}

void lightmaps::DrawLightPoint()
{
	if (active)
	{
	glPushAttrib(GL_CURRENT_BIT);
	glColor3f(0.0f, 1.0f, 0.0);
	glBegin(GL_POINTS);
		glVertex2f(X,Y);
	glEnd();
	glPopAttrib();
	}
}

void lightmaps::DrawLightMap()
{
	if (active)
	{
	glEnable(GL_ALPHA_TEST);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	glBindTexture(GL_TEXTURE_2D, 4);
	glColor4f(1.0f, 1.0f, 1.0f, 0.85f);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_POLYGON);
	glTexCoord2i(1,1);		glVertex2i(X+radius, Y+radius);
	glTexCoord2i(0,1);		glVertex2i(X-radius, Y+radius);
	glTexCoord2i(0,0);		glVertex2i(X-radius, Y-radius);
	glTexCoord2i(1,0);		glVertex2i(X+radius, Y-radius);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_ALPHA_TEST);
	}
}
