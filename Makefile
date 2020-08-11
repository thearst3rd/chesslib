# Makefile for Chess implemenetation in C
# by thearst3rd

CC = gcc
CFLAGS = -g0 -Wall

SOURCES = $(wildcard src/*.c)
OBJECTS = $(patsubst %.c, %.o, $(SOURCES))
OBJECTS_NO_MAINS = $(filter-out src/main.o src/tests.o, $(OBJECTS))

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


$(OBJECTS): src/%.o : src/%.c
	$(CC) $(CFLAGS) -o $@ -c $<


$(TESTS_EXE): src/tests.o $(OBJECTS_NO_MAINS) | bin
	$(CC) $(CFLAGS) -o $(TESTS_EXE) src/tests.o $(OBJECTS_NO_MAINS)

$(CHESS_EXE): src/main.o $(OBJECTS_NO_MAINS) | bin
	$(CC) $(CFLAGS) -o $(CHESS_EXE) src/main.o $(OBJECTS_NO_MAINS)


bin:
	@mkdir -p bin

clean:
	rm -rf **/*.o bin

test: $(TESTS_EXE)
	./$(TESTS_EXE)

run: $(CHESS_EXE)
	./$(CHESS_EXE)
