/*
 * Chess game implementation
 * Created by thearst3rd on 10/24/2020
 */

#include <stdlib.h>

#include "chesslib/chess.h"
#include "chesslib/move.h"

chess *chessCreate()
{
	return chessCreateFen(INITIAL_FEN);
}

chess *chessCreateFen(const char *fen)
{
	chess *g = (chess *) malloc(sizeof(chess));

	if (chessInitFenInPlace(g, fen))
	{
		free(g);
		return NULL;
	}

	return g;
}

void chessInitInPlace(chess *g)
{
	chessInitFenInPlace(g, INITIAL_FEN);
}

uint8_t chessInitFenInPlace(chess *g, const char *fen)
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
	chessCalculateFields(g);

	return 0;
}

void chessFree(chess *g)
{
	boardListFree(g->boardHistory);
	moveListFree(g->moveHistory);
	moveListFree(g->currentLegalMoves);
	free(g);
}

board *chessGetBoard(chess *g)
{
	return g->boardHistory->tail->board;
}

uint8_t chessPlayMove(chess *g, move m)
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

	board *newBoard = boardPlayMove(chessGetBoard(g), m);

	boardListAdd(g->boardHistory, newBoard);
	moveListAdd(g->moveHistory, m);

	chessCalculateFields(g);

	return 0;
}

uint8_t chessUndo(chess *g)
{
	// If there's only one board (so, no moves), return failure
	if (g->boardHistory->head == g->boardHistory->tail)
		return 1;

	boardListUndo(g->boardHistory);
	moveListUndo(g->moveHistory);

	if (g->terminal == tsDrawClaimed50MoveRule || g->terminal == tsDrawClaimedThreefold)
		g->terminal = tsOngoing;

	chessCalculateFields(g);

	return 0;
}

// Functions that mirror the board struct
piece chessGetPiece(chess *g, sq s)
{
	return boardGetPiece(chessGetBoard(g), s);
}

pieceColor chessGetPlayer(chess *g)
{
	return chessGetBoard(g)->currentPlayer;
}

uint8_t chessGetCastleState(chess *g)
{
	return chessGetBoard(g)->castleState;
}

sq chessGetEpTarget(chess *g)
{
	return chessGetBoard(g)->epTarget;
}

uint32_t chessGetHalfMoveClock(chess *g)
{
	return chessGetBoard(g)->halfMoveClock;
}

uint32_t chessGetMoveNumber(chess *g)
{
	return chessGetBoard(g)->moveNumber;
}

uint8_t chessIsInCheck(chess *g)
{
	return boardIsInCheck(chessGetBoard(g));
}

// TODO, refactor to use sqSet attacked instead
uint8_t chessIsSquareAttacked(chess *g, sq s)
{
	board *b = chessGetBoard(g);
	return boardIsSquareAttacked(b, s, b->currentPlayer == pcWhite ? pcBlack : pcWhite);
}

char *chessGetFen(chess *g)
{
	return boardGetFen(chessGetBoard(g));
}

void chessCalculateFields(chess *g)
{
	board *currentBoard = chessGetBoard(g);

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
