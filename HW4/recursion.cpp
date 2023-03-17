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
1. FIND INTERSECTION IF EXISTS
2. IF IT EXISTS, DO I = A + E + SUMMATION LIGHT CALCULATIONS!
3. RECURSIVE CALL: RETURN COLOR + S
*/
    std::cout << "GLOBAL ATTENUATION: " << attenuation.x << " " << attenuation.y << " " << attenuation.z << "\n";
    if (currdepth == maxdepth + 1) {
        //std::cout << "RECURSION DONE\n";
        return glm::vec3(0.0f,0.0f,0.0f);
    }

    Primitive * primHolder = NULL;
    float t = 0.0f;
    float &tRef = t;

    
    for (std::vector<Primitive*>::iterator it = primitives->begin(); it != primitives->end(); it++) {
        float original = tRef;
        (*it)->calculateIntersection(tRef, mirrorRayHolder);
        if (tRef != original) {
            primHolder = (*it);
        }
        
    }
    
    if (!primHolder) {
        return glm::vec3(0.0f, 0.0f, 0.0f);
    }

    glm::vec3 finalColor = primHolder->ambient + primHolder->emission;
    
    //first, grab the exact intersection
    
    glm::vec3 intersectionPoint;
    glm::vec3 N;
    
    if(Sphere * currSphere = dynamic_cast<Sphere*>(primHolder))
    {
        //handles Sphere case, which deals with transformations
        //we wanna transform our intersection point of: transformed ray and UNtransformed sphere
        //into intersection point of: UNtransformed ray and transformed sphere
        glm::vec3 originalStart = mirrorRayHolder->rayStart;
        glm::vec3 originalVec = mirrorRayHolder->rayVec;
        
        glm::vec4 newStart = glm::inverse(currSphere->transformation) * glm::vec4(mirrorRayHolder->rayStart, 1.0f);
        glm::vec4 newVec = glm::inverse(currSphere->transformation) * glm::vec4(mirrorRayHolder->rayVec, 0.0f);
        
        mirrorRayHolder->rayStart = glm::vec3(newStart.x / newStart.w, newStart.y / newStart.w, newStart.z / newStart.w);
        mirrorRayHolder->rayVec = glm::vec3(newVec.x, newVec.y, newVec.z);
        
        glm::vec3 transformedIntersectionPoint = mirrorRayHolder->rayStart + t * mirrorRayHolder->rayVec;
        N = glm::normalize(transformedIntersectionPoint - currSphere->spherePos);
        
        //now WE transform BACK, equations shown below
        // M * p for point
        // inverse(transpose(M)) * N for normal
        N = glm::normalize(glm::inverse(glm::transpose(glm::mat3(currSphere->transformation))) * N);
        
        glm::vec4 intersectionPointHom = currSphere->transformation * glm::vec4(transformedIntersectionPoint, 1.0f);
        
        intersectionPoint = glm::vec3(intersectionPointHom.x / intersectionPointHom.w, intersectionPointHom.y / intersectionPointHom.w, intersectionPointHom.z / intersectionPointHom.w);
       
        
        mirrorRayHolder->rayStart = originalStart;
        mirrorRayHolder->rayVec = originalVec;
        
        
            
        //now we have correct intersection point and normal associated with it
      
    }
    else if(Triangle * currTriangle = dynamic_cast<Triangle*>(primHolder))
    {
        //our triangles are pre transformed, no further calculations
        intersectionPoint = mirrorRayHolder->rayStart + t * mirrorRayHolder->rayVec;
        N = glm::normalize(currTriangle->triangleNorm);
        
    }
    for (std::vector<Light*>::iterator it = lights->begin(); it != lights->end(); it++) {
        //incorporates SHADOW BIAS  0.001 * Normal Direction
        
        shadowRayHolder->setShadowRay(intersectionPoint, (*it), N);

        float currT = 0.0f;
        float & currTRef = currT;
        
        
        float lightT = 0.0f;
        if (!(*it)->isDirectional) {
            lightT = (((*it)->lightPos).x - shadowRayHolder->rayStart.x) / (shadowRayHolder->rayVec).x;
        }
        
        

        
        bool foundIntersection = false;
        for (std::vector<Primitive*>::iterator it2 = primitives->begin(); it2 != primitives->end(); it2++) {
            (*it2)->calculateIntersection(currTRef, shadowRayHolder);
            
            if (currT == 0.0f) {
                continue;
            }
            if (!(*it)->isDirectional) {
                
                if (currTRef < lightT) {
                    
                    foundIntersection = true;
                    break;
                }
            }
            else {
                if (currTRef != 0.0f) {
                    foundIntersection = true;
                    break;
                }
            }
        }
        if (foundIntersection) {
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
            
            glm::vec3 rest = lightColor * primHolder->diffuse * glm::max(glm::dot(N, L), 0.0f) + lightColor * primHolder->specular * pow(glm::max(glm::dot(N, H), 0.0f), float(primHolder->shininess));
            
            finalColor += rest;
            
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
    return glm::vec3(0.0f,0.0f,0.0f);
}
