/***********************************************************
	
	Starter code for Assignment 3

	Implementations of functions in raytracer.h, 
	and the main function which specifies the scene to be rendered.	

***********************************************************/


#include "raytracer.h"
#include <cmath>
#include <iostream>
#include <cstdlib>

void Raytracer::traverseScene(Scene &scene, Ray3D &ray) {
    for (size_t i = 0; i < scene.size(); ++i) {
        SceneNode *node = scene[i];

        if (node->obj->intersect(ray, node->worldToModel, node->modelToWorld, node->mat)) {
            ray.intersection.mat = node->mat;
        }
    }
}

void Raytracer::computeTransforms(Scene &scene) {
    // right now this method might seem redundant. But if you decide to implement
    // scene graph this is where you would propagate transformations to child nodes
    for (size_t i = 0; i < scene.size(); ++i) {
        SceneNode *node = scene[i];

        node->modelToWorld = node->trans;
        node->worldToModel = node->invtrans;
    }
}

void Raytracer::computeShading(Ray3D &ray, LightList &light_list, Scene &scene) {
    for (size_t i = 0; i < light_list.size(); ++i) {
        LightSource *light = light_list[i];

        // Each lightSource provides its own shading function.
        // Implement shadows here if needed.

        /** switch for shadow, 'true' to turn on.*/
        bool shadowOn = false;


        if (shadowOn) {

            //create shadow ray
            Ray3D shadow;
            shadow.origin = ray.intersection.point - 0.01 * ray.dir;
            shadow.dir = light->get_position() - shadow.origin;
            shadow.dir.normalize();

            traverseScene(scene, shadow);

            //no shadow if light didn't hit object
            if (shadow.intersection.none) {
                light->shade(ray);
            } else {
                //Color the shadow
                ray.col = ray.col + 0.9 * ray.intersection.mat->ambient;
                ray.col.clamp();
            }
        } else {
            light->shade(ray);
        }
    }
}

Color Raytracer::shadeRay(Ray3D &ray, Scene &scene, LightList &light_list, int depth) {
    Color col(0.0, 0.0, 0.0);
    traverseScene(scene, ray);

    // Don't bother shading if the ray didn't hit
    // anything.
    if (!ray.intersection.none) {
        computeShading(ray, light_list, scene);
        col = ray.col;

        if (depth <= 2) {
            /** Preparation **/
            Vector3D normalVec = ray.intersection.normal;
            normalVec.normalize();
            Vector3D rayDirection = ray.dir;
            rayDirection.normalize();


            /** Recursively Ray Tracing for Reflection Calculation **/
            Color reflectionColor(0.0, 0.0, 0.0);
            Vector3D reflectVec = rayDirection - 2 * rayDirection.dot(normalVec) * normalVec;
            reflectVec.normalize();
            Point3D reflectOrigin = ray.intersection.point + 0.001 * normalVec;
            Ray3D reflectionRay = Ray3D(reflectOrigin, reflectVec);
            reflectionColor = shadeRay(reflectionRay, scene, light_list, depth + 1);
            // for reflection only
            // uncomment to enable reflection feature
            col = col + 0.1 * reflectionColor;


            /** Refraction Calculation **/
            normalVec = ray.intersection.normal;
            normalVec.normalize();
            rayDirection = -ray.dir;
            rayDirection.normalize();
            // refraction coefficient
            double n = 1.0;
            double nt = 1.0;
            if (rayDirection.dot(normalVec) > 0) {
                // enter  object
                nt = ray.intersection.mat->refractionCoeff;
            } else {
                // leave object
                n = ray.intersection.mat->refractionCoeff;
                normalVec = -normalVec;
            }
            // compute refraction vector and ray
            double refractionRatio = n / nt;
            double cosI = normalVec.dot(rayDirection);
            double cosT2 = 1.0 - (refractionRatio * refractionRatio * (1.0 - cosI * cosI));
            if (cosT2 >= 0) {
                double cosT = sqrt(cosT2);
                Vector3D refractionVec =
                        (refractionRatio * rayDirection) - refractionRatio * cosI * normalVec - cosT * normalVec;
                refractionVec.normalize();

                Point3D refractionOrigin = ray.intersection.point + 0.001 * refractionVec;
                Ray3D refractionRay(refractionOrigin, refractionVec);

                //fresnel formula
                double Rs = (nt * cosI - n * cosT) / (nt * cosI + n * cosT);
                double Rp = (n * cosI - nt * cosT) / (n * cosI + nt * cosT);
                double kr = (Rs * Rs + Rp * Rp) / 2.0;
                // uncomment to enable refraction feature
                col = col + (1.0 - kr) * shadeRay(refractionRay, scene, light_list, depth + 1);
            }
        }
    }
    col.clamp();
    return col;
}

void Raytracer::render(Camera &camera, Scene &scene, LightList &light_list, Image &image) {
    computeTransforms(scene);

    Matrix4x4 viewToWorld;
    double factor = (double(image.height) / 2) / tan(camera.fov * M_PI / 360.0);

    viewToWorld = camera.initInvViewMatrix();

    // Construct a ray for each pixel.
    for (int i = 0; i < image.height; i++) {
        for (int j = 0; j < image.width; j++) {
            // Sets up ray origin and direction in view space,
            // image plane is at z = -1.
            Point3D origin(0, 0, 0);
            Point3D imagePlane;

            bool antiAliasing = 1; // 1 is On, 0 is Off
            double FOCALRANGE = 0.0; //0.0 is off, 4.5 is on
            Color col;


            if (!antiAliasing) {
                /**  No Anti Aliasing and DOF start  **/
                //Old code for part 1
                //Cast a ray to the center of each pixel
                imagePlane[0] = (-double(image.width) / 2 + 0.5 + j) / factor;
                imagePlane[1] = (-double(image.height) / 2 + 0.5 + i) / factor;
                imagePlane[2] = -1;
                //Ray Casting
                Ray3D ray;
                Vector3D RayDirection = Vector3D(imagePlane[0], imagePlane[1], imagePlane[2]);
                ray = Ray3D(viewToWorld * origin, viewToWorld * RayDirection);
                col = shadeRay(ray, scene, light_list, 0);
                image.setColorAtPixel(i, j, col);

                /**  No Anti Aliasing and DOF start  **/
            } else {

                /** Anti Aliasing and DOF start  **/
                /**       sampling 16 rays each pixel      **/

                for (int x = 0; x < 4; x++) {
                    for (int y = 0; y < 4; y++) {
                        imagePlane[0] = (-double(image.width) / 2 + 0.5 + x * 0.25 + j) / factor;
                        imagePlane[1] = (-double(image.height) / 2 + 0.5 + x * 0.25 + i) / factor;
                        imagePlane[2] = -1;

                        if (FOCALRANGE == 0.0) {
                            //no DOF
                            Ray3D ray;
                            Vector3D RayDirection = Vector3D(imagePlane[0], imagePlane[1], imagePlane[2]);
                            ray = Ray3D(viewToWorld * origin, viewToWorld * RayDirection);
                            col = col + shadeRay(ray, scene, light_list, 0);

                        } else {
                            /**----------------------------------*/
                            /**          Depth of Field          */
                            Vector3D dir = imagePlane - origin;
                            dir.normalize();
                            double fo = -(FOCALRANGE) / dir[2];

                            //calculate focal point
                            Point3D focalPoint = origin + fo * dir;
                            double phi = 2 * M_PI * (rand() * 1.0f / RAND_MAX);

                            //change coefficient of r to change blurriness
                            double r = 0.27 * sqrt(rand() * 1.0f / RAND_MAX) / 2.0;
                            Point3D newOrigin(r * cos(phi), r * sin(phi), 0);
                            Vector3D RayDirection = focalPoint - newOrigin;

                            Ray3D ray;
                            ray = Ray3D(viewToWorld * newOrigin, viewToWorld * RayDirection);
                            ray.dir.normalize();
                            col = col + shadeRay(ray, scene, light_list, 0);
                        }
                    }
                }

                //get the average value for each pixel
                col[0] = col[0] / 16;
                col[1] = col[1] / 16;
                col[2] = col[2] / 16;
                image.setColorAtPixel(i, j, col);
            }

            /** Anti Aliasing and DOF end  **/
        }


    }
}


