#include "output.h"
#include <fstream.h>
#include <windows.h>		
#include <gl\gl.h>			
#include <gl\glu.h>
#include <string.h>			


outputclass::outputclass()
{
	strcpy (tempfile, ".dlf");
	tempx = 200.0f;
}

void outputclass::save(char *file)
{
	//strncat (file, ".dlf", 4);
	char tempfile[20];
	strcpy (tempfile,file);
	strxfrm (file, "Maps/", 5);
	for(int i = 5; i < 20; i++)
	{
		file[i] = tempfile[i-5];
	}

}

void outputclass::showsave(float x)
{
	if (tempx < 200.0f)
	{
	glBegin(GL_POLYGON);
	glVertex2f(tempx+200.0f, 300.0f);
	glVertex2f(200.0f, 300.0f);
	glVertex2f(200.0f, 275.0f);
	glVertex2f(tempx+200.0f, 275.0f);
	glEnd();
	tempx+=x;
	}
}

void outputclass::saving()
{
	glEnable(GL_BLEND);
	glBegin(GL_LINES);
		glVertex2i(100, 300);
		glVertex2i(540, 300);
		glVertex2i(100, 300);
		glVertex2i(100, 240);
		glVertex2i(540, 300);
		glVertex2i(540, 240);
		glVertex2i(100, 240);
		glVertex2i(540, 240);

		glVertex2i(150, 300);
		glVertex2i(150, 350);
		glVertex2i(150, 350);
		glVertex2i(490, 350);
		glVertex2i(490, 350);
		glVertex2i(490, 300);
	glEnd();
	glPushAttrib(GL_CURRENT_BIT);
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_POLYGON);
		glVertex2i(539, 299);
		glVertex2i(101, 299);
		glVertex2i(101, 241);
		glVertex2i(539, 241);
	glEnd();
	glBegin(GL_POLYGON);
		glVertex2i(490, 350);
		glVertex2i(150, 350);
		glVertex2i(150, 303);
		glVertex2i(490, 303);
	glEnd();
	glPopAttrib();
	glDisable(GL_BLEND);

}

bool outputclass::SaveButton(float mousex, float mousey, int lowx, int highx, int lowy, int highy, float blend)
{
	if (mousex > lowx && mousex < highx && mousey < highy && mousey > lowy)
	{
		glColor4f(1.0f, 0.0f, 0.0f, blend);
		return true;
	}
	else
	{
		glColor4f(1.0f, 1.0f, 1.0f, blend);
		return false;
	}
}
