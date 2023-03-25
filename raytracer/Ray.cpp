//
//  Ray.cpp
//  HW4
//
//
#include "Ray.h"
Ray::Ray()
{
}

void Ray::setShadowRay(glm::vec3 intersectionPoint, Light *light, glm::vec3 N)
{
    // make sure to add SHADOW BIAS
    this->rayStart = intersectionPoint + 0.01f * N;
    if (light->isDirectional)
    {
        this->rayVec = glm::normalize(light->lightPos);
    }
    else
    {
        this->rayVec = glm::normalize(light->lightPos - this->rayStart);
    }
}
void Ray::setPixelRay(float i, float j, float width, float height, Camera *mainCamera)
{
    float alpha = glm::tan(mainCamera->fovx / 2.0f) * ((j - (width / 2.0f)) / (width / 2.0f));
    float beta = glm::tan(mainCamera->fovy / 2.0f) * ((-i + (height / 2.0f)) / (height / 2.0f));

    glm::vec3 w = glm::normalize(mainCamera->cameraPos - mainCamera->lookAtPos);
    glm::vec3 u = glm::normalize(glm::cross(glm::normalize(mainCamera->upVec), w));
    glm::vec3 v = glm::cross(w, u);

    this->rayStart = mainCamera->cameraPos;
    this->rayVec = glm::normalize(alpha * u + beta * v - w);
}

void Ray::setMirrorRay(glm::vec3 rayStart, glm::vec3 rayVec)
{
    this->rayStart = rayStart;
    this->rayVec = rayVec;
}

Ray::~Ray()
{
}
