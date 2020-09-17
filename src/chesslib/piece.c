/*
 * Piece implementation
 * Created by thearst3rd on 8/6/2020
 */

#include <ctype.h>

#include "chesslib/piece.h"

pieceType getPieceType(piece p)
{
	if (p >= pBPawn)
		p = p - pBPawn + 1;

	return (pieceType) p;
}

pieceColor getPieceColor(piece p)
{
	if (p >= pWPawn && p <= pWKing)
		return white;

	if (p >= pBPawn && p <= pBKing)
		return black;

	return noColor;
}

char getPieceTypeLetter(pieceType p)
{
	switch (p)
	{
		case pawn:
			return 'p';

		case knight:
			return 'n';

		case bishop:
			return 'b';

		case rook:
			return 'r';

		case queen:
			return 'q';

		case king:
			return 'k';

		case empty:
			return ' ';

		default:
			return 0;
	}
}

char getPieceLetter(piece p)
{
	char c = getPieceTypeLetter(getPieceType(p));

	if (p < pBPawn)
		c = toupper(c);

	return c;
}
