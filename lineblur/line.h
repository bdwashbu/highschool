#ifndef _LINE_H_
#define _LINE_H_

class lineclass
{
public:
	lineclass();
	void DrawLine(float scalar);
	bool SetLine(float x, float y, float x2, float y2);
private:
	float X, Y, X2, Y2;
	float Life;
	bool Alive;
};

#endif