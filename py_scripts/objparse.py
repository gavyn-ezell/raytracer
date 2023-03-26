# imports

import sys
import os.path
import re

# grabbing file

file1 = open(sys.argv[1], 'r')

path = './result.txt'

exists = os.path.isfile(path)
if exists:
    os.remove(path)


def grabVertexAndNormals(fString):
    # form f 1//2 ... ...
    pattern2 = r'^[1-9]\d*//([1-9]\d*)+$'

    # form f 1/2/3 ... ...
    # pattern4 = r'^[1-9]\d*/([1-9]\d*)/([1-9]\d*)+$'

    if re.match(pattern2, fString):
        i = fString.find('/')
        return (int(fString[0:i]), int(fString[i+2:]))
    else:
        i = fString.find('/')
        j = fString.find('/', i + 1)
        return (int(fString[0:i]), int(fString[j+1:]))


file2 = open("result.txt", 'w')
lines = file1.readlines()
for line in lines:
    # currLine splits line from string into list of words
    currLine = line.split()
    if len(currLine) == 0:
        continue
    if currLine[0] == "#":
        continue
    elif currLine[0] == "v":
        if len(currLine) == 5:
            x = float(currLine[1]) / float(currLine[4])
            y = float(currLine[2]) / float(currLine[4])
            z = float(currLine[3]) / float(currLine[4])

            file2.write("v {} {} {}\n".format(x, y, z))
        else:
            file2.write(line)
    elif currLine[0] == "vn":
        file2.write(line)
    elif currLine[0] == "f":
        # TODO: make better for all forms of f, use Obj wikipedia

        # form f 1 2 3
        pattern1 = r'^[1-9]\d*$'
        pattern2 = r'^[1-9]\d*/[1-9]\d*$'

        if re.match(pattern1, currLine[1]):
            for i in range(2, len(currLine)-1):
                file2.write("tri {} {} {}\n".format(
                    int(currLine[1])-1, int(currLine[i])-1, int(currLine[i+1])-1))
        elif re.match(pattern2, currLine[1]):
            v1 = int(currLine[1][0:currLine[1].find('/')])-1
            for i in range(2, len(currLine)-1):
                v2 = int(currLine[i][0:currLine[i].find('/')])-1
                v3 = int(currLine[i+1][0:currLine[i+1].find('/')])-1
                file2.write("tri {} {} {}\n".format(v1, v2, v3))
        else:
            v1 = grabVertexAndNormals(currLine[1])
            for i in range(2, len(currLine)-1):
                v2 = grabVertexAndNormals(currLine[i])
                v3 = grabVertexAndNormals(currLine[i+1])
                file2.write("trinorm {} {} {} {} {} {}\n".format(
                    v1[0]-1, v2[0]-1, v3[0]-1, v1[1]-1, v2[1]-1, v3[1]-1))
    else:
        continue
        # if currLine[0] = "#"
        # currList = line.split()
        # if currList[]

file1.close()
file2.close()
