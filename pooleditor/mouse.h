#ifndef __mouseclass__
#define __mouseclass__


#include <gl\gl.h>			
#include <gl\glu.h>
#include <windows.h>		

class mouseclass
{
public:
	mouseclass();
	void DrawMouse(void);
	float mouse_x;
	float mouse_y;
private:
};

#endif