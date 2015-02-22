#include <windows.h>		
#include <gl\gl.h>			
#include <gl\glu.h>

#include "puzzle.h"

puzzleclass::puzzleclass()
{
	Restart();
	position = 4;
	Speed = 55.0f;
	hit = false;
	hit2 = false;
}

void puzzleclass::DrawPiece(void)
{
	glColor4f(Red, Green, Blue, 1.0f);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2i(1,1); glVertex2f(X*16.0f+144.0f, Y+16.0f+8.0f);
		glTexCoord2i(0,1); glVertex2f(X*16.0f+128.0f, Y+16.0f+8.0f);
		glTexCoord2i(1,0); glVertex2f(X*16.0f+144.0f, Y+16.0f-8.0f);
		glTexCoord2i(0,0); glVertex2f(X*16.0f+128.0f, Y+16.0f-8.0f);
	glEnd();
	glColor4f(Red2, Green2, Blue2, 1.0f);
	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2i(1,1); glVertex2f(X2*16.0f+144.0f, Y2+16.0f+8.0f);
		glTexCoord2i(0,1); glVertex2f(X2*16.0f+128.0f, Y2+16.0f+8.0f);
		glTexCoord2i(1,0); glVertex2f(X2*16.0f+144.0f, Y2+16.0f-8.0f);
		glTexCoord2i(0,0); glVertex2f(X2*16.0f+128.0f, Y2+16.0f-8.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

bool puzzleclass::DetectCollison(float scalar, gridclass &grid)
{
	float temp2 = ((Y-18)-1)/16.0f+1;
	int temp = ((Y-18))/16+1;
	if (temp2 < 0)
	{
		timer-=scalar;
		Y = 2.0f;
		hit = true;
	}
	else if (grid.Taken[X][temp] == true)
	{
		timer-=scalar;
		hit = true;
	}

	temp2 = ((Y2-18)-1)/16.0f+1;
	temp = ((Y2-18))/16+1;
	if (temp2 < 0)
	{
		timer2-=scalar;
		Y2 = 2.0f;
		hit2 = true;
	}
	else if (grid.Taken[X2][temp] == true)
	{
		timer2-=scalar;
		hit2 = true;
	}


	if (hit) {
		int temp = ((Y-2)-1)/16+1;
		grid.Red[X][temp] = Red;
		grid.Green[X][temp] = Green;
		grid.Blue[X][temp] = Blue;
		grid.Taken[X][temp] = true;
		grid.BlockType[X][temp] = 2;
	}
	if (hit2) {
		temp = ((Y2-2)-1)/16+1;
		grid.Red[X2][temp] = Red2;
		grid.Green[X2][temp] = Green2;
		grid.Blue[X2][temp] = Blue2;
		grid.Taken[X2][temp] = true;
		grid.BlockType[X2][temp] = 2;
	}
	if (timer < 0 && timer2 < 0 && hit && hit2) {
		return true;
		hit = false;
		hit2 = false;
	}
	else {
		if (hit == false) {
			timer = 0.4f;
			if (hit2) 
				Speed = 55*8;
			Y-=scalar*Speed;
		}
		if (hit2 == false) {
			timer2 = 0.4f;
			if (hit) 
				Speed = 55*8;
			Y2-=scalar*Speed;
		}
		return false;
	}
}

void puzzleclass::HandleInput(bool &left, bool &right, bool &down, bool &up, gridclass &grid, FSOUND_SAMPLE *flip)
{
	int highest = 0;
	bool breakit = false;
	if (left && hit == false)	{
	int hit = false;
		int temp = (Y+16-10)/16;
		int temp2 = (Y+16-3)/16;
		int temp3 = (Y+16-17)/16;
		if (X < 1 || grid.Taken[X-1][temp] == true || grid.Taken[X-1][temp2] == true || grid.Taken[X-1][temp3] == true ||
			(X < 2 && X2 == X-1)) 
			hit = true;
		temp = (Y2+16-10)/16;
		temp2 = (Y2+16-3)/16;
		temp3 = (Y2+16-17)/16;
		if (X2 < 1 || grid.Taken[X2-1][temp] == true || grid.Taken[X2-1][temp2] == true || grid.Taken[X2-1][temp3] == true ||
			(X2 < 2 && X == X2-1)) 
			hit = true;

	if (hit == false)	{
		X-=1; X2-=1;
		left = false;
	}
	}

	if (right)	{
	int hit = false;
		int temp = (Y+16-10)/16;
		int temp2 = (Y+16-3)/16;
		int temp3 = (Y+16-17)/16;
		if (X > 22 || grid.Taken[X+1][temp] == true || grid.Taken[X+1][temp2] == true || grid.Taken[X+1][temp3] == true ||
			(X > 21 && X2 == X+1))
			hit = true;
		temp = (Y2+16-10)/16;
		temp2 = (Y2+16-3)/16;
		temp3 = (Y2+16-17)/16;
		if (X2 > 22 || grid.Taken[X2+1][temp] == true || grid.Taken[X2+1][temp2] == true || grid.Taken[X2+1][temp3] == true ||
			(X2 > 21 && X == X2+1))
			hit = true;

	if (hit == false)	{
		X+=1; X2+=1;
		right = false;
	}
	}

	if (down)	{
	Speed = 55*8;
	//down = false;
	}
	else
		Speed = 55;

	if (up)
	{
		FSOUND_PlaySound(FSOUND_FREE, flip);
		int temp = (Y+16-10)/16;
		if ((position == 4 && grid.Taken[X][temp-1] == false) || (grid.Taken[X+1][temp] == true && grid.Taken[X-1][temp] == true && position == 1)) {
			position = 3;
			Y2 = Y-16.0f;
			X2 = X;
		}
		else if ((position == 3 && grid.Taken[X-1][temp] == false) || grid.Taken[X][temp-1] == true) {
			position = 2;
			X2 = X-1;
			Y2 = Y;
		}
		else if ((position == 2 && grid.Taken[X][temp+1] == false) || (grid.Taken[X-1][temp] == true && grid.Taken[X+1][temp] == true && position == 3)) {
			position = 1;
			X2 = X;
			Y2 = Y+16.0f;
		}
		else if ((position == 1 & grid.Taken[X+1][temp] == false) || grid.Taken[X+1][temp+1] == true) {
			position = 4;
			X2 = X+1;
			Y2 = Y;
		}




	up = false;
	}
}

void puzzleclass::Restart()
{
	X=16; Y=420.0f;
	int tempcolor = int(rand()%7);
	if (tempcolor == 0) 
	{
		Red = 1.0f;		Green = 0.15f;		Blue = 0.15f;
	}
	else if (tempcolor == 1) //orange
	{
		Red = 0.98f;	Green = 0.725f;		Blue = 0.28f;
	}
	else if (tempcolor == 2) //cyan
	{
		Red = 0.0f;		Green = 1.0f;		Blue = 1.0f;
	}
	else if (tempcolor == 3) //purple
	{
		Red = 1.0f;		Green = 0.0f;		Blue = 1.0f;
	}
	else if (tempcolor == 4) //yellow
	{
		Red = 1.0f;		Green = 1.0f;		Blue = 0.0f;
	}
	else if (tempcolor == 5) //green
	{
		Red = 0.1f;		Green = 1.00f;		Blue = 0.1f;
	}
	else if (tempcolor == 6) //blue
	{
		Red = 0.2f;		Green = 0.2f;		Blue = 1.0f;
	}

	X2=17; Y2=420.0f;
	tempcolor = int(rand()%7);
	if (tempcolor == 0) 
	{
		Red2 = 1.0f;		Green2 = 0.15f;		Blue2 = 0.15f;
	}
	else if (tempcolor == 1) //orange
	{
		Red2 = 0.98f;	Green2 = 0.725f;		Blue2 = 0.28f;
	}
	else if (tempcolor == 2) //cyan
	{
		Red2 = 0.0f;		Green2 = 1.0f;		Blue2 = 1.0f;
	}
	else if (tempcolor == 3) //purple
	{
		Red2 = 1.0f;		Green2 = 0.0f;		Blue2 = 1.0f;
	}
	else if (tempcolor == 4) //yellow
	{
		Red2 = 1.0f;		Green2 = 1.0f;		Blue2 = 0.0f;
	}
	else if (tempcolor == 5) //green
	{
		Red2 = 0.1f;		Green2 = 1.00f;		Blue2 = 0.1f;
	}
	else if (tempcolor == 6) //blue
	{
		Red2 = 0.2f;		Green2 = 0.2f;		Blue2 = 1.0f;
	}

}