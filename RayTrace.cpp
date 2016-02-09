#include "RayTrace.h"
#include "Material.h"
#include "Camera.h"
#include <math.h>

RayTrace::RayTrace(Scene &s)
{
	Scn = &s;
	MaxDepth = 5;
	ShadowRays = Scn->GetNumLights();

}

bool RayTrace::TraceRay(const Ray &ray, Intersection &hit, int depth)
{

		Color background(0,0,0);
		Color orig;
			if(depth == 1) {
					background = ScatterCloud(ray, ray.Origin, ray.Direction, hit.Position);
			}



		if (Scn->Intersect(ray, hit) == false)	
		{
			if(depth == 1)
			{
        
				double smooth = Camera::noise.getValue(ray.x,ray.y);
				background.Add(Color(smooth,smooth,smooth));
				orig = background;
				background.Inverse(background);
				background.Add(Color(1,1,1));
				background.Multiply(Color(171.0/255.0, 210.0/255.0, 1));
				orig.Add(background);
				hit.Shade = orig;
			
			}
			else
			
			hit.Shade = Scn->GetSkyColor();
			return false;
		}

		else 
			{

				float r = 0;
				float g = 0;
				float b = 0;
				Color col;
				Vector3 toLight;
				Vector3 ltPos;
				float bright;
				hit.Shade = Color::BLACK;
				for(int i = 0; i < Scn->GetNumLights(); i++)
				{
				
					bright = Scn->GetLight(i).Illuminate(hit.Position, col, toLight, ltPos);


					//Only check for shadows if the intensity isnt 0 or if the angle betweeb the hit
					// normal and light direction is greater than 90 degrees
					if( bright != 0 || hit.Normal.Dot(toLight) > 0)
					{			
						Ray shadowRay;
						shadowRay.Origin = hit.Position ;
						shadowRay.Direction = toLight;
						Intersection hitInfoShadow;
						double light_dist =  ltPos.Magnitude();
						
						hitInfoShadow.HitDistance = light_dist;
						toLight.Normalize();

						
						
						
						if( !(Scn->Intersect(shadowRay, hitInfoShadow)))
						{
							Color refColor;

							hit.Mtl->ComputeReflectance(refColor, -ray.Direction, toLight, hit);
							col.Multiply(refColor);
						    col.Scale(hit.Normal.Dot(toLight));
							hit.Shade.Add(col);
						}

					}
				
				}

		

				if(depth == MaxDepth)
					return true;
		

				

				
				Ray secondaryRay;
				Color sampleColor;
				Intersection newHit;
				newHit.Shade = Color::BLACK;
				sampleColor = Color::BLACK;
				secondaryRay.Origin = hit.Position;


				
				hit.Mtl->GenerateSample(sampleColor, -ray.Direction, secondaryRay.Direction, hit);
				if(hit.Normal.Dot(secondaryRay.Direction) > 0.00001)
				{
					TraceRay(secondaryRay, newHit, depth + 1);
					sampleColor.Multiply(newHit.Shade);
					if(sampleColor.getRed() > 0 && sampleColor.getGreen() > 0 && sampleColor.getBlue() > 0)
					{
						hit.Shade.Add(sampleColor);
					}
				}
				
			}

			
		
		float u = hit.TexCord.x;
		float v = hit.TexCord.y;

		float u_ = u * (texture.GetWidth() - 1);
		float v_ = v * (texture.GetWidth() - 1);

		int s = (u_ >= 0) ? (int)(u_+ 0.5) : (int)(u_ - 0.5);
		int t = (v_ >= 0) ? (int)(v_+ 0.5) : (int)(v_ - 0.5);
		
		int index = s + t * texture.GetWidth();
		


		

		if(hit.texture == 0)
		{
			Color pixel;
			pixel = color_data[index];
			
			hit.Shade.Multiply(pixel);
		}

		else if(hit.texture == 1)
		{
			Color pixel;
			pixel = color_data2[index];
			hit.Shade.Multiply(pixel);
		}
	
		background.Scale(0.5);
		hit.Shade.Add(background);
		return true;

}


///////////////////////////////////////////////////////////////////////////////////

bool RayTrace::loadTexture(const char* filename)
{
	
	printf("loading texture\n");
	texture.Load(filename);

	int numpixels = texture.GetHeight() * texture.GetWidth();

	RGBA *pixels;
	pixels = (RGBA*)texture.GetBits();
	color_data = new Color[numpixels];
	

	for(int i = 0; i < numpixels; i++)
	{
		float r = pixels[i].Red / (float)255;
		float g = pixels[i].Green / (float)255;
		float b = pixels[i].Blue / (float)255;		
		color_data[i].Set(r,g,b);
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////////

bool RayTrace::loadTexture2(const char* filename)
{
	
	printf("loading texture2\n");
	texture2.Load(filename);

	int numpixels = texture2.GetHeight() * texture2.GetWidth();

	RGBA *pixels;
	pixels = (RGBA*)texture2.GetBits();
	color_data2 = new Color[numpixels];
	

	for(int i = 0; i < numpixels; i++)
	{
		float r = pixels[i].Red / (float)255;
		float g = pixels[i].Green / (float)255;
		float b = pixels[i].Blue / (float)255;		
		color_data2[i].Set(r,g,b);
	}

	return true;
}

/////////////////////////////////////////////////////////
Color RayTrace::ScatterCloud(Ray ray, Vector3 start, Vector3 direction, Vector3 end)
{


	int x = ray.x;
	int y = ray.y;
	int z_end = end.z;
	Color col(0,0,0);
	double noise;
	double total = 0.0;
	Vector3 segment = start;

	for(int z = 0; z < 1; z++)
	{
		segment = segment + direction*z;
		noise = Camera::noise.getValue3D(x,y,z);
		col.Set(noise, noise, noise);

	}

	return col;

}

///////////////////////////////////////////////////////////////////////
double RayTrace::interpolate(Vector3 p)
{

	p.x = fmod(fabs(p.x),1000);
	p.y = fmod(fabs(p.y), 1000);
	p.z = fmod(fabs(p.z), 100);



	int f_x = (int)floor(p.x);
	int f_y = (int)floor(p.y);
	int f_z = (int)floor(p.z);
	int c_x = (int)ceil(p.x);
	int c_y = (int)ceil(p.y);
	int c_z = (int)ceil(p.z);


 

	double c000 = Camera::noise.getValue3D(f_x, f_y, f_z);
	double c001 = Camera::noise.getValue3D(f_x, f_y, c_z);
	double c010 = Camera::noise.getValue3D(f_x, c_y, f_z);
	double c011 = Camera::noise.getValue3D(f_x, c_y, c_z);
	double c100 = Camera::noise.getValue3D(c_x, f_y, f_z);
	double c101 = Camera::noise.getValue3D(c_x, f_y, f_z);
	double c110 = Camera::noise.getValue3D(c_x, c_y, f_z);
	double c111 = Camera::noise.getValue3D(c_x, c_y, c_z);

			
	double c00  = c000 * (1 - p.x) + c100 * p.x;
	double c01  = c010 * (1 - p.x) + c110 * p.x;
	double c10  = c001 * (1 - p.x) + c101 * p.x;
	double c11  = c011 * (1 - p.x) + c111 * p.x;
			
	double c0   = c00 * (1 - p.y) + c10 * p.y;
	double c1   = c01 * (1 - p.y) + c11 * p.y;
				
	
	double c    = c0 * (1 - p.z) + c1 * p.z;            


	return c;

}