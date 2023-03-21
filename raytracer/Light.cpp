//
//  Light.cpp
//  HW4
//

#include "Light.h"

Light::Light(bool isDirectional, glm::vec3 lightPos, glm::vec3 lightColor) {
    this->isDirectional = isDirectional;
    this->lightPos = lightPos;
    this->lightColor = lightColor;
}
Light::~Light() {
}
