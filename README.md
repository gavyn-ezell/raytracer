# Ray Tracer from Scratch
###### by Gavyn Ezell

Ray tracer built in C++.

## Description
When run, reads a input file (given as an argument when running) and creates a PNG file of the scene described by the input file. 
Simply use the makefile to compile code, then in terminal type "./raytrace [some scene file]" to create your render. 


Follows simple Phong shading, additionally incorporates a [**Bounding Volume Hiererarchy**](https://en.wikipedia.org/wiki/Bounding_volume_hierarchy) to accelerate rendering times. 

The py_scripts directory, contains two python scripts.

**1. random_sphere_scene.py**

Generates a random scene file for testing/fun

**2. objparse.py**

Translates an obj file (used as argument when running py script in terminal) into readable format for the main raytracer. Result gained from objparse.py is not immediately readable by main raytracer (only converts triangle information), needs some manual additions for overall scene description at top of the resultant file. Use input file format notes below as a guideline. 

## Input File Format Notes

The input file consists of a sequence of lines, each of which has a command. For examples and clarifications, see the example input files. The lines have the following form:

**command parameter1 parameter2** ... The first part of the line is always the command. Based on what the command is, it has certain parameters which should be parsed appropriately.

### Commands
**size width height**: The size command must be the first command of the file, which controls the image size.

**maxdepth depth:** The maximum depth (number of bounces) for a ray (default should be 5).

**output filename:** The output file to which the image should be written. You can either require this to be specified in the input, or you can use a suitable default like stdout or raytrace.png

**sphere x y z radius:** Defines a sphere with a given position and radius. 

**maxverts number:** Defines a maximum number of vertices for later triangle specifications. It must be set before vertices are defined. (Your program may not need this; it is simply a convenience to allocate arrays accordingly. You can ignore this command [but still parse it] if you don't need it). 

**maxvertnorms:** number Defines a maximum number of vertices with normals for later specifications. It must be set before vertices with normals are defined. (same discussion as above) 

**vertex x y z:** Defines a vertex at the given location. The vertex is put into a pile, starting to be numbered at 0. 

**vertexnormal x y z nx ny nz:** Similar to the above, but define a surface normal with each vertex. The vertex and vertexnormal set of vertices are completely independent (as are maxverts and maxvertnorms). 

**tri v1 v2 v3:** Create a triangle out of the vertices involved (which have previously been specified with the vertex command). The vertices are assumed to be specified in counter-clockwise order. Your code should internally compute a face normal for this triangle. 

**trinormal v1 v2 v3:** Same as above but for vertices specified with normals. In this case, each vertex has an associated normal, and when doing shading, you should interpolate the normals for intermediate points on the triangle.

**translate x y z:** A translation 3-vector. 

**rotate x y z angle:** Rotate by angle (in degrees) about the given axis as in OpenGL. 

**scale x y z:** Scale by the corresponding amount in each axis (a non-uniform scaling). 

**pushTransform** Push the current modeling transform on the stack.

**popTransform** Pop the current transform from the stack as in OpenGL. The sequence of popTransform and pushTransform can be used if desired before every primitive to reset the transformation (assuming the initial camera transformation is on the stack as discussed above).

**directional x y z r g b** The direction to the light source, and the color, as in OpenGL. 

**point x y z r g b** The location of a point source and the color, as in OpenGL. 

**attenuation const linear quadratic** Sets the constant, linear and quadratic attenuations (default 1,0,0) as in OpenGL. By default there is no attenuation (the constant term is 1, linear and quadratic are 0; that's what we mean by 1,0,0). 

**ambient r g b** The global ambient color to be added for each object (default is .2,.2,.2).

**diffuse r g b** specifies the diffuse color of the surface.
**specular r g b** specifies the specular color of the surface.
**shininess s** specifies the shininess of the surface.
**emission r g b** gives the emissive color of the surface.