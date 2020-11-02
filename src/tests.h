/*
 * Testing definitions for Chess implementation in C
 * Created by thearst3rd on 8/6/2020
 */

#pragma once

// Fail the currently running test and halt the program. NULL for no message
void failTest(const char *msg);
void validateString(const char *actual, const char *expected);

// Square testing
void testSqI();
void testSqS();
void testSqGetStr();
void testSqIsDark();

// Move testing
void testMoveCreate();
void testMoveFromUci();
void testMoveGetUci();

// Move list testing
void testMoveList();

// Board testing
void testBoardCreate();
void testBoardCreateFromFen();
void testBoardEq();

// Piece moves testing
void testPawnMoves();
void testKnightMoves();
void testBishopMoves();
void testRookMoves();
void testQueenMoves();
void testKingMoves();

// Square attacking/check testing
void testIsSquareAttacked();
void testIsInCheck();

// Test playing moves
void testBoardPlayMove();

// Test full move generation
void testBoardGenerateMoves();
void testBoardGenerateMovesCastling();

// Test FEN generation
void testBoardGetFen();

// Test draw by insufficient material
void testBoardIsInsufficientMaterial();

// Test board list
void testBoardList();

// Test square set
void testSqSetSet();
void testSqSetGet();
