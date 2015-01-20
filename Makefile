CC=g++
CFLAGS=-c -Wall

all: globe

globe: main.o heightmap.o globemesh.o
	$(CC) main.o heightmap.o globemesh.o -o globe -framework GLUT -framework OpenGL

heightmap.o: heightmap.cpp
	$(CC) $(CFLAGS) heightmap.cpp

globemesh.o: globemesh.cpp
	$(CC) $(CFLAGS) globemesh.cpp

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

clean:
	rm -rf *.o globe
