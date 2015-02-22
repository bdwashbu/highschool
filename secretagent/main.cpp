#include <windows.h>		
#include <stdio.h>
#include <math.h>
#include <fstream.h>
#include <iostream.h>		
#include <gl\gl.h>			
#include <gl\glu.h>
#include "glext.h"

//#include "walls.h"
#include "agent.h"
#include "lights.h"
#include "texture.h"
#include "output.h"
#include "mouse.h"
#include "timer.h"

HDC			hDC=NULL;		
HGLRC		hRC=NULL;		
HWND		hWnd=NULL;		
HINSTANCE	hInstance;		

bool 	done=FALSE;								// Bool Variable To Exit Loop
bool	keys[256];			
bool	active=TRUE;		
bool	fullscreen=TRUE;

const MAX_WALLS = 100;

lines line[100];
agents agent;
tga tgatexture[5];
output2 output;
mouseinput mouse;
clock2 timer2;
lightmaps light[5];

static charplace = -1;
bool loading;

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
	gluOrtho2D(0, 640, 0, 480);
	glMatrixMode(GL_MODELVIEW);							
	glLoadIdentity();									
}

void InitGL(GLvoid)										
{
	gluOrtho2D(0, 640, 0, 480);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_BLEND);	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_POINT_SMOOTH); glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_LINE_SMOOTH);  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glShadeModel(GL_FLAT);
	glLineWidth(1.4f);
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				
	glPointSize(3.3f);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER,0.1f);
	glScissor(15, 15, 470, 405);

	tgatexture[0].LoadTGA(1,"Data/Font.tga"); tgatexture[0].BuildFont();
	//tgatexture[1].LoadTGA(2,"Data/Arrow.tga");
	//tgatexture[2].LoadTGA(3,"Data/Backround.tga");
	//tgatexture[3].LoadTGA(4,"Data/haha0.lmp");
}

void DrawGLScene(GLvoid)									
{
	timer2.begintime = timeGetTime();
	glClear(GL_COLOR_BUFFER_BIT); // | GL_DEPTH_BUFFER_BIT);

		agent.DrawCharacter();
	
//	for (int i = 0; i < 5; i++)
	//	light[i].DrawLightMap();

	for (int i = 0; i < MAX_WALLS; i++)
	{
	line[i].DrawLine(i);
		if (line[i].moving == true)
		{
			line[i].WallMove(timer2.scalar*3);
			if (agent.CheckAhead(line[i].slope, line[i].yintercept, line[i].linex, line[i].line2x, line[i].liney, line[i].line2y) == true)
				agent.Collision(line[i].slope, line[i].yintercept, true);
		}
	}

	mouse.DrawMouse();

	glEnable(GL_TEXTURE_2D);
	tgatexture[0].glPrint(0.8f, 190, 455, 0, "agentED build 3/19/02");
	tgatexture[0].glPrint(0.6f, 255, 440, 0, "Brian Washburn");

	 //things that will only light up while not saving
	output.SaveButton(mouse.mouse_x, mouse.mouse_y, 580, 630, 62, 78);
	tgatexture[0].glPrint(0.8f, 580, 63, 0, "EXIT");
	glDisable(GL_TEXTURE_2D);


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

		case WM_CHAR:
		{
		//	if (saving == true && keys[VK_RETURN] == false && keys[VK_BACK] == false)
		//	{
		//	if (charplace<9)
		//		charplace++;
		//	output.savefile[charplace] = (char)wParam;
		//	strcpy (output.tempfile, output.savefile);
		//	strncat (output.tempfile, ".dlf", 4);
		//	}
		//	else if (loading == true && keys[VK_RETURN] == false && keys[VK_BACK] == false)
		//	{
		//	if (charplace<9)
		//		charplace++;
		//	output.loadfile[charplace] = (char)wParam;
		//	strcpy (output.tempfile, output.loadfile);
		//	strncat (output.tempfile, ".dlf", 4);

			// *file = *inputtext;
		//	}
			break;
		}

		case WM_LBUTTONDOWN:
		{
		break;
		}

		case WM_LBUTTONUP:
		{
		if (mouse.mouse_x > 580 && mouse.mouse_x < 630 && mouse.mouse_y > 62 && mouse.mouse_y < 78)
			done = true;
		break;
		}

		case WM_RBUTTONDOWN:
		{
		break;
		}

		case WM_RBUTTONUP:
		{
		break;
		}




		case WM_MOUSEMOVE:
		{
	    mouse.mouse_x = LOWORD(lParam);          
		mouse.mouse_y = HIWORD(lParam);
		mouse.mouse_y = 480 - mouse.mouse_y;
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
				if (keys[VK_ESCAPE])				// Was ESC Pressed?
				{
					done=TRUE;						// ESC Signalled A Quit
				}
				else								// Not Time To Quit, Update Screen
				{
					DrawGLScene();					// Draw The Scene
					SwapBuffers(hDC);				// Swap Buffers (Double Buffering)
					timer2.scalar = (timeGetTime() - timer2.begintime)/1000.0f;
				}
			}

			if (keys[VK_UP])
			{
				bool move = true;
				agent.MoveForward(9, timer2.scalar,1);
				for (int i = 0; i < MAX_WALLS; i++)
				{
					if (agent.CheckAhead(line[i].slope, line[i].yintercept, line[i].linex, line[i].line2x, line[i].liney, line[i].line2y) == true)
					{
						agent.Collision(line[i].slope, line[i].yintercept, false);
						move = false;
						if (line[i].door)
							line[i].moving = true;
						break;
					}
				}
				if (move == true)
					agent.Move(line.xoffset, line.yoffset);
				//keys[VK_UP] = false;
			}

			if (keys[VK_DOWN])
			{
				bool move = true;
				agent.MoveForward(9, timer2.scalar,-1);
				for (int i = 0; i < MAX_WALLS; i++)
				{
					if (agent.CheckAhead(line[i].slope, line[i].yintercept, line[i].linex, line[i].line2x, line[i].liney, line[i].line2y) == true)
					{	
						agent.Collision(line[i].slope, line[i].yintercept, false);
						move = false;
						if (line[i].door)
							line[i].moving = true;
						break;
					}
				}
				if (move == true)
					agent.Move(line.xoffset, line.yoffset);
				//keys[VK_UP] = false;
			}


			if (keys[VK_LEFT])
			{
				agent.MoveLeft(timer2.scalar);
				//keys[VK_LEFT] = false;
			}

			if (keys[VK_RIGHT])
			{
				agent.MoveRight(timer2.scalar);
				//keys[VK_RIGHT] = false;
			}

			if (keys[VK_BACK])
			{
			break;
			}
				
			if (keys[VK_RETURN])
			{
				if (loading == true)
				{
				strcpy (output.tempfile,"Maps/");
				strncat (output.tempfile, output.loadfile, strlen(output.loadfile));
				strncat (output.tempfile, ".dlf", 4);

				int temp;
				ifstream fin;
				fin.open(output.tempfile, ios::nocreate);
				if (fin.good())
				{
				for (int i = 0; i < MAX_WALLS; i++)
				{
					fin >> line[i].linex;
					fin >> line[i].line2x;
					fin >> line[i].liney;
					fin >> line[i].line2y;
					fin >> line[i].slope;
					fin >> line[i].yintercept;
					fin >> temp;
					if (temp == 1)
						line[i].door = true;
					else
						line[i].door = false;
					fin >> line[i].movelinex;
					fin >> line[i].moveliney;
					fin >> line[i].moveline2x;
					fin >> line[i].moveline2y;
					fin >> line[i].red;
					fin >> line[i].green;
					fin >> line[i].blue;
				}
				}
				fin.close();

				char finalstring[30];

				strcpy (finalstring,"Maps/");
				strncat (finalstring, output.loadfile, strlen(output.loadfile));
				strncat (finalstring, ".dat", 4);


				fin.open(finalstring, ios::nocreate);
				fin >> temp; fin >> "\n";
				for (int i = 0; i < temp; i++)
				{
				fin.getline(light[i].name,30);
				fin >> light[i].radius;
				fin >> light[i].X;
				fin >> light[i].Y;
				fin >> "\n";
				light[i].active = true;

				//strcpy (finalstring,"Data/");
				//strncat (finalstring, light[i].name, strlen(light[i].name));
				//strncat (finalstring, ".lmp", 4);
				//tgatexture[i+3].LoadTGA(4,finalstring);
				}
				fin.close();


				line.xoffset = 0.0f;
				line.yoffset = 0.0f;
				loading = false;
				charplace = -1;
				//for (int i = 0; i < 30; i++)
				//	output.loadfile[i] = ' ';
				strcpy (output.tempfile, ".dlf");
				strcpy (output.loadfile, "");
				}
				keys[VK_RETURN] = false;
			}

			if (keys[VK_F1])						// Is F1 Being Pressed?
			{
				keys[VK_F1]=FALSE;					// If So Make Key FALSE
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

	// Shutdown
//	for (int i = 0; i < 3; i++)
//		glDeleteTextures(1, tgatexture[i].texture);
	tgatexture[0].KillFont();
	KillGLWindow();									// Kill The Window
	return (msg.wParam);							// Exit The Program
}
