PROJECT			= harris_detector

CC				= g++
CFLAGS			= -c -Wall
LDFLAGS			= 

SOURCES  		= harris.cpp harrisMain.cpp gaussian.cpp testGauss.cpp
OBJECTS 		= ${SOURCES:.cpp=.o}

EXECUTABLE	= harrisDetector


CFLAGS			+= `pkg-config --cflags opencv`
LDFLAGS			+= `pkg-config --libs opencv`


gauss: gaussian.h gaussian.cpp testGauss.cpp
	$(CC) $(CFLAGS) gaussian.cpp -o gaussian.o
	$(CC) $(CFLAGS) testGauss.cpp -o testGauss.o
	$(CC) gaussian.o testGauss.o -o gauss $(LDFLAGS)

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	-rm $(OBJECTS) $(EXECUTABLE)
