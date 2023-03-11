//
//  Ray.h
//  HW4
//
//

#pragma once

#include "glm/glm.hpp"
#include "Camera.h"
class Ray {
    public:
    glm::vec3 rayStart;
    glm::vec3 rayVec;
    Ray();
    //void setRay(float i, float j, float width, float height, Camera * mainCamera);
    void setRay(float i, float j, float width, float height, Camera * mainCamera);
    ~Ray();
};
