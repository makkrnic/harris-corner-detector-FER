PROJECT			= harris_detector

CC				  = g++
CFLAGS			= -c -g -Wall -std=c++0x
LDFLAGS			= 

SOURCES  		= harris.cpp harrisMain.cpp gaussian.cpp get_harris_points.cpp
OBJECTS 		= ${SOURCES:.cpp=.o}

EXECUTABLE	= harrisDetector


CFLAGS			+= `pkg-config --cflags opencv`
LDFLAGS			+= `pkg-config --libs opencv`


all: $(SOURCES) $(EXECUTABLE)

gauss: gaussian.h gaussian.cpp testGauss.cpp
	$(CC) $(CFLAGS) gaussian.cpp -o gaussian.o
	$(CC) $(CFLAGS) testGauss.cpp -o testGauss.o
	$(CC) gaussian.o testGauss.o -o gauss $(LDFLAGS)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	-rm $(OBJECTS) $(EXECUTABLE)
