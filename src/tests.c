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
#include "chesslib/piecemoves.h"

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

	// Test Piece Moves
	RUN_TEST(testPawnMoves);
	RUN_TEST(testKnightMoves);
	RUN_TEST(testBishopMoves);
	RUN_TEST(testRookMoves);
	RUN_TEST(testQueenMoves);
	RUN_TEST(testKingMoves);

	// Test Attacked Squares
	RUN_TEST(testIsSquareAttacked);

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
	if (!posEq(m.from, expectedFrom) || !posEq(m.to, expectedTo)
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

	if (b.currentPlayer != white)
		failTest("Actual: black to play, expected: white to play");

	if (b.castleState != 0b1111)
		failTest("Not all castling flags enabled");

	if (posEq(b.epTarget, POS_INVALID))
		failTest("EP target square was not POS_INVALID");
}

void testBoardCreateFromFen()
{
	board b = createBoardFromFen("8/8/3k4/8/4Pp2/2K5/8/5QQQ b - e3");

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

	if (b.currentPlayer != black)
		failTest("Actual: white to play, expected: black to play");

	if (b.castleState != 0b0000)
		failTest("Not all castling flags disabled");

	if (!posEq(b.epTarget, posI(5, 3)))
	{
		char message[50];
		sprintf(message, "Actual EP target square: %s, expected: e3", posGetStr(b.epTarget));
		failTest(message);
	}
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
	board b;
	moveList *list;

	// Lone white pawn on starting rank
	b = createBoardFromFen("8/8/8/8/8/8/4P3/8 w - -");

	list = getPawnMoves(&b, posS("e2"));

	validateListSize(list, 2);
	validateUciIsInMovelist(list, "e2e3");
	validateUciIsInMovelist(list, "e2e4");

	freeMoveList(list);

	// Pawn with captures
	b = createBoardFromFen("8/8/8/8/8/3q1n2/4P3/8 w - -");

	list = getPawnMoves(&b, posS("e2"));

	validateListSize(list, 4);
	validateUciIsInMovelist(list, "e2e3");
	validateUciIsInMovelist(list, "e2e4");
	validateUciIsInMovelist(list, "e2d3");
	validateUciIsInMovelist(list, "e2f3");

	freeMoveList(list);

	// Pawn blocked
	b = createBoardFromFen("8/8/8/8/8/4N3/4P3/8 w - -");

	list = getPawnMoves(&b, posS("e2"));

	validateListSize(list, 0);

	freeMoveList(list);

	// Pawn not on first rank
	b = createBoardFromFen("8/8/8/8/4P3/8/8/8 w - -");

	list = getPawnMoves(&b, posS("e4"));

	validateListSize(list, 1);
	validateUciIsInMovelist(list, "e4e5");

	freeMoveList(list);

	// White pawn about to promote
	b = createBoardFromFen("8/6P1/8/8/8/8/8/8 w - -");

	list = getPawnMoves(&b, posS("g7"));

	validateListSize(list, 4);
	validateUciIsInMovelist(list, "g7g8q");
	validateUciIsInMovelist(list, "g7g8r");
	validateUciIsInMovelist(list, "g7g8b");
	validateUciIsInMovelist(list, "g7g8n");

	freeMoveList(list);

	// Black pawn on first rank
	b = createBoardFromFen("8/6p1/8/8/8/8/8/8 b - -");

	list = getPawnMoves(&b, posS("g7"));

	validateListSize(list, 2);
	validateUciIsInMovelist(list, "g7g6");
	validateUciIsInMovelist(list, "g7g5");

	freeMoveList(list);

	// Pawn blocked with one free spot, opp color
	b = createBoardFromFen("8/6p1/8/6P1/8/8/8/8 b - -");

	list = getPawnMoves(&b, posS("g7"));

	validateListSize(list, 1);
	validateUciIsInMovelist(list, "g7g6");

	freeMoveList(list);

	// Black pawn about to promote, with capturing ability
	b = createBoardFromFen("8/8/8/8/8/8/p7/1Q6 b - -");

	list = getPawnMoves(&b, posS("a2"));

	validateListSize(list, 8);
	validateUciIsInMovelist(list, "a2a1q");
	validateUciIsInMovelist(list, "a2a1r");
	validateUciIsInMovelist(list, "a2a1b");
	validateUciIsInMovelist(list, "a2a1n");
	validateUciIsInMovelist(list, "a2b1q");
	validateUciIsInMovelist(list, "a2b1r");
	validateUciIsInMovelist(list, "a2b1b");
	validateUciIsInMovelist(list, "a2b1n");

	freeMoveList(list);

	// White with opportunity to capture en passant
	b = createBoardFromFen("8/8/8/4Pp2/8/8/8/8 w - f6");

	b.currentPlayer = white;
	b.epTarget = posS("f6");

	list = getPawnMoves(&b, posS("e5"));

	validateListSize(list, 2);
	validateUciIsInMovelist(list, "e5e6");
	validateUciIsInMovelist(list, "e5f6");

	freeMoveList(list);

	// Black with opportunity to capture en passant
	b = createBoardFromFen("8/8/8/8/6Pp/8/8/8 b - g3");

	b.currentPlayer = black;
	b.epTarget = posS("g3");

	list = getPawnMoves(&b, posS("h4"));

	validateListSize(list, 2);
	validateUciIsInMovelist(list, "h4h3");
	validateUciIsInMovelist(list, "h4g3");

	freeMoveList(list);
}

void testKnightMoves()
{
	board b;
	moveList *list;

	// Lone knight
	b = createBoardFromFen("8/8/8/8/8/2N5/8/8 w - -");

	list = getKnightMoves(&b, posS("c3"));

	validateListSize(list, 8);
	validateUciIsInMovelist(list, "c3d5");
	validateUciIsInMovelist(list, "c3e4");
	validateUciIsInMovelist(list, "c3e2");
	validateUciIsInMovelist(list, "c3d1");
	validateUciIsInMovelist(list, "c3b1");
	validateUciIsInMovelist(list, "c3a2");
	validateUciIsInMovelist(list, "c3a4");
	validateUciIsInMovelist(list, "c3b5");

	freeMoveList(list);

	// Knight near the edge
	b = createBoardFromFen("8/7n/8/8/8/8/8/8 b - -");

	list = getKnightMoves(&b, posS("h7"));

	validateListSize(list, 3);
	validateUciIsInMovelist(list, "h7g5");
	validateUciIsInMovelist(list, "h7f6");
	validateUciIsInMovelist(list, "h7f8");

	freeMoveList(list);

	// Knight with some blocks
	b = createBoardFromFen("6N1/3BQQQn/4QnQ1/4QQQ1/4b3/8/8/8 b - -");

	list = getKnightMoves(&b, posS("f6"));

	validateListSize(list, 6);
	validateUciIsInMovelist(list, "f6g8");
	validateUciIsInMovelist(list, "f6h5");
	validateUciIsInMovelist(list, "f6g4");
	validateUciIsInMovelist(list, "f6d5");
	validateUciIsInMovelist(list, "f6d7");
	validateUciIsInMovelist(list, "f6e8");

	freeMoveList(list);
}

void testBishopMoves()
{
	board b;
	moveList *list;

	// Lone bishop
	b = createBoardFromFen("8/4b3/8/8/8/8/8/8 b - -");

	list = getBishopMoves(&b, posS("e7"));

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

	freeMoveList(list);

	// Position with some blocks captures
	b = createBoardFromFen("8/8/5n2/8/1r6/2B5/3Q4/N7 w - -");

	list = getBishopMoves(&b, posS("c3"));

	validateListSize(list, 5);
	validateUciIsInMovelist(list, "c3d4");
	validateUciIsInMovelist(list, "c3e5");
	validateUciIsInMovelist(list, "c3f6");
	validateUciIsInMovelist(list, "c3b2");
	validateUciIsInMovelist(list, "c3b4");

	freeMoveList(list);
}

void testRookMoves()
{
	board b;
	moveList *list;

	// Lone rook
	b = createBoardFromFen("8/3R4/8/8/8/8/8/8 w - -");

	list = getRookMoves(&b, posS("d7"));

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

	freeMoveList(list);

	// Rook with some stuff around
	b = createBoardFromFen("8/3R4/8/2k1r1BQ/8/8/3n1R2/4b3 b - -");

	list = getRookMoves(&b, posS("e5"));

	validateListSize(list, 9);
	validateUciIsInMovelist(list, "e5e6");
	validateUciIsInMovelist(list, "e5e7");
	validateUciIsInMovelist(list, "e5e8");
	validateUciIsInMovelist(list, "e5f5");
	validateUciIsInMovelist(list, "e5g5");
	validateUciIsInMovelist(list, "e5e4");
	validateUciIsInMovelist(list, "e5e3");
	validateUciIsInMovelist(list, "e5e2");

	freeMoveList(list);
}

void testQueenMoves()
{
	board b;
	moveList *list;

	// Lone queen
	b = createBoardFromFen("8/8/8/8/8/8/1Q6/8 w - -");

	list = getQueenMoves(&b, posS("b2"));

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

	freeMoveList(list);

	// Queen with stuff around
	b = createBoardFromFen("8/1Q3N2/2br4/2Rq2P1/2Rr4/6p1/5P2/8 b - -");

	list = getQueenMoves(&b, posS("d5"));

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

	freeMoveList(list);
}

void testKingMoves()
{
	// NOTE: THIS TEST DOES NOT CONSIDER CHECK!
	// The king will be able to move into check since getKingMoves() does not consider the legality of a move.

	// TODO - refactor this test when I add castling

	board b;
	moveList *list;

	// Lone king
	b = createBoardFromFen("8/2k5/8/8/8/8/8/8 b - -");

	list = getKingMoves(&b, posS("c7"));

	validateListSize(list, 8);
	validateUciIsInMovelist(list, "c7c8");
	validateUciIsInMovelist(list, "c7d8");
	validateUciIsInMovelist(list, "c7d7");
	validateUciIsInMovelist(list, "c7d6");
	validateUciIsInMovelist(list, "c7c6");
	validateUciIsInMovelist(list, "c7b6");
	validateUciIsInMovelist(list, "c7b7");
	validateUciIsInMovelist(list, "c7b8");

	freeMoveList(list);

	// King in corner
	b = createBoardFromFen("8/8/8/8/8/8/8/K7 w - -");

	list = getKingMoves(&b, posS("a1"));

	validateListSize(list, 3);
	validateUciIsInMovelist(list, "a1a2");
	validateUciIsInMovelist(list, "a1b2");
	validateUciIsInMovelist(list, "a1b1");

	freeMoveList(list);

	// King with stuff around
	b = createBoardFromFen("8/2B2R2/3nb3/3k4/2BRn3/8/3q4/8 b - -");

	list = getKingMoves(&b, posS("d5"));

	validateListSize(list, 5);
	validateUciIsInMovelist(list, "d5e5");
	validateUciIsInMovelist(list, "d5d4");
	validateUciIsInMovelist(list, "d5c4");
	validateUciIsInMovelist(list, "d5c5");
	validateUciIsInMovelist(list, "d5c6");

	freeMoveList(list);
}


/////////////////////////////////////
// TEST ATTACKED SQUARES AND CHECK //
/////////////////////////////////////

void testIsSquareAttacked()
{
	board b;

	// Knight in corner and pawn
	b = createBoardFromFen("7N/8/8/8/8/8/1P6/8 w - -");

	for (int i = 0; i < 64; i++)
	{
		pos p = posIndex(i);

		uint8_t expectedAttacked = posEq(p, posS("a3")) || posEq(p, posS("c3")) || posEq(p, posS("f7"))
				|| posEq(p, posS("g6"));

		uint8_t actualAttacked = boardIsSquareAttacked(&b, p, white);

		if (expectedAttacked != actualAttacked)
		{
			char message[50];
			sprintf(message, "Actual %s attacked: %u, expected: %u", posGetStr(p), actualAttacked, expectedAttacked);
			failTest(message);
		}
	}

	// Lone rook
	b = createBoardFromFen("8/8/8/3r4/8/8/8/8 b - -");

	for (int i = 0; i < 64; i++)
	{
		pos p = posIndex(i);

		uint8_t expectedAttacked = (p.file == 4) ^ (p.rank == 5);

		uint8_t actualAttacked = boardIsSquareAttacked(&b, p, black);

		if (expectedAttacked != actualAttacked)
		{
			char message[50];
			sprintf(message, "Actual %s attacked: %u, expected: %u", posGetStr(p), actualAttacked, expectedAttacked);
			failTest(message);
		}
	}

	// Rook with blocks
	b = createBoardFromFen("8/8/8/8/8/1r6/8/1r1R4 w - - 0 1");
}
