#include "output.h"
#include <fstream.h>
#include <windows.h>		
#include <gl\gl.h>			
#include <gl\glu.h>
#include <string.h>			


output2::output2()
{
	strcpy (tempfile, ".dlf");
	tempx = 200.0f;
}

void output2::save(char *file)
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

void output2::showsave(float x)
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

void output2::saving()
{
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
	glDisable(GL_BLEND);
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

	glEnable(GL_BLEND);
	glPopAttrib();
}

bool output2::SaveButton(float mousex, float mousey, int lowx, int highx, int lowy, int highy)
{
	if (mousex > lowx && mousex < highx && mousey < highy && mousey > lowy)
	{
		glColor3f(1.0f, 0.0f, 0.0f);
		return true;
	}
	else
	{
		glColor3f(1.0f, 1.0f, 1.0f);
		return false;
	}
}
