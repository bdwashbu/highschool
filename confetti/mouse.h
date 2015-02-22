#include <gl\gl.h>			
#include <gl\glu.h>
#include <windows.h>		

class mouseclass
{
public:
	void DrawMouse(void);
	float mouse_x;
	float mouse_y;
	bool holdingright;
	bool holdingdown;
	bool holdingleft;
	bool holdingup;
private:
};