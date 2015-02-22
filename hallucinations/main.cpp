#include <windows.h>		
#include <gl\gl.h>			
#include <gl\glu.h>			
#include <time.h>
#include <math.h>
//#include <gl\glaux.h>	only for bitmaps
//#include <stdlib.h> include for rand() and seeding

#pragma comment( lib, "opengl32.lib" )
#pragma comment( lib, "glu32.lib" )
#pragma comment( lib, "winmm.lib" )

HDC			hDC=NULL;		
HGLRC		hRC=NULL;		
HWND		hWnd=NULL;	
HINSTANCE	hInstance;
// HMENU		menu = NULL;	

bool keys[256], active = true, fullscreen = true;		
double scalar = 0, begintime = 0;
double offset = 0.0;
bool backwards = true;
double Level = 4.0;
float LineWidth = 2.5f;

//unsigned int texture[1]; //space for textures

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

	//gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f); // 3D
	gluOrtho2D(0,640,0,480); //for 2D

	glMatrixMode(GL_MODELVIEW);							
	glLoadIdentity();									
}

int InitGL(GLvoid)									
{
	srand( time(NULL) );
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glShadeModel(GL_SMOOTH);							
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				
	glClearDepth(1.0f);	
	glPointSize(5.0f);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POINT_SMOOTH);
	glScissor(0,0,320,240);
	glDepthMask(0);
	glLineWidth(2.5f);
	glEnable(GL_SCISSOR_TEST);

	//Texture loading Prototype
    //glGenTextures(1, &texture[0]);
	//glBindTexture(GL_TEXTURE_2D, texture[0]);
	//LoadTGA("Data/logo.tga", true);


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	//glEnable(GL_ALPHA_TEST);
	//glAlphaFunc(GL_NOTEQUAL,0.0f);

	return TRUE;										
}

int DrawGLScene(GLvoid)									
{
	begintime = timeGetTime();
	glClear(GL_COLOR_BUFFER_BIT);	

	glEnable(GL_SCISSOR_TEST);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	for (int y = 1; y < Level+1; y++) {
		for (int x = 1; x < Level+1; x++) {
			glScissor((x-1)*(int)(640.0/Level),(y-1)*(int)(480.0/Level),(int)(640.0/Level),(int)(480.0/Level));
			for (double i = 0.0; i < (640.0/Level)/10.0; i++) {
				double X = x*(640.0/Level)-(640.0/Level/2.0);
				double Y = y*(480.0/Level)-(480.0/Level/2.0);
				glBegin(GL_LINE_LOOP);
				if (backwards) {
					glVertex2d(X-i*10.0-offset, Y);
					glVertex2d(X, Y+i*10.0+offset);			
					glVertex2d(X+i*10.0+offset, Y);			
					glVertex2d(X, Y-i*10.0-offset);	
				} else {
					glVertex2d(X-i*10.0+offset, Y);
					glVertex2d(X, Y+i*10.0-offset);					
					glVertex2d(X+i*10.0-offset, Y);				
					glVertex2d(X, Y-i*10.0+offset);				
				}
				glEnd();
			}
		}
	}
	glDisable(GL_SCISSOR_TEST);

	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	glBegin(GL_POINTS);
	glVertex2i(320,240);
	glEnd();

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
								NULL,  // <-- menu goes here								
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

int WINAPI WinMain(	HINSTANCE	hInstance,			
					HINSTANCE	hPrevInstance,		
					LPSTR		lpCmdLine,			
					int			nCmdShow)			
{
	MSG		msg;									// Windows Message Structure
	BOOL	done=FALSE;								// Bool Variable To Exit Loop

	// Ask The User Which Screen Mode They Prefer
	//if (MessageBox(NULL,"Would You Like To Run In Fullscreen Mode?", "Start FullScreen?",MB_YESNO|MB_ICONQUESTION)==IDNO)
	//{
	//	fullscreen=FALSE;							// Windowed Mode
	//}

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
				done=TRUE;							
			}
			else									
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		else										// If There Are No Messages
		{
			if (active)								
			{
				if (keys[VK_ESCAPE])				
				{
					done=TRUE;						
				}
				else								
				{
					scalar = (timeGetTime()-begintime)/1000;
					if (scalar < 1.0)
						offset+=scalar*25.0;
					if (offset > 9.5 && backwards)
						offset = 0.0f;
					else if(offset > -0.5 && !backwards)
						offset = -10.0;
					DrawGLScene();					
					SwapBuffers(hDC);				
				}
				if (keys[VK_UP]) {
					if (Level < 16)
						Level++;
					if (Level == 11)
						Level+=5;
					if (Level == 6)
						Level = 8;
					if (Level == 9)
						Level++;
					keys[VK_UP] = false;
				}
				if (keys[VK_DOWN]) {
					if (Level > 2)
						Level--;
					if (Level == 15)
						Level-=5;
					if (Level == 7)
						Level=5;
					if (Level == 9)
						Level--;
					keys[VK_DOWN] = false;
				}
				if (keys[VK_RIGHT]) {
					LineWidth+=0.3f;
					glLineWidth(LineWidth);
					keys[VK_RIGHT] = false;
				}
				if (keys[VK_LEFT]) {
					LineWidth-=0.3f;
					glLineWidth(LineWidth);
					keys[VK_LEFT] = false;
				}
			}
		}
	}

	// Shutdown commands
	KillGLWindow();
	//glDeleteTextures(1, &texture[0]) //delete texture
	//delete commands go here
	return (msg.wParam);							
}
