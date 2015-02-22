#include "terrain.h"
#include <windows.h>		
#include <gl\gl.h>			
#include <gl\glu.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

terrainclass::terrainclass()
{
	srand( time(NULL) );
	for (int z = 0; z < 50; z++) {
		for (int i = 0; i < 50; i++) {
			X[i][z] = i;
			Y[i][z] = z;
			Z[i][z] = (sin(i/2.0f)+cos(z/2.0f));
		}
	}

	/*
	int randx = 0, randy = 0;
	float height = 0.0f;
	for (int i = 0; i < 20; i++) {
		randx = rand()%50;
		randy = rand()%50;
		height = rand()%10-5;
	for (int y = 0; y < 50; y++) {
		for (int x = 0; x < 50; x++) {
			float dist = sqrt((y-randy)*(y-randy)+(x-randx)*(x-randx));
			if (dist < 20.0f)
				Z[x][y] += height*(1.0f-dist/20.0f);
		}
	}
	}
	*/
	//for (z = 0; z < 50; z++) {
	//	for (int i = 0; i < 50; i++) {
	//		NormalX[i][z] = 1;
	//		NormalY[i][z] = 1;
	//		NormalZ[i][z] = Z[i][z]-Z[i][z+1];
	//		Normalize(NormalX[i][z], NormalY[i][z], NormalZ[i][z]);
	//	}
	//}

	LowestPoint();

	glMatrixMode(GL_PROJECTION);						
	glTranslatef(-25.0f, -25.0f, -48.0f);
	glMatrixMode(GL_MODELVIEW);							

}

void terrainclass::Normalize(float &XVector, float &YVector, float &ZVector)
{
	float length = sqrtf((XVector * XVector)+(YVector * YVector)+(ZVector * ZVector));
	XVector/=length;
	YVector/=length;
	ZVector/=length;	
}

void terrainclass::LowestPoint()
{
	for (int y = 0; y < 50; y++) {
		for (int x = 0; x < 50; x++) {
			if (Z[x][y] < lowestpoint)
				lowestpoint = Z[x][y];
		}
	}
}

void terrainclass::DrawTerrain()
{
	for (int z = 0; z < 49; z++) {
	glBegin(GL_TRIANGLE_STRIP);
		for (int i = 0; i < 49; i++) {
			glColor3f(fabs(Z[i][z]-lowestpoint)/4.0f, fabs(Z[i][z]-lowestpoint)/4.0f, fabs(Z[i][z]-lowestpoint)/4.0f);
			glTexCoord2f(X[i][z]/50.0f*10.0f, Y[i][z]/50.0f*10.0f); glVertex3f(X[i][z], Y[i][z], Z[i][z]);

			glColor3f(fabs(Z[i][z+1]-lowestpoint)/4.0f, fabs(Z[i][z+1]-lowestpoint)/4.0f, fabs(Z[i][z+1]-lowestpoint)/4.0f);
			glTexCoord2f(X[i][z+1]/50.0f*10.0f, Y[i][z+1]/50.0f*10.0f); glVertex3f(X[i][z+1], Y[i][z+1], Z[i][z+1]);
		}
	glEnd();
	}
}