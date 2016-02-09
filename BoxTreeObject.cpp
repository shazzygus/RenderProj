#include "BoxTreeObject.h"
#include <time.h>

BoxTreeObject::BoxTreeObject()
{
	RootNode = new BoxTreeNode();

}

BoxTreeObject::~BoxTreeObject()
{

}


void BoxTreeObject::Construct(MeshObject &mesh)
{
	clock_t start = clock();
	Triangle *ptr = mesh.Triangles;
	Triangle **tris=new Triangle*[mesh.GetNumTriangles()];
	for(int i = 0; i < mesh.GetNumTriangles(); i++)
	{
		tris[i] = &ptr[i];

	}

	RootNode->Construct(mesh.GetNumTriangles(), tris);

	delete []tris;
	clock_t  end = clock();
	printf("Tree Construct: %f\n", ((float)(end - start)) / CLOCKS_PER_SEC);

}


bool BoxTreeObject::Intersect(const Ray &ray, Intersection &hit)
{
	Intersection volHit;
	if(RootNode->IntersectVolume(ray, volHit))
	{
		if(RootNode->IntersectChildren(ray, hit))
		{
			return true;
		}
	}

	
    return false;

}