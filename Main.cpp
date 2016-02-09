////////////////////////////////////////
// Main.cpp
////////////////////////////////////////

#include "MeshObject.h"
#include "InstanceObject.h"
#include "Camera.h"
#include "PointLight.h"
#include "DirectLight.h"
#include "Scene.h"
#include "BoxTreeObject.h"
#include "LambertMaterial.h"
#include "AshikhminMaterial.h"
#include "objloader.h"
#include "BitmapLoader.h"
#include "PerlinNoise.h"

#include <stdio.h>
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <iostream>
#include <sstream>
#include <fstream>

void project1();
void project2();
void project3();
void finalProject();
void clouds();
static void PrintInfo(const std::vector<tinyobj::shape_t>& shapes);


////////////////////////////////////////////////////////////////////////////////

int main(int argc,char **argv) {
	project3();
	return 0;
}

////////////////////////////////////////////////////////////////////////////////

void project1() {
	// Create scene
	printf("start");
	Scene scn;
	scn.SetSkyColor(Color(0.8f, 0.9f, 1.0f));

	// Create boxes
	MeshObject box1;
	box1.MakeBox(5.0f,0.1f,5.0f);
	scn.AddObject(box1);

	MeshObject box2;
	box2.MakeBox(1.0f,1.0f,1.0f);

	InstanceObject inst1(box2);
	Matrix34 mtx;
	mtx.MakeRotateX(0.5f);
	mtx.d.y=1.0f;
	inst1.SetMatrix(mtx);
	scn.AddObject(inst1);

	InstanceObject inst2(box2);
	mtx.MakeRotateY(1.0f);
	mtx.d.Set(-1.0f,0.0f,1.0f);
	inst2.SetMatrix(mtx);
	scn.AddObject(inst2);

	// Create lights
	DirectLight sunlgt;
	sunlgt.SetBaseColor(Color(1.0f, 1.0f, 0.9f));
	sunlgt.SetIntensity(0.5f);
	sunlgt.SetDirection(Vector3(-0.5f, -1.0f, -0.5f));
	scn.AddLight(sunlgt);

	PointLight redlgt;
	redlgt.SetBaseColor(Color(1.0f, 0.2f, 0.2f));
	redlgt.SetIntensity(2.0f);
	redlgt.SetPosition(Vector3(2.0f, 2.0f, 0.0f));
	scn.AddLight(redlgt);

	// Create camera
	Camera cam;
	cam.LookAt(Vector3(2.0f,2.0f,5.0f),Vector3(0.0f,0.0f,0.0f));
	cam.SetResolution(800,600);
	cam.SetFOV(40.0f);
	cam.SetAspect(1.33f);

	// Render image
	cam.Render(scn);
	cam.SaveBitmap("Project1.bmp");
	printf("Done");
}

////////////////////////////////////////////////////////////////////////////////



void project2() { 
	printf("Project2");
	// Create scene 
	Scene scn; 
	scn.SetSkyColor(Color(0.8f, 0.8f, 1.0f)); 

	// Create ground 
	MeshObject ground; 
	ground.MakeBox(5.0f,0.1f,5.0f); 
	scn.AddObject(ground); 

	// Create dragon 
	MeshObject dragon; 
	dragon.LoadPLY("dragon2.ply", 0); 
	dragon.Smooth(); 

	BoxTreeObject tree; 
	tree.Construct(dragon); 
	scn.AddObject(tree); 

	// Create instance 
	InstanceObject inst(tree); 
	Matrix34 mtx; 
	mtx.MakeRotateY(PI); 
	mtx.d.Set(-0.05f,0.0f,-0.1f); 
	inst.SetMatrix(mtx); 
	scn.AddObject(inst);

	// Create lights 
	DirectLight sunlgt; 
	sunlgt.SetBaseColor(Color(1.0f, 1.0f, 0.9f)); 
	sunlgt.SetIntensity(1.0f); 
	sunlgt.SetDirection(Vector3(2.0f, -3.0f, -2.0f)); 
	scn.AddLight(sunlgt); 

	PointLight redlgt; 
	redlgt.SetBaseColor(Color(1.0f, 0.2f, 0.2f)); 
	redlgt.SetIntensity(0.02f); 
	redlgt.SetPosition(Vector3(-0.2f, 0.2f, 0.2f)); 
	scn.AddLight(redlgt); 

	PointLight bluelgt; 
	bluelgt.SetBaseColor(Color(0.2f, 0.2f, 1.0f)); 
	bluelgt.SetIntensity(0.02f); 
	bluelgt.SetPosition(Vector3(0.1f, 0.1f, 0.3f)); 
	scn.AddLight(bluelgt); 

	// Create camera 
	Camera cam; 
	cam.LookAt(Vector3(-0.1f,0.1f,0.2f),Vector3(-0.05f,0.12f,0.0f)); 
	cam.SetFOV(40.0f); 
	cam.SetAspect(1.33f); 
	cam.SetResolution(800,600); 


	// Render image 
	cam.Render(scn); 
	cam.SaveBitmap("mydragon2.bmp"); 
	

} 


void project3() { 
	// Create scene 
	Scene scn; 
	scn.SetSkyColor(Color(0.8f, 0.9f, 1.0f)); 

	Camera cam;

	// Materials 
	const int nummtls=4; 
	AshikhminMaterial mtl[nummtls]; 

	// Diffuse 
	mtl[0].SetSpecularLevel(0.0f); 
	mtl[0].SetDiffuseLevel(1.0f); 
	mtl[0].SetDiffuseColor(Color(0.7f,0.7f,0.7f)); 

	// Roughened copper 
	mtl[1].SetDiffuseLevel(0.0f); 
	mtl[1].SetSpecularLevel(1.0f); 
	mtl[1].SetSpecularColor(Color(0.9f,0.6f,0.5f)); 
	mtl[1].SetRoughness(100.0f,100.0f); 

	// Anisotropic gold 
	mtl[2].SetDiffuseLevel(0.0f); 
	mtl[2].SetSpecularLevel(1.0f); 
	mtl[2].SetSpecularColor(Color(0.95f,0.7f,0.3f)); 
	mtl[2].SetRoughness(1.0f,1000.0f); 

	// Red plastic 
	mtl[3].SetDiffuseColor(Color(1.0f,1.0f,1.0f)); 
	mtl[3].SetDiffuseLevel(0.8f); 
	mtl[3].SetSpecularLevel(0.2f); 
	mtl[3].SetSpecularColor(Color(1.0f,1.0f,1.0f)); 
	mtl[3].SetRoughness(1000.0f,1000.0f); 



	std::vector<tinyobj::shape_t> shapes;
	std::string err = tinyobj::LoadObj(shapes, "dragon_small.obj", NULL);

	MeshObject Dragon;
	Dragon.setData(shapes, 0);//, "greenmesh.bmp");

	BoxTreeObject tree; 
	tree.Construct(Dragon); 

	Matrix34 mtx; 
	InstanceObject *inst=new InstanceObject(tree); 
	mtx.d.Set(0.0f,-2.0f,0); 
	inst->SetMatrix(mtx); 
	inst->SetMaterial(&mtl[3]); 
	inst->SetTexture(0);
	scn.AddObject(*inst); 

	Matrix34 mtx2; 
	InstanceObject *inst2=new InstanceObject(tree); 
	mtx2.d.Set(3.0f,-3.5f, -3.5f); 
	inst2->SetMatrix(mtx2); 
	inst2->SetMaterial(&mtl[3]); 
	inst2->SetTexture(1);
	scn.AddObject(*inst2); 


	
	// Create ground 
	LambertMaterial lambert; 
	lambert.SetDiffuseColor(Color(0.3f,0.3f,0.35f)); 

	MeshObject ground;
	ground.MakeBox(2.0f,0.11f,2.0f,&lambert); 
	//scn.AddObject(ground); 

	// Create lights 
	DirectLight sunlgt; 
	sunlgt.SetBaseColor(Color(1.0f, 1.0f, 0.9f)); 
	sunlgt.SetIntensity(1.0f); 
	sunlgt.SetDirection(Vector3(2.0f, -3.0f, -2.0f)); 
	scn.AddLight(sunlgt); 

	// Create camera
    cam.LookAt(Vector3(-6.5f,2.25f,0.2f),Vector3(0.0f,0.15f,-0.15f));
	cam.SetFOV(40.0f); 
	cam.SetAspect(1.33f); 
	cam.SetResolution(800,600); 
	cam.SetSuperSample(1); 

	printf("here\n");
	//PrintInfo(shapes);

	// Render image 
	cam.Render(scn); 
	cam.SaveBitmap("drag8.bmp");
}

void finalProject()
{
	std::vector<tinyobj::shape_t> shapes;
	std::string err = tinyobj::LoadObj(shapes, "box.obj", NULL);

	MeshObject Dragon;
}


static void PrintInfo(const std::vector<tinyobj::shape_t>& shapes)
{
	std::cout << "# of shapes : " << shapes.size() << std::endl;

	
	for (size_t i = 0; i < shapes.size(); i++) {
		printf("shape[%ld].name = %s\n", i, shapes[i].name.c_str());
		printf("shape[%ld].indices: %ld\n", i, shapes[i].mesh.indices.size());
		assert((shapes[i].mesh.indices.size() % 3) == 0);
		for (size_t f = 0; f < shapes[i].mesh.indices.size(); f++) {
			printf("  idx[%ld] = %d\n", f, shapes[i].mesh.indices[f]);
		}

		printf("shape[%ld].vertices: %ld\n", i, shapes[i].mesh.positions.size());
		assert((shapes[i].mesh.positions.size() % 3) == 0);
		for (size_t v = 0; v < shapes[i].mesh.positions.size() / 3; v++) {
			printf("  v[%ld] = (%f, %f, %f)\n", v,
				shapes[i].mesh.positions[3 * v + 0],
				shapes[i].mesh.positions[3 * v + 1],
				shapes[i].mesh.positions[3 * v + 2]);
		}

		printf("shape[%ld].material.name = %s\n", i, shapes[i].material.name.c_str());
		printf("  material.Ka = (%f, %f ,%f)\n", shapes[i].material.ambient[0], shapes[i].material.ambient[1], shapes[i].material.ambient[2]);
		printf("  material.Kd = (%f, %f ,%f)\n", shapes[i].material.diffuse[0], shapes[i].material.diffuse[1], shapes[i].material.diffuse[2]);
		printf("  material.Ks = (%f, %f ,%f)\n", shapes[i].material.specular[0], shapes[i].material.specular[1], shapes[i].material.specular[2]);
		printf("  material.Tr = (%f, %f ,%f)\n", shapes[i].material.transmittance[0], shapes[i].material.transmittance[1], shapes[i].material.transmittance[2]);
		printf("  material.Ke = (%f, %f ,%f)\n", shapes[i].material.emission[0], shapes[i].material.emission[1], shapes[i].material.emission[2]);
		printf("  material.Ns = %f\n", shapes[i].material.shininess);
		printf("  material.Ni = %f\n", shapes[i].material.ior);
		printf("  material.dissolve = %f\n", shapes[i].material.dissolve);
		printf("  material.illum = %d\n", shapes[i].material.illum);
		printf("  material.map_Ka = %s\n", shapes[i].material.ambient_texname.c_str());
		printf("  material.map_Kd = %s\n", shapes[i].material.diffuse_texname.c_str());
		printf("  material.map_Ks = %s\n", shapes[i].material.specular_texname.c_str());
		printf("  material.map_Ns = %s\n", shapes[i].material.normal_texname.c_str());
		std::map<std::string, std::string>::const_iterator it(shapes[i].material.unknown_parameter.begin());
		std::map<std::string, std::string>::const_iterator itEnd(shapes[i].material.unknown_parameter.end());
		for (; it != itEnd; it++) {
			printf("  material.%s = %s\n", it->first.c_str(), it->second.c_str());
		}
		printf("\n");
	}
	
}



/////////////////////////////////////////////////////////////////////////

bool loadTexture(const char* filename)
{
	CBitmap texture;
	printf("loading texture\n");
	texture.Load(filename);

	int numpixels = texture.GetHeight() * texture.GetWidth();


	RGBA *pixels;
	pixels = (RGBA*)texture.GetBits();

	printf("width %d: numpixels: %d\n", texture.GetWidth(),numpixels);
	
	Color * color_data;
	color_data = new Color[numpixels];
	

	for(int i = 0; i < numpixels; i++)
	{
		float r = pixels[i].Red / (float)255;
		float g = pixels[i].Green / (float)255;
		float b = pixels[i].Blue / (float)255;		
		color_data[i].Set(r,g,b);
	}

	return true;
}

/////////////////////////////////////////////////////////////////////////
void clouds()
{
	PerlinNoise noise;
	noise.GenerateNoise();
	noise.SaveNoise();
	

}