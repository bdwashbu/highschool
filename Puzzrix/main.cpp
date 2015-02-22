#include <windows.h>
#include <stdio.h>		
#include <gl\gl.h>			
#include <gl\glu.h>
#include <math.h>
#include "block.h"
#include "grid.h"
#include "texture.h"
#include "time.h"
//#include "fmod.h"
#include "particle.h"
#include "player.h"
//#include "resource.h"

void NewMusic();
void ReassignBlock();

//int GRIDX = 12;
int GRIDY = 20;

HDC			hDC=NULL;		
HGLRC		hRC=NULL;		
HWND		hWnd=NULL;	
HINSTANCE	hInstance;		

bool	keys[256];		
bool	active=TRUE;	
bool	fullscreen=TRUE;
double begintime, scalar;

FSOUND_SAMPLE *boxhit = 0, *stomp = 0, *flip = 0, *fight = 0, *gameover = 0, *menuselect;
FMUSIC_MODULE *mainsong = 0;
blockclass *block = 0;
blockclass *nextblock = 0;
gridclass grid(70,10,12,20, 428, false);
playerclass *player = new playerclass;
textureclass texture[8];
int stickcount = 0;
const int Gridxoffset = 42;
bool framebyframe = false;
bool done = false;
float polartime = 0.0f;
bool TitleScreen = true;
bool Transitioningin = true;
bool HighScoreBoard = false;
bool Music[8] = {false, false, false, false, false, false, false, false};
bool IsLoaded = false;
float wavetimer = 0.0f;
waterheadclass waterhead;

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	

GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		
{
	if (height==0)										
	{
		height=1;										
	}

	glViewport(0,0,width,height);						

	glMatrixMode(GL_PROJECTION);						
	glLoadIdentity();								

	//gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);
	glOrtho(0, 640, 0, 480, 6, 0);


	glMatrixMode(GL_MODELVIEW);							
	glLoadIdentity();									
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

bool Load()
{
	flip = FSOUND_Sample_Load(FSOUND_UNMANAGED, "Data/flip.wav", FSOUND_NORMAL, 0);
	stomp = FSOUND_Sample_Load(FSOUND_UNMANAGED, "Data/stomp.wav", FSOUND_NORMAL, 0);
	boxhit = FSOUND_Sample_Load(FSOUND_UNMANAGED, "Data/boxhit.wav", FSOUND_NORMAL, 0);
	fight = FSOUND_Sample_Load(FSOUND_UNMANAGED, "Data/fight.wav", FSOUND_NORMAL, 0);
	gameover = FSOUND_Sample_Load(FSOUND_UNMANAGED, "Data/gameover.wav", FSOUND_NORMAL, 0);

	//FSOUND_Init(44100, 32, 0);
	//FSOUND_SetOutput(FSOUND_OUTPUT_DSOUND);
	//FSOUND_SetDriver(0);

	//FSOUND_SetVolume(FSOUND_ALL, 1);

	texture[4].LoadTGA(4, "Data/cloud.tga", true);
	texture[3].LoadTGA(3, "Data/lightmap.tga", true);

	int temp = rand()%7;
	if (temp == 0)
		nextblock = new stick();
	else if (temp == 1)
		nextblock = new square();
	else if (temp == 2)
		nextblock = new curveright();
	else if (temp == 3)
		nextblock = new curveleft();
	else if (temp == 4)
		nextblock = new longcurveright();
	else if (temp == 5)
		nextblock = new longcurveleft();
	else if (temp == 6)
		nextblock = new goodpiece();
	nextblock->Restart(true);
	//if (nextblock)
	//	nextblock->NextBlock();

	IsLoaded = true;

	return true;
}

int InitGL(GLvoid)									
{
	srand( time(NULL) );
	glShadeModel(GL_SMOOTH);							
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);				
	glClearDepth(1.0f);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glAlphaFunc(GL_NOTEQUAL,0.0f);
	glEnable(GL_ALPHA_TEST);
	glDepthFunc(GL_NEVER);
	glPointSize(4.0f);
	glDisable(GL_DEPTH_TEST);
	texture[2].LoadTGA(2, "Data/font.tga", true);
	texture[0].LoadTGA(0, "Data/Block.tga", true);
	texture[7].LoadTGA(7, "Data/loading.tga", true);
	//texture[5].LoadTGA(5, "Data/shadowmap.tga", true);
	//texture[6].LoadTGA(6, "Data/particle.tga", true);
	//texture[7].LoadTGA(7, "Data/opening.tga", false);
	texture[5].LoadTGA(5, "Data/particle.tga", true);
	texture[6].LoadTGA(6, "Data/opening.tga", false);
	texture[1].LoadTGA(1, "Data/logo.tga", true);


	menuselect = FSOUND_Sample_Load(FSOUND_UNMANAGED, "Data/menu_select.wav", FSOUND_NORMAL, 0);

	NewMusic();

	texture[2].BuildFont();
	//begin fmod init
	//Load();

	glScissor(20, 8, 292, 430);
	return TRUE;

	//block = new blockclass();	//init first block piece
	//TetrisBlock = true;
}

bool DrawHighScoreBoard(GLvoid)
{
	static snowparticleheadclass head;
	static double timer = 0;
	begintime = float(timeGetTime());
	glClear(GL_COLOR_BUFFER_BIT);
	if (keys[VK_RETURN]) {
		HighScoreBoard = false;
		TitleScreen = true;
		head.DeleteAll();
		keys[VK_RETURN] = false;
	}

	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	texture[2].glPrint(1.0f, 100, 400, 0, "Highest Scores");

	for (int i = 0; i < 5; i++) {
		texture[2].glPrint(1.0f, 100, 300-(float(i)*25), 0, "%2i", player->highscore[i]);
		texture[2].glPrint(1.0f, 350, 300-(float(i)*25), 0, "%c", player->highscorename[i][0]);
		texture[2].glPrint(1.0f, 365, 300-(float(i)*25), 0, "%c", player->highscorename[i][1]);
		texture[2].glPrint(1.0f, 380, 300-(float(i)*25), 0, "%c", player->highscorename[i][2]);
	}
	texture[2].glPrint(0.9f, 595, 455, 0, "%2i", player->calculatefps(scalar));


	glColor4f(1.0f, 1.0f, 1.0f, 0.5f+0.5*sin(3*player->timeelapsed));
	texture[2].glPrint(0.8f, 490, 10, 0, "Press Enter");

	glBindTexture(GL_TEXTURE_2D, texture[5].texID);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);

	head.Check();
	glBegin(GL_QUADS);
	for (snow *draw2 = head.first.next; draw2 != 0; draw2=draw2->next) {
		draw2->DrawParticle(scalar);
	}
	glEnd();

	if (timer+scalar < 0.015f)
		timer+=scalar;
	else {
		head.Insert();
		timer = 0.0f;
	}

	player->timeelapsed += float(scalar);
	return false;
}

float myarctan(float x, float y)
{
    if (fabs(x) < 0.000001) {
		if (fabs(y) < 0.000001) {
			return(0.0);
        }
		else if (y > 0.0) {
			return(1.570796327f);  /*(pi*0.5);*/
        }
		else {
			return(4.712388980f);  /*(pi*1.5);*/
        }
    } else if (x<0.0) {
		return(atan(y/x)+ 3.1415926535);
    } else {
        return(atan(y/x)); 
    }
}

bool DrawTitleScreen(GLvoid)
{
	static float fadealpha = 0.0f;
	static int menuselection = 1;
	static float SelectionBoxY = 232.0f;
	static explodeparticleheadclass head;
	static fireheadclass firehead;
	static float firetimer = 0.0f;
	begintime = float(timeGetTime());
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	glClear(GL_COLOR_BUFFER_BIT);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);

	if (fadealpha > 0.0f && fadealpha < 1.0f && Transitioningin == true) {
		keys[VK_RETURN] = false;
		fadealpha+=float(scalar);
	}
	else if (fadealpha >= 1.0f && Transitioningin == true) {
		if (IsLoaded == false) { //only do loading the first time when block == 0
			glBindTexture(GL_TEXTURE_2D, texture[7].texID);

			for (float total = 0.0f, Y = 0.0f, X = 0.0f; total < 3.0f; total+=float(scalar)) { //being loading loop
			scalar = (timeGetTime() - begintime)/1000.0;
			begintime = float(timeGetTime());
			glClear(GL_COLOR_BUFFER_BIT);
			if (total+scalar < 2.0f) {
				glColor4f(1.0f, 1.0f, 1.0f, total/2.0f);//-(total/2.55f));
				glBegin(GL_QUADS);
				X+=scalar*3.0f;
				Y+=scalar*6.0f;
				for (float i = 0; i < 20; i++) {
					glTexCoord2f(i/20.0f+0.05f,1);	glVertex2f(230+i*10+10+X*sin((total*4.0f+i+1)/2.0f), 255+Y*sin((total*4.0f+i+1)/2.0f));
					glTexCoord2f(i/20.0f+0.05f,0);	glVertex2f(230+i*10+10+X*sin((total*4.0f+i+1)/2.0f), 225+Y*sin((total*4.0f+i+1)/2.0f));
					glTexCoord2f(i/20.0f,0);		glVertex2f(230+i*10+X*sin((total*4.0f+i)/2.0f), 225+Y*sin((total*4.0f+i)/2.0f));
					glTexCoord2f(i/20.0f,1);		glVertex2f(230+i*10+X*sin((total*4.0f+i)/2.0f), 255+Y*sin((total*4.0f+i)/2.0f));
				}
				glEnd();
					
				//texture[2].glPrint(1.0f, 260.0f, 225.0f+sinf(total*3.14f)*(65.0f*(1.0f-(total/1.9f))), 0, "Loading");
				//glColor4f(1.0f, 1.0f, 1.0f, 0.9f-(total/2.4f));
				//texture[2].glPrint(1.0f, 260.0f, 225.0f+sinf(total*3.14f-0.1f)*(65.0f*(1.0f-(total/1.9f))), 0, "Loading");
				//glColor4f(1.0f, 1.0f, 1.0f, 0.8f-(total/2.4f));
				//texture[2].glPrint(1.0f, 260.0f, 225.0f+sinf(total*3.14f-0.2f)*(65.0f*(1.0f-(total/1.9f))), 0, "Loading");
				//glColor4f(1.0f, 1.0f, 1.0f, 0.7f-(total/2.4f));
				//texture[2].glPrint(1.0f, 260.0f, 225.0f+sinf(total*3.14f-0.3f)*(65.0f*(1.0f-(total/1.9f))), 0, "Loading");
			}
			else if (total+scalar > 2.0f && total < 2.0f) {
				for (int i = 0; i < 400; i++)
					head.Insert(260,235,100,30);
				glDisable(GL_TEXTURE_2D);
				glEnable(GL_POINT_SMOOTH);
				glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
			}
			else
			{
				head.Check();
				for (explodeparticle *draw2 = head.first.next; draw2 != 0; draw2=draw2->next)
					draw2->DrawParticle(scalar);
			}
			SwapBuffers(hDC);
			}
			firehead.DeleteAll();
			head.DeleteAll();
			glDisable(GL_POINT_SMOOTH);
			glDisable(GL_TEXTURE_2D);
		} //end loading loop

		if (IsLoaded == false)
			Load();
		if (IsLoaded == true) {
			Transitioningin = false;
			TitleScreen = false;
		}
	}

	if (fadealpha >= 0.0f && Transitioningin == false) {
		fadealpha-=float(scalar);
		keys[VK_RETURN] = false;
	}
	else if (fadealpha < 0.0f && Transitioningin == false) {
		Transitioningin = true;
	}

	glBindTexture(GL_TEXTURE_2D, texture[6].texID);
	float dist = 0.0f;
	float theta = 0.0f;
	float xpos = 320+sinf(player->timeelapsed)*200;
	float ypos = 240+sinf(player->timeelapsed/2.0f)*100;
	float i = 0.0f, z = 0.0f;
	float XGRIDSIZE = 33.0f;
	float YGRIDSIZE = 25.0f;
	float XSQUARESIZE = (680.0f/XGRIDSIZE);
	float YSQUARESIZE = (480.0f/YGRIDSIZE);
	float XTEXSIZE = 1.0f/XGRIDSIZE;
	float YTEXSIZE = 1.0f/YGRIDSIZE;

	glBegin(GL_QUADS);
	for(i = 0; i < YGRIDSIZE; i++) {
		for (z = 0; z < XGRIDSIZE; z++) {

	glTexCoord2f(z/XGRIDSIZE+XTEXSIZE,i/YGRIDSIZE+YTEXSIZE);

	dist = sqrt((XSQUARESIZE*z+XSQUARESIZE-xpos)*(XSQUARESIZE*z+XSQUARESIZE-xpos)+(YSQUARESIZE*i+YSQUARESIZE-ypos)*(YSQUARESIZE*i+YSQUARESIZE-ypos));
	theta = myarctan((XSQUARESIZE*z+XSQUARESIZE-xpos),(YSQUARESIZE*i+YSQUARESIZE-ypos));
	if (dist < 55.0f)
		glColor3f(0.0f, 0.0f, 0.0f);
	else
		glColor3f(1.0f, 1.0f, 1.0f);
	if (dist < 100.0f)
		dist=exp(log((dist/100.0f))*0.1f)*100.0f;


	glVertex2f(dist * cos(theta)+xpos, dist * sin(theta)+ypos);
	glTexCoord2f(z/XGRIDSIZE+XTEXSIZE,i/YGRIDSIZE);

	dist = sqrt((XSQUARESIZE*z+XSQUARESIZE-xpos)*(XSQUARESIZE*z+XSQUARESIZE-xpos)+(YSQUARESIZE*i-ypos)*(YSQUARESIZE*i-ypos));
	theta = myarctan((XSQUARESIZE*z+XSQUARESIZE-xpos),(YSQUARESIZE*i-ypos));
	if (dist < 55.0f)
		glColor3f(0.0f, 0.0f, 0.0f);
	else
		glColor3f(1.0f, 1.0f, 1.0f);
	if (dist < 100.0f)
		dist=exp(log((dist/100.0f))*0.1f)*100.0f;

	glVertex2f(dist * cos(theta)+xpos, dist * sin(theta)+ypos);
	glTexCoord2f(z/XGRIDSIZE,i/YGRIDSIZE);

	dist = sqrt((XSQUARESIZE*z-xpos)*(XSQUARESIZE*z-xpos)+(YSQUARESIZE*i-ypos)*(YSQUARESIZE*i-ypos));
	theta = myarctan((XSQUARESIZE*z-xpos),(YSQUARESIZE*i-ypos));
	if (dist < 55.0f)
		glColor3f(0.0f, 0.0f, 0.0f);
	else
		glColor3f(1.0f, 1.0f, 1.0f);
	if (dist < 100.0f)
		dist=exp(log((dist/100.0f))*0.1f)*100.0f;

	glVertex2f(dist * cos(theta)+xpos, dist * sin(theta)+ypos);
	glTexCoord2f(z/XGRIDSIZE,i/YGRIDSIZE+YTEXSIZE);

	dist = sqrt((XSQUARESIZE*z-xpos)*(XSQUARESIZE*z-xpos)+(YSQUARESIZE*i+YSQUARESIZE-ypos)*(YSQUARESIZE*i+YSQUARESIZE-ypos));
	theta = myarctan((XSQUARESIZE*z-xpos),(YSQUARESIZE*i+YSQUARESIZE-ypos));
	if (dist < 55.0f)
		glColor3f(0.0f, 0.0f, 0.0f);
	else
		glColor3f(1.0f, 1.0f, 1.0f);
	if (dist < 100.0f)
		dist=exp(log((dist/100.0f))*0.1f)*100.0f;

	glVertex2f(dist * cos(theta)+xpos, dist * sin(theta)+ypos);
	}
}
glEnd();

	//glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	glColor4f(1.0f, 1.0f, 1.0f, 0.9f);
	
	if (firetimer > 0.01) {
		if (menuselection == 1 && IsLoaded == false) {
			firehead.Insert(205, int(SelectionBoxY), 190, 1);
			firehead.Insert(205, int(SelectionBoxY), 190, 1);
			firehead.Insert(205, int(SelectionBoxY), 190, 1);
		}
		else if (menuselection == 1 && IsLoaded == true) {
			firehead.Insert(205, int(SelectionBoxY), 110, 1);
			firehead.Insert(205, int(SelectionBoxY), 110, 1);
			firehead.Insert(205, int(SelectionBoxY), 110, 1);
		}
		else if (menuselection == 2) {
			firehead.Insert(205, int(SelectionBoxY), 160, 1);
			firehead.Insert(205, int(SelectionBoxY), 160, 1);
		}
		else
			firehead.Insert(205, int(SelectionBoxY), 50, 1);
		firetimer = 0.0f;
	}
	else
		firetimer+=scalar;
	firehead.Check();
	glBindTexture(GL_TEXTURE_2D, texture[5].texID);
	for (fire *draw2 = firehead.first.next; draw2 != 0; draw2=draw2->next) {
		draw2->DrawParticle(scalar);
	}

	//glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(1.0f, 1.0f, 1.0f, 0.43f);
	texture[2].glPrint(3.0f, 80.0f+5.0f*sinf(-player->timeelapsed-1.0f), 340.0f+5.0f*sinf(player->timeelapsed-4.0f), 0, "Elementrix");
	texture[2].glPrint(3.0f, 80.0f+5.0f*sinf(player->timeelapsed-2.0f), 340.0f+5.0f*sinf(-player->timeelapsed-3.0f), 0, "Elementrix");
	texture[2].glPrint(3.0f, 80.0f+5.0f*sinf(-player->timeelapsed-3.0f), 340.0f+5.0f*sinf(player->timeelapsed-1.0f), 0, "Elementrix");
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	texture[2].glPrint(0.9f, 595, 455, 0, "%2i", player->calculatefps(scalar));
	texture[2].glPrint(0.9f, 200, 300, 0, "By Brian Washburn");
	texture[2].glPrint(0.75f, 421, 4, 0, "Build 2.1 12/06/02");
	glBlendFunc(GL_DST_COLOR,GL_ONE_MINUS_SRC_ALPHA); //func used to make text outline black
	if (menuselection == 1)
		SelectionBoxY -= ((SelectionBoxY-222.0f)*float(scalar)*8.0f);
	else if (menuselection == 2)
		SelectionBoxY -= ((SelectionBoxY-182.0f)*float(scalar)*8.0f);
	else
		SelectionBoxY -= ((SelectionBoxY-142.0f)*float(scalar)*8.0f);

		if (menuselection == 1)
			glColor3f(0.75f+sinf(player->timeelapsed*3)*0.25f, 0.0f, 0.0f);
		else 
			glColor3f(1.0f, 1.0f, 1.0f);
		if (IsLoaded == false)
			texture[2].glPrint(1.0f, 200, 220, 0, "Play glTetris");
		else
			texture[2].glPrint(1.0f, 200, 220, 0, "Continue");

		if (menuselection == 2)
			glColor3f(0.75f+sinf(player->timeelapsed*3)*0.25f, 0.0f, 0.0f);
		else
			glColor3f(1.0f, 1.0f, 1.0f);
		texture[2].glPrint(1.0f, 200, 180, 0, "High Scores");
				
		if (menuselection == 3)
			glColor3f(0.75f+sinf(player->timeelapsed*3)*0.25f, 0.0f, 0.0f);
		else
			glColor3f(1.0f, 1.0f, 1.0f);
		texture[2].glPrint(1.0f, 200, 140, 0, "Exit");


	if (keys[VK_DOWN] && menuselection < 3) {
		keys[VK_DOWN] = false;
		FSOUND_PlaySound(FSOUND_FREE, menuselect);
		menuselection++;
	}
	if (keys[VK_UP] && menuselection > 1)
	{
		keys[VK_UP] = false;
		FSOUND_PlaySound(FSOUND_FREE, menuselect);
		menuselection--;
	}

	if (keys[VK_RETURN] && menuselection == 1) {
		float limit = 0;
		fadealpha = 0.01f;
		keys[VK_RETURN] = false;
		if (FMUSIC_GetMasterVolume(mainsong) == 75)
			FMUSIC_SetMasterVolume(mainsong, 200);
	}
	else if (keys[VK_RETURN] && menuselection == 2) {
		firehead.DeleteAll();
		TitleScreen = false;
		HighScoreBoard = true;
		glEnable(GL_BLEND);
		glEnable(GL_TEXTURE_2D);
	}
	else if (keys[VK_RETURN] && menuselection == 3) {
		firehead.DeleteAll();
		done = true;
	}
	glDisable(GL_TEXTURE_2D);
	glColor4f(0.0f, 0.0f, 0.0, fadealpha); //fading quad
	glBegin(GL_QUADS);
		glVertex2i(640,480);
		glVertex2i(0, 480);
		glVertex2i(0,0);
		glVertex2i(640, 0);
	glEnd();
	glEnable(GL_TEXTURE_2D);
	player->timeelapsed += float(scalar);
	return false;
}

//**********************MAIN SCENE DRAWING CODE*********************


int DrawGLScene(GLvoid)									
{
	static float Red = player->Red, Green = player->Green, Blue = player->Blue;
	static float Rotation = 0;
	static int menuselection = 1;
	static float SelectionBoxY = 110;
	static float waveheight = 1.0f;
	//bool *key = &keys[VK_RETURN];
	begintime = timeGetTime();
	glClear(GL_COLOR_BUFFER_BIT);	
	//glLoadIdentity();

	glEnable(GL_TEXTURE_2D);

	//glMatrixMode(GL_TEXTURE);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	Red += (player->Red - Red)*float(scalar);
	Green += (player->Green - Green)*float(scalar);
	Blue += (player->Blue - Blue)*float(scalar);

	//background
	glColor4f(Red, Green, Blue, 0.333f);
	glPushMatrix();
	glTranslatef(320, 240, 0);
	glRotatef(Rotation, 0, 0, 1);
	glBindTexture(GL_TEXTURE_2D, texture[4].texID);
	glBegin(GL_QUADS);   
	glTexCoord2i(1,1); glVertex2i(500, 500);
	glTexCoord2i(1,0); glVertex2i(-500, 500);
	glTexCoord2i(0,0); glVertex2i(-500, -500);
	glTexCoord2i(0,1); glVertex2i(500, -500);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(320, 240, 0);
	glRotatef(Rotation+90, 0, 0, 1);
	//glBindTexture(GL_TEXTURE_2D, texture[4].texID);
	glBegin(GL_QUADS);   
	glTexCoord2i(1,1); glVertex2i(500, 500);
	glTexCoord2i(1,0); glVertex2i(-500, 500);
	glTexCoord2i(0,0); glVertex2i(-500, -500);
	glTexCoord2i(0,1); glVertex2i(500, -500);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(320, 240, 0);
	glRotatef(-Rotation-180.0f, 0, 0, 1);
	//glBindTexture(GL_TEXTURE_2D, texture[4].texID);
	glBegin(GL_QUADS);   
	glTexCoord2i(1,1); glVertex2i(500, 500);
	glTexCoord2i(1,0); glVertex2i(-500, 500);
	glTexCoord2i(0,0); glVertex2i(-500, -500);
	glTexCoord2i(0,1); glVertex2i(500, -500);
	glEnd();
	glPopMatrix();


	if (Rotation < 360)
		Rotation+=float(scalar)*10;
	else
		Rotation = 0;
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);	
	glBindTexture(GL_TEXTURE_2D, texture[1].texID);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);   //main logo
	glTexCoord2i(1,1); glVertex2i(580, 450);
	glTexCoord2i(1,0); glVertex2i(580, 386);
	glTexCoord2i(0,0); glVertex2i(410, 386);
	glTexCoord2i(0,1); glVertex2i(410, 450);
	glEnd();
	texture[2].glPrint(0.9f, 595, 455, 0, "%2i", player->calculatefps(scalar));
	texture[2].glPrint(1.0f, 385, 220, 0, "Score");
	texture[2].glPrint(1.0f, 490, 200, 0, "%2i", int(player->FloatScore));
	texture[2].glPrint(0.8f, 410, 357, 0, "Brian Washburn");
	texture[2].glPrint(0.75f, 491, 4, 0, "Enter: Pause");
	texture[2].glPrint(0.75f, 491, 27, 0, "Home: Music");

	glDisable(GL_TEXTURE_2D);
	//end texturing

	//glEnable(GL_BLEND);
	//*********DRAW GRID OUTLINE AND SCORE BAR
	glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
	glLineWidth(2.5f);
	glEnable(GL_LINE_SMOOTH);
	glLineWidth(2.0f);
	glBegin(GL_LINE_LOOP); //nextpiece box
		glVertex2i(400, 270);
		glVertex2i(400, 340);
		glVertex2i(515, 340);
		glVertex2i(515, 270);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex2i(10, 10);
		glVertex2i(55, 10);
		glVertex2i(55, 470);
		glVertex2i(10, 470);
	glEnd();

	//level guage

	glColor4f(0.0f, 0.0f, 0.0f, 0.4f);
	glBegin(GL_QUADS);
		glVertex2i(12, 12);
		glVertex2i(53, 12);
		glVertex2i(53, 468);
		glVertex2i(12, 468);
	glEnd();

	//render level bar and waves

	glBlendFunc(GL_SRC_ALPHA,GL_ONE);

	waterhead.Check();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[5].texID);
	for (water *draw2 = waterhead.first.next; draw2 != 0; draw2=draw2->next) {
		draw2->DrawParticle(scalar);
	}
	glDisable(GL_TEXTURE_2D);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (player->FloatScore < player->Score)
		waveheight+=scalar;
	else if (waveheight > 1.0f)
		waveheight-=scalar/2.0;


	glColor4f(0.0f, 0.0f, 1.0f, 0.3f);
	glBegin(GL_QUAD_STRIP);
		glColor4f(0.1f, 0.1f, 0.6f, 0.7f);
		glVertex2f(11, 15+423*((player->FloatScore-(player->NewLevel-player->Levelgap))/player->Levelgap)+waveheight*(sin(wavetimer)+0.5f*cos(wavetimer/2)));
		glColor4f(0.07f, 0.07f, 0.07f, 0.7f);
		glVertex2i(11, 10);
		glColor4f(0.1f, 0.1f, 0.6f, 0.7f);
		glVertex2f(15, 15+423*((player->FloatScore-(player->NewLevel-player->Levelgap))/player->Levelgap)+waveheight*(sin(wavetimer+1)+0.5f*cos((wavetimer+1)/2)));
		glColor4f(0.07f, 0.07f, 0.07f, 0.7f);
		glVertex2i(15, 10);

	for (int i = 0; i < 9; i++) {
		glColor4f(0.1f, 0.1f, 0.6f, 0.7f);
		glVertex2f(19+i*4.2, 15+423*((player->FloatScore-(player->NewLevel-player->Levelgap))/player->Levelgap)+waveheight*(sin(wavetimer+i+2)+0.5f*cos((wavetimer+i+2)/2)));
		glColor4f(0.07f, 0.07f, 0.07f, 0.7f);
		glVertex2f(19+i*4.2, 10);
	}
	glEnd();

	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	glColor4f(0.0f, 0.0f, 1.0f, 0.6f);
	glBegin(GL_LINE_STRIP);
	for (i = 0; i < 11; i++) {
		glVertex2f(11+i*4.2f, 15+423*((player->FloatScore-(player->NewLevel-player->Levelgap))/player->Levelgap)+waveheight*(sin(wavetimer+i)+0.5f*cos((wavetimer+i)/2)));
	}
	glEnd();

	wavetimer+=scalar*3;

	//********DRAW GRID*********
	grid.DrawGrid(&texture[0], float(scalar)); //draw grid and particles that belong to the grid

	if (player->MonitorLevel(float(scalar), &texture[0]) == true)
	{
		for (int i = 0; i < 1000; i++) {
			waterhead.Insert();
		}
	}

	//*****DRAW BLOCKS****
	//glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_LINE_SMOOTH);
	if (block) {
		glEnable(GL_SCISSOR_TEST);
		glBindTexture(GL_TEXTURE_2D, texture[0].texID);
		block->DrawPiece(float(scalar), player->timeelapsed);
		glDisable(GL_SCISSOR_TEST);

	}
	if (nextblock) {  //draw the next piece
		glBindTexture(GL_TEXTURE_2D, texture[0].texID);
		nextblock->DrawPiece(float(scalar), 90);
	}

	grid.DrawOutline(player->BonusTimer);
	player->Score+=grid.BlowUpBlocks(scalar);

	//if (grid.Tetris == true) {
	//	block->hit = false;
	//}
	if (grid.GameOver() == true) {
		if (player->GameOver == false)
			FSOUND_PlaySound(FSOUND_FREE, gameover);
		player->GameOver = true;
		//check if score deserves to be top 10
		for (int i = 0; i < 5; i++) {
			if (player->Score > player->highscore[i]) {
				player->Inputting = true;
				player->InputPlace = i;
				break;
			}
		}
		if (keys[VK_RETURN] && menuselection == 1 && player->Inputting == false) {
			if (player) {
				delete player;
				player = 0;
				player = new playerclass;
			}
			grid.Nuke();
			ReassignBlock();
			keys[VK_RETURN] = false;
		}
		else if (keys[VK_RETURN] && menuselection == 2 && player->Inputting == false) {
			done = true;
			keys[VK_RETURN] = false;
		}
		else if (keys[VK_RETURN] && player->Inputting) {
			if (player->InputString[2] != 0) {
				player->Inputting = false;
				for (int i = 4; i > player->InputPlace; i--) {
				player->highscorename[i][0] = player->highscorename[i-1][0];
				player->highscorename[i][1] = player->highscorename[i-1][1];
				player->highscorename[i][2] = player->highscorename[i-1][2];
				player->highscore[i] = player->highscore[i-1];
				}

				player->highscorename[player->InputPlace][0] = player->InputString[0];
				player->highscorename[player->InputPlace][1] = player->InputString[1];
				player->highscorename[player->InputPlace][2] = player->InputString[2];
				player->highscore[player->InputPlace] = player->Score;
				player->Score = 0;	
				}
			keys[VK_RETURN] = false;
		}

		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);	
		glEnable(GL_TEXTURE_2D);

		if (player->Inputting) {
			keys[VK_UP] = false;
			keys[VK_DOWN] = false;
			glDisable(GL_TEXTURE_2D);

			glColor4f(0.0f, 0.0f, 0.0f, 0.5f);
			glBegin(GL_QUADS);
				glVertex2i(60, 370);
				glVertex2i(580, 370);
				glVertex2i(580, 180);
				glVertex2i(60, 180);
			glEnd();
			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
			glBegin(GL_LINE_LOOP);
				glVertex2i(60, 370);
				glVertex2i(580, 370);
				glVertex2i(580, 180);
				glVertex2i(60, 180);
			glEnd();

			if (player->InputCount < 3)	{
			glColor4f(1.0f, 1.0f, 1.0f, 0.5f+0.5f*sinf(3.0f*player->timeelapsed));
			glBegin(GL_LINES);
				glVertex2i(293+player->InputCount*13, 200);
				glVertex2i(293+player->InputCount*13+10, 200);
			glEnd();
			}

		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glEnable(GL_TEXTURE_2D);
		texture[2].glPrint(1.1f, 75, 300, 0, "You made it to the top five!");
		texture[2].glPrint(1.1f, 195, 250, 0, "Enter ititials");
		texture[2].glPrint(1.0f, 290, 200, 0, "%c", player->InputString[0]);
		texture[2].glPrint(1.0f, 303, 200, 0, "%c", player->InputString[1]);
		texture[2].glPrint(1.0f, 316, 200, 0, "%c", player->InputString[2]);
		}

		glDisable(GL_TEXTURE_2D);

		glBlendFunc(GL_SRC_ALPHA,GL_ONE);	
		glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
		glBegin(GL_QUADS);
			glVertex2f(400.0f, SelectionBoxY+15.0f);
			glVertex2f(560.0f, SelectionBoxY+15.0f);
			glVertex2f(560.0f, SelectionBoxY-15.0f);
			glVertex2f(400.0f, SelectionBoxY-15.0f);
		glEnd();
		glEnable(GL_LINE_SMOOTH);
		glLineWidth(4.0f);
		glColor4f(Red/2.0f, Green/2.0f, Blue/2.0f, 1.0f);
		glBegin(GL_LINE_LOOP);
			glVertex2f(400.0f, SelectionBoxY+15.0f);
			glVertex2f(560.0f, SelectionBoxY+15.0f);
			glVertex2f(560.0f, SelectionBoxY-15.0f);
			glVertex2f(400.0f, SelectionBoxY-15.0f);
		glEnd();
		glDisable(GL_LINE_SMOOTH);

		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_TEXTURE_2D);

		if (menuselection == 1) {
			glColor3f(0.75f+sinf(player->timeelapsed*3)*0.25f, 0.0f, 0.0f);
			SelectionBoxY -= ((SelectionBoxY-152.0f)*float(scalar)*8.0f);
		}
		else
			glColor3f(1.0f, 1.0f, 1.0f);
		texture[2].glPrint(1.0f, 415, 140, 0, "New Game");
		if (menuselection == 2) {
			glColor3f(0.75f+sinf(player->timeelapsed*3)*0.25f, 0.0f, 0.0f);
			SelectionBoxY -= ((SelectionBoxY-103.0f)*float(scalar)*8.0f);
		}
		else
			glColor3f(1.0f, 1.0f, 1.0f);
		texture[2].glPrint(1.0f, 415, 90, 0, "Quit");
		glDisable(GL_TEXTURE_2D);

		if (keys[VK_UP] && menuselection == 2) {
			FSOUND_PlaySound(FSOUND_FREE, menuselect);
			keys[VK_UP] = false;
			menuselection--;
		}
		else if (keys[VK_DOWN] && menuselection == 1) {
			FSOUND_PlaySound(FSOUND_FREE, menuselect);
			keys[VK_DOWN] = false;
			menuselection++;
		}

	}

	grid.Checktetris(); //check to see if a tetris is on the board
	if (player->untilstart < 0.0f) {
	if (block)  {
	block->HandleInput(keys[VK_LEFT], keys[VK_RIGHT], keys[VK_DOWN], keys[VK_UP], grid, flip, stomp, scalar);
		if (block->DetectCollison(float(scalar), grid, player->TotalSpeed, keys[VK_SPACE]) == true) {
//	if (keys[VK_SPACE] == true) {
		delete block; block = 0;
		if (grid.GameOver() == false) {
			ReassignBlock();
		}
		keys[VK_DOWN] = false; //prevents blocks from going down at start
		//keys[VK_LEFT] = false; //prevents from going left
		//keys[VK_RIGHT] = false; //...right
	}
	}
	}
	else if (player->untilstart < 3.0f && player->untilstart > 2.333f) {
		glEnable(GL_TEXTURE_2D);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f-((3.0f-player->untilstart)*1.5f));
		texture[2].glPrint(1.5f, 200.0f+rand()%20, 220.0f+rand()%20, 0, "READY...?");
		glDisable(GL_TEXTURE_2D);
	}
	else if (player->untilstart < 2.333f && player->untilstart > 1.5f) {
		glEnable(GL_TEXTURE_2D);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f-((2.333f-player->untilstart)*1.5f));
		texture[2].glPrint(1.5f, 200.0f+rand()%20, 220.0f+rand()%20, 0, "SET...?");
		glDisable(GL_TEXTURE_2D);
	}
	else if (player->untilstart < 1.0f && player->playedfightsound == false) {
		FSOUND_PlaySound(FSOUND_FREE, fight);
		player->playedfightsound = true;
	}
	else if (player->untilstart < 1.0f) {
		glEnable(GL_TEXTURE_2D);
		glColor4f(1.0f, 1.0f, 1.0f, player->untilstart*2.0f);
		texture[2].glPrint(1.5f, 200, 220, 0, "GO!!!!!!!");
		if (block == 0)
		{
			ReassignBlock();
		}
		if (FMUSIC_IsPlaying(mainsong) == false)
			FMUSIC_PlaySong(mainsong);
		//}
		glDisable(GL_TEXTURE_2D);
	}

	//******DRAWING CODE END, NON-DRAWING CODE*********

	player->timeelapsed += float(scalar);
	if (player->untilstart > 0.0f)
		player->untilstart-=float(scalar);

	if (FMUSIC_IsFinished(mainsong)) {
		NewMusic();
	}

	return TRUE;										
}

GLvoid KillGLWindow(GLvoid)							
{
	if (fullscreen)									
	{
		ChangeDisplaySettings(NULL,0);				
		ShowCursor(TRUE);							
	}

	if (hRC)										
	{
		if (!wglMakeCurrent(NULL,NULL))				
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;									
	}

	if (hDC && !ReleaseDC(hWnd,hDC))				
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;									
	}

	if (hWnd && !DestroyWindow(hWnd))				
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;									
	}

	if (!UnregisterClass("OpenGL",hInstance))		
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;								
	}
}
 
BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{
	GLuint		PixelFormat;			
	WNDCLASS	wc;						
	DWORD		dwExStyle;				
	DWORD		dwStyle;			
	RECT		WindowRect;			
	WindowRect.left=(long)0;			
	WindowRect.right=(long)width;		
	WindowRect.top=(long)0;				
	WindowRect.bottom=(long)height;		

	fullscreen=fullscreenflag;			

	hInstance			= GetModuleHandle(NULL);				
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	
	wc.lpfnWndProc		= (WNDPROC) WndProc;					
	wc.cbClsExtra		= 0;									
	wc.cbWndExtra		= 0;									
	wc.hInstance		= hInstance;							
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			
	wc.hbrBackground	= NULL;									
	wc.lpszMenuName		= NULL;								
	wc.lpszClassName	= "OpenGL";								

	if (!RegisterClass(&wc))									
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											
	}
	
	if (fullscreen)											
	{
		DEVMODE dmScreenSettings;								
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);		
		dmScreenSettings.dmPelsWidth	= width;				
		dmScreenSettings.dmPelsHeight	= height;			
		dmScreenSettings.dmBitsPerPel	= bits;					
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{	
			if (MessageBox(NULL,"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?","glTetris",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{
				fullscreen=FALSE;		
			}
			else
			{
				MessageBox(NULL,"Program Will Now Close.","ERROR",MB_OK|MB_ICONSTOP);
				return FALSE;									
			}
		}
	}

	if (fullscreen)												
	{
		dwExStyle=WS_EX_APPWINDOW;								
		dwStyle=WS_POPUP;										
		ShowCursor(FALSE);										
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			
		dwStyle=WS_OVERLAPPEDWINDOW;							
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		

	// Create The Window
	if (!(hWnd=CreateWindowEx(	dwExStyle,							
								"OpenGL",							
								title,								
								dwStyle |							
								WS_CLIPSIBLINGS |					
								WS_CLIPCHILDREN,					
								0, 0,								
								WindowRect.right-WindowRect.left,	
								WindowRect.bottom-WindowRect.top,	
								NULL,							
								NULL,								
								hInstance,							
								NULL)))								
	{
		KillGLWindow();								
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	static	PIXELFORMATDESCRIPTOR pfd=			
	{
		sizeof(PIXELFORMATDESCRIPTOR),				
		1,										
		PFD_DRAW_TO_WINDOW |					
		PFD_SUPPORT_OPENGL |					
		PFD_DOUBLEBUFFER,						
		PFD_TYPE_RGBA,							
		bits,									
		0, 0, 0, 0, 0, 0,						
		0,									
		0,									
		0,									
		0, 0, 0, 0,							
		16,								
		0,									
		0,									
		PFD_MAIN_PLANE,						
		0,									
		0, 0, 0							
	};
	
	if (!(hDC=GetDC(hWnd)))					
	{
		KillGLWindow();						
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;						
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	
	{
		KillGLWindow();								
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		
	{
		KillGLWindow();								
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	if (!(hRC=wglCreateContext(hDC)))				
	{
		KillGLWindow();								
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	if(!wglMakeCurrent(hDC,hRC))					
	{
		KillGLWindow();								
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	ShowWindow(hWnd,SW_SHOW);						
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

		case WM_CHAR:
		{
			if (player->Inputting && (((char)wParam > 96 && (char)wParam < 123) || keys[VK_BACK])) {
				if (keys[VK_BACK] && player->InputCount > 0) {
					player->InputString[player->InputCount-1] = 0;
					player->InputCount-=1;
				}
				else if (player->InputCount < 3 && keys[VK_BACK] == false) {
				player->InputString[player->InputCount] = (char)wParam-32;
				player->InputCount++;
				}
			//else
			//	player->Inputting = false;
			}
			return 0;
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

	 //Ask The User Which Screen Mode They Prefer
	//if (MessageBox(NULL,"Would You Like To Run In Fullscreen Mode?", "Start FullScreen?",MB_YESNO|MB_ICONQUESTION)==IDNO)
	//{
	//	fullscreen=FALSE;							// Windowed Mode
	//}

	// Create Our OpenGL Window
	if (!CreateGLWindow("glTetris: Brian Washburn",640,480,16,fullscreen))
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
				if (keys[VK_ESCAPE] && grid.GameOver() == false)				
				{
					//done=TRUE;
					if (TitleScreen == false)
					{
						TitleScreen = true;
						glEnable(GL_TEXTURE_2D); //enable texturing before drawtitlescreen()
						if (FMUSIC_GetMasterVolume(mainsong) == 200)
							FMUSIC_SetMasterVolume(mainsong, 75);

					}
					keys[VK_ESCAPE] = false;
				}
				else								
				{
					if (!player->Paused) {

					if (TitleScreen)
						DrawTitleScreen();
					else if (HighScoreBoard)
						DrawHighScoreBoard();
					else 
						DrawGLScene();

					SwapBuffers(hDC);
					scalar = (timeGetTime() - begintime)/1000.0;
					}
				}
			}

			if (keys[VK_RETURN]) {
				if (!player->Paused && TitleScreen == false && HighScoreBoard == false)
				{
					glEnable(GL_TEXTURE_2D);
					glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
					texture[2].glPrint(2.0f, 230, 220, 0, "PAUSED");
					glDisable(GL_TEXTURE_2D);
					SwapBuffers(hDC);
					player->Paused = true;
				}
				else
					player->Paused = false;
				keys[VK_RETURN] = false;
			}

			if (keys[VK_HOME]) {
				NewMusic();
				keys[VK_HOME] = false;
			}

			if (keys[VK_END]) {
				CaptureScreen();
				keys[VK_UP] = false;
			}

			if (keys[VK_INSERT]) {
				texture[0].SCREENSHOT_WriteTGA("Picture.bmp", 640, 480);
					keys[VK_INSERT] = false;
			}

			if (keys[VK_SPACE]) {
				player->Score+= 20;
				player->FloatScore+=20;
			}
		}
	}

	// Shutdown
	if (block)
		delete block;
	if (nextblock)
		delete nextblock;
	if (player)
		delete player;

	waterhead.DeleteAll();

	texture[2].KillFont();
	FSOUND_Sample_Free(stomp);
	FSOUND_Sample_Free(flip);
	FSOUND_Sample_Free(boxhit);
	FSOUND_Sample_Free(fight);
	FSOUND_Sample_Free(gameover);
	FSOUND_Sample_Free(menuselect);
	FMUSIC_FreeSong(mainsong);
	FSOUND_Close();
	glDeleteTextures(1, texture[0].texture);
	glDeleteTextures(1, texture[1].texture);
	glDeleteTextures(1, texture[2].texture);
	glDeleteTextures(1, texture[3].texture);
	glDeleteTextures(1, texture[4].texture);
	glDeleteTextures(1, texture[5].texture);
	glDeleteTextures(1, texture[6].texture);
	KillGLWindow();								
	return (msg.wParam);							
}

void NewMusic()
{
	FMUSIC_FreeSong(mainsong);
	int songnumber;
	do {
		if (Music[0] && Music[1] && Music[2] && Music[3] && Music[4] && Music[5] && Music[6] && Music[7]) {
			Music[0] = false;
			Music[1] = false;
			Music[2] = false;
			Music[3] = false;
			Music[4] = false;
			Music[5] = false;
			Music[6] = false;
			Music[7] = false;
		}
		songnumber = rand()%8;
	} while (Music[songnumber] == true);

	if (songnumber == 0) {
		mainsong = FMUSIC_LoadSong("Data/s1.dat");
		Music[0] = true;
	}
	else if (songnumber == 1) {
		mainsong = FMUSIC_LoadSong("Data/s2.dat");
		Music[1] = true;
	}
	else if (songnumber == 2) {
		mainsong = FMUSIC_LoadSong("Data/s3.dat");
		Music[2] = true;
	}
	else if (songnumber == 3) {
		mainsong = FMUSIC_LoadSong("Data/s4.dat");
		Music[3] = true;
	}
	else if (songnumber == 4) {
		mainsong = FMUSIC_LoadSong("Data/s5.dat");
		Music[4] = true;
	}
	else if (songnumber == 5) {
		mainsong = FMUSIC_LoadSong("Data/s6.dat");
		Music[5] = true;
	}
	else if (songnumber == 6) {
		mainsong = FMUSIC_LoadSong("Data/s7.dat");
		Music[6] = true;
	}
	else if (songnumber == 7) {
		mainsong = FMUSIC_LoadSong("Data/s8.dat");
		Music[7] = true;
	}


	//FMUSIC_SetLooping(mainsong,false);
	FMUSIC_SetMasterVolume(mainsong, 200);
	FMUSIC_PlaySong(mainsong);
}

void ReassignBlock()
{
		block = nextblock;  //make nextblock the current block
		block->Restart(false);
		int temp = rand()%30;
		if (temp < 6)
			nextblock = new stick;
		else if (temp < 10)
			nextblock = new square;
		else if (temp < 14)
			nextblock = new curveright;
		else if (temp < 18)
			nextblock = new curveleft;
		else if (temp < 22)
			nextblock = new longcurveright;
		else if (temp < 26)
			nextblock = new longcurveleft;
		else if (temp < 30)
			nextblock = new goodpiece;
		nextblock->Restart(true);
}