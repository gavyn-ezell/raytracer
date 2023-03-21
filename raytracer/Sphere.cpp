
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


float Sphere::calculateT(float a, float b, float c, float discriminant) {

    //float realPart, imaginaryPart;
    float x1, x2;
    //float result;
    // 2 pos
    //BELOW ARE ALL IGNORED CASES, WE DONT CARE
    
    // tangent, ignore
    if (discriminant == 0) {
        x1 = -b/(2*a);
        return -1.0f;
    }
    //no intersection, ignore
    else if (discriminant < 0.0f) {
        return -1.0f;
    }
    
    x1 = (-b + sqrt(discriminant)) / (2.0f*a);
    x2 = (-b - sqrt(discriminant)) / (2.0f*a);
    //two intersection points, convert to real world and then compare
    if ((x1 > 0.0f && x2 < 0.0f) || (x1 < 0.0f && x2 > 0.0f)) {
        return std::max(x1,x2);
    }
    if (x1 > 0.0f && x2 > 0.0f) {
        return std::min(x1,x2);
    }
    return -1.0f;
}


 // intersection

void Sphere::calculateClosestIntersection(std::tuple<float, glm::vec3, glm::vec3> &intersectionTrackerRef, Ray * currRay) {
    
    float previousT = std::get<0>(intersectionTrackerRef);
    //transform ray  by inverse transformation of the sphere FIRST
    
    glm::vec4 newStart = glm::inverse(this->transformation) * glm::vec4(currRay->rayStart, 1.0f);
    glm::vec4 newVec = glm::inverse(this->transformation) * glm::vec4(currRay->rayVec, 0.0f);
    
    Ray transformedRay = Ray();
    transformedRay.rayStart = glm::vec3(newStart.x / newStart.w, newStart.y / newStart.w, newStart.z / newStart.w);
    transformedRay.rayVec  = glm::vec3(newVec.x, newVec.y, newVec.z);
    
    float a,b,c, discriminant;
    
    a = glm::dot(transformedRay.rayVec, transformedRay.rayVec);
    b = glm::dot(transformedRay.rayVec + transformedRay.rayVec, transformedRay.rayStart - this->spherePos);
    c = glm::dot(transformedRay.rayStart - this->spherePos, transformedRay.rayStart - this->spherePos) - pow(this->radius, 2.0f);
    discriminant = b*b - 4.0f*a*c;
    
    //we have currently grabbed the t value from the transformed Ray to the untransformed sphere
    
    float currT = calculateT(a, b, c, discriminant);
    if (currT > 0.0f) {
        //we need the original world t, convert this point to REAL point
        
        if ((currT < previousT && previousT > 0.0f) || previousT == 0.0f) {
            //we need the original world t, convert this point to REAL point
            glm::vec3 transformedPoint = transformedRay.rayStart + currT * transformedRay.rayVec;
            glm::vec4 homPoint = this->transformation * glm::vec4(transformedPoint, 1.0f);
            glm::vec3 realPoint = glm::vec3(homPoint.x / homPoint.w,homPoint.y / homPoint.w, homPoint.z / homPoint.w);
            
            glm::vec3 transformedNormal = glm::normalize(transformedPoint - this->spherePos);
            glm::vec3 realNormal = glm::normalize(glm::inverse(glm::transpose(glm::mat3(this->transformation))) * transformedNormal);
            
            std::get<0>(intersectionTrackerRef) = currT;
            
            std::get<1>(intersectionTrackerRef) = realPoint;
            std::get<2>(intersectionTrackerRef) = realNormal;
            return;
        }
    }
    return;
}

bool Sphere::blockingLight(Light * light, Ray * shadowRay) {
    
    glm::vec4 newStart = glm::inverse(this->transformation) * glm::vec4(shadowRay->rayStart, 1.0f);
    glm::vec4 newVec = glm::inverse(this->transformation) * glm::vec4(shadowRay->rayVec, 0.0f);
    
    Ray transformedRay = Ray();
    transformedRay.rayStart = glm::vec3(newStart.x / newStart.w, newStart.y / newStart.w, newStart.z / newStart.w);
    transformedRay.rayVec = glm::vec3(newVec.x, newVec.y, newVec.z);
    
    float a,b,c, discriminant;
    
    a = glm::dot(transformedRay.rayVec, transformedRay.rayVec);
    b = glm::dot(transformedRay.rayVec + transformedRay.rayVec, transformedRay.rayStart - this->spherePos);
    c = glm::dot(transformedRay.rayStart - this->spherePos, transformedRay.rayStart - this->spherePos) - pow(this->radius, 2.0f);
    discriminant = b*b - 4.0f*a*c;
    
    //we have currently grabbed the t value from the transformed Ray to the untransformed sphere
    float currT = calculateT(a, b, c, discriminant);
    if (currT > 0.0f) {
        
        if (light->isDirectional) {
            return true;
        }
        
        float lightT = (light->lightPos.x - shadowRay->rayStart.x) / shadowRay->rayVec.x;
        if (currT <= lightT) {
            return true;
        }
        
        
    }
    return false;
    
}

Sphere::~Sphere() {};
