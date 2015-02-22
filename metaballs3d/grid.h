#ifndef _GRID_H_
#define _GRID_H_

class gridclass
{
public:
	void SetUp(int setX, int setY, int setZ);
	void SetGrid(int tempX, int tempY, int tempZ, int Radius);
	void DrawGrid();
private:
	bool UpFrontRightBool, UpFrontLeftBool, UpBackRightBool, UpBackLeftBool, 
		DownFrontRightBool, DownFrontLeftBool, DownBackRightBool, DownBackLeftBool;
	float X, Y, Z;
	float UpFrontRight[3], UpFrontLeft[3], UpBackRight[3], UpBackLeft[3], 
		DownFrontRight[3], DownFrontLeft[3], DownBackRight[3], DownBackLeft[3];
};

#endif