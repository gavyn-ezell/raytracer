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
maxdepth depth: The maximum depth (number of bounces) for a ray (default should be 5).
output filename: The output file to which the image should be written. You can either require this to be specified in the input, or you can use a suitable default like stdout or raytrace.png (The final files should be .png, with the specific names the autograder requests).


