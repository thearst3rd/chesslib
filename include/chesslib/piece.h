/*
 * Piece definitions
 * Created by thearst3rd on 8/6/2020
 */

#pragma once

// How many types of pieces have we programmed?
#define NUM_PIECE_TYPES 11
#define NUM_PIECES (2 * NUM_PIECE_TYPES)

typedef enum
{
	ptEmpty,	// empty square, can be inhabited
	ptBlocker,	// blocked square, cannot be inhabited
	ptPawn,
	ptKnight,
	ptBishop,
	ptRook,
	ptQueen,
	ptKing,
	ptWazir,
	ptMann,
	ptArchbishop,
	ptChancellor,
	ptAmazon
} pieceType;

typedef enum
{
	pEmpty,
	pBlocker,
	pWPawn,
	pWKnight,
	pWBishop,
	pWRook,
	pWQueen,
	pWKing,
	pWWazir,
	pWMann,
	pWArchbishop,
	pWChancellor,
	pWAmazon,
	pBPawn,
	pBKnight,
	pBBishop,
	pBRook,
	pBQueen,
	pBKing,
	pBWazir,
	pBMann,
	pBArchbishop,
	pBChancellor,
	pBAmazon
} piece;

typedef enum
{
	pcNoColor,
	pcWhite,
	pcBlack
} pieceColor;

pieceType pieceGetType(piece p);
pieceColor pieceGetColor(piece p);
char pieceTypeGetLetter(pieceType p);
char pieceGetLetter(piece p);
piece pieceMake(pieceType type, pieceColor color);
