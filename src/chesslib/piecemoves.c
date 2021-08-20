/*
 * Implementations of how pieces move
 * I would want this to be in move.c but I want to avoid circular dependancies...
 * Created by thearst3rd on 9/15/2020
 */

#include "chesslib/piecemoves.h"


//////////////////////
// HELPER FUNCTIONS //
//////////////////////

// Returns false if there is a piece of the same color at square
int canMoveHere(board *b, sq s, pieceColor ourColor)
{
	piece p = boardGetPiece(b, s);
	if (p == pEmpty)
		return 1;
	if (p == pBlocker)
		return 0;

	pieceColor theirColor = pieceGetColor(p);

	return ourColor != theirColor;
}

moveList *pmLeaperMoveList(board *b, sq s, pieceType pt, int8_t dirs[][2], size_t numDirs)
{
	moveList *list = moveListCreate();

	piece p = boardGetPiece(b, s);
	if (pieceGetType(p) != pt)
		return list;

	pieceColor color = pieceGetColor(p);

	sq newSq;
	for (int i = 0; i < numDirs; i++)
	{
		newSq.file = s.file + dirs[i][0];
		newSq.rank = s.rank + dirs[i][1];

		if (newSq.file < 1 || newSq.file > b->width || newSq.rank < 1 || newSq.rank > b->height)
			continue;

		if (canMoveHere(b, newSq, color))
			moveListAdd(list, moveSq(s, newSq));
	}

	return list;
}

moveList *pmRiderMoveList(board *b, sq s, pieceType pt, int8_t dirs[][2], size_t numDirs)
{
	moveList *list = moveListCreate();

	piece p = boardGetPiece(b, s);
	if (pieceGetType(p) != pt)
		return list;

	pieceColor color = pieceGetColor(p);

	sq newSq;
	for (int i = 0; i < numDirs; i++)
	{
		newSq = s;

		while (1)
		{
			newSq.file += dirs[i][0];
			newSq.rank += dirs[i][1];

			if (newSq.file < 1 || newSq.file > b->width || newSq.rank < 1 || newSq.rank > b->height)
				break;

			piece capturePiece = boardGetPiece(b, newSq);

			if (canMoveHere(b, newSq, color))
				moveListAdd(list, moveSq(s, newSq));

			if (capturePiece != pEmpty)
				break;
		}
	}

	return list;
}


//////////
// PAWN //
//////////

// HELPER FUNCTION:
// Same as moveListAdd, but will automatically add promotion moves as well
void addPawnMoveToMoveList(board *b, moveList *list, sq oldSq, sq newSq)
{
	// Should this be a promotion?
	if (newSq.rank == 1 || newSq.rank == b->height)
	{
		moveListAdd(list, movePromote(oldSq, newSq, ptQueen));
		moveListAdd(list, movePromote(oldSq, newSq, ptRook));
		moveListAdd(list, movePromote(oldSq, newSq, ptBishop));
		moveListAdd(list, movePromote(oldSq, newSq, ptKnight));
		// Uncomment for fairy pieces
		/*
		moveListAdd(list, movePromote(oldSq, newSq, ptWazir));
		moveListAdd(list, movePromote(oldSq, newSq, ptMann));
		moveListAdd(list, movePromote(oldSq, newSq, ptArchbishop));
		moveListAdd(list, movePromote(oldSq, newSq, ptChancellor));
		moveListAdd(list, movePromote(oldSq, newSq, ptAmazon));
		*/
	}
	else
	{
		moveListAdd(list, moveSq(oldSq, newSq));
	}
}

moveList *pmGetPawnMoves(board *b, sq s)
{
	moveList *list = moveListCreate();

	piece p = boardGetPiece(b, s);
	if (pieceGetType(p) != ptPawn)
		return list;

	pieceColor color = pieceGetColor(p);
	int delta = color == pcWhite ? 1 : -1;

	// Handle forward moves
	sq newSq = sqI(s.file, s.rank + delta);
	if (boardGetPiece(b, newSq) == pEmpty)
	{
		addPawnMoveToMoveList(b, list, s, newSq);

		// Can this piece move two squares?
		if (color == pcWhite ? (s.rank <= 2) : (s.rank >= (b->height - 1)))
		{
			newSq.rank += delta;
			if (boardGetPiece(b, newSq) == pEmpty)
				addPawnMoveToMoveList(b, list, s, newSq);
		}
	}

	// Handle captures
	piece capturePiece;
	if (s.file > 1)
	{
		newSq = sqI(s.file - 1, s.rank + delta);
		capturePiece = boardGetPiece(b, newSq);
		if (((capturePiece != pEmpty) && canMoveHere(b, newSq, color))
				|| sqEq(newSq, b->epTarget))
			addPawnMoveToMoveList(b, list, s, newSq);
	}

	if (s.file < b->width)
	{
		newSq = sqI(s.file + 1, s.rank + delta);
		capturePiece = boardGetPiece(b, newSq);
		if (((capturePiece != pEmpty) && canMoveHere(b, newSq, color))
				|| sqEq(newSq, b->epTarget))
			addPawnMoveToMoveList(b, list, s, newSq);
	}

	return list;
}

// Pawns are special...
// Note, this is not getting the squares a pawn is CURRENTLY attacking, just where it COULD attack were there a piece
moveList *pmGetPawnAttacks(board *b, sq s)
{
	moveList *list = moveListCreate();

	piece p = boardGetPiece(b, s);
	if (pieceGetType(p) != ptPawn)
		return list;

	pieceColor color = pieceGetColor(p);
	int delta = color == pcWhite ? 1 : -1;

	if (s.file > 1)
	{
		sq newSq = sqI(s.file - 1, s.rank + delta);
		if (canMoveHere(b, newSq, color))
			moveListAdd(list, moveSq(s, newSq));
	}

	if (s.file < b->width)
	{
		sq newSq = sqI(s.file + 1, s.rank + delta);
		if (canMoveHere(b, newSq, color))
			moveListAdd(list, moveSq(s, newSq));
	}

	return list;
}


////////////
// KNIGHT //
////////////

int8_t knightOffsets[8][2] = {{1, 2}, {2, 1}, {2, -1}, {1, -2}, {-1, -2}, {-2, -1}, {-2, 1}, {-1, 2}};

moveList *pmGetKnightMoves(board *b, sq s)
{
	return pmLeaperMoveList(b, s, ptKnight, knightOffsets, 8);
}


////////////
// BISHOP //
////////////

int8_t bishopOffsets[4][2] = {{1, 1}, {1, -1}, {-1, -1}, {-1, 1}};

moveList *pmGetBishopMoves(board *b, sq s)
{
	return pmRiderMoveList(b, s, ptBishop, bishopOffsets, 4);
}


//////////
// ROOK //
//////////

int8_t rookOffsets[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

moveList *pmGetRookMoves(board *b, sq s)
{
	return pmRiderMoveList(b, s, ptRook, rookOffsets, 4);
}


///////////
// QUEEN //
///////////

int8_t royalOffsets[8][2] = {{0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}, {-1, 0}, {-1, 1}};

moveList *pmGetQueenMoves(board *b, sq s)
{
	return pmRiderMoveList(b, s, ptQueen, royalOffsets, 8);
}


//////////
// KING //
//////////

moveList *pmGetKingMoves(board *b, sq s)
{
	return pmLeaperMoveList(b, s, ptKing, royalOffsets, 8);
}


///////////
// FAIRY //
///////////

moveList *pmGetWazirMoves(board *b, sq s)
{
	return pmLeaperMoveList(b, s, ptWazir, rookOffsets, 4);
}
moveList *pmGetMannMoves(board *b, sq s)
{
	return pmLeaperMoveList(b, s, ptMann, royalOffsets, 8);
}
moveList *pmGetArchbishopMoves(board *b, sq s)
{
	moveList *bishopMoves = pmRiderMoveList(b, s, ptArchbishop, bishopOffsets, 4);
	moveList *knightMoves = pmLeaperMoveList(b, s, ptArchbishop, knightOffsets, 8);
	for (moveListNode *n = knightMoves->head; n; n = n->next)
	{
		moveListAdd(bishopMoves, n->move);
	}
	moveListFree(knightMoves);
	return bishopMoves;
}
moveList *pmGetChancellorMoves(board *b, sq s)
{
	moveList *rookMoves = pmRiderMoveList(b, s, ptChancellor, rookOffsets, 4);
	moveList *knightMoves = pmLeaperMoveList(b, s, ptChancellor, knightOffsets, 8);
	for (moveListNode *n = knightMoves->head; n; n = n->next)
	{
		moveListAdd(rookMoves, n->move);
	}
	moveListFree(knightMoves);
	return rookMoves;
}
moveList *pmGetAmazonMoves(board *b, sq s)
{
	moveList *queenMoves = pmRiderMoveList(b, s, ptAmazon, royalOffsets, 8);
	moveList *knightMoves = pmLeaperMoveList(b, s, ptAmazon, knightOffsets, 8);
	for (moveListNode *n = knightMoves->head; n; n = n->next)
	{
		moveListAdd(queenMoves, n->move);
	}
	moveListFree(knightMoves);
	return queenMoves;
}
