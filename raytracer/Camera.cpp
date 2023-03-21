//
//  Camera.cpp
//  HW4
//
//

#include <stdio.h>
#include "Camera.h"
Camera::Camera() {
    
}

void Camera::setCamera(glm::vec3 cameraPos, glm::vec3 lookAtPos, glm::vec3 upVec, float fovy, float fovx) {
    this->cameraPos = cameraPos;
    this->lookAtPos = lookAtPos;
    this->upVec = upVec;
    this->fovy = fovy;
    this->fovx= fovx;
}

Camera::~Camera() {
    
}
