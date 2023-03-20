//
//  BBoxNode.cpp
//  HW4
//
//  Created by Gavyn Ezell on 3/18/23.
//

#include "BBoxNode.h"
bool intervalOverlap(glm::vec2 interval1, glm::vec2 interval2) {
    return (((interval1.y >= interval2.x) && (interval1.x <= interval2.x)) ||
            ((interval1.x <= interval2.y) && (interval1.x >= interval2.x)));
}
bool BBoxNode::rayIntersects(Ray *ray) {
    float t_x_min, t_x_max, t_y_min, t_y_max, t_z_min, t_z_max;
    t_x_min = (this->minCoords.x - ray->rayStart.x) / ray->rayVec.x;
    t_x_max = (this->maxCoords.x - ray->rayStart.x) / ray->rayVec.x;
    
    t_y_min = (this->minCoords.y - ray->rayStart.y) / ray->rayVec.y;
    t_y_max = (this->maxCoords.y - ray->rayStart.y) / ray->rayVec.y;
    
    t_z_min = (this->minCoords.z - ray->rayStart.z) / ray->rayVec.z;
    t_z_max = (this->maxCoords.z - ray->rayStart.z) / ray->rayVec.z;
    
    glm::vec2 xInterval = glm::vec2(std::min(t_x_min,t_x_max), std::max(t_x_min,t_x_max));
    glm::vec2 yInterval = glm::vec2(std::min(t_y_min,t_y_max), std::max(t_y_min,t_y_max));
    glm::vec2 zInterval = glm::vec2(std::min(t_z_min,t_z_max), std::max(t_z_min,t_z_max));
    return (intervalOverlap(xInterval,yInterval) && intervalOverlap(yInterval, zInterval) && intervalOverlap(xInterval, zInterval));
}
//helper function, grabs bounds of array of objects
std::tuple<glm::vec3, glm::vec3> getBounds(std::vector<Primitive*> *primitives) {
    std::tuple<glm::vec3, glm::vec3> finalBound;
    float inf = 999999999.0f;
    float negInf = inf*-1.0f;
    
    glm::vec3 minXYZ = glm::vec3(inf,inf,inf);
    glm::vec3 maxXYZ = glm::vec3(negInf,negInf,negInf);
    for (std::vector<Primitive*>::iterator it = primitives->begin(); it != primitives->end(); it ++) {
        if (Sphere * currSphere = dynamic_cast<Sphere*>((*it))) {
            glm::vec4 transformedCenter = currSphere->transformation * glm::vec4(currSphere->spherePos, 1.0f);
            glm::vec3 finalCenter = glm::vec3(transformedCenter.x / transformedCenter.w, transformedCenter.y / transformedCenter.w, transformedCenter.z / transformedCenter.w);
            //now we grab the the max sx, sy, sz
            float sx = currSphere->transformation[0][0];
            float sy = currSphere->transformation[1][1];
            float sz = currSphere->transformation[2][2];
            
            float maxRad = currSphere->radius * std::max(std::max(abs(sx),abs(sy)),abs(sz));
            //best min???
            
            minXYZ.x = std::min(minXYZ.x, finalCenter.x - maxRad);
            minXYZ.y = std::min(minXYZ.y, finalCenter.y - maxRad);
            minXYZ.z = std::min(minXYZ.z, finalCenter.z - maxRad);
            
            maxXYZ.x = std::max(maxXYZ.x, finalCenter.x + maxRad);
            maxXYZ.y = std::max(maxXYZ.y, finalCenter.y + maxRad);
            maxXYZ.z = std::max(maxXYZ.z, finalCenter.z + maxRad);
        }
        else if (Triangle * currTriangle = dynamic_cast<Triangle*>(*it)) {
            std::vector<glm::vec3> vertices;
            vertices.push_back(currTriangle->vA);
            vertices.push_back(currTriangle->vB);
            vertices.push_back(currTriangle->vC);
            
            for (std::vector<glm::vec3>::iterator it = vertices.begin(); it != vertices.end(); it++) {
                //best min???
                minXYZ.x = std::min(minXYZ.x, (*it).x);
                minXYZ.y = std::min(minXYZ.y, (*it).y);
                minXYZ.z = std::min(minXYZ.z, (*it).z);
                
                maxXYZ.x = std::max(maxXYZ.x, (*it).x);
                maxXYZ.y = std::max(maxXYZ.y, (*it).y);
                maxXYZ.z = std::max(maxXYZ.z, (*it).z);
            }
        }
    }
    finalBound = std::make_tuple(minXYZ, maxXYZ);
    return finalBound;
}
//helper function, given an array of primitives, and an axis, finds the range along that axis and gives the midpoint (or plane) within that range
float findMidPlane(std::vector<Primitive*> *objectArray, int axis) {
    float inf = 9999999999.0f;
    float negInf = inf*-1.0f;
    float min = inf;
    float max = negInf;
    for (std::vector<Primitive*>::iterator it = objectArray->begin(); it != objectArray->end(); it++) {
        if (Sphere * currSphere = dynamic_cast<Sphere*>(*it)) {
            glm::vec4 transformedCenter = currSphere->transformation * glm::vec4(currSphere->spherePos, 1.0f);
            glm::vec3 finalCenter = glm::vec3(transformedCenter.x / transformedCenter.w, transformedCenter.y / transformedCenter.w, transformedCenter.z / transformedCenter.w);
            
            float sx = currSphere->transformation[0][0];
            float sy = currSphere->transformation[1][1];
            float sz = currSphere->transformation[2][2];
            
            float maxRad = currSphere->radius * std::max(std::max(abs(sx),abs(sy)),abs(sz));
            
            min = std::min(min, finalCenter[axis] - maxRad);
            max = std::max(max, finalCenter[axis] + maxRad);
        }
        else if (Triangle * currTriangle = dynamic_cast<Triangle*>(*it)) {
            float minOfVertices = std::min(std::min(currTriangle->vA[axis], currTriangle->vB[axis]), currTriangle->vC[axis]);
            float maxOfVertices = std::max(std::max(currTriangle->vA[axis], currTriangle->vB[axis]), currTriangle->vC[axis]);
            
            min = std::min(min, minOfVertices);
            max = std::max(max, maxOfVertices);
        }
    }
    
    float range = max - min;
    float halfAdd = float(range / 2.0f);
    return min + halfAdd;
}

//BBoxNode Constructor, uses recursion for tree creation, bottom up
BBoxNode::BBoxNode(std::vector<Primitive*> *objectArray, int axis) {
    int length = int(objectArray->size());
    
    if (length == 1) {
        this->childPrimitives = new std::vector<Primitive*>();
        this->childPrimitives->push_back((*objectArray)[0]);
        this->isLeaf = true;
        std::tuple<glm::vec3, glm::vec3> bounds = getBounds(objectArray);
        this->minCoords = std::get<0>(bounds);
        this->maxCoords = std::get<1>(bounds);
    }
    else if (length == 2) {
        this->childPrimitives = new std::vector<Primitive*>();
        this->childPrimitives->push_back((*objectArray)[0]);
        this->childPrimitives->push_back((*objectArray)[1]);
        this->isLeaf = true;
        
        std::tuple<glm::vec3, glm::vec3> bounds = getBounds(objectArray);

        this->minCoords = std::get<0>(bounds);
        this->maxCoords = std::get<1>(bounds);
    }
    else {
        
        float midPlane = findMidPlane(objectArray, axis);
        //now we create two vectors
        
        std::vector<Primitive*> *vec1 = new std::vector<Primitive*>();
        std::vector<Primitive*> *vec2 = new std::vector<Primitive*>();
        
        for (std::vector<Primitive*>::iterator it = objectArray->begin(); it != objectArray->end(); it++) {
            if (Sphere * currSphere = dynamic_cast<Sphere*>(*it)) {
                glm::vec4 transformedCenter = currSphere->transformation * glm::vec4(currSphere->spherePos, 1.0f);
                glm::vec3 finalCenter = glm::vec3(transformedCenter.x / transformedCenter.w, transformedCenter.y / transformedCenter.w, transformedCenter.z / transformedCenter.w);
                
                /*
                float sx = currSphere->transformation[0][0];
                float sy = currSphere->transformation[1][1];
                float sz = currSphere->transformation[2][2];
                
                float maxRad = currSphere->radius * std::max(std::max(abs(sx),abs(sy)),abs(sz));
                
                float minOnAxis = finalCenter[axis] - maxRad;
                float maxOnAxis = finalCenter[axis] + maxRad;
                 */
                //best min???
                if (finalCenter[axis] < midPlane) {
                    vec1->push_back((*it));
                }
                else {
                    vec2->push_back(*it);
                }
                
                
            }
            else if (Triangle * currTriangle = dynamic_cast<Triangle*>(*it)) {
                float avg = (currTriangle->vA[axis] + currTriangle->vB[axis] + currTriangle->vC[axis])/ 3.0f;
                if (avg < midPlane) {
                    vec1->push_back((*it));
                }
                else {
                    vec2->push_back(*it);
                }
            }
        }
        
        this->isLeaf = false;
        
        if (vec1->size() == 0 || vec2->size() == 0) {
            //just stop THE RECURSION already I DONT KNOW WHY I HAVE TO DO THIS
            this->childPrimitives = new std::vector<Primitive*>();
            this->isLeaf = true;
            for (std::vector<Primitive*>::iterator it = vec1->begin(); it != vec1->end(); it++) {
                this->childPrimitives->push_back((*it));
            }
            for (std::vector<Primitive*>::iterator it = vec2->begin(); it != vec2->end(); it++) {
                this->childPrimitives->push_back((*it));
            }
            std::tuple<glm::vec3, glm::vec3> bounds = getBounds(objectArray);
            
            this->minCoords = std::get<0>(bounds);
            this->maxCoords = std::get<1>(bounds);
            
            delete vec1;
            delete vec2;
            
        }
        else {
            
            this->leftBBox = new BBoxNode(vec1, (axis+1) % 3);
            this->rightBBox = new BBoxNode(vec2, (axis+1) % 3);
            
            this->minCoords = this->leftBBox->minCoords;
            this->maxCoords = this->leftBBox->maxCoords;
            
            (this->minCoords).x = std::min((this->minCoords).x, (this->rightBBox->minCoords).x);
            (this->minCoords).y = std::min((this->minCoords).y, (this->rightBBox->minCoords).y);
            (this->minCoords).z = std::min((this->minCoords).z, (this->rightBBox->minCoords).z);
            
            (this->maxCoords).x = std::max((this->maxCoords).x, (this->rightBBox->maxCoords).x);
            (this->maxCoords).y = std::max((this->maxCoords).y, (this->rightBBox->maxCoords).y);
            (this->maxCoords).z = std::max((this->maxCoords).z, (this->rightBBox->maxCoords).z);
            
            delete vec1;
            delete vec2;
        }
        
        
    }
    
}

void BBoxNode::calculateClosestIntersection(std::tuple<float, glm::vec3, glm::vec3> &intersectionTrackerRef, Ray *ray, Primitive * &primHolderRef) {
    
    //does this ray even hit me???
    if (!this->rayIntersects(ray)) {
        return;
    }
    if (this->isLeaf) {
        for (std::vector<Primitive*>::iterator it = this->childPrimitives->begin(); it != this->childPrimitives->end(); it++) {
            float originalT = get<0>(intersectionTrackerRef);
            (*it)->calculateClosestIntersection(intersectionTrackerRef, ray);
            if (get<0>(intersectionTrackerRef) != originalT) {
                primHolderRef = (*it);
            }
        }
        return;
    }
    else if (!(this->isLeaf)) {
        
        this->leftBBox->calculateClosestIntersection(intersectionTrackerRef, ray, primHolderRef);
        this->rightBBox->calculateClosestIntersection(intersectionTrackerRef, ray, primHolderRef);
        return;
    }
}
bool BBoxNode::testIfBlocked(Light * light, Ray * shadowRay) {
    //if our ray doesn't even hit current bbox node, it couldn't possibly hit primitives
    //therefore, the shadow ray is not blocked, and hits the light, return false
    if (!this->rayIntersects(shadowRay)) {
        return false;
    }
    //our ray hits the bbox, and our bbox is a leaf node, just check with all primitives
    if (this->isLeaf) {
        for (std::vector<Primitive*>::iterator it = this->childPrimitives->begin(); it != this->childPrimitives->end(); it++) {
            bool intersectionFound = (*it)->blockingLight(light, shadowRay);
            if (intersectionFound) {
                return true;
            }
        }
    }
    else {
        return this->leftBBox->testIfBlocked(light, shadowRay) || this->rightBBox->testIfBlocked(light, shadowRay);
    }
    return false;
}
BBoxNode::~BBoxNode() {
    delete this->childPrimitives;
}
