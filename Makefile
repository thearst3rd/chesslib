# Makefile for Chess implemenetation in C
# by thearst3rd

CC = gcc
CFLAGS = -g0 -Wall

SOURCES = $(wildcard src/*.c)
OBJECTS = $(patsubst %.c, %.o, $(SOURCES))
OBJECTS_NO_MAINS = $(filter-out src/main.o src/tests.o, $(OBJECTS))

HEADERS_NO_MAINS = $(filter-out src/tests.h, $(wildcard src/*.h))

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
	$(CC) $(CFLAGS) -o $@ -c $<


$(CHESS_LIB): $(OBJECTS_NO_MAINS) | bin
	ar -crs $(CHESS_LIB) $(OBJECTS_NO_MAINS)
	cp $(HEADERS_NO_MAINS) bin

$(CLI_CHESS_EXE): src/main.o $(OBJECTS_NO_MAINS) | bin
	$(CC) $(CFLAGS) -o $(CLI_CHESS_EXE) src/main.o $(OBJECTS_NO_MAINS)

$(TESTS_EXE): src/tests.o $(OBJECTS_NO_MAINS) | bin
	$(CC) $(CFLAGS) -o $(TESTS_EXE) src/tests.o $(OBJECTS_NO_MAINS)


bin:
	mkdir -p bin

clean:
	rm -rf **/*.o bin

test: $(TESTS_EXE)
	./$(TESTS_EXE)

run: $(CLI_CHESS_EXE)
	./$(CLI_CHESS_EXE)
