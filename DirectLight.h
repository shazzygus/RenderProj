#ifndef CSE168_DIRECT_H
#define CSE168_DIRECT_H

#include "Light.h"

class DirectLight: public Light {
public: 
	DirectLight() {} 
	float Illuminate(const Vector3 &pos, Color &col, Vector3 &toLight, Vector3 &ltPos) { 
		toLight = -Direction; 
		col = BaseColor; 
		ltPos = pos - (1000000.0 * Direction); // Create virtual position 
		return Intensity; 
	} 
	void SetDirection(Vector3 &dir) {Direction=dir; Direction.Normalize();} 

private: 
	Vector3 Direction; 
}; 

#endif