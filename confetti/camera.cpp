#include "camera.h"

cameraclass::cameraclass()
{
	Angle = -10.0f;
}

void cameraclass::MoveCamera(float scalar)
{
	if (Rotation+(scalar*15) < 360)
		Rotation+=(scalar*15);
	else
		Rotation = 0;

	if (Goingup == true)
	{
	if (Angle+(1.5f*scalar) < -15)
		Angle+= (1.5f*scalar);
	else
	{
		Goingup = false;
	}
	}
	else
	{
	if (Angle-(1.5f*scalar) > -25)
		Angle-=(1.5f*scalar);
	else
	{
		Goingup = true;
	}
	}
}