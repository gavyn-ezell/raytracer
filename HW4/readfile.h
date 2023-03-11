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
#include "Primitive.h"
#include "Sphere.h"
#include "Triangle.h"

// goes through our .test file, and properly creates objects/saves data for scene
/*
 Possible Objects that are updated for main.cpp
 1. Camera
    - Creates single camera object and saves to main file
 2. Primitives (covers SPhere and Tiranlges)
    - Simply a base class for sphere and triangle
 2. Sphere :: Primitive
    - Creates Sphere(s) object via 'new Sphere()' and pushes to main file's vector of primitives
 3. Triangle :: Primitive
    - Create Triangle(s) object via 'new Triangle()' and pushed to main file's vector of primitives
 4. Light
    -Creates Light(s) object via 'new Light()'
*/

 //OLD: void readfile(std::string filename, int & width, int & height, Camera * mainCamera, std::vector<Sphere*> *spheres, std::vector<Triangle*> *Triangle);
void readfile(std::string filename, int & width, int & height, Camera * mainCamera, std::vector<Primitive*> *primitives);
