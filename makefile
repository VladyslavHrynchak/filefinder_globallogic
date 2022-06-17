bin/findfile: lib/file_finder.a main.cpp
	g++ main.cpp lib/file_finder.a -o bin/findfile
	
lib/file_finder.a: build/file_finder.o
	ar rvs lib/file_finder.a build/file_finder.o	

build/file_finder.o: file_finder.cpp file_finder.h
	g++ -c file_finder.cpp -o build/file_finder.o
