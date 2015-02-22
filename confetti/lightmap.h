#include "confetti.h"

class lightmapclass
{
public:
	lightmapclass();
	void RenderLightMap(confetticlass *confetti);
	static void DrawLightMapDot(float Rotation, float Angle);
	static void MoveLightMap(float scalar);
	void WriteLightMaps(int texID, unsigned char *imageData, confetticlass *confetti, float scalar);
	float Size;
	static float X;
	static float Y;
	static float Z;
	int TexelWidth;
	float Height;
	float Width;
	float BottomLeftPoint;
	static float Angle;
	static bool YRaise;
	static bool Skipframe;
private:
	float World2Texel(float coord);
	float Texel2World(float coord);
};