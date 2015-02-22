#include <windows.h>		
#include <gl\gl.h>			
#include <gl\glu.h>
#include <fstream.h>
#include "grid.h"
#include "texture.h"

extern double begintime;
extern int MouseX, MouseY;
extern int Width, Height;
float XPos = 0.0f, YPos = 0.0f;
float Rotation = 135.0f;
gridclass *Grid = 0;
extern bool MakeSelection;
extern bool keys[256];
extern bool Moved;
extern bool RightButtonDown, LeftButtonDown;
extern int MouseX1, MouseX2, MouseY1, MouseY2;

int DrawGLScene(GLvoid)									
{
	//static gridclass Grid(Width, Height);
	begintime = timeGetTime();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0f,640.0f/480.0f,0.1f,100.0f); // 3D
		glRotatef(45, 1.0f, 0.0f, 0.0f);
		glRotatef(Rotation, 0.0f, 1.0f, 0.0f);
		glTranslatef(5.0f+XPos, -10.0f, 5.0f+YPos);
	glMatrixMode(GL_MODELVIEW);

	if (Grid) { //only if grid has been loaded, ya know?
	glEnable (GL_POLYGON_OFFSET_FILL);
	Grid->DrawGrid();
	Grid->DrawSelection();
	glDisable (GL_POLYGON_OFFSET_FILL);

	if (MakeSelection == true) {
		Grid->Selection(MouseX, MouseY, XPos, YPos, Rotation, keys[VK_SHIFT], keys[VK_CONTROL]);
		MakeSelection = false;
	}

	if (Moved == false && RightButtonDown == false) {
		Moved = true;
		Grid->Unselect();
	}
	if (keys[VK_UP] || keys[VK_DOWN] || keys[VK_LEFT] || keys[VK_RIGHT])
		Grid->EditTerrain(keys[VK_UP], keys[VK_DOWN], keys[VK_LEFT], keys[VK_RIGHT]);
	keys[VK_UP] = false;
	keys[VK_DOWN] = false;
	keys[VK_LEFT] = false;
	keys[VK_RIGHT] = false;
	}

	if (keys[VK_END]) {
		//CaptureScreen();
		system("tactics.exe");
		keys[VK_END] = false;
	}


	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0,640,0,480); //for 2D
	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_POINTS);
		glVertex2i(MouseX, MouseY);
	glEnd();
	if (LeftButtonDown && !RightButtonDown) {
		glBegin(GL_LINE_LOOP);
			glVertex2i(MouseX1, MouseY1);
			glVertex2i(MouseX2, MouseY1);
			glVertex2i(MouseX2, MouseY2);
			glVertex2i(MouseX1, MouseY2);
		glEnd();
	}
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	return TRUE;
}
