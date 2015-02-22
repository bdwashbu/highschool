class lines
{
public:
	lines();
	void DrawLine(int i);
	void Equation(void);
	void GridLock(float *templinex, float *templiney);
	void WallMove(float scalar);
	void DrawColors(void);
	static float *SetPointer(lines *line, float mousex, float mousey, int count);
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
	static float xoffset, yoffset;
	static void DrawMiniMap(float agentX, float agentY);
	bool moving;
	bool closed;
	bool door;
	float linex, line2x, liney, line2y;
	float movelinex, moveline2x, moveliney, moveline2y;
	float red, green, blue;
private:
	float savelinex, saveline2x, saveliney, saveline2y;
	void FindSlope(void);
};
