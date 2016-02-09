#ifndef TREE_NODE_H
#define TREE_NODE_H

#include "Triangle.h"

class BoxTreeNode {
public:
	BoxTreeNode();
	~BoxTreeNode();
	bool Intersect(const Ray &ray, Intersection &hit);
	bool IntersectVolume(const Ray &ray, Intersection &hit);//, float &t);
	bool IntersectChildren(const Ray &ray, Intersection &hit);
	void Construct(int count, Triangle ** tri);

private:
	Vector3 boxMin;
	Vector3 boxMax;
	BoxTreeNode *Child1, *Child2;
	BoxTreeNode *children[2];
	Triangle *TriLeaf[10];
	int numChild;
	int num_tris;
};


#endif