class vectorclass
{
public:
	float XVector;
	float YVector;
	float ZVector;
	static void Normalize(float *XVector, float *YVector, float *ZVector);
	static void Lighting(float *red, float *green, float *blue, float Lightx, float Lighty, float Lightz, float CurrentXVector,
	float CurrentYVector, float CurrentZVector, float CurrentXRayStart, float CurrentYRayStart, float CurrentZRayStart,
	float spherecolorred, float spherecolorgreen, float spherecolorblue);
	static float vectorclass::RotateX(float angle, float X);
	static float vectorclass::RotateZ(float angle, float Z);
private:
};
