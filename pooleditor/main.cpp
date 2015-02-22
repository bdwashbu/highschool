#include <windows.h>		
#include <stdio.h>
#include <math.h>
#include <fstream.h>
#include <iostream.h>		
#include <gl\gl.h>			
#include <gl\glu.h>

//#include "walls.h"
#include "texture.h"
#include "output.h"
#include "mouse.h"
#include "timer.h"
#include "walls.h"
#include "balls.h"
#include "holes.h"
#include "resource.h"

HDC			hDC=NULL;		
HGLRC		hRC=NULL;		
HWND		hWnd=NULL;		
HINSTANCE	hInstance;		
HMENU		menu = NULL;

bool 	done=FALSE;								// Bool Variable To Exit Loop
bool	keys[256];			
bool	active=TRUE;		
bool	fullscreen=TRUE;

const MAX_WALLS = 100;

ballclass ball[20];
holeclass hole[20];
wallclass wall[100];
textureclass texture[5];
outputclass output;
mouseclass mouse;
timerclass timer;

static unsigned long rightclick = timeGetTime();
static charplace = -1;
float *templiney = &wall[0].liney, *templinex = &wall[0].linex,xpoop = 0;;
bool saving = false, loading = false;
int ballcount, holecount, count;

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM lParam);	
LRESULT CALLBACK DlgOK(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK DlgPasswordProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK DlgOpening(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK DlgLoadProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK DlgSliderProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

//LRESULT CALLBACK DlgSliderProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//CSelectionSliderCtrl m_sliderTest;
//ShowCursor(TRUE);
 //   switch( message )										// Switch on all of the dialogs messages
 ////   {
//		case WM_COMMAND:									// If we clicked on anything in the dialog box
//
//			switch( LOWORD( wParam ) )						// Check the LOWORD of the wParam (Which holds the ID of what was clicked on)
  //          {    
//				case IDOK:
	//			{																			                    
//				EndDialog( hWnd, FALSE );
//				break;
//				}
//				case IDC_THICKNESS:
//					{					
//					wall[wall.currentwall].thickness = m_sliderTest.GetPos(IDC_THICKNESS);
//					break;
//				}
//			}
///	}
//	return false;
//	ShowCursor(FALSE);
//}


LRESULT CALLBACK DlgOpening(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
//ShowCursor(TRUE);
    switch( message )										// Switch on all of the dialogs messages
    {
		case WM_COMMAND:									// If we clicked on anything in the dialog box

			switch( LOWORD( wParam ) )						// Check the LOWORD of the wParam (Which holds the ID of what was clicked on)
            {    
				case IDOK:
					{
					if (IsDlgButtonChecked(hWnd, IDC_FULLSCREENCHECK))
						fullscreen = true;
					else
						fullscreen = false;
					EndDialog( hWnd, FALSE );
					break;
					}
				case IDCANCEL:
					{
					done = true;
					EndDialog( hWnd, FALSE );
					break;
					}
			}
	}
	return false;
//ShowCursor(FALSE);
}

LRESULT CALLBACK DlgOK(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
//ShowCursor(TRUE);
    switch( message )										// Switch on all of the dialogs messages
    {
		case WM_COMMAND:									// If we clicked on anything in the dialog box

			switch( LOWORD( wParam ) )						// Check the LOWORD of the wParam (Which holds the ID of what was clicked on)
            {    
				case IDOK:									// Check if the OK button was clicked																			                    
				EndDialog( hWnd, FALSE );
			}
	}
	return false;
//	ShowCursor(FALSE);
}




// HIDE CURSOR

LRESULT CALLBACK DlgLoadProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	char szPassword[25]={0};
	char tempfile[25]={0};
    switch( message )										// Switch on all of the dialogs messages
    {
		case WM_INITDIALOG:									// If Initilizing the dialog box		
			loading = true;
			ShowCursor(TRUE);
			// Do initialization here	(like WM_CREATE)
			
            return TRUE;

		case WM_COMMAND:									// If we clicked on anything in the dialog box
            
			switch( LOWORD( wParam ) )						// Check the LOWORD of the wParam (Which holds the ID of what was clicked on)
            {    
				case IDOK:									// Check if the OK button was clicked																			                    
															// This gets what the user typed into the password field.
															// It takes the hWnd, the ID of the dialog box control, a string to hold what they typed in,
															// and how many characters you want to retrieve from the field.
					GetDlgItemText(hWnd, IDC_PASSWORD, szPassword, 25);
															
															// Check if they typed in the right password
					if(strcmp(szPassword, "\0"))
					{
				strcpy (tempfile,"Maps/");
				strncat (tempfile, szPassword, strlen(szPassword));
				strncat (tempfile, ".dlf", 4);

				ifstream fin;
				fin.open(tempfile, ios::nocreate);
				if (fin.good())
				{
				fin >> count;
				fin >> ballcount;
				fin >> holecount;
				int *temp = new int;
				for (int i = 0; i < count; i++)
				{
					fin >> wall[i].linex;
					fin >> wall[i].line2x;
					fin >> wall[i].liney;
					fin >> wall[i].line2y;
					fin >> *temp;
					fin >> *temp;
					fin >> *temp;
					fin >> *temp;
					fin >> wall[i].slope;
					fin >> wall[i].yintercept;
					fin >> wall[i].red;
					fin >> wall[i].green;
					fin >> wall[i].blue;
				}
				for (i = 0; i < ballcount; i++)
				{
					fin >> ball[i].X;
					fin >> ball[i].Y;
				}
				for (i = 0; i < holecount; i++)
				{
					fin >> hole[i].X;
					fin >> hole[i].Y;
				}
				delete temp;
				fin.close();
				templinex = wallclass::SetPointer(&wall[0], mouse.mouse_x, mouse.mouse_y, 1);
				templiney = wallclass::SetPointer(&wall[0], mouse.mouse_x, mouse.mouse_y, 2);
				EndDialog( hWnd, FALSE );
				}
					}
					return TRUE;							// Return from the dialog proc

				case IDCANCEL:
					{
					EndDialog( hWnd, FALSE );
					return TRUE;
					}// Quit from this function
            }
            break;

        case WM_CLOSE:
			EndDialog( hWnd, FALSE );						// Close the dialog box
           
			break;

		case WM_DESTROY:									// This message happens when the dialog box closes          
			// If we need to free anything, do it here
			ShowCursor(FALSE);
			loading = false;
			break;										// Break from the loop
    }
    return FALSE;											// Return a default false
}

LRESULT CALLBACK DlgPasswordProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	char szPassword[25]={0};
	char tempfile[25]={0};
    switch( message )										// Switch on all of the dialogs messages
    {
		case WM_INITDIALOG:									// If Initilizing the dialog box		
			saving = true;
			// Do initialization here	(like WM_CREATE)

            return TRUE;

		case WM_COMMAND:									// If we clicked on anything in the dialog box
            
			switch( LOWORD( wParam ) )						// Check the LOWORD of the wParam (Which holds the ID of what was clicked on)
            {    
				case IDOK:									// Check if the OK button was clicked																			                    
															// This gets what the user typed into the password field.
															// It takes the hWnd, the ID of the dialog box control, a string to hold what they typed in,
															// and how many characters you want to retrieve from the field.
					GetDlgItemText(hWnd, IDC_PASSWORD, szPassword, 25);
															
															// Check if they typed in the right password
					if(strcmp(szPassword, "\0"))
					{
				strcpy (tempfile,"Maps/");
				strncat (tempfile, szPassword, strlen(szPassword));
				strncat (tempfile, ".dlf", 4);

				*templinex = 0.0f; *templiney = 0.0f;
				int count;
				for (int i = 0; i < MAX_WALLS; i++)
				{
					if (wall[i].linex != 0)
						count = i+1;
				}
				ofstream fout(tempfile);					
				fout << count << "\n";
				fout << ballcount << "\n";
				fout << holecount << "\n";
				fout.close();

				//save geometry
				for (i = 0; i < count; i++)
				{
					wall[i].WriteFile(tempfile);
				}
				for (i = 0; i < ballcount; i++)
				{
					ball[i].WriteFile(tempfile);
				}
				for (i = 0; i < holecount; i++)
				{
					hole[i].WriteFile(tempfile);
				}

				output.tempx = 0.0f;
						EndDialog( hWnd, FALSE );			// Close the dialog box	
					}
					return TRUE;							// Return from the dialog proc

				case IDCANCEL:
					{
					EndDialog( hWnd, FALSE );
					return TRUE;
					}// Quit from this function
            }
            break;

        case WM_CLOSE:
			//password = false;
			EndDialog( hWnd, FALSE );						// Close the dialog box
            
			break;

		case WM_DESTROY:
			saving = false;         
			// If we need to free anything, do it here

			break;										// Break from the loop
    }
    return FALSE;											// Return a default false
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

	glDisable(GL_BLEND);	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LINE_SMOOTH);  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_POINT_SMOOTH);  glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glShadeModel(GL_FLAT);
	glLineWidth(1.4f);
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				
	glPointSize(3.3f);
	glDisable(GL_DITHER);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_NOTEQUAL,0.0f);
	texture[0].LoadTGA(1,"Data/Font.tga"); 
	texture[0].BuildFont();
	//texture[2].LoadTGA(3,"Data/Backround.tga");
	//FSOUND_Init(44100, 32, 0);

	//sound1 = FSOUND_Sample_Load(FSOUND_UNMANAGED, "Data/Leftclick.wav", FSOUND_NORMAL, 0);
	//sound2 = FSOUND_Sample_Load(FSOUND_UNMANAGED, "Data/Rightclick.wav", FSOUND_NORMAL, 0);


	//stream = FSOUND_Stream_OpenFile("Data/bob.mp3", FSOUND_2D, 0);
//	FSOUND_Stream_Play(0, stream);

	//tgatexture[3].LoadTGA(4,"Data/haha0.lmp");
}

void DrawGLScene(GLvoid)									
{
			float distance = 3.0f;

	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();

	timer.begintime = timeGetTime();
	glClear(GL_COLOR_BUFFER_BIT); // | GL_DEPTH_BUFFER_BIT);
	
	mouse.DrawMouse();

	glEnable(GL_BLEND);
	//glScissor(0,0,640-xpoop, 480);
	//glEnable(GL_SCISSOR_TEST);
	glBegin(GL_LINES);
	for (int i = 0; i < MAX_WALLS; i++)
	{
	wall[i].DrawLine(i);
	}
	glEnd();
	glPointSize(10);
	glBegin(GL_POINTS);
	for (i = 0; i < ballcount; i++)
	{
	ball[i].DrawBall();
	}
	glEnd();
	for (i = 0; i < holecount; i++)
	{
	hole[i].DrawHole();
	}

	//glDisable(GL_SCISSOR_TEST);
	glColor3f(1.0f, 1.0f, 1.0f);
	glDisable(GL_BLEND);
	glFlush();

	//last so loading/saving screen is over everything
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

	menu = LoadMenu(hInstance,MAKEINTRESOURCE(IDR_MYMENU));

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
								menu,								// No Menu
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
	DrawGLScene();

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

		case WM_LBUTTONDOWN:
		{
		if (wall.currentwall != -1)
		{
			//change currentline color
			if (mouse.mouse_x > 510 && mouse.mouse_x < 620 && mouse.mouse_y > 255 && mouse.mouse_y < 265)
				wall[wall.currentwall].red = (mouse.mouse_x-510)/110;
			if (mouse.mouse_x > 510 && mouse.mouse_x < 620 && mouse.mouse_y > 225 && mouse.mouse_y < 235)
				wall[wall.currentwall].green = (mouse.mouse_x-510)/110;
			if (mouse.mouse_x > 510 && mouse.mouse_x < 620 && mouse.mouse_y > 195 && mouse.mouse_y < 205)
				wall[wall.currentwall].blue = (mouse.mouse_x-510)/110;
		}
		//detect if mouse if being held over arrows
		break;
		}

		case WM_LBUTTONUP:
		{
		//FSOUND_PlaySound(0, sound1);

		for (int i = 0; i < MAX_WALLS; i++)
			{
			if ((wall[i].linex != 0 || wall[i].liney != 0 ) && (wall[i].line2y != 0 || wall[i].line2x != 0))
				wall[i].Equation();
			}

		if (wall.gridlock)
		{
		bool gridoff = false;
		if (gridoff == false)
		{
		wall.distance = 5.0f;
		for (int i = 0; i < MAX_WALLS; i++)
		{
			wall[i].GridLock(templinex, templiney);
			if (wall.distance < 5)
			{
			*templinex = *wall.templinex2;
			*templiney = *wall.templiney2;
			}
		}
		}
		}
		if (saving != true && loading != true && mouse.mouse_y < 460 && wall.currentwall == -1)
		{
		templinex = wallclass::SetPointer(&wall[0], mouse.mouse_x, mouse.mouse_y, 1);
		templiney = wallclass::SetPointer(&wall[0], mouse.mouse_x, mouse.mouse_y, 2);
		*templinex = mouse.mouse_x;
		*templiney = mouse.mouse_y;					
		}
		break;
		}

		case WM_RBUTTONDOWN:
		{
		break;
		}

		case WM_RBUTTONUP:
		{
		//FSOUND_PlaySound(1, sound2);
		bool skip = false;
		float distance = 6.0f;
		//if right click is within editing bounries
		if (timeGetTime() - rightclick > 500)
		{
		for (int i = 0; i < count; i++)
		{
			if (wall[i].SetxRightPointer(mouse.mouse_x, mouse.mouse_y, templinex, templiney) != 0)
			{
				templinex = wall[i].SetxRightPointer(mouse.mouse_x, mouse.mouse_y, templinex, templiney);
				templiney = wall[i].SetyRightPointer(mouse.mouse_x, mouse.mouse_y, templinex, templiney);
				*templinex = mouse.mouse_x;
				*templiney = mouse.mouse_y;
				skip = true;
				break;
			}
		}
		for (i = 0; i < ballcount; i++)
		{
			if (ball[i].SetxRightPointer(mouse.mouse_x, mouse.mouse_y, templinex, templiney, 1) != 0)
			{
			*templinex = 0;
			*templiney = 0;
			templinex = &ball[i].X;
			templiney = &ball[i].Y;
			*templinex = mouse.mouse_x;
			*templiney = mouse.mouse_y;
			break;
			}
		}
		for (i = 0; i < holecount; i++)
		{
			if (hole[i].SetxRightPointer(mouse.mouse_x, mouse.mouse_y, templinex, templiney, 1) != 0)
			{
			*templinex = 0;
			*templiney = 0;
			templinex = &hole[i].X;
			templiney = &hole[i].Y;
			*templinex = mouse.mouse_x;
			*templiney = mouse.mouse_y;
			break;
			}
		}

		}
		else
		{
		//wall selection
		for (int i = 0; i < MAX_WALLS; i++)
		{
			if (wall[i].SelectWall(mouse.mouse_x, mouse.mouse_y, &distance))
			{
				skip = true;
				wall.currentwall = i;
			}
			if (skip == false)
				wall.currentwall = -1;
		}
		}
		rightclick = timeGetTime();
		break;
		}


		case WM_MOUSEMOVE:
		{
	    mouse.mouse_x = LOWORD(lParam);          
		mouse.mouse_y = HIWORD(lParam);
		mouse.mouse_y = 462 - mouse.mouse_y;
			*templinex = mouse.mouse_x;
			*templiney = mouse.mouse_y;
		//MoveCursor();
		break;
		}

		case WM_COMMAND:
			{
			if(LOWORD(wParam) == IDM_QUIT)					// If we check the Low-bits (LOWORD) of the DWORD wParam we can see what menu item they clicked on.
			{
				if (MessageBox(NULL,"Are you sure you want to leave?", "Really Leave?",MB_YESNO|MB_ICONQUESTION)==IDYES)
				done = true;						// If they clicked on the menu ID: IDM_QUIT, then we want to post the quit message.
			}
			else if(LOWORD(wParam) == IDM_SAVE)					// If we check the Low-bits (LOWORD) of the DWORD wParam we can see what menu item they clicked on.
			{
			if (loading != true)
			{
			DialogBox(NULL, MAKEINTRESOURCE(IDD_PASSWORD), NULL, (DLGPROC)DlgPasswordProc);
			}
			}
			else if(LOWORD(wParam) == IDM_LOAD)					// If we check the Low-bits (LOWORD) of the DWORD wParam we can see what menu item they clicked on.
			{
			if (saving != true)
			{
			DialogBox(NULL, MAKEINTRESOURCE(IDD_PASSWORD), NULL, (DLGPROC)DlgLoadProc);
			}
			}
			else if(LOWORD(wParam) == IDM_POINTLOCK)					// If we check the Low-bits (LOWORD) of the DWORD wParam we can see what menu item they clicked on.
			{
			if (wall.gridlock)
			{
				wall.gridlock = false;
				CheckMenuItem(menu, IDM_POINTLOCK, MF_BYCOMMAND | MF_UNCHECKED); 
			}
			else
			{
				wall.gridlock = true;
				CheckMenuItem(menu, IDM_POINTLOCK, MF_BYCOMMAND | MF_CHECKED);
 
			}
			}
			else if (LOWORD(wParam) == IDM_CONTROLS)
			{
			DialogBox(NULL, MAKEINTRESOURCE(IDD_CONTROLS), NULL, (DLGPROC)DlgOK);
			case IDOK:
				EndDialog(NULL, FALSE );
			}
			else if (LOWORD(wParam) == ID_ADDBALL)
			{
			*templinex = 0;
			*templiney = 0;
			templinex = ballclass::SetPointer(&ball[0], mouse.mouse_x, mouse.mouse_y, 1);
			templiney = ballclass::SetPointer(&ball[0], mouse.mouse_x, mouse.mouse_y, 2);
			*templinex = mouse.mouse_x;
			*templiney = mouse.mouse_y;				
			ballcount++;
			}
			else if (LOWORD(wParam) == ID_ADDHOLE)
			{
			*templinex = 0;
			*templiney = 0;
			templinex = holeclass::SetPointer(&hole[0], mouse.mouse_x, mouse.mouse_y, 1);
			templiney = holeclass::SetPointer(&hole[0], mouse.mouse_x, mouse.mouse_y, 2);
			*templinex = mouse.mouse_x;
			*templiney = mouse.mouse_y;				
			holecount++;
			}
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
	DialogBox(NULL, MAKEINTRESOURCE(IDD_OPENING), NULL, (DLGPROC)DlgOpening);
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
				SwapBuffers(hDC);				// Swap Buffers (Double Buffering)
				DrawGLScene();					// Draw The Scene
				timer.scalar = (timeGetTime() - timer.begintime)/1000.0f;
			}

			if (keys[VK_DELETE])
			{
				if (wall.currentwall != -1)
				{
					int poop = 0;
					for (int i = 0; i < MAX_WALLS; i++)
					{
						if (wall[i].linex != 0)
							poop = i+1;
					}
					wall[wall.currentwall].linex = 0;
					wall[wall.currentwall].liney = 0;
					wall[wall.currentwall].line2y = 0;
					wall[wall.currentwall].line2x = 0;
					for (i = wall.currentwall+1; i < poop-1; i++)
					{
						wall[i-1].linex = wall[i].linex;
						wall[i-1].line2x = wall[i].line2x;
						wall[i-1].liney = wall[i].liney;
						wall[i-1].line2y = wall[i].line2y;
						wall[i-1].slope = wall[i].slope;
						wall[i-1].yintercept = wall[i].yintercept;

					if (i == poop-2)
					{
						wall[i].linex = 0;
						wall[i].line2x = 0;
						wall[i].liney = 0;
						wall[i].line2y = 0;
					}
					}
					count--;
					wall.currentwall = -1;
				}
			}
				
			if (keys[VK_RETURN])
			{
				if (saving == true)
				{

				strcpy (output.tempfile,"Maps/");
				strncat (output.tempfile, output.savefile, strlen(output.savefile));
				strncat (output.tempfile, ".dlf", 4);

				*templinex = 0.0f; *templiney = 0.0f;
				int count;
				for (int i = 0; i < MAX_WALLS; i++)
				{
					if (wall[i].linex != 0)
						count = i+1;
				}
				ofstream fout(output.tempfile);					
				fout << count << "\n";
				fout << ballcount << "\n";
				fout << holecount << "\n";
				fout.close();

				//save geometry
				for (i = 0; i < count; i++)
				{
					wall[i].WriteFile(output.tempfile);
				}
				for (i = 0; i < ballcount; i++)
				{
					ball[i].WriteFile(output.tempfile);
				}
				for (i = 0; i < holecount; i++)
				{
					hole[i].WriteFile(output.tempfile);
				}

				output.tempx = 0.0f;
				saving = false;
				charplace = -1;
				memset(output.savefile,'\0', 29);
				strcpy (output.tempfile, ".dlf");
				}

				if (loading == true)
				{
				strcpy (output.tempfile,"Maps/");
				strncat (output.tempfile, output.loadfile, strlen(output.loadfile));
				strncat (output.tempfile, ".dlf", 4);

				ifstream fin;
				fin.open(output.tempfile, ios::nocreate);
				if (fin.good())
				{
				int *temp = new int;
				int count;
				fin >> count;
				fin >> *temp;
				for (int i = 0; i < count; i++)
				{
				fin >> wall[i].linex;
				fin >> wall[i].line2x;
				fin >> wall[i].liney;
				fin >> wall[i].line2y;
				fin >> *temp;
				fin >> *temp;
				fin >> *temp;
				fin >> *temp;
				fin >> wall[i].slope;
				fin >> wall[i].yintercept;
				fin >> wall[i].red;
				fin >> wall[i].green;
				fin >> wall[i].blue;
				}
				delete temp;
				templinex = wallclass::SetPointer(&wall[0], mouse.mouse_x, mouse.mouse_y, 1);
				templiney = wallclass::SetPointer(&wall[0], mouse.mouse_x, mouse.mouse_y, 2);
				}
				fin.close();

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
			//if (saving == true || loading == true)
			//	ShowCursor(TRUE);
			//else
			//	ShowCursor(FALSE);

		}
	}

	// Shutdown
	for (int i = 0; i < 1; i++)
		glDeleteTextures(1, texture[i].texture);
	texture[0].KillFont();

	//FSOUND_Stream_Close(stream);
	//FSOUND_Sample_Free(sound2);
	//FSOUND_Sample_Free(sound1);
//	FSOUND_Close();

	KillGLWindow();									// Kill The Window
	return (msg.wParam);							// Exit The Program
}
