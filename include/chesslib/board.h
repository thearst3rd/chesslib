/*
 * Board definitions
 * Created by thearst3rd on 8/23/2020
 */

#pragma once

#include "chesslib/pos.h"
#include "chesslib/piece.h"
#include "chesslib/movelist.h"

#define CASTLE_WK 0x0001
#define CASTLE_WQ 0x0010
#define CASTLE_BK 0x0100
#define CASTLE_BQ 0x1000

typedef struct
{
	piece pieces[64];
	pieceColor currentPlayer;
	uint8_t castleState; 	// Bitmask describing castle state
	pos epTarget;
} board;

board createBoard();
board createBoardFromFen(const char *fen);

void boardSetPiece(board *b, pos p, piece pe);
piece boardGetPiece(board *b, pos p);

moveList *boardGenerateMoves(board *b);

uint8_t boardIsSquareAttacked(board *b, pos p, pieceColor attacker);
uint8_t boardIsInCheck(board *b);
uint8_t boardIsPlayerInCheck(board *b, pieceColor player);
