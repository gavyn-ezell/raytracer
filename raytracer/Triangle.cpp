//
//  Triangle Class Implementation
//

#include "Triangle.h"
#include <iostream>
Triangle::Triangle(glm::vec3 vA, glm::vec3 vB, glm::vec3 vC, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 emission, float shininess, glm::mat4 transformation)
{
    // calculate normal given A, B, C
    this->vA = vA;
    this->vB = vB;
    this->vC = vC;
    this->triangleNorm = glm::normalize(glm::cross(vB - vA, vC - vA));
    // this->triangleNorm = glm::normalize(glm::cross(vC - vA, vB-vA));

    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;
    this->emission = emission;
    this->shininess = shininess;
    this->transformation = transformation;
}
Triangle::Triangle(glm::vec3 vA, glm::vec3 vB, glm::vec3 vC)
{
    this->vA = vA;
    this->vB = vB;
    this->vC = vC;

    this->triangleNorm = glm::normalize(glm::cross(vB - vA, vC - vA));
}

void Triangle::calculateClosestIntersection(std::tuple<float, glm::vec3, glm::vec3> &intersectionTrackerRef, Ray *currRay)
{

    float previousT = std::get<0>(intersectionTrackerRef);
    if (glm::dot(currRay->rayVec, this->triangleNorm) == 0.0f)
    {
        // no intersection to plane, just return, without changing tuple
        return;
    }
    // intersection exists. is it in triangle though?

    float currT = (glm::dot(this->vA, this->triangleNorm) - glm::dot(currRay->rayStart, this->triangleNorm)) / glm::dot(currRay->rayVec, this->triangleNorm);

    // TWO cases where we dont care about this point of intersection to the plane
    // 1. our t value to the plane is negative, we don't care
    // 2. our t value is positive, but it's greater than currently tracked t value, so it couldn't be cloesest point of intersection
    if (currT <= 0.0f || (previousT > 0.0f && currT >= previousT))
    {
        return;
    }

    // we hit the plane, now we check if in triangle and also less than current value of T
    else
    {
        // calculate barycentric
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
        // barycentric variables found, now do final check

        if (beta >= 0.0f && beta <= 1.0f && gamma >= 0.0f && gamma <= 1.0f && beta + gamma <= 1.0f)
        {
            if (((currT < previousT) && (previousT > 0.0f)) || previousT == 0.0f)
            {

                glm::vec3 intersectionPoint = currRay->rayStart + currT * currRay->rayVec;
                glm::vec3 normal = glm::normalize(this->triangleNorm);

                std::get<0>(intersectionTrackerRef) = currT;
                std::get<1>(intersectionTrackerRef) = intersectionPoint;
                std::get<2>(intersectionTrackerRef) = normal;
                return;
            }
        }
    }
    return;
}
bool Triangle::blockingLight(Light *light, Ray *shadowRay)
{
    if (glm::dot(shadowRay->rayVec, this->triangleNorm) == 0.0f)
    {
        // no intersection to plane, just return false;
        return false;
    }

    // grab the exact t value where we hit the plane
    float currT = (glm::dot(this->vA, this->triangleNorm) - glm::dot(shadowRay->rayStart, this->triangleNorm)) / glm::dot(shadowRay->rayVec, this->triangleNorm);
    if (currT <= 0.0f)
    {
        return false;
    }
    glm::vec3 P = shadowRay->rayStart + currT * shadowRay->rayVec;
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

    // barycentric variables found, are we even inside the triangle? or are we just hitting the plane
    if (beta >= 0.0f && beta <= 1.0f && gamma >= 0.0f && gamma <= 1.0f && beta + gamma <= 1.0f)
    {
        // we are inside of a triangle, final lighting check, different for point or directional light

        // if light is directional, and we are inside triangle, we are guaranteed blocking directional light
        if (light->isDirectional)
        {
            return true;
        }

        // if our light is point we need to verify
        // our t value to the triangle is less than t value to the light
        float lightT = (light->lightPos.x - shadowRay->rayStart.x) / shadowRay->rayVec.x;
        if (currT <= lightT)
        {
            return true;
        }
    }
    return false;
}
Triangle::~Triangle(){};
