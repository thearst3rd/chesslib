/*
 * Move list definitions
 * Created by thearst3rd on 9/09/2020
 */

#pragma once

#include <stddef.h>

#include "chesslib/move.h"

typedef struct _moveListNode
{
	move move;
	struct _moveListNode *next;
} moveListNode;

typedef struct _moveList
{
	moveListNode *head;
	moveListNode *tail;
	size_t size;
} moveList;

// Creates an empty moveList/moveListNode with given move
moveList *moveListCreate();
moveListNode *moveListNodeCreate(move move);

// Move list operations
void moveListAdd(moveList *list, move move);
move moveListGet(moveList *list, unsigned int index);
void moveListUndo(moveList *list);

// Frees the movelist and all nodes
void moveListFree(moveList *list);
