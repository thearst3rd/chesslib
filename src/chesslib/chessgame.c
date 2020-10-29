/*
 * Chess game implementation
 * Created by thearst3rd on 10/24/2020
 */

#include <stdlib.h>

#include "chesslib/chessgame.h"
#include "chesslib/move.h"

chessGame *chessGameCreate()
{
	return chessGameCreateFromFen(INITIAL_FEN);
}

chessGame *chessGameCreateFromFen(const char *fen)
{
	chessGame *g = (chessGame *) malloc(sizeof(chessGame));

	if (chessGameInitFromFenInPlace(g, fen))
	{
		free(g);
		return NULL;
	}

	return g;
}

void chessGameInitInPlace(chessGame *g)
{
	chessGameInitFromFenInPlace(g, INITIAL_FEN);
}

uint8_t chessGameInitFromFenInPlace(chessGame *g, const char *fen)
{
	board *b = boardCreateFromFen(fen);
	if (b == NULL)
		return 1;

	g->boardHistory = boardListCreate();
	boardListAdd(g->boardHistory, b);

	g->moveHistory = moveListCreate();

	g->currentLegalMoves = NULL;
	g->repetitions = 1;
	g->terminal = tsOngoing;

	// Calculates repetitions and terminal state
	chessGameCalculateFields(g);

	return 0;
}

void chessGameFree(chessGame *g)
{
	boardListFree(g->boardHistory);
	moveListFree(g->moveHistory);
	moveListFree(g->currentLegalMoves);
	free(g);
}

board *chessGameGetCurrentBoard(chessGame *g)
{
	return g->boardHistory->tail->board;
}

uint8_t chessGamePlayMove(chessGame *g, move m)
{
	if (g->terminal != tsOngoing)
		return 1;

	uint8_t found = 0;

	for (moveListNode *n = g->currentLegalMoves->head; n; n = n->next)
	{
		if (moveEq(m, n->move))
		{
			found = 1;
			break;
		}
	}

	if (!found)
		return 1;

	board *newBoard = boardPlayMove(chessGameGetCurrentBoard(g), m);

	boardListAdd(g->boardHistory, newBoard);
	moveListAdd(g->moveHistory, m);

	chessGameCalculateFields(g);

	return 0;
}

uint8_t chessGameUndo(chessGame *g)
{
	// If there's only one board (so, no moves), return failure
	if (g->boardHistory->head == g->boardHistory->tail)
		return 1;

	boardListUndo(g->boardHistory);
	moveListUndo(g->moveHistory);

	if (g->terminal == tsDrawClaimed50MoveRule || g->terminal == tsDrawClaimedThreefold)
		g->terminal = tsOngoing;

	chessGameCalculateFields(g);

	return 0;
}

void chessGameCalculateFields(chessGame *g)
{
	board *currentBoard = chessGameGetCurrentBoard(g);

	g->repetitions = 0;
	for (boardListNode *n = g->boardHistory->head; n; n = n->next)
	{
		if (boardEqContext(currentBoard, n->board))
			g->repetitions++;
	}

	if (g->currentLegalMoves)
		moveListFree(g->currentLegalMoves);

	g->currentLegalMoves = boardGenerateMoves(currentBoard);

	if (g->currentLegalMoves->size == 0)
	{
		if (boardIsInCheck(currentBoard))
			g->terminal = tsCheckmate;
		else
			g->terminal = tsDrawStalemate;
	}
	else
	{
		if (g->repetitions >= 5)
			g->terminal = tsDrawFivefold;
		else if (currentBoard->halfMoveClock >= 150)
			g->terminal = tsDraw75MoveRule;
		else if (boardIsInsufficientMaterial(currentBoard))
			g->terminal = tsDrawInsufficient;
		else if ((g->terminal != tsDrawClaimed50MoveRule) && (g->terminal != tsDrawClaimedThreefold))
			g->terminal = tsOngoing;
	}
}
