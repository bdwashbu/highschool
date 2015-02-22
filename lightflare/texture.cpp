#include "texture.h"
#include <fstream.h>
#include <windows.h>		
#include <gl\gl.h>
#include <stdio.h>		
#include <gl\glu.h>
#include <math.h>

//#define GL_CLAMP_TO_EDGE 0x812F
int base;
int loop;

bool LoadTGA(char *filename, bool linear)// Loads A TGA File Into Memory
{   
	char unsigned *imageData;
	int unsigned bpp;// Image Color Depth In Bits Per Pixel
	int unsigned width;// Image Width
	int unsigned height;// Image Height
	GLubyte		TGAheader[12]={0,0,2,0,0,0,0,0,0,0,0,0};// Uncompressed TGA Header
	GLubyte		TGAcompare[12];// Used To Compare TGA Header
	GLubyte		header[6];// First 6 Useful Bytes From The Header
	GLuint		bytesPerPixel;// Holds Number Of Bytes Per Pixel Used In The TGA File
	GLuint		imageSize;// Used To Store The Image Size When Setting Aside Ram
	GLuint		temp;// Temporary Variable
	GLuint		type=GL_RGBA;// Set The Default GL Mode To RBGA (32 BPP) 
	
	FILE *file = fopen(filename, "rb");// Open The TGA File 
	
if ( file==NULL ||// Does File Even Exist?
	fread(TGAcompare,1,sizeof(TGAcompare),file)!=sizeof(TGAcompare) ||// Are There 12 Bytes To Read?
	memcmp(TGAheader,TGAcompare,sizeof(TGAheader))!=0 ||// Does The Header Match What We Want?
	fread(header,1,sizeof(header),file)!=sizeof(header))// If So Read Next 6 Header Bytes
{
	if (file == NULL)// Does The File Even Exist? *Added Jim Strong*
		return FALSE;// Return False
	else// Otherwise
	{
		fclose(file);// If Anything Failed, Close The File
		return FALSE;// Return False
	}
} 
	
	width  = header[1] * 256 + header[0];// Determine The TGA Width (highbyte*256+lowbyte)
	height = header[3] * 256 + header[2];// Determine The TGA Height (highbyte*256+lowbyte)
   
	if (width<=0 ||// Is The Width Less Than Or Equal To Zero
		height<=0 ||// Is The Height Less Than Or Equal To Zero
		(header[4]!=24 && header[4]!=32))// Is The TGA 24 or 32 Bit?
	{
		fclose(file);// If Anything Failed, Close The File
		return FALSE;// Return False
	} 

	bpp= header[4];// Grab The TGA's Bits Per Pixel (24 or 32)
	bytesPerPixel= bpp/8;// Divide By 8 To Get The Bytes Per Pixel
	imageSize= width*height*bytesPerPixel;// Calculate The Memory Required For The TGA Data 
	
	imageData=(GLubyte *)malloc(imageSize);// Reserve Memory To Hold The TGA Data 
	
	if (imageData==NULL ||// Does The Storage Memory Exist?
		fread(imageData, 1, imageSize, file)!=imageSize)// Does The Image Size Match The Memory Reserved?
	{
		if(imageData!=NULL)// Was Image Data Loaded
			free(imageData);// If So, Release The Image Data 
		fclose(file);// Close The File
		return FALSE;// Return False
	} 
	
	for(GLuint i=0; i<int(imageSize); i+=bytesPerPixel)// Loop Through The Image Data
	{// Swaps The 1st And 3rd Bytes ('R'ed and 'B'lue)
		temp=imageData[i];// Temporarily Store The Value At Image Data 'i'
		imageData[i] = imageData[i + 2];// Set The 1st Byte To The Value Of The 3rd Byte
		imageData[i + 2] = temp;// Set The 3rd Byte To The Value In 'temp' (1st Byte Value)
	}
	
	fclose (file);// Close The File // Build A Texture From The Data

	//glBindTexture(GL_TEXTURE_2D, texture);// Bind Our Texture
	if (linear) {
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);// Linear Filtered
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);// Linear Filtered
	}
	else {
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);// Linear Filtered
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);// Linear Filtered
	}
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	if (bpp==24)// Was The TGA 24 Bits
	{
		type=GL_RGB;// If So Set The 'type' To GL_RGB
	} 
	glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, imageData); 
	return true;// Texture Building Went Ok, Return True
}

void BuildFont()
{
	base=glGenLists(95);
	//glBindTexture(GL_TEXTURE_2D, texID);
	for (loop=0; loop<95; loop++)
	{
		float cx = float(loop%16)/16.0f;
		float cy = float(loop/16)/8.0f;
		glNewList(base+loop,GL_COMPILE);
		glBegin(GL_QUADS);
			glTexCoord2f(cx+0.001,1.0f-cy-0.120f);
			glVertex2d(0,0);
			glTexCoord2f(cx+0.0620f,1.0f-cy-0.120f);
			glVertex2i(16,0);
			glTexCoord2f(cx+0.0620f,0.980f-cy);
			glVertex2i(16,13);
			glTexCoord2f(cx+0.001,0.980f-cy);
			glVertex2i(0,13);
		glEnd();
	glTranslated(16,0,0);
	glEndList();
	}
}

void KillFont()
{
	glDeleteLists(base,256);
}

void glPrint(float size, GLfloat x, GLfloat y, int set, const char *string, ...)// Where The Printing Happens
{
	char text[256];// Holds Our String
	va_list ap;// Pointer To List Of Arguments 
	if (string == NULL)// If There's No Text
		return;
	va_start(ap, string);// Parses The String For Variables
		vsprintf(text, string, ap);// And Converts Symbols To Actual Numbers
	va_end(ap);// Results Are Stored In Text 

	glPushMatrix();
	glLoadIdentity();
	glTranslatef(x,y,0);// Position The Text (0,0 - Bottom Left)
	glListBase(base-32);// Choose The Font Set (0 or 1) 
	glScalef((1.0f * size), (1.8f * size), 1.0f);// Enlarge Font Width And Height
	glCallLists(strlen(text),GL_UNSIGNED_BYTE, text);
	glPopMatrix();
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




