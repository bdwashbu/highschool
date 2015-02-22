#include <windows.h>		
#include <gl\gl.h>			
#include <gl\glu.h>
#include "particle.h"

void particleclass::DrawParticle(float scalar)
{
	if (active)
	{
	glColor4f(1.0f, 1.0f, 1.0f, life);
	glPointSize(6.0);
	glBegin(GL_POINTS);
		glVertex2f(X,Y);
	glEnd();
	X+=XVector*(100*scalar);
	Y+=YVector*(100*scalar);
	life-=4*scalar;
	if (life < 0)
		active = false;
	}
}