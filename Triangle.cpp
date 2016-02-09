#include "Triangle.h"
#include "Matrix34.h"


Triangle::Triangle()
{

}


bool Triangle::Intersect(const Ray &ray, Intersection &hit) const
{
	bool success = false;
	Vector3 a = Vtx[0]->Position;
	Vector3 b = Vtx[1]->Position;
	Vector3 c = Vtx[2]->Position;
	Vector3 d = ray.Direction;
	Vector3 p = ray.Origin;

	Vector3 normal;
	normal.Cross((b-a), (c-a));

	float detM = (-d).Dot(normal);
	float t;
	float alpha;
	float beta;
	Vector3 temp;

	t = ((p - a).Dot(normal)) / detM;

	temp.Cross(p-a, c-a);
	alpha = (-d).Dot(temp) / detM;

	Vector3 temp2;
	temp2.Cross(b-a, p-a);
	beta = (-d).Dot(temp2) / detM;


	if( (alpha > 0) && (beta > 0) && ((alpha + beta) < 1) && t > 0.0001 && t < hit.HitDistance)
	{
		success = true;
		normal.Normalize();
		hit.Normal = normal;
		hit.Position = ray.Origin + ray.Direction * t;
		hit.HitDistance = t;
		hit.Mtl = Mtl;
		hit.texture = 0xBAADF00D;

		float gamma = 1 - alpha - beta;
		Vector3 texCoord;

		texCoord = Vtx[0]->TexCoord * gamma  + Vtx[1]->TexCoord * alpha + Vtx[2]->TexCoord * beta;
		hit.TexCord = texCoord;

		

	}


	
	hit.TangentU.Cross(Vector3::YAXIS, hit.Normal);
	if(hit.TangentU.Magnitude() < 0.00001)
	{
		hit.TangentU.Cross(Vector3::XAXIS, hit.Normal);
	}


	hit.TangentU.Normalize();
	hit.TangentV.Cross(hit.Normal, hit.TangentU);


	return success;
}


////////////////////////////////////////////////////////////////////////

Vertex Triangle::GetVtx(int i)
{

	return *Vtx[i];
}