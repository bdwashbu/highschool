#ifndef _BALL_H_
#define _BALL_H_

class ballclass
{
public:
	float X, Y, Z;
	float Radius;
	ballclass();
	void MoveBall(float scalar);
	bool CheckPoint(float Xpoint, float Ypoint);
private:
};

#endif