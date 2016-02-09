#ifndef BOXTREEOBJ_H
#define BOXTREEOBJ_H

#include "MeshObject.h"
#include "BoxTreeNode.h"


class BoxTreeObject: public Object {
public: 
	BoxTreeObject(); 
	~BoxTreeObject(); 
	void Construct(MeshObject &mesh); 
	bool Intersect(const Ray &ray, Intersection &hit); 

private: 
	BoxTreeNode *RootNode; 
}; 





#endif