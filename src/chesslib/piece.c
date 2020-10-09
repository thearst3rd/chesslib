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
		return pcWhite;

	if (pe >= pBPawn && pe <= pBKing)
		return pcBlack;

	return pcNoColor;
}

char pieceTypeGetLetter(pieceType pe)
{
	switch (pe)
	{
		case ptPawn:
			return 'P';

		case ptKnight:
			return 'N';

		case ptBishop:
			return 'B';

		case ptRook:
			return 'R';

		case ptQueen:
			return 'Q';

		case ptKing:
			return 'K';

		// TODO - does this make sense?
		case ptEmpty:
			return ' ';

		default:
			return 0;
	}
}

char pieceGetLetter(piece pe)
{
	char c = pieceTypeGetLetter(pieceGetType(pe));

	if (pieceGetColor(pe) == pcBlack)
		c = tolower(c);

	return c;
}

piece pieceMake(pieceType type, pieceColor color)
{
	if ((color != pcWhite) && (color != pcBlack))
	{
		return pEmpty;
	}

	switch(type)
	{
		case ptKing:
			return (color == pcWhite) ? pWKing : pBKing;

		case ptQueen:
			return (color == pcWhite) ? pWQueen : pBQueen;

		case ptRook:
			return (color == pcWhite) ? pWRook : pBRook;

		case ptBishop:
			return (color == pcWhite) ? pWBishop : pBBishop;

		case ptKnight:
			return (color == pcWhite) ? pWKnight : pBKnight;

		case ptPawn:
			return (color == pcWhite) ? pWPawn : pBPawn;

		default:
			return pEmpty;
	}
}
