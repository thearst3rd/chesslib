/*
 * Piece definitions
 * Created by thearst3rd on 8/6/2020
 */

#pragma once

typedef enum
{
	empty,
	pawn,
	knight,
	bishop,
	rook,
	queen,
	king
} pieceType;

typedef enum
{
	pEmpty,
	pWPawn,
	pWKnight,
	pWBishop,
	pWRook,
	pWQueen,
	pWKing,
	pBPawn,
	pBKnight,
	pBBishop,
	pBRook,
	pBQueen,
	pBKing
} piece;

typedef enum
{
	noColor,
	white,
	black
} pieceColor;

pieceType getPieceType(piece p);
pieceColor getPieceColor(piece p);
char getPieceTypeLetter(pieceType p);
char getPieceLetter(piece p);
