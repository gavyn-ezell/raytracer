//
//  Triangle.h
//  HW4
//
#pragma once

#include "glm/glm.hpp"
#include "Ray.h"
#include "Primitive.h"
class Triangle : public Primitive
{
public:
    // values by definition
    glm::vec3 vA;
    glm::vec3 vB;
    glm::vec3 vC;
    glm::vec3 faceNorm;
    glm::vec3 vANorm;
    glm::vec3 vBNorm;
    glm::vec3 vCNorm;

    Triangle(glm::vec3 vA, glm::vec3 vB, glm::vec3 vC,
             glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 emission, float shininess, glm::mat4 transformation);
    Triangle(glm::vec3 vA, glm::vec3 vB, glm::vec3 vC,
             glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
             glm::vec3 emission, float shininess, glm::mat4 transformation,
             glm::vec3 vANorm, glm::vec3 vBNorm, glm::vec3 vCNorm);

    void calculateClosestIntersection(std::tuple<float, glm::vec3, glm::vec3> &intersectionTrackerRef, Ray *currRay);
    bool blockingLight(Light *light, Ray *shadowRay);
    virtual ~Triangle();
};
