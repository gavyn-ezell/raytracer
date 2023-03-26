import os.path
import random

path = './simple.txt'

exists = os.path.isfile(path)
if exists:
    os.remove(path)

f = open("simple.txt", "x")
f.close()

file1 = open("simple.txt", "a")

file1.write("size 800 600\n")
file1.write("output simple.png\n")
file1.write("maxdepth 3\n")
file1.write("camera 0 0 75 0 0 0 0 1 0 60\n")
# file1.write("point 25 0 75 1 1 1\n")
file1.write("point 25 0 75 1 1 1\n")


file1.write("shininess 20\n\n")


c1 = [1, 0.298, 0.298]
c2 = [0.1, 0.745, 0.525]
c3 = [0.93, 0.95, 0.365]
c4 = [0.93, 0.75, 0.827]
c5 = [0.286, 0.211, 0.38]

colors = [c1, c2, c3, c4, c5]

sphereCnt = 0
for i in range(300):
    # get a random color
    colorIndex = i % 4
    r = colors[colorIndex][0]
    g = colors[colorIndex][1]
    b = colors[colorIndex][2]
    # line = "ambient {} {} {}".format(r * 0.25, g * 0.25, b * 0.25)
    if (random.randint(1, 10) < 11):
        line = "specular 0 0 0\n"
    else:
        line = "specular 0.1 0.1 0.1\n"
        r = r * 0.5
        g *= 0.5
        b *= 0.5
    file1.write(line)
    line = "diffuse {} {} {}\n".format(r*1.3, g*1.3, b*1.3)
    file1.write(line)

    x = random.randint(-40, 40)
    y = random.randint(-40, 40)
    z = random.randint(-50, 50)
    file1.write("sphere {} {} {} {}\n".format(x, y, z, random.randint(2, 6)))
file1.close()
