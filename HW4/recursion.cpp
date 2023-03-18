//
//  recursion.cpp
//  HW4
//
//  Created by Gavyn Ezell on 3/14/23.
//

#include "recursion.h"
#include <iostream>
glm::vec3 recursiveTracing(int currdepth, Ray * mirrorRayHolder, Ray * shadowRayHolder, std::vector<Primitive*> *primitives, std::vector<Light*> *lights, Camera * mainCamera, glm::vec3 attenuation, int maxdepth)
{
/*
 PRETTY MUCH THE SAME PROCESS AS BEFORE
1. FIND INTERSECTION IF EXISTS
2. IF IT EXISTS, DO I = A + E + SUMMATION LIGHT CALCULATIONS!
3. RECURSIVE CALL: RETURN COLOR + S
*/
    if (currdepth == maxdepth + 1) {
        return glm::vec3(0.0f,0.0f,0.0f);
    }

    Primitive * primHolder = NULL;
    //float t = 0.0f;
    //float &tRef = t;

    std::tuple<float, glm::vec3, glm::vec3> intersectionTracker;
    std::tuple<float, glm::vec3, glm::vec3> &intersectionTrackerRef = intersectionTracker;
    get<0>(intersectionTrackerRef) = 0.0f;
    get<1>(intersectionTrackerRef) = glm::vec3(0.0f,0.0f,0.0f);
    get<2>(intersectionTrackerRef) = glm::vec3(0.0f,0.0f,0.0f);
    
    for (std::vector<Primitive*>::iterator it = primitives->begin(); it != primitives->end(); it++) {
        
        float originalT = get<0>(intersectionTrackerRef);
        (*it)->calculateClosestIntersection(intersectionTrackerRef, mirrorRayHolder);
        if (get<0>(intersectionTrackerRef) != originalT) {
            primHolder = (*it);
        }
        
    }
    
    if (!primHolder) {
        return glm::vec3(0.0f, 0.0f, 0.0f);
    }

    glm::vec3 finalColor = primHolder->ambient + primHolder->emission;
    
    glm::vec3 intersectionPoint = get<1>(intersectionTrackerRef);
    glm::vec3 N = get<2>(intersectionTrackerRef);
    
    for (std::vector<Light*>::iterator it = lights->begin(); it != lights->end(); it++) {
        //incorporates SHADOW BIAS  0.001 * Normal Direction
        
        shadowRayHolder->setShadowRay(intersectionPoint, (*it), N);
        
        bool lightBlocked = false;
        for (std::vector<Primitive*>::iterator it2 = primitives->begin(); it2 != primitives->end(); it2++) {
            
            if ((*it2)->blockingLight((*it), shadowRayHolder)) {
                lightBlocked = true;
                break;
            }
            
        }
        if (lightBlocked) {
            continue;
        }
        else {
            glm::vec3 currAttenuation;
            float r = 0.0f;
            glm::vec3 L;
            glm::vec3 H;
            
            
            if ((*it)->isDirectional) {
                currAttenuation = glm::vec3(1.0f, 0.0f, 0.0f);
                L = glm::normalize((*it)->lightPos);
            }
            else {
                currAttenuation = attenuation;
                L = glm::normalize((*it)->lightPos - shadowRayHolder->rayStart);
                r = sqrt( pow((shadowRayHolder->rayStart.x - ((*it)->lightPos).x), 2.0f) + pow((shadowRayHolder->rayStart.y - ((*it)->lightPos).y), 2.0f) + pow((shadowRayHolder->rayStart.z - ((*it)->lightPos).z), 2.0f));
            }
            
            H = glm::normalize(L + glm::normalize(mirrorRayHolder->rayStart - shadowRayHolder->rayStart));
            //H = glm::normalize(L + glm::normalize(mainCamera->cameraPos - shadowRayHolder->rayStart));
            
            float denom = currAttenuation.x + currAttenuation.y * r + currAttenuation.z * pow(r, 2.0f);
            
            glm::vec3 lightColor = ((*it)->lightColor / denom);
            
            glm::vec3 currAdded = lightColor * primHolder->diffuse * glm::max(glm::dot(N, L), 0.0f) + lightColor * primHolder->specular * pow(glm::max(glm::dot(N, H), 0.0f), float(primHolder->shininess));
            
            finalColor += currAdded;
            
        }
     
        
    }
    
    //do a final recursive ray trace, only if specular component is nonzero in some color
    if (primHolder->specular.x != 0.0f || primHolder->specular.y != 0.0f  || primHolder->specular.z != 0.0f ) {
        //calculate mirror ray
        glm::vec3 mirrorVec = mirrorRayHolder->rayVec - 2.0f * glm::dot(mirrorRayHolder->rayVec, N) * N;
        mirrorVec = glm::normalize(mirrorVec);
        
        mirrorRayHolder->setMirrorRay(shadowRayHolder->rayStart, mirrorVec);

        glm::vec3 recursiveColor = primHolder->specular * recursiveTracing(currdepth+1, mirrorRayHolder, shadowRayHolder, primitives, lights, mainCamera, attenuation, maxdepth);
        
        return finalColor + recursiveColor;
    }
    return finalColor;
}
