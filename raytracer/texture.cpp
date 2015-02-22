#include "texture.h"
#include <fstream.h>
#include <windows.h>		
#include <gl\gl.h>
#include <stdio.h>		
#include <gl\glu.h>
#include <math.h>

bool textureclass::LoadTGA(int number,char *filename)// Loads A TGA File Into Memory
{   
	texID = number;
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

	glGenTextures(1, &texture[0]);// Generate OpenGL texture IDs 
	glBindTexture(GL_TEXTURE_2D, texID);// Bind Our Texture
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);// Linear Filtered
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);// Linear Filtered
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	if (bpp==24)// Was The TGA 24 Bits
	{
		type=GL_RGB;// If So Set The 'type' To GL_RGB
	} 
	glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, imageData); 
	return true;// Texture Building Went Ok, Return True
}

void textureclass::BuildFont()
{
	base=glGenLists(95);// Creating 256 Display Lists
	glBindTexture(GL_TEXTURE_2D, texID);// Select Our Font Texture
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
	}
}

void textureclass::KillFont()
{
	glDeleteLists(base,256);
}

void textureclass::glPrint(float size, GLint x, GLint y, int set, const char *string, ...)// Where The Printing Happens
{
	char text[256];// Holds Our String
	va_list ap;// Pointer To List Of Arguments 
	if (string == NULL)// If There's No Text
		return;
	va_start(ap, string);// Parses The String For Variables
		vsprintf(text, string, ap);// And Converts Symbols To Actual Numbers
	va_end(ap);// Results Are Stored In Text 

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texID);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0,640,0,480);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glTranslated(x,y,0);// Position The Text (0,0 - Bottom Left)
	glListBase(base-32);// Choose The Font Set (0 or 1) 
	glScalef((1.0f * size), (1.8f * size), 1.0f);// Enlarge Font Width And Height
	glColor3f(1.0f, 1.0f, 1.0f);
	glCallLists(strlen(text),GL_UNSIGNED_BYTE, text);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

bool textureclass::SCREENSHOT_WriteTGA(const char *fname,int w, int h)
{
unsigned char *image  = (unsigned char*)malloc(sizeof(unsigned char)*w*h*4);
glReadPixels( 0, 0, w, h, GL_BGRA_EXT, GL_UNSIGNED_BYTE, image );
    int i;
    FILE *fp;
    bool ret = false;   
    /* See http://www.dcs.ed.ac.uk/home/mxr/gfx/2d/TGA.txt  for spec */
    struct TGAHeader {
        unsigned char idfield_len;
        unsigned char cmap_type;
        unsigned char image_type;
        unsigned char cmap_spec[5];
        unsigned char x_orig[2];
        unsigned char y_orig[2];
        unsigned char width[2];
        unsigned char height[2];
        unsigned char pixel_size;
        unsigned char image_desc;
    }; TGAHeader header;//__attribute__((packed)) ;
    /* Open file */
    fp = fopen(fname, "wb");
    if (!fp) {
        ret = false;
    } else {
        /* Construct header */
        header.idfield_len = 0;
        header.cmap_type = 0;
        header.image_type = 2;
        for (i = 0; i < 5; i++) {
            header.cmap_spec[i] = 0;
        }
        for (i = 0; i < 2; i++) {
            header.x_orig[i] = 0;
            header.y_orig[i] = 0;
        }
        /* Lo bits */
        header.width[0] = w & 0xFF;
        /* Hi bits */
        header.width[1] = (w >> 8) & 0xFF;
        header.height[0] = h & 0xFF;
        header.height[1] = (h >> 8) & 0xFF;
        header.pixel_size = 32;
        header.image_desc = 8;
        /* Output header */

		unsigned char *colorpointer;
		for (int y = 0; y < h; y++)
		{
		colorpointer = image + y * h * 4;
		for (int x = 0; x < w; x++, colorpointer+=4)
		{
			if (colorpointer[0] > 0.2f || colorpointer[1] > 0.2f || colorpointer[2] > 0.2f)
				colorpointer[3] = ((255.0f - sqrtf((y - h/2.0f)*(y-h/2.0f)+(x-w/2.0f)*(x-w/2.0f)) / (w/2.0f) * 255.0f));
			else
				colorpointer[3] = 0;
		}
		}

		for (int i = 0; i < 5; i++)
		{
		for (y = 0; y < h; y++)
		{
		colorpointer = image + y * h * 4;
		for (int x = 0; x < w; x++, colorpointer+=4)
		{
			if (colorpointer[0] > 0.2f || colorpointer[1] > 0.2f || colorpointer[2] > 0.2f)
				colorpointer[3] = (colorpointer[3] + (colorpointer-4)[3] + (colorpointer+4)[3] +
				(colorpointer+1024)[3] + (colorpointer-1024)[3]) / 5;
		}
		}
		}

        fwrite(&header, sizeof(header), 1, fp);
       
        /* Output image */
        fwrite(image, sizeof(unsigned char), w*h*4, fp);
    }
    if (fp) {
        fclose(fp);
    }

    return ret;
}

