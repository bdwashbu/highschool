#include <windows.h>		
#include <gl\gl.h>
#include <gl\glu.h>	
#include <fstream.h>
#include <math.h>		
#include "grid.h"

extern int MouseX1, MouseX2, MouseY1, MouseY2;

gridclass::gridclass(int X, int Y)
{
	Width = X;
	Height = Y;
	Grid = new tileclass *[Width]; 
	for (int i = 0; i < Width; i++)
		Grid[i] = new tileclass[Height];
	for (int y = 0; y < Height; y++) {
		for (int x = 0; x < Width; x++) {
			Grid[x][y].Init(x,y);
		}
	}
}

gridclass::gridclass(istream &is)
{
	is >> Width >> Height;
	Grid = new tileclass *[Width]; 
	for (int i = 0; i < Width; i++)
		Grid[i] = new tileclass[Height];
	for (int y = 0; y < Height; y++) {
		for (int x = 0; x < Width; x++) {
			Grid[x][y].Init(is, x, y);
		}
	}
}

gridclass::~gridclass()
{
	for (int i = 0; i < Width; i++)
		delete []Grid[i];
	delete []Grid;
}

void gridclass::DrawGrid()
{
	int y, x;
	for (y = 0; y < Height; y++) {
		for (x = 0; x < Width; x++) {
			Grid[x][y].DrawTile(Width, Height, Grid);
		}
	}
	for (y = 0; y < Height; y++) {
		for (x = 0; x < Width; x++) {
			Grid[x][y].DrawOutline(false, Width, Height, Grid);
		}
	}
	for (y = 0; y < Height; y++) {
		for (x = 0; x < Width; x++) {
			Grid[x][y].DrawOutline(true, Width, Height, Grid);
		}
	}
}

void gridclass::Unselect()
{ 
	int y, x;
	for (y = 0; y < Height; y++) {
		for (x = 0; x < Width; x++) {
			Grid[x][y].Unselect();
		}
	}
}

void gridclass::Selection(int X, int Y, float XView, float YView, float Rotation, bool shift, bool ctrl)
{
	bool skip = false;
	GLuint SelectionBuffer[400];
	GLuint hits = 0;
	int viewport[4];
	glSelectBuffer(400, SelectionBuffer);
	glGetIntegerv(GL_VIEWPORT, viewport);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glRenderMode(GL_SELECT);
	glLoadIdentity();
	gluPickMatrix(X, Y, 9, 9, viewport);
	gluPerspective(45.0f,640.0f/480.0f,0.1f,100.0f);
	glRotatef(45, 1.0f, 0.0f, 0.0f);
	glRotatef(Rotation, 0.0f, 1.0f, 0.0f);
	glTranslatef(5.0f+XView, -10.0f, 5.0f+YView);

	int x, y, s;
	glInitNames();
	glPushName(0);
	for (y = 0; y < Height; y++) {
		for (x = 0; x < Width; x++) {
			for (s = 1; s < 5; s++) {
				glLoadName(y);
				glPushName(x);
				glPushName(s);
				Grid[x][y].DrawSelectionPoints(s);
				glPopName();
				glPopName();
			}
		}
	}

	hits = glRenderMode(GL_RENDER);
	if (hits > 0) {
		skip = true;
		ProcessSelection(SelectionBuffer);
	}
	if (skip != true) {
	glSelectBuffer(400, SelectionBuffer);
	glRenderMode(GL_SELECT);
	glLoadIdentity();
	if (fabs(MouseX1-MouseX2) >= 2 && fabs(MouseY1-MouseY2) >= 2 && !ctrl) //mouse is still
		gluPickMatrix(X+((MouseX1-MouseX2)/2.0f), Y+((MouseY1-MouseY2)/2.0f), fabs(MouseX1-MouseX2), fabs(MouseY1-MouseY2), viewport);
	else 
		gluPickMatrix(X, Y, 1, 1, viewport);
	gluPerspective(45.0f,640.0f/480.0f,0.1f,100.0f);
	glRotatef(45.0f, 1.0f, 0.0f, 0.0f);
	glRotatef(Rotation, 0.0f, 1.0f, 0.0f);
	glTranslatef(5.0f+XView, -10.0f, 5.0f+YView);
	glInitNames();
	glPushName(0);

	if (!ctrl) {   //not for sides
	for (y = 0; y < Height; y++) {
		for (x = 0; x < Width; x++) {
			glLoadName(y);
			glPushName(x);
			if (fabs(MouseX1-MouseX2) > 2 || fabs(MouseY1-MouseY2) > 2)
				Grid[x][y].DrawTile(1);
			else
				Grid[x][y].DrawTile(6);
			glPopName();
		}
	}
	hits = glRenderMode(GL_RENDER);
	if (hits > 0) {
		//if (!shift)
		//	Unselect();
		if ((MouseX1-MouseX2) == 0 && (MouseY1-MouseY2) == 0)
			ProcessSelection(SelectionBuffer, hits, false, shift);
		else
			ProcessSelection(SelectionBuffer, hits, true, shift);
	}
	} else {
	for (y = 0; y < Height; y++) {
		for (x = 0; x < Width; x++) {
			for (s = 1; s < 6; s++) {
			glLoadName(y);
			glPushName(x);
			glPushName(s);
			Grid[x][y].DrawTile(s);
			glPopName();
			glPopName();
			}
		}
	}
	hits = glRenderMode(GL_RENDER);
	if (hits > 0) {
		//if (!shift) {
		//	Unselect();
		//}
		ProcessIndividualSelection(SelectionBuffer, hits);
	}
	}
	}
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void gridclass::ProcessSelection(GLuint *buffer, GLuint hits, bool All, bool Shift)
{
	double z1, z2, shortest;
	unsigned int id, id2;

	GLuint *ptr, names;
	ptr = (GLuint*) buffer;
	
	if (All && !Shift)
		Unselect();

	if (hits > 0)
	{
		for (unsigned i = 0 ; i < hits && i < 80; i++) //max # hits == 80
		{
			names = *ptr;
			ptr++;


			z1 = (double) *ptr/0x7fffffff;
			ptr++;

			z2 = (double) *ptr/0x7fffffff;
			ptr++;

			if (i == 0)
			{
				shortest = z1;
				id = *ptr;
				id2 = *(ptr+1);
				if (All)
					Grid[id2][id].Select(All, Shift);
			}
		    else
			{
				if (All) {
					id = *ptr;
					id2 = *(ptr+1);
					Grid[id2][id].Select(All, Shift);
				}
				if (z1 < shortest) {
					id = *ptr;
					id2 = *(ptr+1);
					shortest = z1;
				}
			}
			for (unsigned j = 0 ; j < names; j++)
			{
				 ptr++;
			}
		}
		if (!All)
			if (Grid[id2][id].Select(All, Shift) == true && !Shift) {
				Unselect();
				Grid[id2][id].Select(All, Shift);
			}


	}
}

void gridclass::ProcessIndividualSelection(GLuint *buffer, GLuint hits) //for sides, texturing
{
	double z1, z2, shortest;
	unsigned int id, id2, id3;

	GLuint *ptr, names;
	ptr = (GLuint *) buffer;
	
	if (hits > 0 )
	{
		for (unsigned i = 0 ; i < hits ; i++)
		{
			names = *ptr;
			ptr++;

			z1 = (double) *ptr/0x7fffffff;
			ptr++;
			z2 = (double) *ptr/0x7fffffff;
			ptr++;

			if (i == 0)
			{
				shortest = z1;
				id = *ptr;
				id2 = *(ptr+1);
				id3 = *(ptr+2);
			}
		    else
			{
				if (z1 < shortest) {
					id = *ptr;
					id2 = *(ptr+1);
					id3 = *(ptr+2);
					shortest = z1;
				}
			}
			for (unsigned j = 0 ; j < names; j++)
			{
				 ptr++ ;
			}
	}
	Grid[id2][id].Select(id3);
}
}

void gridclass::ProcessSelection(GLuint *buffer)
{
	Grid[buffer[4]][buffer[3]].SelectHeight(buffer[5]);
}

void gridclass::EditTerrain(bool &up, bool &down, bool &left, bool &right)
{
	int x, y;
	for (y = 0; y < Height; y++) {
		for (x = 0; x < Width; x++) {
			Grid[x][y].EditTile(up, down, left, right);
		}
	}
}

void gridclass::WriteData(ostream& os, int number)
{
	int x, y;
	os << Width << " " << Height;
	for (y = 0; y < Height; y++) {
		for (x = 0; x < Width; x++) {
			Grid[x][y].WriteTile(os, number, Width, Height, Grid);
		}
	}
}

void gridclass::DrawSelection()
{
	int y, x, s;
	glDisable(GL_DEPTH_TEST);
	glPushAttrib(GL_POINT_BIT);
	glPointSize(7.0f);
	for (y = 0; y < Height; y++) {
		for (x = 0; x < Width; x++) {
			for (s = 1; s < 5; s++) {
			Grid[x][y].DrawSelectionPoints(s);
			}
		}
	}
	glEnable(GL_DEPTH_TEST);
	glPopAttrib();
}