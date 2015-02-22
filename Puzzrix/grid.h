#ifndef _GRID_H_
#define _GRID_H_

#include "fmod.h"
#include "particle.h"
#include "texture.h"
#include "headclass.h"
#include "lightmap.h"

class gridclass
{
public:
	gridclass(int tempxoffset, int tempyoffset, int tempxsize, int tempysize, int scissorheight, bool fill);
	~gridclass();
	bool Tetris;
	//bool Exploding; for chain reactions
	float Explodingtime;
	bool **Taken;
	float **Red, **Green, **Blue;//, **Alpha;
	//float *timer;
	void DrawOutline(float bonustimer) const;
	void DrawGrid(textureclass *texture, float scalar);
	void GemExplode(int x, int y);
	void Checktetris();
	int BlowUpBlocks(float scalar);
	bool GameOver(void) const;
	float AssignColor(int x, int y, int color, float defaultcolor) const;
	void Nuke();
	int fourcombos, threecombos, twocombos, singles;
private:
	int GRIDX, GRIDY;
	int scissorY;
	int xoffset, yoffset;
	FSOUND_SAMPLE *tetrissound;
	blockparticleheadclass head;
	lightheadclass lighthead;
	//bool Chain[12][18];
};

#endif