#include "vector.h"
#include <math.h>

void vectorclass::Normalize(float *XVector, float *YVector, float *ZVector)
{
	float length = sqrt((*XVector* *XVector)+(*YVector* *YVector)+(*ZVector* *ZVector));
	*XVector/=length;
	*YVector/=length;
	*ZVector/=length;
}

void vectorclass::Lighting(float *red, float *green, float *blue, float Lightx, float Lighty, float Lightz, float CurrentXVector,
float CurrentYVector, float CurrentZVector, float CurrentXRayStart, float CurrentYRayStart, float CurrentZRayStart, float spherecolorred,
float spherecolorgreen, float spherecolorblue)
{
	float X2LightVector, Y2LightVector, Z2LightVector;
	//float length = sqrt((XLightVector*XLightVector)+(YLightVector*YLightVector)+(ZLightVector*ZLightVector));
	//XLightVector/=length;
	//YLightVector/=length;
	//ZLightVector/=length;
	X2LightVector = Lightx-CurrentXRayStart;
	Y2LightVector = Lighty-CurrentYRayStart;
	Z2LightVector = Lightz-CurrentZRayStart;
	vectorclass::Normalize(&X2LightVector, &Y2LightVector, &Z2LightVector);
	X2LightVector=(CurrentXVector+X2LightVector)/2;
	Y2LightVector=(CurrentYVector+Y2LightVector)/2;
	Z2LightVector=(CurrentZVector+Z2LightVector)/2;
	vectorclass::Normalize(&X2LightVector, &Y2LightVector, &Z2LightVector);
	float FinalLight = (CurrentXVector*X2LightVector)+(CurrentYVector*Y2LightVector)+(CurrentZVector*Z2LightVector);
	float specular = pow(FinalLight,40);
	*red =  1.0*(spherecolorred*FinalLight+specular);
	*green = 1.0*(spherecolorgreen*FinalLight+specular);
	*blue = 1.0*(spherecolorblue*FinalLight+specular);
}

float vectorclass::RotateX(float angle, float X)
{
	return 300+(cosf(angle/57.29f) - sinf(angle/57.29f)*(X-300));
}

float vectorclass::RotateZ(float angle, float Z)
{
	return 150+(sinf(angle/57.29f) + cosf(angle/57.29f)*(Z-150));
}

	//losx = X+(cosf(angle/57.29f) - sinf(angle/57.29f)*radius);
	//losy = Y+(sinf(angle/57.29f) + cosf(angle/57.29f)*radius);
