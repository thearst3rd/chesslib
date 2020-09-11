# Makefile for Chess implemenetation in C
# by thearst3rd

CC = gcc
CFLAGS = -g0 -Wall

SOURCES = $(wildcard src/chesslib/*.c) $(wildcard src/*.c)
OBJECTS = $(patsubst %.c,%.o,$(SOURCES))
OBJECTS_NO_MAINS = $(filter-out src/cli-chess-main.o src/tests-main.o,$(OBJECTS))

HEADERS_NO_MAINS = $(wildcard src/chesslib/*.h)

# Platform independance
ifeq ($(OS), Windows_NT)
	CLI_CHESS_EXE = bin/cli-chess.exe
	TESTS_EXE = bin/tests.exe
	# TODO - is this next line right? Should we use .dll?
	CHESS_LIB = bin/libchesslib.a
else
	CLI_CHESS_EXE = bin/cli-chess
	TESTS_EXE = bin/tests
	CHESS_LIB = bin/libchesslib.a
endif


all: chesslib

chesslib: $(CHESS_LIB)
cli-chess: $(CLI_CHESS_EXE)
tests: $(TESTS_EXE)


$(OBJECTS): src/%.o : src/%.c
	$(CC) $(CFLAGS) -o $@ -Isrc -c $<


$(CHESS_LIB): $(OBJECTS_NO_MAINS) | bin
	ar rcs $(CHESS_LIB) $(OBJECTS_NO_MAINS)
	mkdir -p bin/chesslib
	cp $(HEADERS_NO_MAINS) bin/chesslib

$(CLI_CHESS_EXE): src/cli-chess.o chesslib | bin
	$(CC) $(CFLAGS) -o $(CLI_CHESS_EXE) -Isrc src/cli-chess.o -Lbin -lchesslib

$(TESTS_EXE): src/tests.o chesslib | bin
	$(CC) $(CFLAGS) -o $(TESTS_EXE) -Isrc src/tests.o -Lbin -lchesslib


bin:
	mkdir -p bin

clean:
	rm -rf src/*.o src/chesslib/*.o bin

test: $(TESTS_EXE)
	./$(TESTS_EXE)

run: $(CLI_CHESS_EXE)
	./$(CLI_CHESS_EXE)
