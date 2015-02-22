#ifndef _TILE_H_
#define _TILE_H_

#include <iostream.h>

class tileclass
{
public:
	tileclass();
	void Init(int X, int Y);
	void Init(istream &is, int X, int Y);
	void EditTile(bool &up, bool &down, bool &left, bool &right);
	void DrawTile(int Xlimit, int Ylimit, tileclass **Grid);
	void DrawTile(int s);
	void WriteTile(ostream &os, int number, int Xlimit, int Ylimit, tileclass **Grid);
	void Unselect();
	bool Select(bool All, bool Shift);
	void SelectHeight(int s);
	void Select(int s);
	void DrawOutline(bool Selected, int Xlimit, int Ylimit, tileclass **Grid);
	void DrawSelectionPoints(int s);
	int ReturnHeight(int Number);
private:
	float Brightness1;
	float Brightness2;
	float Brightness3;
	float Brightness4;
	int Height1Tally;
	int Height2Tally;
	int Height3Tally;
	int Height4Tally;
	int XOffset, YOffset;
	int Side11Texture;
	int Side12Texture;
	int Side13Texture;
	int Side14Texture;
	int Side2Texture;
	int Side3Texture;
	int Side4Texture;
	int Side5Texture;
	bool Height1Selected;
	bool Height2Selected;
	bool Height3Selected;
	bool Height4Selected;
	bool Side1Selected;
	bool Side2Selected;
	bool Side3Selected;
	bool Side4Selected;
	bool Side5Selected;
};

#endif