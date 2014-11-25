CC=g++
CXXFLAGS=-g -std=c++11
LDFLAGS=
#SOURCES=parserInput.hpp parserInput.cpp tileSolver.hpp tileSolver.cpp
EXECUTABLE=tileSolver

all: $(EXECUTABLE)

$(EXECUTABLE): bin/parserInput.o bin/tileToSetCover.o bin/dancinglink.o 
		$(CC) $(CXXFLAGS) $(LDFLAGS) -o tileSolver src/main.cpp bin/parserInput.o bin/tileToSetCover.o bin/dancinglink.o

bin/parserInput.o: src/parserInput.cpp
		$(CC) $(CXXFLAGS) $(LDFLAGS) -c src/parserInput.cpp -o bin/parserInput.o

bin/tileToSetCover.o: src/tileToSetCover.cpp
		$(CC) $(CXXFLAGS) $(LDFLAGS) -c src/tileToSetCover.cpp -o bin/tileToSetCover.o

bin/dancinglink.o: src/dancinglink.cpp
		$(CC) $(CXXFLAGS) $(LDFLAGS) -c src/dancinglink.cpp -o bin/dancinglink.o

clean:
	$(RM)  bin/*.o tileSolver