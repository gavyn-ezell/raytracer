# Ray Tracer from Scratch
###### by Gavyn Ezell

Ray tracer built in C++.

Final project for CSE 167 Winter 2023, taught by Ravi Ramamoorthi.

Runs through Xcode.

## Description
When run, reads a input file (given as an argument when running) and creates a PPM file from the scene described 
within the input file. PPM can convert to PNG within OSX, simply change .ppm to .png. 

# Input File Format Notes (from assignment specification)

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
