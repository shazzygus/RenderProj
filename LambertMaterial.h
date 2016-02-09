////////////////////////////////////////
// LambertMaterial.h
////////////////////////////////////////

#ifndef CSE168_LAMBERTMATERIAL_H
#define CSE168_LAMBERTMATERIAL_H

#include <time.h>
#include <cstdlib>

#include "Material.h"

////////////////////////////////////////////////////////////////////////////////

class LambertMaterial:public Material {
public:
	virtual void ComputeReflectance(Color &col, const Vector3 &in, const Vector3 &out, const Intersection &hit) {
		col.Scale(DiffuseColor,1.0f/PI);
	}
	virtual void GenerateSample(Color &col,const Vector3 &in,Vector3 &out,const Intersection &hit) {
			float s = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			float t = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			float u = 2 * PI * s;
			float v = sqrt( 1 - t );

			float px = v * cos(u);
			float py = sqrt(t);
			float pz = v * sin(u);

			out = px * hit.TangentU + py * hit.Normal + pz * hit.TangentV;
			col = DiffuseColor;

	}
	void SetDiffuseColor(Color clr) { DiffuseColor = clr;}

private:
	Color DiffuseColor;
};

////////////////////////////////////////////////////////////////////////////////

#endif
