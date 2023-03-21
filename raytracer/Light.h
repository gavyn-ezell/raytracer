//
//  Light.h
//  HW4
//
//

#pragma once

#include "glm/glm.hpp"
 class Light{
    public:
    bool isDirectional;
    //although named lightPos for "light position", can also just be vec for directional lights
    glm::vec3 lightPos;
    glm::vec3 lightColor;
    Light(bool isDirectional, glm::vec3 lightPos, glm::vec3 lightColor);
    ~Light();
};
