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
	bool lbutton;
	bool holdingright;
	bool holdingdown;
	bool holdingleft;
	bool holdingup;
private:
};