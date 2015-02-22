#include <windows.h>		
#include <gl\gl.h>			
#include <gl\glu.h>	
#include <math.h>
#include "grid.h"
#include "fmod.h"

const float gridblur = 1.35f;
const float gridblur2 = 2.35f;

gridclass::gridclass(int tempxoffset, int tempyoffset, int tempxsize, int tempysize, int scissorheight, bool fill)
	{
	GRIDX = tempxsize;
	GRIDY = tempysize;
	FSOUND_Init(44100, 32, 0);
	tetrissound = FSOUND_Sample_Load(FSOUND_UNMANAGED, "Data/tetris.wav", FSOUND_NORMAL, 0);
	scissorY = scissorheight;
	fourcombos = 0; threecombos = 0; twocombos = 0; singles = 0;
	Explodingtime = 0;
	yoffset = tempyoffset;
	xoffset = tempxoffset;
	Red = new float*[GRIDX];
	Green = new float*[GRIDX];
	Blue = new float*[GRIDX];
	//Alpha = new float*[GRIDX];
	Taken = new bool*[GRIDX];
	///timer = 0;
    for (int r = 0; r < GRIDX; r++) {
         Red[r] = new float[GRIDY];
		 Green[r] = new float[GRIDY];
		 Blue[r] = new float[GRIDY];
		 //Alpha[r] = new float[GRIDY];
		 Taken[r] = new bool[GRIDY];
	}
	for (int i = 0; i < GRIDX; i++) {
		for (int h = 0; h < GRIDY; h++) {
			Red[i][h] = 0;
			Green[i][h] = 0;
			Blue[i][h] = 0;
			//Alpha[i][h] = 0;
			Taken[i][h] = false;
		}
	}
	if (fill) {
	for (i = 0; i < GRIDX; i++) {
		for (int h = 0; h < GRIDY; h++) {
			Taken[i][h] = true;
			do {
			Red[i][h] = rand()%100/100.0f;
			Green[i][h] = rand()%100/100.0f;
			Blue[i][h] = rand()%100/100.0f;
			//Alpha[i][h] = rand()%100/100.0f;
			} while ((Red[i][h]+Green[i][h]+Blue[i][h] < 1.0f || Red[i][h]+Green[i][h]+Blue[i][h] > 1.5f) ||
				(Red[i][h] < 0.7f && Green[i][h] < 0.7f && Blue[i][h] < 0.7f));
		}
	}
	}

}


gridclass::~gridclass()
{
	head.DeleteAll();
	lighthead.DeleteAll();
	FSOUND_Sample_Free(tetrissound);
	for (int r = GRIDX-1; r > 0; r--) {
          delete [] Red[r];
		  delete [] Green[r];
		  delete [] Blue[r];
		  delete [] Taken[r];
		  //delete [] Alpha[r];
	}
	//delete [] timer;
	//delete [] Alpha;
    delete [] Red;
	delete [] Green;
	delete [] Blue;
	delete [] Taken;
}


void gridclass::DrawGrid(textureclass *texture, float scalar)
{ 

	//tranparent backround, so goes in back
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_LINE_LOOP);
		glVertex2i(xoffset+GRIDX*24+2, yoffset+scissorY+1);
		glVertex2i(xoffset-2, yoffset+scissorY+1);
		glVertex2i(xoffset-2, yoffset-2);
		glVertex2i(xoffset+GRIDX*24+2, yoffset-2);
	glEnd();

	glScissor(xoffset-1, yoffset, GRIDX*24+2, scissorY);
	glEnable(GL_SCISSOR_TEST);

	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(0.0f, 0.0f, 0.0f, 0.4f);
	glBegin(GL_QUADS);  
	glVertex2i(xoffset+GRIDX*24+2, yoffset+GRIDY*24+1);
	glVertex2i(xoffset-3, yoffset+GRIDY*24+1);
	glVertex2i(xoffset-3, yoffset);
	glVertex2i(xoffset+GRIDX*24+2, yoffset);
	glEnd();

	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	glBindTexture(GL_TEXTURE_2D, texture[0].texID);
	glBegin(GL_QUADS);
	for(int i = 0; i < GRIDX; i++)
	{
		for(int f = 0; f < GRIDY; f++)
		{
			if (Taken[i][f] == false)  //draws blocks
				continue;

				//glColor3f(Red[i][f], Green[i][f], Blue[i][f]);
				glColor3f((Red[i][f]*gridblur+(AssignColor(i+1,f,1,Red[i][f])+AssignColor(i+1,f+1,1,Red[i][f])+AssignColor(i,f+1,1,Red[i][f]))/3.0f)/gridblur2, (Green[i][f]*gridblur+(AssignColor(i+1,f,2,Green[i][f])+AssignColor(i+1,f+1,2,Green[i][f])+AssignColor(i,f+1,2,Green[i][f]))/3.0f)/gridblur2, (Blue[i][f]*gridblur+(AssignColor(i+1,f,3,Blue[i][f])+AssignColor(i+1,f+1,3,Blue[i][f])+AssignColor(i,f+1,3,Blue[i][f]))/3.0f)/gridblur2);
					glTexCoord2i(1,1); glVertex2i((i*24)+24+xoffset, (f*24)+24+yoffset);
				glColor3f((Red[i][f]*gridblur+(AssignColor(i-1,f,1,Red[i][f])+AssignColor(i-1,f+1,1,Red[i][f])+AssignColor(i,f+1,1,Red[i][f]))/3.0f)/gridblur2, (Green[i][f]*gridblur+(AssignColor(i-1,f,2,Green[i][f])+AssignColor(i-1,f+1,2,Green[i][f])+AssignColor(i,f+1,2,Green[i][f]))/3.0f)/gridblur2, (Blue[i][f]*gridblur+(AssignColor(i-1,f,3,Blue[i][f])+AssignColor(i-1,f+1,3,Blue[i][f])+AssignColor(i,f+1,3,Blue[i][f]))/3.0f)/gridblur2);
					glTexCoord2i(0,1); glVertex2i((i*24)+xoffset, (f*24)+24+yoffset);
				glColor3f((Red[i][f]*gridblur+(AssignColor(i-1,f,1,Red[i][f])+AssignColor(i-1,f-1,1,Red[i][f])+AssignColor(i,f-1,1,Red[i][f]))/3.0f)/gridblur2, (Green[i][f]*gridblur+(AssignColor(i-1,f,2,Green[i][f])+AssignColor(i-1,f-1,2,Green[i][f])+AssignColor(i,f-1,2,Green[i][f]))/3.0f)/gridblur2, (Blue[i][f]*gridblur+(AssignColor(i-1,f,3,Blue[i][f])+AssignColor(i-1,f-1,3,Blue[i][f])+AssignColor(i,f-1,3,Blue[i][f]))/3.0f)/gridblur2);
					glTexCoord2f(0.0f,0.01f); glVertex2i((i*24)+xoffset, (f*24)+yoffset);
				glColor3f((Red[i][f]*gridblur+(AssignColor(i+1,f,1,Red[i][f])+AssignColor(i+1,f-1,1,Red[i][f])+AssignColor(i,f-1,1,Red[i][f]))/3.0f)/gridblur2, (Green[i][f]*gridblur+(AssignColor(i+1,f,2,Green[i][f])+AssignColor(i+1,f-1,2,Green[i][f])+AssignColor(i,f-1,2,Green[i][f]))/3.0f)/gridblur2, (Blue[i][f]*gridblur+(AssignColor(i+1,f,3,Blue[i][f])+AssignColor(i+1,f-1,3,Blue[i][f])+AssignColor(i,f-1,3,Blue[i][f]))/3.0f)/gridblur2);
					glTexCoord2f(1.0f,0.01f); glVertex2i((i*24)+24+xoffset, (f*24)+yoffset);
		}
	}
	glEnd();
	glEnable(GL_BLEND);

	lighthead.Check();
	glBindTexture(GL_TEXTURE_2D, texture[3].texID);
		for (lightmapclass *draw = lighthead.first.next; draw != 0; draw=draw->next) {
		draw->DrawLightmap(scalar);
	}
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_LINE_SMOOTH);
	glDisable(GL_SCISSOR_TEST);

	//watch over the particles you own
	head.Check();
	for (blockparticle *draw2 = head.first.next; draw2 != 0; draw2=draw2->next) {
		draw2->DrawParticle(scalar);
	}
}

void gridclass::DrawOutline(float bonustimer) const
{
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_SCISSOR_TEST);
	glColor4f(1.0f, 1.0f-bonustimer/5.0f, 1.0f-bonustimer/5.0f, 0.5f);
	glDisable(GL_LINE_SMOOTH);
	glLineWidth(1.5f);
	glBegin(GL_LINES);
	for(int i = 0; i < GRIDX; i++) {
		for(int f = 0; f < GRIDY; f++) {
			if (Taken[i][f] == false)
				continue;
			if (f+1 < GRIDY) {
				if (Taken[i][f+1] == false) {
						glVertex2i((i*24)+xoffset, (f*24)+24+yoffset);
						glVertex2i((i*24)+24+xoffset, (f*24)+24+yoffset);
				}
			}
			if (i+1 < GRIDX) {
				if (Taken[i+1][f] == false) {
						glVertex2i((i*24)+24+xoffset, (f*24)+24+yoffset);
						glVertex2i((i*24)+24+xoffset, (f*24)+yoffset);
				}
			}
			if (f-1 > -1) {
				if (Taken[i][f-1] == false) {
						glVertex2i((i*24)+24+xoffset, (f*24)+yoffset);
						glVertex2i((i*24)+xoffset, (f*24)+yoffset);
				}
			}
			if (i-1 > -1) {
				if (Taken[i-1][f] == false) {
						glVertex2i((i*24)+xoffset, (f*24)+yoffset);
						glVertex2i((i*24)+xoffset, (f*24)+24+yoffset);
				}
			}
		}
	}
	glEnd();
	glDisable(GL_SCISSOR_TEST);
}

/*
void gridclass::ComputeHeightMap()
{
	for (int i = 0; i < GRIDX; i++)
	{
		for (int f = GRIDY-1; f > -1; f--)
		{
			if (Taken[i][f])
			{
				HeightMap[i] = f;
				break;
			}
			else
				HeightMap[i] = 0;
		}
	}
}
*/

/*
void gridclass::GemExplode(int x, int y)
{
	bool temp = false;
	bool Chain2[GRIDX][GRIDY];
	for (int f = 0; f < GRIDX; f++) {
		for (int i = 0; i < GRIDY; i++)
			Chain2[f][i] = false;
	}

	if (x != -1 && y != -1) {
		Chain[x][y] = true;
		currentactive = 1;
	}

	if (currentactive > 0)
	{
		for (int f = 0; f < GRIDY; f++)
		{
		for (int i = 0; i < GRIDX; i++)
		{
		if (Chain[i][f] == true)
		{
			if (i-1 > -1)
			{
			if (Red[i-1][f] == Red[i][f] && Green[i-1][f] == Green[i][f] && Blue[i-1][f] == Blue[i][f] && Taken[i-1][f] && Chain2[i-1][f] != true)
			{
				currentactive++;
				Chain2[i-1][f] = true;
			}
			}

			if (f-1 > -1)
			{
			if (Red[i][f-1] == Red[i][f] && Green[i][f-1] == Green[i][f] && Blue[i][f-1] == Blue[i][f] && Taken[i][f-1] && Chain2[i][f-1] != true)
			{
				currentactive++;
				Chain2[i][f-1] = true;
			}
			}

			if (i+1 < GRIDX)
			{
			if (Red[i+1][f] == Red[i][f] && Green[i+1][f] == Green[i][f] && Blue[i+1][f] == Blue[i][f] && Taken[i+1][f] && Chain2[i+1][f] != true)
			{
				currentactive++;
				Chain2[i+1][f] = true;
			}
			}

			if (f+1 < GRIDY)
			{
			if (Red[i][f+1] == Red[i][f] && Green[i][f+1] == Green[i][f] && Blue[i][f+1] == Blue[i][f] && Taken[i][f+1] && Chain2[i][f+1] != true)
			{
				currentactive++;
				Chain2[i][f+1] = true;
			}
			}
			
			particlearray[i][f] = new particleclass[10];
			for (int h = 0; h < 10; h++) {
			particlearray[i][f][h].ParticleInit(i*24.0f+190, f*24.0f+22, Red[i][f], Green[i][f], Blue[i][f], float(rand()%720)-360, float(rand()%720)-360);
			}
			Chain[i][f] = false;
			Taken[i][f] = false;
			Red[i][f] = 0.0f;
			Green[i][f] = 0.0f;
			Blue[i][f] = 0.0f;
			currentactive--;
		}
		}
		}
	}
	else
		Exploding = false;

	for (f = 0; f < GRIDX; f++) {
		for (int i = 0; i < GRIDY; i++) {
			if (Chain2[f][i]) {
				Chain2[f][i] = false;
				Chain[f][i] = true;
			}
		}
	}

}
*/

void gridclass::Checktetris()
{
	Tetris = false;
	bool temptetris = false;
	for (int i = 0; i < GRIDY; i++)
	{
		temptetris = true;
		for(int f = 0; f < GRIDX; f++)
		{
			if (Taken[f][i] == false)
			{
				temptetris = false;
				break;
			}
		}
		if (temptetris == true) {//a tetris DOES exist
			Tetris = true;
			break;
		}
	}
}

bool gridclass::GameOver() const
{
	for (int i = 5; i < 8; i++) {
		if (Taken[i][GRIDY-3] || Taken[i][GRIDY-2] || Taken[i][GRIDY-1])
			return true;
	}
	return false;
}

int gridclass::BlowUpBlocks(float scalar)
{
	int total = 0;
	bool temptetris = true;
	if (Tetris) {
	bool gosound = false;
	for (int i = 0; i < GRIDY; i++)
	{
		temptetris = true;
		for(int f = 0; f < GRIDX; f++)
		{
			if (Taken[f][i] == false)
			{
				temptetris = false;
				break;
			}
		}

		if (temptetris) {
		for (f = 0; f < GRIDX; f++) {
			Red[f][i]+=scalar*5.5f;
			Green[f][i]+=scalar*5.5f;
			Blue[f][i]+=scalar*5.5f;
		}
		break;
		}
	}

		for (int y = 0; y < GRIDY-1; y++) {
			bool blown = true;
				for (int x = 0; x < GRIDX; x++) {
					if (Red[x][y] < 1.0f || Green[x][y] < 1.0f || Blue[x][y] < 1.0f) {
						blown = false;
						break;
					}
				}

				if (blown)	{
				bool temptetris = true;
				int counter = 0;
				for (int i = 0; i < GRIDY; i++)
				{
					temptetris = true;
					for(int f = 0; f < GRIDX; f++)
					{
						if (Taken[f][i] == false)
						{
							temptetris = false;
							break;
						}
					}
					if (temptetris == true)
						counter++;
				}
				total=counter*25;
				for (int f = 0; f < GRIDX; f++) {
				for (int h = 0; h < 10; h++) {
					head.Insert(f*24+xoffset+12, y*24+yoffset+12);
				}
				}

				for (f = 0; f < GRIDX; f++) {
					lighthead.Insert(f*24.0f+xoffset+12, y*24.0f+yoffset+12);
				}
					

				for (int y2 = y; y2 < GRIDY-1; y2++) {
				for (int x = 0; x < GRIDX; x++) {
						gosound = true;
						Taken[x][y2] = Taken[x][y2+1];
						Red[x][y2] = Red[x][y2+1];
						Green[x][y2] = Green[x][y2+1];
						Blue[x][y2] = Blue[x][y2+1];
				}
				}
				}

			}
			

	//}
	if (gosound)
		FSOUND_PlaySound(FSOUND_FREE, tetrissound);
	}
	return total;

}

float gridclass::AssignColor(int x, int y, int color, float defaultcolor) const
{
	if (y < 0 || x < 0 || y > GRIDY-1 || x > GRIDX-1)
		return 1.15f;
	else if (Taken[x][y] == false)
		return defaultcolor;
	else {
		if (color == 1)
			return Red[x][y];
		else if (color == 2)
			return Green[x][y];
		else
			return Blue[x][y];
	}
}

void gridclass::Nuke()
{
	for (int y = 0; y < GRIDY; y++) {
		for (int f = 0; f < GRIDX; f++) {
			if (Taken[f][y] == true) {
			Taken[f][y] = false;
			for (int h = 0; h < 5; h++) {
				head.Insert(f*24+xoffset+12, y*24+yoffset+12);
			}
			lighthead.Insert(f*24.0f+xoffset+12, y*24.0f+yoffset+12);
			}
		}
	}
}
