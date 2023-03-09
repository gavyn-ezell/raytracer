//
//  Ray.cpp
//  HW4
//
//
#include "Ray.h"
Ray::Ray() {
    
}

void Ray::setRay(float i, float j, float width, float height, Camera * mainCamera) {
    float alpha = glm::tan(mainCamera->fovx / 2.0f) * ( (j - (width / 2.0f)) / (width / 2.0f));
    float beta = glm::tan(mainCamera->fovy / 2.0f) * ( (-i + (height / 2.0f)) / (height / 2.0f));
    
    glm::vec3 w = glm::normalize(mainCamera->cameraPos - mainCamera->lookAtPos);
    glm::vec3 u = glm::normalize(glm::cross(glm::normalize(mainCamera->upVec), w));
    glm::vec3 v = glm::cross(w, u);
    
    this->rayStart = mainCamera->cameraPos;
    this->rayVec= glm::normalize(alpha * u + beta * v - w);
    
}

Ray::~Ray() {
    
}
