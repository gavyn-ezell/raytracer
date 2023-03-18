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
        //virtual std::tuple<float, glm::vec3, glm::vec3> calculateClosestIntersection(std::tuple<float, glm::vec3, glm::vec3>, Ray * currRay) = 0;
        virtual void calculateClosestIntersection(std::tuple<float, glm::vec3, glm::vec3> &intersectionTrackerRef, Ray * currRay) = 0;
        virtual bool blockingLight(Light * light, Ray * shadowRay) = 0;
        //virtual void calculateIntersection(float &tRef, Ray * currRay) = 0;
};
