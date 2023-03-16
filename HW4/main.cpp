//
// RAY TRACER ASSIGNMENT MAIN FILE
// AUTHORS: GAVYN EZELL AND CHAEYEON PARK
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
#include "Camera.h"
#include "recursion.h"
using namespace std;

Camera *mainCamera = new Camera();

//we use one vector to hold all primitives
vector<Primitive*> *primitives = new vector<Primitive*>();
vector<Light*> *lights = new vector<Light*>();


glm::vec3 attenuation = glm::vec3(1.0f, 0.0f, 0.0f);
glm::vec3 &attenuationRef = attenuation;

int maxdepth = 5;
int & maxdepthRef = maxdepth;
int imgWidth;
int imgHeight;
int &width = imgWidth;
int &height = imgHeight;

int main(int argc, const char * argv[]) {
    
     

    string filename = argv[1];
    
    readfile(filename, width, height, mainCamera, primitives, lights, attenuationRef, maxdepthRef);

    glm::vec3 finalImage[imgHeight][imgWidth];
    
    float t;
    float &tRef = t;
 
    

    Ray * rayHolder = new Ray();
    Primitive * primHolder;
    Ray * shadowRayHolder = new Ray();
    Ray * mirrorRayHolder = new Ray();
    
    
    for (int i = 0; i < imgHeight; i++) {
        for (int j = 0; j < imgWidth; j++) {
            cout << i << " " << j << "\n";
            
            tRef = 0.0f;
            
            rayHolder->setRay(float(i) + 0.5f, float(j) + 0.5f, float(imgWidth), float(imgHeight), mainCamera);
            
            //loop thru primitives and make calculations
            
            primHolder = NULL;
            for (vector<Primitive*>::iterator it = primitives->begin(); it != primitives->end(); it++) {
                float original = tRef;
                (*it)->calculateIntersection(tRef, rayHolder);
                if (tRef != original) {
                    primHolder = (*it);
                }
                
            }
        
            
            //our primHolder wont be NULL if we found a valid intersection
            if (!primHolder) {
                finalImage[i][j] = glm::vec3(0.0f, 0.0f, 0.0f);
            }
            else {
                // we know our the exact primitive we've hit, use lighting equation
                glm::vec3 finalColor = primHolder->ambient + primHolder->emission;
                //first, grab the exact intersection
                
                glm::vec3 intersectionPoint;
                
                //we want the NORMAL of the primitive we've intersected, for SHADOW BIAS
                glm::vec3 N;
                
                if(Sphere * currSphere = dynamic_cast<Sphere*>(primHolder))
                {
                    //handles Sphere case, which deals with transformations
                    //we wanna transform our intersection point of: transformed ray and UNtransformed sphere
                    //into intersection point of: UNtransformed ray and transformed sphere
                    glm::vec3 originalStart = rayHolder->rayStart;
                    glm::vec3 originalVec = rayHolder->rayVec;
                    
                    glm::vec4 newStart = glm::inverse(primHolder->transformation) * glm::vec4(rayHolder->rayStart, 1.0f);
                    glm::vec4 newVec = glm::inverse(primHolder->transformation) * glm::vec4(rayHolder->rayVec, 0.0f);
                    
                    rayHolder->rayStart = glm::vec3(newStart.x / newStart.w, newStart.y / newStart.w, newStart.z / newStart.w);
                    rayHolder->rayVec = glm::vec3(newVec.x, newVec.y, newVec.z);
                    
                    glm::vec3 transformedIntersectionPoint = rayHolder->rayStart + t * rayHolder->rayVec;
                    N = glm::normalize(transformedIntersectionPoint - currSphere->spherePos);
                    
                    //now WE transform BACK, equations shown below
                    
                    // M * p for point
                    // inverse(transpose(M)) * N for normal
                    N = glm::normalize(glm::inverse(glm::transpose(glm::mat3(currSphere->transformation))) * N);
                    
                    glm::vec4 intersectionPointHom = primHolder->transformation * glm::vec4(transformedIntersectionPoint, 1.0f);
                    
                    intersectionPoint = glm::vec3(intersectionPointHom.x / intersectionPointHom.w, intersectionPointHom.y / intersectionPointHom.w, intersectionPointHom.z / intersectionPointHom.w);
                    
                    rayHolder->rayStart = originalStart;
                    rayHolder->rayVec = originalVec;
                    
                    
                        
                    //now we have correct intersection point and normal associated with it
                  
                }
                else if(Triangle * currTriangle = dynamic_cast<Triangle*>(primHolder))
                {
                    //our triangles are pre transformed, no further calculations
                    intersectionPoint = rayHolder->rayStart + t * rayHolder->rayVec;
                    
                    N = glm::normalize(currTriangle->triangleNorm);
                    
                }
                
                
                for (vector<Light*>::iterator it = lights->begin(); it != lights->end(); it++) {
                    //incorporates SHADOW BIAS  0.001 * Normal Direction
                    
                    shadowRayHolder->setShadowRay(intersectionPoint, (*it), N);

                    float currT = 0.0f;
                    float & currTRef = currT;
                    
                    
                    float lightT = 0.0f;
                    if (!(*it)->isDirectional) {
                        lightT = (((*it)->lightPos).x - shadowRayHolder->rayStart.x) / (shadowRayHolder->rayVec).x;
                    }
                    
                    

                    
                    bool foundIntersection = false;
                    for (vector<Primitive*>::iterator it2 = primitives->begin(); it2 != primitives->end(); it2++) {
                        (*it2)->calculateIntersection(currTRef, shadowRayHolder);
                        
                        if (currTRef == 0.0f) {
                            continue;
                        }
                        if (!(*it)->isDirectional) {
                            //cout << "WHAT";
                            if (currTRef < lightT) {
                                //cout << currTRef << " " << lightT << "\n";
                                foundIntersection = true;
                                break;
                            }
                        }
                        else {
                            if (currTRef != 0.0f) {
                                foundIntersection = true;
                                break;
                            }
                        }
                    }
                    if (foundIntersection) {
                        //cout<< "WHAT\n";
                        continue;
                    }
                    else {
                        //cout << "CALCULATING LIGHTING\n";
                        glm::vec3 currAttenuation;
                        float r = 0.0f;
                        glm::vec3 L;
                        glm::vec3 H;
                        
                        
                        if ((*it)->isDirectional) {
                            currAttenuation = glm::vec3(1.0f, 0.0f, 0.0f);
                            L = glm::normalize((*it)->lightPos);
                        }
                        else {
                            currAttenuation = attenuation;
                            L = glm::normalize((*it)->lightPos - shadowRayHolder->rayStart);
                            r = sqrt( pow((shadowRayHolder->rayStart.x - ((*it)->lightPos).x), 2.0f) + pow((shadowRayHolder->rayStart.y - ((*it)->lightPos).y), 2.0f) + pow((shadowRayHolder->rayStart.z - ((*it)->lightPos).z), 2.0f));
                        }
                        
                        
                        H = glm::normalize(L + glm::normalize(mainCamera->cameraPos - shadowRayHolder->rayStart));
                        /*
                        if (glm::dot(N, H) <= 0.0f) {
                            cout << "AAAGH\n";
                        }
                         */
                        glm::vec3 rest = primHolder->diffuse * glm::max(glm::dot(N, L), 0.0f) + primHolder->specular * pow(glm::max(glm::dot(N, H), 0.0f), float(primHolder->shininess));
                        
                        float denom = currAttenuation.x + currAttenuation.y * r + currAttenuation.z * pow(r, 2.0f);
                        

                        glm::vec3 added = ((*it)->lightColor / denom) * rest;

                        finalColor += added;
                    }
                 
                    
                }
                
                //cout << "FINALCOLOR BEFORE RECURSION: " << finalColor.x << " " << finalColor.y << " " << finalColor.z << "\n";
                //do a final recursive ray trace, only if specular component is nonzero in some color
                if (primHolder->specular.x != 0.0f || primHolder->specular.y != 0.0f  || primHolder->specular.z != 0.0f ) {
                    //calculate mirror ray

                    glm::vec3 mirrorVec = rayHolder->rayVec - 2.0f * glm::dot(rayHolder->rayVec, N) * N;
                    mirrorVec = glm::normalize(mirrorVec);
                    
                    mirrorRayHolder->setMirrorRay(shadowRayHolder->rayStart, mirrorVec);
                    
                    glm::vec3 recursiveColor = primHolder->specular * recursiveTracing(1, mirrorRayHolder, shadowRayHolder, primitives, lights, attenuation, maxdepth);
                    
                    
                    finalColor = finalColor + recursiveColor;
                }
                //cout << "FINALCOLOR AFTER RECURSION: " << finalColor.x << " " << finalColor.y << " " << finalColor.z << "\n\n";
                finalColor = glm::vec3(clamp(finalColor.x,0.0f,1.0f), clamp(finalColor.y,0.0f,1.0f), clamp(finalColor.z, 0.0f,1.0f));
                finalColor = 255.0f * finalColor;
                
                //finalColor = glm::vec3(clamp(finalColor.x,0.0f,255.0f), clamp(finalColor.y,0.0f,255.0f), clamp(finalColor.z, 0.0f,255.0f));
                finalImage[i][j] = finalColor;
                
            }
        }
    }
    
    //DEALLOCATION AND MEMORY CLEANING BEFORE FILE END
    delete mainCamera;

    if (primitives->size() > 0) {
        for (vector<Primitive*>::iterator it = primitives->begin(); it != primitives->end(); it++) {
            delete (*it);
        }
    }
    delete primitives;
    if (lights->size() > 0) {
        for (vector<Light*>::iterator it = lights->begin(); it != lights->end(); it++) {
            delete (*it);
        }
    }
    delete lights;
    
    delete rayHolder;
    delete shadowRayHolder;
    delete mirrorRayHolder;

    //ACTUAL PPM CREATION, using final image values, 2d array
    ofstream MyFile("result.ppm");
    if (MyFile.is_open()) {
        MyFile << "P3 " << imgWidth << " " << imgHeight << " 255\n";
        for (int i = 0; i < imgHeight; i++) {
            for (int j = 0; j < imgWidth; j++) {
                MyFile << int(finalImage[i][j].x) << " " << int(finalImage[i][j].y) << " " << int(finalImage[i][j].z);
                if (j == imgHeight - 1) {
                    MyFile << "\n";
                }
                else {
                    MyFile << " ";
                }
            }
            
        }
        cout << "DONE CREATING FILE\n";
    }
    MyFile.close();
    return 0;
}
