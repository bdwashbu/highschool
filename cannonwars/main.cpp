


#include <windows.h>		
#include <stdio.h>			
#include <gl\gl.h>			
#include <gl\glu.h>			
#include <math.h>
#include <stdarg.h>
#include <time.h>
#include "main.h"	

HDC			hDC=NULL;		
HGLRC		hRC=NULL;		
HWND		hWnd=NULL;		
HINSTANCE	hInstance;		

bool	keys[256];			
bool	active=TRUE;		
bool	fullscreen=TRUE;
	
TextureImage textures[7]; GLuint loop, base;
int terrain, b, intwind, intgravity, hurtamount, menu2 = 4, p1cannonheight[20], p1cannonplacment[20], p2cannonheight[20], p2cannonplacment[20], savenumbers[4], player1health = 100, player2health = 100, launchtimetemp = 20, startgamechoice = 1, menuon = true, cannon, cannonbase, selection = 1, angle, menuselection2 = 1, temp = 45, temp2 = 0, fps = 0, collidewhere, color1  = 2, color2 = 1, temptemp, temptemptemp,
placebuildingx, placebuildingy, realxplacment, realyplacment, realxplacment2, realyplacment2;
float fa[SIZE], waterlevel, rotationangle, particletime2, turmoiltime = 0.0, turmoiltemp, resize, gravity = 0.05f, ballslope[4], bouncewhere[2], wind, particletime = 0.0, menucannon, gamecannon, rightbar = 480.0f, x = 200.0f, y = 480.0f, texrightbar = 1.0f, placment[4], starttime, timetaken, initialspeed = 0.008, fpstime,
yspeed, xspeed, scalar = 0.0, launchtime, launchsave; 
bool baractive, about, billy, turmoil, player1hurt, player2hurt, firststart = true, goingintomain, resize2, launchready, waiting, startgame=false, bargoingright, bargoingleft, options, player1 = true, player2 = false, bob4;
unsigned long endtime;
int lx=0, tx=640, ly=0, ty=480;
float printplace[2] = {400.0f, 1000.0f};
float colors[9][3] =
{
	{0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.98f, 0.625f, 0.12f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 1.0f}, {0.60f, 0.40f, 0.12f},
	{0.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 0.0f}, {0.25f, 0.60f, 0.80f}
};
int hole[SIZE];
float hole1[SIZE];
float hole2[SIZE];
float slope[SIZE];
float water[SIZE];

float tlighting[SIZE][3];

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	

GLvoid BuildFont(GLvoid)// Build Our Font Display List
{
	base=glGenLists(95);// Creating 256 Display Lists
	glBindTexture(GL_TEXTURE_2D, textures[4].texID);// Select Our Font Texture
	for (loop=0; loop<95; loop++)// Loop Through All 256 Lists
	{
		float cx = float(loop%16)/16.0f;// X Position Of Current Character
		float cy = float(loop/16)/8.0f;// Y Position Of Current Character 
		glNewList(base+loop,GL_COMPILE);// Start Building A List
		glBegin(GL_QUADS);// Use A Quad For Each Character
			glTexCoord2f(cx,1.0f-cy-0.120f);// Texture Coord (Bottom Left)
			glVertex2d(0,0);// Vertex Coord (Bottom Left)
			glTexCoord2f(cx+0.0625f,1.0f-cy-0.120f);// Texture Coord (Bottom Right)
			glVertex2i(16,0);// Vertex Coord (Bottom Right)
			glTexCoord2f(cx+0.0625f,0.99f-cy);// Texture Coord (Top Right)
			glVertex2i(16,13);// Vertex Coord (Top Right)
			glTexCoord2f(cx,0.99f-cy);// Texture Coord (Top Left)
			glVertex2i(0,13);// Vertex Coord (Top Left)
		glEnd();// Done Building Our Quad (Character)
	glTranslated(16,0,0);// Move To The Right Of The Character
	glEndList();// Done Building The Display List
	}// Loop Until All 256 Are Built
}

GLvoid KillFont(GLvoid)
{
	glDeleteLists(base,256);
}

GLvoid readstr(FILE *f,char *string)
{
	do
	{
		fgets(string, 255, f);
	} while ((string[0] == '/') || (string[0] == '\n'));
	return;
}

GLvoid readfile()
{
	int loop = 0;
	int readuntil0, readuntil1, readuntil2, readuntil3, holdsave[4]; 
	float a;
	int j, k, l, m;
	char oneline[255];
	FILE *filein, *fileout;
	filein = fopen("data/data.txt", "rt");

	readstr(filein,oneline);
	sscanf(oneline, "readuntil %d\n", &readuntil0);
	holdsave[0] = readuntil0;
	while (loop < readuntil0)
	{
		readstr(filein,oneline);
		sscanf(oneline, "%f", &a);
		p1cannonheight[loop] = a;
		loop++;
	}
	loop = 0;
	readstr(filein,oneline);
	sscanf(oneline, "readuntil1 %d\n", &readuntil1);
	holdsave[1] = readuntil1;
	while (loop < readuntil1)
	{
		readstr(filein,oneline);
		sscanf(oneline, "%f", &a);
		p1cannonplacment[loop] = a;
		loop++;
	}
	loop = 0;
	readstr(filein,oneline);
	sscanf(oneline, "readuntil %d\n", &readuntil2);
	holdsave[2] = readuntil2;
	while (loop < readuntil2)
	{
		readstr(filein,oneline);
		sscanf(oneline, "%f", &a);
		p2cannonheight[loop] = a;
		loop++;
	}
	loop = 0;
	readstr(filein,oneline);
	sscanf(oneline, "readuntil %d\n", &readuntil3);
	holdsave[3] = readuntil3;
	while (loop < readuntil3)
	{
		readstr(filein,oneline);
		sscanf(oneline, "%f", &a);
		p2cannonplacment[loop] = a;
		loop++;
	}
	fclose(filein);


	fileout = fopen("data/data2.txt", "rt");
	readstr(fileout,oneline);
	sscanf(oneline, "%d %d %d %d", &j, &k, &l, &m);
	if (j > (holdsave[0]) - 1)
		j = 0;
	if (k > (holdsave[1]) - 1)
		k = 0;
	if (l > (holdsave[2]) - 1)
		l = 0;
	if (m > (holdsave[3]) - 1)
		m = 0;
	extern savenumbers[4] = {j, k, l, m};
	fclose(fileout);
	fileout = fopen("data/data2.txt", "w");
	j = savenumbers[0] + 1; k = savenumbers[1] + 1; l = savenumbers[2] + 1; m = savenumbers[3] + 1;
	fprintf(fileout, "%d %d %d %d", j, k, l, m);
	fclose(fileout);
}


GLvoid glPrint(float size, GLint x, GLint y, int set, const char *string, ...)// Where The Printing Happens
{
	char text[256];// Holds Our String
	va_list ap;// Pointer To List Of Arguments 
	if (string == NULL)// If There's No Text
		return;
	va_start(ap, string);// Parses The String For Variables
		vsprintf(text, string, ap);// And Converts Symbols To Actual Numbers
	va_end(ap);// Results Are Stored In Text 

	glBindTexture(GL_TEXTURE_2D, textures[4].texID);
	glPushMatrix();
	glLoadIdentity();
	glTranslated(x,y,0);// Position The Text (0,0 - Bottom Left)
	glListBase(base-32);// Choose The Font Set (0 or 1) 
	glScalef((1.0f * size), (1.8f * size), 1.0f);// Enlarge Font Width And Height
	glCallLists(strlen(text),GL_UNSIGNED_BYTE, text);
	glPopMatrix();
}

GLvoid genslope()
{
	for (int i = 0; i < SIZE; i++)
	{
		slope[i] = (fa[i+1] - fa[i]) / 4;
	}
}

void CaptureScreen()
{
BITMAPFILEHEADER bf;
BITMAPINFOHEADER bi;
unsigned char *image	= (unsigned char*)malloc(sizeof(unsigned char)*640*480*3);
FILE *file				= fopen("screenshot.bmp", "wb");
if( image!=NULL )
{
if( file!=NULL )
{
glReadPixels( 0, 0, 640, 480, GL_BGR_EXT, GL_UNSIGNED_BYTE, image );
memset( &bf, 0, sizeof( bf ) );
memset( &bi, 0, sizeof( bi ) );
bf.bfType			= 'MB';
bf.bfSize			= sizeof(bf)+sizeof(bi)+640*480*3;
bf.bfOffBits		= sizeof(bf)+sizeof(bi);
bi.biSize			= sizeof(bi);
bi.biWidth			= 640;
bi.biHeight			= 480;
bi.biPlanes			= 1;
bi.biBitCount		= 24;
bi.biSizeImage		= 640*480*3;
fwrite( &bf, sizeof(bf), 1, file );
fwrite( &bi, sizeof(bi), 1, file );
fwrite( image, sizeof(unsigned char), 480*640*3, file );
fclose( file );
}
free( image );
}
}

float findterrainheight(bool player1)
{
	float temp = 0.0;
	int temp1 = -2;
	if (player1)
	{
		for (temp1 = -2; temp1 < 2; temp1++)
		{
		if (fa[p1cannonplacment[savenumbers[1]]+temp1]+12 > temp)
			temp = fa[p1cannonplacment[savenumbers[1]]+temp1]+12;
		}
	}
	else
	{
		for (temp1 = -2; temp1 < 2; temp1++)
		{
		if (fa[p2cannonplacment[savenumbers[3]]+temp1]+12 > temp)
			temp = fa[p2cannonplacment[savenumbers[3]]+temp1]+12;
		}
	}
return temp;
}


GLvoid Constructbuilding(int direction, int player)
{
	glBindTexture(GL_TEXTURE_2D, textures[0].texID);
	// front face
	if (player == 1)
	{
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0f, 0.0f); glVertex2i(realxplacment - 5, realyplacment - 11);
	glTexCoord2f(1.0f, 0.0f); glVertex2i(realxplacment + 5, realyplacment - 11);
	glTexCoord2f(1.0f, 1.0f); glVertex2i(realxplacment + 5, realyplacment - 1);
	glTexCoord2f(0.0f, 1.0f); glVertex2i(realxplacment - 5, realyplacment - 1);
	glEnd();
	glPushMatrix();
	glTranslatef(realxplacment, realyplacment, 0.0f);
	glRotatef((GLfloat) cannonball[0].cannonup, 0.0f, 0.0f, (direction * 1.0f));
	glTranslatef((direction * 4.0f), 0.0f, 0.0f);
	glCallList(cannon);
	glPopMatrix();

	}
	else
	{
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0f, 0.0f); glVertex2i(realxplacment2 - 5, realyplacment2 - 11);
	glTexCoord2f(1.0f, 0.0f); glVertex2i(realxplacment2 + 5, realyplacment2 - 11);
	glTexCoord2f(1.0f, 1.0f); glVertex2i(realxplacment2 + 5, realyplacment2 - 1);
	glTexCoord2f(0.0f, 1.0f); glVertex2i(realxplacment2 - 5, realyplacment2 - 1);
	glEnd();
	glPushMatrix();
	glTranslatef(realxplacment2, realyplacment2, 0.0f);
	glRotatef((GLfloat) cannonball[1].cannonup, 0.0f, 0.0f, (direction * 1.0f));
	glTranslatef((direction * 4.0f), 0.0f, 0.0f);
	glCallList(cannon);
	glPopMatrix();
	}
}

int findhigh()
{
	int current = 0;
	for (int i = 0;  i <MAX_PARTICLES; i++)
	{
		if (particle[i].active)
			if (i > current)
				current = i;
	}
	return current;
}


GLvoid calculatefps()
{
	static int temp;
	fpstime = (scalar + fpstime);
	temp++;
	if (fpstime > 1.0)
	{
		fpstime = 0.0;
		fps = (temp);
		temp = 0;
	}
	glPrint(0.75f, 40,40,0,"fps:%3i", fps);
}
		
GLvoid Addparticle()
{
int particlenum = 0, type = 0;
	while (type < 1)
	{
		if (particle[particlenum].active == false)
			type++;
		else
		{
		if (particlenum < MAX_PARTICLES)
			particlenum++;
		else
			break;
		}
	}
			if (cannonball[0].y > waterlevel && cannonball[0].active)
			{
			particle[particlenum].x = cannonball[0].x;
			particle[particlenum].y = cannonball[0].y;
			particle[particlenum].startxpos = cannonball[0].x;
			particle[particlenum].startypos = cannonball[0].y;

			particle[particlenum].life = 1.0f;
			particle[particlenum].fade=0.015f;
			particle[particlenum].speed = 0.002f;
			particle[particlenum].type = 3;
			particle[particlenum].active = true;
			particle[particlenum].size = float(rand()%75)/100.0f+1.5f;
			if (player1)
			{
			particle[particlenum].red = colors[color1][0] + float(rand()%750)/576.9f-0.65f;
			particle[particlenum].green = colors[color1][1] + float(rand()%750)/576.9f-0.65f;
			particle[particlenum].blue = colors[color1][2] + float(rand()%750)/576.9f-0.65f;
			}
			else
			{
			particle[particlenum].red = colors[color2][0] + float(rand()%750)/576.9f-0.65f;
			particle[particlenum].green = colors[color2][1] + float(rand()%750)/576.9f-0.65f;
			particle[particlenum].blue = colors[color2][2] + float(rand()%750)/576.9f-0.65f;
			}
			if (cannonball[0].x > ballslope[2])
			{
				particle[particlenum].player1 = true;
				particle[particlenum].player2 = false;
			}
			else
			{
				particle[particlenum].player2 = true;
				particle[particlenum].player1 = false;
			}

			
			if (ballslope[0] < 4.0f && ballslope[0] > -4.0f)
				particle[particlenum].yi=ballslope[0] + float(rand()%400)/100.0f-2.0f;
			else if (ballslope[0] > 4.0f || ballslope[0] < -4.0f)
				particle[particlenum].yi=ballslope[0] + float(rand()%800)/100.0f-4.0f;
			}
			else if (cannonball[0].y < waterlevel && cannonball[0].active)
			{
			particle[particlenum].x = cannonball[0].x;
			particle[particlenum].y = cannonball[0].y;
			particle[particlenum].active = true;
			particle[particlenum].player2 = true;
			particle[particlenum].player1 = false;
			particle[particlenum].time = 0.0f;
			particle[particlenum].fade=0.015f;
			particle[particlenum].type = 4;
			particle[particlenum].speed = 0.002f;
			particle[particlenum].red = 1.0f;
			particle[particlenum].green = 1.0f;
			particle[particlenum].blue = 1.0f;
			particle[particlenum].life = float(rand()%50)/100.0f+0.5f;
			particle[particlenum].startxpos = cannonball[0].x;
			particle[particlenum].startypos = cannonball[0].y;
			particle[particlenum].xi=45.0f + float(rand()%200)/10.0f + 10;
			particle[particlenum].yi=90.0 - particle[particlenum].xi;
			particle[particlenum].size = float(rand()%100)/100.0f+1.5f;
			}
}


GLvoid Collisionresponse(float radius)
{
	for (int i = 0; i < SIZE; i++)
	{
		if ((collidewhere * 4) - (i * 4) < radius && (collidewhere * 4) - (i * 4) > -radius)
			if ((collidewhere * 4) - (i * 4) >= 0 && fa[collidewhere] - sqrt((radius * radius) - ((i * 4 - (collidewhere * 4)) * (i * 4 - (collidewhere * 4)))) <= fa[i])
			{	
				hole[temp2] = i;
				hole2[temp2] = 5.5f;
				hole1[temp2] = fa[collidewhere] - sqrt((radius * radius) - ((i * 4 - (collidewhere * 4)) * (i * 4 - (collidewhere * 4))));
				temp2++;
			}
			else if ((collidewhere * 4) - (i * 4) <= 0 && fa[collidewhere] - sqrt((radius * radius) - ((i * 4 - (collidewhere * 4)) * (i * 4 - (collidewhere * 4)))) <= fa[i])
			{	
				hole[temp2] = i;
				hole2[temp2] = 5.5f;
				hole1[temp2] = fa[collidewhere] - sqrt((radius * radius) - ((i * 4 - (collidewhere * 4)) * (i * 4 - (collidewhere * 4))));
				temp2++;
			}
		if (temp2 > SIZE - 5)
			temp2 = 0;
	}
}

GLvoid Resizescreen(int xleft, int xright, int ydown, int yup, int speed)
{
	if (lx - (lx - xleft)/speed > 0 && tx - (tx - xright)/speed < 640)
	{
		lx = lx - (lx - xleft)/speed;
		tx = tx - (tx - xright)/speed;
	}
	//if (lx > 0 && tx < 640)
	//{
	//ly = ;
	//ty = ;
	ty = ((ty - (ty - yup)/speed)+ tx * 0.75) / 2;
	ly = ((ly + (ly - ydown)/speed) + lx * 0.75) /2;
	//}
	
	//if (lx > -15)
		//lx = lx + (lx - xleft)/125;
	//else if (tx > 655)
	//	tx = tx + (tx - xright)/125;	

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(lx, tx, ly, ty);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
	
GLvoid Editterrain()
{
	for(int i = 0; i < 170; i++)
	{
		if (i > p1cannonplacment[savenumbers[1]] - 4 && i < p1cannonplacment[savenumbers[1]] + 4 && fa[i] > realyplacment - 12)
			fa[i] = realyplacment - 12;
	}
		for(i = 0; i < 170; i++)
	{
		if (i > p2cannonplacment[savenumbers[3]] - 4 && i < p2cannonplacment[savenumbers[3]] + 4 && fa[i] > realyplacment2 - 12)
			fa[i] = realyplacment2 - 12;
	}

}

GLvoid Addrain()
{
int i=0, i2 =0, starttemp;
		temptemp = 0;
		i2 = 0;
		while (i < 1)
		{
			if (particle[i2].active == false)
				i++;
			else
			{
			if (i2 < MAX_PARTICLES)
				i2++;
			else
				break;
			}
		}
			starttemp=float(rand()%200)+200.0f;
			particle[i2].player1 = true;
			particle[i2].player2 = false;
			particle[i2].startxpos = starttemp;
			particle[i2].startypos = 550;
			particle[i2].x = starttemp;
			particle[i2].y = 550;
			particle[i2].xi=float((rand()%180)-90.0f);
			particle[i2].yi=90+float((rand()%180)-90.0f);
			particle[i2].size = 3.0f;
			particle[i2].active = true;
			particle[i2].life = float((rand()%100)/100.0f);
			particle[i2].type = 5;
			particle[i2].fade=0.0;
			particle[i2].red = 0.0f;
			particle[i2].blue = 1.0f;
			particle[i2].green = 0.0f;
			particle[i2].speed = 0.002f;
			particle[i2].time = 0.0f;
}

GLvoid Newround()
{
			player1hurt = false; menuon = false;
			player2hurt = false;
			cannonball[0].yi = 0; cannonball[0].active = false; cannonball[0].xi = 90; cannonball[0].x = realxplacment;cannonball[1].yi = 0; cannonball[1].xi = 90;
			cannonball[0].y = realyplacment;cannonball[0].time = 1.0f;cannonball[0].cannonup = 40.0;player1health = 100;player2health=100;
				
			for (loop=0;loop<MAX_PARTICLES;loop++)// Initializes All The particles
			{
				particle[loop].life = 0.0f;
				particle[loop].active = false;
				particle[loop].fade = float(rand()%5)/1000.0f+0.004f;
				particle[loop].x = realxplacment;
				particle[loop].y = realyplacment;
				particle[loop].player1 = true;
				particle[loop].player2 = false;
				particle[loop].gravity = 0.05f;
				particle[loop].wind = 0.0f;
				particle[loop].size = 4.0f;
			}

			for (int i = 0; i < SIZE; i++)
			{
				tlighting[i][0] = 1.0;
				tlighting[i][1] = 1.0;
				tlighting[i][2] = 1.0;
			}
			color1 = float(rand()%8);
			do
			{
			color2 = float(rand()%8);
			} while (color2 == color1);

			readfile();
			do
			{
			srand( time(NULL) );
			fill1DFractArray(SIZE);
			terrainsmooth(4);
			waterlevel = findlowheight() - 10;
			realxplacment = p1cannonplacment[savenumbers[1]] * 4;
			realyplacment = findterrainheight(true);
			realxplacment2 = p2cannonplacment[savenumbers[3]] * 4;
			realyplacment2 = findterrainheight(false);
			} 	while (Averageheight() > 320);
			//Editterrain();
			genslope();
			firststart = false;
			startgame = true;
}

GLvoid Addlighting()
{
	float xtemp, ytemp, hypot, xtempp[MAX_PARTICLES], ytempp[MAX_PARTICLES], hypotp[MAX_PARTICLES];
	for (int i = 0; i<SIZE; i++)
	{
	if (cannonball[0].active == true)
	{
		xtemp = cannonball[0].x - i * 4;
		ytemp = cannonball[0].y - fa[i];
		hypot = sqrt((xtemp * xtemp) + (ytemp * ytemp));
		xtemp = hypot / 100;
		if (xtemp <= 1.0 && cannonball[0].x != fa[i])
		{
			if (player1)
			{
			tlighting[i][0] = (colors[color1][0] - xtemp + tlighting[i][0]) / 2;
			tlighting[i][1] = (colors[color1][1] - xtemp + tlighting[i][1]) / 2;
			tlighting[i][2] = (colors[color1][2] - xtemp + tlighting[i][2]) / 2;
			}
			else
			{
			tlighting[i][0] = (colors[color2][0] - xtemp + tlighting[i][0]) / 2;
			tlighting[i][1] = (colors[color2][1] - xtemp + tlighting[i][1]) / 2;
			tlighting[i][2] = (colors[color2][2] - xtemp + tlighting[i][2]) / 2;
			}
		}
	}
		for (int f = 0; f<MAX_PARTICLES; f++)
		{
		if (particle[f].active && (particle[f].type == 1 || particle[f].type == 2 || particle[f].type == 5))
		{
		xtempp[f] = particle[f].x - i * 4;
		ytempp[f] = particle[f].y - fa[i];
		hypotp[f] = sqrt((xtempp[f] * xtempp[f]) + (ytempp[f] * ytempp[f]));
		xtempp[f] = hypotp[f] / 20;
		if (xtempp[f] <= 1.0)
		{
			tlighting[i][0] = (particle[f].red/particle[f].life*0.5f * xtempp[f] + tlighting[i][0]) / 2;
			tlighting[i][1] = (particle[f].green/particle[f].life*0.5f * xtempp[f] + tlighting[i][1]) / 2;
			tlighting[i][2] = (particle[f].blue/particle[f].life*0.5f * xtempp[f] + tlighting[i][2]) / 2;
		}


		}
		}
	for (f = 0; f < 3; f++)
	{
		if (tlighting[i][f] < 1.00)
			tlighting[i][f] = tlighting[i][f] + ((1.0 - tlighting[i][f]) * 2.5 * scalar);
		else
			tlighting[i][f] = 1.00;
	}
	}
	
}

GLvoid watersplash(float xposition, int loop, float speed)
{
int temp = 0, temp2 = 0;
	while (temp < loop)
	{
		if (particle[temp2].active == false)
		{
			temp++;
			particle[temp2].active = true;
			particle[temp2].player1 = false;
			particle[temp2].player2 = true;
			particle[temp2].startxpos = xposition;
			particle[temp2].startypos = waterlevel+5;
			particle[temp2].x = xposition;
			particle[temp2].y = waterlevel+5;
			particle[temp2].xi=0+float((rand()%80)-40.0f);
			particle[temp2].yi=90+float((rand()%80)-40.0f);
			particle[temp2].red = 0.0f;
			particle[temp2].green = 0.0f;
			particle[temp2].blue = 1.0f;
			particle[temp2].life = float(rand()%50)/100.0f+0.5f;
			particle[temp2].fade=float(rand()%5)/650.0f;
			particle[temp2].speed = speed/2000;
			particle[temp2].time = 0.0f;
			particle[temp2].type = 6;
			particle[temp2].size = float(rand()%350)/100.0f+0.5f;
		}
		if (temp2 < MAX_PARTICLES)
			temp2++;
		else
			break;
	}
}

GLvoid Earthquake (int size)
{
hole1[0] = 240;
hole[0] = 0;

for(int i = 1; i < size; i++)
{
hole[i] = i;
hole1[i] = float((rand()%40)-20.0f);
hole2[i] = float((rand()%35)/10+1.0f);
hole1[i] = hole1[i] + hole1[i-1];
while (hole1[i] < 0.0)
{
hole1[i] = float((rand()%40)-20.0);
hole1[i] = hole1[i] + hole1[i-1];
}
}
}

GLvoid Menu(bool close)
{
	static float x = 0;
	if (close == false)
	{
	if (x < 170)
		x = x + (scalar*180);
	}
	else
	{
	if (x > 0)
		x = x - (scalar*180);
	else
		x = -1;
	}
	glColor3f(0.0f, 0.0f, 0.0f);
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_POLYGON);
		glVertex3f(x, 480, 1.0f);
		glVertex3f(0, 480, 1.0f); 
		glVertex3f(0, 0, 1.0f);
		glVertex3f(x,0, 1.0f);
	glEnd();
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_LINES);
		glVertex3i(x, 0, 1);
		glVertex3i(x, 480, 1);
	glEnd();
	glEnable(GL_TEXTURE_2D);
	if (x > 0)
	{
	menuon = true;
	glPrint(0.75, x-165,410,0,"Max Particles");
	if (menu2 == 1)
		glColor3f(1.0f, 0.0f, 0.0f);
	glPrint(0.9, x-100,380,0,"%2i", MAX_PARTICLES);
	glColor3f(1.0f, 1.0f, 1.0f);
	glPrint(0.75, x-130,330,0,"Gravity");
	if (menu2 == 2)
		glColor3f(1.0f, 0.0f, 0.0f);
	glPrint(0.75, x-110,300,0,"%2i", intgravity);
	glColor3f(1.0f, 1.0f, 1.0f);
	if (menu2 == 4)
		glColor3f(1.0f, 0.0f, 0.0f);
	glPrint(0.75, x-110,100,0,"Smooth");
	//glPrint(1.0, 300,410,0,"Angle:%2i", findhigh());
	glColor3f(1.0f, 1.0f, 1.0f);
	}
	else
		menuon = false;
}

GLvoid Explosion()
{
int loop = 0, type = 0;
	while (type < 20)
	{
		if (particle[loop].active == false)
		{
			type++;
			particle[loop].player1 = false;
			particle[loop].player2 = true;
			particle[loop].startxpos = collidewhere*4;
			particle[loop].startypos = fa[collidewhere]+2;
			particle[loop].x = collidewhere*4;
			particle[loop].y = fa[collidewhere]+2;
			particle[loop].xi=float((rand()%180)-90.0f);
			particle[loop].yi=90+float((rand()%180)-90.0f);
			particle[loop].size = float((rand()%2)+2);
			particle[loop].active = true;
			particle[loop].life = 1.0f;
			particle[loop].type = 2;
			particle[loop].fade=float((rand()%3)+3.5)/550.0f;
			particle[loop].red = float((rand()%100)/100.0f);
			particle[loop].blue = float((rand()%100)/100.0f);
			particle[loop].green = float((rand()%100)/100.0f);
			particle[loop].speed = 0.0035f;
			particle[loop].time = 0.0f;
		}
		else
		{
		if (loop < MAX_PARTICLES)
			loop++;
		else
			break;
		}
	}
}



GLvoid Addhurt()
{
		float xtemp;

		xtemp = sqrt((realxplacment - collidewhere*4)*(realxplacment - collidewhere*4) - (realyplacment - fa[collidewhere])*(realyplacment - fa[collidewhere]));
		if (xtemp < 60.0f && xtemp > 0.0f)
		{
			player1health = player1health - (60 - xtemp/2);
			player1hurt = true;
			hurtamount = (60 - xtemp);
		}

		xtemp = sqrt((realxplacment2 - collidewhere*4)*(realxplacment2 - collidewhere*4) - (realyplacment2 - fa[collidewhere])*(realyplacment2 - fa[collidewhere]));
		
		if (xtemp < 60.0f && xtemp > 0.0f)
		{
			player2health = player2health - (60 - xtemp/2);
			player2hurt = true;
			hurtamount = (60 - xtemp);
		}
}

GLvoid floathurts()
{
	static float tempy = 5;
	if (player1hurt)
	{
		glEnable(GL_BLEND);
		glColor4f(1.0f, 1.0f, 1.0f, 8/tempy);
		glPrint(0.7f, realxplacment-10,realyplacment+tempy,0,"%2i", hurtamount);
		glDisable(GL_BLEND);
	}
	if (player2hurt)
	{
		glEnable(GL_BLEND);
		glColor4f(1.0f, 1.0f, 1.0f, 8/tempy);
		glPrint(0.7f, realxplacment2-10,realyplacment2+tempy,0,"%2i", hurtamount);
		glDisable(GL_BLEND);
	}
	tempy = tempy + (scalar*15);
	if (tempy > 35)
	{
		player1hurt = false;
		player2hurt = false;
		tempy = 5;
	}
}


bool Collision(float xposition, float yposition)
{
bool collide = false;
for(int i = 0; i < SIZE; i++)
{
	if ((yposition-2<= (4 * i * slope[i] * -1 + fa[i] + (slope[i] * xposition)) && xposition >= i*4 && xposition < i*4+4))
	{
		collide = true;
		extern collidewhere = i;
		break;
	}
}
return collide;
}

GLvoid Convertslope2cannon(int collidewhere, int particlenum)
{
			if (slope[collidewhere] >= 1.0f)
			{
				particle[particlenum].xi=(90-(45/slope[collidewhere]));
				particle[particlenum].yi=90.0f - particle[particlenum].xi;
				particle[particlenum].player1 = false;
				particle[particlenum].player2 = true;
			}
			else if (slope[collidewhere] < 1.0f && slope[collidewhere] >= 0.0f)
			{
				particle[particlenum].yi=(90-(45*slope[collidewhere]));
				particle[particlenum].xi=90.0f - particle[particlenum].yi;
				particle[particlenum].player1 = false;
				particle[particlenum].player2 = true;
			}
			if (slope[collidewhere] <= -1.0f)
			{
				particle[particlenum].xi=(90-(-45/slope[collidewhere]));
				particle[particlenum].yi=90.0f - particle[particlenum].xi;
				particle[particlenum].player1 = true;
				particle[particlenum].player2 = false;
			}
			else if (slope[collidewhere] > -1.0f && slope[collidewhere] <= 0.0f)
			{
				particle[particlenum].yi=(90-(-45*slope[collidewhere]));
				particle[particlenum].xi=90.0f - particle[particlenum].yi;
				particle[particlenum].player1 = true;
				particle[particlenum].player2 = false;
			}
}	

//DotProduct = (x1*x2 + y1*y2)
//Length = SquareRoot(x*x + y*y)

GLvoid particlebounce(int collidewhere, int particlenum)
{			
			bouncewhere[1]++;
			particle[particlenum].startxpos = bouncewhere[0];
			particle[particlenum].startypos = bouncewhere[1];
			particle[particlenum].x = bouncewhere[0];
			particle[particlenum].y = bouncewhere[1];

			Convertslope2cannon(collidewhere, particlenum);

			particle[particlenum].life = particle[particlenum].life * 0.98f;
			particle[particlenum].time = 0.0f;
			particle[particlenum].size = particle[particlenum].size - 0.3;
}

int findmaxheight()
{
	int height = 0;
	for (int i = 0; i < SIZE; i++)
	{
		if (fa[i] > height)
			height = fa[i];
	}
	return height;
}

float Howmuchwater()
{
	float temp = 1;
	for (int i = 0; i < SIZE; i++)
	{
		if (fa[i] > waterlevel)
			temp++;
	}
	temp = 0.1f/(temp/170);
	return temp;
}

int Averageheight()
{
	int temp = 0;
	for (int i = 0; i < SIZE; i++)
	{
		temp = temp + fa[i];
	}
	temp = temp / SIZE;
	return temp;
}

int findlowheight()
{
	int height = 640;
	for (int i = 0; i < SIZE; i++)
	{
		if (fa[i] < height)
			height = fa[i];
	}
	return height;
}

GLvoid drawwater()
{
	glDepthMask(false);
	glEnable(GL_BLEND);
	glBindTexture(GL_TEXTURE_2D, textures[6].texID);
	glColor4f(1.0f, 1.0f, 1.0f, 0.8f);

	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2f(10,waterlevel/64);				glVertex2f(640, waterlevel);
		glTexCoord2f(0,waterlevel/64);				glVertex2f(0, waterlevel);
		glTexCoord2i(10,0);							glVertex2i(640, 0);
		glTexCoord2i(0,0);							glVertex2i(0,0);
	glEnd();

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glColor3f(0.0f, 0.0f, 0.0f);
	for(int i=0;i<SIZE;i++)
	{
			glBegin(GL_TRIANGLE_STRIP);
				glVertex2f((i+1)*4, fa[i+1]-1);
				glVertex2f((i)*4, fa[i]-1);
				glVertex2f((i+1)*4, 0.0f);
				glVertex2f((i)*4, 0.0f);
			glEnd();
	}
	glDepthMask(true);
	glEnable(GL_TEXTURE_2D);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		
{
	if (height==0)										
	{
		height=1;										
	}

	glViewport(0,0,width,height);						

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 640, 0, 480);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int InitGL(GLvoid)										
{
	if ((!LoadTGA(&textures[0],"Data/Building.tga")) ||
		(!LoadTGA(&textures[1],"Data/cannonball.tga")) ||
		(!LoadTGA(&textures[2],"Data/powerbar.tga")) ||
		(!LoadTGA(&textures[3],"Data/particle.tga")) ||
		(!LoadTGA(&textures[4],"Data/Font.tga")) ||
		(!LoadTGA(&textures[5],"Data/bubble.tga")) ||
		(!LoadTGA(&textures[6],"Data/water.tga")))


	{
		return false;// If Loading Failed, Return False
	}
	gluOrtho2D(0, 640, 0, 480);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	cannon = glGenLists(1);
	glNewList(cannon, GL_COMPILE);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glLineWidth(3.5f);
	glBegin(GL_LINES);
		glVertex2i(-5, 0);
		glVertex2i( 5, 0);
	glEnd();
	glDisable(GL_BLEND);
	glLineWidth(2.0f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glEndList();
	
	glEnable(GL_TEXTURE_2D);							
	glShadeModel(GL_SMOOTH);							
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);				
	glClearDepth(1.0f);									
	glEnable(GL_DEPTH_TEST);
	glAlphaFunc(GL_GREATER,0.1f);
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);
	glLineWidth(1.2f);
	srand( time(NULL) );

	color1 = float(rand()%8);
	do
	{
	color2 = float(rand()%8);
	} while (color2 == color1);
	BuildFont();
		
	cannonball[0].yi = 0; cannonball[0].xi = 90; cannonball[0].x = realxplacment;cannonball[1].yi = 0; cannonball[1].xi = 90;
	cannonball[0].y = realyplacment;cannonball[0].time = 1.0f;cannonball[0].cannonup = 40.0;player1health = 100;player2health=100;

	for (loop=0;loop<MAX_PARTICLES;loop++)// Initializes All The particles
		{
			particle[loop].life = 1.0f;
			particle[loop].active = false;
			particle[loop].fade = float(rand()%5)/1000.0f+0.004f;
			particle[loop].x = realxplacment;
			particle[loop].y = realyplacment;
			particle[loop].player1 = true;
			particle[loop].gravity = 0.05f;
			particle[loop].wind = wind;
			particle[loop].size = 4.0f;
		}

	for (int i = 0; i < SIZE; i++)
	{
		tlighting[i][0] = 1.0;
		tlighting[i][1] = 1.0;
		tlighting[i][2] = 1.0;
	}
	return TRUE;										
}

int DrawGLScene()									
{
	starttime = timeGetTime();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	calculatefps();

	if (cannonball[0].active && startgame == true)
	{	
		//draws moving cannonball
		glBindTexture(GL_TEXTURE_2D, textures[1].texID);
		glBegin(GL_TRIANGLE_STRIP);
			glTexCoord2i(1,1); glVertex2f(cannonball[0].x + 2, cannonball[0].y + 2);
			glTexCoord2i(0,1); glVertex2f(cannonball[0].x - 2, cannonball[0].y + 2);
			glTexCoord2i(1,0); glVertex2f(cannonball[0].x + 2, cannonball[0].y - 2);
			glTexCoord2i(0,0); glVertex2f(cannonball[0].x - 2, cannonball[0].y - 2);
		glEnd();

		ballslope[2] = cannonball[0].x;
		ballslope[3] = cannonball[0].y;
		//moves cannonball
		if (player1 && menuon == false)
		{
			cannonball[0].x = realxplacment + ((cannonball[0].xo * ((cannonball[0].speed - (cannonball[0].speed*0.01f)))) * cannonball[0].time - (0.005f * wind * (cannonball[0].time * cannonball[0].time)));
			cannonball[0].y = realyplacment + (((cannonball[0].yo * ((cannonball[0].speed - (cannonball[0].speed*0.01f)))) * cannonball[0].time) - (0.005f * gravity * (cannonball[0].time * cannonball[0].time)));
		}
		else if (player2 && menuon == false)
		{
			cannonball[0].x = realxplacment2 - ((cannonball[1].xo * (cannonball[0].speed - (cannonball[0].speed*0.01f))) * cannonball[0].time - (0.005f * (wind*-1) * (cannonball[0].time * cannonball[0].time)));
			cannonball[0].y = realyplacment2 + (((cannonball[1].yo * (cannonball[0].speed - (cannonball[0].speed*0.01f))) * cannonball[0].time) - (0.005f * gravity * (cannonball[0].time * cannonball[0].time)));
		}
		if (cannonball[0].y > waterlevel && menuon == false)
			cannonball[0].time = (cannonball[0].time + (700 * scalar));
		else if (cannonball[0].y < waterlevel && menuon == false)
			cannonball[0].time = (cannonball[0].time + (200 * scalar));

		if (ballslope[3] > waterlevel && cannonball[0].y <= waterlevel)
			watersplash(cannonball[0].x, 10, 7);
		ballslope[1] = cannonball[0].y - ballslope[2];
		ballslope[0] = cannonball[0].x - ballslope[3];
		ballslope[0] = ballslope[1]/ballslope[0];
		if (menuon == false)
			particletime = particletime + scalar;
		if (particletime > 0.065f && player1health > 0 && player2health > 0 && Collision(cannonball[0].x, cannonball[0].y) == false)
		{
			Addparticle();
			particletime = 0.0;
		}
		//tests cannonball collision detection
		if ((Collision(cannonball[0].x, cannonball[0].y) == true || cannonball[0].x < -1 || cannonball[0].x > 641 || cannonball[0].y < 0) && waiting != true)
		{
		if (cannonball[0].x > 1 && cannonball[0].x < 640)
		{
			Explosion();
			Addhurt();
			Collisionresponse(40.0f);
		}
		wind = float(((rand()%10)-5.0)/1000.0f);
		waiting = true;
		cannonball[0].active = false;
		launchtime = 0.0;
		}

	}
	else if (startgame == true)
	{
			if (player1)
			{
			cannonball[0].xo = cannonball[0].xi;
			cannonball[0].yo = cannonball[0].yi;
			}
			else
			{
			cannonball[1].xo = cannonball[1].xi;
			cannonball[1].yo = cannonball[1].yi;
			}
	}

		if (waiting == true)
		{
			if (launchtime < 2 && startgame == true)
			{
				launchtime = launchtime + scalar;
				keys[VK_RETURN] = false;
				keys[VK_UP] = false;
				keys[VK_DOWN] = false;
			}
			else if (launchtime > 2 && startgame == true && menuon == false)
			{
			waiting = false;
			launchtime = 0.0;
			cannonball[0].time = 1.0f;
			if (player1 == true)
			{
			cannonball[0].x = realxplacment;
			cannonball[0].y = realyplacment;
			ballslope[3] = realyplacment;
			player1 = false;
			player2 = true;
			}
			else
			{
			cannonball[0].x = realxplacment2;
			cannonball[0].y = realyplacment2;
			ballslope[3] = realyplacment2;
			player1 = true;
			player2 = false;
			}
			}
		}
		
		if (menuon == false)
		{
		for (int i = 0; i<SIZE; i++)
		{
			if (((fa[hole[i]] - hole1[i] > 0.25) || (fa[hole[i]] - hole1[i] < -0.25 && turmoil)) && hole1[i] != 0 && fa[hole[i]] > 1.0)
			{
				fa[hole[i]] = fa[hole[i]] - (fa[hole[i]] - hole1[i]) * (scalar * hole2[i]);
				realyplacment = findterrainheight(true);
				realyplacment2 = findterrainheight(false);
				genslope();
			}
			else if (fa[hole[i]] < 1.0)
				fa[hole[i]] = 1.0f;
			else
				hole1[i] = 0;
		}
		}

	if (about == true)
	{
		startgame = true;
		glEnable(GL_TEXTURE_2D);
		glPrint(1.0f, 130,440,0,"Welcome to gLCannonWars.");
		glPrint(1.0f, 150,410,0,"By: Brian Washburn");
		glColor3f(1.0f,0.0f,0.0f);
		glPrint(1.0f, 50,340,0,"Controls:");
		glColor3f(1.0f,1.0f,1.0f);
		glPrint(1.0f, 50,310,0,"up/down: Adjust cannon angle");
		glPrint(1.0f, 50,280,0,"Enter: Fire");
		glColor3f(1.0f,0.0f,0.0f);
		glPrint(1.0f, 50,240,0,"Rules:");
		glColor3f(1.0f,1.0f,1.0f);
		glPrint(1.0f, 50,210,0,"You have a 10-second delay before");
		glPrint(1.0f, 60,180,0,"ur cannon fires on its own.");
		glPrint(1.0f, 50,150,0,"Lower the opponents health");
		glPrint(1.0f, 60,120,0,"to 0 to win.");
		glPrint(1.0f, 280,60,0,"Enjoy.");
	}


	if (startgame == true && about == false)
	{
	//terrain drawing function
	if (menuon == false)
		Addlighting();
	drawwater();
	glEnable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	drawterrain(SIZE);
	if (menuon == false)
		particletime2 = particletime2 + scalar;
	if (particletime2 > Howmuchwater() && player1health > 0 && player2health > 0)
	{
		Addrain();
		particletime2 = 0.0;
	}


	if (cannonball[0].active != true && waiting != true && menuon == false)
	{
		launchtime = launchtime + scalar;
		if (launchtime > 0.15)
		{
		temptemp = 0;
		launchtimetemp = 0;
		while (temptemp < 1)
		{
			if (particle[launchtimetemp].active == false)
				temptemp++;
			else
			{
			if (launchtimetemp < MAX_PARTICLES)
				launchtimetemp++;
			else
				break;
			}
		}

			if (player1)
			{
			particle[launchtimetemp].player1 = false;
			particle[launchtimetemp].player2 = true;
			particle[launchtimetemp].startxpos = realxplacment;
			particle[launchtimetemp].startypos = realyplacment;
			particle[launchtimetemp].x = realxplacment;
			particle[launchtimetemp].y = realyplacment;
			particle[launchtimetemp].xi=cannonball[0].yo+float((rand()%80)-40.0f);
			particle[launchtimetemp].yi=cannonball[0].xo+float((rand()%80)-40.0f);
			}
			else
			{
			particle[launchtimetemp].player1 = true;
			particle[launchtimetemp].player2 = false;
			particle[launchtimetemp].startxpos = realxplacment2;
			particle[launchtimetemp].startypos = realyplacment2;
			particle[launchtimetemp].x = realxplacment2;
			particle[launchtimetemp].y = realyplacment2;
			particle[launchtimetemp].xi=cannonball[1].yo+float((rand()%80)-40.0f);
			particle[launchtimetemp].yi=cannonball[1].xo+float((rand()%80)-40.0f);
			}
			particle[launchtimetemp].size = 3.0f;
			particle[launchtimetemp].active = true;
			particle[launchtimetemp].life = 1.0f;
			particle[launchtimetemp].type = 2;
			particle[launchtimetemp].fade=float(rand()%5)/550.0f+0.004f;
			particle[launchtimetemp].red = 1.0f;
			particle[launchtimetemp].blue = 0.0f;
			particle[launchtimetemp].green = float((rand()%100)/100.0f);
			particle[launchtimetemp].speed = (0.004f - (launchsave * 0.00037f));
			particle[launchtimetemp].time = 0.0f;
			launchsave = launchsave + launchtime;
			launchtime = 0.0;
		}
	}
	if (launchsave > 10.0)
	{
		cannonball[0].speed = initialspeed;
		launchready = true;
		launchsave = 0.0;
		keys[VK_RETURN] = true;
	}
	 //make cannons and buildings
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	Constructbuilding(1, 1);
	Constructbuilding(-1, 2);
	glDepthMask(true);
	}
	else if (about == false)
	{
			glEnable(GL_DEPTH_TEST);
			menuon = true;
			glColor3f(1.0f, 1.0f, 1.0f);
			glBindTexture(GL_TEXTURE_2D, textures[0].texID);
			glBegin(GL_POLYGON);
			glTexCoord2f(0.0f, 0.0f); glVertex2i(100 - 5, 150 - 11);
			glTexCoord2f(1.0f, 0.0f); glVertex2i(100 + 5, 150 - 11);
			glTexCoord2f(1.0f, 1.0f); glVertex2i(100 + 5, 150 - 1);
			glTexCoord2f(0.0f, 1.0f); glVertex2i(100 - 5, 150 - 1);
			glEnd();
			glPushMatrix();
			glTranslatef(100, 150, 0.0f);
			glRotatef((GLfloat) cannonball[0].cannonup, 0.0f, 0.0f, (1.0f * 1.0f));
			glTranslatef((1.0f * 4), 0.0f, 0.0f);
			glLineWidth(3.5f);
			glCallList(cannon);
			glLineWidth(1.2f);
			glPopMatrix();
			glDisable(GL_TEXTURE_2D);
			glEnable(GL_BLEND);
			glBegin(GL_LINE_STRIP);
				glVertex2i(120, 0);
				glVertex2i(110, 138);
				glVertex2i(90, 138);
				glVertex2i(80, 0);
			glEnd();

			glDisable(GL_BLEND);
			glEnable(GL_TEXTURE_2D);


			glColor3f(1.0, 1.0, 1.0f);
			glPrint(0.6f, 485,450,0,"build 10/05/01");
			glPrint(1.0f, 210,300,0,"gLCannonWars");
			glPrint(1.0f, 210,340,0,"iNsaNEiVaN's");
			if (50.0f > cannonball[0].cannonup  && cannonball[0].cannonup > 35.0f)
			{
				glColor3f(1.0f, 0.0f, 0.0f);
				startgamechoice = 1;
			}
			glPrint(1.0f, 220,200,0,"Start Game");
			if (cannonball[0].cannonup >= 50.0f)
				startgamechoice = 0;

			if (cannonball[0].cannonup <= 35.0f && cannonball[0].cannonup >= 10.0f)
			{
				glColor3f(1.0f, 0.0f, 0.0f);
				startgamechoice = 2;
			}
			else
				glColor3f(1.0f, 1.0f, 1.0f);
			glPrint(1.0f, 220,170,0,"About");
			if (cannonball[0].cannonup <= 10.0f)
			{
				glColor3f(1.0f, 0.0f, 0.0f);
				startgamechoice = 3;
			}
			else
				glColor3f(1.0f, 1.0f, 1.0f);
			glPrint(1.0f, 220,140,0,"Quit");
			glColor3f(1.0f, 1.0f, 1.0f);

	}

		glDepthMask(false);
		glEnable(GL_BLEND);

	// draw particles
	if (startgame)
	{
	for (loop=0;loop<MAX_PARTICLES;loop++)
	{
			if (particle[loop].active)
			{	
				if (particle[loop].type == 4)
					glBindTexture(GL_TEXTURE_2D, textures[5].texID);
				else
					glBindTexture(GL_TEXTURE_2D, textures[3].texID);
				if (particle[loop].player1 && Collision(particle[loop].x, particle[loop].y) == false && particle[loop].type != 4 && particle[loop].type != 3 && menuon == false)
				{
					bouncewhere[0] = particle[loop].x;
					bouncewhere[1] = particle[loop].y;
					particle[loop].y = particle[loop].startypos + (((particle[loop].yi * ((particle[loop].speed - particle[loop].speed*0.1f))) * particle[loop].time) - (0.005f * particle[loop].gravity * (particle[loop].time * particle[loop].time)));
					particle[loop].x = particle[loop].startxpos + ((particle[loop].xi * (particle[loop].speed - particle[loop].speed*0.1f)) * particle[loop].time) - (particle[loop].wind * (particle[loop].time * particle[loop].time));
				}
				else if (particle[loop].player2 && Collision(particle[loop].x, particle[loop].y) == false && particle[loop].type != 4 && particle[loop].type != 3 && menuon == false)
				{
					bouncewhere[0] = particle[loop].x;
					bouncewhere[1] = particle[loop].y;
					particle[loop].y = particle[loop].startypos + ((particle[loop].yi * ((particle[loop].speed - particle[loop].speed*0.1f))) * particle[loop].time) - (0.005f * particle[loop].gravity * (particle[loop].time * particle[loop].time));
					particle[loop].x = particle[loop].startxpos - ((particle[loop].xi * (particle[loop].speed - particle[loop].speed*0.1f)) * particle[loop].time) - (particle[loop].wind*-1 * (particle[loop].time * particle[loop].time));
				}
				else if (Collision(particle[loop].x, particle[loop].y) == false && particle[loop].type == 3 && particle[loop].startypos > waterlevel && menuon == false)
				{
					if (particle[loop].player1)
					{
					particle[loop].y = particle[loop].y - particle[loop].yi * scalar * 60;
					particle[loop].x = particle[loop].x - 1*scalar*60;
					}
					else
					{
					particle[loop].y = particle[loop].y + particle[loop].yi * scalar * 60;
					particle[loop].x = particle[loop].x + 1*scalar*60;
					}
				}
				else if (Collision(particle[loop].x, particle[loop].y) == false && particle[loop].type == 4 && particle[loop].y < waterlevel && menuon == false)
				{
					particle[loop].y = particle[loop].startypos + ((particle[loop].yi * ((particle[loop].speed - particle[loop].speed*0.1f))) * particle[loop].time) + (0.005f * particle[loop].gravity * (particle[loop].time * particle[loop].time));
					particle[loop].x = particle[loop].startxpos - ((particle[loop].xi * (particle[loop].speed - particle[loop].speed*0.1f)) * particle[loop].time) - (particle[loop].wind*-1 * (particle[loop].time * particle[loop].time));
					particle[loop].red = 1.0f;
					particle[loop].green = 1.0f;
					particle[loop].blue = 1.0f;
				}

				if (Collision(particle[loop].x, particle[loop].y) == true || (particle[loop].y < waterlevel && (particle[loop].type == 5 || particle[loop].type == 6)) || (particle[loop].y > waterlevel && particle[loop].type == 4) || particle[loop].x < -1 || particle[loop].x > 642 || particle[loop].y < 0)
				{
					if (particle[loop].x > -1 && particle[loop].x < 642 && particle[loop].y > -1)
					{
						if (particle[loop].type == 1)
						{
							Collisionresponse(8.0f);
							particle[loop].active = false;
						}
						else if (particle[loop].type == 2 && particle[loop].x > -1 && particle[loop].x < 642 )
							particlebounce(collidewhere, loop);
						else if (particle[loop].type == 4 && particle[loop].active)
						{
							watersplash(particle[loop].x, 2, float((rand()%3)+1));
							particle[loop].active = false;
						}
						else if (particle[loop].type == 5 && startgame==true && about == false && waterlevel < 480)
						{
							waterlevel = waterlevel + 0.4f;
							if (particle[loop].y < waterlevel)
								watersplash(particle[loop].x, 5, float((rand()%2)+3));
							particle[loop].active = false;
						}	
						else 
							particle[loop].active = false;
					}
					else
						particle[loop].active = false;
				}

				glColor4f(particle[loop].red, particle[loop].green, particle[loop].blue, particle[loop].life);
				if (particle[loop].y < 480)
				{
				glBegin(GL_TRIANGLE_STRIP);
					glTexCoord2i(1,1); glVertex2f(particle[loop].x+particle[loop].size,particle[loop].y+particle[loop].size);
					glTexCoord2i(0,1); glVertex2f(particle[loop].x-particle[loop].size,particle[loop].y+particle[loop].size); 
					glTexCoord2i(1,0); glVertex2f(particle[loop].x+particle[loop].size,particle[loop].y-particle[loop].size);
					glTexCoord2i(0,0); glVertex2f(particle[loop].x-particle[loop].size,particle[loop].y-particle[loop].size);
				glEnd();
				}
				if (menuon == false)
				{
				particle[loop].time = (particle[loop].time + (700 * scalar));
				particle[loop].life -= (particle[loop].fade * (scalar * 40));
				}
				if (particle[loop].type == 3 && particle[loop].size > 2)
					particle[loop].size = particle[loop].life * 4;
				if (particle[loop].life < 0.2)
					particle[loop].active = false;
			}
	}
	}

	//draws player huds
if (player1health > 0 && player2health > 0 && startgame == true && about == false)
{
	if (cannonball[0].active == true)
		glColor4f(1.0f,1.0f,1.0f, 0.5f);
	else
		glColor3f(1.0f,1.0f,1.0f);
	if (player1)
	{
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_LINE_STRIP);
		glVertex2i(180, 480);
		glVertex2i(180, 365);
		glVertex2i(160, 345);
		glVertex2i(0, 345);
		glVertex2i(0, 480);
		glVertex2i(180, 480);
	glEnd();
	glColor4f(colors[color1][0], colors[color1][1], colors[color1][2], 0.07*player1health);
	glBegin(GL_TRIANGLE_STRIP);
		glVertex2i(1.6*player1health, 370);
		glVertex2i(10, 370); 
		glVertex2i(1.6*player1health, 360);
		glVertex2i(10, 360);
	glEnd();
	glColor4f(1.0f, 1.0f, 1.0f, 0.0f);
	glEnable(GL_TEXTURE_2D);
	if (cannonball[0].active != active)
		glColor3f(colors[color1][0], colors[color1][1], colors[color1][2]);
	else
		glColor4f(colors[color1][0], colors[color1][1], colors[color1][2], 0.5f);
	glPrint(1.0, 10,445,0,"player 1"); angle = cannonball[0].cannonup;
	if (cannonball[0].active != active)
		glColor3f(1.0f, 1.0f, 1.0f);
	else
		glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
	glPrint(1.0, 10,410,0,"Angle:%2i", angle);
	glPrint(1.0, 10,380,0,"Health:%2i", player1health);

	glDisable(GL_BLEND);

	}
	else
	{
	if (cannonball[0].active == true)
		glColor4f(1.0f,1.0f,1.0f, 0.5f);
	else
		glColor3f(1.0f,1.0f,1.0f);

	glDisable(GL_TEXTURE_2D);
	glBegin(GL_LINE_STRIP);
		glVertex2i(460, 480);
		glVertex2i(460, 365);
		glVertex2i(480, 345);
		glVertex2i(640, 345);
		glVertex2i(640, 480);
		glVertex2i(460, 480);
	glEnd();
		glColor4f(colors[color2][0], colors[color2][1], colors[color2][2], 0.07*player2health);
	glBegin(GL_TRIANGLE_STRIP);
		glVertex2i(640-(1.6*player2health), 370);
		glVertex2i(630, 370); 
		glVertex2i(640-(1.6*player2health), 360);
		glVertex2i(630, 360);
	glEnd();
	glColor4f(1.0f, 1.0f, 1.0f, 0.0f);
		glEnable(GL_TEXTURE_2D);
	if (cannonball[0].active != active)
		glColor3f(colors[color2][0], colors[color2][1], colors[color2][2]);
	else
		glColor4f(colors[color2][0], colors[color2][1], colors[color2][2], 0.5f);
		glPrint(1.0f, 480,445,0,"player 2"); 
		angle = cannonball[1].cannonup;
		if (cannonball[0].active != active)
			glColor3f(1.0f, 1.0f, 1.0f);
		else
			glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
		glPrint(1.0f, 480,410,0,"Angle:%2i", angle);
		glPrint(1.0, 480,380,0,"Health:%2i", player2health);
		glDisable(GL_BLEND);

	}
}


	glBindTexture(GL_TEXTURE_2D, textures[2].texID);
	//draws scrolling power par
	if (baractive == true)
	{
    glBegin(GL_POLYGON);		
		glTexCoord2i(0, 1);					glVertex3i(120, 100, 1);
		glTexCoord2f(texrightbar, 1.0f);	glVertex3f(rightbar, 100.0f, 1.0f);
		glTexCoord2f(texrightbar, 0.0f);	glVertex3f(rightbar, 75.0, 1.0f);
		glTexCoord2i(0, 0);					glVertex3i(120, 75, 1);
	glEnd();

	if (startgame == true)
	{
		if (bargoingleft == true)
		{
			rightbar = (rightbar - (350 * scalar)); cannonball[0].speed = (cannonball[0].speed - (0.001f * scalar));
			texrightbar = (texrightbar - (350 * scalar) / 360);
		}
		if (bargoingright == true)
		{
			rightbar = (rightbar + (350 * scalar)); cannonball[0].speed = (cannonball[0].speed + (0.001f * scalar));
			texrightbar = (texrightbar + (350 * scalar) / 360);
		}
		if (rightbar < 120.0)
		{
			bargoingleft = false;
			bargoingright = true;
		}
		if (rightbar > 480.0)
		{
			bargoingleft = true;
			bargoingright = false;
		}
	}	
	}
	//if someone won!
	if (player1health <= 0 || player2health <= 0)
	{
		menuon = true;
		glEnable(GL_BLEND);
		glColor3f(1.0f, 1.0f, 1.0f);
		glBegin(GL_LINE_STRIP);
		 glVertex2i(96, 300);
		 glVertex2i(544, 300);
		 glVertex2i(544, 150);
		 glVertex2i(96, 150);
		 glVertex2i(96, 300);
		glEnd();
		glDisable(GL_BLEND);
		if (player1health > 0)
			glPrint(1.5f, 135, 340, 0, "Player 1 wins!!!");
		else
			glPrint(1.5f, 135, 340, 0, "Player 2 wins!!!");
	glPrint(1.1f, 155, 245, 0, "Want to play again?");
	if (menuselection2 == 1)
		glColor3f(1.0f, 0.0f, 0.0f);
	glPrint(1.1f, 220, 190, 0, "Yes"); glColor3f(1.0f, 1.0f, 1.0f);
	if (menuselection2 == 2)
		glColor3f(1.0f, 0.0f, 0.0f);
	glPrint(1.1f, 365, 190, 0, "No"); glColor3f(1.0f, 1.0f, 1.0f);
		if (bob4 == false)
		{
			PlaySound("Data/party.wav", NULL, SND_ASYNC);
			bob4 = true;
		}
	}
	if (startgame == true && about != true && cannonball[0].active == false && menuon == false)
	{
	glScissor(200, 430, 230, 50);
	glEnable(GL_SCISSOR_TEST);
	intgravity = (gravity * 200);
	intwind = (wind * 1000);
	glPrint(0.9f, printplace[0], 430, 0, "Gravity:%3i", intgravity);
	glPrint(0.9f, printplace[1], 430,0,"Wind:%3i", intwind);
	for (int i = 0;i<2;i++)
	{
		printplace[i] = printplace[i] - (90.0f * scalar);
		if (printplace[i] < -600.0f)
			printplace[i] = 600.0f;
	}
	glDisable(GL_SCISSOR_TEST);
	}

	//if (turmoil)
	//	Resizescreen(cannonball[0].x-175, cannonball[0].x+175*1.3, cannonball[0].y-240, cannonball[0].y+240, 75);
	//if (turmoil != true)
	//	Resizescreen(0, 640, 0, 480, 75);
	if (turmoil)
	{
		turmoiltime = turmoiltime + scalar;
		if (turmoiltime < 0.20)
			Resizescreen(lx + turmoiltemp, 599, ly, ty, 40);
		else if (turmoiltime >= 0.20 && turmoiltime < 0.40)
			Resizescreen(lx - turmoiltemp, tx - turmoiltemp, ly, ty, 40);
		else
		{
			if (turmoiltemp > 0.0)
				turmoiltemp = float((rand()%100)+50)*-1;
			else
				turmoiltemp = float((rand()%100)+50);
			turmoiltime = 0.0;
		}
	}
	else
	{
		turmoiltime = 0.0;
		Resizescreen(0, 639, 0, 480, 75);
	}

		

	if (goingintomain)
	{
		if (resize < 1120)
		{
				//glMatrixMode(GL_PROJECTION);
				//glLoadIdentity();
				//gluOrtho2D(0+resize, 640-resize, 0, 480);
				//glMatrixMode(GL_MODELVIEW);
				//glLoadIdentity();
			if (resize < 640)
				glViewport(0+(resize*0.5), 0, 640-resize, 480);
			else
				glViewport(320, (resize-480)*0.5, 0, 480 - (resize-480));
			resize=resize+(800*scalar);
		}
		else
		{
		gamecannon = cannonball[0].cannonup;
		cannonball[0].cannonup = menucannon;
		startgame = false;
		menuon = true;
		resize = 0;
		goingintomain = false;
		//glMatrixMode(GL_PROJECTION);
		//glLoadIdentity();
		//gluOrtho2D(0, 640, 0, 480);
		//glMatrixMode(GL_MODELVIEW);
		//glLoadIdentity();
		glViewport(0, 0, 640, 480);

		}
	}

	if (player1hurt || player2hurt)
		floathurts();

	if (billy && startgame && about == false)
		Menu(false);
	else if (startgame && about == false)
		Menu(true);
	glPrint(0.7, 435,20,0,"High:%2i", Averageheight());
	glPrint(0.7, 550,20,0,"Low:%2i", findlowheight());
	glDepthMask(GL_TRUE);

	return TRUE;										
}

GLvoid KillGLWindow(GLvoid)								// Properly Kill The Window
{
	if (fullscreen)										// Are We In Fullscreen Mode?
	{
		ChangeDisplaySettings(NULL,0);					// If So Switch Back To The Desktop
		ShowCursor(TRUE);								// Show Mouse Pointer
	}

	if (hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd,hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL",hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									// Set hInstance To NULL
	}
}

/*	This Code Creates Our OpenGL Window.  Parameters Are:					*
 *	title			- Title To Appear At The Top Of The Window				*
 *	width			- Width Of The GL Window Or Fullscreen Mode				*
 *	height			- Height Of The GL Window Or Fullscreen Mode			*
 *	bits			- Number Of Bits To Use For Color (8/16/24/32)			*
 *	fullscreenflag	- Use Fullscreen Mode (TRUE) Or Windowed Mode (FALSE)	*/
 
BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left=(long)0;			// Set Left Value To 0
	WindowRect.right=(long)width;		// Set Right Value To Requested Width
	WindowRect.top=(long)0;				// Set Top Value To 0
	WindowRect.bottom=(long)height;		// Set Bottom Value To Requested Height

	fullscreen=fullscreenflag;			// Set The Global Fullscreen Flag

	hInstance			= GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= hInstance;							// Set The Instance
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu
	wc.lpszClassName	= "OpenGL";								// Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											// Return FALSE
	}
	
	if (fullscreen)												// Attempt Fullscreen Mode?
	{
		DEVMODE dmScreenSettings;								// Device Mode
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth	= width;				// Selected Screen Width
		dmScreenSettings.dmPelsHeight	= height;				// Selected Screen Height
		dmScreenSettings.dmBitsPerPel	= bits;					// Selected Bits Per Pixel
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			// If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
			if (MessageBox(NULL,"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?","NeHe GL",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{
				fullscreen=FALSE;		// Windowed Mode Selected.  Fullscreen = FALSE
			}
			else
			{
				// Pop Up A Message Box Letting User Know The Program Is Closing.
				MessageBox(NULL,"Program Will Now Close.","ERROR",MB_OK|MB_ICONSTOP);
				return FALSE;									// Return FALSE
			}
		}
	}

	if (fullscreen)												// Are We Still In Fullscreen Mode?
	{
		dwExStyle=WS_EX_APPWINDOW;								// Window Extended Style
		dwStyle=WS_POPUP;										// Windows Style
		ShowCursor(FALSE);										// Hide Mouse Pointer
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
		dwStyle=WS_OVERLAPPEDWINDOW;							// Windows Style
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if (!(hWnd=CreateWindowEx(	dwExStyle,							// Extended Style For The Window
								"OpenGL",							// Class Name
								title,								// Window Title
								dwStyle |							// Defined Window Style
								WS_CLIPSIBLINGS |					// Required Window Style
								WS_CLIPCHILDREN,					// Required Window Style
								0, 0,								// Window Position
								WindowRect.right-WindowRect.left,	// Calculate Window Width
								WindowRect.bottom-WindowRect.top,	// Calculate Window Height
								NULL,								// No Parent Window
								NULL,								// No Menu
								hInstance,							// Instance
								NULL)))								// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		bits,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};
	
	if (!(hDC=GetDC(hWnd)))							// Did We Get A Device Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(hRC=wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!wglMakeCurrent(hDC,hRC))					// Try To Activate The Rendering Context
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	ShowWindow(hWnd,SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window
	ReSizeGLScene(width, height);					// Set Up Our Perspective GL Screen

	if (!InitGL())									// Initialize Our Newly Created GL Window
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	return TRUE;									// Success
}

LRESULT CALLBACK WndProc(	HWND	hWnd,			// Handle For This Window
							UINT	uMsg,			// Message For This Window
							WPARAM	wParam,			// Additional Message Information
							LPARAM	lParam)			// Additional Message Information
{
	switch (uMsg)									// Check For Windows Messages
	{
		case WM_ACTIVATE:							// Watch For Window Activate Message
		{
			if (!HIWORD(wParam))					// Check Minimization State
			{
				active=TRUE;						// Program Is Active
			}
			else
			{
				active=FALSE;						// Program Is No Longer Active
			}

			return 0;								// Return To The Message Loop
		}

		case WM_SYSCOMMAND:							// Intercept System Commands
		{
			switch (wParam)							// Check System Calls
			{
				case SC_SCREENSAVE:					// Screensaver Trying To Start?
				case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
				return 0;							// Prevent From Happening
			}
			break;									// Exit
		}

		case WM_CLOSE:								// Did We Receive A Close Message?
		{
			PostQuitMessage(0);						// Send A Quit Message
			return 0;								// Jump Back
		}

		case WM_KEYDOWN:							// Is A Key Being Held Down?
		{
			keys[wParam] = TRUE;					// If So, Mark It As TRUE
			return 0;								// Jump Back
		}

		case WM_KEYUP:								// Has A Key Been Released?
		{
			keys[wParam] = FALSE;					// If So, Mark It As FALSE
			return 0;								// Jump Back
		}

		case WM_SIZE:								// Resize The OpenGL Window
		{
			ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));  // LoWord=Width, HiWord=Height
			return 0;								// Jump Back
		}
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

int WINAPI WinMain(	HINSTANCE	hInstance,			// Instance
					HINSTANCE	hPrevInstance,		// Previous Instance
					LPSTR		lpCmdLine,			// Command Line Parameters
					int			nCmdShow)			// Window Show State
{
	MSG		msg;									// Windows Message Structure
	BOOL	done=FALSE;								// Bool Variable To Exit Loop

	// Ask The User Which Screen Mode They Prefer
	if (MessageBox(NULL,"gLCannonwars: by iNsaNEiVaN    Would You Like To Run In Fullscreen Mode?", "Start FullScreen?",MB_YESNO|MB_ICONQUESTION)==IDNO)
	{
		fullscreen=FALSE;							// Windowed Mode
	}

	// Create Our OpenGL Window
	if (!CreateGLWindow("gLCannonwars",640,480,16,fullscreen))
	{
		return 0;									// Quit If Window Was Not Created
	}

	while(!done)									// Loop That Runs While done=FALSE
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message==WM_QUIT)				// Have We Received A Quit Message?
			{
				done=TRUE;							// If So done=TRUE
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		else										// If There Are No Messages
		{
			// Draw The Scene.  Watch For ESC Key And Quit Messages From DrawGLScene()
			if (active)								// Program Active?
			{
				if (keys[VK_ESCAPE])				// Was ESC Pressed?
				{
					if (startgame == true && about == false && (player1health > 0 || player2health > 0))
					{
						goingintomain = true;
					}
					if (about == true)
					{
						about = false;
						startgame = false;
					}
					keys[VK_ESCAPE] = false;
				}
				else								// Not Time To Quit, Update Screen
				{
					endtime = timeGetTime();
					timetaken = endtime - starttime;
					scalar = timetaken/1000.0f;
					DrawGLScene();					// Draw The Scene
					SwapBuffers(hDC);				// Swap Buffers (Double Buffering)
				}
			}

			if (keys[VK_UP])
			{
				if (cannonball[0].cannonup > 90 || cannonball[1].cannonup > 90)
				{
					if (cannonball[0].cannonup > 89)
						cannonball[0].cannonup = 90;
					else 
						cannonball[1].cannonup = 90;
				}
				else if (player1 != true || player2 != true)
					{
						if (player1 == true || startgame == false)
						{
							cannonball[0].cannonup = (cannonball[0].cannonup + 170.0f * scalar);
							cannonball[0].xi = 90.0f - cannonball[0].cannonup;
							cannonball[0].yi = cannonball[0].cannonup;
						}
						if (player2 == true)
						{
							cannonball[1].cannonup = (cannonball[1].cannonup + 170.0f * scalar);
							cannonball[1].xi = 90.0f - cannonball[1].cannonup;
							cannonball[1].yi = cannonball[1].cannonup;
						}
					}
				if (billy)
				{
					if (menu2 > 1)
					{
						keys[VK_UP] = false;
						menu2--;
					}
				}
			}

			if (keys[VK_DOWN])
			{
				if (cannonball[0].cannonup < 0 || cannonball[1].cannonup < 0)
				{
					if (cannonball[0].cannonup < 1)
						cannonball[0].cannonup = 0;
					else
						cannonball[1].cannonup = 0;
				}
				else if (player1 != true || player2 != true)
					{
						if (player1 == true || startgame == false)
						{
							cannonball[0].cannonup = (cannonball[0].cannonup - 170.0f * scalar);
							cannonball[0].xi = 90.0f - cannonball[0].cannonup;
							cannonball[0].yi = cannonball[0].cannonup;
						}
						if (player2 == true)
						{
							cannonball[1].cannonup = (cannonball[1].cannonup - 170.0f * scalar);
							cannonball[1].xi = 90.0f - cannonball[1].cannonup;
							cannonball[1].yi = cannonball[1].cannonup;
						}
					}
				if (billy)
				{
					if (menu2 < 5)
					{
						keys[VK_DOWN] = false;
						menu2++;
					}
				}
			}
			if (keys[VK_RIGHT])
			{
				if (player1health < 0 || player2health < 0 && menuselection2 == 1)
				{
					menuselection2 = 2;
				}
				if (menu2 == 2)
					intgravity++;
				keys[VK_RIGHT] = false;
			}
			if (keys[VK_LEFT])
			{
				if ((player1health < 0 || player2health < 0) && menuselection2 == 2)
				{
					menuselection2 = 1;
				}
				if (menu2 == 2)
					intgravity--;
				keys[VK_LEFT] = false;
			}
			if (keys[VK_HOME])
			{
				if (cannonball[0].active != true)
				{
				if (player1)
				{
				color1++;
				if (color1 == 9)
					color1 = 0;
				}
				else
				{
				color2++;
				if (color2 == 9)
					color2 = 0;
				}
				}
				keys[VK_HOME] = false;
			}
			if (keys[VK_END])
			{
					//if (turmoil)
					//	turmoil = false;
					//else
						//turmoil = true;
				//gravity = (gravity + 0.01f);
				//CaptureScreen();
					//if (billy)
						//billy = false;
					//else
						//billy = true;
				Earthquake(SIZE);
				keys[VK_END] = false;
			}



			if (keys[VK_RETURN])
			{
			if (billy == true && menu2 == 4)
			{
				terrainsmooth(1);
				genslope();
			for (int i = 0; i < SIZE; i++)
			{
			tlighting[i][0] = 1.0;
			tlighting[i][1] = 1.0;
			tlighting[i][2] = 1.0;
			}
				Addlighting();
				realyplacment = findterrainheight(true);
				realyplacment2 = findterrainheight(false);
				keys[VK_RETURN] = false;
			}
			if ((player1health <= 0 || player2health <= 0) && menuselection2 == 2)
			{
				done = TRUE;
			}
			if (baractive == false && launchready == false && menuon == false && cannonball[0].active == false && player1health > 0 && player2health > 0)
			{
			baractive = true; cannonball[0].speed = initialspeed;
			bargoingleft = true; bargoingright = false;
			keys[VK_RETURN] = false;
			}
			else
			{
				if ((baractive == true || launchready) && menuon == false && cannonball[0].active == false && player1health > 0 && player2health > 0)
				{
				temptemp = 0;
				int loop = 0;
				while (temptemp < 10)
				{
					if (particle[loop].active == false)
					{
						if (player1)
						{
						particle[loop].active = true;
						particle[loop].life = 1.0f;
						particle[loop].fade=float(rand()%5)/1000.0f+0.004f;
						particle[loop].player1 = true;
						particle[loop].player2 = false;
						particle[loop].type = 1;
						particle[loop].startxpos = realxplacment;
						particle[loop].startypos = realyplacment;
						particle[loop].x = realxplacment;
						particle[loop].y = realyplacment;
						particle[loop].xi=cannonball[0].xo+float((rand()%60)-30.0f);
						particle[loop].yi=cannonball[0].yo+float((rand()%60)-30.0f);
						particle[loop].speed = (cannonball[0].speed * 1.2f);
						particle[loop].time = 0.0f;
						particle[loop].red = 1.0f;
						particle[loop].green = 0.0f;
						particle[loop].blue = 0.0f;
						particle[loop].size = 3.0f;
						}
						else
						{
						particle[loop].active = true;
						particle[loop].life = 1.0f;
						particle[loop].fade=float(rand()%5)/1000.0f+0.004f;
						particle[loop].player1 = false;
						particle[loop].player2 = true;
						particle[loop].type = 1;
						particle[loop].x = realxplacment;
						particle[loop].y = realyplacment;
						particle[loop].xi=cannonball[1].xo+float((rand()%60)-30.0f);
						particle[loop].yi=cannonball[1].yo+float((rand()%60)-30.0f);
						particle[loop].startxpos=realxplacment2;
						particle[loop].startypos=realyplacment2;
						particle[loop].speed = (cannonball[0].speed * 1.2f);
						particle[loop].time = 0.0f;
						particle[loop].red = 1.0f;
						particle[loop].green = 0.0f;
						particle[loop].blue = 0.0f;
						particle[loop].size = 3.0f;
						}
						temptemp++;
					}
					else
					{
					if (loop < MAX_PARTICLES)
						loop++;
					else
						break;
					}
				}

				launchready = false; launchtime = 0.0; launchsave = 0.0;
				cannonball[0].active = true;
				baractive = false; rightbar = 480.0; texrightbar = 1.0;
				keys[VK_RETURN] = false;
				PlaySound("Data/boom.wav", NULL, SND_ASYNC);
				}	
			}
			if (startgame == false)
			{
			switch (startgamechoice)
			{
			case 1:
				Newround();
				keys[VK_RETURN] = false;
				break;
			case 2:
				about = true;
				break;
			case 3:
				done = true;
			}
			}


			//restarts game
			if ((player1health < 0 || player2health < 0) && menuselection2 == 1)
			{
				Newround();
				keys[VK_RETURN] = false;
			}
			}

			if (keys[VK_F1])						// Is F1 Being Pressed?
			{
				keys[VK_F1]=FALSE;					// If So Make Key FALSE
				KillGLWindow();						// Kill Our Current Window
				fullscreen=!fullscreen;				// Toggle Fullscreen / Windowed Mode
				// Recreate Our OpenGL Window
				if (!CreateGLWindow("gLCannonwars: iNsaNEiVaN",640,480,16,fullscreen))
				{
					return 0;						// Quit If Window Was Not Created
				}
			}
		}
	}

	// Shutdown
	KillFont();
	KillGLWindow();									// Kill The Window
	return (msg.wParam);							// Exit The Program
}
