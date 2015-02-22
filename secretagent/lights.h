#include "walls.h"

class lightmaps
{
public:
	lightmaps();
	char name[30];
	float X;
	float Y;
	float radius;
	float red;
	float green;
	float blue;
	static int currentlight;
	bool active;
	void DrawLOS(lines *line, int angle);
	void SaveLights(char *newname, char *filename, int i);
	void DrawLightPoint(void);
	void DrawLightMap(void);
private:
	void FindShortest(float *shortestx, float *shortesty, float *longestx, float *longesty, float linex, float line2x, float liney, float line2y);
};
