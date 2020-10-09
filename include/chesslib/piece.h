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

pieceType pieceGetType(piece p);
pieceColor pieceGetColor(piece p);
char pieceTypeGetLetter(pieceType p);
char pieceGetLetter(piece p);
piece pieceMake(pieceType type, pieceColor color);
