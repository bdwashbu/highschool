#ifndef __ballclass__
#define __ballclass__


#include "walls.h"
#include "windows.h"
#include "GL/glu.h"

class ballclass
{
public:
	ballclass();
	void DrawBall(void);
	void WriteFile(char *file);
	static float *SetPointer(ballclass *ball, float mousex, float mousey, int count);
	float *SetxRightPointer(float mousex, float mousey, float *templinex, float *templiney, int choice);
	float X;
	float Y;
private:
};

#endif
