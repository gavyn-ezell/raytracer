//
//  readfile.h
//  HW4
//
//  Created by Gavyn Ezell on 3/5/23.
//

#pragma once
#include <vector>
#include <string>
#include "Camera.h"
#include "Sphere.h"
#include "Triangle.h"

// goes through our .test file, and properly creates objects/saves data for scene
/*
 Possible Objects that are updated for main.cpp
 1. Camera
    - Creates single camera object and saves to main file
 2. Sphere
    - Creates Sphere(s) object via 'new Sphere()' and pushes to main files vector of spheres
 3. Triangle
    - Create Triangle(s) object via 'new Triangle()' and pushed to main files vector of triangles
 4. Light
    -Creates Light(s) object via 'new Light()'

//void readfile(const char * filename, vector<Sphere> * sceneSpheres, vector<Triangle> scene);
*/
//void readfile(std::vector<int> *inVec);
void readfile(std::string filename, int & width, int & height, Camera * mainCamera, std::vector<Sphere*> *spheres, std::vector<Triangle*> *Triangle);
