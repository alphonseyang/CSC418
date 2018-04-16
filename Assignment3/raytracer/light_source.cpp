/***********************************************************
	
	Starter code for Assignment 3

	Implements light_source.h

***********************************************************/

#include <cmath>
#include "light_source.h"

void PointLight::shade(Ray3D& ray) {
	// TODO: implement this function to fill in values for ray.col 
	// using phong shading.  Make sure your vectors are normalized, and
	// clamp colour values to 1.0.
	//
	// It is assumed at this point that the intersection information in ray 
	// is available.  So be sure that traverseScene() is called on the ray 
	// before this function.

    Point3D lightPos = this->get_position();
    //get intersection point
    Point3D intersectPoint = ray.intersection.point;
    //get normal vector
    Vector3D normalVector = ray.intersection.normal;
    normalVector.normalize();
    //get view vector
    Vector3D viewVector = -ray.dir;
    viewVector.normalize();
    //calculate light vector
    Vector3D light_vector = (lightPos - intersectPoint);
    light_vector.normalize();
    //calculate reflect
    Vector3D reflect = -light_vector + 2*(light_vector.dot(normalVector))*normalVector;
    reflect.normalize();
    //implement phong
    double diffCos = fmax(light_vector.dot(normalVector), 0.0);
    double specCos =  fmax(reflect.dot(viewVector), 0.0);
    //calculate color
    ray.col = ray.intersection.mat->ambient*col_ambient + diffCos*ray.intersection.mat->diffuse*col_diffuse
            +ray.intersection.mat->specular * (pow(specCos, ray.intersection.mat->specular_exp) * col_specular);

    // map texture if the flag is set
    if (ray.intersection.mat->hasTexture) {
        ray.col = ray.intersection.mat->texture * ray.col;
    }

    ray.col.clamp();

}

