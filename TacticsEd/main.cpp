// add opengl32.lib glu32.lib winmm.lib to linker

#include <windows.h>
#include <gl\gl.h>			
#include <gl\glu.h>			
#include <time.h>
#include <fstream.h>
#include <math.h>
#include "texture.h"
#include "grid.h"
#include "resource.h"
//#include <gl\glaux.h>	only for bitmaps
//#include <stdlib.h> include for rand() and seeding

#pragma comment( lib, "opengl32.lib" )
#pragma comment( lib, "glu32.lib" )
#pragma comment( lib, "winmm.lib" )

#define BUFF_MAX 256

HDC			hDC=NULL;		
HGLRC		hRC=NULL;		
HWND		hWnd=NULL;	
HINSTANCE	hInstance;
HMENU		menu = NULL;	

bool keys[256], active = true, fullscreen = true, done = false;		
double scalar = 0, begintime = 0;
int MouseX, MouseY;
int Width = 0, Height = 0;
bool RightButtonDown = false, LeftButtonDown = false;
extern float XPos, YPos;
extern float Rotation;
extern gridclass *Grid;
int MouseX1, MouseX2, MouseY1, MouseY2;
bool Moved = true;
bool Rotated = false;
bool MakeSelection = false;
OPENFILENAME global_params = {0};
	
int DrawGLScene(GLvoid);

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

LRESULT CALLBACK NewProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
ShowCursor(TRUE);
    switch( message )										// Switch on all of the dialogs messages
    {
		case WM_COMMAND:									// If we clicked on anything in the dialog box

			switch( LOWORD( wParam ) )						// Check the LOWORD of the wParam (Which holds the ID of what was clicked on)
            {    
				case IDOK:
					{
					Width = GetDlgItemInt(hWnd, IDC_WIDTH, 0, 0);
					Height = GetDlgItemInt(hWnd, IDC_HEIGHT, 0, 0);
					if (Grid != 0)
						delete Grid;
					Grid = new gridclass(Width,Height);
					ShowCursor(FALSE);
					EndDialog( hWnd, FALSE );
					break;
					}
				case IDCANCEL:
					{
					ShowCursor(FALSE);
					EndDialog( hWnd, FALSE );
					break;
					}
			}
	}
	return false;
}

void FillOpenParams(OPENFILENAME &open_params, HWND hwnd, char *filter, char *file_name)
{
	open_params.lStructSize = sizeof(OPENFILENAME); 
	open_params.hwndOwner = hwnd; 
	open_params.lpstrFilter = filter;
	
	open_params.lpstrFile = file_name; 
				
	open_params.nMaxFile = BUFF_MAX; 
				
	open_params.lpstrInitialDir = "/Maps"; 

	open_params.lpstrFileTitle = NULL; 
									  
	open_params.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR
						| OFN_HIDEREADONLY;
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

	glMatrixMode(GL_MODELVIEW);							
	glLoadIdentity();									
}

int InitGL(GLvoid)									
{
	srand( time(NULL) );
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glShadeModel(GL_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				
	glClearDepth(1.0f);									
	glEnable(GL_DEPTH_TEST);							
	glDepthFunc(GL_LEQUAL);								
	glPolygonOffset(5.0, 1.2f);
	glCullFace(GL_BACK);
	glPointSize(4.0f);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_CULL_FACE);

	//Texture loading Prototype

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	//glEnable(GL_ALPHA_TEST);
	//glAlphaFunc(GL_NOTEQUAL,0.0f);

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

	menu = LoadMenu(hInstance,MAKEINTRESOURCE(IDR_MENU1));

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
								menu,								
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
		case WM_MOUSEMOVE:
		{
		if (LeftButtonDown) {
			MouseX2 = MouseX;
			MouseY2 = MouseY;
		}
		else {
			MouseX1 = 0;
			MouseX2 = 0;
			MouseX2 = 0;
			MouseY2 = 0;
		}
	    MouseX = LOWORD(lParam);          
		MouseY = HIWORD(lParam);
		MouseY = 460 - MouseY;
		if (Moved == false && RightButtonDown) {
			SetCursorPos(320,240);
			Moved = true;
		}
		else if (Moved) {
		if (fullscreen == false) {
		if (RightButtonDown && !LeftButtonDown && (LOWORD(lParam) != 316 || HIWORD(lParam) != 197)) {
			XPos+=cosf(Rotation/57.2958f)*(316-LOWORD(lParam))*(float)scalar*2.0f;
			YPos+=sinf(Rotation/57.2958f)*(316-LOWORD(lParam))*(float)scalar*2.0f;
			XPos-=sinf(Rotation/57.2958f)*(197-HIWORD(lParam))*(float)scalar*2.0f;
			YPos+=cosf(Rotation/57.2958f)*(197-HIWORD(lParam))*(float)scalar*2.0f;
			SetCursorPos(320,240);
		}
		if (RightButtonDown && LeftButtonDown && (LOWORD(lParam) != 316 || HIWORD(lParam) != 197)) {
			if (Rotation-(316-LOWORD(lParam))*(float)scalar*14.0f > 0)
				Rotation-=(316-LOWORD(lParam))*(float)scalar*14.0f;
			else
				Rotation = 360;
			if (Rotation > 360)
				Rotation = 0;

			SetCursorPos(320,240);
		}
		}
		else {
		if (RightButtonDown && !LeftButtonDown && (LOWORD(lParam) != 320 || HIWORD(lParam) != 221)) {
			XPos+=cosf(Rotation/57.2958f)*(320-LOWORD(lParam))*(float)scalar*2.0f;
			YPos+=sinf(Rotation/57.2958f)*(320-LOWORD(lParam))*(float)scalar*2.0f;
			XPos-=sinf(Rotation/57.2958f)*(221-HIWORD(lParam))*(float)scalar*2.0f;
			YPos+=cosf(Rotation/57.2958f)*(221-HIWORD(lParam))*(float)scalar*2.0f;
			SetCursorPos(320,240);
		}
		if (RightButtonDown && LeftButtonDown && (LOWORD(lParam) != 320 || HIWORD(lParam) != 221)) {
		Rotated = true;
			if (Rotation-(320-LOWORD(lParam))*(float)scalar*14.0f > 0)
				Rotation-=(320-LOWORD(lParam))*(float)scalar*14.0f;
			else
				Rotation = 360;
			if (Rotation > 360)
				Rotation = 0;

			SetCursorPos(320,240);
		}
		}
		}
		break;
		}

		case WM_LBUTTONDOWN:
		{
			MouseX1 = MouseX;
			MouseY1 = MouseY;
			MouseX2 = MouseX;
			MouseY2 = MouseY;
			LeftButtonDown = true;
		break;
		}

		case WM_LBUTTONUP:
		{
			LeftButtonDown = false;
			MouseX2 = MouseX;
			MouseY2 = MouseY;
			if (RightButtonDown == false && Rotated == false)
				MakeSelection = true;
			if (Rotated)
				Rotated = false;
		break;
		}

		case WM_RBUTTONDOWN:
		{
			Moved = false;
			RightButtonDown = true;
		break;
		}

		case WM_RBUTTONUP:
		{
			RightButtonDown = false;
		break;
		}

		case WM_COMMAND:
		{
			if(LOWORD(wParam) == IDM_EXIT)					
			{
				if (MessageBox(NULL,"Are you sure you want to leave?", "Really Leave?",MB_YESNO|MB_ICONQUESTION)==IDYES)
				done = true;						
			}
			else if(LOWORD(wParam) == IDM_NEW)
			{
			DialogBox(NULL, MAKEINTRESOURCE(IDD_NEWDIALOG), NULL, (DLGPROC)NewProc);
			}
			else if(LOWORD(wParam) == IDM_SAVE && Grid != 0)
			{
				ShowCursor(TRUE);
				OPENFILENAME open_params = {0};
	            char filter[BUFF_MAX] = {0}; 
	            char file_name[BUFF_MAX] = {0};
				strcat(filter,"Map Editor Files"); 

				int index = strlen(filter) + 1; 
												
				filter[index++] = '*';
				filter[index++] = '.';
				filter[index++] = 'm';
				filter[index++] = 'e';
				filter[index++] = 'f';
				
				FillOpenParams(open_params,hWnd,filter,file_name);

				const char * directory_name = "Maps";
				if (!CreateDirectory(directory_name, NULL))
					if (GetLastError() != ERROR_ALREADY_EXISTS)
						cerr << "CreateDirectory" << endl;

				if(GetSaveFileName(&open_params))
				{
					if (strstr(open_params.lpstrFile,".mef") == NULL)
						strcat(open_params.lpstrFile,".mef"); 
					global_params.lpstrFile = open_params.lpstrFile;
					ofstream fout(global_params.lpstrFile);
					Grid->WriteData(fout,2);
					fout.close();
				}
				ShowCursor(FALSE);

			}
			else if(LOWORD(wParam) == IDM_EXPORT && Grid != 0)
			{
				ShowCursor(TRUE);
				OPENFILENAME open_params = {0};
	            char filter[BUFF_MAX] = {0}; 
	            char file_name[BUFF_MAX] = {0};
				strcat(filter,"Map Files"); 

				int index = strlen(filter) + 1; 
												
				filter[index++] = '*';
				filter[index++] = '.';
				filter[index++] = 'm';
				filter[index++] = 'a';
				filter[index++] = 'p';
				
				FillOpenParams(open_params,hWnd,filter,file_name);

				const char * directory_name = "Maps";
				if (!CreateDirectory(directory_name, NULL))
					if (GetLastError() != ERROR_ALREADY_EXISTS)
						cerr << "CreateDirectory" << endl;

				if(GetSaveFileName(&open_params))
				{
					if (strstr(open_params.lpstrFile,".map") == NULL)
						strcat(open_params.lpstrFile,".map"); 
					global_params.lpstrFile = open_params.lpstrFile;
					ofstream fout(global_params.lpstrFile);
					Grid->WriteData(fout,1);
					fout.close();
				}
				ShowCursor(FALSE);

			}
			else if(LOWORD(wParam) == IDM_LOAD)
			{
				ShowCursor(TRUE);
				OPENFILENAME open_params = {0};
	            char filter[BUFF_MAX] = {0}; 
	            char file_name[BUFF_MAX] = {0};
				strcat(filter,"Map Files"); 

				int index = strlen(filter) + 1; 
												
				filter[index++] = '*';
				filter[index++] = '.';
				filter[index++] = 'm';
				filter[index++] = 'e';
				filter[index++] = 'f';
				
				FillOpenParams(open_params,hWnd,filter,file_name);

				const char * directory_name = "Maps";
				if (!CreateDirectory(directory_name, NULL))
					if (GetLastError() != ERROR_ALREADY_EXISTS)
						cerr << "CreateDirectory" << endl;

				if(GetOpenFileName(&open_params))
				{
					if (Grid != 0)
						delete Grid;
					global_params.lpstrFile = open_params.lpstrFile;
					ifstream fin(global_params.lpstrFile);
					Grid = new gridclass(fin);
					fin.close();

				}
				ShowCursor(FALSE);
			}
		break;
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
					DrawGLScene();					
					SwapBuffers(hDC);				
				}
			}
		}
	}

	// Shutdown commands
	KillGLWindow();
	delete Grid;
	//glDeleteTextures(1, &texture[0]) //delete texture
	//delete commands go here
	return (msg.wParam);							
}





