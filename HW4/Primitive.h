//
//  Primitive.h
//  HW4
//
#pragma once

#include "glm/glm.hpp"
#include "Ray.h"
#include "Light.h"
#include <vector>

class Primitive
{
    public:
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        glm::vec3 emission;
        float shininess;
        glm::mat4 transformation;
        virtual ~Primitive() {}
        virtual void calculateIntersection(float &tRef, Ray * currRay) = 0;
        //virtual bool isLit(Ray shadowRay, std::vector<Primitive*> * primitives, Light * currLight);
        //virtual void calculateLighting(glm::vec3 pointOfIntersection, std::vector<Light*> *lights);
};
