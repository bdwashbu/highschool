#ifndef __TEXTURE__
#define __TEXTURE__

bool LoadTGA(char *filename, bool linear);
void BuildFont();
void KillFont();
void glPrint(float size, float x, float y, int set, const char *string, ...);
void CaptureScreen(void);
bool SCREENSHOT_WriteTGA(const char *fname,int w, int h);

#endif
