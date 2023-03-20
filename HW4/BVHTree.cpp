//
//  BVHTree.cpp
//  HW4
//
//  Created by Gavyn Ezell on 3/18/23.
//

#include "BVHTree.h"

BVHTree::BVHTree(std::vector<Primitive*> *objectArray) {
    //before we create our tree, we find the starting AXIS, x y or z (0 1 or 2)
    /*
     Step 1: find the lowest possible xyz coordinate, and highest possible xyz coordinates given our objects
     Step 2: Pick the highest difference among the respective coords
     Step 3: THIs will be our starting axis for our tree creation, pass into BBoxNode's constructor and set to root;
     */
    
    
    float inf = 999999999.0f;
    float negInf = inf*-1.0f;
    
    glm::vec3 minXYZ = glm::vec3(inf,inf,inf);
    glm::vec3 maxXYZ = glm::vec3(negInf,negInf,negInf);
    //LOOP THROUGH ALL PRIMITIVES TO FIND THE RANGES FOR ALL AXES
    for (std::vector<Primitive*>::iterator it = objectArray->begin(); it != objectArray->end(); it++) {
        if (Sphere * currSphere = dynamic_cast<Sphere*>(*it)) {
            //sphere case is different
            //we transform the center of sphere, and check the radisu within each
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
    //now we have max coords, lets find if biggest range is x y or z, then assign to START_AXIS
    int START_AXIS;
    float xRange = maxXYZ.x - minXYZ.x;
    float yRange = maxXYZ.y - minXYZ.y;
    float zRange = maxXYZ.z - minXYZ.z;
    
    float maxRange = std::max(std::max(xRange, yRange), zRange);
    
    if (xRange == maxRange) {
        
        START_AXIS = 0;
    }
    else if (yRange == maxRange) {
        START_AXIS = 1;
    }
    else {
        START_AXIS = 2;
    }
    this->root = new BBoxNode(objectArray, START_AXIS);
}

void deleteTree(BBoxNode *root)
{
    if(root->isLeaf) {
        delete root;
        return;
    }
    deleteTree(root->leftBBox);
    deleteTree(root->rightBBox);
    delete root;
}

void BVHTree::calculateClosestIntersection(std::tuple<float, glm::vec3, glm::vec3> &intersectionTrackerRef, Ray * currRay, Primitive*& primHolder) {
    this->root->calculateClosestIntersection(intersectionTrackerRef, currRay, primHolder);
}

bool BVHTree::testIfBlocked(Light *light, Ray *shadowRay) {
    return this->root->testIfBlocked(light, shadowRay);
}

BVHTree::~BVHTree() {
    //do inorder
    deleteTree(this->root);
}
