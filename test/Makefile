CC = g++
CPPFLAGS = -Wall -std=c++11 -pedantic
LIBS = 
BIN = main

.PHONY: all clean

all: $(BIN)
	
main: main.cc
	$(CC) $(CPPFLAGS) -o $@ $< $(LIBS)

clean:
	$(RM) $(BIN) *.o
