/*
 * Piece implementation
 * Created by thearst3rd on 8/6/2020
 */

#include <ctype.h>

#include "chesslib/piece.h"

pieceType pieceGetType(piece pe)
{
	if (pe >= pBPawn)
		pe = pe - pBPawn + 1;

	return (pieceType) pe;
}

pieceColor pieceGetColor(piece pe)
{
	if (pe >= pWPawn && pe <= pWKing)
		return white;

	if (pe >= pBPawn && pe <= pBKing)
		return black;

	return noColor;
}

char pieceTypeGetLetter(pieceType pe)
{
	switch (pe)
	{
		case pawn:
			return 'P';

		case knight:
			return 'N';

		case bishop:
			return 'B';

		case rook:
			return 'R';

		case queen:
			return 'Q';

		case king:
			return 'K';

		// TODO - does this make sense?
		case empty:
			return ' ';

		default:
			return 0;
	}
}

char pieceGetLetter(piece pe)
{
	char c = pieceTypeGetLetter(pieceGetType(pe));

	if (pieceGetColor(pe) == black)
		c = tolower(c);

	return c;
}

piece pieceMake(pieceType type, pieceColor color)
{
	if ((color != white) && (color != black))
	{
		return pEmpty;
	}

	switch(type)
	{
		case king:
			return (color == white) ? pWKing : pBKing;

		case queen:
			return (color == white) ? pWQueen : pBQueen;

		case rook:
			return (color == white) ? pWRook : pBRook;

		case bishop:
			return (color == white) ? pWBishop : pBBishop;

		case knight:
			return (color == white) ? pWKnight : pBKnight;

		case pawn:
			return (color == white) ? pWPawn : pBPawn;

		default:
			return pEmpty;
	}
}
