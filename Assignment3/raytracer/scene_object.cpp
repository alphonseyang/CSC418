/***********************************************************
	
	Starter code for Assignment 3
	
	Implements scene_object.h

***********************************************************/
#include <iostream>
#include <cmath>
#include "scene_object.h"
#include "texture.h"

void updatePlaneTextureMapping(Point3D intersectionPoint, Material* mat){
    // set the flag
    mat->hasTexture = true;
    // map the color to the pixel
    unsigned int x = ((unsigned int)(std::abs(intersectionPoint[0]+0.5) * textureWidth));
    unsigned int y = ((unsigned int)(std::abs(intersectionPoint[1]+0.5) * textureHeight));
    mat->texture = Color(rarray[int(y * textureWidth + x)]/255.0,garray[int(y * textureWidth + x)]/255.0,barray[int(y * textureWidth + x)]/255.0);
}

void updateSphereTextureMapping(Point3D intersection, Vector3D normal, Material* mat){
    mat->hasTexture = true;

//    // old texture mapping
//    normal.normalize();
//    double U=0,V=0,PI = 22/7, phi = 0, theta = 0;
//    Vector3D pole(0, 1, 0);
//    Vector3D equator(1, 0, 0);
//
//    phi = acos(pole.dot(normal));
//    V=phi/PI;
//    theta = (acos( normal.dot(equator))/ sin( phi )) / ( 2 * PI);
//
//    if ( normal.dot(pole.cross(equator)) > 0 )
//        U = theta;
//    else
//        U = 1 - theta;

    // new sphere texture mapping
    double pi = 3.1415926;
    double U = 0.5 + atan2(intersection[2], intersection[0]) / (2*pi);
    double V = 0.5 - asin(intersection[1]) / (2*pi);

    int y = (int)std::abs(U*textureHeight);
    int x = (int)std::abs(V*textureWidth);
    y = y % (int)textureHeight;
    x = x % (int)textureWidth;
    mat->texture = Color(rarray[int(y * textureWidth + x)]/255.0,garray[int(y * textureWidth + x)]/255.0,barray[int(y * textureWidth + x)]/255.0);
}

void updateCubeTextureMapping(Point3D intersection, Material* mat){
    mat->hasTexture = true;
    double u, v;
    if (std::abs(intersection[0]) == 1){
        u = (intersection[2]+1)/2;
        v = (intersection[1]+1)/2;
    }
    else if (std::abs(intersection[1]) == 1){
        u = (intersection[0]+1)/2;
        v = (intersection[2]+1)/2;
    }
    else {
        u = (intersection[0]+1)/2;
        v = (intersection[1]+1)/2;
    }

    int y = (int)std::abs(u*textureHeight);
    int x = (int)std::abs(v*textureWidth);
    y = y % (int)textureHeight;
    x = x % (int)textureWidth;
    mat->texture = Color(rarray[int(y * textureWidth + x)]/255.0,garray[int(y * textureWidth + x)]/255.0,barray[int(y * textureWidth + x)]/255.0);
}

bool UnitSquare::intersect(Ray3D& ray, const Matrix4x4& worldToModel,
		const Matrix4x4& modelToWorld, Material* mat) {
	// TODO: implement intersection code for UnitSquare, which is
	// defined on the xy-plane, with vertices (0.5, 0.5, 0),
	// (-0.5, 0.5, 0), (-0.5, -0.5, 0), (0.5, -0.5, 0), and normal
	// (0, 0, 1).
	//
	// Your goal here is to fill ray.intersection with correct values
	// should an intersection occur.  This includes intersection.point,
	// intersection.normal, intersection.none, intersection.t_value.
	//
	// HINT: Remember to first transform the ray into object space
	// to simplify the intersection test.

    //transform ray into object space
    Point3D rayOrigin = worldToModel * (ray.origin);
    Vector3D raydirection = worldToModel * (ray.dir);

    //init vertices of the unit squre
    Point3D sqVertice[4];
    sqVertice[0] = Point3D(0.5, 0.5, 0);
    sqVertice[1] = Point3D(-0.5, 0.5, 0);
    sqVertice[2] = Point3D(-0.5, -0.5, 0);
    sqVertice[3] = Point3D(0.5, -0.5, 0);

    //get normal
    Vector3D vec1 = sqVertice[1] - sqVertice[0];
    Vector3D vec2 = sqVertice[2] - sqVertice[0];
    Vector3D n = vec1.cross(vec2);
    //square plain normal
    n.normalize();

    //check if dir dot n = 0
    if (raydirection.dot(n) == 0){
        //ray.intersection.none = true;
        return false;
    }

    //compute t
    double t = (sqVertice[0] - rayOrigin).dot(n) / raydirection.dot(n);
    t = std::abs(t);
    //check if point lies inside unit square
    Point3D intersectionPoint = rayOrigin + t * raydirection;
    if (intersectionPoint[0] < -0.5 || intersectionPoint[0] > 0.5 || intersectionPoint[1] < -0.5 || intersectionPoint[1] > 0.5 || (intersectionPoint[2]<-0.00001 || intersectionPoint[2] > 0.00001)){
        //outside unit square
        //ray.intersection.none = true;
        return false;
    }

    //valid intersection
    Point3D worldIntersectionPoint = modelToWorld * intersectionPoint;
    Vector3D worldNormal = worldToModel.transpose() * n;
    if (ray.intersection.none == true || ray.intersection.t_value >= t) {
        //found a closer intersection, replace
        ray.intersection.none = false;
        ray.intersection.normal = worldNormal;
        ray.intersection.point = worldIntersectionPoint;
        ray.intersection.t_value = t;
		// uncomment this to enable texture mapping for plane
        //updatePlaneTextureMapping(intersectionPoint, mat);
        return true;
    }
    else {
        //do not replace
        return false;
    }

}

bool UnitSphere::intersect(Ray3D& ray, const Matrix4x4& worldToModel,
		const Matrix4x4& modelToWorld, Material* mat) {
	// TODO: implement intersection code for UnitSphere, which is centred
	// on the origin.
	//
	// Your goal here is to fill ray.intersection with correct values
	// should an intersection occur.  This includes intersection.point,
	// intersection.normal, intersection.none, intersection.t_value.
	//
	// HINT: Remember to first transform the ray into object space
	// to simplify the intersection test.
    Point3D rayOrigin = worldToModel * (ray.origin);
    Vector3D raydirection = worldToModel * (ray.dir);
    raydirection.normalize();
    Point3D sphere_center = Point3D(0,0,0);


    double t0, t1; // solutions for t if the ray intersects
    Vector3D L = sphere_center - rayOrigin;
    double tca = L.dot(raydirection);
    //if sphere lines on the other side of the origin
    if (tca < 0){
        return false;
    }
    double d2 = L.dot(L) - tca * tca;
    //if d bigger than radius, ray does not intersect sphere
    if (d2 > 1){
        return false;
    }
    double thc = sqrt(1 - d2);
    t0 = tca - thc;
    t1 = tca + thc;

    //find the smaller value of t0 and t1, which represents the closest intersection point
    if (t0 > t1) std::swap(t0, t1);

    if (t0 < 0) {
        t0 = t1; // if t0 is negative, let's use t1 instead
        if (t0 < 0) {
            return false; // both t0 and t1 are negative
        }
    }

    double previous_t_value = ray.intersection.t_value;
    //check if current object is in front of previous stored object
    if (ray.intersection.none == true || previous_t_value > t0){
        ray.intersection.t_value = t0;
        ray.intersection.none = false;
        Point3D intersectionPoint = Point3D(rayOrigin + t0*raydirection);
        Vector3D n = intersectionPoint - sphere_center;
        n.normalize();
        ray.intersection.normal = worldToModel.transpose() * n;
        ray.intersection.point = modelToWorld * intersectionPoint;
		// uncomment this to enable texture mapping for sphere
        //updateSphereTextureMapping(intersectionPoint, worldToModel.transpose() * n, mat);
        return true;
    }
    return false;
}

bool UnitCylinder::intersect(Ray3D &ray, const Matrix4x4 &worldToModel,
                             const Matrix4x4 &modelToWorld, Material* mat) {
    Point3D origin = worldToModel * ray.origin;
    Vector3D direction = worldToModel * ray.dir;
    double top = 0.5;
    double bottom = -1.5;
    double radius = 0.5;

    //intersect with the top and bottom bases
    double t;
    double intersectBot = (bottom - origin[2]) / direction[2];
    double intersectTop = (top - origin[2]) / direction[2];
    t = intersectBot < intersectTop ? intersectBot : intersectTop;
    Vector3D normal0;
    if (intersectBot < intersectTop)
        normal0 = Vector3D(0, 0, -1);
    else
        normal0 = Vector3D(0, 0, 1);
    normal0.normalize();
    Point3D intersectPoint = origin + t * direction;
    if (intersectPoint[0] * intersectPoint[0] + intersectPoint[1] * intersectPoint[1] <= radius &&
        (ray.intersection.none || ray.intersection.t_value > t)) {
        ray.intersection.t_value = t;
        ray.intersection.point = intersectPoint;
        ray.intersection.none = false;
        ray.intersection.normal = normal0;
        return true;
    }

    // intersect with the body
    double a = direction[0] * direction[0] + direction[1] * direction[1];
    double b = 2 * origin[0] * direction[0] + 2 * origin[1] * direction[1];
    double c = origin[0] * origin[0] + origin[1] * origin[1] - radius;
    double delta = b * b - 4 * a * c;

    if (delta < 0)
        return false;
    double result1 = (-b + sqrt(delta)) / (2 * a);
    double result2 = (-b - sqrt(delta)) / (2 * a);
    if (result1 > 0 && result2 < 0)
        t = result1;
    else
        t = result2;

    intersectPoint = origin + t * direction;
    Vector3D normal1(intersectPoint[0], intersectPoint[1], 0);
    normal1.normalize();

    if (intersectPoint[2] < top && intersectPoint[2] > bottom &&
        (ray.intersection.none || ray.intersection.t_value > t)) {
        ray.intersection.t_value = t;
        ray.intersection.point = modelToWorld * intersectPoint;
        ray.intersection.none = false;
        ray.intersection.normal = modelToWorld * normal1;
        return true;
    }

    return false;
}

bool UnitCube::intersect(Ray3D& ray, const Matrix4x4& worldToModel,
                           const Matrix4x4& modelToWorld, Material* mat) {
    Vector3D vmin = Vector3D(0, 0, 0);
    Vector3D vmax = Vector3D(1, 1, 1);
    Vector3D bounds[2] = {vmin, vmax};
    Point3D origin = worldToModel * ray.origin;
    Vector3D direction = (worldToModel * ray.dir);
    direction.normalize();

    Vector3D invdir = Vector3D(1/direction[0], 1/direction[1], 1/direction[2]);
    int sign[3];
    sign[0] = (invdir[0] < 0);
    sign[1] = (invdir[1] < 0);
    sign[2] = (invdir[2] < 0);

    double tmin, tmax, txmin, txmax, tymin, tymax, tzmin, tzmax;

    // calculate the t values with 6 planes
    txmin = (bounds[sign[0]][0] - origin[0]) * invdir[0];
    txmax = (bounds[1-sign[0]][0] - origin[0]) * invdir[0];
    tymin = (bounds[sign[1]][1] - origin[1]) * invdir[1];
    tymax = (bounds[1-sign[1]][1] - origin[1]) * invdir[1];
    tzmin = (bounds[sign[2]][2] - origin[2]) * invdir[2];
    tzmax = (bounds[1-sign[2]][2] - origin[2]) * invdir[2];

    // update the tmin and tmax
    tmin = txmin;
    tmax = txmax;
    if ((tmin > tymax) || (tymin > tmax))
        return false;
    if (tymin > tmin)
        tmin = tymin;
    if (tymax < tmax)
        tmax = tymax;
    if ((tmin > tzmax) || (tzmin > tmax))
        return false;
    if (tzmin > tmin)
        tmin = tzmin;
    if (tzmax < tmax)
        tmax = tzmax;

    // select the final t value
    double t;
    if (tmax > 0 && tmin < 0)
        t = tmax;
    else
        t = tmin;

    // set the corresponding normal vector based on the plane
    Vector3D normal;
    if (t == txmax)
        normal = Vector3D(-1.0, 0.0, 0.0);
    else if (t == txmin)
        normal = Vector3D(1.0, 0.0, 0.0);
    else if (t == tymax)
        normal = Vector3D(0.0, -1.0, 0.0);
    else if (t == tymin)
        normal = Vector3D(0.0, 1.0, 0.0);
    else if (t == tzmax)
        normal = Vector3D(0.0, 0.0, -1.0);
    else
        normal = Vector3D(0.0, 0.0, 1.0);
    Point3D intersectPoint = origin + t * direction;
    normal.normalize();
    if (ray.intersection.none || ray.intersection.t_value > t) {
        ray.intersection.t_value = tmin;
        ray.intersection.point = modelToWorld * intersectPoint;
        ray.intersection.none = false;
        ray.intersection.normal = modelToWorld * normal;
		// uncomment this to enable texture mapping for cube
        //updateCubeTextureMapping(intersectPoint, mat);
        return true;
    }
    return false;
}

void SceneNode::rotate(char axis, double angle) {
	Matrix4x4 rotation;
	double toRadian = 2*M_PI/360.0;
	int i;

	for (i = 0; i < 2; i++) {
		switch(axis) {
			case 'x':
				rotation[0][0] = 1;
				rotation[1][1] = cos(angle*toRadian);
				rotation[1][2] = -sin(angle*toRadian);
				rotation[2][1] = sin(angle*toRadian);
				rotation[2][2] = cos(angle*toRadian);
				rotation[3][3] = 1;
			break;
			case 'y':
				rotation[0][0] = cos(angle*toRadian);
				rotation[0][2] = sin(angle*toRadian);
				rotation[1][1] = 1;
				rotation[2][0] = -sin(angle*toRadian);
				rotation[2][2] = cos(angle*toRadian);
				rotation[3][3] = 1;
			break;
			case 'z':
				rotation[0][0] = cos(angle*toRadian);
				rotation[0][1] = -sin(angle*toRadian);
				rotation[1][0] = sin(angle*toRadian);
				rotation[1][1] = cos(angle*toRadian);
				rotation[2][2] = 1;
				rotation[3][3] = 1;
			break;
		}
		if (i == 0) {
			this->trans = this->trans*rotation;
			angle = -angle;
		}
		else {
			this->invtrans = rotation*this->invtrans;
		}
	}
}

void SceneNode::translate(Vector3D trans) {
	Matrix4x4 translation;

	translation[0][3] = trans[0];
	translation[1][3] = trans[1];
	translation[2][3] = trans[2];
	this->trans = this->trans*translation;
	translation[0][3] = -trans[0];
	translation[1][3] = -trans[1];
	translation[2][3] = -trans[2];
	this->invtrans = translation*this->invtrans;
}

void SceneNode::scale(Point3D origin, double factor[3] ) {
	Matrix4x4 scale;

	scale[0][0] = factor[0];
	scale[0][3] = origin[0] - factor[0] * origin[0];
	scale[1][1] = factor[1];
	scale[1][3] = origin[1] - factor[1] * origin[1];
	scale[2][2] = factor[2];
	scale[2][3] = origin[2] - factor[2] * origin[2];
	this->trans = this->trans*scale;
	scale[0][0] = 1/factor[0];
	scale[0][3] = origin[0] - 1/factor[0] * origin[0];
	scale[1][1] = 1/factor[1];
	scale[1][3] = origin[1] - 1/factor[1] * origin[1];
	scale[2][2] = 1/factor[2];
	scale[2][3] = origin[2] - 1/factor[2] * origin[2];
	this->invtrans = scale*this->invtrans;
}


