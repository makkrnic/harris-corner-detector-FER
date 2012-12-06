PROJECT			= harris_detector

CC					= g++
CFLAGS			= -c -Wall
LDFLAGS			= 

SOURCES  		= harris.cpp harrisMain.cpp
OBJECTS 		= ${SOURCES:.cpp=.o}

EXECUTABLE	= harrisDetector


CFLAGS			+= `pkg-config --cflags opencv`
LDFLAGS			+= `pkg-config --libs opencv`


all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	-rm $(OBJECTS) $(EXECUTABLE)
