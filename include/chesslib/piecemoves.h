/*
 * Definitions for how pieces move
 * I would want this to be in move.h but I want to avoid circular dependancies...
 * Created by thearst3rd on 9/15/2020
 */

#pragma once

#include "chesslib/board.h"
#include "chesslib/movelist.h"

// Each of these functions return a moveList of _potential_ moves, but they might leave the current player in check
moveList *getPawnMoves(board *b, pos p);
moveList *getKnightMoves(board *b, pos p);
moveList *getBishopMoves(board *b, pos p);
moveList *getRookMoves(board *b, pos p);
moveList *getQueenMoves(board *b, pos p);
moveList *getKingMoves(board *b, pos p);
