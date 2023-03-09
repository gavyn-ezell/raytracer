//
//  Triangle.h
//  HW4
//
//
#pragma once

#include "glm/glm.hpp"
#include "Ray.h"
class Triangle {
public:
    //values by definition
    glm::vec3 vA;
    glm::vec3 vB;
    glm::vec3 vC;
    glm::vec3 triangleNorm;
    
    //values of coloring, lighting, transformation
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::vec3 emission;
    float shininess;
    glm::mat4 transformation;
    
    Triangle(glm::vec3 vA, glm::vec3 vB, glm::vec3 vC, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 emission, float shininess, glm::mat4 transformation);
    void triangleIntersection(float &tRef, Ray * currRay, glm::vec3 &specificAmbientRef);
    ~Triangle();
};

