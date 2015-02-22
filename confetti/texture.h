class textureclass
{
public:
	bool LoadTGA(int number, char *filename);
	void BuildFont();
	void KillFont(void);
	char unsigned *imageData;
	void glPrint(float size, int x, int y, int set, const char *string, ...);
	void CaptureScreen(void);
	bool SCREENSHOT_WriteTGA(const char *fname,int w, int h);
	unsigned int texture[1];
	int unsigned texID;// Texture ID Used To Select A Texture
private:
// Image Data (Up To 32 Bits)
	int unsigned bpp;// Image Color Depth In Bits Per Pixel
	int unsigned width;// Image Width
	int unsigned height;// Image Height
	int base;
	int loop;
};
