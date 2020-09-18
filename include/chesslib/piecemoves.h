/*
 * Definitions for how pieces move
 * I would want this to be in move.h but I want to avoid circular dependancies...
 * Created by thearst3rd on 9/15/2020
 */

#pragma once

#include "chesslib/board.h"
#include "chesslib/movelist.h"

// Returns a list for a generic piece type

// Leapers can leap to a square with the given offsets
// dirs is an array of tuples that define all offests to where the leaper can move
// For example: the knights array would be:
// 		dirs = {{1, 2}, {2, 1}, {2, -1}, {1, -2}, {-1, -2}, {-2, -1}, {-2, 1}, {-1, 2}}
// 		numDirs = 8
moveList *leaperMoveList(board *b, pos p, pieceType pt, int8_t dirs[][2], size_t numDirs);
// A rider is a leaper that can keep moving in a fixed direction any number of times until it hits something
moveList *riderMoveList(board *b, pos p, pieceType pt, int8_t dirs[][2], size_t numDirs);

// Each of these functions return a moveList of _potential_ moves, but they might leave the current player in check
moveList *getPawnMoves(board *b, pos p);
moveList *getKnightMoves(board *b, pos p);
moveList *getBishopMoves(board *b, pos p);
moveList *getRookMoves(board *b, pos p);
moveList *getQueenMoves(board *b, pos p);
moveList *getKingMoves(board *b, pos p);
