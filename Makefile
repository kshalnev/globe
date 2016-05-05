CC=g++
CFLAGS=-c -Wall -std=c++11 -Ofast -m64

all: globe

globe: main.o heightmap.o globemesh.o
	$(CC) main.o heightmap.o globemesh.o -o globe -framework GLUT -framework OpenGL

heightmap.o: heightmap.cpp
	$(CC) $(CFLAGS) heightmap.cpp

globemesh.o: globemesh.cpp heightmap.h
	$(CC) $(CFLAGS) globemesh.cpp

main.o: main.cpp globemesh.h
	$(CC) $(CFLAGS) main.cpp

clean:
	rm -rf *.o globe
