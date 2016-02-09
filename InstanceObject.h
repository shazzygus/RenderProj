#ifndef CSE168_INSTANCE_H
#define CSE168_INSTANCE_H


#include "Matrix34.h"
#include "MeshObject.h"
#include "AshikhminMaterial.h"
class InstanceObject: public Object{


public: 
	InstanceObject(); 
	InstanceObject(Object &obj);
	bool Intersect(const Ray &ray, Intersection &hit); 
	void SetChild(Object &obj); 
	void SetMatrix(Matrix34 &mtx); 
	void SetMaterial(Material *mtl);
	void SetTexture(int x);


private: 
	Matrix34 Matrix; 
	Matrix34 Inverse; // Pre-computed inverse of Matrix 
	Object *Child; 
	Material *material;
	int texture;



};


#endif