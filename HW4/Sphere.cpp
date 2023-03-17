
#include "Sphere.h"
#include <algorithm>
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


void Sphere::calculateForT(float a, float b, float c, float discriminant, float &tRef, Ray * currRay) {

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
        
            //this currRoot, or t, is in the frame of "transformed Ray" and "untransformed Sphere",
            //however, we only want to replace t in the "untransformed ray" and "transformed Sphere"
            
            //1. convert point into "untransformed ray" and "transformed Sphere" space
            //2. find t value from "untransformed ray" and "transformed Sphere" space
            //3. THIS is the t value we want to compare
            /*
            glm::vec3 transformedRayPoint = currRay->rayStart + currRoot * currRay->rayVec;
            
            glm::vec4 untransformedRayPointHom = this->transformation * glm::vec4(transformedRayPoint, 1.0f);
            
            glm::vec3 untransformedRayPoint =  glm::vec3(untransformedRayPointHom.x / untransformedRayPointHom.w,  untransformedRayPointHom.y / untransformedRayPointHom.w, untransformedRayPointHom.z / untransformedRayPointHom.w);
            */
            
            
            //we havent assigned yet, so assign
            if (tRef == 0.0f) {

                tRef = currRoot;
                //primHolder = this;
            }
            //we HAVE assigned, check if found t is better
            else if (tRef > 0.0f && currRoot < tRef) {

                tRef = currRoot;
                //primHolder = dynamic_cast<Primitive*>(this);
            }
            else if (tRef > 0.0f && currRoot > tRef) {
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
                //primHolder = dynamic_cast<Primitive*>(this);
            }
            //we HAVE assigned, check if found t is better
            else if (tRef > 0.0f && currRoot < tRef) {
                tRef = currRoot;
                //primHolder = dynamic_cast<Primitive*>(this);
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
        return;
    }
    return;
}


 // intersection

void Sphere::calculateIntersection(float &tRef , Ray * currRay) {
    
    //transform ray  by inverse transformation of the sphere FIRST
    //transform the origin point, 1 as homogenous w
    glm::vec3 originalRayStart = currRay->rayStart;
    glm::vec3 originalRayVec = currRay->rayVec;
    
    glm::vec4 newStart = glm::inverse(this->transformation) * glm::vec4(currRay->rayStart, 1.0f);
    glm::vec4 newVec = glm::inverse(this->transformation) * glm::vec4(currRay->rayVec, 0.0f);
    
    currRay->rayStart = glm::vec3(newStart.x / newStart.w, newStart.y / newStart.w, newStart.z / newStart.w);
    currRay->rayVec = glm::vec3(newVec.x, newVec.y, newVec.z);

    float a,b,c, discriminant;
    
    a = glm::dot(currRay->rayVec, currRay->rayVec);
    b = glm::dot( currRay->rayVec + currRay->rayVec, currRay->rayStart - this->spherePos);
    c = glm::dot(currRay->rayStart - this->spherePos, currRay->rayStart - this->spherePos) - pow(this->radius, 2.0f);
    discriminant = b*b - 4.0f*a*c;
    
    calculateForT(a, b, c, discriminant, tRef, currRay);
    
    //put ray back to original form
    currRay->rayStart = originalRayStart;
    currRay->rayVec = originalRayVec;
    return;
    
    
}
Sphere::~Sphere() {};
