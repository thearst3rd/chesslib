/*
 * Testing for Chess implementation in C
 * Created by thearst3rd on 8/6/2020
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tests.h"
#include "chesslib/squareset.h"
#include "chesslib/movelist.h"
#include "chesslib/board.h"
#include "chesslib/piecemoves.h"
#include "chesslib/boardlist.h"

const char *currTest;

#define RUN_TEST(testName) currTest=#testName; testName();


// Runs tests. Will halt when one is failed
int main(int argc, char *argv[])
{
	// Test Square
	RUN_TEST(testSqI);
	RUN_TEST(testSqS);
	RUN_TEST(testSqGetStr);
	RUN_TEST(testSqIsDark);

	// Test Move
	RUN_TEST(testMoveCreate);
	RUN_TEST(testMoveGetUci);
	RUN_TEST(testMoveFromUci);

	// Test Move List
	RUN_TEST(testMoveList);

	// Test Board
	RUN_TEST(testBoardCreate);
	RUN_TEST(testBoardCreateFromFen);
	//RUN_TEST(testBoardEq);

	// Test Piece Moves
	RUN_TEST(testPawnMoves);
	RUN_TEST(testKnightMoves);
	RUN_TEST(testBishopMoves);
	RUN_TEST(testRookMoves);
	RUN_TEST(testQueenMoves);
	RUN_TEST(testKingMoves);

	// Test Attacked Squares
	RUN_TEST(testIsSquareAttacked);
	RUN_TEST(testIsInCheck);

	// Test playing moves
	RUN_TEST(testBoardPlayMove);

	// Test board move generation
	RUN_TEST(testBoardGenerateMoves);
	RUN_TEST(testBoardGenerateMovesCastling);

	// Test FEN generation
	RUN_TEST(testBoardGetFen);

	// Test draw by insufficient material
	RUN_TEST(testBoardIsInsufficientMaterial);

	// Test board list
	RUN_TEST(testBoardList);

	// Test square set
	RUN_TEST(testSqSetSet);
	RUN_TEST(testSqSetGet);

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


/////////////////
// TEST SQUARE //
/////////////////

void testSqI()
{
	for (uint8_t rank = 1; rank <= 8; rank++)
	{
		for (uint8_t file = 1; file <= 8; file++)
		{
			sq s = sqI(file, rank);

			if (s.file != file || s.rank != rank)
			{
				char msg[50];
				sprintf(msg, "Actual (%d, %d), but expected (%d, %d)", s.file, s.rank, file, rank);
				failTest(msg);
			}
		}
	}
}

void testSqS()
{
	for (uint8_t rank = 1; rank <= 8; rank++)
	{
		for (uint8_t file = 1; file <= 8; file++)
		{
			char str[3] = {'a' + file - 1, '0' + rank, 0};

			sq s = sqS(str);

			if (s.file != file || s.rank != rank)
			{
				char msg[60];
				sprintf(msg, "Actual (%d, %d), but expected (%d, %d) from \"%s\"", s.file, s.rank, file, rank, str);
				failTest(msg);
			}
		}
	}
}

void testSqGetStr()
{
	for (uint8_t rank = 1; rank <= 8; rank++)
	{
		for (uint8_t file = 1; file <= 8; file++)
		{
			sq s = sqI(file, rank);

			char str[3] = {'a' + file - 1, '0' + rank, 0};

			validateString(sqGetStr(s), str);
		}
	}
}

void testSqIsDark()
{
	uint8_t expectedIsDark = 1; 	// a1 is expected to be dark
	for (uint8_t rank = 1; rank <= 8; rank++)
	{
		for (uint8_t file = 1; file <= 8; file++)
		{
			sq s = sqI(file, rank);

			uint8_t actualIsDark = sqIsDark(s);

			if (actualIsDark != expectedIsDark)
			{
				char message[50];
				sprintf(message,
						"Square %s was %s, expected %s",
						sqGetStr(s),
						actualIsDark ? "dark" : "light",
						expectedIsDark ? "dark" : "light");
				failTest(message);
			}

			expectedIsDark = !expectedIsDark;
		}
		expectedIsDark = !expectedIsDark;
	}
}


///////////////
// TEST MOVE //
///////////////

void validateMove(move m, sq expectedFrom, sq expectedTo, pieceType expectedPromotion)
{
	if (!sqEq(m.from, expectedFrom) || !sqEq(m.to, expectedTo)
			|| m.promotion != expectedPromotion)
	{
		char msg[55];
		sprintf(msg, "Actual (%s, %s, %d), but expected (%s, %s, %d)",
				sqGetStr(m.from), sqGetStr(m.to), m.promotion,
				sqGetStr(expectedFrom), sqGetStr(expectedTo), expectedPromotion);
		failTest(msg);
	}
}

void testMoveCreate()
{
	// maybe TODO - make this test more thorough
	move m = moveSq(sqI(5, 8), sqI(5, 7));
	validateMove(m, sqI(5, 8), sqI(5, 7), ptEmpty);

	m = movePromote(sqI(3, 2), sqI(2, 1), ptQueen);
	validateMove(m, sqI(3, 2), sqI(2, 1), ptQueen);
}

void testMoveGetUci()
{
	// maybe TODO - make this test more thorough
	move m = moveSq(sqI(5, 8), sqI(5, 7));
	validateString(moveGetUci(m), "e8e7");

	m = movePromote(sqI(3, 2), sqI(2, 1), ptQueen);
	validateString(moveGetUci(m), "c2b1q");
}

void testMoveFromUci()
{
	// maybe TODO - make this test more thorough
	move m = moveFromUci("e8e7");
	validateMove(m, sqI(5, 8), sqI(5, 7), ptEmpty);

	m = moveFromUci("c2b1q");
	validateMove(m, sqI(3, 2), sqI(2, 1), ptQueen);
}


///////////////////
// TEST MOVELIST //
///////////////////

void testMoveList()
{
	move m1 = moveFromUci("e2e4");
	move m2 = moveFromUci("e7e5");
	move m3 = moveFromUci("e1e2");

	moveList *list = moveListCreate();

	moveListAdd(list, m1);
	moveListAdd(list, m2);
	moveListAdd(list, m3);

	move m11 = moveListGet(list, 0);
	move m12 = moveListGet(list, 1);
	move m13 = moveListGet(list, 2);

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

	moveListFree(list);
}


////////////////
// TEST BOARD //
////////////////

void assertPiece(piece actual, piece expected)
{
	if (actual != expected)
	{
		char actualL = pieceGetLetter(actual);
		char expectedL = pieceGetLetter(expected);
		char message[45];
		sprintf(message, "Actual piece '%c' found, expected '%c'", actualL, expectedL);
		failTest(message);
	}
}

void testBoardCreate()
{
	board *b = boardCreate();

	if (b == NULL)
		failTest("Created board was NULL");

	assertPiece(b->pieces[0], pWRook);
	assertPiece(b->pieces[1], pWKnight);
	assertPiece(b->pieces[2], pWBishop);
	assertPiece(b->pieces[3], pWQueen);
	assertPiece(b->pieces[4], pWKing);
	assertPiece(b->pieces[5], pWBishop);
	assertPiece(b->pieces[6], pWKnight);
	assertPiece(b->pieces[7], pWRook);

	for (int i = 8; i < 16; i++)
		assertPiece(b->pieces[i], pWPawn);

	for (int i = 16; i < 48; i++)
		assertPiece(b->pieces[i], pEmpty);

	for (int i = 48; i < 56; i++)
		assertPiece(b->pieces[i], pBPawn);

	assertPiece(b->pieces[56], pBRook);
	assertPiece(b->pieces[57], pBKnight);
	assertPiece(b->pieces[58], pBBishop);
	assertPiece(b->pieces[59], pBQueen);
	assertPiece(b->pieces[60], pBKing);
	assertPiece(b->pieces[61], pBBishop);
	assertPiece(b->pieces[62], pBKnight);
	assertPiece(b->pieces[63], pBRook);

	if (b->currentPlayer != pcWhite)
		failTest("Actual: black to play, expected: white to play");

	if (b->castleState != 0b1111)
		failTest("Not all castling flags enabled");

	if (!sqEq(b->epTarget, SQ_INVALID))
		failTest("EP target square was not SQ_INVALID");

	if (b->halfMoveClock != 0)
	{
		char message[50];
		sprintf(message, "Half move clock was %u, expected 0", b->halfMoveClock);
		failTest(message);
	}

	if (b->moveNumber != 1)
	{
		char message[50];
		sprintf(message, "Full move number was %u, expected 1", b->moveNumber);
		failTest(message);
	}

	free(b);
}

void testBoardCreateFromFen()
{
	board *b = boardCreateFromFen("8/8/3k4/8/4Pp2/2K5/8/5QQQ b - e3 0 46");

	if (b == NULL)
		failTest("Created board was NULL");

	for (int i = 0; i < 64; i++)
	{
		// Special case out the pieces that are actually there
		if ((i == 5) || (i == 6) || (i == 7) || (i == 18) || (i == 28) || (i == 29) || (i == 43))
			continue;

		assertPiece(b->pieces[i], pEmpty);
	}

	assertPiece(b->pieces[5], pWQueen);
	assertPiece(b->pieces[6], pWQueen);
	assertPiece(b->pieces[7], pWQueen);
	assertPiece(b->pieces[28], pWPawn);
	assertPiece(b->pieces[29], pBPawn);
	assertPiece(b->pieces[18], pWKing);
	assertPiece(b->pieces[43], pBKing);

	if (b->currentPlayer != pcBlack)
		failTest("Actual: white to play, expected: black to play");

	if (b->castleState != 0b0000)
		failTest("Not all castling flags disabled");

	if (!sqEq(b->epTarget, sqI(5, 3)))
	{
		char message[50];
		sprintf(message, "Actual EP target square: %s, expected: e3", sqGetStr(b->epTarget));
		failTest(message);
	}

	if (b->halfMoveClock != 0)
	{
		char message[50];
		sprintf(message, "Half move clock was %u, expected 0", b->halfMoveClock);
		failTest(message);
	}

	if (b->moveNumber != 46)
	{
		char message[50];
		sprintf(message, "Full move number was %u, expected 46", b->moveNumber);
		failTest(message);
	}

	free(b);
}

void testBoardEq()
{
	failTest("Not yet implemented");
	// TODO
	//board *b1 = boardCreateFromFen("")
}


/////////////////////
// TEST PIECEMOVES //
/////////////////////

// Helper function - checks if given UCI string exists in moveList
void validateUciIsInMovelist(moveList *list, char *expectedUci)
{
	for (moveListNode *n = list->head; n; n = n->next)
	{
		char *actualUci = moveGetUci(n->move);
		int cmp = strcmp(actualUci, expectedUci);
		free(actualUci);
		if (cmp == 0)
			return;
	}
	char message[60];
	sprintf(message, "Expected %s to be in move list but was absent", expectedUci);
	failTest(message);
}

// Helper function - validates the size of the list
void validateListSize(moveList *list, size_t expectedSize)
{
	if (list->size != expectedSize)
	{
		char message[70];
		sprintf(message, "Actual list was %zu element(s), expected %zu", list->size, expectedSize);
		failTest(message);
	}
}

void testPawnMoves()
{
	board *b;
	moveList *list;

	// Lone white pawn on starting rank
	b = boardCreateFromFen("8/8/8/8/8/8/4P3/8 w - - 0 1");

	list = pmGetPawnMoves(b, sqS("e2"));

	validateListSize(list, 2);
	validateUciIsInMovelist(list, "e2e3");
	validateUciIsInMovelist(list, "e2e4");

	moveListFree(list);

	// Pawn with captures
	boardInitFromFenInPlace(b, "8/8/8/8/8/3q1n2/4P3/8 w - - 0 1");

	list = pmGetPawnMoves(b, sqS("e2"));

	validateListSize(list, 4);
	validateUciIsInMovelist(list, "e2e3");
	validateUciIsInMovelist(list, "e2e4");
	validateUciIsInMovelist(list, "e2d3");
	validateUciIsInMovelist(list, "e2f3");

	moveListFree(list);

	// Pawn blocked
	boardInitFromFenInPlace(b, "8/8/8/8/8/4N3/4P3/8 w - - 0 1");

	list = pmGetPawnMoves(b, sqS("e2"));

	validateListSize(list, 0);

	moveListFree(list);

	// Pawn not on first rank
	boardInitFromFenInPlace(b, "8/8/8/8/4P3/8/8/8 w - - 0 1");

	list = pmGetPawnMoves(b, sqS("e4"));

	validateListSize(list, 1);
	validateUciIsInMovelist(list, "e4e5");

	moveListFree(list);

	// White pawn about to promote
	boardInitFromFenInPlace(b, "8/6P1/8/8/8/8/8/8 w - - 0 1");

	list = pmGetPawnMoves(b, sqS("g7"));

	validateListSize(list, 4);
	validateUciIsInMovelist(list, "g7g8q");
	validateUciIsInMovelist(list, "g7g8r");
	validateUciIsInMovelist(list, "g7g8b");
	validateUciIsInMovelist(list, "g7g8n");

	moveListFree(list);

	// Black pawn on first rank
	boardInitFromFenInPlace(b, "8/6p1/8/8/8/8/8/8 b - - 0 1");

	list = pmGetPawnMoves(b, sqS("g7"));

	validateListSize(list, 2);
	validateUciIsInMovelist(list, "g7g6");
	validateUciIsInMovelist(list, "g7g5");

	moveListFree(list);

	// Pawn blocked with one free spot, opp color
	boardInitFromFenInPlace(b, "8/6p1/8/6P1/8/8/8/8 b - - 0 1");

	list = pmGetPawnMoves(b, sqS("g7"));

	validateListSize(list, 1);
	validateUciIsInMovelist(list, "g7g6");

	moveListFree(list);

	// Black pawn about to promote, with capturing ability
	boardInitFromFenInPlace(b, "8/8/8/8/8/8/p7/1Q6 b - - 0 1");

	list = pmGetPawnMoves(b, sqS("a2"));

	validateListSize(list, 8);
	validateUciIsInMovelist(list, "a2a1q");
	validateUciIsInMovelist(list, "a2a1r");
	validateUciIsInMovelist(list, "a2a1b");
	validateUciIsInMovelist(list, "a2a1n");
	validateUciIsInMovelist(list, "a2b1q");
	validateUciIsInMovelist(list, "a2b1r");
	validateUciIsInMovelist(list, "a2b1b");
	validateUciIsInMovelist(list, "a2b1n");

	moveListFree(list);

	// White with opportunity to capture en passant
	boardInitFromFenInPlace(b, "8/8/8/4Pp2/8/8/8/8 w - f6 0 1");

	//b->currentPlayer = pcWhite;
	//b->epTarget = sqS("f6");

	list = pmGetPawnMoves(b, sqS("e5"));

	validateListSize(list, 2);
	validateUciIsInMovelist(list, "e5e6");
	validateUciIsInMovelist(list, "e5f6");

	moveListFree(list);

	// Black with opportunity to capture en passant
	boardInitFromFenInPlace(b, "8/8/8/8/6Pp/8/8/8 b - g3 0 1");

	//b->currentPlayer = pcBlack;
	//b->epTarget = sqS("g3");

	list = pmGetPawnMoves(b, sqS("h4"));

	validateListSize(list, 2);
	validateUciIsInMovelist(list, "h4h3");
	validateUciIsInMovelist(list, "h4g3");

	moveListFree(list);
	free(b);
}

void testKnightMoves()
{
	board b;
	moveList *list;

	// Lone knight
	boardInitFromFenInPlace(&b, "8/8/8/8/8/2N5/8/8 w - - 0 1");

	list = pmGetKnightMoves(&b, sqS("c3"));

	validateListSize(list, 8);
	validateUciIsInMovelist(list, "c3d5");
	validateUciIsInMovelist(list, "c3e4");
	validateUciIsInMovelist(list, "c3e2");
	validateUciIsInMovelist(list, "c3d1");
	validateUciIsInMovelist(list, "c3b1");
	validateUciIsInMovelist(list, "c3a2");
	validateUciIsInMovelist(list, "c3a4");
	validateUciIsInMovelist(list, "c3b5");

	moveListFree(list);

	// Knight near the edge
	boardInitFromFenInPlace(&b, "8/7n/8/8/8/8/8/8 b - - 0 1");

	list = pmGetKnightMoves(&b, sqS("h7"));

	validateListSize(list, 3);
	validateUciIsInMovelist(list, "h7g5");
	validateUciIsInMovelist(list, "h7f6");
	validateUciIsInMovelist(list, "h7f8");

	moveListFree(list);

	// Knight with some blocks
	boardInitFromFenInPlace(&b, "6N1/3BQQQn/4QnQ1/4QQQ1/4b3/8/8/8 b - - 0 1");

	list = pmGetKnightMoves(&b, sqS("f6"));

	validateListSize(list, 6);
	validateUciIsInMovelist(list, "f6g8");
	validateUciIsInMovelist(list, "f6h5");
	validateUciIsInMovelist(list, "f6g4");
	validateUciIsInMovelist(list, "f6d5");
	validateUciIsInMovelist(list, "f6d7");
	validateUciIsInMovelist(list, "f6e8");

	moveListFree(list);
}

void testBishopMoves()
{
	board b;
	moveList *list;

	// Lone bishop
	boardInitFromFenInPlace(&b, "8/4b3/8/8/8/8/8/8 b - - 0 1");

	list = pmGetBishopMoves(&b, sqS("e7"));

	validateListSize(list, 9);
	validateUciIsInMovelist(list, "e7f8");
	validateUciIsInMovelist(list, "e7f6");
	validateUciIsInMovelist(list, "e7g5");
	validateUciIsInMovelist(list, "e7h4");
	validateUciIsInMovelist(list, "e7d6");
	validateUciIsInMovelist(list, "e7c5");
	validateUciIsInMovelist(list, "e7b4");
	validateUciIsInMovelist(list, "e7a3");
	validateUciIsInMovelist(list, "e7d8");

	moveListFree(list);

	// Position with some blocks captures
	boardInitFromFenInPlace(&b, "8/8/5n2/8/1r6/2B5/3Q4/N7 w - - 0 1");

	list = pmGetBishopMoves(&b, sqS("c3"));

	validateListSize(list, 5);
	validateUciIsInMovelist(list, "c3d4");
	validateUciIsInMovelist(list, "c3e5");
	validateUciIsInMovelist(list, "c3f6");
	validateUciIsInMovelist(list, "c3b2");
	validateUciIsInMovelist(list, "c3b4");

	moveListFree(list);
}

void testRookMoves()
{
	board b;
	moveList *list;

	// Lone rook
	boardInitFromFenInPlace(&b, "8/3R4/8/8/8/8/8/8 w - - 0 1");

	list = pmGetRookMoves(&b, sqS("d7"));

	validateListSize(list, 14);
	validateUciIsInMovelist(list, "d7d8");
	validateUciIsInMovelist(list, "d7e7");
	validateUciIsInMovelist(list, "d7f7");
	validateUciIsInMovelist(list, "d7g7");
	validateUciIsInMovelist(list, "d7h7");
	validateUciIsInMovelist(list, "d7d6");
	validateUciIsInMovelist(list, "d7d5");
	validateUciIsInMovelist(list, "d7d4");
	validateUciIsInMovelist(list, "d7d3");
	validateUciIsInMovelist(list, "d7d2");
	validateUciIsInMovelist(list, "d7d1");
	validateUciIsInMovelist(list, "d7c7");
	validateUciIsInMovelist(list, "d7b7");
	validateUciIsInMovelist(list, "d7a7");

	moveListFree(list);

	// Rook with some stuff around
	boardInitFromFenInPlace(&b, "8/3R4/8/2k1r1BQ/8/8/3n1R2/4b3 b - - 0 1");

	list = pmGetRookMoves(&b, sqS("e5"));

	validateListSize(list, 9);
	validateUciIsInMovelist(list, "e5e6");
	validateUciIsInMovelist(list, "e5e7");
	validateUciIsInMovelist(list, "e5e8");
	validateUciIsInMovelist(list, "e5f5");
	validateUciIsInMovelist(list, "e5g5");
	validateUciIsInMovelist(list, "e5e4");
	validateUciIsInMovelist(list, "e5e3");
	validateUciIsInMovelist(list, "e5e2");

	moveListFree(list);
}

void testQueenMoves()
{
	board b;
	moveList *list;

	// Lone queen
	boardInitFromFenInPlace(&b, "8/8/8/8/8/8/1Q6/8 w - - 0 1");

	list = pmGetQueenMoves(&b, sqS("b2"));

	validateListSize(list, 23);
	validateUciIsInMovelist(list, "b2b3");
	validateUciIsInMovelist(list, "b2b4");
	validateUciIsInMovelist(list, "b2b5");
	validateUciIsInMovelist(list, "b2b6");
	validateUciIsInMovelist(list, "b2b7");
	validateUciIsInMovelist(list, "b2b8");

	validateUciIsInMovelist(list, "b2c3");
	validateUciIsInMovelist(list, "b2d4");
	validateUciIsInMovelist(list, "b2e5");
	validateUciIsInMovelist(list, "b2f6");
	validateUciIsInMovelist(list, "b2g7");
	validateUciIsInMovelist(list, "b2h8");

	validateUciIsInMovelist(list, "b2c2");
	validateUciIsInMovelist(list, "b2d2");
	validateUciIsInMovelist(list, "b2e2");
	validateUciIsInMovelist(list, "b2f2");
	validateUciIsInMovelist(list, "b2g2");
	validateUciIsInMovelist(list, "b2h2");

	validateUciIsInMovelist(list, "b2c1");
	validateUciIsInMovelist(list, "b2b1");
	validateUciIsInMovelist(list, "b2a1");
	validateUciIsInMovelist(list, "b2a2");
	validateUciIsInMovelist(list, "b2a3");

	moveListFree(list);

	// Queen with stuff around
	boardInitFromFenInPlace(&b, "8/1Q3N2/2br4/2Rq2P1/2Rr4/6p1/5P2/8 b - - 0 1");

	list = pmGetQueenMoves(&b, sqS("d5"));

	validateListSize(list, 11);
	validateUciIsInMovelist(list, "d5e6");
	validateUciIsInMovelist(list, "d5f7");
	validateUciIsInMovelist(list, "d5e5");
	validateUciIsInMovelist(list, "d5f5");
	validateUciIsInMovelist(list, "d5g5");
	validateUciIsInMovelist(list, "d5e4");
	validateUciIsInMovelist(list, "d5f3");
	validateUciIsInMovelist(list, "d5g2");
	validateUciIsInMovelist(list, "d5h1");
	validateUciIsInMovelist(list, "d5c4");
	validateUciIsInMovelist(list, "d5c5");

	moveListFree(list);
}

void testKingMoves()
{
	// NOTE: pmGetKingMoves DOES NOT CONSIDER CHECK!
	// The king will be able to move into check since pmGetKingMoves() does not consider the legality of a move.

	board b;
	moveList *list;

	// Lone king
	boardInitFromFenInPlace(&b, "8/2k5/8/8/8/8/8/8 b - - 0 1");

	list = pmGetKingMoves(&b, sqS("c7"));

	validateListSize(list, 8);
	validateUciIsInMovelist(list, "c7c8");
	validateUciIsInMovelist(list, "c7d8");
	validateUciIsInMovelist(list, "c7d7");
	validateUciIsInMovelist(list, "c7d6");
	validateUciIsInMovelist(list, "c7c6");
	validateUciIsInMovelist(list, "c7b6");
	validateUciIsInMovelist(list, "c7b7");
	validateUciIsInMovelist(list, "c7b8");

	moveListFree(list);

	// King in corner
	boardInitFromFenInPlace(&b, "8/8/8/8/8/8/8/K7 w - - 0 1");

	list = pmGetKingMoves(&b, sqS("a1"));

	validateListSize(list, 3);
	validateUciIsInMovelist(list, "a1a2");
	validateUciIsInMovelist(list, "a1b2");
	validateUciIsInMovelist(list, "a1b1");

	moveListFree(list);

	// King with stuff around
	boardInitFromFenInPlace(&b, "8/2B2R2/3nb3/3k4/2BRn3/8/3q4/8 b - - 0 1");

	list = pmGetKingMoves(&b, sqS("d5"));

	validateListSize(list, 5);
	validateUciIsInMovelist(list, "d5e5");
	validateUciIsInMovelist(list, "d5d4");
	validateUciIsInMovelist(list, "d5c4");
	validateUciIsInMovelist(list, "d5c5");
	validateUciIsInMovelist(list, "d5c6");

	moveListFree(list);
}


/////////////////////////////////////
// TEST ATTACKED SQUARES AND CHECK //
/////////////////////////////////////

void testIsSquareAttacked()
{
	board b;

	// Knight in corner and pawn
	boardInitFromFenInPlace(&b, "7N/8/8/8/8/8/1P6/8 w - - 0 1");

	for (int i = 0; i < 64; i++)
	{
		sq s = sqIndex(i);

		uint8_t expectedAttacked = sqEq(s, sqS("a3")) || sqEq(s, sqS("c3")) || sqEq(s, sqS("f7"))
				|| sqEq(s, sqS("g6"));

		uint8_t actualAttacked = boardIsSquareAttacked(&b, s, pcWhite);

		if (expectedAttacked != actualAttacked)
		{
			char message[50];
			sprintf(message, "Actual %s attacked: %u, expected: %u", sqGetStr(s), actualAttacked, expectedAttacked);
			failTest(message);
		}
	}

	// Lone rook
	boardInitFromFenInPlace(&b, "8/8/8/3r4/8/8/8/8 b - - 0 1");

	for (int i = 0; i < 64; i++)
	{
		sq s = sqIndex(i);

		uint8_t expectedAttacked = (s.file == 4) ^ (s.rank == 5);

		uint8_t actualAttacked = boardIsSquareAttacked(&b, s, pcBlack);

		if (expectedAttacked != actualAttacked)
		{
			char message[50];
			sprintf(message, "Actual %s attacked: %u, expected: %u", sqGetStr(s), actualAttacked, expectedAttacked);
			failTest(message);
		}
	}

	// Rook with blocks/captures
	boardInitFromFenInPlace(&b, "8/8/8/8/1p6/8/8/1r1R4 w - - 0 1");

	for (int i = 0; i < 64; i++)
	{
		sq s = sqIndex(i);

		uint8_t expectedAttacked = sqEq(s, sqS("a1")) || sqEq(s, sqS("c1")) || sqEq(s, sqS("d1"))
				|| sqEq(s, sqS("b2")) || sqEq(s, sqS("b3")) || sqEq(s, sqS("a3")) || sqEq(s, sqS("c3"));

		uint8_t actualAttacked = boardIsSquareAttacked(&b, s, pcBlack);

		if (expectedAttacked != actualAttacked)
		{
			char message[50];
			sprintf(message, "Actual %s attacked: %u, expected: %u", sqGetStr(s), actualAttacked, expectedAttacked);
			failTest(message);
		}
	}
}

void testIsInCheck()
{
	board b;

	// Simple example - in check by a rook
	boardInitFromFenInPlace(&b, "8/8/1k6/1r2K3/8/8/8/8 w - - 0 1");

	if (!boardIsInCheck(&b))
		failTest("Board was not in check, expected board to be in check");

	if (boardIsPlayerInCheck(&b, pcBlack))
		failTest("Board was in check, expected board to be not in check");

	// Scholar's mate
	boardInitFromFenInPlace(&b, "r1bqkb1r/pppp1Qpp/2n2n2/4p3/2B1P3/8/PPPP1PPP/RNB1K1NR b KQkq - 0 4");

	if (!boardIsInCheck(&b))
		failTest("Board was not in check, expected board to be in check");

	if (boardIsPlayerInCheck(&b, pcWhite))
		failTest("Board was in check, expected board to be not in check");

	// Checks blocked by knights
	boardInitFromFenInPlace(&b, "7k/3q4/1q3q2/2NNN3/q1NKN1q1/2NNN3/1q3q2/3q4 w - - 0 1");

	if (boardIsInCheck(&b))
		failTest("Board was in check, expected board to be not in check");

	if (boardIsPlayerInCheck(&b, pcBlack))
		failTest("Board was in check, expected board to be not in check");
}


////////////////////////
// TEST PLAYING MOVES //
////////////////////////

// HELPER FUNCTION - validates that two boards are equal and fails the test if they are not
// Do not make message "name" more than 65ish characters pls :)
void validateBoardEq(const char *name, board *b1, board *b2)
{
	if (!boardEq(b1, b2))
	{
		char message[100];
		sprintf(message, "%s boards were not exactly the same", name);
		failTest(message);
	}
}

void testBoardPlayMove()
{
	board *b = boardCreate();
	board *bCheck;

	// 1. e4
	boardPlayMoveInPlace(b, moveSq(sqS("e2"), sqS("e4")));
	bCheck = boardCreateFromFen("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");
	board *bCheckFuzzy = boardCreateFromFen("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq - 32 53");

	validateBoardEq("1. e4", b, bCheck);

	// Manually check for fuzziness
	if (boardEq(b, bCheckFuzzy))
		failTest("1. e4 boards were EXACTLY equal - they should have differed in EP target and move counts");

	if (!boardEqContext(b, bCheckFuzzy))
		failTest("1. e4 boards were not contextually the same, expected the same board");

	free(bCheckFuzzy);

	// 1... Nf6
	boardPlayMoveInPlace(b, moveSq(sqS("g8"), sqS("f6")));
	boardInitFromFenInPlace(bCheck, "rnbqkb1r/pppppppp/5n2/8/4P3/8/PPPP1PPP/RNBQKBNR w KQkq - 1 2");

	validateBoardEq("1... Nf6", b, bCheck);

	// Test capturing
	// Rook captures rook
	boardInitFromFenInPlace(b, "4k3/8/8/8/2R2r2/8/8/4K3 w - - 0 1");

	boardPlayMoveInPlace(b, moveFromUci("c4f4"));
	boardInitFromFenInPlace(bCheck, "4k3/8/8/8/5R2/8/8/4K3 b - - 0 1");

	validateBoardEq("Rook captures rook", b, bCheck);

	// Bishop captures pawn in opening
	boardInitFromFenInPlace(b, "rnbqkbnr/pppp1ppp/8/4p3/4P3/P7/1PPP1PPP/RNBQKBNR b KQkq - 0 2");

	boardPlayMoveInPlace(b, moveFromUci("f8a3"));
	boardInitFromFenInPlace(bCheck, "rnbqk1nr/pppp1ppp/8/4p3/4P3/b7/1PPP1PPP/RNBQKBNR w KQkq - 0 3");

	validateBoardEq("Bishop captures pawn", b, bCheck);

	// Really bad draw
	boardInitFromFenInPlace(b, "8/5k2/4q3/8/8/8/1K6/8 b - - 0 1");

	boardPlayMoveInPlace(b, moveFromUci("e6b3"));
	boardInitFromFenInPlace(bCheck, "8/5k2/8/8/8/1q6/1K6/8 w - - 1 2");

	validateBoardEq("Really bad draw move 1", b, bCheck);

	boardPlayMoveInPlace(b, moveFromUci("b2b3"));
	boardInitFromFenInPlace(bCheck, "8/5k2/8/8/8/1K6/8/8 b - - 0 2");

	validateBoardEq("Really bad draw move 2", b, bCheck);

	// Check castling
	// White O-O
	boardInitFromFenInPlace(b, "rnbqk2r/pppp1ppp/5n2/2b1p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 4 4");

	boardPlayMoveInPlace(b, moveFromUci("e1g1"));
	boardInitFromFenInPlace(bCheck, "rnbqk2r/pppp1ppp/5n2/2b1p3/2B1P3/5N2/PPPP1PPP/RNBQ1RK1 b kq - 5 4");

	validateBoardEq("White O-O", b, bCheck);

	// Black O-O
	boardPlayMoveInPlace(b, moveFromUci("e8g8"));
	boardInitFromFenInPlace(bCheck, "rnbq1rk1/pppp1ppp/5n2/2b1p3/2B1P3/5N2/PPPP1PPP/RNBQ1RK1 w - - 6 5");

	validateBoardEq("Black O-O", b, bCheck);

	// White O-O-O
	boardInitFromFenInPlace(b, "r3kbnr/ppp1pppp/2nq4/3p1b2/3P1B2/2NQ4/PPP1PPPP/R3KBNR w KQkq - 6 5");

	boardPlayMoveInPlace(b, moveFromUci("e1c1"));
	boardInitFromFenInPlace(bCheck, "r3kbnr/ppp1pppp/2nq4/3p1b2/3P1B2/2NQ4/PPP1PPPP/2KR1BNR b kq - 7 5");

	validateBoardEq("White O-O-O", b, bCheck);

	// Black O-O-O
	boardPlayMoveInPlace(b, moveFromUci("e8c8"));
	boardInitFromFenInPlace(bCheck, "2kr1bnr/ppp1pppp/2nq4/3p1b2/3P1B2/2NQ4/PPP1PPPP/2KR1BNR w - - 8 6");

	validateBoardEq("Black O-O-O", b, bCheck);

	// Test En Passant
	// White EP - black moves
	boardInitFromFenInPlace(b, "rnbqkbnr/ppppppp1/7p/4P3/8/8/PPPP1PPP/RNBQKBNR b KQkq - 0 2");

	boardPlayMoveInPlace(b, moveFromUci("f7f5"));
	boardInitFromFenInPlace(bCheck, "rnbqkbnr/ppppp1p1/7p/4Pp2/8/8/PPPP1PPP/RNBQKBNR w KQkq f6 0 3");

	validateBoardEq("White EP - black moves", b, bCheck);

	// White EP - capture
	boardPlayMoveInPlace(b, moveFromUci("e5f6"));
	boardInitFromFenInPlace(bCheck, "rnbqkbnr/ppppp1p1/5P1p/8/8/8/PPPP1PPP/RNBQKBNR b KQkq - 0 3");

	validateBoardEq("White EP - capture", b, bCheck);

	// Black EP - black moves
	boardInitFromFenInPlace(b, "rnbqkbnr/pppp1ppp/8/8/4p2P/8/PPPPPPP1/RNBQKBNR w KQkq - 0 3");

	boardPlayMoveInPlace(b, moveFromUci("d2d4"));
	boardInitFromFenInPlace(bCheck, "rnbqkbnr/pppp1ppp/8/8/3Pp2P/8/PPP1PPP1/RNBQKBNR b KQkq d3 0 3");

	validateBoardEq("Black EP - white moves", b, bCheck);

	// Black EP - capture
	boardPlayMoveInPlace(b, moveFromUci("e4d3"));
	boardInitFromFenInPlace(bCheck, "rnbqkbnr/pppp1ppp/8/8/7P/3p4/PPP1PPP1/RNBQKBNR w KQkq - 0 4");

	validateBoardEq("Black EP - capture", b, bCheck);

	// Free boards
	free(b);
	free(bCheck);
}


////////////////////////////////
// TEST BOARD MOVE GENERATION //
////////////////////////////////

// Helper function - asserts that given UCI string does NOT in moveList. For castling tests
void validateUciIsNotInMovelist(moveList *list, char *expectedUci)
{
	for (moveListNode *n = list->head; n; n = n->next)
	{
		char *actualUci = moveGetUci(n->move);
		int cmp = strcmp(actualUci, expectedUci);
		free(actualUci);
		if (cmp == 0)
		{
			char message[60];
			sprintf(message, "Expected %s to be absent but was in movelist", expectedUci);
			failTest(message);
			return;
		}
	}
	// All good, was not hit
}

void testBoardGenerateMoves()
{
	board b;
	moveList *list;

	// Test all 20 moves on initial board
	boardInitInPlace(&b);
	list = boardGenerateMoves(&b);

	validateListSize(list, 20);
	// Pawn moves
	validateUciIsInMovelist(list, "a2a3");
	validateUciIsInMovelist(list, "a2a4");
	validateUciIsInMovelist(list, "b2b3");
	validateUciIsInMovelist(list, "b2b4");
	validateUciIsInMovelist(list, "c2c3");
	validateUciIsInMovelist(list, "c2c4");
	validateUciIsInMovelist(list, "d2d3");
	validateUciIsInMovelist(list, "d2d4");
	validateUciIsInMovelist(list, "e2e3");
	validateUciIsInMovelist(list, "e2e4");
	validateUciIsInMovelist(list, "f2f3");
	validateUciIsInMovelist(list, "f2f4");
	validateUciIsInMovelist(list, "g2g3");
	validateUciIsInMovelist(list, "g2g4");
	validateUciIsInMovelist(list, "h2h3");
	validateUciIsInMovelist(list, "h2h4");
	// Knight moves
	validateUciIsInMovelist(list, "b1a3");
	validateUciIsInMovelist(list, "b1c3");
	validateUciIsInMovelist(list, "g1f3");
	validateUciIsInMovelist(list, "g1h3");

	moveListFree(list);

	// Board with some moves played
	boardInitFromFenInPlace(&b, "r1bqkbnr/ppp2ppp/2np4/4p3/2B1P3/3P4/PPP2PPP/RNBQK1NR w KQkq - 1 4");
	list = boardGenerateMoves(&b);

	validateListSize(list, 12 + 5 + 11 + 6 + 3);
	// Pawn moves, 12
	validateUciIsInMovelist(list, "a2a3");
	validateUciIsInMovelist(list, "a2a4");
	validateUciIsInMovelist(list, "b2b3");
	validateUciIsInMovelist(list, "b2b4");
	validateUciIsInMovelist(list, "c2c3");
	validateUciIsInMovelist(list, "d3d4");
	validateUciIsInMovelist(list, "f2f3");
	validateUciIsInMovelist(list, "f2f4");
	validateUciIsInMovelist(list, "g2g3");
	validateUciIsInMovelist(list, "g2g4");
	validateUciIsInMovelist(list, "h2h3");
	validateUciIsInMovelist(list, "h2h4");
	// Knight moves, 6
	validateUciIsInMovelist(list, "b1a3");
	validateUciIsInMovelist(list, "b1c3");
	validateUciIsInMovelist(list, "b1d2");
	validateUciIsInMovelist(list, "g1e2");
	validateUciIsInMovelist(list, "g1f3");
	validateUciIsInMovelist(list, "g1h3");
	// Bishop moves, 11
	// (Light square)
	validateUciIsInMovelist(list, "c4b5");
	validateUciIsInMovelist(list, "c4a6");
	validateUciIsInMovelist(list, "c4b3");
	validateUciIsInMovelist(list, "c4d5");
	validateUciIsInMovelist(list, "c4e6");
	validateUciIsInMovelist(list, "c4f7");
	// (Dark square)
	validateUciIsInMovelist(list, "c1d2");
	validateUciIsInMovelist(list, "c1e3");
	validateUciIsInMovelist(list, "c1f4");
	validateUciIsInMovelist(list, "c1g5");
	validateUciIsInMovelist(list, "c1h6");
	// Queen moves, 5
	validateUciIsInMovelist(list, "d1d2");
	validateUciIsInMovelist(list, "d1e2");
	validateUciIsInMovelist(list, "d1f3");
	validateUciIsInMovelist(list, "d1g4");
	validateUciIsInMovelist(list, "d1h5");
	// King moves, 3
	validateUciIsInMovelist(list, "e1d2");
	validateUciIsInMovelist(list, "e1e2");
	validateUciIsInMovelist(list, "e1f1");

	moveListFree(list);

	// Less pieces on the board
	boardInitFromFenInPlace(&b, "8/1k4n1/4n3/8/5P2/6R1/3Q1K2/8 b - - 0 1");
	list = boardGenerateMoves(&b);

	validateListSize(list, 7 + 3 + 8);
	// Knight e6 moves, 7
	validateUciIsInMovelist(list, "e6f8");
	validateUciIsInMovelist(list, "e6g5");
	validateUciIsInMovelist(list, "e6f4");
	validateUciIsInMovelist(list, "e6d4");
	validateUciIsInMovelist(list, "e6c5");
	validateUciIsInMovelist(list, "e6c7");
	validateUciIsInMovelist(list, "e6d8");
	// Knight g7 moves, 3
	validateUciIsInMovelist(list, "g7h5");
	validateUciIsInMovelist(list, "g7f5");
	validateUciIsInMovelist(list, "g7e8");
	// King moves, 8
	validateUciIsInMovelist(list, "b7b8");
	validateUciIsInMovelist(list, "b7c8");
	validateUciIsInMovelist(list, "b7c7");
	validateUciIsInMovelist(list, "b7c6");
	validateUciIsInMovelist(list, "b7b6");
	validateUciIsInMovelist(list, "b7a6");
	validateUciIsInMovelist(list, "b7a7");
	validateUciIsInMovelist(list, "b7a8");

	moveListFree(list);

	// Test a pinned knight - it can't move
	boardInitFromFenInPlace(&b, "8/8/8/6k1/1K1N2q1/8/8/8 w - - 0 1");
	list = boardGenerateMoves(&b);

	// should ONLY have king moves
	validateListSize(list, 8);
	validateUciIsInMovelist(list, "b4c4");
	validateUciIsNotInMovelist(list, "d4e6");

	moveListFree(list);

	// King is in check, but it can be blocked
	boardInitFromFenInPlace(&b, "3b3k/8/8/8/8/2Q5/4K3/8 b - - 0 1");
	list = boardGenerateMoves(&b);

	validateListSize(list, 3);
	// Bishop can move to block the check...
	validateUciIsInMovelist(list, "d8f6");
	// ...but it CANNOT make any other move
	validateUciIsNotInMovelist(list, "d8e7");
	validateUciIsNotInMovelist(list, "d8g5");
	validateUciIsNotInMovelist(list, "d8h4");

	moveListFree(list);
}

void testBoardGenerateMovesCastling()
{
	board b;
	moveList *list;

	// White O-O
	boardInitFromFenInPlace(&b, "rnbqk2r/pppp1ppp/3b1n2/4p3/4P3/3B1N2/PPPP1PPP/RNBQK2R w KQkq - 4 4");
	list = boardGenerateMoves(&b);

	validateUciIsInMovelist(list, "e1g1");

	moveListFree(list);

	// If we remove the castling flag?
	b.castleState = b.castleState & (~CASTLE_WK);
	list = boardGenerateMoves(&b);

	validateUciIsNotInMovelist(list, "e1g1");

	moveListFree(list);

	// Black O-O
	b.currentPlayer = pcBlack;
	list = boardGenerateMoves(&b);

	validateUciIsInMovelist(list, "e8g8");

	moveListFree(list);

	// White O-O-O
	boardInitFromFenInPlace(&b, "r3kbnr/ppp1pppp/2nqb3/3p4/3P4/2NQB3/PPP1PPPP/R3KBNR w KQkq - 6 5");
	list = boardGenerateMoves(&b);

	validateUciIsInMovelist(list, "e1c1");

	moveListFree(list);

	// Black O-O-O
	b.currentPlayer = pcBlack;
	list = boardGenerateMoves(&b);

	validateUciIsInMovelist(list, "e8c8");

	moveListFree(list);

	// King is in check - no castling
	boardInitFromFenInPlace(&b, "4k3/8/4r3/8/8/8/8/R3K2R w KQ - 0 1");
	list = boardGenerateMoves(&b);

	validateUciIsNotInMovelist(list, "e1g1");
	validateUciIsNotInMovelist(list, "e1c1");

	moveListFree(list);

	// Rook is checking the f1 square - no O-O but yes O-O-O
	boardInitFromFenInPlace(&b, "4k3/8/5r2/8/8/8/8/R3K2R w KQ - 0 1");
	list = boardGenerateMoves(&b);

	validateUciIsNotInMovelist(list, "e1g1");
	validateUciIsInMovelist(list, "e1c1");

	moveListFree(list);

	// Rook is checking the g1 square - no O-O but yes O-O-O
	boardInitFromFenInPlace(&b, "4k3/8/6r1/8/8/8/8/R3K2R w KQ - 0 1");
	list = boardGenerateMoves(&b);

	validateUciIsNotInMovelist(list, "e1g1");
	validateUciIsInMovelist(list, "e1c1");

	moveListFree(list);

	// Rook is attacking the h rook - both castlings allowed
	boardInitFromFenInPlace(&b, "4k3/8/7r/8/8/8/8/R3K2R w KQ - 0 1");
	list = boardGenerateMoves(&b);

	validateUciIsInMovelist(list, "e1g1");
	validateUciIsInMovelist(list, "e1c1");

	moveListFree(list);

	// Rook is checking d8, no O-O-O
	boardInitFromFenInPlace(&b, "r3k2r/8/8/8/8/3R4/8/4K3 b kq - 0 1");
	list = boardGenerateMoves(&b);

	validateUciIsInMovelist(list, "e8g8");
	validateUciIsNotInMovelist(list, "e8c8");

	moveListFree(list);

	// Rook is checking c8, no O-O-O
	boardInitFromFenInPlace(&b, "r3k2r/8/8/8/8/2R5/8/4K3 b kq - 0 1");
	list = boardGenerateMoves(&b);

	validateUciIsInMovelist(list, "e8g8");
	validateUciIsNotInMovelist(list, "e8c8");

	moveListFree(list);

	// Rook is checking b8, both castlings allowed
	boardInitFromFenInPlace(&b, "r3k2r/8/8/8/8/1R6/8/4K3 b kq - 0 1");
	list = boardGenerateMoves(&b);

	validateUciIsInMovelist(list, "e8g8");
	validateUciIsInMovelist(list, "e8c8");

	moveListFree(list);

	// Rook is attacking a rook, both castlings allowed
	boardInitFromFenInPlace(&b, "r3k2r/8/8/8/8/R7/8/4K3 b kq - 0 1");
	list = boardGenerateMoves(&b);

	validateUciIsInMovelist(list, "e8g8");
	validateUciIsInMovelist(list, "e8c8");

	moveListFree(list);

	// Subtle case - PINNED piece is checking d1, but O-O-O still NOT allowed!!
	boardInitFromFenInPlace(&b, "8/8/8/8/1k6/2n5/3B4/R3K3 w Q - 0 1");
	list = boardGenerateMoves(&b);

	validateUciIsNotInMovelist(list, "e8c8");

	moveListFree(list);
}


/////////////////////////
// TEST FEN GENERATION //
/////////////////////////

// HELPER - takes in a FEN string, creates board, validates output
void validateBoardFen(char *fen)
{
	board b;
	boardInitFromFenInPlace(&b, fen);
	char *actualFen = boardGetFen(&b);

	validateString(actualFen, fen);

	free(actualFen);
}

void testBoardGetFen()
{
	validateBoardFen(INITIAL_FEN);
	validateBoardFen("rnb1kbnr/pppp1ppp/8/4p3/6Pq/5P2/PPPPP2P/RNBQKBNR w KQkq - 1 3"); 	// Fool's mate
	validateBoardFen("r1bqkb1r/pppp1Qpp/2n2n2/4p3/2B1P3/8/PPPP1PPP/RNB1K1NR b KQkq - 0 4"); 	// Scholar's mate
	validateBoardFen("r3r1k1/pp3pbp/1qp1b1p1/2B5/2BP4/Q1n2N2/P4PPP/3R1K1R w - - 4 18"); 	// GotC Be6!!
	validateBoardFen("r1q1k2r/8/8/8/3P4/8/8/R3K2R b Kq d3 0 1"); 	// Misc setup, varying castling states, EP
}


////////////////////////////////////////
// TEST DRAW BY INSUFFICIENT MATERIAL //
////////////////////////////////////////

void validateBoardIsInsufficientMaterial(board *b, uint8_t expected)
{
	uint8_t actual = boardIsInsufficientMaterial(b);
	if (actual != expected)
	{
		char message[100];
		sprintf(message,
				"Board %s draw by insufficient material, expected board %s",
				actual ? "is" : "is not",
				expected ? "is" : "is not");
		failTest(message);
	}
}

void testBoardIsInsufficientMaterial()
{
	board b;

	// Initial board
	boardInitInPlace(&b);
	validateBoardIsInsufficientMaterial(&b, 0);

	// Bare kings, true
	boardInitFromFenInPlace(&b, "8/4k3/8/8/2K5/8/8/8 w - - 0 1");
	validateBoardIsInsufficientMaterial(&b, 1);

	// K vs K+N, true
	boardInitFromFenInPlace(&b, "8/4k3/8/8/2K5/8/8/N7 w - - 0 1");
	validateBoardIsInsufficientMaterial(&b, 1);

	// K vs K+N+N, false
	boardInitFromFenInPlace(&b, "8/4k1n1/8/7n/2K5/8/8/8 w - - 0 1");
	validateBoardIsInsufficientMaterial(&b, 0);

	// K+B vs K+B, different colors, false
	boardInitFromFenInPlace(&b, "8/4kb2/8/8/2K5/6B1/8/8 w - - 0 1");
	validateBoardIsInsufficientMaterial(&b, 0);

	// K+B vs K+B, same colors, true
	boardInitFromFenInPlace(&b, "8/4k1b1/8/8/2K5/6B1/8/8 w - - 0 1");
	validateBoardIsInsufficientMaterial(&b, 1);

	// K+B+B+B+B vs K+B+B+B, all same colors, true
	boardInitFromFenInPlace(&b, "3b4/4k1b1/5b2/8/1BK5/6B1/3B1B2/8 w - - 0 1");
	validateBoardIsInsufficientMaterial(&b, 1);
}


/////////////////////
// TEST BOARD LIST //
/////////////////////

void testBoardList()
{
	board *b1 = boardCreate();
	board *b2 = boardPlayMove(b1, moveFromUci("e2e4"));
	board *b3 = boardPlayMove(b1, moveFromUci("c7c5"));

	boardList *l = boardListCreate();

	boardListAdd(l, b1);
	boardListAdd(l, b2);
	boardListAdd(l, b3);

	if (boardListGet(l, 0) != b1)
		failTest("boardListGet(l, 0) didn't return b1");

	if (boardListGet(l, 1) != b2)
		failTest("boardListGet(l, 1) didn't return b2");

	if (boardListGet(l, 2) != b3)
		failTest("boardListGet(l, 2) didn't return b3");

	// This also frees all boards
	boardListFree(l);
}


/////////////////////
// TEST SQUARE SET //
/////////////////////

void testSqSetSet()
{
	uint64_t ss = 0;

	sqSetSet(&ss, sqS("e4"), 1);

	if (ss != (uint64_t) 0x0000000010000000)
		failTest("Setting e4 didn't work");

	sqSetSet(&ss, sqS("d5"), 1);

	if (ss != (uint64_t) 0x0000000810000000)
		failTest("Setting d5 didn't work");

	sqSetSet(&ss, sqS("a1"), 1);

	if (ss != (uint64_t) 0x0000000810000001)
		failTest("Setting a1 didn't work");

	sqSetSet(&ss, sqS("h8"), 1);

	if (ss != (uint64_t) 0x8000000810000001)
		failTest("Setting h8 didn't work");
}

void testSqSetGet()
{
	// The diagonal from a1 to h8
	uint64_t ss = 0x8040201008040201;

	for (int i = 0; i < 64; i++)
	{
		sq s = sqIndex(i);

		uint8_t expected = (s.file == s.rank);
		uint8_t actual = sqSetGet(&ss, s);

		if (expected != actual)
		{
			char message[100];
			sprintf("Square %s was %d in the set, expected %d", sqGetStr(s), actual, expected);
			failTest(message);
		}
	}

	// Entire c file, f file, 3 rank, and 6 rank
	ss = 0x2424FF2424FF2424;

	for (int i = 0; i < 64; i++)
	{
		sq s = sqIndex(i);

		uint8_t expected = (s.file == 3 || s.rank == 3 || s.file == 6 || s.rank == 6);
		uint8_t actual = sqSetGet(&ss, s);

		if (expected != actual)
		{
			char message[100];
			sprintf("Square %s was %d in the set, expected %d", sqGetStr(s), actual, expected);
			failTest(message);
		}
	}
}
