#include "lightmap.h"
#include <windows.h>		
#include <gl\gl.h>			
#include <gl\glu.h>

lightmapclass::lightmapclass()
{
	next = 0;
}

lightmapclass::lightmapclass(float tempX, float tempY)
{
	X = tempX;
	Y = tempY;
	Size = 40.0f;
	Life = 0.3f;
	Alive = true;
	next = 0;

}

void lightmapclass::DrawLightmap(float scalar)
{
	if (Alive) {
	glColor4f(1.0f, 1.0f, 1.0f, Life);
	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2i (1,1); glVertex2f(X+Size, Y+Size);
		glTexCoord2i (0,1); glVertex2f(X-Size, Y+Size);
		glTexCoord2i (1,0); glVertex2f(X+Size, Y-Size);
		glTexCoord2i (0,0); glVertex2f(X-Size, Y-Size);
	glEnd();
	Life-=scalar*0.5f;
	if (Life < 0.0f)
		Alive = false;
	}
}

