#ifndef __wallclass__
#define __wallclass__

class wallclass
{
public:
	wallclass();
	void DrawLine(int i);
	void Equation(void);
	void GridLock(float *templinex, float *templiney);
	void DrawColors(void);
	static float *SetPointer(wallclass *wall, float mousex, float mousey, int count);
	float *SetxRightPointer(float mousex, float mousey, float *templinex, float *templiney);
	float *SetyRightPointer(float mousex, float mousey, float *templinex, float *templiney);
	bool SelectWall(float mousex, float mousey, float *maxdistance);
	void WriteFile(char *file);
	float yintercept, slope;
	int collidex, collidey;
	static bool gridlock;
	static float *templinex2;
	static float *templiney2;
	static float distance;
	static int currentwall;
	float linex, line2x, liney, line2y;
	float red, green, blue;
	float thickness;
private:
	float savelinex, saveline2x, saveliney, saveline2y;
	void FindSlope(void);
};

#endif