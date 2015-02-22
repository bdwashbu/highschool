#include <stdio.h>
#include <math.h>
#include <iostream.h>
#include <windows.h>				

#include "mouse.h"

mouseclass::mouseclass()
{
}

void mouseclass::DrawMouse()
{
	glLineWidth(1.0f);
	glDisable(GL_LINE_SMOOTH);
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_LINES);
		glVertex2f(mouse_x+2.0f, mouse_y);
		glVertex2f(mouse_x-3.0f, mouse_y);
		glVertex2f(mouse_x, mouse_y+3.0f);
		glVertex2f(mouse_x, mouse_y-2.0f);
	glEnd();
	glEnable(GL_LINE_SMOOTH);
	glLineWidth(1.4f);
}