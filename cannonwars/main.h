#define MAX_PARTICLES 60
#define SIZE 170

extern float fa[SIZE];
extern int color1, color2, realxplacment, realyplacment, realxplacment2, realyplacment2;
extern float tlighting[SIZE][3], colors[9][3], scalar;
extern bool player1;

int findlowheight();
int Averageheight();

typedef struct// Create A Structure
{
	GLubyte *imageData;// Image Data (Up To 32 Bits)
	GLuint bpp;// Image Color Depth In Bits Per Pixel
	GLuint width;// Image Width
	GLuint height;// Image Height
	GLuint texID;// Texture ID Used To Select A Texture
} 
TextureImage;

typedef struct
{
	bool active;
	double time;
	float x;
	float y;
	float xi; 
	float yi;
	int xo;
	int yo;
	float cannonup;
	float speed;
}
cannonball2; cannonball2 cannonball[2];

struct // Create A Structure For The Timer Information
{
  __int64       frequency;// Timer Frequency
  float         resolution;// Timer Resolution
  unsigned long mm_timer_start;// Multimedia Timer Start Value
  unsigned long mm_timer_elapsed;// Multimedia Timer Elapsed Time
  bool performance_timer;// Using The Performance Timer?
  __int64       performance_timer_start;// Performance Timer Start Value
  __int64       performance_timer_elapsed;// Performance Timer Elapsed Time
} timer;

typedef struct
{
	bool active;
	float fade;
	float life;
	float x;
	float y;
	float xi;
	float yi;
	float speed;
	int startxpos;
	int startypos;
	bool player1;
	bool player2;
	float time;
	float red;
	float green;
	float blue;
	float gravity;
	float wind;
	float size;
	int type;
}
particles;
particles particle[65];

void TimerInit(void)// Initialize Our Timer (Get It Ready)
{
memset(&timer, 0, sizeof(timer));// Clear Our Timer Structure // Check To See If A Performance Counter Is Available
// If One Is Available The Timer Frequency Will Be Updated
if (!QueryPerformanceFrequency((LARGE_INTEGER *) &timer.frequency))
{
// No Performace Counter Available
timer.performance_timer= FALSE;// Set Performance Timer To FALSE
timer.mm_timer_start= timeGetTime();// Use timeGetTime() To Get Current Time
timer.resolution= 1.0f/1000.0f;// Set Our Timer Resolution To .001f
timer.frequency= 1000;// Set Our Timer Frequency To 1000
timer.mm_timer_elapsed= timer.mm_timer_start;// Set The Elapsed Time To The Current Time
}
else
{
QueryPerformanceCounter((LARGE_INTEGER *) &timer.performance_timer_start);
timer.performance_timer= TRUE;// Set Performance Timer To TRUE
// Calculate The Timer Resolution Using The Timer Frequency
timer.resolution= (float) (((double)1.0f)/((double)timer.frequency));
// Set The Elapsed Time To The Current Time
timer.performance_timer_elapsed= timer.performance_timer_start;
}
}
float TimerGetTime()// Get Time In Milliseconds
{
__int64 time;// time Will Hold A 64 Bit Integer 
if (timer.performance_timer)// Are We Using The Performance Timer?
{
QueryPerformanceCounter((LARGE_INTEGER *) &time);// Grab The Current Performance Time
// Return The Current Time Minus The Start Time Multiplied By The Resolution And 1000 (To Get MS)
return ( (float) ( time - timer.performance_timer_start) * timer.resolution)*1000.0f;
}
else
{
// Return The Current Time Minus The Start Time Multiplied By The Resolution And 1000 (To Get MS)
return( (float) ( timeGetTime() - timer.mm_timer_start) * timer.resolution)*1000.0f;
}
}

GLvoid terrainsmooth(int smooth)
{
for (int f = 0; f < smooth; f++)
{
for (int i = 0; i <170-1; i++)
{
fa[i] = (fa[i] + fa[i+1]) / 2;
}
}
}

GLvoid fill1DFractArray (int size)
{
fa[0] = 240;

for(int i = 1; i < size; i++)
{
fa[i] = float((rand()%40)-20.0);
fa[i] = fa[i] + fa[i-1];
while (fa[i] < 0.0)
{
fa[i] = float((rand()%40)-20.0);
fa[i] = fa[i] + fa[i-1];
}
}
}

GLvoid drawterrain(int size)
{
	int	i;
    float	x, inc;
    inc = 4.0;
    x = 0.0;
	glDepthMask(false);
	glBegin(GL_LINE_STRIP);
    for (i=0; i<size; i++) 
	{
			glColor3f(tlighting[i][0], tlighting[i][1], tlighting[i][2]);
			glVertex2f(x*inc, fa[i]);	
	x++;
    }
	glEnd();
	glDepthMask(true);
	glColor3f(1.0f, 1.0f, 1.0f);
}

bool LoadTGA(TextureImage *texture, char *filename)// Loads A TGA File Into Memory
{   
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
	
	texture->width  = header[1] * 256 + header[0];// Determine The TGA Width (highbyte*256+lowbyte)
	texture->height = header[3] * 256 + header[2];// Determine The TGA Height (highbyte*256+lowbyte)
   
	if (texture->width<=0 ||// Is The Width Less Than Or Equal To Zero
		texture->height<=0 ||// Is The Height Less Than Or Equal To Zero
		(header[4]!=24 && header[4]!=32))// Is The TGA 24 or 32 Bit?
	{
		fclose(file);// If Anything Failed, Close The File
		return FALSE;// Return False
	} 

	texture->bpp= header[4];// Grab The TGA's Bits Per Pixel (24 or 32)
	bytesPerPixel= texture->bpp/8;// Divide By 8 To Get The Bytes Per Pixel
	imageSize= texture->width*texture->height*bytesPerPixel;// Calculate The Memory Required For The TGA Data 
	
	texture->imageData=(GLubyte *)malloc(imageSize);// Reserve Memory To Hold The TGA Data 
	
	if (texture->imageData==NULL ||// Does The Storage Memory Exist?
		fread(texture->imageData, 1, imageSize, file)!=imageSize)// Does The Image Size Match The Memory Reserved?
	{
		if(texture->imageData!=NULL)// Was Image Data Loaded
			free(texture->imageData);// If So, Release The Image Data 
		fclose(file);// Close The File
		return FALSE;// Return False
	} 
	
	for(GLuint i=0; i<int(imageSize); i+=bytesPerPixel)// Loop Through The Image Data
	{// Swaps The 1st And 3rd Bytes ('R'ed and 'B'lue)
		temp=texture->imageData[i];// Temporarily Store The Value At Image Data 'i'
		texture->imageData[i] = texture->imageData[i + 2];// Set The 1st Byte To The Value Of The 3rd Byte
		texture->imageData[i + 2] = temp;// Set The 3rd Byte To The Value In 'temp' (1st Byte Value)
	}
	
	fclose (file);// Close The File // Build A Texture From The Data

	glGenTextures(1, &texture[0].texID);// Generate OpenGL texture IDs 
	glBindTexture(GL_TEXTURE_2D, texture[0].texID);// Bind Our Texture
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);// Linear Filtered
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);// Linear Filtered
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	if (texture[0].bpp==24)// Was The TGA 24 Bits
	{
		type=GL_RGB;// If So Set The 'type' To GL_RGB
	} 
	glTexImage2D(GL_TEXTURE_2D, 0, type, texture[0].width, texture[0].height, 0, type, GL_UNSIGNED_BYTE, texture[0].imageData); 
	return true;// Texture Building Went Ok, Return True
}
