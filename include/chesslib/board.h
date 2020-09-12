/*
 * Board definitions
 * Created by thearst3rd on 8/23/2020
 */

#pragma once

#include "chesslib/pos.h"
#include "chesslib/piece.h"

#define CASTLE_WK 0x0001
#define CASTLE_WQ 0x0010
#define CASTLE_BK 0x0100
#define CASTLE_BQ 0x1000

typedef struct
{
	piece pieces[64];
	uint8_t blackToPlay;
	uint8_t castleState; 	// Bitmask describing castle state
	pos epTarget;
	uint32_t halfMoveClock;
	uint32_t moveNumber;
} board;

board createBoard();
board createBoardFromFen(const char *fen);

void boardSetPiece(board *board, pos pos, piece p);
piece boardGetPiece(board *board, pos pos);
