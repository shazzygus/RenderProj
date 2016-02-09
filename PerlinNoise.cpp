#include "PerlinNoise.h"

#include <stdlib.h>  



PerlinNoise::PerlinNoise()
{


}


void PerlinNoise::GenerateNoise()
{
   noise = new double*[noise_height];
   smoothed = new double*[noise_height];
   for(int i = 0; i < noise_height; i++)
   {
	   noise[i] = new double[noise_width];
	   smoothed[i] = new double[noise_width];
   }


   BMP = new Bitmap(noise_width, noise_height);
   Color color;
   for(int x = 0; x < noise_width; x++)
   {
       for(int y = 0; y < noise_height; y++)
	   {
			noise[x][y] = (rand() % 32768) / 32768.0;
			double smooth = Turbulence(x, y, 200);
			smooth = CloudExposure(smooth);
			smoothed[x][y] = smooth;
			color.Set(smooth, smooth , smooth);
			BMP->SetPixel(x,y,color.ToInt());
		
	   }
   }
}

double PerlinNoise::CloudExposure(double v)
{
	  v *= 255;
	  double CloudCover = 100;
	  double CloudSharpness = .90987;

	  double c = v - CloudCover;
      if (c < 0 )
		  c=0;
 
     double CloudDensity = 255 - ((pow(CloudSharpness, c)) * 255);

     return CloudDensity / 255;
}


double PerlinNoise::SmoothNoise(double x, double y)
{
   //get fractional part of x and y
   double fractX = x - int(x);
   double fractY = y - int(y);
   
   //wrap around
   int x1 = (int(x) + noise_width) % noise_width;
   int y1 = (int(y) + noise_height) % noise_height;
   
   //neighbor values
   int x2 = (x1 + noise_width - 1) % noise_width;
   int y2 = (y1 + noise_height - 1) % noise_height;

   //smooth the noise with bilinear interpolation
   double value = 0.0;
   value += fractX       * fractY       * noise[x1][y1];
   value += fractX       * (1 - fractY) * noise[x1][y2];
   value += (1 - fractX) * fractY       * noise[x2][y1];
   value += (1 - fractX) * (1 - fractY) * noise[x2][y2];

   return value;

}


double PerlinNoise::Turbulence(double x, double y, double size)
{
    double value = 0.0, initialSize = size;
    
    while(size >= 1)
    {
        value += 0.5 * SmoothNoise(x / size, y / size) * size;
        size /= 2.0;
    }
    
	//value = (128 + (value * 255)) / 255;

    return(value / initialSize);
}

///////////////////////////////////////////////////////////////////////////
double PerlinNoise::getValue(int x, int y)
{
	
	return smoothed[x][y];
}

///////////////////////////////////////////////////////////////
void PerlinNoise::SaveNoise()
{
	
	BMP->SaveBMP("3D.bmp");

}


/////////////////////////////////////// VOLUMETRIC ////////////////////////////////////////////////
void PerlinNoise::GenerateNoise3D()
{
   // noise = new double[noise_width][noise_height];
   noise3D = new double**[noise_height];
   smoothed3D = new double**[noise_height];
   for(int i = 0; i < noise_height; i++)
   {
	   noise3D[i] = new double*[noise_width];
	   smoothed3D[i] = new double*[noise_width];
	   for(int j =0; j < noise_width; j++)
	   {
		   noise3D[i][j] = new double[noise_depth];
		   smoothed3D[i][j] = new double[noise_depth];
	   }
   }




   BMP = new Bitmap(noise_width, noise_height);
   Color color;
   for(int x = 0; x < noise_width; x++)
   {
       for(int y = 0; y < noise_height; y++)
	   {
		   for(int z = 0; z < noise_depth; z++)
		   {
			noise3D[x][y][z] = (rand() % 32768) / 32768.0;
			double smooth = Turbulence3D(x, y, z, 250);
			smooth = CloudExposure3D(smooth);
			smoothed3D[x][y][z] = smooth;
			color.Set(smooth, smooth , smooth);
			BMP->SetPixel(x,y,color.ToInt());
		   }
		
	   }
   }

   BMP->SaveBMP("clouds3D.bmp");
}

//////////////////////////////////////////////////////////////////////////////////

double PerlinNoise::Turbulence3D(double x, double y, double z, double size)
{
    double value = 0.0, initialSize = size;
   
    while(size >= 1)
    {
        value += SmoothNoise3D(x / size, y / size, z / size) * size;
        size /= 2.0;
    }
   
    return(value / initialSize);
}

//////////////////////////////////////////////////////////////////////////////////
double PerlinNoise::SmoothNoise3D(double x, double y, double z)
{ 
   //get fractional part of x and y
   double fractX = x - int(x);
   double fractY = y - int(y);
   double fractZ = z - int(z);   
  
   //wrap around
   int x1 = (int(x) + noise_width) % noise_width;
   int y1 = (int(y) + noise_height) % noise_height;
   int z1 = (int(z) + noise_depth) % noise_depth;
  
   //neighbor values
   int x2 = (x1 + noise_width - 1) % noise_width;
   int y2 = (y1 + noise_height - 1) % noise_height;
   int z2 = (z1 + noise_depth - 1) % noise_depth;

   //smooth the noise with bilinear interpolation
   double value = 0.0;
   value += fractX       * fractY       * fractZ       * noise3D[x1][y1][z1];
   value += fractX       * (1 - fractY) * fractZ       * noise3D[x1][y2][z1];
   value += (1 - fractX) * fractY       * fractZ       * noise3D[x2][y1][z1];
   value += (1 - fractX) * (1 - fractY) * fractZ       * noise3D[x2][y2][z1];
															  
   value += fractX       * fractY       * (1 - fractZ) * noise3D[x1][y1][z2];
   value += fractX       * (1 - fractY) * (1 - fractZ) * noise3D[x1][y2][z2];
   value += (1 - fractX) * fractY       * (1 - fractZ) * noise3D[x2][y1][z2];
   value += (1 - fractX) * (1 - fractY) * (1 - fractZ) * noise3D[x2][y2][z2];

   return value;
}
/////////////////////////////////////////////////////////////////////////////////////////
double PerlinNoise::CloudExposure3D(double v)
{
	  v *= 255;
	  double CloudCover = 210;
	  double CloudSharpness = .95987;

	  double c = v - CloudCover;
      if (c < 0 )
		  c=0;
 
     double CloudDensity = 255 - ((pow(CloudSharpness, c)) * 255);

     return CloudDensity / 255;
}


/////////////////////////////////////////////////////////////////////////////////////////////
double PerlinNoise::getValue3D(int x, int y, int z)
{
	
	return smoothed3D[x][y][z];
}

/////////////////////////////////////////////////////////////////////////////////////////////
/*
double PerlinNoise::GenerateSample3D()
{

	noise3D[x][y][z] = (rand() % 32768) / 32768.0;
			double smooth = Turbulence3D(x, y, z, 250);
			smooth = CloudExposure3D(smooth);

}*/