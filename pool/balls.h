#ifndef _BALLS_H_
#define _BALLS_H_

#include "particle.h"
#include "fmod.h"
#include "windows.h"
#include "GL/glu.h"
#include "texture.h"

class ballclass
{
public:
	ballclass();
	void Move(float scalar, int iteration);
	bool CollisionDetection(wallclass *wall, float scalar, ballclass *ball, int ballnumber, particleclass *particle, FSOUND_SAMPLE *sound1, int count, int ballcount);
	void Aim(float mousex, float mousey, ballclass *ball, int ballcount, bool shoot, wallclass *wall, int wallcount, bool left, bool right, float scalar);
	void Hole(float HoleX, float HoleY, float scalar);
	void DrawBall(GLuint sphereDL);
	void DrawBlur(float scalar);
	bool Moving(ballclass *ball, int ballcount);
	float X;
	float Y;
	float XVector;
	float YVector;
	bool moving;
	float RotateX;
	float RotateY;
	float Blurx[20];
	float Blury[20];
	float Blurlife[20];
	bool Bluractive[20];
	//float Blurdist;
	bool Alive;
	float speed;
	float angle;
private:
	float Red, Green, Blue;
	int currentwallhit;
	int currentballhit;
	float Oncewallhitxspot;
	float Oncewallhityspot;
	float Oncewallhitxvector;
	float Oncewallhityvector;
	void ballclass::Normalize(float *XVector, float *YVector);
};

#endif