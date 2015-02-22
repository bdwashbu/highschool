#include "agent.h"
#include <fstream.h>
#include <windows.h>		
#include <gl\gl.h>			
#include <gl\glu.h>
#include <math.h>

agents::agents()
{
	rotation = 0;
	X = 250.0f; Y = 250.0f;
}

agents::~agents()
{
}

void agents::MoveForward(float speed, float scalar, short int direction)
{
if (rotation >= 0 && rotation <= 90)
{
	float declinevalue = (speed*speed)/(sqrtf((rotation*rotation)+((90.0f-rotation)*(90.0f-rotation))));
	Xmoving=direction*rotation*declinevalue*scalar;
	Ymoving=direction*(90-rotation)*declinevalue*scalar;
}
else if (rotation > 90 && rotation <= 180)
{
	float temprotation = 180-rotation;
	float declinevalue = (speed*speed)/(sqrtf(((90-temprotation)*(90-temprotation))+((temprotation)*(temprotation))));
	Xmoving=direction*temprotation*declinevalue*scalar;
	Ymoving=direction*(90-temprotation)*-declinevalue*scalar;
}
else if (rotation > 180 && rotation <= 270)
{
	float temprotation = 270-rotation;
	float declinevalue = (speed*speed)/(sqrtf(((90-temprotation)*(90-temprotation))+((temprotation)*(temprotation))));
	Xmoving=direction*(90-temprotation)*-declinevalue*scalar;
	Ymoving=direction*temprotation*-declinevalue*scalar;
}
else if (rotation > 270 && rotation <= 360)
{
	float temprotation = 360-rotation;
	float declinevalue = (speed*speed)/(sqrtf(((90-temprotation)*(90-temprotation))+((temprotation)*(temprotation))));
	Xmoving=direction*temprotation*-declinevalue*scalar;
	Ymoving=direction*(90-temprotation)*declinevalue*scalar;
}
}

void agents::MoveRight(float scalar)
{
	//X+=0.075f;
	if (rotation <= 360.0f)
		rotation+=(300*scalar);
	else
		rotation = 0.2f;
}

void agents::MoveLeft(float scalar)
{			
	//X-=0.075f;
	if (rotation >= 0.0f)
		rotation-=(300*scalar);
	else
		rotation = 359.8f;
}

void agents::FindShortest(float *shortestx = 0, float *shortesty = 0, float *longestx = 0, float *longesty = 0, float linex = 0, float line2x = 0, float liney = 0, float line2y = 0)
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

bool agents::CheckAhead(float lineslope, float yintercept, float linex, float line2x, float liney, float line2y)
{
	float shortestx, shortesty, longesty, longestx;
	FindShortest(&shortestx, &shortesty,&longestx, &longesty, linex, line2x, liney, line2y);

	if ((X > shortestx-4 && X < longestx+4) && (Y > shortesty-4 && Y < longesty+4))
	{
	if ((Y < lineslope*X + yintercept) && (Y > lineslope*(X+4) + yintercept)) 
		return true;
	else if ((Y > lineslope*X + yintercept) && (Y < lineslope*(X-4) + yintercept))
		return true;
	else if ((Y > lineslope*X + yintercept) && (Y < lineslope*(X+4) + yintercept))
		return true;
	else if ((Y < lineslope*X + yintercept) && (Y > lineslope*(X-4) + yintercept))
		return true;
	else if ((Y < lineslope*X + yintercept) && ((Y+4) > lineslope*X + yintercept))
		return true;
	else if ((Y > lineslope*X + yintercept) && ((Y-4) < lineslope*X + yintercept))
		return true;
	else if ((Y > lineslope*X + yintercept) && ((Y+4) < lineslope*X + yintercept))
		return true;
	else if ((Y < lineslope*X + yintercept) && ((Y-4) > lineslope*X + yintercept))
		return true;
	else
		return false;
	}
	else
		return false;
}

void agents::DrawCharacter()
{
	glColor3f(1.0f, 0.0f, 0.0f);
	glEnable(GL_POINT_SMOOTH);
	glBegin(GL_POINTS);
		glVertex2f(X,Y);
	glEnd();
	glColor3f(1.0f, 1.0f, 1.0f);
	glPushMatrix();
	glTranslatef(X,Y,0.0f);
	glRotatef(-rotation,0,0,1.0);
	glPushAttrib(GL_POINT_BIT);
	glPointSize(1);
	glDisable(GL_POINT_SMOOTH);
	glBegin(GL_POINTS);
		glVertex2f(0, 1.8f);
	glEnd();
	glPopAttrib();
	glPopMatrix();

}

void agents::Collision(float lineslope, float yintercept, bool noinput)
{
	if (noinput)
	{
		Xmoving = 0.0f;
		Ymoving = 0.0f;
	}
	int move;
	if (Y > (lineslope*X) + yintercept)
		move = -1;
	else
		move = 1;
	float tempyintercept = (Y+Ymoving) - (-1.0f/lineslope)*(X+Xmoving);

	X = (-(Y - (lineslope*X))+move+tempyintercept)/(lineslope-(-1.0f/lineslope));
	Y = (-1.0f/lineslope)*X+tempyintercept;
}

void agents::Move(float xoffset, float yoffset)
{
	if (X + Xmoving-xoffset > 17 && X + Xmoving - xoffset < 487)
		X += Xmoving;
	if (Y + Ymoving-yoffset > 17 && Y + Ymoving - yoffset < 422)
		Y += Ymoving;
}
