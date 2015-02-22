#include <windows.h>		
#include <iostream.h>
#include <math.h>
#include <fstream.h>
#include <gl\gl.h>			
#include <gl\glu.h>
#include <time.h>
#include "vector.h"
#include "sphere.h"
#include "texture.h"

HDC			hDC=NULL;		
HGLRC		hRC=NULL;		
HWND		hWnd=NULL;		
HINSTANCE	hInstance;		

bool 	done=FALSE;								// Bool Variable To Exit Loop
bool	keys[256];			
bool	active=TRUE;		
bool	fullscreen=TRUE;

vectorclass Vector[640][480];
sphereclass sphere[5];
textureclass texture;
float Lightx = 320.0f;
float Lighty = 240.0f;
float Lightz = -50.0f;



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
	gluOrtho2D(0,640,0,480);
	glMatrixMode(GL_MODELVIEW);							
	glLoadIdentity();									
}

void InitGL(GLvoid)										
{
	glMatrixMode(GL_PROJECTION);						
	glLoadIdentity();
	gluOrtho2D(0,640,0,480);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glShadeModel(GL_FLAT);
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glClearDepth(1.0f);
	//glDisable(GL_DITHER);
	glPointSize(1.0f);
	//glEnable(GL_DEPTH_TEST);
	//glEnable(GL_POINT_SMOOTH);
	glDepthFunc(GL_LEQUAL);
	srand( time(NULL) );
	ofstream fout("Data.txt");
	fout.close();
//	vectorclass::RotateX(90.0f, 200)
//	vectorclass::RotateZ(90.0f, 200)
	sphere[1].SetAttributes(320, 240, 150, 100);
	sphere[0].SetAttributes(340, 480, 80, 50);
	sphere[2].SetAttributes(540, 220, 20, 50);
	sphere[3].SetAttributes(320, -10, 100, 50);
	sphere[4].SetAttributes(80, 200, 100, 50);
	sphere[0].Red = 1.0f;
	sphere[0].Green = 0.0f;
	sphere[0].Blue = 0.0f;
	sphere[1].Red = 0.0f;
	sphere[1].Green = 1.0f;
	sphere[1].Blue = 0.0f;
	sphere[2].Red = 0.0f;
	sphere[2].Green = 0.0f;
	sphere[2].Blue = 1.0f;
	sphere[3].Red = 1.0f;
	sphere[3].Green = 1.0f;
	sphere[3].Blue = 0.0f;
	sphere[4].Red = 1.0f;
	sphere[4].Green = 0.0f;
	sphere[4].Blue = 1.0f;


	for (int x = 0; x < 640; x++)
	{
	for (int y = 0; y < 480; y++)
	{
	Vector[x][y].XVector = x - 320;
	Vector[x][y].YVector = y - 240;
	Vector[x][y].ZVector = 400;
	vectorclass::Normalize(&Vector[x][y].XVector, &Vector[x][y].YVector, &Vector[x][y].ZVector);
	//float tempXVector = Vector[x][y].XVector;
	//float tempZVector = Vector[x][y].ZVector;
	//Vector[x][y].ZVector = tempZVector * cos(0.1f*3.14f) - tempXVector * sin(0.1f*3.14f);
	//Vector[x][y].XVector = tempXVector * cos(0.1f*3.14f) + tempZVector * sin(0.1f*3.14f);
	//vectorclass::Normalize(&Vector[x][y].XVector, &Vector[x][y].YVector, &Vector[x][y].ZVector);

//	    [((1 - cos(0.2f*3.14f)) * x2 + c)        ((1 - cos(0.2f*3.14f)) * x * y - s * z) ((1 - cos(0.2f*3.14f)) * x * z + s * y)]
//	r =	[((1 - cos(0.2f*3.14f)) * x * y + s * z) ((1 - cos(0.2f*3.14f)) * y2 + c)        ((1 - cos(0.2f*3.14f)) * y * z - s * x)]
//		[((1 - cos(0.2f*3.14f)) * x * z - s * z) ((1 - cos(0.2f*3.14f)) * y * z + s * x) ((1 - cos(0.2f*3.14f)) * z2 + c)       ]
	}
	}
}

void DrawGLScene(GLvoid)									
{
int savei, savei2;
bool test1 = false, test2 = false, test3 = false;
float Xintersect2, Yintersect2, Zintersect2;
float B, C, Xintersect, Yintersect, Zintersect, XLightVector, YLightVector , ZLightVector, XNormal, YNormal, ZNormal,
FinalXLightVector, FinalYLightVector, FinalZLightVector, CurrentXVector, CurrentYVector, CurrentZVector,
CurrentXRayStart, CurrentYRayStart, CurrentZRayStart, red = 0, green = 0, blue = 0, red2, green2, blue2,
currentred, currentgreen, currentblue;
//ofstream fout2("Data.txt", ios::app);
glBegin(GL_POINTS);
//glColor3f(1.0f, 0.0f, 0.0);
for (int x = 0; x < 640; x++)
{
for (int y = 0; y < 480; y++)
{
	test1 = false, test2 = false, test3 = false;
	float t = 10000;
	CurrentXVector = Vector[x][y].XVector; CurrentYVector = Vector[x][y].YVector; CurrentZVector = Vector[x][y].ZVector;
	CurrentXRayStart = 320; CurrentYRayStart = 240; CurrentZRayStart = -500;

	for (int r = 0; r < 2; r++)
	{
	for (int i = 0; i < 5; i++)
	{
		if (r == 1 && i == savei)
			continue;
		B = 2*((CurrentXVector)*(CurrentXRayStart-sphere[i].X)+(CurrentYVector)*(CurrentYRayStart-sphere[i].Y)+(CurrentZVector)*(CurrentZRayStart-sphere[i].Z));
		C = (CurrentXRayStart-sphere[i].X)*(CurrentXRayStart-sphere[i].X)+(CurrentYRayStart-sphere[i].Y)*(CurrentYRayStart-sphere[i].Y)+(CurrentZRayStart-sphere[i].Z)*(CurrentZRayStart-sphere[i].Z) - (sphere[i].Radius*sphere[i].Radius);

		if ((-B + sqrt(B*B-4*C))/2 < t && (-B + sqrt(B*B-4*C))/2 > 0)
		{
			if (r == 0)
			{
			test1 = true;
			savei = i;
			}
			if (r == 1)
			{
				test2 = true;
				savei2 = i;
			}
			t = (-B + sqrt(B*B-4*C))/2;
			XNormal = (CurrentXRayStart+CurrentXVector*t)-sphere[i].X;
			YNormal = (CurrentYRayStart+CurrentYVector*t)-sphere[i].Y;
			ZNormal = (CurrentZRayStart+CurrentZVector*t)-sphere[i].Z;
			vectorclass::Normalize(&XNormal, &YNormal, &ZNormal);
			currentred = sphere[i].Red;
			currentgreen = sphere[i].Green;
			currentblue = sphere[i].Blue;
		}
		if ((-B - sqrt(B*B-4*C))/2 < t && (-B - sqrt(B*B-4*C))/2 > 0)
		{
			if (r == 0)
			{
			test1 = true;
			savei = i;
			}
			if (r == 1)
			{
				test2 = true;
				savei2 = i;
			}
			t = (-B - sqrt(B*B-4*C))/2;
			XNormal = (CurrentXRayStart+CurrentXVector*t)-sphere[i].X;
			YNormal = (CurrentYRayStart+CurrentYVector*t)-sphere[i].Y;
			ZNormal = (CurrentZRayStart+CurrentZVector*t)-sphere[i].Z;
			vectorclass::Normalize(&XNormal, &YNormal, &ZNormal);
			currentred = sphere[i].Red;
			currentgreen = sphere[i].Green;
			currentblue = sphere[i].Blue;
		}
	}
	if (t == 10000)
		break;
	CurrentXRayStart = CurrentXRayStart+CurrentXVector*t;
	CurrentYRayStart = CurrentYRayStart+CurrentYVector*t;
	CurrentZRayStart = CurrentZRayStart+CurrentZVector*t;
	float tempx = CurrentXVector;
	float tempy = CurrentYVector;
	float tempz = CurrentZVector;
	CurrentXVector = tempx - 2*((tempx*XNormal)+(tempy*YNormal)+(tempz*ZNormal))*XNormal;
	CurrentYVector = tempy - 2*((tempx*XNormal)+(tempy*YNormal)+(tempz*ZNormal))*YNormal;
	CurrentZVector = tempz - 2*((tempx*XNormal)+(tempy*YNormal)+(tempz*ZNormal))*ZNormal;
	 //CurrentXVector = XNormal;	CurrentYVector = YNormal;	CurrentZVector = ZNormal;
	vectorclass::Normalize(&CurrentXVector, &CurrentYVector, &CurrentZVector);
	if (r == 0)
		vectorclass::Lighting(&red, &green, &blue, Lightx, Lighty, Lightz, CurrentXVector, CurrentYVector, CurrentZVector,
		CurrentXRayStart, CurrentYRayStart, CurrentZRayStart, currentred, currentgreen, currentblue);
	else
		vectorclass::Lighting(&red2, &green2, &blue2, Lightx, Lighty, Lightz, CurrentXVector, CurrentYVector, CurrentZVector,
		CurrentXRayStart, CurrentYRayStart, CurrentZRayStart, currentred, currentgreen, currentblue);
	t = 10000;
	}

	
	if (t != 10000)
	{
	t == 10000;
	for (int i = 0; i < 5; i++)
	{
		if (i == savei)
			continue;
		B = 2*((XLightVector)*(Xintersect-sphere[i].X)+(YLightVector)*(Yintersect-sphere[i].Y)+(ZLightVector)*(Zintersect-sphere[i].Z));
		C = (Xintersect-sphere[i].X)*(Xintersect-sphere[i].X)+(Yintersect-sphere[i].Y)*(Yintersect-sphere[i].Y)+(Zintersect-sphere[i].Z)*(Zintersect-sphere[i].Z) - (sphere[i].Radius*sphere[i].Radius);

		if ((-B + sqrt(B*B-4*C))/2 > 0 || (-B - sqrt(B*B-4*C))/2 > 0)
		{
		if ((-B + sqrt(B*B-4*C))/2 < t && (-B + sqrt(B*B-4*C))/2 > 0)
		{
			test2 = true;
			t = (-B + sqrt(B*B-4*C))/2;
			Xintersect2 = Xintersect-XLightVector*t;
			Yintersect2 = Yintersect-YLightVector*t;
			Zintersect2 = Zintersect-ZLightVector*t;
			XNormal = Xintersect2-sphere[i].X;
			YNormal = Yintersect2-sphere[i].Y;
			ZNormal = Zintersect2-sphere[i].Z;
			//vectorclass::Normalize(&XNormal, &YNormal, &ZNormal);
			FinalXLightVector = XLightVector - 2*((XLightVector*XNormal)+(YLightVector*YNormal)+(ZLightVector*ZNormal))*XNormal;
			FinalYLightVector = YLightVector - 2*((XLightVector*XNormal)+(YLightVector*YNormal)+(ZLightVector*ZNormal))*YNormal;
			FinalZLightVector = ZLightVector - 2*((XLightVector*XNormal)+(YLightVector*YNormal)+(ZLightVector*ZNormal))*ZNormal;
			XLightVector = XNormal;
			YLightVector = YNormal;
			ZLightVector = ZNormal;

			//vectorclass::Normalize(&XLightVector, &YLightVector, &ZLightVector);
		}
		if ((-B - sqrt(B*B-4*C))/2 < t && (-B - sqrt(B*B-4*C))/2 > 0)
		{
			test2 = true;
			t = (-B - sqrt(B*B-4*C))/2;
			Xintersect2 = Xintersect-XLightVector*t;
			Yintersect2 = Yintersect-YLightVector*t;
			Zintersect2 = Zintersect-ZLightVector*t;
			XNormal = Xintersect2-sphere[i].X;
			YNormal = Yintersect2-sphere[i].Y;
			ZNormal = Zintersect2-sphere[i].Z;
			//vectorclass::Normalize(&XNormal, &YNormal, &ZNormal);
			FinalXLightVector = XLightVector - 2*((XLightVector*XNormal)+(YLightVector*YNormal)+(ZLightVector*ZNormal))*XNormal;
			FinalYLightVector = YLightVector - 2*((XLightVector*XNormal)+(YLightVector*YNormal)+(ZLightVector*ZNormal))*YNormal;
			FinalZLightVector = ZLightVector - 2*((XLightVector*XNormal)+(YLightVector*YNormal)+(ZLightVector*ZNormal))*ZNormal;
			XLightVector = XNormal;
			YLightVector = YNormal;
			ZLightVector = ZNormal;

			//vectorclass::Normalize(&FinalXLightVector, &FinalYLightVector, &FinalZLightVector);
		}
		}
	}
	}
	
		
		if (test1 == false && test2 == false)
			glColor3f(0.0f, 0.0f, 0.0f);
		else
		{
			if (test2 == true)
			{
			red = (red+red2*0.5)/2;
			green = (green+green2*0.5)/2;
			blue = (blue+blue2*0.5)/2;
			}
			glColor3f(red, green, blue);
		}
	glVertex2f(x-0.1f,y-0.1f);

}
}
glEnd();
texture.SCREENSHOT_WriteTGA("Pic.tga",640, 480);
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
	DrawGLScene();					// Draw The Scene
	//create classes
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
		break;
		}

		case WM_LBUTTONDOWN:
		{
		break;
		}

		case WM_LBUTTONUP:
		{
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

	//DONT Ask The User Which Screen Mode They Prefer
	fullscreen=FALSE;							// Windowed Mode

	// Create Our OpenGL Window
	if (!CreateGLWindow("Brian Washburn Raytracer",640,480,16,fullscreen))
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
					SwapBuffers(hDC);				// Swap Buffers (Double Buffering)
				//	timer.scalar = (timeGetTime() - timer.begintime)/1000.0f;
				}
			}

			if (keys[VK_END])
			{
				keys[VK_END] = false;
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
	KillGLWindow();									// Kill The Window
	return (msg.wParam);							// Exit The Program
}
