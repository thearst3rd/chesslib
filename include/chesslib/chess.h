/*
 * Chess game definitions
 * Created by thearst3rd on 10/24/2020
 */

#pragma once

#include "chesslib/boardlist.h"
#include "chesslib/movelist.h"

typedef enum
{
	tsOngoing,
	tsCheckmate,
	tsDrawStalemate,
	tsDrawClaimed50MoveRule,
	tsDraw75MoveRule,
	tsDrawClaimedThreefold,
	tsDrawFivefold,
	tsDrawInsufficient
} terminalState;

typedef struct
{
	boardList *boardHistory;
	moveList *moveHistory;
	moveList *currentLegalMoves;
	uint8_t repetitions; 	// How many times we have seen the current position
	terminalState terminal;
} chess;

// Creates and initializes a chess game. Must be freed. In FromFen: if invalid FEN, then NULL is returned.
chess *chessCreate();
chess *chessCreateFen(const char *fen);

// Initializes the chess game
void chessInitInPlace(chess *g);
// Returns 0 if successful, 1 if invalid FEN (and does not init)
uint8_t chessInitFenInPlace(chess *g, const char *fen);

// Frees a chess game and all components
void chessFree(chess *g);

board *chessGetBoard(chess *g);

// Plays the given move. Returns 0 if successful, 1 if unsuccessful (move was illegal)
uint8_t chessPlayMove(chess *g, move m);
// Undoes the last move played. Returns 0 if successful, 1 if unsuccessful (no moves left)
uint8_t chessUndo(chess *g);

// Internal - updates the currentLegalMoves, repetitions, and terminalState fields. Called (interally) every move
void chessCalculateFields(chess *g);
