CC = g++ -O3 -c -Wall -std=c++11 -pg -g
LD = g++ -O3 -Wall -std=c++11
LDFLAGS = -pg -g

all: main
	$(LD) $(LDFLAGS) main.o Shader.o -o visibility_graph_fun

main.o: main.cpp
	$(CC) main.cpp

Shader.o: OglFramework/Shader.cpp
	$(CC) OglFramework/Shader.cpp

clean:
	rm *.o visibility_graph_fun
