#include <windows.h>		
#include <gl\gl.h>			
#include <gl\glu.h>
#include <stdlib.h>

#include "math.h"


#include "particle.h"

blockparticle::blockparticle()
{
	next = 0;
}

snow::snow()
{
	next = 0;
}

explodeparticle::explodeparticle()
{
	next = 0;
}

fire::fire()
{
	next = 0;
}

water::water()
{
	next = 0;
}

blockparticle::blockparticle(float tempX, float tempY, float tempRed, float tempGreen, float tempBlue, float tempXVector, float tempYVector)
{
	Alive = true;
	X = tempX;
	Y = tempY;
	XVector = tempXVector;
	YVector = tempYVector;
	Normalize(&XVector, &YVector);
	Alpha = (rand()%50+50)/100.0f;
	Size = float((rand()%200+200))/100.0f;
	next = 0;
}

explodeparticle::explodeparticle(float tempX, float tempY, float tempRed, float tempGreen, float tempBlue, float tempXVector, float tempYVector)
{
	Alive = true;
	X = tempX;
	Y = tempY;
	//Red = tempRed;
	//Green = tempGreen;
	//Blue = tempBlue;
	XVector = tempXVector;
	YVector = tempYVector;
	Alpha = (rand()%50+50)/100.0f;
	Size = float((rand()%200+200))/100.0f;
	next = 0;
}

snow::snow(float tempX, float tempY, float tempRed, float tempGreen, float tempBlue, float tempXVector, float tempYVector)
{
	Alive = true;
	X = tempX;
	Y = tempY;
	totaltimer = rand()%312/100.0f;
	XVector = tempXVector;
	YVector = tempYVector;
	Size = float((rand()%200+100))/100.0f;
	XVector *= (1.0f/(4.0-Size));
	YVector *= (1.0f/(4.0-Size));
	Alpha = 0.5f/(3.6f-Size);
	next = 0;
}

fire::fire(float tempX, float tempY, float tempRed, float tempGreen, float tempBlue, float tempXVector, float tempYVector)
{
	Alive = true;
	X = tempX;
	Y = tempY;
	XVector = tempXVector;
	YVector = tempYVector;
	Red = 1.0f;
	Green = 0.6f;
	Blue = 0.207f;
	AlphaDecline = ((rand()%2-1+13.0f)/100.0f)*(YVector/6.0f);
	YSpeedIncrease = (rand()%40+60)/100.0f;
	Size = 10.0f;
	Alpha = 1.0f;
	next = 0;
}

water::water(float tempX, float tempY, float tempRed, float tempGreen, float tempBlue, float tempXVector, float tempYVector)
{
	Alive = true;
	X = tempX;
	Y = tempY;
	XVector = tempXVector;
	YVector = tempYVector;
	Size = rand()%4+4.0f;
	Alpha = 1.0f;
	next = 0;
}

void particleclass::Normalize(float *XVector, float *YVector)
{
	float length = sqrtf((*XVector * *XVector)+(*YVector * *YVector));
	*XVector/=length;
	*YVector/=length;
}

void blockparticle::DrawParticle(float scalar)
{
	glColor4f(1.0f, 1.0f, 1.0f, Alpha);
	glPointSize(Size);
	glBegin(GL_POINTS);
		glVertex2f(X, Y);
	glEnd();
	MoveParticle(scalar);
}

void blockparticle::MoveParticle(float scalar)
{
	X+=XVector*(scalar*150);
	Y+=YVector*(scalar*150);
	YVector-=scalar*6.0f;
	//Normalize(&XVector, &YVector);
	Alpha-=(scalar*1.5f);
	if (Alpha < 0.0f || Y < 0.0f || X < 0.0f)
		Alive = false;
}

void snow::DrawParticle(float scalar)
{
	glColor4f(1.0f, 1.0f, 1.0f, Alpha);
		glTexCoord2i (1,1); glVertex2f(X+(15.0f/(4.0f-Size))*sin(totaltimer*1.0f)+Size, Y+Size);
		glTexCoord2i (0,1); glVertex2f(X+(15.0f/(4.0f-Size))*sin(totaltimer*1.0f)-Size, Y+Size);
		glTexCoord2i (0,0); glVertex2f(X+(15.0f/(4.0f-Size))*sin(totaltimer*1.0f)-Size, Y-Size);
		glTexCoord2i (1,0); glVertex2f(X+(15.0f/(4.0f-Size))*sin(totaltimer*1.0f)+Size, Y-Size);
	MoveParticle(scalar);
}

void snow::MoveParticle(float scalar)
{
	totaltimer+=scalar;
	X+=XVector*(scalar);
	Y+=YVector*(scalar);
	if (Y < 0.0f || X < 0 || X>640)
		Alive = false;
}

void explodeparticle::DrawParticle(float scalar)
{
	glColor4f(1.0f, 1.0f, 1.0f, Alpha);
	glPointSize(Size);
	glBegin(GL_POINTS);
		glVertex2f(X, Y);
	glEnd();
	MoveParticle(scalar);
}

void explodeparticle::MoveParticle(float scalar)
{
	X+=XVector*scalar;
	Y+=YVector*scalar;
	Size=6.0*Alpha;
	//Normalize(&XVector, &YVector);
	Alpha-=(scalar*1.0f);
	if (Alpha < 0.0f || Y < 0.0f || X < 0.0f)
		Alive = false;
}

void fire::DrawParticle(float scalar)
{
	glColor4f(Red, Green, Blue, Alpha);
	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2i (1,1); glVertex2f(X+Size, Y+Size);
		glTexCoord2i (0,1); glVertex2f(X-Size, Y+Size);
		glTexCoord2i (1,0); glVertex2f(X+Size, Y-Size);
		glTexCoord2i (0,0); glVertex2f(X-Size, Y-Size);
	glEnd();
	MoveParticle(scalar);
}

void fire::MoveParticle(float scalar)
{
	X+=XVector*scalar;
	Y+=YVector*scalar;
	//if (YVector != 0)
	//	YVector+=scalar*YSpeedIncrease;
	Green=0.6f*Alpha;
	Blue=0.207*Alpha;
	Alpha-=AlphaDecline*scalar;
	if (Alpha < 0.0f)
		Alive = false;
}

void water::DrawParticle(float scalar)
{
	glColor4f(0.0, 0.0, 1.0f, Alpha);
	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2i (1,1); glVertex2f(X+Size, Y+Size);
		glTexCoord2i (0,1); glVertex2f(X-Size, Y+Size);
		glTexCoord2i (1,0); glVertex2f(X+Size, Y-Size);
		glTexCoord2i (0,0); glVertex2f(X-Size, Y-Size);
	glEnd();
	MoveParticle(scalar);
}

void water::MoveParticle(float scalar)
{
	X+=XVector*scalar;
	Y+=YVector*scalar;
	YVector-=(300*scalar);
	//if (YVector != 0)
	//	YVector+=scalar*YSpeedIncrease;
	if (Y < 0.0f || X < 0.0f)
		Alive = false;
}