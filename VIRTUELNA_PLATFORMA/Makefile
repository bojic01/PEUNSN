SYSTEMC_HOME=/usr/local/systemc

CC=g++
CFLAGS=-c -I$(SYSTEMC_HOME)/include

LDFLAGS=-L$(SYSTEMC_HOME)/lib-linux64 -lsystemc

SOURCES=main.cpp BRAM.cpp CPU.cpp HARD.cpp INTERCONNECT.cpp UTILS.cpp VP.cpp

OBJECTS=$(SOURCES:.cpp=.o)

EXECUTABLE=main

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@


clean:
	rm -f $(EXECUTABLE)

.PHONY: all clean
