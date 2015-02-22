#ifndef _H_TERRAIN_H
#define _H_TERRAIN_H

class terrainclass
{
public:
	terrainclass();
	void DrawTerrain();
	void LowestPoint();
private:
	void Normalize(float &XVector, float &YVector, float &ZVector);
	float lowestpoint;
	float NormalX[50][50];
	float NormalY[50][50];
	float NormalZ[50][50];
	float X[50][50];
	float Y[50][50];
	float Z[50][50];
};

#endif