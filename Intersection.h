////////////////////////////////////////
// Intersection.h
////////////////////////////////////////

#ifndef CSE168_INTERSECTION_H
#define CSE168_INTERSECTION_H

#include "Vector3.h"
#include "Color.h"

class Material;

////////////////////////////////////////////////////////////////////////////////

class Intersection {
public:
	Intersection()			{HitDistance=1e10; Mtl=0; texture = -1;}

public:
	// Ray intersection data
	float HitDistance;
	Vector3 Position;
	Vector3 Normal;
	Material *Mtl;
	Vector3 TangentU, TangentV;
	Vector3 TexCord;
	

	// Shaded color
	Color Shade;
	Color texColor;
	int texture;
};

////////////////////////////////////////////////////////////////////////////////

#endif
