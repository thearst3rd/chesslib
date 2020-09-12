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
#include "chesslib/board.h"

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

	// Test Board
	RUN_TEST(testBoardCreate);
	RUN_TEST(testBoardCreateFromFen);

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


////////////////
// TEST BOARD //
////////////////

void assertPiece(piece actual, piece expected)
{
	if (actual != expected)
	{
		char actualL = getPieceLetter(actual);
		char expectedL = getPieceLetter(expected);
		char message[45];
		sprintf(message, "Actual piece '%c' found, expected '%c'", actualL, expectedL);
		failTest(message);
	}
}

void testBoardCreate()
{
	board b = createBoard();

	return;

	assertPiece(b.pieces[0], pWRook);
	assertPiece(b.pieces[1], pWKnight);
	assertPiece(b.pieces[2], pWBishop);
	assertPiece(b.pieces[3], pWQueen);
	assertPiece(b.pieces[4], pWKing);
	assertPiece(b.pieces[5], pWBishop);
	assertPiece(b.pieces[6], pWKnight);
	assertPiece(b.pieces[7], pWRook);

	for (int i = 8; i < 16; i++)
		assertPiece(b.pieces[i], pWPawn);

	for (int i = 16; i < 48; i++)
		assertPiece(b.pieces[i], pEmpty);

	for (int i = 48; i < 56; i++)
		assertPiece(b.pieces[i], pBPawn);

	assertPiece(b.pieces[56], pWRook);
	assertPiece(b.pieces[57], pWKnight);
	assertPiece(b.pieces[58], pWBishop);
	assertPiece(b.pieces[59], pWQueen);
	assertPiece(b.pieces[60], pWKing);
	assertPiece(b.pieces[61], pWBishop);
	assertPiece(b.pieces[62], pWKnight);
	assertPiece(b.pieces[63], pWRook);

	if (b.blackToPlay)
		failTest("Actual: black to play, expected: white to play");

	if (b.castleState != 0b1111)
		failTest("Not all castling flags enabled");

	if (b.enPassantTarget.file != -1 || b.enPassantTarget.rank != -1)
		failTest("EP target square was not POS_INVALID");

	if (b.halfMoveClock != 0)
	{
		char message[50];
		sprintf(message, "Half move clock was %du, expected 0", b.halfMoveClock);
	}

	if (b.moveNumber != 1)
	{
		char message[50];
		sprintf(message, "Full move number was %du, expected 1", b.moveNumber);
	}
}

void testBoardCreateFromFen()
{
	board b = createBoardFromFen("8/8/3k4/8/4Pp2/2K5/8/5QQQ b - e3 0 46");

	for (int i = 0; i < 64; i++)
	{
		// Special case out the pieces that are actually there
		if ((i == 5) || (i == 6) || (i == 7) || (i == 18) || (i == 28) || (i == 29) || (i == 43))
			continue;

		assertPiece(b.pieces[i], pEmpty);
	}

	assertPiece(b.pieces[5], pWQueen);
	assertPiece(b.pieces[6], pWQueen);
	assertPiece(b.pieces[7], pWQueen);
	assertPiece(b.pieces[28], pWPawn);
	assertPiece(b.pieces[29], pBPawn);
	assertPiece(b.pieces[18], pWKing);
	assertPiece(b.pieces[43], pBKing);

	if (!b.blackToPlay)
		failTest("Actual: white to play, expected: black to play");

	if (b.castleState != 0b0000)
		failTest("Not all castling flags disabled");

	if (b.enPassantTarget.file != 5 || b.enPassantTarget.rank != 3)
	{
		char message[50];
		sprintf(message, "Actual EP target square: %s, expected: e3", posGetStr(b.enPassantTarget));
	}

	if (b.halfMoveClock != 0)
	{
		char message[50];
		sprintf(message, "Half move clock was %du, expected 0", b.halfMoveClock);
	}

	if (b.moveNumber != 46)
	{
		char message[50];
		sprintf(message, "Full move number was %du, expected 46", b.moveNumber);
	}
}
