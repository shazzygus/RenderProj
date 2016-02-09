#include "InstanceObject.h"
#include "AshikhminMaterial.h"

InstanceObject::InstanceObject()
{
	material = 0;
	texture = -1;
}


InstanceObject::InstanceObject(Object &obj)
{

	Child = &obj;
}

bool InstanceObject::Intersect(const Ray &ray, Intersection &hit)
{
	Ray ray2;
	Inverse.Transform(ray.Origin, ray2.Origin); 
	Inverse.Transform3x3(ray.Direction, ray2.Direction); 
	if(Child->Intersect(ray2, hit)==false) return false; 
	Matrix.Transform(hit.Position, hit.Position); 
	Matrix.Transform3x3(hit.Normal, hit.Normal); 
	hit.HitDistance=ray.Origin.Distance(hit.Position);
	hit.Mtl = material;
	if(hit.texture == 0xBAADF00D)
	{
		hit.texture = texture;
	}
	return true; 

}




 void InstanceObject::SetChild(Object &obj)
 {
	 Child = &obj;
 }


 void InstanceObject::SetTexture(int x)
 {
	 texture = x;
 }

 void InstanceObject::SetMatrix(Matrix34 &mtx)
 {
	 Matrix = mtx;
	 Inverse=Matrix;
	 Inverse.Inverse();
 }


 void InstanceObject::SetMaterial(Material *mtl)
 {
	 material = mtl;
 }