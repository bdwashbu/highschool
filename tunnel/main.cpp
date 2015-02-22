// add opengl32.lib glu32.lib winmm.lib to linker
#include <stdarg.h>
#include <stdio.h>			

#include <windows.h>		
#include <gl\gl.h>			
#include <gl\glu.h>	
#include <stdlib.h>		
#include <time.h>
#include <math.h>
#include "texture.h"
//#include <gl\glaux.h>	only for bitmaps
//#include <stdlib.h> include for rand() and seeding

#pragma comment( lib, "opengl32.lib" )
#pragma comment( lib, "glu32.lib" )
#pragma comment( lib, "winmm.lib" )

HDC			hDC=NULL;		
HGLRC		hRC=NULL;		
HWND		hWnd=NULL;	
HINSTANCE	hInstance;		

bool keys[256], active = true, fullscreen = true;		
double scalar = 0, begintime = 0;
textureclass texture;

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

	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f); // 3D
	//gluOrtho2D(0,640,0,480); //for 2D

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

int InitGL(GLvoid)									
{
	srand( time(NULL) );
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glHint(GL_FOG_HINT, GL_NICEST);
	glEnable(GL_LINE_SMOOTH);
	//glEnable(GL_POLYGON_SMOOTH);

	glShadeModel(GL_SMOOTH);							
	glClearColor(1.0f, 1.0f, 1.0f, 0.5f);				
	glClearDepth(1.0f);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	//glCullFace(GL_FRONT);
	//glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LEQUAL);	
	//glEnable(GL_BLEND);
	glEnable(GL_FOG);
	glFogi(GL_FOG_MODE, GL_EXP2);
	GLfloat fogColor[4] = {1.0f, 1.0f, 1.0f, 1.0f};
	glFogfv(GL_FOG_COLOR, fogColor);
	glFogf(GL_FOG_DENSITY, 0.035f);
	glFogf(GL_FOG_START, 20.0f);
	glFogf(GL_FOG_END, 70.0f);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	texture.LoadTGA(0, "Data/ground.tga", true);
	glBindTexture(GL_TEXTURE_2D, texture.texID);
	glEnable(GL_TEXTURE_2D);
	glMatrixMode(GL_PROJECTION);						

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	//glEnable(GL_ALPHA_TEST);
	//glAlphaFunc(GL_NOTEQUAL,0.0f);

	return TRUE;										
}

float ApplySinX(int i, float time)
{
	//float returnvalue = (3*sinf(i/40.0f*(2.0f*3.14159)+time));//+(6*cosf(i/40.0f/(3.0f*3.14159)+time));
	const float radian = (i/40.0f*(2.0f*3.14159f));
	float returnvalue = sinf(radian/2.0f+time)+10.0*cosf(radian/4.0f+time);//+6*cosf((radian+time)/10.0f);
	return returnvalue;
}

float ApplySinY(int i, float time)
{
	//float returnvalue = (3*sinf(i/40.0f*(2.0f*3.14159)+time));//+(6*cosf(i/40.0f/(3.0f*3.14159)+time));
	const float radian = (i/40.0f*(2.0f*3.14159f));
	float returnvalue = 3.0f*sinf(radian/8.0f+time)+4.0f*cosf(radian/2.0f+time)*sinf(radian/4.0f+time);//+6*cosf((radian+time)/10.0f);
	return returnvalue;
}

float ApplyCos(int i, float time)
{
	//float returnvalue = (3*sinf(i/40.0f*(2.0f*3.14159)+time));//+(6*cosf(i/40.0f/(3.0f*3.14159)+time));
	const float radian = (i/40.0f*(2.0f*3.14159f));
	float returnvalue = 18.0f*cosf((radian*2.0f+time));//+6*cosf((radian+time)/10.0f);
	return returnvalue;
}


int DrawGLScene(GLvoid)									
{
	static float time2 = 0.0f;
	static float time = 0.0f;
	static float time3 = 0.0f;
	static float time4 = 0.0f;
	static float time5 = 0.0f;
	static float XCounter = 0.0f;
	static float YCounter = 0.0f;
	XCounter+=scalar*1.5f;
	YCounter+=scalar*0.5f;


	time2+=scalar*0.8f;
	if (time2 > 1.0f)
		time2 = 0.0f;
	time+=scalar*0.15f;
	if (time > 1.0f)
		time = 0.0f;
	time3+=scalar*1.5f;
	if (time3 > 6.29f)
		time3 = 0.0f;
	time4+=scalar*5.0f;
	if (time4 > 50.0f)
		time4 = 0.0f;
	time5+=scalar*0.5f;
	if (time5 > 6.29f)
		time5 = 0.0f;


	//if (time4 > 25.0f)
	//	glFogf(GL_FOG_DENSITY, 0.03f+0.5f*((time4-25.0f)/25.0f));

	begintime = timeGetTime();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glMatrixMode(GL_PROJECTION);						
	glLoadIdentity();								

	gluPerspective(75+35.0f*sin(time5),(GLfloat)640.0f/(GLfloat)480.0f,0.1f,100.0f); // 3D
	//gluOrtho2D(0,640,0,480); //for 2D
	glRotatef(time*360.0f, 0.0f, 0.0f, 1.0f);
	glRotatef(atanf((ApplySinX(3, XCounter)-ApplySinX(0,XCounter))/3.0f)*(180.0f/3.14159f), 0.0f, 1.0f, 0.0f);
	glRotatef(atanf((ApplySinY(0,YCounter)-ApplySinY(7, YCounter))/7.0f)*(180.0f/3.14159f), 1.0f, 0.0f, 0.0f);

	glTranslatef(-ApplySinX(0,XCounter), -ApplySinY(0,YCounter), 0.0f);

	//glMatrixMode(GL_MODELVIEW);							

	glEnable(GL_TEXTURE_2D);
	float CircleSubdivision = 25.0f;
	float StripLength = 50.0f;
	glColor3f(1.0f, 1.0f, 1.0f);
	for (float z = 0.0f; z < CircleSubdivision; z++) {
	glBegin(GL_TRIANGLE_STRIP);
	for(float i = 0.0f; i < StripLength; i++) {
		glTexCoord2f(z/CircleSubdivision*4.0f, i/CircleSubdivision+time2*4.0f);
		glVertex3f((2.0f+0.3f*sinf(z/CircleSubdivision*(6.0f*3.14159f)+time3))*sinf(z/CircleSubdivision*(2.0f*3.14159f))+ApplySinX(i,XCounter), (2.0f+0.3f*sinf(z/CircleSubdivision*(6.0f*3.14159f)+time3))*cosf(z/CircleSubdivision*(2.0f*3.14159f))+ApplySinY(i,YCounter), -i);
		glTexCoord2f((z+1)/CircleSubdivision*4.0f, i/CircleSubdivision+time2*4.0f);
		glVertex3f((2.0f+0.3f*sinf((z+1)/CircleSubdivision*(6.0f*3.14159f)+time3))*sinf((z+1)/CircleSubdivision*(2.0f*3.14159f))+ApplySinX(i,XCounter), (2.0f+0.3f*sinf((z+1)/CircleSubdivision*(6.0f*3.14159f)+time3))*cosf((z+1)/CircleSubdivision*(2.0f*3.14159f))+ApplySinY(i,YCounter), -i);
	}
	glEnd();
	}
	glEnable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glColor4f(1.0f, 1.0f, 1.0f, 0.2f);
	for (z = 0.0f; z < CircleSubdivision; z++) {
	glBegin(GL_LINE_STRIP);
	for(float i = 0.0f; i < StripLength-10.0f; i++) {
		glVertex3f((2.0f+0.3f*sinf(z/CircleSubdivision*(6.0f*3.14159f)+time3))*sinf(z/CircleSubdivision*(2.0f*3.14159f))+ApplySinX(i,XCounter), (2.0f+0.3f*sinf(z/CircleSubdivision*(6.0f*3.14159f)+time3))*cosf(z/CircleSubdivision*(2.0f*3.14159f))+ApplySinY(i,YCounter), -i);
		glVertex3f((2.0f+0.3f*sinf((z+1)/CircleSubdivision*(6.0f*3.14159f)+time3))*sinf((z+1)/CircleSubdivision*(2.0f*3.14159f))+ApplySinX(i,XCounter), (2.0f+0.3f*sinf((z+1)/CircleSubdivision*(6.0f*3.14159f)+time3))*cosf((z+1)/CircleSubdivision*(2.0f*3.14159f))+ApplySinY(i,YCounter), -i);
	}
	glEnd();
	}
	glDisable(GL_BLEND);

	if (keys[VK_END])
		CaptureScreen();


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
			if (MessageBox(NULL,"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?","NeHe GL",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
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
	if (MessageBox(NULL,"Would You Like To Run In Fullscreen Mode?", "Start FullScreen?",MB_YESNO|MB_ICONQUESTION)==IDNO)
	{
		fullscreen=FALSE;							// Windowed Mode
	}

	// Create Our OpenGL Window
	if (!CreateGLWindow("NeHe's OpenGL Framework",640,480,16,fullscreen))
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
				if (keys[VK_ESCAPE])				
				{
					done=TRUE;						
				}
				else								
				{
					scalar = (timeGetTime()-begintime)/1000;
					DrawGLScene();					
					SwapBuffers(hDC);				
				}
			}
			if (keys[VK_END])
			{
				keys[VK_END] = false;
			}	

			if (keys[VK_F1])						
			{
				keys[VK_F1]=FALSE;					
				KillGLWindow();						// Kill Our Current Window
				fullscreen=!fullscreen;				// Toggle Fullscreen / Windowed Mode
				// Recreate Our OpenGL Window
				if (!CreateGLWindow("NeHe's OpenGL Framework",640,480,16,fullscreen))
				{
					return 0;						// Quit If Window Was Not Created
				}
			}
		}
	}

	// Shutdown commands
	KillGLWindow();	
	//delete commands go here
	return (msg.wParam);							
}
