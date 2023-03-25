# imports

import os.path
import re

# grabbing file
file1 = open('politoed.obj', 'r')

path = './result.txt'

exists = os.path.isfile(path)
if exists:
    os.remove(path)


def grabVertexIndex(fString):
    found = fString.find("/")
    if (found == -1):
        return int(fString)
    else:
        return int(fString[0:found])


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
        """
        x = float(currLine[1])
        y = float(currLine[2])
        z = float(currLine[3])
        file2.write("vn {} {} {}\n".format(x, y, z))
        """
        continue
    elif currLine[0] == "f":
        # TODO: make better for all forms of f, use Obj wikipedia
        v1 = grabVertexIndex(currLine[1])
        for i in range(2, len(currLine)-1):
            v2 = grabVertexIndex(currLine[i])
            v3 = grabVertexIndex(currLine[i+1])
            file2.write("tri {} {} {}\n".format(v1-1, v2-1, v3-1))
    else:
        continue
        # if currLine[0] = "#"
        # currList = line.split()
        # if currList[]

file1.close()
file2.close()
