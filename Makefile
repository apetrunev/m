CC := g++
CFLAGS += -Wall -std=c++11 -pedantic 
CPPFLAGS =
HEADERS = Token.h TokenType.h Routine.h Lexer.h
SRC = $(wildcard *.cc)
OBJ = $(patsubst %.cc, %.o, $(SRC))
BIN = rmumps

.PHONY: all clean

all: $(BIN)

TokenType.h: TokenType.def

rmumps: $(OBJ) $(HEADERS)
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LIBS)

clean:
	$(RM) *.o $(BIN)
