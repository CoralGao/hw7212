CC=g++
CFLAGS=-g
LDFLAGS=
SOURCES=parserInput.cpp parserInput.hpp 
EXECUTABLE=tileSolver

all: $(EXECUTABLE)

$(EXECUTABLE): bin/parserInput.o
		$(CC) $(LDFLAGS) -o tileSolver src/main.cpp bin/parserInput.o

bin/parserInput.o: src/parserInput.cpp
		$(CC) $(LDFLAGS) -c src/parserInput.cpp -o bin/parserInput.o
clean:
	$(RM)  bin/*.o *~