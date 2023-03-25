//
// RAY TRACER ASSIGNMENT MAIN FILE
//

#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <vector>
#include <float.h>
#include <typeinfo>

#include "glm/glm.hpp"
#include "readfile.h"

#include "Camera.h"
#include "Primitive.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Light.h"
#include "recursion.h"

#include "BVHTree.h"
#include "BBoxNode.h"
using namespace std;

Camera *mainCamera = new Camera();

// we use one vector to hold all primitives
vector<Primitive *> *primitives = new vector<Primitive *>();
vector<Light *> *lights = new vector<Light *>();

glm::vec3 attenuation = glm::vec3(1.0f, 0.0f, 0.0f);
glm::vec3 &attenuationRef = attenuation;

int maxdepth = 5;
int &maxdepthRef = maxdepth;
int imgWidth;
int imgHeight;
int &width = imgWidth;
int &height = imgHeight;

int main(int argc, const char *argv[])
{

    std::string output = "result.png";
    std::string &outputRef = output;
    string filename = argv[1];
    readfile(filename, width, height, mainCamera, primitives, lights, attenuationRef, maxdepthRef, outputRef);

    glm::vec3 finalImage[imgHeight][imgWidth];

    // setting up parameters for intersection testing
    // these will simply hold our ray's
    // 1. rayHolder holds rays emitted from our camera
    // 2. shadowRayHolder will simply hold shadow Rays
    // 3. mirroRayHolder will simply hold our reflect/mirror rays as we recursively reflect
    Ray *rayHolder = new Ray();
    Ray *shadowRayHolder = new Ray();
    Ray *mirrorRayHolder = new Ray();

    // simple pointer for grabbing our closest intersected primitive, if there is one that is
    Primitive *primHolder;
    tuple<float, glm::vec3, glm::vec3> intersectionTracker = make_tuple(0.0f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    tuple<float, glm::vec3, glm::vec3> &intersectionTrackerRef = intersectionTracker;

    // constructing our BVH Tree
    BVHTree *sceneTree = new BVHTree(primitives);

    for (int i = 0; i < imgHeight; i++)
    {
        cout << i << "\n";
        for (int j = 0; j < imgWidth; j++)
        {
            // cout << i << " " << j << "\n";

            rayHolder->setPixelRay(float(i) + 0.5f, float(j) + 0.5f, float(imgWidth), float(imgHeight), mainCamera);

            // loop thru primitives and find an intersecting point
            primHolder = NULL;
            Primitive *&primHolderRef = primHolder;

            // tuple to hold intersection information
            // of the form <T VALUE, INTERSECTION_POINT, INTERSECTION_POINT'S NORMAL>
            get<0>(intersectionTrackerRef) = 0.0f;
            get<1>(intersectionTrackerRef) = glm::vec3(0.0f, 0.0f, 0.0f);
            get<2>(intersectionTrackerRef) = glm::vec3(0.0f, 0.0f, 0.0f);

            // use our BVH Tree for faster calculations
            sceneTree->calculateClosestIntersection(intersectionTrackerRef, rayHolder, primHolderRef);

            // did we intersect and store a valid primitive?
            if (!primHolder)
            {
                // cout << "MISS\n";
                finalImage[i][j] = glm::vec3(208.0f, 239.0f, 255.0f);
                // finalImage[i][j] = glm::vec3(0.0f, 0.0f, 0.0f);
            }
            else
            {

                // we know our the exact primitive we've hit, use phong equation, start with ambient and emission terms
                glm::vec3 finalColor = primHolder->ambient + primHolder->emission;

                glm::vec3 intersectionPoint = get<1>(intersectionTrackerRef);
                glm::vec3 N = get<2>(intersectionTrackerRef);

                for (vector<Light *>::iterator it = lights->begin(); it != lights->end(); it++)
                {
                    // incorporates SHADOW BIAS  0.001 * Normal Direction

                    shadowRayHolder->setShadowRay(intersectionPoint, (*it), N);

                    // using BVH Tree for shadow ray calculations
                    bool lightBlocked = sceneTree->testIfBlocked((*it), shadowRayHolder);

                    if (lightBlocked)
                    {
                        // we don't care, go to the next light!
                        continue;
                    }
                    else
                    {

                        glm::vec3 currAttenuation;
                        float r = 0.0f;
                        glm::vec3 L;
                        glm::vec3 H;

                        if ((*it)->isDirectional)
                        {
                            currAttenuation = glm::vec3(1.0f, 0.0f, 0.0f);
                            L = glm::normalize((*it)->lightPos);
                        }
                        else
                        {
                            currAttenuation = attenuation;
                            L = glm::normalize((*it)->lightPos - shadowRayHolder->rayStart);
                            r = sqrt(pow((shadowRayHolder->rayStart.x - ((*it)->lightPos).x), 2.0f) + pow((shadowRayHolder->rayStart.y - ((*it)->lightPos).y), 2.0f) + pow((shadowRayHolder->rayStart.z - ((*it)->lightPos).z), 2.0f));
                        }

                        H = glm::normalize(L + glm::normalize(mainCamera->cameraPos - shadowRayHolder->rayStart));

                        float denom = currAttenuation.x + currAttenuation.y * r + currAttenuation.z * pow(r, 2.0f);
                        glm::vec3 lightColor = ((*it)->lightColor / denom);

                        glm::vec3 rest = lightColor * primHolder->diffuse * glm::max(glm::dot(N, L), 0.0f) + lightColor * primHolder->specular * pow(glm::max(glm::dot(N, H), 0.0f), float(primHolder->shininess));

                        finalColor += rest;
                    }
                }

                if (primHolder->specular.x != 0.0f || primHolder->specular.y != 0.0f || primHolder->specular.z != 0.0f)
                {
                    // calculate mirror ray
                    glm::vec3 mirrorVec = glm::normalize(rayHolder->rayVec) - 2.0f * glm::dot(glm::normalize(rayHolder->rayVec), N) * N;
                    mirrorVec = glm::normalize(mirrorVec);

                    mirrorRayHolder->setMirrorRay(shadowRayHolder->rayStart, mirrorVec);

                    glm::vec3 recursiveColor = primHolder->specular * recursiveTracing(1, sceneTree, mirrorRayHolder, shadowRayHolder, primitives, lights, mainCamera, attenuation, maxdepth);

                    finalColor = finalColor + recursiveColor;
                }

                // clamping and multiplying so our final color is in the proper range
                finalColor = glm::vec3(clamp(finalColor.x, 0.0f, 1.0f), clamp(finalColor.y, 0.0f, 1.0f), clamp(finalColor.z, 0.0f, 1.0f));
                finalColor = 255.0f * finalColor;
                finalImage[i][j] = finalColor;
            }
        }
    }

    // DEALLOCATION AND MEMORY CLEANING BEFORE FILE END
    delete mainCamera;
    delete sceneTree;

    if (primitives->size() > 0)
    {
        for (vector<Primitive *>::iterator it = primitives->begin(); it != primitives->end(); it++)
        {
            delete (*it);
        }
    }
    delete primitives;
    if (lights->size() > 0)
    {
        for (vector<Light *>::iterator it = lights->begin(); it != lights->end(); it++)
        {
            delete (*it);
        }
    }
    delete lights;

    delete rayHolder;
    delete shadowRayHolder;
    delete mirrorRayHolder;

    // ACTUAL PPM CREATION, using final image values, 2d array
    ofstream MyFile(output);
    if (MyFile.is_open())
    {
        MyFile << "P3 " << imgWidth << " " << imgHeight << " 255\n";
        for (int i = 0; i < imgHeight; i++)
        {
            for (int j = 0; j < imgWidth; j++)
            {
                MyFile << int(finalImage[i][j].x) << " " << int(finalImage[i][j].y) << " " << int(finalImage[i][j].z);
                if (j == imgHeight - 1)
                {
                    MyFile << "\n";
                }
                else
                {
                    MyFile << " ";
                }
            }
        }
        cout << "DONE CREATING FILE\n";
    }
    MyFile.close();
    return 0;
}
