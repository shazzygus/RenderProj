#ifndef RAY_TRACE_H
#define RAY_TRACE_H

#include "Scene.h"
#include "BitmapLoader.h"
class RayTrace {
public:	
	RayTrace(Scene &s);
	bool TraceRay(const Ray &ray, Intersection &hit, int depth=1); 
	bool loadTexture(const char* filename);
	bool loadTexture2(const char* filename);
	Color ScatterCloud(Ray ray, Vector3 start, Vector3 directions, Vector3 end);
	double interpolate(Vector3);
private: 
	Scene *Scn; 
	Color * color_data;
	Color * color_data2;
	CBitmap texture;
	CBitmap texture2;
 
	// Settings 
	int MaxDepth; 
	
	// Statistics 
	int PrimaryRays; 
	int SecondaryRays; 
	int ShadowRays; 



};


#endif