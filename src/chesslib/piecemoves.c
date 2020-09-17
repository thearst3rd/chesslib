/*
 * Implementations of how pieces move
 * I would want this to be in move.c but I want to avoid circular dependancies...
 * Created by thearst3rd on 9/15/2020
 */

#include "chesslib/piecemoves.h"

// HELPER FUNCTION:
// Returns false if there is a piece of the same color at pos
int canMoveHere(board *b, pos p, pieceColor ourColor)
{
	piece bPiece = boardGetPiece(b, p);
	if (bPiece == pEmpty)
		return 1;

	pieceColor theirColor = getPieceColor(bPiece);

	return ourColor != theirColor;
}

// HELPER FUNCTION:
// Same as addToMoveList, but will automatically add promotion moves as well
void addPawnMoveToMoveList(moveList *list, pos oldPos, pos newPos)
{
	// Should this be a promotion?
	if (newPos.rank == 1 || newPos.rank == 8)
	{
		addToMoveList(list, movePromote(oldPos, newPos, queen));
		addToMoveList(list, movePromote(oldPos, newPos, rook));
		addToMoveList(list, movePromote(oldPos, newPos, bishop));
		addToMoveList(list, movePromote(oldPos, newPos, knight));
	}
	else
	{
		addToMoveList(list, movePos(oldPos, newPos));
	}
}

moveList *getPawnMoves(board *b, pos p)
{
	moveList *list = createMoveList();

	piece bPiece = boardGetPiece(b, p);
	if (bPiece != pWPawn && bPiece != pBPawn)
		return list;

	pieceColor color = bPiece == pWPawn ? white : black;
	int delta = color == white ? 1 : -1;

	// Handle forward moves
	pos newPos = posI(p.file, p.rank + delta);
	if (boardGetPiece(b, newPos) == pEmpty)
	{
		addPawnMoveToMoveList(list, p, newPos);

		// Can this piece move two squares?
		if (color == white ? (p.rank <= 2) : (p.rank >= 7))
		{
			newPos.rank += delta;
			if (boardGetPiece(b, newPos) == pEmpty)
				addPawnMoveToMoveList(list, p, newPos);
		}
	}

	// Handle captures
	piece capturePiece;
	if (p.file > 1)
	{
		newPos = posI(p.file - 1, p.rank + delta);
		capturePiece = boardGetPiece(b, newPos);
		if (((capturePiece != pEmpty) && canMoveHere(b, newPos, color))
				|| posEq(newPos, b->epTarget))
			addPawnMoveToMoveList(list, p, newPos);
	}

	if (p.file < 8)
	{
		newPos = posI(p.file + 1, p.rank + delta);
		capturePiece = boardGetPiece(b, newPos);
		if (((capturePiece != pEmpty) && canMoveHere(b, newPos, color))
				|| posEq(newPos, b->epTarget))
			addPawnMoveToMoveList(list, p, newPos);
	}

	return list;
}
