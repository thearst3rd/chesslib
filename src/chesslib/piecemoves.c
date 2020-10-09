/*
 * Implementations of how pieces move
 * I would want this to be in move.c but I want to avoid circular dependancies...
 * Created by thearst3rd on 9/15/2020
 */

#include "chesslib/piecemoves.h"


//////////////////////
// HELPER FUNCTIONS //
//////////////////////

// Returns false if there is a piece of the same color at pos
int canMoveHere(board *b, pos p, pieceColor ourColor)
{
	piece bPiece = boardGetPiece(b, p);
	if (bPiece == pEmpty)
		return 1;

	pieceColor theirColor = pieceGetColor(bPiece);

	return ourColor != theirColor;
}

moveList *leaperMoveList(board *b, pos p, pieceType pt, int8_t dirs[][2], size_t numDirs)
{
	moveList *list = createMoveList();

	piece bPiece = boardGetPiece(b, p);
	if (pieceGetType(bPiece) != pt)
		return list;

	pieceColor color = pieceGetColor(bPiece);

	pos newPos;
	for (int i = 0; i < numDirs; i++)
	{
		newPos.file = p.file + dirs[i][0];
		newPos.rank = p.rank + dirs[i][1];

		if (newPos.file < 1 || newPos.file > 8 || newPos.rank < 1 || newPos.rank > 8)
			continue;

		if (canMoveHere(b, newPos, color))
			addToMoveList(list, movePos(p, newPos));
	}

	return list;
}

moveList *riderMoveList(board *b, pos p, pieceType pt, int8_t dirs[][2], size_t numDirs)
{
	moveList *list = createMoveList();

	piece bPiece = boardGetPiece(b, p);
	if (pieceGetType(bPiece) != pt)
		return list;

	pieceColor color = pieceGetColor(bPiece);

	pos newPos;
	for (int i = 0; i < numDirs; i++)
	{
		newPos = p;

		while (1)
		{
			newPos.file += dirs[i][0];
			newPos.rank += dirs[i][1];

			if (newPos.file < 1 || newPos.file > 8 || newPos.rank < 1 || newPos.rank > 8)
				break;

			piece capturePiece = boardGetPiece(b, newPos);

			if (canMoveHere(b, newPos, color))
				addToMoveList(list, movePos(p, newPos));

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
// Same as addToMoveList, but will automatically add promotion moves as well
void addPawnMoveToMoveList(moveList *list, pos oldPos, pos newPos)
{
	// Should this be a promotion?
	if (newPos.rank == 1 || newPos.rank == 8)
	{
		addToMoveList(list, movePromote(oldPos, newPos, ptQueen));
		addToMoveList(list, movePromote(oldPos, newPos, ptRook));
		addToMoveList(list, movePromote(oldPos, newPos, ptBishop));
		addToMoveList(list, movePromote(oldPos, newPos, ptKnight));
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
	if (pieceGetType(bPiece) != ptPawn)
		return list;

	pieceColor color = pieceGetColor(bPiece);
	int delta = color == pcWhite ? 1 : -1;

	// Handle forward moves
	pos newPos = posI(p.file, p.rank + delta);
	if (boardGetPiece(b, newPos) == pEmpty)
	{
		addPawnMoveToMoveList(list, p, newPos);

		// Can this piece move two squares?
		if (color == pcWhite ? (p.rank <= 2) : (p.rank >= 7))
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

// Pawns are special...
// Note, this is not getting the squares a pawn is CURRENTLY attacking, just where it COULD attack were there a piece
moveList *getPawnAttacks(board *b, pos p)
{
	moveList *list = createMoveList();

	piece bPiece = boardGetPiece(b, p);
	if (pieceGetType(bPiece) != ptPawn)
		return list;

	pieceColor color = pieceGetColor(bPiece);
	int delta = color == pcWhite ? 1 : -1;

	if (p.file > 1)
	{
		pos newPos = posI(p.file - 1, p.rank + delta);
		if (canMoveHere(b, newPos, color))
			addToMoveList(list, movePos(p, newPos));
	}

	if (p.file < 8)
	{
		pos newPos = posI(p.file + 1, p.rank + delta);
		if (canMoveHere(b, newPos, color))
			addToMoveList(list, movePos(p, newPos));
	}

	return list;
}


////////////
// KNIGHT //
////////////

int8_t knightOffsets[8][2] = {{1, 2}, {2, 1}, {2, -1}, {1, -2}, {-1, -2}, {-2, -1}, {-2, 1}, {-1, 2}};

moveList *getKnightMoves(board *b, pos p)
{
	return leaperMoveList(b, p, ptKnight, knightOffsets, 8);
}


////////////
// BISHOP //
////////////

int8_t bishopOffsets[4][2] = {{1, 1}, {1, -1}, {-1, -1}, {-1, 1}};

moveList *getBishopMoves(board *b, pos p)
{
	return riderMoveList(b, p, ptBishop, bishopOffsets, 4);
}


//////////
// ROOK //
//////////

int8_t rookOffsets[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

moveList *getRookMoves(board *b, pos p)
{
	return riderMoveList(b, p, ptRook, rookOffsets, 4);
}


///////////
// QUEEN //
///////////

int8_t royalOffsets[8][2] = {{0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}, {-1, 0}, {-1, 1}};

moveList *getQueenMoves(board *b, pos p)
{
	return riderMoveList(b, p, ptQueen, royalOffsets, 8);
}


//////////
// KING //
//////////

moveList *getKingMoves(board *b, pos p)
{
	return leaperMoveList(b, p, ptKing, royalOffsets, 8);
}
