CC=g++
CFLAGS=-c -Wall -DGL_GLEXT_PROTOTYPES -std=c++0x -pg -g
LDFLAGS=-lGLEW -lGL -lm -lGLU -lglut -std=c++0x -pg -g

all: app

clean:
	rm *.o

app: Shader.o main.o
	$(CC) main.o Shader.o $(LDFLAGS) -o hello

Shader.o: ./Shader/Shader.cpp
	$(CC) $(CFLAGS) ./Shader/Shader.cpp $(LDFLAGS)

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp $(LDFLAGS)

