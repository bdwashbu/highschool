#include <windows.h>		
#include <gl\gl.h>			
#include "grid.h"

void gridclass::SetUp(int setX, int setY, int setZ)
{
	X = setX;
	Y = setY;
	Z = setZ;
	UpFrontRight[0] = X+1; UpFrontRight[1] = Y+1; UpFrontRight[2] = Z+1;
	UpFrontLeft[0] = X-1; UpFrontLeft[1] = Y+1; UpFrontLeft[2] = Z+1;
	UpBackRight[0] = X+1; UpBackRight[1] = Y+1; UpBackRight[2] = Z-1;
	UpBackLeft[0] = X-1;  UpBackLeft[1] = Y+1;  UpBackLeft[2] = Z-1;
	DownFrontRight[0] = X+1; DownFrontRight[1] = Y-1; DownFrontRight[0] = Z+1;
	DownFrontLeft[0] = X-1; DownFrontLeft[1] = Y-1; DownFrontLeft[2] = Z+1;
	DownBackRight[0] = X+1; DownBackRight[1] = Y-1; DownBackRight[2] = Z-1;
	DownBackLeft[0] = X-1; DownBackLeft[1] = Y-1; DownBackLeft[2] = Z-1;
}

void gridclass::DrawGrid()
{
	bool inverse = false;
	int count = 0;
	if (UpFrontRightBool)
		count++;
	if (UpFrontLeftBool)
		count++;
	if (UpBackRightBool)
		count++;
	if (UpBackLeftBool)
		count++;
	if (DownFrontRightBool)
		count++;
	if (DownFrontLeftBool)
		count++;
	if (DownBackRightBool)
		count++;
	if (DownBackLeftBool)
		count++;

	if (count > 4) {
		count = 8-count;
		inverse = true;
	}

	//if (count == 4 || count == 3)
	//{
	//	glBegin(GL_POLYGON);
	glBegin(GL_TRIANGLE_STRIP);
		//glVertex3f(0, Y+1, Z);
		//glVertex3f(0, 0, Z);
		//glVertex3f(X+1, 0, Z);

		if (UpFrontRightBool)
			glVertex3f(UpFrontRight[0], UpFrontRight[1], UpFrontRight[2]);
		if (UpFrontLeftBool)
			glVertex3f(UpFrontLeft[0], UpFrontLeft[1], UpFrontLeft[2]);
		if (UpBackRightBool)
			glVertex3f(UpBackRight[0], UpBackRight[1], UpBackRight[2]);
		if (UpBackLeftBool)
			glVertex3f(UpBackLeft[0], UpBackLeft[1], UpBackLeft[2]);
		if (DownFrontRightBool)
			glVertex3f(DownFrontRight[0], DownFrontRight[1], DownFrontRight[2]);
		if (DownFrontLeftBool)
			glVertex3f(DownFrontLeft[0], DownFrontLeft[1], DownFrontLeft[2]);
		if (DownBackRightBool)
			glVertex3f(DownBackRight[0], DownBackRight[1], DownBackRight[2]);
		if (DownBackLeftBool)
			glVertex3f(DownBackLeft[0], DownBackLeft[1], DownBackLeft[2]);
		glEnd();
	//}
}

void gridclass::SetGrid(int tempX, int tempY, int tempZ, int Radius)
{
	float temp = 0.0f;
	temp=(1.0f / ((UpFrontRight[0] - tempX)*(UpFrontRight[0] - tempX) + (UpFrontRight[1] - tempY)*(UpFrontRight[1] - tempY) + (UpFrontRight[2] - tempZ)*(UpFrontRight[2] - tempZ)))*Radius;
	if (temp > 0.3f)
		UpFrontRightBool = true;
	else
		UpFrontRightBool = false;

	temp=(1.0f / ((UpFrontLeft[0] - tempX)*(UpFrontLeft[0] - tempX) + (UpFrontLeft[1] - tempY)*(UpFrontLeft[1] - tempY) + (UpFrontLeft[2] - tempZ)*(UpFrontLeft[2] - tempZ)))*Radius;
	if (temp > 0.3f)
		UpFrontLeftBool = true;
	else
		UpFrontLeftBool = false;

	temp=(1.0f / ((UpBackRight[0] - tempX)*(UpBackRight[0] - tempX) + (UpBackRight[1] - tempY)*(UpBackRight[1] - tempY) + (UpBackRight[2] - tempZ)*(UpBackRight[2] - tempZ)))*Radius;
	if (temp > 0.3f)
		UpBackRightBool = true;
	else
		UpBackRightBool = false;

	temp=(1.0f / ((UpBackLeft[0] - tempX)*(UpBackLeft[0] - tempX) + (UpBackLeft[1] - tempY)*(UpBackLeft[1] - tempY) + (UpBackLeft[2] - tempZ)*(UpBackLeft[2] - tempZ)))*Radius;
	if (temp > 0.3f)
		UpBackLeftBool = true;
	else
		UpBackLeftBool = false;
	temp=(1.0f / ((DownFrontRight[0] - tempX)*(DownFrontRight[0] - tempX) + (DownFrontRight[1] - tempY)*(DownFrontRight[1] - tempY) + (UpFrontRight[2] - tempZ)*(UpFrontRight[2] - tempZ)))*Radius;
	if (temp > 0.3f)
		DownFrontRightBool = true;
	else
		DownFrontRightBool = false;

	temp=(1.0f / ((DownFrontLeft[0] - tempX)*(DownFrontLeft[0] - tempX) + (DownFrontLeft[1] - tempY)*(DownFrontLeft[1] - tempY) + (DownFrontLeft[2] - tempZ)*(DownFrontLeft[2] - tempZ)))*Radius;
	if (temp > 0.3f)
		DownFrontLeftBool = true;
	else
		DownFrontLeftBool = false;

	temp=(1.0f / ((DownBackRight[0] - tempX)*(DownBackRight[0] - tempX) + (DownBackRight[1] - tempY)*(DownBackRight[1] - tempY) + (DownBackRight[2] - tempZ)*(DownBackRight[2] - tempZ)))*Radius;
	if (temp > 0.3f)
		DownBackRightBool = true;
	else
		DownBackRightBool = false;

	temp=(1.0f / ((DownBackLeft[0] - tempX)*(DownBackLeft[0] - tempX) + (DownBackLeft[1] - tempY)*(DownBackLeft[1] - tempY) + (DownBackLeft[2] - tempZ)*(DownBackLeft[2] - tempZ)))*Radius;
	if (temp > 0.3f)
		DownBackLeftBool = true;
	else
		DownBackLeftBool = false;
}