//
//  BVHTree.h
//  HW4
//
//  Created by Gavyn Ezell on 3/18/23.
//

#pragma once
#include "glm/glm.hpp"
#include "Primitive.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Light.h"
#include "Ray.h"
#include <limits>
#include <iostream>

class BBoxNode {
public:
    glm::vec3 minCoords;
    glm::vec3 maxCoords;
    
    BBoxNode *leftBBox;
    BBoxNode *rightBBox;
    
    bool isLeaf;
    
    std::vector<Primitive*> *childPrimitives;
    
    BBoxNode(std::vector<Primitive*> *objectArray, int axis);
    bool rayIntersects(Ray *ray);
    void calculateClosestIntersection(std::tuple<float, glm::vec3, glm::vec3> &intersectionTrackerRef, Ray *ray, Primitive* &primHolderRef);
    bool testIfBlocked(Light * light, Ray * shadowRay);
    ~BBoxNode();
};
