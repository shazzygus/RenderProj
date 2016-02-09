#ifndef ASHIKHMIN_MATERIAL
#define ASHIKHMIN_MATERIAL

#include "Material.h"
#include <math.h>
#include <time.h>
#include <cstdlib>
#include "Matrix34.h"

class AshikhminMaterial : public Material {
public:
	
	void SetSpecularLevel(float lvl) { specularLevel = lvl;}
	void SetSpecularColor(Color clr) { specularColor = clr;}
	void SetDiffuseLevel(float lvl) {diffuseLevel = lvl;}
	void SetDiffuseColor(Color clr) {diffuseColor = clr;}
	void SetRoughness(float val, float val2) {roughU = val, roughV = val2;}
	virtual void GenerateSample(Color &col,const Vector3 &in, Vector3 &out,const Intersection &hit)
	{
		float nu = roughU;
		float nv = roughV;

		float r1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float r2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float r3 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float r4 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

		if(in.Dot(hit.Normal) < 0)
			return;

		if( r1 < specularLevel)
		{
			float phi = atan(sqrt( (nu + 1) / (nv + 1) ) * tan ( (PI * r2)/2 ));
			float denom = (nu * pow(cos(phi), 2)) + (nv * pow(sin(phi),2)) + 1;
			float costheta = pow((1 - r4), ( 1 / denom));
			float sintheta = sqrt(1.0f - costheta*costheta);

			float x = sintheta * cos(phi);
			float y = costheta;
			float z = sintheta * sin(phi);

			Vector3 h(x,y,z);
			Vector3 k1;
			Vector3 k2;


            if( r3 < 0.5) {
				h.x *= -1.0;
            }

            if( (0.0 < r3 && r3 < 0.25) || (0.5 < r3 && r3 < .75) ) {
				h.z *= -1.0;
            }

			h = h.x * hit.TangentU + h.y * hit.Normal + h.z * hit.TangentV;
			h.Normalize();

			k1 = in;
			k2 = -k1 + 2 * (k1.Dot(h)) * h;

			if(k2.Dot(hit.Normal) < 0.0)
			{
				out = -1 * hit.Normal;
				return;
			}

			out = k2;
			out.Normalize();
			col = specularColor;

		}

		else //Cosine Weighted Hemisphere
		{

			float s = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			float t = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			float u = 2 * PI * s;
			float v = sqrt( 1 - t );

			float px = v * cos(u);
			float py = sqrt(t);
			float pz = v * sin(u);

			out = px * hit.TangentU + py * hit.Normal + pz * hit.TangentV;
			out.Normalize();

			col = diffuseColor;

		}		

	}



	virtual void ComputeReflectance(Color &col, const Vector3 &in, const Vector3 &out, const Intersection &hit) {

	


	Vector3 n = hit.Normal;
	Vector3 l = out;
    Vector3 v = in;
	Vector3 h =  l + v;
	h.Normalize();
 
    // Define the coordinate frame
    Vector3 epsilon( 0.0f, 1.0f, 0.0f );
	Vector3 tangent, bitangent;
	tangent.Cross(n, epsilon);
	tangent.Normalize();
    bitangent.Cross(n, tangent);
	bitangent.Normalize();

	float VdotN = v.Dot(n);
    float LdotN = l.Dot(n); 
    float HdotN = h.Dot(n);
    float HdotL = h.Dot(l);
    float HdotT = h.Dot(tangent);
    float HdotB = h.Dot(bitangent);
	float HdotU = h.Dot(hit.TangentU);
	float HdotV = h.Dot(hit.TangentV);

	if(VdotN <= 0 || LdotN <= 0)
		return;
 
    float Nu = roughU;
    float Nv = roughV;
 
    // Compute the diffuse term
	float Pd = (28.0f * diffuseLevel) / ( 23.0f * 3.14159f );
	Pd *= (1.0f - specularLevel);
    Pd *= (1.0f - pow(1.0f - (LdotN / 2.0f), 5.0f));
    Pd *= (1.0f - pow(1.0f - (VdotN / 2.0f), 5.0f));

 
    // Compute the specular term
    float ps_num_exp = Nu * HdotU * HdotU + Nv * HdotV * HdotV;
    ps_num_exp /= (1.0f - HdotN * HdotN);
 
    float Ps_num = sqrt( (Nu + 1) * (Nv + 1) );
    Ps_num *= pow( HdotN, ps_num_exp );
 
    float Ps_den = 8.0f * 3.14159f * HdotL;
    Ps_den *= Max( LdotN, VdotN );

    float Ps = specularLevel * ((float) Ps_num / (float) Ps_den);

    Ps *= ( specularLevel + (1.0f - specularLevel) * pow( 1.0f - HdotL, 5.0f ) );
 

	Vector3 dC(diffuseColor.getRed(), diffuseColor.getGreen(), diffuseColor.getBlue());
	Vector3 sC(specularColor.getRed(), specularColor.getGreen(), specularColor.getBlue());

	dC = dC * Pd;
	sC = sC * Ps;


	Vector3 fc = dC + sC;

	Color finalColor;
	finalColor.Set(fc.x, fc.y, fc.z);

	col = finalColor;
        
	}

private:
	float specularLevel;
	Color specularColor;
	float diffuseLevel;
	Color diffuseColor;
	float roughU, roughV;
	Color finalColor;


};



#endif