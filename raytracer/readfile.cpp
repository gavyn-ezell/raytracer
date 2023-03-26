//
//  readfile.cpp
//  reafile structured around CSE 167 HW2
//  HW4
//
#include "readfile.h"
#include <iostream>
#include <string>
#include <stack>
#include <fstream>
#include <sstream>
#include "glm/glm.hpp"

// Function to read the input data values
bool readvals(std::stringstream &s, const int numvals, float *values)
{
    for (int i = 0; i < numvals; i++)
    {
        s >> values[i];
        if (s.fail())
        {
            std::cout << "Failed reading value " << i << " will skip\n";
            return false;
        }
    }
    return true;
}

void readfile(std::string filename, int &width, int &height, Camera *mainCamera, std::vector<Primitive *> *primitives, std::vector<Light *> *lights, glm::vec3 &attenuationRef, int &maxdepthRef, std::string &outputRef)
{
    // starts the readfile
    std::string str, cmd;
    std::ifstream in;
    std::cout << "PARSING: " << filename << " \n";

    glm::vec3 runningAmbient = glm::vec3(0.2f, 0.2f, 0.2f);
    glm::vec3 runningDiffuse = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 runningSpecular = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 runningEmission = glm::vec3(0.0f, 0.0f, 0.0f);
    float runningShininess = 0.0f;

    /*
    std::vector<glm::vec3> vertices = {};
    std::vector<glm::vec3> vertexNormals = {};
    */
    std::vector<glm::vec3> *vertices = new std::vector<glm::vec3>();
    std::vector<glm::vec3> *vertexNormals = new std::vector<glm::vec3>();

    in.open(filename);
    if (!in.is_open())
    {
        std::cout << "FAILED TO OPEN\n";
        exit(EXIT_FAILURE);
    }

    if (in.is_open())
    {
        std::stack<glm::mat4> transfstack;
        transfstack.push(glm::mat4(1.0f));

        getline(in, str);
        while (in)
        {
            if ((str.find_first_not_of(" \t\r\n") != std::string::npos) && (str[0] != '#'))
            {
                // Ruled out comment and blank lines

                std::stringstream s(str);
                s >> cmd;
                float values[10];
                bool validinput; // Validity of input

                // Process the light, add it to database.
                // Lighting Command
                if (cmd == "size")
                {
                    validinput = readvals(s, 2, values);

                    if (validinput)
                    {
                        width = values[0];
                        height = values[1];
                    }
                }
                else if (cmd == "output")
                {
                    std::string outputName;
                    s >> outputName;
                    outputRef = outputName;
                }
                else if (cmd == "maxdepth")
                {
                    validinput = readvals(s, 1, values);

                    if (validinput)
                    {
                        maxdepthRef = values[0];
                    }
                }
                else if (cmd == "directional")
                {
                    validinput = readvals(s, 6, values);

                    if (validinput)
                    {
                        glm::vec3 lightPos = glm::vec3(float(values[0]), float(values[1]), float(values[2]));
                        glm::vec3 lightColor = glm::vec3(float(values[3]), float(values[4]), float(values[5]));

                        Light *newLight = new Light(true, lightPos, lightColor);
                        lights->push_back(newLight);
                    }
                }

                else if (cmd == "point")
                {
                    validinput = readvals(s, 6, values);

                    if (validinput)
                    {
                        glm::vec3 lightPos = glm::vec3(float(values[0]), float(values[1]), float(values[2]));
                        glm::vec3 lightColor = glm::vec3(float(values[3]), float(values[4]), float(values[5]));

                        Light *newLight = new Light(false, lightPos, lightColor);
                        lights->push_back(newLight);
                    }
                }
                else if (cmd == "attenuation")
                {
                    validinput = readvals(s, 3, values);

                    if (validinput)
                    {
                        attenuationRef = glm::vec3(float(values[0]), float(values[1]), float(values[2]));
                    }
                }

                else if (cmd == "ambient")
                {
                    validinput = readvals(s, 3, values); // colors
                    if (validinput)
                    {
                        runningAmbient.x = float(values[0]);
                        runningAmbient.y = float(values[1]);
                        runningAmbient.z = float(values[2]);
                    }
                }
                else if (cmd == "diffuse")
                {
                    validinput = readvals(s, 3, values); // colors
                    if (validinput)
                    {
                        runningDiffuse.x = float(values[0]);
                        runningDiffuse.y = float(values[1]);
                        runningDiffuse.z = float(values[2]);
                    }
                }
                else if (cmd == "specular")
                {
                    validinput = readvals(s, 3, values); // colors
                    if (validinput)
                    {
                        runningSpecular.x = float(values[0]);
                        runningSpecular.y = float(values[1]);
                        runningSpecular.z = float(values[2]);
                    }
                }
                else if (cmd == "emission")
                {

                    validinput = readvals(s, 3, values); // colors
                    if (validinput)
                    {
                        runningEmission.x = float(values[0]);
                        runningEmission.y = float(values[1]);
                        runningEmission.z = float(values[2]);
                    }
                }
                else if (cmd == "shininess")
                {

                    validinput = readvals(s, 1, values);
                    if (validinput)
                    {
                        runningShininess = float(values[0]);
                    }
                }
                else if (cmd == "camera")
                {
                    validinput = readvals(s, 10, values); // 10 values eye cen up fov
                    if (validinput)
                    {

                        for (int i = 0; i < 10; i++)
                        {
                            values[i] = float(values[i]);
                        }
                        glm::vec3 cameraPos = glm::vec3(values[0], values[1], values[2]);
                        glm::vec3 lookAtPos = glm::vec3(values[3], values[4], values[5]);
                        glm::vec3 upVec = glm::vec3(values[6], values[7], values[8]);
                        float fovy = glm::radians(values[9]);
                        float aspect = float(width) / float(height);
                        float fovx = 2.0 * glm::atan(glm::tan(fovy * 0.5) * aspect);

                        mainCamera->setCamera(cameraPos, lookAtPos, upVec, fovy, fovx);
                    }
                }

                else if (cmd == "sphere")
                {
                    validinput = readvals(s, 4, values);
                    if (validinput)
                    {
                        glm::vec3 spherePos = glm::vec3(double(values[0]), double(values[1]), double(values[2]));
                        float radius = float(values[3]);
                        Primitive *newSphere = new Sphere(spherePos, radius, runningAmbient, runningDiffuse, runningSpecular, runningEmission, runningShininess, transfstack.top());
                        primitives->push_back(newSphere);
                    }
                }
                else if (cmd == "maxverts")
                {
                    validinput = readvals(s, 1, values);
                    if (validinput)
                    {
                        // do nothing
                    }
                }
                else if (cmd == "vertex" || cmd == "v")
                {
                    validinput = readvals(s, 3, values);
                    if (validinput)
                    {
                        vertices->push_back(glm::vec3(float(values[0]), float(values[1]), float(values[2])));
                    }
                }
                else if (cmd == "vn")
                {
                    validinput = readvals(s, 3, values);

                    if (validinput)
                    {
                        vertexNormals->push_back(glm::vec3(float(values[0]), float(values[1]), float(values[2])));
                    }
                }
                else if (cmd == "tri")
                {
                    // MAKE TRIANGLE OBJECT
                    validinput = readvals(s, 3, values);
                    if (validinput)
                    {

                        // set up vertices in homogenous
                        glm::vec4 vA = glm::vec4(*(vertices->begin() + values[0]), 1.0f);
                        glm::vec4 vB = glm::vec4(*(vertices->begin() + values[1]), 1.0f);
                        glm::vec4 vC = glm::vec4(*(vertices->begin() + values[2]), 1.0f);

                        vA = transfstack.top() * vA;
                        vB = transfstack.top() * vB;
                        vC = transfstack.top() * vC;

                        glm::vec3 finalA = glm::vec3(vA.x / vA.w, vA.y / vA.w, vA.z / vA.w);
                        glm::vec3 finalB = glm::vec3(vB.x / vB.w, vB.y / vB.w, vB.z / vB.w);
                        glm::vec3 finalC = glm::vec3(vC.x / vC.w, vC.y / vC.w, vC.z / vC.w);

                        // triangle is made after transforming points
                        Primitive *newTriangle = new Triangle(finalA, finalB, finalC, runningAmbient, runningDiffuse, runningSpecular, runningEmission, runningShininess, transfstack.top());

                        primitives->push_back(newTriangle);
                    }
                }
                else if (cmd == "trinorm")
                {

                    validinput = readvals(s, 6, values);

                    if (validinput)
                    {

                        glm::vec4 vA = glm::vec4(*(vertices->begin() + values[0]), 1.0f);
                        glm::vec4 vB = glm::vec4(*(vertices->begin() + values[1]), 1.0f);
                        glm::vec4 vC = glm::vec4(*(vertices->begin() + values[2]), 1.0f);

                        vA = transfstack.top() * vA;
                        vB = transfstack.top() * vB;
                        vC = transfstack.top() * vC;

                        glm::vec3 finalA = glm::vec3(vA.x / vA.w, vA.y / vA.w, vA.z / vA.w);
                        glm::vec3 finalB = glm::vec3(vB.x / vB.w, vB.y / vB.w, vB.z / vB.w);
                        glm::vec3 finalC = glm::vec3(vC.x / vC.w, vC.y / vC.w, vC.z / vC.w);

                        glm::vec3 vANorm = glm::normalize(glm::vec3(*(vertexNormals->begin() + values[3])));
                        glm::vec3 vBNorm = glm::normalize(glm::vec3(*(vertexNormals->begin() + values[4])));
                        glm::vec3 vCNorm = glm::normalize(glm::vec3(*(vertexNormals->begin() + values[5])));

                        Primitive *newTriangle = new Triangle(finalA, finalB, finalC,
                                                              runningAmbient, runningDiffuse, runningSpecular, runningEmission, runningShininess, transfstack.top(),
                                                              vANorm, vBNorm, vCNorm);

                        primitives->push_back(newTriangle);
                    }
                }

                else if (cmd == "translate")
                {
                    validinput = readvals(s, 3, values);
                    if (validinput)
                    {

                        glm::mat4 translationMat = glm::mat4(1.0f, 0.0f, 0.0f, 0.0f,
                                                             0.0f, 1.0f, 0.0f, 0.0f,
                                                             0.0f, 0.0f, 1.0f, 0.0f,
                                                             values[0], values[1], values[2], 1.0f);
                        glm::mat4 &T = transfstack.top();
                        T = T * translationMat;
                    }
                }
                else if (cmd == "scale")
                {
                    validinput = readvals(s, 3, values);
                    if (validinput)
                    {

                        glm::mat4 scaleMat = glm::mat4(values[0], 0.0f, 0.0f, 0.0f,
                                                       0.0f, values[1], 0.0f, 0.0f,
                                                       0.0f, 0.0f, values[2], 0.0f,
                                                       0.0f, 0.0f, 0.0f, 1.0f);

                        glm::mat4 &T = transfstack.top();
                        T = T * scaleMat;
                    }
                }
                else if (cmd == "rotate")
                {
                    validinput = readvals(s, 4, values);
                    if (validinput)
                    {

                        glm::vec3 a(values[0], values[1], values[2]);
                        a = glm::normalize(a);

                        // now, setup first matrix in formula
                        float cosineTheta = glm::cos(glm::radians(values[3]));
                        float sinTheta = glm::sin(glm::radians(values[3]));
                        glm::mat3 firstMatrix(cosineTheta, 0.0f, 0.0f, 0.0f, cosineTheta, 0.0f, 0.0f, 0.0f, cosineTheta);
                        // now, setup second matrix in formula

                        glm::mat3 secondMatrix = (1.0f - cosineTheta) * glm::mat3(a[0] * a[0], a[0] * a[1], a[0] * a[2],
                                                                                  a[0] * a[1], a[1] * a[1], a[1] * a[2],
                                                                                  a[0] * a[2], a[1] * a[2], a[2] * a[2]);
                        // now, setup last matrix in formula
                        glm::mat3 thirdMatrix = sinTheta * glm::mat3(0.0f, a[2], -a[1],
                                                                     -a[2], 0.0f, a[0],
                                                                     a[1], -a[0], 0.0f);
                        glm::mat4 rotationMat = glm::mat4(firstMatrix + secondMatrix + thirdMatrix);

                        glm::mat4 &T = transfstack.top();
                        T = T * rotationMat;
                    }
                }

                // I include the basic push/pop code for matrix stacks
                else if (cmd == "pushTransform")
                {
                    transfstack.push(transfstack.top());
                }
                else if (cmd == "popTransform")
                {
                    if (transfstack.size() <= 1)
                    {
                        std::cerr << "Stack has no elements.  Cannot Pop\n";
                    }
                    else
                    {
                        transfstack.pop();
                    }
                }

                else
                {
                    std::cerr << "Unknown Command: " << cmd << " Skipping \n";
                }
            }
            getline(in, str);
        }
        std::cout << "DONE READING FILE\n";
        in.close();
    }
    delete vertices;
    delete vertexNormals;
    return;
}
