/*
 * Move list implementation
 * Created by thearst3rd on 9/09/2020
 */

#include <stdlib.h>

#include "chesslib/movelist.h"

moveList *moveListCreate()
{
	moveList *list = (moveList *) malloc(sizeof(moveList));

	list->head = NULL;
	list->tail = NULL;
	list->size = 0;

	return list;
}

moveListNode *moveListNodeCreate(move move)
{
	moveListNode *node = (moveListNode *) malloc(sizeof(moveListNode));
	node->move = move;
	node->next = NULL;

	return node;
}

void moveListAdd(moveList *list, move move)
{
	moveListNode *node = moveListNodeCreate(move);

	if (list->head == NULL)
	{
		list->head = node;
		list->tail = node;
	}
	else
	{
		moveListNode *prevTail = list->tail;
		prevTail->next = node;
		list->tail = node;
	}
	list->size++;
}

move moveListGet(moveList *list, unsigned int index)
{
	moveListNode *currNode = list->head;
	while (index)
	{
		currNode = currNode->next;
		index--;
	}
	return currNode->move;
}

void moveListFree(moveList *list)
{
	moveListNode *node = list->head;
	while (node)
	{
		moveListNode *next = node->next;
		free(node);
		node = next;
	}
	free(list);
}
