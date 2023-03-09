//
// RAY TRACER ASSIGNMENT MAIN FILE
// AUTHORS: GAVYN EZELL AND CHAEYEON PARK
//

#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <vector>
#include "readfile.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Camera.h"
#include "glm/glm.hpp"
using namespace std;

Camera *mainCamera = new Camera();
//we use two vectors 
vector<Sphere*> *spheres = new vector<Sphere*>();
vector<Triangle*> *triangles = new vector<Triangle*>();
//star
int imgWidth;
int imgHeight;
int &width = imgWidth;
int &height = imgHeight;

int main(int argc, const char * argv[]) {

    string filename = argv[1];
    readfile(filename, width, height, mainCamera, spheres, triangles);
    
    //we make Ray
    //pick a primitive
    //calculate intersection
    glm::vec3 finalImage[imgHeight][imgWidth];
    
    float t;
    float &tRef = t;
    Ray *rayHolder = new Ray();
    
    glm::vec3 specificAmbient;
    glm::vec3 &specificAmbientRef = specificAmbient;
    
    for (int i = 0; i < imgHeight; i++) {
        for (int j = 0; j < imgWidth; j++) {
            tRef = 0.0f;
            
            rayHolder->setRay(float(i) + 0.5f, float(j) + 0.5f, float(imgWidth), float(imgHeight), mainCamera);
            
      
            //checking all Sphere primitives
            for (vector<Sphere*>::iterator it = spheres->begin(); it != spheres->end(); it++) {
                (*it)->sphereIntersection(tRef, rayHolder, specificAmbientRef);
                
            }
            
            //checking all Triangle primitives
            for (vector<Triangle*>::iterator it = triangles->begin(); it != triangles->end(); it++) {
                (*it)->triangleIntersection(tRef, rayHolder, specificAmbientRef);
                
            }
            
            
            //if no intersection
            if (t == 0.0f) {
                //cout << "black\n";
                finalImage[i][j] = glm::vec3(0.0f, 0.0f, 0.0f);
            }
            else if (t > 0.0f) {
                finalImage[i][j] = specificAmbient;
            }
        }
    }
    
    
    
    
    
    
    //DEALLOCATION AND MEMORY CLEANING BEFORE FILE END
    delete mainCamera;
    if (spheres->size() > 0) {
        for (vector<Sphere*>::iterator it = spheres->begin(); it != spheres->end(); it++) {
            delete (*it);
        }
    }
    
    delete spheres;
    
    if (triangles->size() > 0) {
        for (vector<Triangle*>::iterator it = triangles->begin(); it != triangles->end(); it++) {
            delete (*it);
        }
    }
    delete triangles;
    delete rayHolder;

    //ACTUAL PPM CREATION
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
