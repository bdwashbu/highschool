#include <windows.h>		
#include <gl\gl.h>			
#include <gl\glu.h>
#include <math.h>
#include <time.h>		
#include "block.h"
#include "grid.h"
//#include "fmod.h"

//extern const GRIDX;
int GRIDX = 12;

blockclass::blockclass()
{
	//srand( time(NULL) );
	BlockLife = 0.0f;
	timer = 0.25f;
	Speed = 55.0f;
	blurlife = 0.0f;
	hit = false;
}

void blockclass::DrawPiece(float scalar, float time)
{
	if (blurlife > 0.0f)
		blurlife -=scalar*4.5f;
	else
		blurlife = 0.0f;
	if (BlockLife < 1.0f)
		BlockLife+=scalar*1.5f;
	else
		BlockLife = 1.0f;

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_TEXTURE_2D);
	for (int i = 0; i < 4; i++)
	{
		glColor4f(Red, Green, Blue, 0.0f);
		glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2i(1,1); glVertex2f(AbsoluteX+(offsetx[i]*24.0f)+12.0f, blurypos);
		glTexCoord2i(0,1); glVertex2f(AbsoluteX+(offsetx[i]*24.0f)-12.0f, blurypos);
		glColor4f(Red, Green, Blue, blurlife);
		glTexCoord2i(1,0); glVertex2f(AbsoluteX+(offsetx[i]*24.0f)+12.0f, Y+(offsety[i]*24.0f)-12.0f);
		glTexCoord2i(0,0); glVertex2f(AbsoluteX+(offsetx[i]*24.0f)-12.0f, Y+(offsety[i]*24.0f)-12.0f);
		glEnd();
	}
	if (blurlife == 0.0f)
		glColor4f(Red, Green, Blue, BlockLife);
	else
		glColor4f(Red, Green, Blue, (1.0f-blurlife));
	//glDisable(GL_BLEND);
	for (i = 0; i < 4; i++)
	{
		glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2i(1,1); glVertex2f((offsetx[i]*24.0f)+AbsoluteX+12.0f, Y+(offsety[i]*24.0f)+12.0f);
		glTexCoord2i(0,1); glVertex2f((offsetx[i]*24.0f)+AbsoluteX-12.0f, Y+(offsety[i]*24.0f)+12.0f);
		glTexCoord2i(1,0); glVertex2f((offsetx[i]*24.0f)+AbsoluteX+12.0f, Y+(offsety[i]*24.0f)-12.0f);
		glTexCoord2i(0,0); glVertex2f((offsetx[i]*24.0f)+AbsoluteX-12.0f, Y+(offsety[i]*24.0f)-12.0f);
		glEnd();
	}
	//glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc(GL_ONE, GL_ONE);
	//glLineWidth(1.5f);

	glColor4f(1.0f-(timer/0.25f), 1.0f-(timer/0.25f), 1.0f-(timer/0.25f), 0.5f);
	glDisable(GL_TEXTURE_2D);
	bool passed = true;
	glBegin(GL_LINES);
	for (i = 0; i < 4; i++)
	{
		passed = true;
		for (int h = 0; h < 4; h++)
		{
			if (offsetx[h] == offsetx[i]+1 && offsety[i] == offsety[h]) {
				passed = false;
				break;
			}
		}
		if (passed == true) {
				glVertex2i(AbsoluteX+(offsetx[i]*24)+12, int(Y)+(offsety[i]*24)+12);
				glVertex2i(AbsoluteX+(offsetx[i]*24)+12, int(Y)+(offsety[i]*24)-12);
		}

		passed = true;
		for (h = 0; h < 4; h++)
		{
			if (offsetx[h] == offsetx[i]-1 && offsety[i] == offsety[h]) {
				passed = false;
				break;
			}
		}
		if (passed == true) {
				glVertex2i(AbsoluteX+(offsetx[i]*24)-12, int(Y)+(offsety[i]*24)+12);
				glVertex2i(AbsoluteX+(offsetx[i]*24)-12, int(Y)+(offsety[i]*24)-12);
		}

		passed = true;
		for (h = 0; h < 4; h++)
		{
			if (offsety[h] == offsety[i]+1 && offsetx[i] == offsetx[h]) {
				passed = false;
				break;
			}
		}
		if (passed == true) {
				glVertex2i(AbsoluteX+(offsetx[i]*24)-12, int(Y)+(offsety[i]*24)+12);
				glVertex2i(AbsoluteX+(offsetx[i]*24)+12, int(Y)+(offsety[i]*24)+12);
		}

		passed = true;
		for (h = 0; h < 4; h++)
		{
			if (offsety[h] == offsety[i]-1 && offsetx[i] == offsetx[h]) {
				passed = false;
				break;
			}
		}
		if (passed == true) {
				glVertex2i(AbsoluteX+(offsetx[i]*24)-12, int(Y)+(offsety[i]*24)-12);
				glVertex2i(AbsoluteX+(offsetx[i]*24)+12, int(Y)+(offsety[i]*24)-12);
		}
	}
	glEnd();
}

bool blockclass::DetectCollison(float scalar, gridclass &grid, float tempspeed, bool &space)
{

	hit = false;
	int highesti2 = 0;
	bool hitground = false;
	float CurrentSpeed = 0;
	if (space)
		CurrentSpeed = 250;
	else
		CurrentSpeed = tempspeed;

	//if (hit == false) {
	int beforemove = int((Y-10-12)/24.0f);
	int aftermove = int((Y-10-12-(CurrentSpeed*scalar))/24.0f);

	for (int i = 0; i < 4; i++)	{
	if (Y+offsety[i]*24-10-13-(CurrentSpeed*scalar) < 0)
	{
		Y = 22.0f-offsety[i]*24.0f;
		hit = true;
		hitground = true;
		break;
	}
	else {
	for (int i2 = beforemove; i2 > aftermove || i2 < 0; i2--) {
		if (grid.Taken[X+offsetx[i]][i2+offsety[i]-1] == true)
		{
		if (i2 > highesti2)
			highesti2 = i2;
		Y=(highesti2)*24.0f+12.0f+10.0f;

		//if ((i2)*24.0f+12.0f+10 > Y)
		//	Y=(i2)*24.0f+12.0f+10;
		hit = true;
		break;
		}
	}
	}
	//}
	}

	if (hit == true && !(hitground == false && grid.Tetris == true))
		timer-=scalar;
	if (timer < 0 && hit == true) {
		int temp = int((Y-10-12)/24.0f);
		for (int i = 0; i < 4; i++)	{
			grid.Red[X+offsetx[i]][temp+offsety[i]] = Red;
			grid.Green[X+offsetx[i]][temp+offsety[i]] = Green;
			grid.Blue[X+offsetx[i]][temp+offsety[i]] = Blue;
			grid.Taken[X+offsetx[i]][temp+offsety[i]] = true;
		}
		return true;
	}
	else {
	if (hit == false) {
		if (space == false)
			Y-=scalar*tempspeed;
		else
			Y-=scalar*250.0f;
	}
	return false;
	}
}

void blockclass::AssignColor()
{
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
}


void blockclass::MoveDown(gridclass &grid, FSOUND_SAMPLE *stomp, bool &down)
{
	int highest = 0;
	int temp = int((Y-10-12)/24);
	if (grid.Taken[X+offsetx[0]][temp-1+offsety[0]] == false && grid.Taken[X+offsetx[1]][temp-1+offsety[1]] == false
		&& grid.Taken[X+offsetx[2]][temp-1+offsety[2]] == false && grid.Taken[X+offsetx[3]][temp-1+offsety[3]] == false) {
	blurypos = Y; blurlife = 1.0f;
	for (int f = 0; f < 4; f++)	{
		for (int i = temp; i > 0; i--)	{
			if (grid.Taken[X+offsetx[f]][i-1+offsety[f]] != false)	{
				if (i > highest)
					highest = i;
				break;
			}
			else if (-offsety[f] > highest)
				highest = -offsety[f];
		}
	}
	FSOUND_PlaySound(FSOUND_FREE, stomp);
	Y = (highest)*24.0f+22.0f;
	hit = true;
	down = false;
	}
}

void blockclass::MoveLeft(gridclass &grid, bool &left, float scalar)
{
	static float lefttimer= 0.0f;
	int lefthit = false;
	for (int i = 0; i < 4; i++) {
		int temp = int((Y+(offsety[i]*24)-10)/24);
		int temp2 = int((Y+(offsety[i]*24)-3)/24);
		int temp3 = int((Y+(offsety[i]*24)-17)/24);
		if (X+offsetx[i] < 1 || grid.Taken[X-1+offsetx[i]][temp] == true || grid.Taken[X-1+offsetx[i]][temp2] == true || grid.Taken[X-1+offsetx[i]][temp3] == true) {
			lefthit = true;
			break;
		}
	}
	if (lefthit == false)	{
		X-=1;
		hit = false;
		AbsoluteX-=24;
		//left = false;
	}
}

void blockclass::MoveRight(gridclass &grid, bool &right, float scalar)
{
	int righthit = false;
	for (int i = 0; i < 4; i++) {
		int temp = int((Y+(offsety[i]*24)-10)/24);
		int temp2 = int((Y+(offsety[i]*24)-3)/24);
		int temp3 = int((Y+(offsety[i]*24)-17)/24);
		if (X+offsetx[i] > 10 || grid.Taken[X+1+offsetx[i]][temp] == true || grid.Taken[X+1+offsetx[i]][temp2] == true || grid.Taken[X+1+offsetx[i]][temp3] == true){
			righthit = true;
			break;
		}
	}
	if (righthit == false)	{
		hit = false;
		X+=1;
		AbsoluteX+=24;
		//right = false;
	}
}

void stick::HandleInput(bool &left, bool &right, bool &down, bool &up, gridclass &grid, FSOUND_SAMPLE *flip, FSOUND_SAMPLE *stomp, float scalar)
{
	bool flipped = false;
	static float lefttimer = 0.0f;
	static float righttimer = 0.0f;
	static float keyholdlength = 0.0f;
	if (left && right) {
		righttimer = 0.0f;
		lefttimer = 0.0f;
	}
	if (left)
		lefttimer+=scalar;
	else if (right == false) {
		keyholdlength = 0.0f;
		lefttimer = 1.0f;
	}
	if (right)
		righttimer+=scalar;
	else if (left == false) {
		righttimer = 1.0f;
		keyholdlength = 0.0f;
	}
	if (lefttimer > keyholdlength && left) {
		if (keyholdlength == 0.0f)
			keyholdlength = 0.15f;
		else if (keyholdlength == 0.15f)
			keyholdlength = 0.07f;
		MoveLeft(grid, left, scalar);
		lefttimer = 0.0f;
	}
	else if (righttimer > keyholdlength && right) {
		if (keyholdlength == 0.0f)
			keyholdlength = 0.15f;
		else if (keyholdlength == 0.15f)
			keyholdlength = 0.07f;
		MoveRight(grid, right, scalar);
		righttimer = 0.0f;
	}
	if (down)
		MoveDown(grid, stomp, down);
	else if (up && !(lefttimer == 0.0f && left) && !(righttimer = 0.0f && right))
	{
		int temp = int((Y+4)/24-1);
		if (Rotation == 0 && X < GRIDX-2 && grid.Taken[X+1][temp] == false && X-1 > -1 && X+2 < 24 && grid.Taken[X+2][temp] == false && grid.Taken[X-1][temp] == false)
		{
			offsetx[0] = 0;		offsety[0] = 0;
			offsetx[1] = 1;		offsety[1] = 0;
			offsetx[2] = 2;		offsety[2] = 0;
			offsetx[3] = -1;		offsety[3] = 0;
			if (Rotation + 90 < 360)
				Rotation+=90;
			else
				Rotation = 0;
			flipped = true;
		}
		else if (Rotation == 0 && X < GRIDX-2 && grid.Taken[X+1][temp-1] == false && X-1 > -1 && X+2 < 24 && grid.Taken[X+2][temp-1] == false && grid.Taken[X-1][temp-1] == false) {
			offsetx[0] = 0;		offsety[0] = 0;
			offsetx[1] = 1;		offsety[1] = 0;
			offsetx[2] = 2;		offsety[2] = 0;
			offsetx[3] = -1;		offsety[3] = 0;
			if (Rotation + 90 < 360)
				Rotation+=90;
			else
				Rotation = 0;
			Y-=24;
			flipped = true;
		}
		else if (Rotation == 90 && temp-1 > 0 && temp < 18 && grid.Taken[X][temp+2] == false && grid.Taken[X][temp+1] == false && grid.Taken[X][temp-1] == false)
		{
			offsetx[0] = 0;		offsety[0] = 0;
			offsetx[1] = 0;		offsety[1] = 1;
			offsetx[2] = 0;		offsety[2] = 2;
			offsetx[3] = 0;		offsety[3] = -1;
			if (Rotation + 90 < 360)
				Rotation+=90;
			else
				Rotation = 0;
			Rotation = 0;
			flipped = true;
		}
		up = false;
		if (flipped)
			FSOUND_PlaySound(FSOUND_FREE, flip);
	}
}

void square::HandleInput(bool &left, bool &right, bool &down, bool &up, gridclass &grid, FSOUND_SAMPLE *flip, FSOUND_SAMPLE *stomp, float scalar)
{
	bool flipped = false;
	static float lefttimer = 0.0f;
	static float righttimer = 0.0f;
	static float keyholdlength = 0.0f;
	if (left && right) {
		righttimer = 0.0f;
		lefttimer = 0.0f;
	}
	if (left)
		lefttimer+=scalar;
	else if (right == false) {
		keyholdlength = 0.0f;
		lefttimer = 1.0f;
	}
	if (right)
		righttimer+=scalar;
	else if (left == false) {
		righttimer = 1.0f;
		keyholdlength = 0.0f;
	}
	if (lefttimer > keyholdlength && left) {
		if (keyholdlength == 0.0f)
			keyholdlength = 0.15f;
		else if (keyholdlength == 0.15f)
			keyholdlength = 0.07f;
		MoveLeft(grid, left, scalar);
		lefttimer = 0.0f;
	}
	else if (righttimer > keyholdlength && right) {
		if (keyholdlength == 0.0f)
			keyholdlength = 0.15f;
		else if (keyholdlength == 0.15f)
			keyholdlength = 0.07f;
		MoveRight(grid, right, scalar);
		righttimer = 0.0f;
	}
	if (down)
		MoveDown(grid, stomp, down);
}

void curveleft::HandleInput(bool &left, bool &right, bool &down, bool &up, gridclass &grid, FSOUND_SAMPLE *flip, FSOUND_SAMPLE *stomp, float scalar)
{
	bool flipped = false;
	static float lefttimer = 0.0f;
	static float righttimer = 0.0f;
	static float keyholdlength = 0.0f;
	if (left && right) {
		righttimer = 0.0f;
		lefttimer = 0.0f;
	}
	if (left)
		lefttimer+=scalar;
	else if (right == false) {
		keyholdlength = 0.0f;
		lefttimer = 1.0f;
	}
	if (right)
		righttimer+=scalar;
	else if (left == false) {
		righttimer = 1.0f;
		keyholdlength = 0.0f;
	}
	if (lefttimer > keyholdlength && left) {
		if (keyholdlength == 0.0f)
			keyholdlength = 0.15f;
		else if (keyholdlength == 0.15f)
			keyholdlength = 0.07f;
		MoveLeft(grid, left, scalar);
		lefttimer = 0.0f;
	}
	else if (righttimer > keyholdlength && right) {
		if (keyholdlength == 0.0f)
			keyholdlength = 0.15f;
		else if (keyholdlength == 0.15f)
			keyholdlength = 0.07f;
		MoveRight(grid, right, scalar);
		righttimer = 0.0f;
	}
	if (down)
		MoveDown(grid, stomp, down);
	else if (up && !(lefttimer == 0.0f && left) && !(righttimer = 0.0f && right))
	{
		int temp = int((Y+4)/24-1);
		if (Rotation == 0 && temp-1 > 0 && grid.Taken[X+1][temp] == false && grid.Taken[X][temp] == false && grid.Taken[X+1][temp+1] == false && grid.Taken[X][temp-1] == false)
		{
			offsetx[0] = 0;		offsety[0] = 0;
			offsetx[1] = 1;		offsety[1] = 0;
			offsetx[2] = 1;		offsety[2] = 1;
			offsetx[3] = 0;		offsety[3] = -1;
			if (Rotation + 90 < 360)
				Rotation+=90;
			else
				Rotation = 0;
			flipped = true;
		}
		else if (Rotation == 90 && X-1 > -1 && X+1 < GRIDX && grid.Taken[X-1][temp+1] == false && grid.Taken[X][temp+1] == false && grid.Taken[X+1][temp] == false && grid.Taken[X+1][temp-1]  == false)
		{
			offsetx[0] = 1;		offsety[0] = 0;
			offsetx[1] = 0;		offsety[1] = 0;
			offsetx[2] = 0;		offsety[2] = 1;
			offsetx[3] = -1;	offsety[3] = 1;
			if (Rotation + 90 < 360)
				Rotation+=90;
			else
				Rotation = 0;
			Rotation = 0;
			flipped = true;
		}
		else if (Rotation == 90 && X-1 > -2 && X+2 < GRIDX && grid.Taken[X][temp] == false && grid.Taken[X+1][temp] == false && grid.Taken[X+2][temp-1] == false && grid.Taken[X+1][temp-1] == false)
		{
			offsetx[0] = 1;		offsety[0] = 0;
			offsetx[1] = 0;		offsety[1] = 0;
			offsetx[2] = 0;		offsety[2] = 1;
			offsetx[3] = -1;	offsety[3] = 1;
			if (Rotation + 90 < 360)
				Rotation+=90;
			else
				Rotation = 0;
			Rotation = 0;
			Y-=24;
			X++; AbsoluteX+=24;
			flipped = true;
		}
		up = false;
		if (flipped) {
			FSOUND_PlaySound(FSOUND_FREE, flip);
			hit = false;
		}
	}
}

void curveright::HandleInput(bool &left, bool &right, bool &down, bool &up, gridclass &grid, FSOUND_SAMPLE *flip, FSOUND_SAMPLE *stomp, float scalar)
{
	bool flipped = false;
	static float lefttimer = 0.0f;
	static float righttimer = 0.0f;
	static float keyholdlength = 0.0f;
	if (left && right) {
		righttimer = 0.0f;
		lefttimer = 0.0f;
	}
	if (left)
		lefttimer+=scalar;
	else if (right == false) {
		keyholdlength = 0.0f;
		lefttimer = 1.0f;
	}
	if (right)
		righttimer+=scalar;
	else if (left == false) {
		righttimer = 1.0f;
		keyholdlength = 0.0f;
	}
	if (lefttimer > keyholdlength && left) {
		if (keyholdlength == 0.0f)
			keyholdlength = 0.15f;
		else if (keyholdlength == 0.15f)
			keyholdlength = 0.07f;
		MoveLeft(grid, left, scalar);
		lefttimer = 0.0f;
	}
	else if (righttimer > keyholdlength && right) {
		if (keyholdlength == 0.0f)
			keyholdlength = 0.15f;
		else if (keyholdlength == 0.15f)
			keyholdlength = 0.07f;
		MoveRight(grid, right, scalar);
		righttimer = 0.0f;
	}
	if (down)
		MoveDown(grid, stomp, down);
	else if (up && !(lefttimer == 0.0f && left) && !(righttimer = 0.0f && right))
	{
		int temp = int((Y+4)/24-1);
		if (Rotation == 0 && temp-1> 0 && X+1 < GRIDX && grid.Taken[X-1][temp+1] == false && grid.Taken[X-1][temp] == false && grid.Taken[X][temp] == false && grid.Taken[X][temp-1] == false)
		{
			offsetx[0] = -1;	offsety[0] = 1;
			offsetx[1] = -1;	offsety[1] = 0;
			offsetx[2] = 0;		offsety[2] = 0;
			offsetx[3] = 0;		offsety[3] = -1;
			if (Rotation + 90 < 360)
				Rotation+=90;
			else
				Rotation = 0;
			flipped = true;
		}
		else if (Rotation == 90 && X+1 < GRIDX && X-1 > -1 && grid.Taken[X+1][temp+1] == false && grid.Taken[X][temp+1] == false && grid.Taken[X-1][temp] == false && grid.Taken[X][temp] == false && grid.Taken[X-1][temp-1] == false)
		{
			offsetx[0] = 1;		offsety[0] = 1;
			offsetx[1] = 0;		offsety[1] = 1;
			offsetx[2] = 0;		offsety[2] = 0;
			offsetx[3] = -1;		offsety[3] = 0;
			if (Rotation + 90 < 360)
				Rotation+=90;
			else
				Rotation = 0;
			Rotation = 0;
			flipped = true;
		}
		else if (Rotation == 90 && X < GRIDX && X-2 > -1 && grid.Taken[X-2][temp-1] == false && grid.Taken[X-1][temp-1] == false && grid.Taken[X-1][temp] == false && grid.Taken[X][temp] == false)
		{
			offsetx[0] = 1;		offsety[0] = 1;
			offsetx[1] = 0;		offsety[1] = 1;
			offsetx[2] = 0;		offsety[2] = 0;
			offsetx[3] = -1;		offsety[3] = 0;
			if (Rotation + 90 < 360)
				Rotation+=90;
			else
				Rotation = 0;
			Rotation = 0;
			Y-=24;
			X--; AbsoluteX-=24;
			flipped = true;
		}
		up = false;
		if (flipped) {
			FSOUND_PlaySound(FSOUND_FREE, flip);
			hit = false;
		}
	}
}

void longcurveright::HandleInput(bool &left, bool &right, bool &down, bool &up, gridclass &grid, FSOUND_SAMPLE *flip, FSOUND_SAMPLE *stomp, float scalar)
{
	bool flipped = false;
	static float lefttimer = 0.0f;
	static float righttimer = 0.0f;
	static float keyholdlength = 0.0f;
	if (left && right) {
		righttimer = 0.0f;
		lefttimer = 0.0f;
	}
	if (left)
		lefttimer+=scalar;
	else if (right == false) {
		keyholdlength = 0.0f;
		lefttimer = 1.0f;
	}
	if (right)
		righttimer+=scalar;
	else if (left == false) {
		righttimer = 1.0f;
		keyholdlength = 0.0f;
	}
	if (lefttimer > keyholdlength && left) {
		if (keyholdlength == 0.0f)
			keyholdlength = 0.15f;
		else if (keyholdlength == 0.15f)
			keyholdlength = 0.07f;
		MoveLeft(grid, left, scalar);
		lefttimer = 0.0f;
	}
	else if (righttimer > keyholdlength && right) {
		if (keyholdlength == 0.0f)
			keyholdlength = 0.15f;
		else if (keyholdlength == 0.15f)
			keyholdlength = 0.07f;
		MoveRight(grid, right, scalar);
		righttimer = 0.0f;
	}
	if (down)
		MoveDown(grid, stomp, down);
	else if (up && !(lefttimer == 0.0f && left) && !(righttimer = 0.0f && right))
	{
		int temp = int((Y+4)/24-1);
		if (Rotation == 0 && X-1 > -1 && temp-1> 0 && grid.Taken[X][temp-1] == false && grid.Taken[X][temp+1] == false && grid.Taken[X+1][temp-1] == false)
		{
			offsetx[0] = -1;	offsety[0] = 1;
			offsetx[1] = 0;		offsety[1] = 1;
			offsetx[2] = 0;		offsety[2] = 0;
			offsetx[3] = 0;		offsety[3] = -1;
				if (Rotation + 90 < 360)
					Rotation+=90;
				else
					Rotation = 0;
				flipped = true;
		}
		else if (Rotation == 90 && X-1 > -1 && temp-1> 0 && X < GRIDX-1 && grid.Taken[X-1][temp-1] == false && grid.Taken[X+1][temp] == false && grid.Taken[X-1][temp] == false && grid.Taken[X][temp] == false)
		{
			offsetx[0] = -1;		offsety[0] = -1;
			offsetx[1] = 1;		offsety[1] = 0;
			offsetx[2] = 0;		offsety[2] = 0;
			offsetx[3] = -1;		offsety[3] = 0;
			if (Rotation + 90 < 360)
				Rotation+=90;
			else
				Rotation = 0;
			flipped = true;
		}
		else if (Rotation == 180 && X < GRIDX-1  && temp-1> 0 && grid.Taken[X][temp+1] == false && grid.Taken[X][temp] == false && grid.Taken[X][temp-1] == false && grid.Taken[X+1][temp-1] == false)
		{
			offsetx[0] = 0;		offsety[0] = 1;
			offsetx[1] = 0;		offsety[1] = 0;
			offsetx[2] = 0;		offsety[2] = -1;
			offsetx[3] = 1;		offsety[3] = -1;
			if (Rotation + 90 < 360)
				Rotation+=90;
			else
				Rotation = 0;
			flipped = true;
		}
		else if (Rotation == 270 && X-1 > -1 && X < GRIDX-1 && grid.Taken[X+1][temp+1] == false && grid.Taken[X+1][temp] == false && grid.Taken[X-1][temp] == false)
		{
			offsetx[0] = 1;		offsety[0] = 1;
			offsetx[1] = 1;		offsety[1] = 0;
			offsetx[2] = 0;		offsety[2] = 0;
			offsetx[3] = -1;		offsety[3] = 0;
			if (Rotation + 90 < 360)
				Rotation+=90;
			else
				Rotation = 0;
			flipped = true;
		}
		up = false;
		if (flipped) {
			FSOUND_PlaySound(FSOUND_FREE, flip);
			hit = false;
		}
	}
}

void longcurveleft::HandleInput(bool &left, bool &right, bool &down, bool &up, gridclass &grid, FSOUND_SAMPLE *flip, FSOUND_SAMPLE *stomp, float scalar)
{
	bool flipped = false;
	static float lefttimer = 0.0f;
	static float righttimer = 0.0f;
	static float keyholdlength = 0.0f;
	if (left && right) {
		righttimer = 0.0f;
		lefttimer = 0.0f;
	}
	if (left)
		lefttimer+=scalar;
	else if (right == false) {
		keyholdlength = 0.0f;
		lefttimer = 1.0f;
	}
	if (right)
		righttimer+=scalar;
	else if (left == false) {
		righttimer = 1.0f;
		keyholdlength = 0.0f;
	}
	if (lefttimer > keyholdlength && left) {
		if (keyholdlength == 0.0f)
			keyholdlength = 0.15f;
		else if (keyholdlength == 0.15f)
			keyholdlength = 0.07f;
		MoveLeft(grid, left, scalar);
		lefttimer = 0.0f;
	}
	else if (righttimer > keyholdlength && right) {
		if (keyholdlength == 0.0f)
			keyholdlength = 0.15f;
		else if (keyholdlength == 0.15f)
			keyholdlength = 0.07f;
		MoveRight(grid, right, scalar);
		righttimer = 0.0f;
	}
	if (down)
		MoveDown(grid, stomp, down);
	else if (up && !(lefttimer == 0.0f && left) && !(righttimer = 0.0f && right))
	{
		int temp = int((Y+4)/24-1);
			if (Rotation == 0 && X-1 > -1 && temp-1 > 0 && grid.Taken[X][temp-1] == false && grid.Taken[X][temp+1] == false && grid.Taken[X-1][temp-1] == false && grid.Taken[X][temp] == false)
			{
				offsetx[0] = 0;		offsety[0] = 1;
				offsetx[1] = 0;		offsety[1] = 0;
				offsetx[2] = 0;		offsety[2] = -1;
				offsetx[3] = -1;		offsety[3] = -1;
				if (Rotation + 90 < 360)
					Rotation+=90;
				else
					Rotation = 0;
				flipped = true;
			}
			else if (Rotation == 90 && X-1 > -1 && temp-1> 0 && X < GRIDX-1 && grid.Taken[X+1][temp+1] == false && grid.Taken[X+1][temp] == false && grid.Taken[X-1][temp] == false)
			{
				offsetx[0] = 1;		offsety[0] = -1;
				offsetx[1] = 1;		offsety[1] = 0;
				offsetx[2] = 0;		offsety[2] = 0;
				offsetx[3] = -1;		offsety[3] = 0;
				if (Rotation + 90 < 360)
					Rotation+=90;
				else
					Rotation = 0;
				flipped = true;
			}
			else if (Rotation == 180 && temp-1 > 0 && X < GRIDX-1 && grid.Taken[X+1][temp+1] == false && grid.Taken[X][temp+1] == false && grid.Taken[X][temp-1] == false && grid.Taken[X][temp] == false)
			{
				offsetx[0] = 1;		offsety[0] = 1;
				offsetx[1] = 0;		offsety[1] = 1;
				offsetx[2] = 0;		offsety[2] = 0;
				offsetx[3] = 0;		offsety[3] = -1;
				if (Rotation + 90 < 360)
					Rotation+=90;
				else
					Rotation = 0;
				flipped = true;
			}
			else if (Rotation == 270 && X-1>-1 && X < GRIDX-1 && grid.Taken[X-1][temp+1] == false && grid.Taken[X+1][temp] == false && grid.Taken[X-1][temp] == false)
			{
				offsetx[0] = -1;		offsety[0] = 1;
				offsetx[1] = 1;		offsety[1] = 0;
				offsetx[2] = 0;		offsety[2] = 0;
				offsetx[3] = -1;		offsety[3] = 0;
				if (Rotation + 90 < 360)
					Rotation+=90;
				else
					Rotation = 0;
				flipped = true;
			}
			up = false;
		if (flipped) {
			FSOUND_PlaySound(FSOUND_FREE, flip);
			hit = false;
		}
	}
}

void goodpiece::HandleInput(bool &left, bool &right, bool &down, bool &up, gridclass &grid, FSOUND_SAMPLE *flip, FSOUND_SAMPLE *stomp, float scalar)
{
	bool flipped = false;
	static float lefttimer = 0.0f;
	static float righttimer = 0.0f;
	static float keyholdlength = 0.0f;
	if (left && right) {
		righttimer = 0.0f;
		lefttimer = 0.0f;
	}
	if (left)
		lefttimer+=scalar;
	else if (right == false) {
		keyholdlength = 0.0f;
		lefttimer = 1.0f;
	}
	if (right)
		righttimer+=scalar;
	else if (left == false) {
		righttimer = 1.0f;
		keyholdlength = 0.0f;
	}
	if (lefttimer > keyholdlength && left) {
		if (keyholdlength == 0.0f)
			keyholdlength = 0.15f;
		else if (keyholdlength == 0.15f)
			keyholdlength = 0.07f;
		MoveLeft(grid, left, scalar);
		lefttimer = 0.0f;
	}
	else if (righttimer > keyholdlength && right) {
		if (keyholdlength == 0.0f)
			keyholdlength = 0.15f;
		else if (keyholdlength == 0.15f)
			keyholdlength = 0.07f;
		MoveRight(grid, right, scalar);
		righttimer = 0.0f;
	}
	if (down)
		MoveDown(grid, stomp, down);
	else if (up && !(lefttimer == 0.0f && left) && !(righttimer = 0.0f && right))
	{
		int temp = int((Y+4)/24-1);
			if (Rotation == 0 && X < GRIDX-1 && temp-1> 0 && grid.Taken[X+1][temp] == false && grid.Taken[X][temp+1] == false && grid.Taken[X][temp-1] == false)
			{
				offsetx[0] = 0;		offsety[0] = 0;
				offsetx[1] = -1;	offsety[1] = 0;
				offsetx[2] = 0;		offsety[2] = 1;
				offsetx[3] = 0;		offsety[3] = -1;

				if (Rotation + 90 < 360)
					Rotation+=90;
				else
					Rotation = 0;
				flipped = true;
			}
			else if (Rotation == 90 && temp-1> 0 && X < GRIDX-1 && X > 0 && grid.Taken[X+1][temp] == false && grid.Taken[X][temp-1] == false && grid.Taken[X-1][temp] == false)
			{
				offsetx[0] = 0;		offsety[0] = 0;
				offsetx[1] = 1;		offsety[1] = 0;
				offsetx[2] = 0;		offsety[2] = -1;
				offsetx[3] = -1;	offsety[3] = 0;
				if (Rotation + 90 < 360)
					Rotation+=90;
				else
					Rotation = 0;
				flipped = true;
			}
			else if (Rotation == 180 && temp-1> 0 && X > 0 && X < GRIDX-1 && grid.Taken[X-1][temp] == false && grid.Taken[X][temp+1] == false && grid.Taken[X][temp-1] == false)
			{
				offsetx[0] = 0;		offsety[0] = 0;
				offsetx[1] = 1;		offsety[1] = 0;
				offsetx[2] = 0;		offsety[2] = 1;
				offsetx[3] = 0;		offsety[3] = -1;
				if (Rotation + 90 < 360)
					Rotation+=90;
				else
					Rotation = 0;
				flipped = true;
			}
			else if (Rotation == 270 && X < GRIDX-1 && X > 0 && grid.Taken[X+1][temp] == false && grid.Taken[X-1][temp] == false && grid.Taken[X][temp+1] == false)
			{
				offsetx[0] = 0;		offsety[0] = 0;
				offsetx[1] = 1;		offsety[1] = 0;
				offsetx[2] = -1;	offsety[2] = 0;
				offsetx[3] = 0;		offsety[3] = 1;
				if (Rotation + 90 < 360)
					Rotation+=90;
				else
					Rotation = 0;
				flipped = true;
			}
			up = false;
		if (flipped) {
			FSOUND_PlaySound(FSOUND_FREE, flip);
			hit = false;
		}
	}
}

void stick::Restart(bool nextpiece)
{
	if (nextpiece) {
		AbsoluteX = 446; Y=305.0f; BlockLife = 0.5f;
	}
	else {
		X=6; AbsoluteX = 82+X*24; Y=451.0f;
	}
	Red = 1.0f;		Green = 0.15f;		Blue = 0.15f;
	Rotation = 90;
	offsetx[0] = 0; offsetx[1] = 1; offsetx[2] = 2; offsetx[3] = -1;
	offsety[0] = 0; offsety[1] = 0; offsety[2] = 0; offsety[3] = 0;
}

void square::Restart(bool nextpiece)
{
	if (nextpiece) {
		AbsoluteX = 446; Y=293.0f; BlockLife = 0.5f;
	}
	else {
		X=6; AbsoluteX = 82+X*24; Y=451.0f;
	}
	Red = 0.98f;	Green = 0.725f;		Blue = 0.28f;
	Rotation = 0;
	offsetx[0] = 1; offsetx[1] = 1; offsetx[2] = 0; offsetx[3] = 0;
	offsety[0] = 1; offsety[1] = 0; offsety[2] = 0; offsety[3] = 1;
}

void curveleft::Restart(bool nextpiece)
{
	if (nextpiece) {
		AbsoluteX = 457; Y=293.0f; BlockLife = 0.5f;
	}
	else {
		X=6; AbsoluteX = 82+X*24; Y=451.0f;
	}
	Red = 0.0f;		Green = 1.0f;		Blue = 1.0f;
	Rotation = 0;
	offsetx[0] = 1; offsetx[1] = 0; offsetx[2] = 0; offsetx[3] = -1;
	offsety[0] = 0; offsety[1] = 0; offsety[2] = 1; offsety[3] = 1;
}

void curveright::Restart(bool nextpiece)
{
	if (nextpiece) {
		AbsoluteX = 457; Y=293.0f; BlockLife = 0.5f;
	}
	else {
		X=6; AbsoluteX = 82+X*24; Y=451.0f;
	}
	Red = 1.0f;		Green = 0.0f;		Blue = 1.0f;
	Rotation = 0;
	offsetx[0] = 1; offsetx[1] = 0; offsetx[2] = 0; offsetx[3] = -1;
	offsety[0] = 1; offsety[1] = 1; offsety[2] = 0; offsety[3] = 0;
}

void goodpiece::Restart(bool nextpiece)
{
	if (nextpiece) { 
		AbsoluteX = 457; Y=293.0f; BlockLife = 0.5f;
	}
	else {
		X=6; AbsoluteX = 82+X*24; Y=451.0f;
	}
	Red = 1.0f;		Green = 1.0f;		Blue = 0.0f;
	Rotation = 0;
	offsetx[0] = -1; offsetx[1] = 0; offsetx[2] = 1; offsetx[3] = 0;
	offsety[0] = 0; offsety[1] = 1; offsety[2] = 0; offsety[3] = 0;
}

void longcurveleft::Restart(bool nextpiece)
{
	if (nextpiece) {
		AbsoluteX = 457; Y=293.0f; BlockLife = 0.5f;
	}
	else {
		X=6; AbsoluteX = 82+X*24; Y=451.0f;		
	}
	Red = 0.1f;		Green = 1.00f;		Blue = 0.1f;
	Rotation = 0;
	offsetx[0] = -1; offsetx[1] = -1; offsetx[2] = 0; offsetx[3] = 1;
	offsety[0] = 1; offsety[1] = 0; offsety[2] = 0; offsety[3] = 0;
}

void longcurveright::Restart(bool nextpiece)
{
	if (nextpiece) {
		AbsoluteX = 457; Y=293.0f; BlockLife = 0.5f;
	}
	else {
		X=6; AbsoluteX = 82+X*24; Y=451.0f;
		
	}
	Red = 0.2f;		Green = 0.2f;		Blue = 1.0f;
	Rotation = 0;
	offsetx[0] = 1; offsetx[1] = 1; offsetx[2] = 0; offsetx[3] = -1;
	offsety[0] = 1; offsety[1] = 0; offsety[2] = 0; offsety[3] = 0;
}
