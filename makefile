# Makefile for Chess implemenetation in C
# by thearst3rd

# Platform independance
ifeq ($(OS),Windows_NT)
	CHESS_EXE = chess.exe
	TESTS_EXE = tests.exe
else
	CHESS_EXE = chess
	TESTS_EXE = tests
endif


all: $(CHESS_EXE) $(TESTS_EXE)


main.o: main.c
	gcc -c main.c

pos.o: pos.c pos.h
	gcc -c pos.c

piece.o: piece.c piece.h
	gcc -c piece.c

move.o: move.c move.h
	gcc -c move.c

tests.o: tests.c tests.h
	gcc -c tests.c


$(TESTS_EXE): tests.o pos.o piece.o move.o
	gcc -o $(TESTS_EXE) tests.o pos.o piece.o move.o

$(CHESS_EXE): main.o pos.o piece.o move.o
	gcc -o $(CHESS_EXE) main.o pos.o piece.o move.o


clean:
	rm -rf *.o $(CHESS_EXE) $(TESTS_EXE)

test: $(TESTS_EXE)
	./$(TESTS_EXE)

run: $(CHESS_EXE)
	./$(CHESS_EXE)
