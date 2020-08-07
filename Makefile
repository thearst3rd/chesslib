# Makefile for Chess implemenetation in C
# by thearst3rd

CC = gcc
CFLAGS = -g0 -Wall

ALLSOURCES = $(wildcard src/*.c)
SOURCES = $(filter-out src/main.c src/tests.c, $(ALLSOURCES))
ALLOBJECTS = $(patsubst %.c, %.o, $(ALLSOURCES))
OBJECTS = $(patsubst %.c, %.o, $(SOURCES))

# Platform independance
ifeq ($(OS), Windows_NT)
	CHESS_EXE = bin/chess.exe
	TESTS_EXE = bin/tests.exe
else
	CHESS_EXE = bin/chess
	TESTS_EXE = bin/tests
endif


all: $(CHESS_EXE)

chess: $(CHESS_EXE)
tests: $(TESTS_EXE)


$(ALLOBJECTS): src/%.o : src/%.c
	$(CC) $(CFLAGS) -o $@ -c $<


$(TESTS_EXE): build src/tests.o $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TESTS_EXE) src/tests.o $(OBJECTS)

$(CHESS_EXE): build src/main.o $(OBJECTS)
	$(CC) $(CFLAGS) -o $(CHESS_EXE) src/main.o $(OBJECTS)


build:
	@mkdir -p bin

clean:
	rm -rf **/*.o bin

test: $(TESTS_EXE)
	./$(TESTS_EXE)

run: $(CHESS_EXE)
	./$(CHESS_EXE)
