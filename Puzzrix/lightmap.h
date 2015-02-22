#ifndef _LIGHTMAP_H_
#define _LIGHTMAP_H_

class lightmapclass
{
public:
	lightmapclass();
	lightmapclass(float tempX, float tempY);
	lightmapclass *next;
	void DrawLightmap(float scalar);
	bool Alive;
private:
	float X, Y;
	float Size;
	float Life;
};

#endif