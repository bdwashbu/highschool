#include <windows.h>		
#include <gl\gl.h>			
#include <gl\glu.h>
#include <math.h>
#include <iostream.h>		
#include "lightmap.h"
#include "texture.h"

//#include "confetti.h"

lightmapclass::lightmapclass()
{
	Size = 8.0f;
}

float lightmapclass::X = 32.0f;
float lightmapclass::Z = 32.0f;
float lightmapclass::Y = 18.0f;
float lightmapclass::Angle = 0.0f;
bool lightmapclass::YRaise = false;
bool lightmapclass::Skipframe = false;

void lightmapclass::RenderLightMap(confetticlass *confetti)
{
	glViewport(0,0,85,64);
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -45.0f);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);

	glPointSize(3.1f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE,GL_ZERO);
	glDepthMask(false);

	glBegin(GL_POINTS);
	for (int x = -60; x < 60; x++)
	{
		for (int y = -40; y < 40; y++)
		{
			float distance = sqrtf((x-X)*(x-X)+(y-Z)*(y-Z));//sqrt((x-lightmap.X)*(x-lightmap.X) + (lightmap.Y*lightmap.Y) + (y-lightmap.Z)*(y-lightmap.Z));
			glColor4f(1.0f-distance/(-Y/3.0f),1.0f-distance/(-Y/3.0f),1.0f-distance/(-Y/3.0f),1.0f);
				glVertex3f(x*2,-1.0f, y*2);
		}
	}
	glEnd();

//	glEnable(GL_BLEND);
	glPointSize(1.0);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBegin(GL_POINTS);
	for (int i = 0; i < 200; i++)
	{
	if (confetti[i].Active == true)
	{
	float t = (((-Y/2) + 1.0f) / (confetti[i].Y - (-Y/2)));

	float Xr = X - t * (confetti[i].X- X);
	float Yr = (-Y/2) - t * ((-Y/2) - confetti[i].Y);
	float Zr = Z - t * (confetti[i].Z-Z);
  
	if (confetti[i].Rotating)
	{
		glColor4f(0.0f, 0.0f, 0.0f, 8/confetti[i].Y);
		glVertex3f(Xr, -1.0, Zr);
	}
	}
	}
	glEnd();

	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 10, 0, 64, 64, 0);

	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0,0,640,480);
	glDepthMask(true);
	glPopMatrix();
}

void lightmapclass::DrawLightMapDot(float Rotation, float Angle)
{

}

void lightmapclass::MoveLightMap(float scalar)
{
	X = (cosf(Angle/57.29f) - sinf(Angle/57.29f)*15);
	Z = (sinf(Angle/57.29f) + cosf(Angle/57.29f)*15);

	if (Angle-(scalar*20) > 0)
		Angle-=(scalar*20);
	else
		Angle = 360;

	if (YRaise == false)
	{
		if (Y < 25.0f)
			Y+=(0.5f*scalar);
		else
			YRaise = true;
	}	
	else
	{
		if (Y > 15.0f)
			Y-=(0.5f*scalar);
		else
			YRaise = false;
	}
}

float lightmapclass::World2Texel(float coord)
{
	return ((0.5*TexelWidth)+(coord/Width*(0.5*TexelWidth)));
}

float lightmapclass::Texel2World(float coord)
{
	return BottomLeftPoint+(Width*2/TexelWidth)*coord;
}


void lightmapclass::WriteLightMaps(int texID, unsigned char *imageData, confetticlass *confetti, float scalar)
{
		static float keeptime = 0.0f;
		keeptime+=scalar;
		if (keeptime > 0.025f)
		{
		int h = TexelWidth; int w = TexelWidth;
		unsigned char *colorpointer;
		for (int y = 0; y < h; y++)
		{
		colorpointer = imageData + y * h * 4;
		for (int x = 0; x < w; x++, colorpointer+=4)
		{
			double distance = sqrt((Texel2World(x)-X)*(Texel2World(x)-X)+(Texel2World(y)+Z)*(Texel2World(y)+Z)+(Height-Y)*(Height-Y));
			float finalcolor = 255 - distance*(6);
			if (finalcolor < 0.0f)
				finalcolor = 0.0f;

				colorpointer[0] = finalcolor;
				colorpointer[1] = finalcolor;
				colorpointer[2] = finalcolor;
				if (finalcolor < 50)
					continue;


			//calculate confetti shadows
			for (int i = 0; i < 200; i++)
			{
			if (confetti[i].Active == true && confetti[i].Rotating == true && Size != 6)
			{
			float t = ((Y - Height) / (-confetti[i].Y - Y));
			float Xr = -X - t * (confetti[i].X + X);
		//	float Yr = -Y - t * (confetti[i].Y + Y);
			float Zr = Z - t * (-confetti[i].Z - Z);

			Zr = World2Texel(Zr);
			Xr = World2Texel(Xr);

			if (Xr - 0.5f < x && Xr + 0.5f > x && Zr + 0.5f > y && Zr - 0.5f < y && confetti[i].Y > Height)
			{
				colorpointer[0] = 50;
				colorpointer[1] = 50;
				colorpointer[2] = 50;
				finalcolor = 50.0;
				break;
			}
			}
			}
			if (finalcolor < 50)
				continue;


			//calculate plane shadow volume
			float t = ((Y - 5.0f) / (-5.0f - Y));
			float Xr = X - t * (Texel2World(x) - X);
			float Yr = Y - t * (Height - Y);
			float Zr = -Z - t * (Texel2World(y) + Z);

		//	Zr = World2Texel(Zr);
			//Xr = World2Texel(Xr);

			if (Xr > -5.0 && Xr < 5.0 && Zr > -5.0 && Zr < 5.0 && Height < 5.0f)
			{
				if (colorpointer[0] > 50.0f)
				{
				colorpointer[0] = 50;
				colorpointer[1] = 50;
				colorpointer[2] = 50;
				}
			}

		
		}
		}

		glBindTexture(GL_TEXTURE_2D, texID);// Bind Our Texture
		glTexSubImage2D (GL_TEXTURE_2D, 0, 0, 0, TexelWidth, TexelWidth, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
		keeptime = 0.0f;
		}
}

