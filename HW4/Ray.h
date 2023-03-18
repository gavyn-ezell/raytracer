//
//  Ray.h
//  HW4
//
//

#pragma once

#include "glm/glm.hpp"
#include "Camera.h"
#include "Light.h"
class Ray {
    public:
    glm::vec3 rayStart;
    glm::vec3 rayVec;
    Ray();
    //void setRay(float i, float j, float width, float height, Camera * mainCamera);
    //void setShadowRay(glm::vec3 intersectionPoint, glm::vec3 lightPos);
    void setMirrorRay(glm::vec3 rayStart, glm::vec3 rayVec);
    void setShadowRay(glm::vec3 intersectionPoint, Light* light, glm::vec3 N);
    void setPixelRay(float i, float j, float width, float height, Camera * mainCamera);
    ~Ray();
};
