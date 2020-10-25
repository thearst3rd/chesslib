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
	tsDrawFivefold
} terminalState;

typedef struct
{
	boardList *boardHistory;
	moveList *currentLegalMoves;
	uint8_t repetitions; 	// How many times we have seen the current position
	terminalState terminal;
} chessGame;

// Initializes the chessGame
void chessGameInit(chessGame *g);
// Returns 0 if successful, 1 if invalid FEN (and inits a normal chess game instead)
uint8_t chessGameInitFromFen(chessGame *g, const char *fen);

// Frees all the components of a chessGame
void chessGameFreeComponents(chessGame *g);

board chessGameGetCurrentBoard(chessGame *g);

// Plays the given move. Returns 0 if successful, 1 if unsuccessful (move was illegal)
uint8_t chessGamePlayMove(chessGame *g, move m);
// Undoes the last move played. Returns 0 if successful, 1 if unsuccessful (no moves left)
uint8_t chessGameUndo(chessGame *g);

// Internal - updates the currentLegalMoves, repetitions, and terminalState fields. Called every move
void chessGameCalculateFields(chessGame *g);
