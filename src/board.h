/*
 * Board definitions
 * Created by thearst3rd on 8/23/2020
 */

#pragma once

#include "pos.h"
#include "piece.h"

typedef struct
{
	piece pieces[64];
	uint8_t blackToPlay;
	uint8_t castleWK;
	uint8_t castleWQ;
	uint8_t castleBK;
	uint8_t castleBQ;
	pos enPassantTarget;
	uint32_t halfMoveClock;
	uint32_t moveNumber;
} board;

void boardSetPiece(board board, pos pos, piece p);
piece boardGetPiece(board board, pos pos);
