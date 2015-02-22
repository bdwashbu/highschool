#ifndef _GRID_H_
#define _GRID_H_

#include "tile.h"
#include <iostream.h>

class gridclass
{
public:
	gridclass(int X, int Y);
	gridclass(istream &is);
	~gridclass();
	void DrawGrid();
	void DrawSelection();
	void Selection(int X, int Y, float XView, float YView, float Rotation, bool shift, bool ctrl);
	void EditTerrain(bool &up, bool &down, bool &left, bool &right);
	void WriteData(ostream &os, int number);
	void Unselect();
	void Select();
	void ProcessSelection(GLuint *buffer, GLuint hits, bool All, bool Shift);
	void ProcessSelection(GLuint *buffer);
	void ProcessIndividualSelection(GLuint *buffer, GLuint hits);

private:
	int Width;
	int Height;
	tileclass **Grid;
};

#endif