//
//  Sphere.h
//  HW4
//

#pragma once

#include "glm/glm.hpp"
#include "Ray.h"
#include "Primitive.h"
class Sphere : public Primitive
{
    
    public:
        glm::vec3 spherePos;
        float radius;
        
        Sphere(glm::vec3 inputSpherePos, float radius, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 emission, float shininess, glm::mat4 transformation);
        float calculateT(float a, float b, float c, float discriminant);
        //void calculateIntersection(float &tRef, Ray * currRay);
        void calculateClosestIntersection(std::tuple<float, glm::vec3, glm::vec3> &intersectionTrackerRef, Ray * currRay);
        bool blockingLight(Light * light, Ray * shadowRay);
        virtual ~Sphere();
        
};

