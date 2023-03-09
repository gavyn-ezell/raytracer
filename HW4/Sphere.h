//
//  Sphere.h
//  HW4
//

#pragma once

#include "glm/glm.hpp"
#include "Ray.h"

class Sphere
{
    
    public:
        glm::vec3 spherePos;
        float radius;
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        glm::vec3 emission;
        float shininess;
        glm::mat4 transformation;
        Sphere(glm::vec3 inputSpherePos, float radius, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 emission, float shininess, glm::mat4 transformation);
        void calculateForT(float a, float b, float c, float discriminant, float &tRef, glm::vec3 &specificAmbientRef);
        void sphereIntersection(float &tRef, Ray * currRay, glm::vec3 & specificAmbientRef);
        ~Sphere();
        
};

