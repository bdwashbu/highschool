#include "confetti.h"
#include <windows.h>		
#include <gl\gl.h>			
#include <gl\glu.h>
#include <stdlib.h>
#include <math.h>


float confetticlass::angle2 = 0;
float confetticlass::fart = 0;
float confetticlass::SpitpointX = 0;
float confetticlass::SpitpointY = 0;
float confetticlass:: SpitpointZ = 0;
float confetticlass:: ConfettiColors = 30.0f;
bool confetticlass:: ConfettiColorsUp = true;

confetticlass::confetticlass()
{
}

void confetticlass::MoveSpitPoint(bool moving)
{
	if (moving == false)
	{
	if (angle2 + 1 < 360)
		angle2++;
	else
		angle2 = 0;

	SpitpointX = (cosf(angle2/57.29f) - sinf(angle2/57.29f)*5);
	SpitpointZ = (cosf(angle2/57.29f) - sinf(angle2/57.29f)*5);
	}
}

void confetticlass::InitilizeConfetti(int trick)
{
	Shape = int(rand()%6);
	Z = SpitpointZ;//float(rand()%20)-10;
	X = SpitpointX;//float(rand()%20)-10;
	Y = 10.0f;//float(rand()%5)+10;
	int TempConfettiColors = ConfettiColors;
	Red = (float(rand()%TempConfettiColors)+(100-TempConfettiColors))/100;
	Green = (float(rand()%TempConfettiColors)+(100-TempConfettiColors))/100;
	Blue = (float(rand()%TempConfettiColors)+(100-TempConfettiColors))/100;
	RotatingXAmount = float((rand()%400)-200);
	RotatingYAmount = float((rand()%400)-200);
	RotatingZAmount = float((rand()%400)-200);
	RotationXOffset = (-0.65f/2+(float(rand()%2)-1)/10);
	RotationYOffset = (-0.65f/2+(float(rand()%2)-1)/10);
	RotationZOffset = (-0.65f/2+(float(rand()%2)-1)/10);
	XRotation = float(rand()%360);
	YRotation = float(rand()%360);
	ZRotation = float(rand()%360);
	float angle = float(rand()%360);
	Life = 1.0f;
	Rotating = true;
	Active = true;
	if (trick == 0)
	{
	MoveY = (float(rand()%90)+10)/10;
	MoveX = (cosf(angle/57.29f) - sinf(angle/57.29f)*5);
	MoveZ = (sinf(angle/57.29f) + cosf(angle/57.29f)*5);
	Gravity = (float(rand()%30)+30)/10;
	}
	//else if (trick == 1)
	//{
	//MoveY = (cosf(angle/57.29f) - sinf(angle/57.29f)*7);
	//MoveX = SpitpointX+(cosf(270/57.29f) - sinf(270/57.29f)*6);
	//MoveZ = SpitpointZ+(sinf(angle/57.29f) + cosf(angle/57.29f)*7);
	//Gravity = (float(rand()%20)+10)/10;
	//}
	if (trick == 1)
	{
	MoveY = (float(rand()%10)+20)/10;
	MoveX = (cosf(angle/57.29f) - sinf(angle/57.29f)*6);
	MoveZ = (sinf(angle/57.29f) + cosf(angle/57.29f)*6);
	Gravity = (float(rand()%10)+20)/10;
	}
	else if (trick == 2)
	{
	MoveY = (float(rand()%160)-80)/10;
	MoveX = (float(rand()%160)-80)/10;
	MoveZ = (float(rand()%160)-80)/10;
	Gravity = (float(rand()%20)+30)/10;
	}

}

void confetticlass::MoveConfetti(float scalar, bool moving)
{
	if (Active)
	{
	float plane[2] = {-1.0f,5.0};
	float planeborder[2] = {20.0, 4.0};
	if (moving == false)
	{
	X+=(MoveX*scalar);
	Z+=(MoveZ*scalar);
	MoveX*=(1.0f-scalar/4);
	MoveZ*=(1.0f-scalar/4);
	MoveY-=(scalar*Gravity);
	if (Rotating == true)
	{
	for (int i = 0; i < 2; i++)
	{
	if (Y+(MoveY*scalar) < plane[i] && X > -planeborder[i] && X < planeborder[i] && Z < planeborder[i] && Z > -planeborder[i])
	{
		Y = plane[i];
		Rotating = false;
		YRotation = float(rand()%100)-50;
		XRotation = float(rand()%100)-50+90;
		RotationXOffset = -0.65f/2;
		RotationYOffset = 0.0f;
		RotationZOffset = 0.0f;
		break;

	}
	}

	if (Rotating)
		Y+=(MoveY*scalar);

	if ((X < -19.0f || X > 19.0f || Z > 19.0f || Z < -19.0f) && Y < -1.5f)
		Active = false;

	if (XRotation < 360)
		XRotation+=(RotatingXAmount*scalar);
	else
		XRotation = 0.0;
	if (YRotation < 360)
		YRotation+=(RotatingYAmount*scalar);
	else
		YRotation = 0.0;
	if (ZRotation < 360)
		ZRotation+=(RotatingZAmount*scalar);
	else
		ZRotation = 0.0;
	}
	else
	{
		//Y += (-1.0f-Y) / (200*scalar);
		XRotation += (90.0f-XRotation) / (0.3f/scalar);
		YRotation += (0.0f - YRotation) / (0.3f/scalar);
		MoveX*=(1.0f-scalar*2.0f);
		MoveZ*=(1.0f-scalar*2.0f);
		Life-=(scalar*0.6f);
		if (Life < 0.0f)
			Active = false;
	}
	}
	}
}

void confetticlass::TossConfetti(confetticlass *confetti, float newtime, bool trick, bool moving)
{
	if (moving == false)
	{
	fart+=newtime;
	if (fart > 0.05f)
	{
	if (ConfettiColorsUp)
	{
		if (ConfettiColors + (4*newtime) < 100)
			ConfettiColors+= (4*newtime);
		else
			ConfettiColorsUp = false;
	}
	else
	{
		if (ConfettiColors - (4*newtime) > 30)
			ConfettiColors -= (4*newtime);
		else
			ConfettiColorsUp = true;
	}

	int temp = int(rand()%100);
	if (temp != 55)
	{
	for (int i = 0; i < 200; i++)
	{
		if (confetti[i].Active == false)
		{
			confetti[i].InitilizeConfetti(0);
			fart = 0;
			break;
		}
	}
	}
	else
	{
	temp = int(rand()%2)+1;
	for (int i = 0; i < 200; i++)
	{
		if (confetti[i].Active == false)
		{
			confetti[i].InitilizeConfetti(temp);
			fart = 0;
		}
	}
	}
	}
	}
}

bool confetticlass::CheckCollison()
{
	if (Y < -1.0)
	{
		return true;
		Rotating = false;
		XRotation = 0.0f;
		YRotation = 0.0f;
		ZRotation = 90.0f;
	}
	else
		return false;
}

void confetticlass::DrawConfetti(float scalar, float lightX, float lightY, float lightZ, bool moving)
{
	if (Active == true)
	{
	float distance = sqrtf((X-lightX)*(X-lightX)+(Z-lightZ)*(Z-lightZ)+(Y-lightY)*(Y-lightY));
	if (distance > 40)
		Active = false;

	glPushMatrix();
	glTranslatef(X, Y, Z);

	glRotatef(XRotation, 1.0f, 0.0f, 0.0f);
	glRotatef(YRotation, 0.0f, 1.0f, 0.0f);
	glRotatef(ZRotation, 0.0f, 0.0f, 1.0f);
	glTranslatef(RotationXOffset,RotationYOffset, RotationZOffset);

	glColor4f(Red*(1.0f-distance/40), Green*(1.0f-distance/40), Blue*(1.0f-distance/40), Life);

	if (Rotating)
		glEnable(GL_POLYGON_SMOOTH);
	if (Shape == 1.0 || Shape == 2.0  || Shape == 3.0)
	{
	glBegin(GL_TRIANGLE_STRIP);
		glVertex2f(0.65f, 0.65f);
		glVertex2f(0.0f, 0.65f);
		glVertex2f(0.65f, 0.0f);
		glVertex2f(0.0f, 0.0f);
	glEnd();
	}
	else if (Shape == 4.0 || Shape == 5.0)
	{
	glBegin(GL_TRIANGLE_STRIP);
		glVertex2f(0.65f, 0.65f);
		glVertex2f(0.0f, 0.65f);
		glVertex2f(0.65f, 0.0f);
	glEnd();
	}
	else
	{
	glBegin(GL_POINTS);
		glVertex3f(0.0, 0.0, 0.0);
	glEnd();
	}
	if (Rotating)
		glDisable(GL_POLYGON_SMOOTH);
	glPopMatrix();
	}

}