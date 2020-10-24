/*
 * Board definitions
 * Created by thearst3rd on 8/23/2020
 */

#pragma once

#include "chesslib/square.h"
#include "chesslib/piece.h"
#include "chesslib/movelist.h"

#define INITIAL_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

#define CASTLE_WK 0b0001
#define CASTLE_WQ 0b0010
#define CASTLE_BK 0b0100
#define CASTLE_BQ 0b1000

typedef struct
{
	piece pieces[64];
	pieceColor currentPlayer;
	uint8_t castleState; 	// Bitmask describing castle state
	sq epTarget;
	uint32_t halfMoveClock;
	uint32_t moveNumber;
} board;

// Initializes the given memory into a board
// boardInitFromFen returns 0 if successful and 1 if not. If not successful, it will init board with default FEN
void boardInit(board *b);
int boardInitFromFen(board *b, const char *fen);

void boardSetPiece(board *b, sq s, piece p);
piece boardGetPiece(board *b, sq s);

moveList *boardGenerateMoves(board *b);

uint8_t boardIsSquareAttacked(board *b, sq s, pieceColor attacker);
uint8_t boardIsInCheck(board *b);
uint8_t boardIsPlayerInCheck(board *b, pieceColor player);

uint8_t boardIsInsufficientMaterial(board *b);

// Returns a new board on which the given move was played on the given board
board boardPlayMove(board *b, move m);

// Returns if two boards are equal in all ways
uint8_t boardEq(board *b1, board *b2);
// Returns if two boards are equal WITHOUT the counters, and filtering the EP target square
uint8_t boardEqContext(board *b1, board *b2);

char *boardGetFen(board *b);
