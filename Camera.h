#ifndef CSE168_CAMERA_H
#define CSE168_CAMERA_H

#include "Matrix34.h"
#include "Bitmap.h"
#include "Scene.h"
#include "PerlinNoise.h"
class Camera {
public: 
	Camera(); 

	void SetFOV(float f); 
	void SetAspect(float a); 
	void SetResolution(int x,int y); 
	void LookAt(Vector3 &pos,Vector3 &target);
	void Render(Scene &s); 
	void SaveBitmap(char *filename); 
	void SetSuperSample(int num);
	static PerlinNoise noise;
private: 
	int XRes,YRes; 
	Matrix34 WorldMatrix; 
	float VerticalFOV; 
	float HorizontalFOV;
	float Aspect; 
	Bitmap *BMP; 
	int numSamples;





};


#endif