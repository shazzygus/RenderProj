#ifndef PERLIN_NOISE_H
#define PERLIN_NOISE_H

#include "Bitmap.h"
#include <vector>
#include "Color.h"
#define noise_width 1000
#define noise_height 1000
#define noise_depth 100

class PerlinNoise {
public:
	PerlinNoise();
	void GenerateNoise();
	double SmoothNoise(double x, double y);
	double Turbulence(double x, double y, double size);
	double CloudExposure(double x);
	double CloudExposure3D(double x);
	double getValue(int x, int y);

	void GenerateNoise3D();
	double SmoothNoise3D(double x, double y, double z);
	double Turbulence3D(double x, double y, double z, double size);
	double getValue3D(int x, int y,int z);

//	double GenerateSample3D();



	void SaveNoise();
private:
//	double noise[noise_width][ noise_height];
	double ** noise;
	double *** noise3D;
	double ** smoothed;
	double *** smoothed3D;
	Bitmap* BMP;

};
#endif