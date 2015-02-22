#include "particle.h"
#include <windows.h>		
#include <gl\gl.h>			
#include <gl\glu.h>			

ParticleClass::ParticleClass(int Ypos)
{
	Next = 0;
	Previous = 0;
	Mass = 2.0;
	XPosition = 320;
	YPosition = Ypos;
	XVelocity = 0.0;
	YVelocity = 0.0;
	XForce = 0.0;
	YForce = 0.0;
}

void ParticleClass::Draw()
{
	glBegin(GL_POINTS);
		glVertex2d(XPosition, YPosition);
	glEnd();
}

void ParticleClass::Move()
{
}