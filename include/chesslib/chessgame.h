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
} chessGame;

// Creates and initializes a chessGame. Must be freed. In FromFen: if invalid FEN, then NULL is returned.
chessGame *chessGameCreate();
chessGame *chessGameCreateFromFen(const char *fen);

// Initializes the chessGame
void chessGameInitInPlace(chessGame *g);
// Returns 0 if successful, 1 if invalid FEN (and does not init)
uint8_t chessGameInitFromFenInPlace(chessGame *g, const char *fen);

// Frees a chessGame and all components
void chessGameFree(chessGame *g);

board *chessGameGetCurrentBoard(chessGame *g);

// Plays the given move. Returns 0 if successful, 1 if unsuccessful (move was illegal)
uint8_t chessGamePlayMove(chessGame *g, move m);
// Undoes the last move played. Returns 0 if successful, 1 if unsuccessful (no moves left)
uint8_t chessGameUndo(chessGame *g);

// Internal - updates the currentLegalMoves, repetitions, and terminalState fields. Called every move
void chessGameCalculateFields(chessGame *g);
