CC=g++
CXXFLAGS=-g
LDFLAGS=
#SOURCES=parserInput.hpp parserInput.cpp tileSolver.hpp tileSolver.cpp
EXECUTABLE=project3

all: $(EXECUTABLE)

$(EXECUTABLE): bin/fifo.o bin/lru.o
		$(CC) $(CXXFLAGS) $(LDFLAGS) -o project3 src/project3.cpp bin/fifo.o 

bin/fifo.o: src/fifo.cpp
		$(CC) $(CXXFLAGS) $(LDFLAGS) -c src/fifo.cpp -o bin/fifo.o

bin/lru.o: src/lru.cpp
		$(CC) $(CXXFLAGS) $(LDFLAGS) -c src/lru.cpp -o bin/lru.o

clean:
	$(RM)  bin/*.o *~ project3