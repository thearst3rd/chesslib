/*
 * Definitions of a game of chess
 * Created by thearst3rd on 9/19/2020
 */

#pragma once

#include "chesslib/board.h"

typedef struct
{
	board *currentBoard;
	board *history; 	// Array of boards
	uint16_t halfMoveClock;
	uint16_t moveNumber;
	uint8_t drawClaimed;
} game;

// Some of these functions mirror the function in board, but with extra conditions
game createGame();
game createGameFromFen(const char *fen);


