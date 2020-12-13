# Makefile for Chess implemenetation in C
# by thearst3rd

CC = gcc
CFLAGS = -Wall

ifeq ($(DEBUG),1)
	CFLAGS += -g
else
	CFLAGS += -g0
endif

SOURCES = $(wildcard src/chesslib/*.c) $(wildcard src/*.c)
OBJECTS = $(patsubst %.c,%.o,$(SOURCES))
OBJECTS_NO_MAINS = $(filter-out src/tests.o,$(OBJECTS))

# Platform independance
ifeq ($(OS),Windows_NT)
	TESTS_EXE = bin/tests.exe
	CHESSLIB = bin/libchesslib.a
else
	TESTS_EXE = bin/tests
	CHESSLIB = bin/libchesslib.a
endif


all: $(CHESSLIB)

chesslib: $(CHESSLIB)
tests: $(TESTS_EXE)


$(OBJECTS): %.o : %.c
	$(CC) $(CFLAGS) -o $@ -Iinclude -c $<


$(CHESSLIB): $(OBJECTS_NO_MAINS) | bin
	ar rcs $(CHESSLIB) $(OBJECTS_NO_MAINS)

$(TESTS_EXE): src/tests.o $(CHESSLIB) | bin
	$(CC) $(CFLAGS) -o $(TESTS_EXE) -Iinclude src/tests.o -Lbin -lchesslib


bin:
	mkdir -p bin

clean:
	rm -rf src/*.o src/chesslib/*.o bin

test: $(TESTS_EXE)
	./$(TESTS_EXE)
