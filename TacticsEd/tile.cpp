#include <windows.h>		
#include <gl\gl.h>
#include <stdlib.h>	
#include <math.h>	
#include "tile.h"

tileclass::tileclass()
{
	Brightness1 = 1.0f;
	Brightness2 = 1.0f;
	Brightness3 = 1.0f;
	Brightness4 = 1.0f;
	XOffset = 0;
	YOffset = 0;
	Height1Selected = false;
	Height2Selected = false;
	Height3Selected = false;
	Height4Selected = false;
	Height1Tally = 1;
	Height2Tally = 1;
	Height3Tally = 1;
	Height4Tally = 1;
	Side1Selected = false;
	Side2Selected = false;
	Side3Selected = false;
	Side4Selected = false;
	Side5Selected = false;
	Side11Texture = 0;
	Side12Texture = 0;
	Side13Texture = 0;
	Side14Texture = 0;
	Side2Texture = 0;
	Side3Texture = 0;
	Side4Texture = 0;
	Side5Texture = 0;
}

void tileclass::Init(int X, int Y)
{
	XOffset = X;
	YOffset = Y;
}

void tileclass::Init(istream &is, int X, int Y)
{
	XOffset = X;
	YOffset = Y;
	is >> Height1Tally >> Height2Tally >> Height3Tally >> Height4Tally
		>> Side11Texture >> Side12Texture >> Side13Texture >> Side14Texture >> 
		Side2Texture >> Side3Texture >> Side4Texture >> Side5Texture;
}

void tileclass::DrawTile(int Xlimit, int Ylimit, tileclass **Grid)
{
		float Height1 = Height1Tally*0.1f;
		float Height2 = Height2Tally*0.1f;
		float Height3 = Height3Tally*0.1f;
		float Height4 = Height4Tally*0.1f;

		if (YOffset != Ylimit-1) {
			int x1 = XOffset;
			int y1 = YOffset+1;
			if (Grid[x1][y1].ReturnHeight(4) > Height1Tally)
				Brightness1 = 1.2f/((Grid[x1][y1].ReturnHeight(4)-Height1Tally));
			else
				Brightness1 = 1.0f;
		}

		if (XOffset != Xlimit-1) {
			int x1 = XOffset+1;
			int y1 = YOffset;

			if (Grid[x1][y1].ReturnHeight(4) > Height3Tally)
				Brightness3 = 1.2f/((Grid[x1][y1].ReturnHeight(4)-Height3Tally));
			else
				Brightness3 = 1.0f;
		}

		if (XOffset != Xlimit && YOffset != Ylimit) {
			int x1 = XOffset+1;
			int y1 = YOffset;
			int x2 = XOffset;
			int y2 = YOffset+1;

			if (XOffset+1 != Xlimit && YOffset+1 != Ylimit) {
			if (Grid[x1][y1].ReturnHeight(1) > Height2Tally && Grid[x2][y2].ReturnHeight(3) > Height2Tally && Grid[x2+1][y2].ReturnHeight(4) > Height2Tally)
				Brightness2 = 1.2f/(((Grid[x1][y1].ReturnHeight(1)-Height2Tally)+(Grid[x2][y2].ReturnHeight(3)-Height2Tally)+(Grid[x2+1][y2].ReturnHeight(4)-Height2Tally))/3.0f);
			else if (Grid[x1][y1].ReturnHeight(1) > Grid[x2][y2].ReturnHeight(3) && Grid[x1][y1].ReturnHeight(1) > Height2Tally && Grid[x2][y2].ReturnHeight(3) != Height2Tally)
				Brightness2 = 1.2f/((Grid[x1][y1].ReturnHeight(1)-Height2Tally));
			else if (Grid[x1][y1].ReturnHeight(1) > Grid[x2][y2].ReturnHeight(3) && Grid[x2][y2].ReturnHeight(3) > Height2Tally) //left
				Brightness2 = 1.2f/((Grid[x2][y2].ReturnHeight(3)-Height2Tally));
			else if (Grid[x2+1][y2].ReturnHeight(4) > Height2Tally) //middle
				Brightness2 = 1.2f/((Grid[x2+1][y2].ReturnHeight(4)-Height2Tally));
			else if (Grid[x1][y1].ReturnHeight(1) == Height2Tally && Grid[x2][y2].ReturnHeight(3) == Height2Tally)
				Brightness2 = 1.0f;
			}
			if (XOffset+1 == Xlimit)
				Brightness2 = Brightness1;
			if (YOffset+1 == Ylimit)
				Brightness2 = Brightness3;
		}


		if (fabs(Height3Tally-Height1Tally) <= fabs(Height4Tally-Height2Tally)) {
			glBegin(GL_TRIANGLES);
			if (Side14Texture == 0)
				glColor3f(1.0f, 1.0f, 1.0f);
			else if (Side14Texture == 1) //snow
				glColor3f(0.8f, 0.8f, 1.0f);
			else if (Side14Texture == 2) //water
				glColor3f(0.0f, 0.0f, 1.0f);
			else if (Side14Texture == 3) //lava
				glColor3f(1.0f, 0.0f, 0.0f);
			else if (Side14Texture == 4) //grass
				glColor3f(0.0f, 0.8f, 0.0f);
			else if (Side14Texture == 5) //dirt
				glColor3f(0.45f, 0.2f, 0.0f);
			else if (Side14Texture == 6) //stone
				glColor3f(0.4f, 0.4f, 0.4f);
			else if (Side14Texture == 7) //sand
				glColor3f(1.0f, 1.0f, 0.25f);
			else if (Side14Texture == 8) //forest
				glColor3f(0.0f, 0.5f, 0.0f);
			glVertex3f((float)XOffset-1, Height4,(float)YOffset-1);
			if (Side11Texture == 0)
				glColor3f(1.0f, 1.0f, 1.0f);
			else if (Side11Texture == 1) //snow
				glColor3f(0.8f, 0.8f, 1.0f);
			else if (Side11Texture == 2) //water
				glColor3f(0.0f, 0.0f, 1.0f);
			else if (Side11Texture == 3) //lava
				glColor3f(1.0f, 0.0f, 0.0f);
			else if (Side11Texture == 4) //grass
				glColor3f(0.0f, 0.8f, 0.0f);
			else if (Side11Texture == 5) //dirt
				glColor3f(0.45f, 0.2f, 0.0f);
			else if (Side11Texture == 6) //stone
				glColor3f(0.4f, 0.4f, 0.4f);
			else if (Side11Texture == 7) //sand
				glColor3f(1.0f, 1.0f, 0.25f);
			else if (Side11Texture == 8) //forest
				glColor3f(0.0f, 0.5f, 0.0f);
			glVertex3f((float)XOffset-1, Height1, (float)YOffset);
			if (Side13Texture == 0)
				glColor3f(1.0f, 1.0f, 1.0f);
			else if (Side13Texture == 1) //snow
				glColor3f(0.8f, 0.8f, 1.0f);
			else if (Side13Texture == 2) //water
				glColor3f(0.0f, 0.0f, 1.0f);
			else if (Side13Texture == 3) //lava
				glColor3f(1.0f, 0.0f, 0.0f);
			else if (Side13Texture == 4) //grass
				glColor3f(0.0f, 0.8f, 0.0f);
			else if (Side13Texture == 5) //dirt
				glColor3f(0.45f, 0.2f, 0.0f);
			else if (Side13Texture == 6) //stone
				glColor3f(0.4f, 0.4f, 0.4f);
			else if (Side13Texture == 7) //sand
				glColor3f(1.0f, 1.0f, 0.25f);
			else if (Side13Texture == 8) //forest
				glColor3f(0.0f, 0.5f, 0.0f);
			glVertex3f((float)XOffset, Height3,(float)YOffset-1); //top
			if (Side11Texture == 0)
				glColor3f(1.0f, 1.0f, 1.0f);
			else if (Side11Texture == 1) //snow
				glColor3f(0.8f, 0.8f, 1.0f);
			else if (Side11Texture == 2) //water
				glColor3f(0.0f, 0.0f, 1.0f);
			else if (Side11Texture == 3) //lava
				glColor3f(1.0f, 0.0f, 0.0f);
			else if (Side11Texture == 4) //grass
				glColor3f(0.0f, 0.8f, 0.0f);
			else if (Side11Texture == 5) //dirt
				glColor3f(0.45f, 0.2f, 0.0f);
			else if (Side11Texture == 6) //stone
				glColor3f(0.4f, 0.4f, 0.4f);
			else if (Side11Texture == 7) //sand
				glColor3f(1.0f, 1.0f, 0.25f);
			else if (Side11Texture == 8) //forest
				glColor3f(0.0f, 0.5f, 0.0f);
			glVertex3f((float)XOffset-1, Height1, (float)YOffset);
			if (Side12Texture == 0)
				glColor3f(1.0f, 1.0f, 1.0f);
			else if (Side12Texture == 1) //snow
				glColor3f(0.8f, 0.8f, 1.0f);
			else if (Side12Texture == 2) //water
				glColor3f(0.0f, 0.0f, 1.0f);
			else if (Side12Texture == 3) //lava
				glColor3f(1.0f, 0.0f, 0.0f);
			else if (Side12Texture == 4) //grass
				glColor3f(0.0f, 0.8f, 0.0f);
			else if (Side12Texture == 5) //dirt
				glColor3f(0.45f, 0.2f, 0.0f);
			else if (Side12Texture == 6) //stone
				glColor3f(0.4f, 0.4f, 0.4f);
			else if (Side12Texture == 7) //sand
				glColor3f(1.0f, 1.0f, 0.25f);
			else if (Side12Texture == 8) //forest
				glColor3f(0.0f, 0.5f, 0.0f);
			glVertex3f((float)XOffset, Height2, (float)YOffset);	
			if (Side13Texture == 0)
				glColor3f(1.0f, 1.0f, 1.0f);
			else if (Side13Texture == 1) //snow
				glColor3f(0.8f, 0.8f, 1.0f);
			else if (Side13Texture == 2) //water
				glColor3f(0.0f, 0.0f, 1.0f);
			else if (Side13Texture == 3) //lava
				glColor3f(1.0f, 0.0f, 0.0f);
			else if (Side13Texture == 4) //grass
				glColor3f(0.0f, 0.8f, 0.0f);
			else if (Side13Texture == 5) //dirt
				glColor3f(0.45f, 0.2f, 0.0f);
			else if (Side13Texture == 6) //stone
				glColor3f(0.4f, 0.4f, 0.4f);
			else if (Side13Texture == 7) //sand
				glColor3f(1.0f, 1.0f, 0.25f);
			else if (Side13Texture == 8) //forest
				glColor3f(0.0f, 0.5f, 0.0f);
			glVertex3f((float)XOffset, Height3,(float)YOffset-1); //top
			glEnd();
		} else {
			glBegin(GL_TRIANGLES);
			if (Side13Texture == 0)
				glColor3f(1.0f, 1.0f, 1.0f);
			else if (Side13Texture == 1) //snow
				glColor3f(0.8f, 0.8f, 1.0f);
			else if (Side13Texture == 2) //water
				glColor3f(0.0f, 0.0f, 1.0f);
			else if (Side13Texture == 3) //lava
				glColor3f(1.0f, 0.0f, 0.0f);
			else if (Side13Texture == 4) //grass
				glColor3f(0.0f, 0.8f, 0.0f);
			else if (Side13Texture == 5) //dirt
				glColor3f(0.45f, 0.2f, 0.0f);
			else if (Side13Texture == 6) //stone
				glColor3f(0.4f, 0.4f, 0.4f);
			else if (Side13Texture == 7) //sand
				glColor3f(1.0f, 1.0f, 0.25f);
			else if (Side13Texture == 8) //forest
				glColor3f(0.0f, 0.5f, 0.0f);
			glVertex3f((float)XOffset, Height3,(float)YOffset-1); //top
			if (Side14Texture == 0)
				glColor3f(1.0f, 1.0f, 1.0f);
			else if (Side14Texture == 1) //snow
				glColor3f(0.8f, 0.8f, 1.0f);
			else if (Side14Texture == 2) //water
				glColor3f(0.0f, 0.0f, 1.0f);
			else if (Side14Texture == 3) //lava
				glColor3f(1.0f, 0.0f, 0.0f);
			else if (Side14Texture == 4) //grass
				glColor3f(0.0f, 0.8f, 0.0f);
			else if (Side14Texture == 5) //dirt
				glColor3f(0.45f, 0.2f, 0.0f);
			else if (Side14Texture == 6) //stone
				glColor3f(0.4f, 0.4f, 0.4f);
			else if (Side14Texture == 7) //sand
				glColor3f(1.0f, 1.0f, 0.25f);
			else if (Side14Texture == 8) //forest
				glColor3f(0.0f, 0.5f, 0.0f);
			glVertex3f((float)XOffset-1, Height4,(float)YOffset-1);
			if (Side12Texture == 0)
				glColor3f(1.0f, 1.0f, 1.0f);
			else if (Side12Texture == 1) //snow
				glColor3f(0.8f, 0.8f, 1.0f);
			else if (Side12Texture == 2) //water
				glColor3f(0.0f, 0.0f, 1.0f);
			else if (Side12Texture == 3) //lava
				glColor3f(1.0f, 0.0f, 0.0f);
			else if (Side12Texture == 4) //grass
				glColor3f(0.0f, 0.8f, 0.0f);
			else if (Side12Texture == 5) //dirt
				glColor3f(0.45f, 0.2f, 0.0f);
			else if (Side12Texture == 6) //stone
				glColor3f(0.4f, 0.4f, 0.4f);
			else if (Side12Texture == 7) //sand
				glColor3f(1.0f, 1.0f, 0.25f);
			else if (Side12Texture == 8) //forest
				glColor3f(0.0f, 0.5f, 0.0f);
			glVertex3f((float)XOffset, Height2, (float)YOffset);
			if (Side11Texture == 0)
				glColor3f(1.0f, 1.0f, 1.0f);
			else if (Side11Texture == 1) //snow
				glColor3f(0.8f, 0.8f, 1.0f);
			else if (Side11Texture == 2) //water
				glColor3f(0.0f, 0.0f, 1.0f);
			else if (Side11Texture == 3) //lava
				glColor3f(1.0f, 0.0f, 0.0f);
			else if (Side11Texture == 4) //grass
				glColor3f(0.0f, 0.8f, 0.0f);
			else if (Side11Texture == 5) //dirt
				glColor3f(0.45f, 0.2f, 0.0f);
			else if (Side11Texture == 6) //stone
				glColor3f(0.4f, 0.4f, 0.4f);
			else if (Side11Texture == 7) //sand
				glColor3f(1.0f, 1.0f, 0.25f);
			else if (Side11Texture == 8) //forest
				glColor3f(0.0f, 0.5f, 0.0f);
			glVertex3f((float)XOffset-1, Height1, (float)YOffset);
			if (Side12Texture == 0)
				glColor3f(1.0f, 1.0f, 1.0f);
			else if (Side12Texture == 1) //snow
				glColor3f(0.8f, 0.8f, 1.0f);
			else if (Side12Texture == 2) //water
				glColor3f(0.0f, 0.0f, 1.0f);
			else if (Side12Texture == 3) //lava
				glColor3f(1.0f, 0.0f, 0.0f);
			else if (Side12Texture == 4) //grass
				glColor3f(0.0f, 0.8f, 0.0f);
			else if (Side12Texture == 5) //dirt
				glColor3f(0.45f, 0.2f, 0.0f);
			else if (Side12Texture == 6) //stone
				glColor3f(0.4f, 0.4f, 0.4f);
			else if (Side12Texture == 7) //sand
				glColor3f(1.0f, 1.0f, 0.25f);
			else if (Side12Texture == 8) //forest
				glColor3f(0.0f, 0.5f, 0.0f);
			glVertex3f((float)XOffset, Height2, (float)YOffset);
			if (Side14Texture == 0)
				glColor3f(1.0f, 1.0f, 1.0f);
			else if (Side14Texture == 1) //snow
				glColor3f(0.8f, 0.8f, 1.0f);
			else if (Side14Texture == 2) //water
				glColor3f(0.0f, 0.0f, 1.0f);
			else if (Side14Texture == 3) //lava
				glColor3f(1.0f, 0.0f, 0.0f);
			else if (Side14Texture == 4) //grass
				glColor3f(0.0f, 0.8f, 0.0f);
			else if (Side14Texture == 5) //dirt
				glColor3f(0.45f, 0.2f, 0.0f);
			else if (Side14Texture == 6) //stone
				glColor3f(0.4f, 0.4f, 0.4f);
			else if (Side14Texture == 7) //sand
				glColor3f(1.0f, 1.0f, 0.25f);
			else if (Side14Texture == 8) //forest
				glColor3f(0.0f, 0.5f, 0.0f);
			glVertex3f((float)XOffset-1, Height4,(float)YOffset-1); //top
			glEnd();
		}
		
		if ((YOffset+1 < Ylimit && (Grid[XOffset][YOffset+1].ReturnHeight(4) < Height1Tally || Grid[XOffset][YOffset+1].ReturnHeight(3) < Height2Tally)) || YOffset == Ylimit-1) {
		if (Side2Texture == 0)
			glColor3f(1.0f, 1.0f, 1.0f);
		else if (Side2Texture == 1) //snow
			glColor3f(0.8f, 0.8f, 1.0f);
		else if (Side2Texture == 2) //water
			glColor3f(0.0f, 0.0f, 1.0f);
		else if (Side2Texture == 3) //lava
			glColor3f(1.0f, 0.0f, 0.0f);
		else if (Side2Texture == 4) //grass
			glColor3f(0.0f, 0.8f, 0.0f);
		else if (Side2Texture == 5) //dirt
			glColor3f(0.45f, 0.2f, 0.0f);
		else if (Side2Texture == 6) //stone
			glColor3f(0.4f, 0.4f, 0.4f);
		else if (Side2Texture == 7) //sand
			glColor3f(1.0f, 1.0f, 0.25f);
		else if (Side2Texture == 8) //forest
			glColor3f(0.0f, 0.5f, 0.0f);
		glBegin(GL_TRIANGLE_STRIP);
		glVertex3f((float)XOffset, Height2, (float)YOffset); //upper right
		glVertex3f((float)XOffset-1, Height1, (float)YOffset);
		glVertex3f((float)XOffset,0.0f, (float)YOffset);
		glVertex3f((float)XOffset-1.0f, 0.0f, (float)YOffset);
		glEnd();
		}

		if ((YOffset > 0 && (Grid[XOffset][YOffset-1].ReturnHeight(2) < Height3Tally || Grid[XOffset][YOffset-1].ReturnHeight(1) < Height4Tally)) || YOffset == 0) {
		if (Side3Texture == 0)
			glColor3f(1.0f, 1.0f, 1.0f);
		else if (Side3Texture == 1) //snow
			glColor3f(0.8f, 0.8f, 1.0f);
		else if (Side3Texture == 2) //water
			glColor3f(0.0f, 0.0f, 1.0f);
		else if (Side3Texture == 3) //lava
			glColor3f(1.0f, 0.0f, 0.0f);
		else if (Side3Texture == 4) //grass
			glColor3f(0.0f, 0.8f, 0.0f);
		else if (Side3Texture == 5) //dirt
			glColor3f(0.45f, 0.2f, 0.0f);
		else if (Side3Texture == 6) //stone
			glColor3f(0.4f, 0.4f, 0.4f);
		else if (Side3Texture == 7) //sand
			glColor3f(1.0f, 1.0f, 0.25f);
		else if (Side3Texture == 8) //forest
			glColor3f(0.0f, 0.5f, 0.0f);
		glBegin(GL_TRIANGLE_STRIP);
		glVertex3f((float)XOffset,0.0f,(float)YOffset-1); //bottom left
		glVertex3f((float)XOffset-1,0.0f,(float)YOffset-1);
		glVertex3f((float)XOffset, Height3,(float)YOffset-1);
		glVertex3f((float)XOffset-1, Height4,(float)YOffset-1);
		glEnd();
		}

		if ((XOffset > 0 && (Grid[XOffset-1][YOffset].ReturnHeight(2) < Height1Tally || Grid[XOffset-1][YOffset].ReturnHeight(3) < Height4Tally)) || XOffset == 0) {
		if (Side4Texture == 0)
			glColor3f(1.0f, 1.0f, 1.0f);
		else if (Side4Texture == 1) //snow
			glColor3f(0.8f, 0.8f, 1.0f);
		else if (Side4Texture == 2) //water
			glColor3f(0.0f, 0.0f, 1.0f);
		else if (Side4Texture == 3) //lava
			glColor3f(1.0f, 0.0f, 0.0f);
		else if (Side4Texture == 4) //grass
			glColor3f(0.0f, 0.8f, 0.0f);
		else if (Side4Texture == 5) //dirt
			glColor3f(0.45f, 0.2f, 0.0f);
		else if (Side4Texture == 6) //stone
			glColor3f(0.4f, 0.4f, 0.4f);
		else if (Side4Texture == 7) //sand
			glColor3f(1.0f, 1.0f, 0.25f);
		else if (Side4Texture == 8) //forest
			glColor3f(0.0f, 0.5f, 0.0f);
		glBegin(GL_TRIANGLE_STRIP);
		glVertex3f((float)XOffset-1, Height1, (float)YOffset); //bottom right
		glVertex3f((float)XOffset-1, Height4,(float)YOffset-1);
		glVertex3f((float)XOffset-1,0.0f, (float)YOffset);
		glVertex3f((float)XOffset-1,0.0f,(float)YOffset-1);
		glEnd();
		}

		if ((XOffset+1 < Xlimit && (Grid[XOffset+1][YOffset].ReturnHeight(4) < Height3Tally || Grid[XOffset+1][YOffset].ReturnHeight(1) < Height2Tally)) || XOffset == Xlimit-1) {
		if (Side5Texture == 0)
			glColor3f(1.0f, 1.0f, 1.0f);
		else if (Side5Texture == 1) //snow
			glColor3f(0.8f, 0.8f, 1.0f);
		else if (Side5Texture == 2) //water
			glColor3f(0.0f, 0.0f, 1.0f);
		else if (Side5Texture == 3) //lava
			glColor3f(1.0f, 0.0f, 0.0f);
		else if (Side5Texture == 4) //grass
			glColor3f(0.0f, 0.8f, 0.0f);
		else if (Side5Texture == 5) //dirt
			glColor3f(0.45f, 0.2f, 0.0f);
		else if (Side5Texture == 6) //stone
			glColor3f(0.4f, 0.4f, 0.4f);
		else if (Side5Texture == 7) //sand
			glColor3f(1.0f, 1.0f, 0.25f);
		else if (Side5Texture == 8) //forest
			glColor3f(0.0f, 0.5f, 0.0f);
		glBegin(GL_TRIANGLE_STRIP);
		glVertex3f((float)XOffset, Height3,(float)YOffset-1); //upper left
		glVertex3f((float)XOffset, Height2, (float)YOffset);
		glVertex3f((float)XOffset,0.0f,(float)YOffset-1);
		glVertex3f((float)XOffset,0.0f, (float)YOffset);
		glEnd();
		}
}

void tileclass::DrawTile(int s)
{
	float Height1 = Height1Tally*0.1f;
	float Height2 = Height2Tally*0.1f;
	float Height3 = Height3Tally*0.1f;
	float Height4 = Height4Tally*0.1f;

	if (s == 1) {
		glBegin(GL_TRIANGLE_STRIP);
		if (fabs(Height3Tally-Height1Tally) <= fabs(Height4Tally-Height2Tally)) {
			glVertex3f((float)XOffset-1, Height4,(float)YOffset-1);
			glVertex3f((float)XOffset-1, Height1, (float)YOffset);
			glVertex3f((float)XOffset, Height3,(float)YOffset-1);
			glVertex3f((float)XOffset, Height2, (float)YOffset);	
		}
		else {
			glVertex3f((float)XOffset, Height3,(float)YOffset-1);
			glVertex3f((float)XOffset-1, Height4,(float)YOffset-1);
			glVertex3f((float)XOffset, Height2, (float)YOffset);
			glVertex3f((float)XOffset-1, Height1, (float)YOffset);
		}
		glEnd();
	}

	if (s == 2) {
		glBegin(GL_TRIANGLE_STRIP);
		glVertex3f((float)XOffset, Height2, (float)YOffset); //upper right
		glVertex3f((float)XOffset-1, Height1, (float)YOffset);
		glVertex3f((float)XOffset,0.0f, (float)YOffset);
		glVertex3f((float)XOffset-1.0f, 0.0f, (float)YOffset);
		glEnd();
	}

	if (s == 3) {
		glBegin(GL_TRIANGLE_STRIP);
		glVertex3f((float)XOffset-1, Height4,(float)YOffset-1);
		glVertex3f((float)XOffset, Height3,(float)YOffset-1);
		glVertex3f((float)XOffset-1,0.0f,(float)YOffset-1);
		glVertex3f((float)XOffset,0.0f,(float)YOffset-1); //bottom left
		glEnd();
	}

	if (s == 4) {
		glBegin(GL_TRIANGLE_STRIP);
		glVertex3f((float)XOffset-1, Height1, (float)YOffset); //bottom right
		glVertex3f((float)XOffset-1, Height4,(float)YOffset-1);
		glVertex3f((float)XOffset-1,0.0f, (float)YOffset);
		glVertex3f((float)XOffset-1,0.0f,(float)YOffset-1);
		glEnd();
	}

	if (s == 5) {
		glBegin(GL_TRIANGLE_STRIP);
		glVertex3f((float)XOffset, Height3,(float)YOffset-1); //upper left
		glVertex3f((float)XOffset, Height2, (float)YOffset);
		glVertex3f((float)XOffset,0.0f,(float)YOffset-1);
		glVertex3f((float)XOffset,0.0f, (float)YOffset);
		glEnd();
	}
	if (s == 6) {
		glBegin(GL_TRIANGLE_STRIP);
		if (fabs(Height3Tally-Height1Tally) <= fabs(Height4Tally-Height2Tally)) {
			glVertex3f((float)XOffset-1.0f, Height4,(float)YOffset-1.0f);
			glVertex3f((float)XOffset-1.0f, Height1, (float)YOffset);
			glVertex3f((float)XOffset, Height3,(float)YOffset-1.0f); //top
			glVertex3f((float)XOffset, Height2, (float)YOffset);	
		}
		else {
			glVertex3f((float)XOffset, Height3,(float)YOffset-1.0f); //top
			glVertex3f((float)XOffset-1.0f, Height4,(float)YOffset-1.0f);
			glVertex3f((float)XOffset, Height2, (float)YOffset);
			glVertex3f((float)XOffset-1.0f, Height1, (float)YOffset);
		}
		glEnd();
		glBegin(GL_TRIANGLE_STRIP);
			glVertex3f((float)XOffset, Height2, (float)YOffset); //upper right
			glVertex3f((float)XOffset-1.0f, Height1, (float)YOffset);
			glVertex3f((float)XOffset,0.0f, (float)YOffset);
			glVertex3f((float)XOffset-1.0f, 0.0f, (float)YOffset);
		glEnd();
		glBegin(GL_TRIANGLE_STRIP);
			glVertex3f((float)XOffset-1.0f, Height4,(float)YOffset-1.0f);
			glVertex3f((float)XOffset, Height3,(float)YOffset-1.0f);
			glVertex3f((float)XOffset-1.0f,0.0f,(float)YOffset-1.0f);
			glVertex3f((float)XOffset,0.0f,(float)YOffset-1.0f); //bottom left
		glEnd();
		glBegin(GL_TRIANGLE_STRIP);
			glVertex3f((float)XOffset-1.0f, Height1, (float)YOffset); //bottom right
			glVertex3f((float)XOffset-1.0f, Height4,(float)YOffset-1.0f);
			glVertex3f((float)XOffset-1.0f,0.0f, (float)YOffset);
			glVertex3f((float)XOffset-1.0f,0.0f,(float)YOffset-1.0f);
		glEnd();
		glBegin(GL_TRIANGLE_STRIP);
			glVertex3f((float)XOffset, Height3,(float)YOffset-1.0f); //upper left
			glVertex3f((float)XOffset, Height2, (float)YOffset);
			glVertex3f((float)XOffset,0.0f,(float)YOffset-1.0f);
			glVertex3f((float)XOffset,0.0f, (float)YOffset);
		glEnd();
	}

}

int tileclass::ReturnHeight(int Number) 
{
	if (Number == 1)
		return Height1Tally;
	else if (Number == 2)
		return Height2Tally;
	else if (Number == 3)
		return Height3Tally;
	else
		return Height4Tally;
}

void tileclass::DrawOutline(bool Selected, int Xlimit, int Ylimit, tileclass **Grid)
{
	float Height1 = Height1Tally*0.1f;
	float Height2 = Height2Tally*0.1f;
	float Height3 = Height3Tally*0.1f;
	float Height4 = Height4Tally*0.1f;

	if (Selected && (Side1Selected || Side2Selected || Side3Selected || Side4Selected || Side5Selected)) {
	glPushAttrib(GL_LINE_BIT);
	glLineWidth(1.5);
	glDepthMask(0);
	glColor3f(1.0f, 0.0f, 0.0f);
	if (Side1Selected) {
	glBegin(GL_LINE_LOOP);
		glVertex3f((float)XOffset, Height3,(float)YOffset-1);// Top Right Of The Quad (Top)
		glVertex3f((float)XOffset-1, Height4,(float)YOffset-1);// Top Left Of The Quad (Top)
		glVertex3f((float)XOffset-1, Height1, (float)YOffset);// Bottom Left Of The Quad (Top)
		glVertex3f((float)XOffset, Height2, (float)YOffset);// Bottom Right Of The Quad (Top)
	glEnd();
	}
	if (Side2Selected) {
		float NextHeight1 = 0, NextHeight2 = 0;
		if (YOffset < Ylimit-1) {
			if (Grid[XOffset][YOffset+1].ReturnHeight(4)*0.1f < Height1)
				NextHeight1 = Grid[XOffset][YOffset+1].ReturnHeight(4)*0.1f;
			if (Grid[XOffset][YOffset+1].ReturnHeight(3)*0.1f < Height2)
				NextHeight2 = Grid[XOffset][YOffset+1].ReturnHeight(3)*0.1f;
		}
		glBegin(GL_LINE_LOOP);
		if (NextHeight1 == Height1 || Height1 == 0) {
			glVertex3f((float)XOffset-1.0f, Height1, (float)YOffset);
			glVertex3f((float)XOffset, Height2, (float)YOffset); //upper right
			glVertex3f((float)XOffset, NextHeight2, (float)YOffset);
			glVertex3f((float)XOffset-1.0f,  NextHeight1, (float)YOffset);
		} else {
			glVertex3f((float)XOffset, Height2, (float)YOffset); //upper right
			glVertex3f((float)XOffset-1.0f, Height1, (float)YOffset);
			glVertex3f((float)XOffset-1.0f,  NextHeight1, (float)YOffset);
			glVertex3f((float)XOffset, NextHeight2, (float)YOffset);
		}
		glEnd();
	}
	if (Side3Selected) {
		float NextHeight3 = 0, NextHeight4 = 0;
		if (YOffset > 0) {
			if (Grid[XOffset][YOffset-1].ReturnHeight(1)*0.1f < Height4)
				NextHeight4 = Grid[XOffset][YOffset-1].ReturnHeight(1)*0.1f;
			if (Grid[XOffset][YOffset-1].ReturnHeight(2)*0.1f < Height3)
				NextHeight3 = Grid[XOffset][YOffset-1].ReturnHeight(2)*0.1f;
		}
		glBegin(GL_LINE_LOOP);
		if (NextHeight3 == Height3 || Height3 == 0) {
			glVertex3f((float)XOffset, Height3,(float)YOffset-1.0f);
			glVertex3f((float)XOffset-1.0f, Height4,(float)YOffset-1.0f);
			glVertex3f((float)XOffset-1.0f, NextHeight4,(float)YOffset-1.0f);
			glVertex3f((float)XOffset, NextHeight3,(float)YOffset-1.0f); //bottom left
		} else {
			glVertex3f((float)XOffset-1.0f, Height4,(float)YOffset-1.0f);
			glVertex3f((float)XOffset, Height3,(float)YOffset-1.0f);
			glVertex3f((float)XOffset, NextHeight3,(float)YOffset-1.0f); //bottom left
			glVertex3f((float)XOffset-1.0f, NextHeight4,(float)YOffset-1.0f);
		}
		glEnd();
	}
	if (Side4Selected) {
		float NextHeight1 = 0, NextHeight4 = 0;
		if (XOffset > 0) {
			if (Grid[XOffset-1][YOffset].ReturnHeight(3)*0.1f < Height4)
				NextHeight4 = Grid[XOffset-1][YOffset].ReturnHeight(3)*0.1f;
			if (Grid[XOffset-1][YOffset].ReturnHeight(2)*0.1f < Height1)
				NextHeight1 = Grid[XOffset-1][YOffset].ReturnHeight(2)*0.1f;
		}
		glBegin(GL_LINE_LOOP);
		if (NextHeight4 == Height4 || Height4 == 0) {
			glVertex3f((float)XOffset-1.0f, Height4,(float)YOffset-1.0f);
			glVertex3f((float)XOffset-1.0f, Height1, (float)YOffset); //bottom right
			glVertex3f((float)XOffset-1.0f, NextHeight1, (float)YOffset);
			glVertex3f((float)XOffset-1.0f, NextHeight4,(float)YOffset-1.0f);
		} else {
			glVertex3f((float)XOffset-1.0f, Height1, (float)YOffset); //bottom right
			glVertex3f((float)XOffset-1.0f, Height4,(float)YOffset-1.0f);
			glVertex3f((float)XOffset-1.0f, NextHeight4,(float)YOffset-1.0f);
			glVertex3f((float)XOffset-1.0f, NextHeight1, (float)YOffset);
		}
		glEnd();
	}
	if (Side5Selected) {
		float NextHeight3 = 0, NextHeight2 = 0;
		if (XOffset < Xlimit-1) {
			if (Grid[XOffset+1][YOffset].ReturnHeight(4)*0.1f < Height3)
				NextHeight3 = Grid[XOffset+1][YOffset].ReturnHeight(4)*0.1f;
			if (Grid[XOffset+1][YOffset].ReturnHeight(1)*0.1f < Height2)
				NextHeight2 = Grid[XOffset+1][YOffset].ReturnHeight(1)*0.1f;
		}
		glBegin(GL_LINE_LOOP);
		if (NextHeight3 == Height3 || Height3 == 0) {
			glVertex3f((float)XOffset, Height3,(float)YOffset-1.0f); //upper left
			glVertex3f((float)XOffset, Height2, (float)YOffset);
			glVertex3f((float)XOffset, NextHeight2, (float)YOffset);
			glVertex3f((float)XOffset, NextHeight3,(float)YOffset-1.0f);
		} else {
			glVertex3f((float)XOffset, Height2, (float)YOffset);
			glVertex3f((float)XOffset, Height3,(float)YOffset-1.0f); //upper left
			glVertex3f((float)XOffset, NextHeight3,(float)YOffset-1.0f);
			glVertex3f((float)XOffset, NextHeight2, (float)YOffset);
		}
		glEnd();
	}
	glDepthMask(1);
	glPopAttrib();
	}
	else if (!Selected) {
	glColor3f(0.0f, 0.0f, 0.0f);
	glDepthMask(0);
	glBegin(GL_LINE_LOOP);
		glVertex3f((float)XOffset, Height3,(float)YOffset-1);// Top Right Of The Quad (Top)
		glVertex3f((float)XOffset-1, Height4,(float)YOffset-1);// Top Left Of The Quad (Top)
		glVertex3f((float)XOffset-1, Height1, (float)YOffset);// Bottom Left Of The Quad (Top)
		glVertex3f((float)XOffset, Height2, (float)YOffset);// Bottom Right Of The Quad (Top)
	glEnd();
	glBegin(GL_LINES);
		glVertex3f((float)XOffset, Height3,(float)YOffset-1);// Top Right Of The Quad (Top)
		glVertex3f((float)XOffset, 0.0f,(float)YOffset-1);
		glVertex3f((float)XOffset-1, Height4,(float)YOffset-1);// Top Left Of The Quad (Top)
		glVertex3f((float)XOffset-1, 0.0f,(float)YOffset-1);
		glVertex3f((float)XOffset-1, Height1, (float)YOffset);// Bottom Left Of The Quad (Top)
		glVertex3f((float)XOffset-1, 0.0f,(float)YOffset);
		glVertex3f((float)XOffset, Height2, (float)YOffset);// Bottom Right Of The Quad (Top)
		glVertex3f((float)XOffset, 0.0f,(float)YOffset);
		if (fabs(Height3Tally-Height1Tally) != fabs(Height4Tally-Height2Tally)) {
			if (fabs(Height3Tally-Height1Tally) < fabs(Height4Tally-Height2Tally)) {
				glVertex3f((float)XOffset-1, Height1, (float)YOffset);
				glVertex3f((float)XOffset, Height3,(float)YOffset-1);
			}
			else {
				glVertex3f((float)XOffset-1, Height4,(float)YOffset-1);
				glVertex3f((float)XOffset, Height2, (float)YOffset);
			}
		}
	glEnd();
	glDepthMask(1);
	}
}

void tileclass::EditTile(bool &up, bool &down, bool &left, bool &right) //change elevation/element
{
	int direction;
	if (up)
		direction = 1;
	if (down)
		direction = -1;
	if (Side1Selected && (up || down)) {
		if (Height1Selected)
			Height1Tally+=direction;
		if (Height2Selected)
			Height2Tally+=direction;
		if (Height3Selected)
			Height3Tally+=direction;
		if (Height4Selected)
			Height4Tally+=direction;
		if (!Height1Selected && !Height2Selected && !Height3Selected && !Height4Selected) {
			Height1Tally+=direction;
			Height2Tally+=direction;
			Height3Tally+=direction;
			Height4Tally+=direction;
		}
	}
	if (right && (Height1Selected || Height2Selected || Height3Selected || Height4Selected)) {
		if (Height1Selected && Side11Texture < 8)
			Side11Texture++;
		else if (Height1Selected && Side11Texture == 8)
			Side11Texture = 0;

		if (Height2Selected && Side12Texture < 8)
			Side12Texture++;
		else if (Height2Selected && Side12Texture == 8)
			Side12Texture = 0;

		if (Height3Selected && Side13Texture < 8)
			Side13Texture++;
		else if (Height3Selected && Side13Texture == 8)
			Side13Texture = 0;

		if (Height4Selected && Side14Texture < 8)
			Side14Texture++;
		else if (Height4Selected && Side14Texture == 8)
			Side14Texture = 0;
	}
	else if (right && Side1Selected && (!Height1Selected && !Height2Selected && !Height3Selected && !Height4Selected)) {
		if (Side11Texture < 8)
			Side11Texture++;
		else
			Side11Texture = 0;

		if (Side12Texture < 8)
			Side12Texture++;
		else
			Side12Texture = 0;

		if (Side13Texture < 8)
			Side13Texture++;
		else
			Side13Texture = 0;

		if (Side14Texture < 8)
			Side14Texture++;
		else
			Side14Texture = 0;
	}

	if (left && Side1Selected && (Height1Selected || Height2Selected || Height3Selected || Height4Selected)) {
		if (Height1Selected && Side11Texture != 0)
			Side11Texture--;
		else if (Height1Selected)
			Side11Texture = 8;

		if (Height2Selected && Side12Texture != 0)
			Side12Texture--;
		else if (Height2Selected)
			Side12Texture = 8;

		if (Height3Selected && Side13Texture != 0)
			Side13Texture--;
		else if (Height3Selected)
			Side13Texture = 8;

		if (Height4Selected && Side14Texture != 0)
			Side14Texture--;
		else if (Height4Selected)
			Side14Texture = 8;
	}
	else if (left && Side1Selected && (!Height1Selected && !Height2Selected && !Height3Selected && !Height4Selected)) {
		if (Side11Texture != 0)
			Side11Texture--;
		else if (Side11Texture == 0)
			Side11Texture = 8;

		if (Side12Texture != 0)
			Side12Texture--;
		else if (Side12Texture == 0)
			Side12Texture = 8;

		if (Side13Texture != 0)
			Side13Texture--;
		else if (Side13Texture == 0)
			Side13Texture = 8;

		if (Side14Texture != 0)
			Side14Texture--;
		else if (Side14Texture == 0)
			Side14Texture = 8;
	}

	if (right && (Side2Selected || Side3Selected || Side4Selected || Side5Selected) && (!Height1Selected && !Height2Selected && !Height3Selected && !Height4Selected)) {
		if (Side2Selected && Side2Texture < 8)
			Side2Texture++;
		else if (Side2Selected)
			Side2Texture = 0;

		if (Side3Selected && Side3Texture < 8)
			Side3Texture++;
		else if (Side3Selected)
			Side3Texture = 0;

		if (Side4Selected && Side4Texture < 8)
			Side4Texture++;
		else if (Side4Selected)
			Side4Texture = 0;

		if (Side5Selected && Side5Texture < 8)
			Side5Texture++;
		else if (Side5Selected)
			Side5Texture = 0;
	}
	if (left && (Side2Selected || Side3Selected || Side4Selected || Side5Selected) && (!Height1Selected && !Height2Selected && !Height3Selected && !Height4Selected)) {
		if (Side2Selected)
			Side2Texture--;
		if (Side2Texture < 0)
			Side2Texture = 8;
		if (Side3Selected)
			Side3Texture--;
		if (Side3Texture < 0)
			Side3Texture = 8;
		if (Side4Selected)
			Side4Texture--;
		if (Side4Texture < 0)
			Side4Texture = 8;
		if (Side5Selected)
			Side5Texture--;
		if (Side5Texture < 0)
			Side5Texture = 8;
	}

}

void tileclass::WriteTile(ostream &os, int number, int Xlimit, int Ylimit, tileclass **Grid)
{
	if (number == 1) {
	os << endl << Side11Texture << " " << Side12Texture << " " << Side13Texture << " " <<
	Side14Texture << " " << Height1Tally << " " << Height2Tally << " "
	<< Height3Tally << " " << Height4Tally << " "
	<< Side2Texture << " " << Side3Texture
	<< " " << Side4Texture << " " << Side5Texture << " " <<
	Brightness1 << " " << Brightness2 << " " << Brightness3 <<
	" " << Brightness4 << " " <<
	((YOffset+1 < Ylimit && (Grid[XOffset][YOffset+1].ReturnHeight(4) < Height1Tally || Grid[XOffset][YOffset+1].ReturnHeight(3) < Height2Tally)) || YOffset == Ylimit-1) << " " <<
	((YOffset > 0 && (Grid[XOffset][YOffset-1].ReturnHeight(2) < Height3Tally || Grid[XOffset][YOffset-1].ReturnHeight(1) < Height4Tally)) || YOffset == 0) << " " <<
	((XOffset > 0 && (Grid[XOffset-1][YOffset].ReturnHeight(2) < Height1Tally || Grid[XOffset-1][YOffset].ReturnHeight(3) < Height4Tally)) || XOffset == 0) << " " <<
	((XOffset+1 < Xlimit && (Grid[XOffset+1][YOffset].ReturnHeight(4) < Height3Tally || Grid[XOffset+1][YOffset].ReturnHeight(1) < Height2Tally)) || XOffset == Xlimit-1);
	}
	else {
	os 	<< endl << Height1Tally << " " << Height2Tally << " "
	<< Height3Tally << " " << Height4Tally << " " << 
	Side11Texture << " " << Side12Texture << " " << Side13Texture << " " <<
	Side14Texture << " " << Side2Texture << " " << Side3Texture
	<< " " << Side4Texture << " " << Side5Texture;
	}
}

void tileclass::DrawSelectionPoints(int s)
{
	float Height1 = Height1Tally*0.1f;
	float Height2 = Height2Tally*0.1f;
	float Height3 = Height3Tally*0.1f;
	float Height4 = Height4Tally*0.1f;

	if (Side1Selected) {
		glColor3f(0.0f, 0.0f, 0.0f);
		glPushAttrib(GL_POINT_BIT);
		if (s == 3) {
		if (Height3Selected)
			glPointSize(10.0f);
		else
			glPointSize(6.0f);
		glBegin(GL_POINTS);
		glVertex3f(XOffset-0.1f, Height3,YOffset-0.9f);
		glEnd();
		}

		if (s == 4) {
		if (Height4Selected)
			glPointSize(10.0f);
		else
			glPointSize(6.0f);
		glBegin(GL_POINTS);
		glVertex3f(XOffset-0.9f, Height4,YOffset-0.9f);
		glEnd();
		}

		if (s == 2) {
		if (Height2Selected)
			glPointSize(10.0f);
		else
			glPointSize(6.0f);
		glBegin(GL_POINTS);
		glVertex3f(XOffset-0.1f, Height2, YOffset-0.1f);
		glEnd();
		}

		if (s == 1) {
		if (Height1Selected)
			glPointSize(10.0f);
		else
			glPointSize(6.0f);
		glBegin(GL_POINTS);
		glVertex3f(XOffset-0.9f, Height1, YOffset-0.1f);
		glEnd();
		}
		glPopAttrib();
	}
}

void tileclass::Unselect()
{ 
	Side1Selected = false;
	Side2Selected = false;
	Side3Selected = false;
	Side4Selected = false;
	Side5Selected = false;
	Height1Selected = false;
	Height2Selected = false;
	Height3Selected = false;
	Height4Selected = false;
}

bool tileclass::Select(bool All, bool Shift)
{ 
	if (!Side1Selected && !Side2Selected && !Side3Selected && !Side4Selected && !Side5Selected) {
		Side1Selected = true;
		Side2Selected = true;
		Side3Selected = true;
		Side4Selected = true;
		Side5Selected = true;
		return true;
	}
	else if (Side1Selected && Side2Selected && Side3Selected && Side4Selected && Side5Selected && (!Shift || !All)) {
		Side1Selected = false;
		Side2Selected = false;
		Side3Selected = false;
		Side4Selected = false;
		Side5Selected = false;
		return false;
	}
	return true;
}

void tileclass::Select(int s)
{ 
	if (s == 1) {
		if (Side1Selected == true) {
			Height1Selected = false;
			Height2Selected = false;
			Height3Selected = false;
			Height4Selected = false;
			Side1Selected = false;
		}
		else {
			Height1Selected = true;
			Height2Selected = true;
			Height3Selected = true;
			Height4Selected = true;
			Side1Selected = true;
		}
	}
	if (s == 2)
		if (Side2Selected == true)
			Side2Selected = false;
		else
			Side2Selected = true;
	if (s == 3)
		if (Side3Selected == true)
			Side3Selected = false;
		else
			Side3Selected = true;
	if (s == 4)
		if (Side4Selected == true)
			Side4Selected = false;
		else
			Side4Selected = true;
	if (s == 5)
		if (Side5Selected == true)
			Side5Selected = false;
		else
			Side5Selected = true;
}

void tileclass::SelectHeight(int s)
{ 
	if (s == 1 && !Height1Selected)
		Height1Selected = true;
	else if (s == 1 && Height1Selected)
		Height1Selected = false;

	if (s == 2 && !Height2Selected)
		Height2Selected = true;
	else if (s == 2 && Height2Selected)
		Height2Selected = false;

	if (s == 3 && !Height3Selected)
		Height3Selected = true;
	else if (s == 3 && Height3Selected)
		Height3Selected = false;

	if (s == 4 && !Height4Selected)
		Height4Selected = true;
	else if (s == 4 && Height4Selected)
		Height4Selected = false;
}