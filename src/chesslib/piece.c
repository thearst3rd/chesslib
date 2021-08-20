/*
 * Piece implementation
 * Created by thearst3rd on 8/6/2020
 */

#include <ctype.h>

#include "chesslib/piece.h"

pieceType pieceGetType(piece p)
{
	if (p >= pBPawn)
		p = p - NUM_PIECE_TYPES;

	return (pieceType) p;
}

pieceColor pieceGetColor(piece p)
{
	if (p >= pWPawn && p < pWPawn + NUM_PIECE_TYPES)
		return pcWhite;

	if (p >= pBPawn && p < pBPawn + NUM_PIECE_TYPES)
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

		case ptWazir:
			return 'W';

		case ptMann:
			return 'M';

		case ptArchbishop:
			return 'H';

		case ptChancellor:
			return 'C';

		case ptAmazon:
			return 'A';

		// TODO - do these make sense?
		case ptEmpty:
			return ' ';

		case ptBlocker:
			return '#';

		default:
			return 0;
	}
}

char pieceGetLetter(piece p)
{
	char c = pieceTypeGetLetter(pieceGetType(p));

	if (pieceGetColor(p) == pcBlack)
		c = tolower(c);

	return c;
}

piece pieceMake(pieceType type, pieceColor color)
{
	if ((color != pcWhite && color != pcBlack) ||
			(type < ptPawn || type >= (ptPawn + NUM_PIECE_TYPES)))
	{
		return type == ptBlocker ? pBlocker : pEmpty;
	}

	return (piece) (type + (color == pcBlack ? NUM_PIECE_TYPES : 0));
}
