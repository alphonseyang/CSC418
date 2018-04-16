/***********************************************************
	
	Starter code for Assignment 3

***********************************************************/

#include <cstdlib>
#include "raytracer.h"

unsigned long int textureWidth;
long int textureHeight;
unsigned char *rarray;
unsigned char *garray;
unsigned char *barray;

int main(int argc, char* argv[])
{
	// Build your scene and setup your camera here, by calling 
	// functions from Raytracer.  The code here sets up an example
	// scene and renders it from two different view points, DO NOT
	// change this if you're just implementing part one of the 
	// assignment.  
	Raytracer raytracer;
	LightList light_list;
	Scene scene;

	// read the texture file and store in the arrays
    bmp_read("texture4.bmp", &textureWidth, &textureHeight, &rarray, &garray, &barray);


	int width = 640;
	int height = 480;

	if (argc == 3) {
		width = atoi(argv[1]);
		height = atoi(argv[2]);
	}
	
	// Define materials for shading.
	Material gold(Color(0.3, 0.3, 0.3), Color(0.75164,0.60648,0.22648),
		Color(0.628281, 0.555802, 0.366065),
		51.2, 0.5);
	Material jade(Color(0, 0, 0), Color(0.54,0.89,0.63),
		Color(0.316228,0.316228,0.316228),
		12.8, 0.3);
    // used for refraction only
	Material glass( Color(0.0, 0.0, 0.0), Color(0.588235, 0.670588, 0.729412),
					Color(0.9, 0.9, 0.9),
					98, 1.3);
    // used for texture mapping only
    Material white(Color(0, 0, 0), Color(1, 1, 1),
                   Color(1,1,1),
                   12.8, 0.3);

	// Defines a point light source.
	PointLight* pLight = new PointLight(Point3D(0,0,5), Color(0.9,0.9,0.9));
	light_list.push_back(pLight);

	// Add a unit square into the scene with material mat.
    // Apply some transformations to the sphere and unit square.
//    SceneNode *sphere = new SceneNode(new UnitSphere(), &gold);
//    scene.push_back(sphere);
//    double factor1[3] = {1.0, 2.0, 1.0};
//    sphere->translate(Vector3D(0, 0, -5));
//    sphere->rotate('x', -45);
//    sphere->rotate('z', 45);
//    sphere->scale(Point3D(0, 0, 0), factor1);

    // for refraction purpose, set material to glass
    // for texture purpose, set material to white
    SceneNode* sphere1 = new SceneNode(new UnitSphere(), &glass);
    scene.push_back(sphere1);
    sphere1->translate(Vector3D(0, 0, -5));
    //sphere1->rotate('x', -45);
    sphere1->rotate('z', 45);

    // for texture purpose, set material to white
    // otherwise, use jade as material
	SceneNode* plane = new SceneNode(new UnitSquare(), &jade);
	scene.push_back(plane);
    double factor2[3] = { 6.0, 6.0, 6.0 };
    plane->translate(Vector3D(0, 0, -8));
    plane->rotate('z', 45);
    plane->scale(Point3D(0, 0, 0), factor2);

//    SceneNode* cylinder = new SceneNode(new UnitCylinder(), &gold);
//    scene.push_back(cylinder);
//    double factor3[3] = { 1.5, 1.5, 1.5 };
//    cylinder->translate(Vector3D(-1, 1, -5));
//    cylinder->rotate('x', -30);
//    cylinder->rotate('y', -30);
//    cylinder->rotate('z', 30);
//    cylinder->scale(Point3D(0, 0, 0), factor3);

    // for texture purpose, set material to white
/*
    SceneNode* cube = new SceneNode(new UnitCube(), &white);
    scene.push_back(cube);
    double factor3[3] = { 1.5, 1.5, 1.5 };
    cube->translate(Vector3D(1.2, 0.7, -5));
    //cube->rotate('x', 60);
    cube->rotate('y', -30);
    cube->scale(Point3D(0, 0, 0), factor3);
*/

	// Render the scene, feel free to make the image smaller for
	// testing purposes.	
	Camera camera1(Point3D(0, 0, 1), Vector3D(0, 0, -1), Vector3D(0, 1, 0), 60.0);
	Image image1(width, height);
	raytracer.render(camera1, scene, light_list, image1); //render 3D scene to image
	image1.flushPixelBuffer("view1.bmp"); //save rendered image to file

	// Render it from a different point of view.
	Camera camera2(Point3D(4, 2, 1), Vector3D(-4, -2, -6), Vector3D(0, 1, 0), 60.0);
	Image image2(width, height);
	raytracer.render(camera2, scene, light_list, image2);
	image2.flushPixelBuffer("view2.bmp");

	// Free memory
	for (size_t i = 0; i < scene.size(); ++i) {
		delete scene[i];
	}

	for (size_t i = 0; i < light_list.size(); ++i) {
		delete light_list[i];
	}

	return 0;
}
