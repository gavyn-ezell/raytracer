//
//  Light.h
//  HW4
//
//

#pragma once

#include "glm/glm.hpp"
 class Light{
    public:
    glm::vec4 lightPos;
    glm::vec3 lightColor;
    Light(glm::vec4 lightPos, glm::vec3 lightColor);
    ~Light();
};
