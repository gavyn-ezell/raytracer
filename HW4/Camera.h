//
//  Camera.h
//  HW4
//
//  Created by Gavyn Ezell on 3/5/23.
//

#pragma once

#include "glm/glm.hpp"

class Camera {
    public:
        glm::vec3 cameraPos;
        glm::vec3 lookAtPos;
        glm::vec3 upVec;
        float fovy;
        float fovx;
        Camera();
        void setCamera(glm::vec3 cameraPos, glm::vec3 lookAtPos, glm::vec3 upVec, float fovy, float fovx);
        //void setCamera(glm::vec3 cameraPos, glm::vec3 lookAtPos, glm::vec3 upVec, double fovy, double fovx);
        ~Camera();
    
    
};

