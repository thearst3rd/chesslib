/*
 * Testing for Chess implementation in C
 * Created by thearst3rd on 8/6/2020
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tests.h"
#include "chesslib/pos.h"
#include "chesslib/movelist.h"

const char *currTest;

#define RUN_TEST(testName) currTest=#testName; testName();


// Runs tests. Will halt when one is failed
int main(int argc, char *argv[])
{
	// Test Pos
	RUN_TEST(testPosI);
	RUN_TEST(testPosS);
	RUN_TEST(testPosGetStr);

	// Test Move
	RUN_TEST(testMoveCreate);
	RUN_TEST(testMoveGetUci);
	RUN_TEST(testMoveFromUci);

	// Test Move List
	RUN_TEST(testMoveList);

	// We made it to the end
	printf("Success - all tests passed!\n");
	return 0;
}


//////////////////////
// HELPER FUNCTIONS //
//////////////////////

// Fail the currently running test and halt the program. NULL for no message
void failTest(const char *msg)
{
	if (msg == NULL)
		fprintf(stderr, "TEST FAILED - %s\n", currTest);
	else
		fprintf(stderr, "TEST FAILED - %s: %s\n", currTest, msg);

	exit(1);
}

void validateString(const char *actual, const char *expected)
{
	if (strcmp(actual, expected) != 0)
	{
		int len1 = strlen(actual);
		int len2 = strlen(expected);

		char *str = (char *) malloc(35 + len1 + len2);

		sprintf(str, "Actual \"%s\", but expected \"%s\"", actual, expected);
		failTest(str);

		// This doesn't actually get run because failTest exits, but it just feels like the right thing to do
		free(str);
	}
}


//////////////
// TEST POS //
//////////////

void testPosI()
{
	for (uint8_t rank = 1; rank <= 8; rank++)
	{
		for (uint8_t file = 1; file <= 8; file++)
		{
			pos p = posI(file, rank);

			if (p.file != file || p.rank != rank)
			{
				char msg[50];
				sprintf(msg, "Actual (%d, %d), but expected (%d, %d)", p.file, p.rank, file, rank);
				failTest(msg);
			}
		}
	}
}

void testPosS()
{
	for (uint8_t rank = 1; rank <= 8; rank++)
	{
		for (uint8_t file = 1; file <= 8; file++)
		{
			char str[3] = {'a' + file - 1, '0' + rank, 0};

			pos p = posS(str);

			if (p.file != file || p.rank != rank)
			{
				char msg[60];
				sprintf(msg, "Actual (%d, %d), but expected (%d, %d) from \"%s\"", p.file, p.rank, file, rank, str);
				failTest(msg);
			}
		}
	}
}

void testPosGetStr()
{
	for (uint8_t rank = 1; rank <= 8; rank++)
	{
		for (uint8_t file = 1; file <= 8; file++)
		{
			pos p = posI(file, rank);

			char str[3] = {'a' + file - 1, '0' + rank, 0};

			validateString(posGetStr(p), str);
		}
	}
}


///////////////
// TEST MOVE //
///////////////

void validateMove(move m, pos expectedFrom, pos expectedTo, pieceType expectedPromotion)
{
	if (m.from.file != expectedFrom.file || m.from.rank != expectedFrom.rank
			|| m.to.file != expectedTo.file || m.to.rank != expectedTo.rank
			|| m.promotion != expectedPromotion)
	{
		char msg[55];
		sprintf(msg, "Actual (%s, %s, %d), but expected (%s, %s, %d)",
				posGetStr(m.from), posGetStr(m.to), m.promotion,
				posGetStr(expectedFrom), posGetStr(expectedTo), expectedPromotion);
		failTest(msg);
	}
}

void testMoveCreate()
{
	// maybe TODO - make this test more thorough
	move m = movePos(posI(5, 8), posI(5, 7));
	validateMove(m, posI(5, 8), posI(5, 7), empty);

	m = movePromote(posI(3, 2), posI(2, 1), queen);
	validateMove(m, posI(3, 2), posI(2, 1), queen);
}

void testMoveGetUci()
{
	// maybe TODO - make this test more thorough
	move m = movePos(posI(5, 8), posI(5, 7));
	validateString(moveGetUci(m), "e8e7");

	m = movePromote(posI(3, 2), posI(2, 1), queen);
	validateString(moveGetUci(m), "c2b1q");
}

void testMoveFromUci()
{
	// maybe TODO - make this test more thorough
	move m = moveFromUci("e8e7");
	validateMove(m, posI(5, 8), posI(5, 7), empty);

	m = moveFromUci("c2b1q");
	validateMove(m, posI(3, 2), posI(2, 1), queen);
}


///////////////////
// TEST MOVELIST //
///////////////////

void testMoveList()
{
	move m1 = moveFromUci("e2e4");
	move m2 = moveFromUci("e7e5");
	move m3 = moveFromUci("e1e2");

	moveList *list = createMoveList();

	addToMoveList(list, m1);
	addToMoveList(list, m2);
	addToMoveList(list, m3);

	move m11 = getFromMoveList(list, 0);
	move m12 = getFromMoveList(list, 1);
	move m13 = getFromMoveList(list, 2);

	char *uci;
	char *expected;

	uci = moveGetUci(m11);
	expected = "e2e4";
	if (strcmp(uci, expected) != 0)
	{
		char failStr[30];
		sprintf(failStr, "Actual \"%s\" but, expected \"%s\"", uci, expected);
		failTest(failStr);
	}
	free(uci);

	uci = moveGetUci(m12);
	expected = "e7e5";
	if (strcmp(uci, expected) != 0)
	{
		char failStr[30];
		sprintf(failStr, "Actual \"%s\" but, expected \"%s\"", uci, expected);
		failTest(failStr);
	}
	free(uci);

	uci = moveGetUci(m13);
	expected = "e1e2";
	if (strcmp(uci, expected) != 0)
	{
		char failStr[30];
		sprintf(failStr, "Actual \"%s\" but, expected \"%s\"", uci, expected);
		failTest(failStr);
	}
	free(uci);

	freeMoveList(list);
}
