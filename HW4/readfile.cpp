//
//  readfile.cpp
//  HW4
//
#include "readfile.h"
#include <iostream>
#include <string>
#include <stack>
#include <fstream>
#include <sstream>
#include <vector>
#include "Camera.h"
#include "Triangle.h"
#include "glm/glm.hpp"

// Function to read the input data values
// Use is optional, but should be very helpful in parsing.
bool readvals(std::stringstream &s, const int numvals, float* values)
{
    for (int i = 0; i < numvals; i++) {
        s >> values[i];
        if (s.fail()) {
            std::cout << "Failed reading value " << i << " will skip\n";
            return false;
        }
    }
    return true;
}
//OLD: void readfile(std::string filename, int & width, int & height, Camera *mainCamera, std::vector<Sphere*> *spheres, std::vector<Triangle*> *triangles)
void readfile(std::string filename, int & width, int & height, Camera *mainCamera, std::vector<Primitive*> *primitives)
{
    //starts the readfile
    std::string str, cmd;
    std::ifstream in;
    std::cout << "PARSING: " << filename << " \n";
    
    glm::vec3 runningAmbient = glm::vec3(0.2f, 0.2f, 0.2f);
    glm::vec3 runningDiffuse = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 runningSpecular = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 runningEmission = glm::vec3(0.0f, 0.0f, 0.0f);
    float runningShininess = 0.0f;
    
    std::vector<glm::vec3> vertices = {};
    
    in.open(filename);
    if (!in.is_open()) {
        exit(EXIT_FAILURE);
    }
    
    if (in.is_open()) {
        std::stack<glm::mat4> transfstack;
        transfstack.push(glm::mat4(1.0f));
        
        
        getline (in, str);
        while (in) {
            if ((str.find_first_not_of(" \t\r\n") != std::string::npos) && (str[0] != '#')) {
                // Ruled out comment and blank lines
                
                std::stringstream s(str);
                s >> cmd;
                //int i;
                float values[10]; // Position and color for light, colors for others
                // Up to 10 params for cameras.
                bool validinput; // Validity of input
                
                // Process the light, add it to database.
                // Lighting Command
                if (cmd == "size") {
                    validinput = readvals(s, 2, values);
                    
                    if (validinput) {
                        width = values[0];
                        height = values[1];
                    }
                    
                }
                else if (cmd == "light") {
                    /*
                     if (numused == numLights) { // No more Lights
                     cerr << "Reached Maximum Number of Lights " << numused << " Will ignore further lights\n";
                     } else {
                     validinput = readvals(s, 8, values); // Position/color for lts.
                     if (validinput) {
                     
                     // YOUR CODE FOR HW 2 HERE.
                     // Note that values[0...7] shows the read in values
                     // Make use of lightposn[] and lightcolor[] arrays in variables.h
                     // Those arrays can then be used in display too.
                     
                     //TODO: ADD values[0:4] to lightposn[] and values[4:8] to lightcolor
                     for (int i = 0; i < 4; i++) {
                     //starting point in lightposn array given by numused * 4
                     lightposn[numused*4 + i] = values[i];
                     }
                     for (int i = 4; i < 8; i++) {
                     lightcolor[numused*4 + i-4] = values[i];
                     }
                     ++numused;
                     }
                     }
                     */
                }
                
                // Material Commands
                // Ambient, diffuse, specular, shininess properties for each object.
                // Filling this in is pretty straightforward, so I've left it in
                // the skeleton, also as a hint of how to do the more complex ones.
                // Note that no transforms/stacks are applied to the colors.
                
                else if (cmd == "ambient") {
                    validinput = readvals(s, 3, values); // colors
                    if (validinput) {
                        runningAmbient.x = float(values[0]);
                        runningAmbient.y = float(values[1]);
                        runningAmbient.z = float(values[2]);
                    }
                }
                else if (cmd == "diffuse") {
                     validinput = readvals(s, 3, values); // colors
                     if (validinput) {
                         runningDiffuse.x = float(values[0]);
                         runningDiffuse.y = float(values[1]);
                         runningDiffuse.z = float(values[2]);
                     }
                }
                else if (cmd == "specular") {
                     validinput = readvals(s, 3, values); // colors
                     if (validinput) {
                         runningSpecular.x = float(values[0]);
                         runningSpecular.y = float(values[1]);
                         runningSpecular.z = float(values[2]);
                     }
                }
                else if (cmd == "emission") {

                     validinput = readvals(s, 3, values); // colors
                     if (validinput) {
                         runningEmission.x = float(values[0]);
                         runningEmission.y = float(values[1]);
                         runningEmission.z = float(values[2]);
                     }

                }
                else if (cmd == "shininess") {

                     validinput = readvals(s, 1, values);
                     if (validinput) {
                         runningShininess = float(values[0]);
                     }

                }
                else if (cmd == "camera") {
                    validinput = readvals(s,10,values); // 10 values eye cen up fov
                    if (validinput) {
                        
                        for (int i = 0; i < 10; i++) {
                            values[i] = float(values[i]);
                        }
                        glm::vec3 cameraPos = glm::vec3(values[0], values[1], values[2]);
                        glm::vec3 lookAtPos = glm::vec3 (values[3], values[4], values[5]);
                        glm::vec3 upVec = glm::vec3(values[6], values[7], values[8]);
                        float fovy = glm::radians(values[9]);
                        float aspect = float(width) / float(height);
                        float fovx = 2.0 * glm::atan(glm::tan(fovy * 0.5) * aspect);
                        
                        mainCamera->setCamera(cameraPos, lookAtPos, upVec, fovy, fovx);

                        
                    }
                }
                
                // I've left the code for loading objects in the skeleton, so
                // you can get a sense of how this works.
                // Also look at demo.txt to get a sense of why things are done this way.
                else if (cmd == "sphere") {
                    /*
                     if (numobjects == maxobjects) { // No more objects
                     cerr << "Reached Maximum Number of Objects " << numobjects << " Will ignore further objects\n";
                     }
                     
                     else {
                    */
                    validinput = readvals(s, 4, values);
                    if (validinput) {
                        glm::vec3 spherePos = glm::vec3(double(values[0]), double(values[1]), double(values[2]));
                        double radius = double(values[3]);
                        Primitive *newSphere = new Sphere(spherePos, radius, runningAmbient, runningDiffuse, runningSpecular, runningEmission, runningShininess, transfstack.top());
                        primitives->push_back(newSphere);
                    }
                }
                else if (cmd == "maxverts") {
                    validinput = readvals(s, 1, values);
                    if (validinput) {
                        //do nothing
                    }
                }
                else if (cmd == "vertex") {
                    validinput = readvals(s, 3, values);
                    if (validinput) {
                        vertices.push_back(glm::vec3(double(values[0]), double(values[1]), double(values[2])));
                    }
                }
                else if (cmd == "tri") {
                    //MAKE TRIANGLE OBJECT
                    validinput = readvals(s, 3, values);
                    if (validinput) {
                        
                        //set up vertices in homogenous
                        glm::vec4 vA = glm::vec4(*(vertices.begin() + values[0]), 1.0f);
                        glm::vec4 vB = glm::vec4(*(vertices.begin() + values[1]), 1.0f);
                        glm::vec4 vC = glm::vec4(*(vertices.begin() + values[2]), 1.0f);
                        
                        vA = transfstack.top() * vA;
                        vB = transfstack.top() * vB;
                        vC = transfstack.top() * vC;
                        
                        glm::vec3 finalA = glm::vec3(vA.x / vA.w, vA.y / vA.w, vA.z / vA.w);
                        glm::vec3 finalB = glm::vec3(vB.x / vB.w, vB.y / vB.w, vB.z / vB.w);
                        glm::vec3 finalC = glm::vec3(vC.x / vC.w, vC.y / vC.w, vC.z / vC.w);
                        
                        
                        //triangle is made after transforming points
                        Primitive *newTriangle = new Triangle(finalA, finalB, finalC, runningAmbient, runningDiffuse, runningSpecular, runningEmission, runningShininess, transfstack.top());
                        
                        primitives->push_back(newTriangle);
                        
                    }
                    
                    
                    
                }
                
                else if (cmd == "translate") {
                    validinput = readvals(s,3,values);
                    if (validinput) {
                        
                        // YOUR CODE FOR HW 2 HERE.
                        // Think about how the transformation stack is affected
                        // You might want to use helper functions on top of file.
                        // Also keep in mind what order your matrix is!
                        
                        glm::mat4 translationMat = glm::mat4(1.0f, 0.0f, 0.0f, 0.0f,
                                                             0.0f, 1.0f, 0.0f, 0.0f,
                                                             0.0f, 0.0f, 1.0f, 0.0f,
                                                             values[0], values[1], values[2], 1.0f);
                        //mat4 translateMatrix = Transform::translate(values[0], values[1], values[2]);
                        glm::mat4 &T = transfstack.top();
                        T = T * translationMat;
                        
                    }
                }
                else if (cmd == "scale") {
                    validinput = readvals(s,3,values);
                    if (validinput) {
                        
                        // YOUR CODE FOR HW 2 HERE.
                        // Think about how the transformation stack is affected
                        // You might want to use helper functions on top of file.
                        // Also keep in mind what order your matrix is!
                        glm::mat4 scaleMat = glm::mat4(values[0],0.0f,0.0f,0.0f,
                                                       0.0f, values[1], 0.0f, 0.0f,
                                                       0.0f, 0.0f, values[2], 0.0f,
                                                       0.0f, 0.0f, 0.0f, 1.0f);
                        
                        glm::mat4 &T = transfstack.top();
                        T = T * scaleMat;
                        
                    }
                }
                else if (cmd == "rotate") {
                    validinput = readvals(s,4,values);
                    if (validinput) {
                        
                        glm::vec3 a(values[0], values[1], values[2]);
                        a = glm::normalize(a);
                        
                        //now, setup first matrix in formula
                        float cosineTheta = glm::cos(glm::radians(values[3]));
                        float sinTheta = glm::sin(glm::radians(values[3]));
                        glm::mat3 firstMatrix(cosineTheta, 0.0f, 0.0f, 0.0f, cosineTheta, 0.0f, 0.0f, 0.0f, cosineTheta);
                        //now, setup second matrix in formula
                        
                        glm::mat3 secondMatrix = (1.0f - cosineTheta) * glm::mat3(a[0] * a[0], a[0] * a[1], a[0] * a[2],
                                                                                  a[0] * a[1], a[1] * a[1], a[1] * a[2],
                                                                                  a[0] * a[2], a[1] * a[2], a[2] * a[2]);
                        //now, setup last matrix in formula
                        glm::mat3 thirdMatrix = sinTheta * glm::mat3(0.0f, a[2], -a[1],
                                                                     -a[2], 0.0f, a[0],
                                                                     a[1], -a[0], 0.0f);
                        glm::mat4 rotationMat = glm::mat4(firstMatrix + secondMatrix + thirdMatrix);
                        
                        glm::mat4 &T = transfstack.top();
                        T = T * rotationMat;
                        
                    }
                }
                
                // I include the basic push/pop code for matrix stacks
                else if (cmd == "pushTransform") {
                    transfstack.push(transfstack.top());
                }
                else if (cmd == "popTransform") {
                    if (transfstack.size() <= 1) {
                        std::cerr << "Stack has no elements.  Cannot Pop\n";
                    }
                    else {
                        transfstack.pop();
                    }
                }
                
                else {
                    std::cerr << "Unknown Command: " << cmd << " Skipping \n";
                }
            }
            getline(in, str);
        }
        std::cout << "DONE READING FILE\n";
        in.close();
        return;
    }
}
