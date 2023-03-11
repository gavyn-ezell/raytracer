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
#include "Primitive.h"
#include "Sphere.h"
#include "Triangle.h"
//#include "Light.h"
#include "Camera.h"
#include "glm/glm.hpp"
using namespace std;

Camera *mainCamera = new Camera();

//we use one vector to hold all primitives
vector<Primitive*> *primitives = new vector<Primitive*>();

int imgWidth;
int imgHeight;
int &width = imgWidth;
int &height = imgHeight;

int main(int argc, const char * argv[]) {

    string filename = argv[1];
    readfile(filename, width, height, mainCamera, primitives);

    glm::vec3 finalImage[imgHeight][imgWidth];
    
    float t;
    float &tRef = t;
 

    Ray *rayHolder = new Ray();
    Primitive * primHolder;
    
    for (int i = 0; i < imgHeight; i++) {
        for (int j = 0; j < imgWidth; j++) {
            
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
            
            //if we found a valid t value for our Ray, then color
            if (t == 0.0f) {
                finalImage[i][j] = glm::vec3(0.0f, 0.0f, 0.0f);
            }
            else if (t > 0.0f) {
                finalImage[i][j] = 255.0f * primHolder->ambient;
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
    delete rayHolder;

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
