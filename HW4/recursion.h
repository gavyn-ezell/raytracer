//
//  recursion.h
//  HW4
//

#pragma once
#include <cstdio>
#include "glm/glm.hpp"
#include "Ray.h"
#include "Light.h"
#include "Primitive.h"
#include "Sphere.h"
#include "Triangle.h"

glm::vec3 recursiveTracing(int currdepth, Ray * mirrorRayHolder, Ray * shadowRayHolder, std::vector<Primitive*> *primitives, std::vector<Light*> *lights, Camera * mainCamera, glm::vec3 attenuation, int maxdepth);
