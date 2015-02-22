#include <windows.h>		
#include <math.h>
#include <iostream.h>		
#include <gl\gl.h>			
#include <gl\glu.h>
#include <time.h>


//#include "texture.h"
//#include "timer.h"
#include "grid.h"
#include "ball.h"
#include "timer.h"
const MAX_XGRID = 160;
const MAX_YGRID = 120;

gridclass grid[MAX_XGRID][MAX_YGRID];
ballclass ball[20];
timerclass timer2;

HDC			hDC=NULL;		
HGLRC		hRC=NULL;		
HWND		hWnd=NULL;		
HINSTANCE	hInstance;		

bool 	done=FALSE;								// Bool Variable To Exit Loop
bool	keys[256];			
bool	active=TRUE;		
bool	fullscreen=TRUE;
int activemetaballs = 10;
float escapekey;

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM lParam);	

GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		
{
	if (height==0)										
	{
		height=1;										
	}

	glViewport(0,0,width,height);						

	glMatrixMode(GL_PROJECTION);						
	glLoadIdentity();									

	// Calculate The Aspect Ratio Of The Window
	//gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);
	gluOrtho2D(0, 320, 0, 240);
	glMatrixMode(GL_MODELVIEW);							
	glLoadIdentity();									
}

void InitGL(GLvoid)										
{
	gluOrtho2D(0, 320, 0, 240);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//glEnable(GL_BLEND);	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//	glEnable(GL_POINT_SMOOTH); glHint(GL_POINT_SMOOTH_HINT, GL_FASTEST);
	glShadeModel(GL_FLAT);
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				
	glPointSize(0.9f);
	grid.circle = true;
	srand( time(NULL) );


	for (int a = 0; a < MAX_XGRID; a++)
	{
	for (int b = 0; b < MAX_YGRID; b++)
	{
		grid[a][b].AssignGrid(a,b,2,2);
	}
	}

	for (a = 0; a < 20; a++)
		ball[a].SetAttributes();
	ball[0].red = 1.0f;		ball[1].red = 0.0f;		ball[2].red = 0.0f;
	ball[0].green = 0.0f;	ball[1].green = 1.0f;	ball[2].green = 0.0f;
	ball[0].blue = 0.0f;	ball[1].blue = 0.0f;	ball[2].blue = 1.0f;
	ball[3].red = 1.0f;		ball[4].red = 0.0f;		ball[5].red = 0.0f;
	ball[3].green = 0.0f;	ball[4].green = 1.0f;	ball[5].green = 0.0f;
	ball[3].blue = 0.0f;	ball[4].blue = 0.0f;	ball[5].blue = 1.0f;
	ball[6].red = 1.0f;		ball[7].red = 0.0f;		ball[8].red = 0.0f;
	ball[6].green = 0.0f;	ball[7].green = 1.0f;	ball[8].green = 0.0f;
	ball[6].blue = 0.0f;	ball[7].blue = 0.0f;	ball[8].blue = 1.0f;
	ball[9].red = 1.0f;		ball[10].red = 0.0f;	ball[11].red = 0.0f;
	ball[9].green = 0.0f;	ball[10].green = 1.0f;	ball[11].green = 0.0f;
	ball[9].blue = 0.0f;	ball[10].blue = 0.0f;	ball[11].blue = 1.0f;
	ball[12].red = 1.0f;	ball[13].red = 0.0f;	ball[14].red = 0.0f;
	ball[12].green = 0.0f;	ball[13].green = 1.0f;	ball[14].green = 0.0f;
	ball[12].blue = 0.0f;	ball[13].blue = 0.0f;	ball[14].blue = 1.0f;
	ball[15].red = 1.0f;	ball[16].red = 0.0f;	ball[17].red = 0.0f;
	ball[15].green = 0.0f;	ball[16].green = 1.0f;	ball[17].green = 0.0f;
	ball[15].blue = 0.0f;	ball[16].blue = 0.0f;	ball[17].blue = 1.0f;
	ball[18].red = 1.0f;	ball[19].red = 0.0f;	
	ball[18].green = 0.0f;	ball[19].green = 1.0f;	
	ball[18].blue = 0.0f;	ball[19].blue = 0.0f;	



}

void DrawGLScene(GLvoid)									
{
	timer2.begintime = timeGetTime();
	glClear(GL_COLOR_BUFFER_BIT); // | GL_DEPTH_BUFFER_BIT);

	float temp = 0;
	bool temp2;

	//calculation call
	int a,b,c,d;
	for (c = 0; c < activemetaballs; c++)
	{
	for (a = (ball[c].X/2)-7*ball[c].size; a < (ball[c].X/2)+7*ball[c].size; a++)
	{
	for (b = (ball[c].Y/2)-7*ball[c].size; b < (ball[c].Y/2)+7*ball[c].size; b++)
	{
		temp2 = false;
		for (d = 0; d < activemetaballs; d++)
			temp+=(1.0f / ((grid[a][b].leftx - ball[d].X)*(grid[a][b].leftx - ball[d].X) + (grid[a][b].bottomy - ball[d].Y)*(grid[a][b].bottomy - ball[d].Y)))*ball[d].size;
		if (temp >= 0.012)
		{
		grid[a][b].red = 0; grid[a][b].green = 0; grid[a][b].blue = 0;
		for (d = 0; d < activemetaballs; d++)
		{
			grid[a][b].red += (1.0f / ((grid[a][b].leftx - ball[d].X)*(grid[a][b].leftx - ball[d].X) + (grid[a][b].bottomy - ball[d].Y)*(grid[a][b].bottomy - ball[d].Y)))*1000*ball[d].red;
			grid[a][b].green += (1.0f / ((grid[a][b].leftx - ball[d].X)*(grid[a][b].leftx - ball[d].X) + (grid[a][b].bottomy - ball[d].Y)*(grid[a][b].bottomy - ball[d].Y)))*1000*ball[d].green;
			grid[a][b].blue += (1.0f / ((grid[a][b].leftx - ball[d].X)*(grid[a][b].leftx - ball[d].X) + (grid[a][b].bottomy - ball[d].Y)*(grid[a][b].bottomy - ball[d].Y)))*1000*ball[d].blue;
		}

		temp2 = true;
		if (grid.circle)
		{
			grid[a][b].point1 = true;

		}
		else if (grid.torus)
		{
		if (temp <= 0.025)
		{
			grid[a][b].point1 = true;
		}
		}
		}
		else
			if (temp2 == true)
				break;


		temp = 0;
	}
	}
	}

	//rendering call
	for (a = 0; a < 160; a++)
	{
	for (b = 0; b < 120; b++)
	{
			glBegin(GL_POINTS);
			if (grid[a][b].point1)
			{	
				grid[a][b].point1 = false;
				grid[a][b].RenderGrid();
			}
			glEnd();

	}
	}


	for (int i = 0; i < activemetaballs; i++)
		ball[i].BallMove(timer2.scalar);

	escapekey+=timer2.scalar;

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
		16,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		8,											// 16Bit Z-Buffer (Depth Buffer)  
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
	InitGL();

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
			if (escapekey > 2)
				done = true;
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

		case WM_MOUSEMOVE:
		{
		//if (escapekey > 2)
		//	done = true;
		break;
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

//	 Ask The User Which Screen Mode They Prefer
//	if (MessageBox(NULL,"Would You Like To Run In Fullscreen Mode?", "Start FullScreen?",MB_YESNO|MB_ICONQUESTION)==IDNO)
//	{
//		fullscreen=FALSE;							// Windowed Mode
//	}

	// Create Our OpenGL Window
	if (!CreateGLWindow("Metaballs:Brian Washburn",320,240,16,fullscreen))
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
					//done=TRUE;						// ESC Signalled A Quit
				}
				else								// Not Time To Quit, Update Screen
				{
					DrawGLScene();					// Draw The Scene
					SwapBuffers(hDC);				// Swap Buffers (Double Buffering)
					timer2.scalar = (timeGetTime() - timer2.begintime)/1000.0f;
				}
			}

			if (keys[VK_RETURN])
			{
				if (grid.circle)
				{
					grid.circle = false;
					grid.torus = true;
				}
				else
				{
					grid.circle = true;
					grid.torus = false;
				}
				keys[VK_RETURN] = false;
			}

			if (keys['A'])
			{
				if (activemetaballs < 19)
					activemetaballs++;
				keys['A'] = false;
			}

			if (keys['S'])
			{
				if (activemetaballs > 1)
					activemetaballs--;
				keys['S'] = false;
			}


		}
	}

	// Shutdown
	KillGLWindow();									// Kill The Window
	return (msg.wParam);							// Exit The Program
}
