#ifndef _WALLS_H_
#define _WALLS_H_

class wallclass
{
public:
	wallclass();
	void DrawLine(void);
	void WriteFile(char *file);
	float yintercept, slope;
	int collidex, collidey;
	static float *templinex2;
	static float *templiney2;
	float linex, line2x, liney, line2y;
	float red, green, blue;
	float longx, shortx, longy, shorty;
private:
	void FindSlope(void);
};

#endif
