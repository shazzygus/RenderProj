#include "Camera.h"
#include <math.h>
#include <time.h>
#include "RayTrace.h"


PerlinNoise Camera::noise;

Camera::Camera()
{
	XRes = 400;
	YRes = 300;
	VerticalFOV = 40;
	Aspect = (float)1.33;
	
	noise.GenerateNoise();
	noise.GenerateNoise3D();
	printf("noise generated\n");
}

void Camera::SetFOV(float f)
{
	VerticalFOV = f;
	VerticalFOV = VerticalFOV * (PI / 180);
}

void Camera::SetAspect(float a)
{
	Aspect = a;
}



void Camera::SetResolution(int x,int y)
{
	XRes = x;
	YRes = y;
}



void Camera::LookAt(Vector3 &pos,Vector3 &target)
{
	Vector3 up(0,1,0);

	WorldMatrix.d = pos;
	WorldMatrix.c = WorldMatrix.d - target;
	WorldMatrix.c.Normalize();
	WorldMatrix.a.Cross(up, WorldMatrix.c);
	WorldMatrix.a.Normalize();
	WorldMatrix.b.Cross(WorldMatrix.c, WorldMatrix.a);

}

void Camera::SetSuperSample(int num)
{
	numSamples = num;

}




void Camera::Render(Scene &s)
{
	clock_t start = clock();
	BMP = new Bitmap(XRes, YRes);
	HorizontalFOV = 2 * atan (Aspect * tan( VerticalFOV / 2));

	//Generate the Rays
	Vector3 a = WorldMatrix.a;
	Vector3 b = WorldMatrix.b;
	Vector3 c = WorldMatrix.c;
	Vector3 d = WorldMatrix.d;

	Vector3 top_left = d - c - (tan (VerticalFOV / 2)*b) - (tan (HorizontalFOV / 2 )* a)  ;
	Vector3 top_right =  d - c -  (tan (VerticalFOV / 2)*b) + (tan (HorizontalFOV / 2 )*a ) ;
	Vector3 bottom_left = d - c + (tan (VerticalFOV / 2)*b) - (tan (HorizontalFOV / 2 )*a ) ;
	Vector3 bottom_right =  d - c +(tan (VerticalFOV / 2)*b) +( tan (HorizontalFOV / 2 )*a ) ;



	Ray principleRay;
	principleRay.Origin = WorldMatrix.d;
	RayTrace ray(s);
	ray.loadTexture("dragonmesh.bmp");
	ray.loadTexture2("mesh24.bmp");
	

	for( int y = 0; y < YRes; y++)
	{
		float yStep = (0.5 + y) / (float)YRes;
		Vector3 left = (1-yStep)*top_left + bottom_left*yStep;
		Vector3 right = (1 - yStep)*top_right + bottom_right*yStep;

		for( int x = 0; x < XRes; x++)
		{
			float xStep = (0.5 + x) / (float)XRes;
		
			Vector3 pixelLocation = (1-xStep)*left + right*xStep;
		
			principleRay.Direction =  pixelLocation - principleRay.Origin;
			principleRay.Direction.Normalize();
			principleRay.x = x;
			principleRay.y = y;

			Color color_avg(0,0,0);
			for(int i = 0; i < numSamples; i++)
			{
				//
				Intersection hitInfo;
				ray.TraceRay(principleRay, hitInfo,1);
				color_avg.Add(hitInfo.Shade);
			
			}

			color_avg.Scale(1.0f/(float)numSamples);

			BMP->SetPixel(x,y,color_avg.ToInt());
		
		}

	}

	clock_t end = clock();
	printf("Render: %f\n", ((float)(end - start)) / CLOCKS_PER_SEC);

}




void Camera::SaveBitmap(char *filename)
{
	BMP->SaveBMP(filename);
}