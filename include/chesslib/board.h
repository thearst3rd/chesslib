/*
 * Board definitions
 * Created by thearst3rd on 8/23/2020
 */

#pragma once

#include "chesslib/pos.h"
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
	pos epTarget;
	uint32_t halfMoveClock;
	uint32_t moveNumber;
} board;

board createBoard();
board createBoardFromFen(const char *fen);

void boardSetPiece(board *b, pos p, piece pe);
piece boardGetPiece(board *b, pos p);

moveList *boardGenerateMoves(board *b);

uint8_t boardIsSquareAttacked(board *b, pos p, pieceColor attacker);
uint8_t boardIsInCheck(board *b);
uint8_t boardIsPlayerInCheck(board *b, pieceColor player);

// Returns a new board on which the given move was played on the given board
board boardPlayMove(board *b, move m);

// Returns if two boards are equal in all ways
uint8_t boardEq(board *b1, board *b2);
// Returns if two boards are equal WITHOUT the counters, and filtering the EP target square
uint8_t boardEqContext(board *b1, board *b2);

char *boardGetFen(board *b);
