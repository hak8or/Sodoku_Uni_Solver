CC=g++
CFLAGS=-c -g
LDFLAGS=
SOURCES= test.cpp Square_Matrix.cpp Sodoku.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=Square_Matrix

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

test:
	./$(EXECUTABLE)

clean:
	rm -f $(EXECUTABLE) $(OBJECTS)
