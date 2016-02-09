////////////////////////////////////////
// MeshObject.h
////////////////////////////////////////

#ifndef CSE168_MESHOBJECT_H
#define CSE168_MESHOBJECT_H

#include "Object.h"
#include "Triangle.h"
#include <vector>
#include "ObjLoader.h"
#include "BitmapLoader.h"
////////////////////////////////////////////////////////////////////////////////

class MeshObject:public Object {
public:
	MeshObject();
	~MeshObject();

	bool Intersect(const Ray &ray,Intersection &hit);

	void MakeBox(float x,float y,float z,Material *mtl=0);
	bool LoadPLY(const char *filename, Material *mtl);
	void setData(std::vector<tinyobj::shape_t> shapes, Material * mtl);//, const char* filename);
	bool loadTexture(const char* filename);
	void Smooth();
	int GetNumTriangles();
	Triangle* GetTriangles();
	Triangle *Triangles;
private:
	int NumVertexes,NumTriangles;
	Vertex *Vertexes;
	Color * color_data;
	CBitmap texture;
	
};

////////////////////////////////////////////////////////////////////////////////

#endif
