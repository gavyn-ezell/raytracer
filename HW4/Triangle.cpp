//
//  Triangle.cpp
//  HW4
//

#include "Triangle.h"
#include <iostream>
Triangle::Triangle(glm::vec3 vA, glm::vec3 vB, glm::vec3 vC, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 emission, float shininess, glm::mat4 transformation)
{
    //calculate normal given A, B, C
    this->vA = vA;
    this->vB = vB;
    this->vC = vC;
    this->triangleNorm = glm::normalize(glm::cross(vC - vA, vB - vA));
    
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;
    this->emission = emission;
    this->shininess = shininess;
    this->transformation = transformation;
    
}

void Triangle::triangleIntersection(float &tRef, Ray * currRay, glm::vec3 & specificAmbientRef) {
    if (glm::dot(currRay->rayVec, this->triangleNorm) == 0.0f) {
        //no intersection to plane, just return
        //std::cout << float(glm::dot(currRay->rayVec, this->triangleNorm)) << "<-- P1 dot n\n";
        return;
    }
    //intersection exists. is it in triangle though?
    
    float currT = ( glm::dot(this->vA, this->triangleNorm) - glm::dot(currRay->rayStart, this->triangleNorm)) / glm::dot(currRay->rayVec, this->triangleNorm);
    //std::cout << currT << "<--currT\n";
    
    if (currT <= 0.0f || (tRef > 0.0f && currT >= tRef)) {
        return;
    }
    //we hit the plane, now we check if in triangle and also less than current value of T
    else {
        //calculate barycentric
        glm::vec3 P = currRay->rayStart + currT * currRay->rayVec;
        float alpha;
        float beta;
        float gamma;
        
        glm::vec3 vZero = this->vB - this->vA;
        glm::vec3 vOne = this->vC - this->vA;
        glm::vec3 vTwo = P - this->vA;
        
        float dotOne = glm::dot(vZero, vZero);
        float dotTwo = glm::dot(vZero, vOne);
        float dotThree = glm::dot(vOne, vOne);
        float dotFour = glm::dot(vTwo, vZero);
        float dotFive = glm::dot(vTwo, vOne);
        
        float denom = dotOne * dotThree - dotTwo * dotTwo;
        
        beta = (dotThree * dotFour - dotTwo * dotFive) / denom;
        gamma = (dotOne * dotFive - dotTwo * dotFour) / denom;
        alpha = 1.0f - beta - gamma;
        //barycentric variables found, now do final check
        
        if (beta >= 0.0f && beta <= 1.0f && gamma >= 0.0f && gamma <= 1.0f && beta + gamma <= 1.0f) {
            if (((currT < tRef) && (tRef > 0.0f)) || tRef == 0.0f) {
                //std::cout << "CHANGING " << tRef << " to " << currT << "\n";
                tRef = currT;
                specificAmbientRef = 255.0f * this->ambient;
            }
        }
        return;
    }
    
}
Triangle::~Triangle() {
    
}
