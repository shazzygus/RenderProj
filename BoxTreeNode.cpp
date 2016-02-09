#include "BoxTreeNode.h"
#include <string>




BoxTreeNode::BoxTreeNode()
{
	boxMin.Set(0,0,0);
	boxMax.Set(0,0,0);
	num_tris = 0;
}


bool BoxTreeNode::Intersect(const Ray &ray, Intersection &hit)
{
	bool success = false;
	if(IntersectVolume(ray, hit))
	{
		if(IntersectChildren(ray, hit))
			success = true;
	}

	return success;
}


bool BoxTreeNode::IntersectVolume( const Ray &ray, Intersection &hit)
{
	Vector3 a = boxMin;
	Vector3 b = boxMax;
	float tx1, tx2, ty1, ty2, tz1, tz2, t_min,t_max;
	Vector3 p = ray.Origin;
	Vector3 d = ray.Direction;
	bool success = false;

	tx1 = (a[0] - p[0]) / d[0];
	tx2 = (b[0] - p[0]) / d[0];

	ty1 = (a[1] - p[1]) / d[1];
	ty2 = (b[1] - p[1]) / d[1];

	tz1 = (a[2] - p[2]) / d[2];
	tz2 = (b[2] - p[2]) / d[2];

	t_min = Max(  Min(tx1,tx2), Max(Min(ty1, ty2), Min(tz1, tz2))  );
	t_max = Min(  Max(tx1,tx2), Min(Max(ty1, ty2), Max(tz1, tz2))  );

	if(t_min <= t_max)
	{	
		if(t_min < 0) {
			hit.HitDistance = 0;
		}
		else {
			if(t_min < hit.HitDistance)
			{
				hit.HitDistance = t_min;
			}
		}

		success = true;
	}

	return success;
}


bool BoxTreeNode::IntersectChildren(const Ray &ray, Intersection &hit)
{

	bool success = false;

	//Leaf Node
	if(children[0] == NULL && children[1] == NULL)
	{
		for(int i = 0; i < num_tris; i++)
		{
			if(TriLeaf[i]->Intersect(ray, hit)) 
				success = true;
		}

		return success;
	}

	
	Intersection volHit[2];
	bool childsuccess[2];
	volHit[0].HitDistance = hit.HitDistance;
	volHit[1].HitDistance = hit.HitDistance;
	for(int i = 0; i < 2; i++)
	{
		childsuccess[i] = false;
		childsuccess[i] = children[i]->IntersectVolume(ray, volHit[i]);
	}


	if(childsuccess[0] && childsuccess[1]) {
		if(volHit[0].HitDistance <= volHit[1].HitDistance)
		{
			if(volHit[0].HitDistance <= hit.HitDistance)
			{
				if(children[0]->IntersectChildren(ray, hit)) success = true;
			}
			if(volHit[1].HitDistance <= hit.HitDistance)
			{
				if(children[1]->IntersectChildren(ray, hit)) success = true;
			}
		}
		else
		{
			if(volHit[1].HitDistance <= hit.HitDistance)
			{
				if(children[1]->IntersectChildren(ray, hit)) success = true;
			}
			if(volHit[0].HitDistance <= hit.HitDistance)
			{
				if(children[0]->IntersectChildren(ray, hit)) success = true;
			}
		}
	}
	else if(childsuccess[0]) {
		if(children[0]->IntersectChildren(ray, hit)) success = true;
	}
	else if(childsuccess[1]) {
		if(children[1]->IntersectChildren(ray, hit)) success = true;
	}
		
	return success;


}



void BoxTreeNode::Construct(int count, Triangle **tri)
{
	numChild = 2;
	
	
	float x_min = tri[0]->Vtx[0]->Position[0];
	float x_max = tri[0]->Vtx[0]->Position[0];
	float y_min = tri[0]->Vtx[0]->Position[1];
	float y_max = tri[0]->Vtx[0]->Position[1];
	float z_min = tri[0]->Vtx[0]->Position[2];
	float z_max = tri[0]->Vtx[0]->Position[2];

	//Iteratate through each triangle, and for each triangle
	// iterate through each vertex and check if its a min or max
	for(int i = 0; i < count; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			Vector3 point = tri[i]->Vtx[j]->Position;

			x_min = Min(x_min, point[0]);
			x_max = Max(x_max, point[0]);
			y_min = Min(y_min, point[1]);
			y_max = Max(y_max, point[1]);
			z_min = Min(z_min, point[2]);
			z_max = Max(z_max, point[2]);
		}
	}


	boxMin.Set(x_min, y_min, z_min);
	boxMax.Set(x_max, y_max, z_max);

	if( count <= 10)
	{
		for(int i = 0; i < count; i++)
		{
			TriLeaf[i] = tri[i];
		}

		children[0] = NULL;
		children[1] = NULL;
		numChild = 0;
		return;
	}

	//Compute Largest Splitting Plane (x,y or z)
	float x_dist = abs(x_max - x_min);
	float y_dist = abs(y_max - y_min);
	float z_dist = abs(z_max - z_min);

	float splittingPlane = Max(Max(x_dist, y_dist), z_dist);
	int plane;

	if(splittingPlane == x_dist)
		plane = 0;
	else if(splittingPlane == y_dist)
		plane = 1;
	else
		plane = 2;

	splittingPlane = splittingPlane / (float) 2;

	
	Triangle **tri1 = new Triangle *[count];
	Triangle **tri2 = new Triangle *[count];

	int count1 = 0;
	int count2 = 0;

	Vector3 p1;
	Vector3 p2;
	Vector3 p3;



	for(int i = 0; i < count; i++)
	{
		
		p1 = tri[i]->Vtx[0]->Position;
	    p2 = tri[i]->Vtx[1]->Position;
	    p3 = tri[i]->Vtx[2]->Position;

		Vector3 center = p1 + p2 + p3;
		center.Scale(0.33f);
		
	 
		if(plane == 0)
		{
			if(center[0] < (x_min + splittingPlane))
			{
				tri1[count1] = tri[i];
				count1++;
			}
	    	else
	    	{
	    		tri2[count2] = tri[i];
	    		count2++;
	    	}
		}
	
		else if(plane == 1)
		{
			if(center[1] < (y_min + splittingPlane))
			{
				tri1[count1] = tri[i];
				count1++;
			}

			else
			{
				tri2[count2] = tri[i];
				count2++;
			}
		}

		else if(plane == 2)
		{
			if(center[2] < (z_min + splittingPlane))
			{
				tri1[count1] = tri[i];
				count1++;
			}

			else
			{
				tri2[count2] = tri[i];
				count2++;
			}
		}
		
	}

		if(count1 == 0)
		{
			tri1[count1] = tri2[count2 - 1];
			count1++;
			count2--;
		}
			
		else if (count2 == 0)
		{
			tri2[count2] = tri1[count1 -1];
			count2++;
			count1--;
		}

		children[0] = new BoxTreeNode();
		children[1] = new BoxTreeNode();
		
		children[0]->Construct(count1, tri1);
		children[1]->Construct(count2, tri2);

		delete []tri1;
		delete []tri2;
}