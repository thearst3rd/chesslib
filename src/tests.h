/*
 * Testing definitions for Chess implementation in C
 * Created by thearst3rd on 8/6/2020
 */

#pragma once

// Fail the currently running test and halt the program. NULL for no message
void failTest(const char *msg);
void validateString(const char *actual, const char *expected);

// Pos testing
void testPosI();
void testPosS();
void testPosGetStr();

// Move testing
void testMoveCreate();
void testMoveFromUci();
void testMoveGetUci();

// Move list testing
void testMoveList();

// Board testing
void testBoardInit();
void testBoardInitFromFen();

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
