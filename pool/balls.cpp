#include "test.h"
#include <math.h>
#include <windows.h>
#include <stdlib.h>		
#include <gl\gl.h>			
#include <gl\glu.h>

ballclass::ballclass()
{
	do
	{
	Red = float(rand()%100)/100.0f;
	Green = float(rand()%100)/100.0f;
	Blue = float(rand()%100)/100.0f;
	} while (Red + Green + Blue < 0.8f);

	Alive = true;
	moving = false;
	speed = 0;
	angle = 0;
	RotateX = 270;
	RotateY = 270;
	for (int i = 0; i < 20; i++)
		Bluractive[i] = false;

	X = 320;
	Y = 240;
}

void ballclass::Normalize(float *XVector, float *YVector)
{
	float length = sqrtf((*XVector * *XVector)+(*YVector * *YVector));
	*XVector/=length;
	*YVector/=length;
}

void ballclass::Hole(float HoleX, float HoleY, float scalar)
{
	if (sqrt((X-HoleX)*(X-HoleX)+(Y-HoleY)*(Y-HoleY)) < 4)
		Alive = false;
	else if (sqrt((X-HoleX)*(X-HoleX)+(Y-HoleY)*(Y-HoleY)) < 9)
	{
	float tempangle;
  float tempHypot = hypot(X-HoleX,Y-HoleY);
  if (X < HoleX)
    tempangle = -acos((-Y+HoleY)/-tempHypot);
  else
    tempangle = acos((Y-HoleY)/tempHypot);
  float tempxvector = HoleX-X;
  float tempyvector = HoleY-Y;
  Normalize(&tempxvector, &tempyvector);
  float dot = (XVector*tempxvector+YVector*tempyvector);
  float dist = sqrt((X-HoleX)*(X-HoleX)+(Y-HoleY)*(Y-HoleY));
  XVector -= sin(tempangle)*(scalar*(1/dist)*90); //set xp according to distance from 0(center of screen)
  YVector -= cos(tempangle)*(scalar*(1/dist)*90); //set yp according to distance from 0(center of screen) 
	Normalize(&XVector, &YVector);
	speed-=200*scalar;
	}

}

void ballclass::Move(float scalar, int iteration)
{
	float tempspeed = speed/iteration;
	static double Blurdist;
	X+=XVector*(scalar*speed)/iteration;
	Y+=YVector*(scalar*speed)/iteration;
	RotateX+=((XVector*(speed*scalar))/(2*3.14f*6)*360)/iteration;
	RotateY+=((YVector*(speed*scalar))/(2*3.14f*6)*360)/iteration;
	if (RotateX > 360)
		RotateX = 0;
	if (RotateX < 0)
		RotateX = 360;
	if (RotateY > 360)
		RotateY = 0;
	if (RotateY < 0)
		RotateY = 360;
	if (speed - (35*scalar/iteration) > 0)
		speed-=(35*scalar/iteration);
	if (speed < 2)
		moving = false;
	Blurdist+=sqrt((XVector*(scalar*tempspeed))*(XVector*(scalar*tempspeed))+(YVector*(scalar*tempspeed))*(YVector*(scalar*tempspeed)));
	if (Blurdist > 3)
	{
		for(int i = 0; i < 20; i++)
		{
			if (Bluractive[i] == false)
			{
				Bluractive[i] = true;
				Blurlife[i] = speed/600;
				if (Blurlife[i] > 1.0f)
					Blurlife[i] = 1.0f;
				Blurx[i] = X-XVector*(scalar*tempspeed);
				Blury[i] = Y-YVector*(scalar*tempspeed);
				Blurdist = 0.0f;
				break;
			}
		}
	}
}

//void ballclass::Aim(bool left, bool right, float scalar, bool shoot, ballclass *ball, int ballcount)
void ballclass::Aim(float mousex, float mousey, ballclass *ball, int ballcount, bool shoot, wallclass *wall, int wallcount,
					bool left, bool right, float scalar)
{	
	float ballrange = 0;
	static float savemousex;
	static float savemousey;
	float shortest = 10000, realfinalxpoint, realfinalypoint;
	float tempxvector = mousex-X;
	float tempyvector = mousey-Y;
	float tempxvector3, tempyvector3;
	float tempyvector4 = 0, tempxvector4 = 0;
	Normalize(&tempxvector, &tempyvector);

	//a.b = |a||b| cos t
	if (savemousex != mousex || savemousey != mousey)
	{
	angle = acosf(tempyvector) * (180 / 3.14159f);
	if (mousex < X)
		angle = 360-angle;
	}
	else
	{
	if (right)
	{
		if (angle - 1*scalar > 0)
			angle-=1*scalar;
		else
			angle = 360;
	}
	if (left)
	{
		if (angle + 1*scalar < 360)
			angle+=1*scalar;
		else
			angle = 0;
	}
	}
	tempxvector = -(cosf(angle/57.29f) - sinf(angle/57.29f)*350);
	tempyvector = (sinf(angle/57.29f) + cosf(angle/57.29f)*350);
	//losx = losx;
	//losy = losy;
	//Normalize(&losx, &losy);
	//tempxvector = (losx+tempxvector)/2;
	//tempyvector = (losy+tempyvector)/2;
	Normalize(&tempxvector, &tempyvector);

	float tempslope = tempyvector/tempxvector;
	float tempyint = Y-tempslope*X;

	//project line to a wall
	for (int p = 0; p < wallcount; p++)
	{
	float finalxpoint = (tempyint - wall[p].yintercept)/(wall[p].slope-tempslope);
	float finalypoint = tempslope*finalxpoint+tempyint;
		float tempx;
		float tempyint2 = finalypoint - (-1/wall[p].slope)*finalxpoint;
			tempx = (tempyint2-(wall[p].yintercept-1))/(wall[p].slope-(-1/wall[p].slope));
		float tempy = wall[p].slope*tempx + (wall[p].yintercept-1);
		float t = 8/sqrtf((finalypoint-tempy)*(finalypoint-tempy)+(finalxpoint-tempx)*(finalxpoint-tempx));
		if ((Y < wall[p].slope*X + wall[p].yintercept && wall[p].slope < 0) ||
			(Y < wall[p].slope*X + wall[p].yintercept && wall[p].slope > 0))
			finalxpoint = (tempyint - (wall[p].yintercept-t))/(wall[p].slope-tempslope);
		else
			finalxpoint = (tempyint - (wall[p].yintercept+t))/(wall[p].slope-tempslope);
		finalypoint = tempslope*finalxpoint+tempyint;
		tempyint2 = finalypoint - (-1/wall[p].slope)*finalxpoint;
		float finalxpoint2 = (tempyint2 - wall[p].yintercept)/(wall[p].slope-(-1/wall[p].slope));
		float finalypoint2 = wall[p].slope*finalxpoint2+wall[p].yintercept;
		if (sqrt((Y-finalypoint)*(Y-finalypoint)+(X-finalxpoint)*(X-finalxpoint)) < shortest && ((
		finalxpoint2 > wall[p].shortx && finalxpoint2 < wall[p].longx) || (finalypoint2 > wall[p].shorty && finalypoint2 < wall[p].longy)) &&
		((finalxpoint > X && tempxvector > 0) || (finalxpoint < X && tempxvector < 0)) && ((finalypoint > Y && tempyvector > 0) || (finalypoint < Y && tempyvector < 0)))
	{
		shortest = sqrtf((Y-finalypoint)*(Y-finalypoint)+(X-finalxpoint)*(X-finalxpoint));
		realfinalxpoint = finalxpoint;
		realfinalypoint = finalypoint;
		float YNormal = 1.0f;
		float XNormal = -wall[p].slope;
		Normalize(&XNormal, &YNormal);
		tempxvector3 = tempxvector - 2*((tempxvector*XNormal)+(tempyvector*YNormal))*XNormal;
		tempyvector3 = tempyvector - 2*((tempxvector*XNormal)+(tempyvector*YNormal))*YNormal;
		Normalize(&tempxvector3, &tempyvector3);
		Oncewallhitxvector = tempxvector3;
		Oncewallhityvector = tempyvector3;
		Oncewallhitxspot = realfinalxpoint;
		Oncewallhityspot = realfinalypoint;
		ballrange = 1/(shortest/50)*100;
		if (ballrange > 50.0f)
			ballrange = 50.0f;
	}
	}

	//project line to a ball
	for (p = 1; p < ballcount; p++)
	{
	if (ball[p].Alive == false)
		continue;
	//if (&ball[p].X == &X);
	//	continue;
	float t;
	float B = 2*((tempxvector)*(X-ball[p].X)+(tempyvector)*(Y-ball[p].Y));
	float C = (X-ball[p].X)*(X-ball[p].X)+(Y-ball[p].Y)*(Y-ball[p].Y) - (12*12);
	if ((-B + sqrtf(B*B-4*C))/2 >= 0 && (-B + sqrt(B*B-4*C))/2 < (-B - sqrt(B*B-4*C))/2)
		t = (-B + sqrtf(B*B-4*C))/2;
	else if ((-B - sqrtf(B*B-4*C))/2 >= 0 && (-B - sqrt(B*B-4*C))/2 < (-B + sqrt(B*B-4*C))/2)
		t = (-B - sqrtf(B*B-4*C))/2;
	else
		continue;
	//check to see if ray is shortest intersection thus far
	if (sqrtf((tempyvector*t)*(tempyvector*t)+(tempxvector*t)*(tempxvector*t)) < shortest)
	{
		shortest = sqrtf((tempyvector*t)*(tempyvector*t)+(tempxvector*t)*(tempxvector*t));
		realfinalxpoint = X+tempxvector*t;
		realfinalypoint = Y+tempyvector*t;
		tempxvector3 = ball[p].X-(X+tempxvector*t);
		tempyvector3 = ball[p].Y-(Y+tempyvector*t);
		Normalize(&tempxvector3, &tempyvector3);


		//calculate cue ball reflection vector
			if((tempxvector*-tempyvector3+tempyvector*tempxvector3) < (tempxvector*tempyvector3+tempyvector*-tempxvector3))
			{
			tempyvector4=-tempxvector3;
			tempxvector4=tempyvector3;
			}
			else
			{
			tempyvector4=tempxvector3;
			tempxvector4=-tempyvector3;			
			}

		ballrange = (tempxvector*tempxvector3+tempyvector*tempyvector3)*40;

		Oncewallhitxvector = 0;//tempxvector3;
		Oncewallhityvector = 0;//tempyvector3;
		Oncewallhitxspot = 0;//realfinalxpoint;
		Oncewallhityspot = 0;//realfinalypoint;

	}
	}

	//for linex, liney corners
	for (int i = 0; i < wallcount; i++)
	{
	//if (&ball[p].X == &X);
	//	continue;
	float t;
	float B = 2*((tempxvector)*(X-wall[i].linex)+(tempyvector)*(Y-wall[i].liney));
	float C = (X-wall[i].linex)*(X-wall[i].linex)+(Y-wall[i].liney)*(Y-wall[i].liney) - (8*8);
	if ((-B + sqrtf(B*B-4*C))/2 >= 0 && (-B + sqrt(B*B-4*C))/2 < (-B - sqrt(B*B-4*C))/2)
		t = (-B + sqrtf(B*B-4*C))/2;
	else if ((-B - sqrtf(B*B-4*C))/2 >= 0 && (-B - sqrt(B*B-4*C))/2 < (-B + sqrt(B*B-4*C))/2)
		t = (-B - sqrtf(B*B-4*C))/2;
	else
		continue;
//	float finalxpoint = X+tempxvector*t;
//	float finalypoint = Y+tempyvector*t;
	if (sqrtf((tempyvector*t)*(tempyvector*t)+(tempxvector*t)*(tempxvector*t)) < shortest)
	{
		shortest = sqrtf((tempyvector*t)*(tempyvector*t)+(tempxvector*t)*(tempxvector*t));
		realfinalxpoint = X+tempxvector*t;
		realfinalypoint = Y+tempyvector*t;
		float XNormal = wall[i].linex-realfinalxpoint;
		float YNormal = wall[i].liney-realfinalypoint;
		Normalize(&XNormal, &YNormal);
		tempxvector3 = tempxvector - 2*((tempxvector*XNormal)+(tempyvector*YNormal))*XNormal;
		tempyvector3 = tempyvector - 2*((tempxvector*XNormal)+(tempyvector*YNormal))*YNormal;
		Normalize(&tempxvector3, &tempyvector3);
		Oncewallhitxvector = tempxvector3;
		Oncewallhityvector = tempyvector3;
		Oncewallhitxspot = realfinalxpoint;
		Oncewallhityspot = realfinalypoint;
	}
	}

	//for line2x, line2y corners
	for (i = 0; i < wallcount; i++)
	{
	//if (&ball[p].X == &X);
	//	continue;
	float t;
	float B = 2*((tempxvector)*(X-wall[i].line2x)+(tempyvector)*(Y-wall[i].line2y));
	float C = (X-wall[i].line2x)*(X-wall[i].line2x)+(Y-wall[i].line2y)*(Y-wall[i].line2y) - (8*8);
	if ((-B + sqrtf(B*B-4*C))/2 >= 0 && (-B + sqrt(B*B-4*C))/2 < (-B - sqrt(B*B-4*C))/2)
		t = (-B + sqrtf(B*B-4*C))/2;
	else if ((-B - sqrtf(B*B-4*C))/2 >= 0 && (-B - sqrt(B*B-4*C))/2 < (-B + sqrt(B*B-4*C))/2)
		t = (-B - sqrtf(B*B-4*C))/2;
	else
		continue;
	float finalxpoint = X+tempxvector*t;
	float finalypoint = Y+tempyvector*t;
	if (sqrt((Y-finalypoint)*(Y-finalypoint)+(X-finalxpoint)*(X-finalxpoint)) < shortest)
	{
		shortest = sqrtf((Y-finalypoint)*(Y-finalypoint)+(X-finalxpoint)*(X-finalxpoint));
		realfinalxpoint = finalxpoint;
		realfinalypoint = finalypoint;
		float XNormal = wall[i].line2x-realfinalxpoint;
		float YNormal = wall[i].line2y-realfinalypoint;
		Normalize(&XNormal, &YNormal);
		tempxvector3 = tempxvector - 2*((tempxvector*XNormal)+(tempyvector*YNormal))*XNormal;
		tempyvector3 = tempyvector - 2*((tempxvector*XNormal)+(tempyvector*YNormal))*YNormal;
		Normalize(&tempxvector3, &tempyvector3);
		Oncewallhitxvector = tempxvector3;
		Oncewallhityvector = tempyvector3;
		Oncewallhitxspot = realfinalxpoint;
		Oncewallhityspot = realfinalypoint;
	}
	}

	glLineWidth(1.0f);
	glColor4f(1.0f, 1.0f, 1.0f, 0.6f);
	glDisable(GL_LINE_SMOOTH);
		glBegin(GL_LINES);
			glVertex2f(realfinalxpoint, realfinalypoint);
			if (ballrange > 25)
				glVertex2f(realfinalxpoint+(tempxvector3*ballrange), realfinalypoint+(tempyvector3*ballrange));
			else
				glVertex2f(realfinalxpoint+(tempxvector3*25), realfinalypoint+(tempyvector3*25));
		glEnd();
		if (tempxvector4 != 0)
		{
		glBegin(GL_LINES);
				glVertex2f(realfinalxpoint, realfinalypoint);
			if ((40-ballrange) > 15)
				glVertex2f(realfinalxpoint+((40-ballrange)*tempxvector4), realfinalypoint+((40-ballrange)*tempyvector4));
			else
				glVertex2f(realfinalxpoint+(15*tempxvector4), realfinalypoint+(15*tempyvector4));
		glEnd();
		}
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_LINE_SMOOTH);
	glLineWidth(1.4f);


	glColor3f(0.6f, 0.4f, 0.12f);
	glBegin(GL_LINES);
		glVertex2f(X-(tempxvector*10),Y-(tempyvector*10));
		glVertex2f(X-(tempxvector*100), Y-(tempyvector*100));
	glEnd();
	glColor3f(1.0f, 1.0f, 1.0f);
	glLineStipple(4, 0xAAAA);
	glEnable(GL_LINE_STIPPLE);
	glBegin(GL_LINES);
		glVertex2f(X,Y);
		glVertex2f(realfinalxpoint, realfinalypoint);
	glEnd();
	glDisable(GL_LINE_STIPPLE);
	glColor4f(0.0f, 0.0f, 0.0f, 0.6f);
	glPointSize(11);
	glBegin(GL_POINTS);
		glVertex2f(realfinalxpoint, realfinalypoint);
	glEnd();

	if (shoot)
	{
	//float tempxslope = RotationX - X;
	//float tempyslope = RotationY - Y;
	XVector = tempxvector;
	YVector = tempyvector;
	moving = true;
	for (int i = 0; i < ballcount; i++)
	{
	ball[i].currentwallhit = -1;
	ball[i].currentballhit = -1;
	}
	speed = 300;
	}
	savemousex = mousex;
	savemousey = mousey;
}

void ballclass::DrawBlur(float scalar)
{	
	glDepthMask(false);
	glPointSize(12);
	glBegin(GL_POINTS);
	for (int i = 0; i < 20; i++)
	{
		if (Bluractive[i] == true)
		{
		glColor4f(Red, Green, Blue, Blurlife[i]);
		glVertex2f(Blurx[i], Blury[i]);
		Blurlife[i] -= (scalar*2.5f);
		if (Blurlife[i] < 0.0f)
			Bluractive[i] = false;
		}
	}
	glEnd();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glDepthMask(true);
}

void ballclass::DrawBall(GLuint sphereDL)
{
	glTranslatef(X, Y, 0.0f);
	//glTranslatef(X/58.181f-5.5f, Y/58.53f-4.1f, 0.0f);
	glRotatef(RotateY, 1.0f, 0.0f, 0.0f);
	glRotatef(RotateX, 0.0f, 1.0f, 0.0f);
	glBindTexture(GL_TEXTURE_2D, 2);
	glColor3f(Red, Green, Blue);
	GLfloat mat_emission[] = {Red/4, Green/4, Blue/4, 0.0f};	glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
	glCallList(sphereDL);
}

bool ballclass::CollisionDetection(wallclass *wall, float scalar, ballclass *ball, int ballnumber, particleclass *particle, FSOUND_SAMPLE *sound1, int count, int ballcount)
{
	bool Alreadyhit = false;
	if (sqrt((X-Oncewallhitxspot)*(X-Oncewallhitxspot)+(Y-Oncewallhityspot)*(Y-Oncewallhityspot)) < 1)
	{
		X = Oncewallhitxspot;
		Y = Oncewallhityspot;
		XVector = Oncewallhitxvector;
		YVector = Oncewallhityvector;
		X+=XVector;
		Y+=YVector;
		Oncewallhitxspot = 0; Oncewallhityspot = 0;
		Alreadyhit = true;
	}

	if (Alreadyhit == false)
	{
	for (int i = 0; i < count; i++)
	{
	//float newintercept = Y-(YVector/XVector)*X;
	//float closestxpoint = (newintercept - wall[i].yintercept)/((wall[i].slope)-(YVector/XVector));
	//float closestypoint = (YVector/XVector)*closestxpoint + newintercept;


	//if (((closestxpoint > wall[i].shortx && closestxpoint < wall[i].longx) || (closestypoint > wall[i].shorty && closestypoint < wall[i].longy)) && i != currentwallhit)
	//{
		float newslope = -1.0f/wall[i].slope;
		float newintercept = Y-newslope*X;
		float closestxpoint = (newintercept - wall[i].yintercept)/((wall[i].slope)-(newslope));
		float closestypoint = newslope*closestxpoint + newintercept;
		if (sqrt((X-closestxpoint)*(X-closestxpoint)+(Y-closestypoint)*(Y-closestypoint)) < 7)
		{
		if (((closestxpoint > wall[i].shortx && closestxpoint < wall[i].longx) || (closestypoint > wall[i].shorty && closestypoint < wall[i].longy)))// && i != currentwallhit)
		{
		speed*=0.9f;
		float XNormal;
		float YNormal;
		float tempx = XVector;
		float tempy = YVector;
		YNormal = 1.0f;
		XNormal = -wall[i].slope;
		Normalize(&XNormal, &YNormal);
		XVector = tempx - 2*((tempx*XNormal)+(tempy*YNormal))*XNormal;
		YVector = tempy - 2*((tempx*XNormal)+(tempy*YNormal))*YNormal;
		Normalize(&XVector, &YVector);
		Y+=YVector;
		X+=XVector;
		//currentwallhit = i;
		//currentballhit = -1;
		for (int f = 0; f < 10; f++)
		{
			if ((Y < wall[i].slope*X + wall[i].yintercept && wall[i].slope > 0) || (Y < wall[i].slope*X + wall[i].yintercept && wall[i].slope < 0))
			{
			//x' = cos(theta)*x - sin(theta)*y
			//y' = sin(theta)*x + cos(theta)*y
			float angle = float(rand()%360)-180;
			float xtemp = (cosf(angle/57.29f)*-XNormal - sinf(angle/57.29f)*-YNormal);
			float ytemp = (sinf(angle/57.29f)*-XNormal + cosf(angle/57.29f)*-YNormal);
			Normalize(&xtemp, &ytemp);
			particle[f].XVector = (xtemp-XNormal)/2;
			particle[f].YVector = (ytemp-YNormal)/2;
			Normalize(&particle[f].XVector, &particle[f].YVector);
			}
			else if ((Y > wall[i].slope*X + wall[i].yintercept && wall[i].slope < 0) || (Y > wall[i].slope*X + wall[i].yintercept && wall[i].slope > 0))
			{
			float angle = float(rand()%360)-180;
			float xtemp = (cosf(angle/57.29f)*XNormal - sinf(angle/57.29f)*YNormal);
			float ytemp = (sinf(angle/57.29f)*XNormal + cosf(angle/57.29f)*YNormal);
			Normalize(&xtemp, &ytemp);
			particle[f].XVector = (xtemp+XNormal)/2;
			particle[f].YVector = (ytemp+YNormal)/2;
			Normalize(&particle[f].XVector, &particle[f].YVector);
			}
			particle[f].active = true;
			particle[f].life = 1.0f;
			particle[f].X = X;
			particle[f].Y = Y;
		}
		Alreadyhit = true;
		break;
		}
		}
	//}
	}
	}

	if (Alreadyhit == false)
	{
	//collisions with corners of walls
	for (int i = 0; i < count; i++)
	{
		if (sqrt((X-wall[i].linex)*(X-wall[i].linex)+(Y-wall[i].liney)*(Y-wall[i].liney)) < 6)// && currentwallhit != i)
		{
			float tempxslope = wall[i].linex-X;
			float tempyslope = wall[i].liney-Y;
			Y-=YVector*(scalar*speed);
			X-=XVector*(scalar*speed);
			Normalize(&tempxslope, &tempyslope);
			float XNormal = X-wall[i].linex;
			float YNormal = Y-wall[i].liney;
			Normalize(&XNormal, &YNormal);
			float tempx = XVector;
			float tempy = YVector;
			XVector = tempx - 2*((tempx*XNormal)+(tempy*YNormal))*XNormal;
			YVector = tempy - 2*((tempx*XNormal)+(tempy*YNormal))*YNormal;
			float dotproduct = (XVector*XNormal)+(YVector*YNormal);
			speed*=dotproduct;
			//currentballhit = -1;
			//currentwallhit = i;
			Alreadyhit = true;
			break;
		}
	}
	}

	if (Alreadyhit == false)
	{
	//collision with corners of walls
	for (int i = 0; i < count; i++)
	{
		if (sqrt((X-wall[i].line2x)*(X-wall[i].line2x)+(Y-wall[i].line2y)*(Y-wall[i].line2y)) < 6)// && currentwallhit != i)
		{
			float tempxslope = wall[i].line2x-X;
			float tempyslope = wall[i].line2y-Y;
			Y-=YVector*(scalar*speed);
			X-=XVector*(scalar*speed);
			Normalize(&tempxslope, &tempyslope);
			float XNormal = X-wall[i].line2x;
			float YNormal = Y-wall[i].line2y;
			Normalize(&XNormal, &YNormal);
			float tempx = XVector;
			float tempy = YVector;
			XVector = tempx - 2*((tempx*XNormal)+(tempy*YNormal))*XNormal;
			YVector = tempy - 2*((tempx*XNormal)+(tempy*YNormal))*YNormal;
			float dotproduct = (XVector*XNormal)+(YVector*YNormal);
			speed*=dotproduct;
			//currentballhit = -1;
			//currentwallhit = i;
			Alreadyhit = true;
			break;
		}
	}
	}

	if (Alreadyhit == false)
	{
	for (int i = 0; i < ballcount; i++)
	{
	if (&ball[i].speed == &speed || ball[i].Alive == false)
		continue;
		if (sqrt((X-ball[i].X)*(X-ball[i].X)+(Y-ball[i].Y)*(Y-ball[i].Y)) < 12)// && currentballhit != i)
		{
			//FSOUND_SetVolume(FSOUND_ALL,0);
		//	FSOUND_SetMute(FSOUND_ALL,true);
			float tempxslope = ball[i].X-X;
			float tempyslope = ball[i].Y-Y;
			while (sqrt((X-ball[i].X)*(X-ball[i].X)+(Y-ball[i].Y)*(Y-ball[i].Y)) < 12)
			{
			Y-=YVector;
			X-=XVector;
			}
			Normalize(&tempxslope, &tempyslope);
			ball[i].XVector = tempxslope;
			ball[i].YVector = tempyslope;
			float dotproduct = (XVector*ball[i].XVector)+(YVector*ball[i].YVector);
			FSOUND_SetVolume(FSOUND_ALL,0);
			//FSOUND_SetSFXMasterVolume(255*dotproduct);
			FSOUND_PlaySound(0, sound1);

			//deflection
			if((XVector*-tempyslope+YVector*tempxslope) < (XVector*tempyslope+YVector*-tempxslope))
			{
				YVector=-tempxslope;
				XVector=tempyslope;
			}
			else
			{
				YVector=tempxslope;
				XVector=-tempyslope;
			}

			if (dotproduct > 0.95f)
				dotproduct = 0.95f;
			ball[i].speed = speed*dotproduct;
			speed*=(1.0f - dotproduct);
			ball[i].moving = true;
			//memset (ball[i].Bluractive,0,20);
			//currentballhit = i;
			//ball[i].currentballhit = ballnumber;
			//ball[i].currentwallhit = -1;
			//currentwallhit = -1;
			break;
		}
	}
	}
	if (Alreadyhit == true)
		return true;
	else
		return false;
}

//make sure all balls have stopped
bool ballclass::Moving(ballclass *ball, int ballcount)
{
	bool okay = true;
	for (int i = 0; i < ballcount; i++)
	{
		if (ball[i].moving == true && ball[i].Alive == true)
		{
			okay = false;
			break;
		}
	}
	if (okay == true)
		return true;
	else
		return false;
}