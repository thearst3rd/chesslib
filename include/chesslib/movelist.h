/*
 * Move list definitions
 * Created by thearst3rd on 9/09/2020
 */

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
moveList *createMoveList();
moveListNode *createMoveListNode(move move);

// Move list operations
void addToMoveList(moveList *list, move move);
move getFromMoveList(moveList *list, unsigned int index);

// Frees the movelist and all nodes
void freeMoveList(moveList *list);
