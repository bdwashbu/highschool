#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "player.h"

class particleclass
{
public:
	bool Alive;
	virtual void DrawParticle(float scalar) = 0;
	virtual void MoveParticle(float scalar) = 0;
protected:
	float Y;
	float X;
	float Alpha;
	float Size;
	float XVector;
	float YVector;
	void Normalize(float *XVector, float *YVector);
};

class snow : public particleclass
{
public:
	snow();
	snow(float tempX, float tempY, float tempRed, float tempGreen, float tempBlue, float tempXVector, float tempYVector);
	snow *next;
	void DrawParticle(float scalar);
	void MoveParticle(float scalar);
private:
	float totaltimer;
};

class blockparticle : public particleclass
{
public:
	blockparticle();
	blockparticle(float tempX, float tempY, float tempRed, float tempGreen, float tempBlue, float tempXVector, float tempYVector);
	blockparticle *next;
	void DrawParticle(float scalar);
	void MoveParticle(float scalar);
};

class explodeparticle : public particleclass
{
public:
	explodeparticle();
	explodeparticle(float tempX, float tempY, float tempRed, float tempGreen, float tempBlue, float tempXVector, float tempYVector);
	explodeparticle *next;
	float Red;
	float Green;
	float Blue;
	void DrawParticle(float scalar);
	void MoveParticle(float scalar);
};

class fire : public particleclass
{
public:
	fire();
	fire(float tempX, float tempY, float tempRed, float tempGreen, float tempBlue, float tempXVector, float tempYVector);
	fire *next;
	float AlphaDecline;
	float YSpeedIncrease;
	float Red, Green, Blue;
	void DrawParticle(float scalar);
	void MoveParticle(float scalar);
};

class water : public particleclass
{
public:
	water();
	water(float tempX, float tempY, float tempRed, float tempGreen, float tempBlue, float tempXVector, float tempYVector);
	water *next;
	void DrawParticle(float scalar);
	void MoveParticle(float scalar);
};

#endif