CC=g++
CFLAGS=-c -g -std=c++0x
LDFLAGS=-pthread
SOURCES= test.cpp Square_Matrix.cpp Sodoku.cpp main.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=Sodoku

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

test:
	./$(EXECUTABLE)

clean:
	rm -f $(EXECUTABLE) $(OBJECTS)
