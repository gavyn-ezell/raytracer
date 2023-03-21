//
//  BVHTree.h
//  HW4
//
//  Created by Gavyn Ezell on 3/18/23.
//

#pragma once
#include <iostream>
#include "glm/glm.hpp"
#include "BBoxNode.h"
#include "Primitive.h"
#include "Light.h"
#include "Ray.h"

class BVHTree {
public:
    BBoxNode * root;
    BVHTree(std::vector<Primitive*> * objectArray);
    void calculateClosestIntersection(std::tuple<float, glm::vec3, glm::vec3> &intersectionTrackerRef, Ray * currRay, Primitive *& primHolderRef);
    bool testIfBlocked(Light *light, Ray *shadowRay);
    void deleteTraversal();
    ~BVHTree();
};
