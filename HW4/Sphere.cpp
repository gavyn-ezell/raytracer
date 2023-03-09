
#include "Sphere.h"
#include <algorithm>
#include <iostream>
// constructor for sphere

Sphere::Sphere(glm::vec3 inputSpherePos, float radius, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 emission, float shininess, glm::mat4 transformation) {
    this->spherePos = inputSpherePos;
    this->radius = radius;
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;
    this->emission = emission;
    this->shininess = shininess;
    this->transformation = transformation;
}


void Sphere::calculateForT(float a, float b, float c, float discriminant, float &tRef, glm::vec3 &specificAmbientRef) {

    //float realPart, imaginaryPart;
    float x1, x2;
    // 2 pos
    if (discriminant > 0.0f) {
        x1 = (-b + sqrt(discriminant)) / (2.0f*a);
        x2 = (-b - sqrt(discriminant)) / (2.0f*a);

        //both positive? choose the smallest one
        if (x1 > 0.0f && x2 > 0.0f) {
            //FOR NOW DO NOT CARE ABOUT ACTUAL INTERSECTION POINT, JUST RETURN TRUE
            float currRoot = std::min(x1, x2);
            
            //we havent assigned yet, so assign
            if (tRef == 0.0f) {

                tRef = currRoot;
                specificAmbientRef = 255.0f * this->ambient;
            }
            //we HAVE assigned, check if found t is better
            else if (tRef > 0.0f && currRoot < tRef) {

                tRef = currRoot;
                specificAmbientRef = 255.0f * this->ambient;
            }
            else if (tRef > 0.0f && currRoot >= tRef) {
                //don't reassign
            }
            return;
        }
        // one pos, one neg
        else if ((x1 > 0 && x2 < 0) || (x1 < 0 && x2 > 0))  {
            float currRoot = std::max(x1,x2);
            
            //we havent assigned yet, so assign
            if (tRef == 0.0f) {
                tRef = currRoot;
                specificAmbientRef = 255.0f * this->ambient;
            }
            //we HAVE assigned, check if found t is better
            else if (tRef > 0.0f && currRoot < tRef) {
                tRef = currRoot;
                specificAmbientRef = 255.0f * this->ambient;
            }
            else if (tRef > 0.0f && currRoot >= tRef) {
                //don't reassign
            }
            return;
        }
    }
    // tangent, ignore
    else if (discriminant == 0) {
        x1 = -b/(2*a);
        return;
    }
    //no intersection, ignore
    else {
        /*
        realPart = -b/(2*a);
        imaginaryPart = sqrt(-discriminant)/(2*a);
        */
        return;
    }
    return;
}


 // intersection

void Sphere::sphereIntersection(float &tRef , Ray * currRay, glm::vec3 & specificAmbientRef) {
    
    //transform ray  by inverse transformation of the sphere FIRST
    //transform the origin point, 1 as homogenous w
    glm::vec3 originalRayStart = currRay->rayStart;
    glm::vec3 originalRayVec = currRay->rayVec;
    
    glm::vec4 newStart = glm::inverse(this->transformation) * glm::vec4(currRay->rayStart, 1.0f);
    glm::vec4 newVec = glm::inverse(this->transformation) * glm::vec4(currRay->rayVec, 0.0f);
    
    currRay->rayStart = glm::vec3(newStart.x / newStart.w, newStart.y / newStart.w, newStart.z / newStart.w);
    currRay->rayVec = glm::vec3(newVec.x, newVec.y, newVec.z);

    float a,b,c, discriminant;
    
    a = float(glm::dot(currRay->rayVec, currRay->rayVec));
    b = float(glm::dot( currRay->rayVec + currRay->rayVec, currRay->rayStart - this->spherePos));
    c = float(glm::dot(currRay->rayStart - this->spherePos, currRay->rayStart - this->spherePos)) - this->radius * this->radius;
    discriminant = sqrt(b*b - 4.0f*a*c);
    
    calculateForT(a, b, c, discriminant, tRef, specificAmbientRef);
    
    //put ray back to original form
    currRay->rayStart = originalRayStart;
    currRay->rayVec = originalRayVec;
    return;
    
    
}
Sphere::~Sphere() {
    
}
