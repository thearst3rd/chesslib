/*
 * Chess game implementation
 * Created by thearst3rd on 10/24/2020
 */

#include "chesslib/chessgame.h"
#include "chesslib/move.h"

void chessGameInit(chessGame *g)
{
	chessGameInitFromFen(g, INITIAL_FEN);
}

uint8_t chessGameInitFromFen(chessGame *g, const char *fen)
{
	g->boardHistory = boardListCreate();

	board b;
	if (boardInitFromFen(&b, fen))
	{
		boardListFree(g->boardHistory);
		chessGameInit(g);
		return 1;
	}

	boardListAdd(g->boardHistory, b);

	g->currentLegalMoves = NULL;
	g->repetitions = 1;
	g->terminal = tsOngoing;

	// Calculates repetitions and terminal state
	chessGameCalculateFields(g);

	return 0;
}

void chessGameFreeComponents(chessGame *g)
{
	boardListFree(g->boardHistory);
	moveListFree(g->currentLegalMoves);
}

board chessGameGetCurrentBoard(chessGame *g)
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

	board oldBoard = chessGameGetCurrentBoard(g);
	board newBoard = boardPlayMove(&oldBoard, m);

	boardListAdd(g->boardHistory, newBoard);

	chessGameCalculateFields(g);

	return 0;
}

uint8_t chessGameUndo(chessGame *g)
{
	// If there's only one board (so, no moves), return failure
	if (g->boardHistory->head == g->boardHistory->tail)
		return 1;

	boardListUndo(g->boardHistory);

	if (g->terminal == tsDrawClaimed50MoveRule || g->terminal == tsDrawClaimedThreefold)
		g->terminal = tsOngoing;

	chessGameCalculateFields(g);

	return 0;
}

void chessGameCalculateFields(chessGame *g)
{
	board currentBoard = chessGameGetCurrentBoard(g);

	g->repetitions = 0;
	for (boardListNode *n = g->boardHistory->head; n; n = n->next)
	{
		if (boardEqContext(&currentBoard, &(n->board)))
			g->repetitions++;
	}

	if (g->currentLegalMoves)
		moveListFree(g->currentLegalMoves);

	g->currentLegalMoves = boardGenerateMoves(&currentBoard);

	if (g->currentLegalMoves->size == 0)
	{
		if (boardIsInCheck(&currentBoard))
			g->terminal = tsCheckmate;
		else
			g->terminal = tsDrawStalemate;
	}
	else
	{
		if (g->repetitions >= 5)
			g->terminal = tsDrawFivefold;
		else if (currentBoard.halfMoveClock >= 150)
			g->terminal = tsDraw75MoveRule;
		else if ((g->terminal != tsDrawClaimed50MoveRule) && (g->terminal != tsDrawClaimedThreefold))
			g->terminal = tsOngoing;
	}
}
