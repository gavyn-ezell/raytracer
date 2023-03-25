raytrace: main.o readfile.o Camera.o Light.o Triangle.o Sphere.o Ray.o recursion.o BVHTree.o BBoxNode.o
	g++ -std=c++17 -w main.o readfile.o Camera.o Light.o Triangle.o Sphere.o Ray.o recursion.o BVHTree.o BBoxNode.o -o raytrace

main.o: main.cpp
	g++ -std=c++17 -w -c main.cpp

readfile.o: readfile.cpp
	g++ -std=c++17 -w -c readfile.cpp

Camera.o: Camera.cpp
	g++ -std=c++17 -w -c Camera.cpp

Light.o: Light.cpp
	g++ -std=c++17 -w -c Light.cpp

Triangle.o: Triangle.cpp
	g++ -std=c++17 -w -c Triangle.cpp

Sphere.o: Sphere.cpp
	g++ -std=c++17 -w -c Sphere.cpp

Ray.o: Ray.cpp
	g++ -std=c++17 -w -c Ray.cpp

recursion.o: recursion.cpp
	g++ -std=c++17 -w -c recursion.cpp

BBoxNode.o: BBoxNode.cpp
	g++ -std=c++17 -w -c BBoxNode.cpp

BVHTree.o: BVHTree.cpp
	g++ -std=c++17 -w -c BVHTree.cpp

clean:
	rm *.o output