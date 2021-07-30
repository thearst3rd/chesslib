/*
 * Move implementation
 * Created by thearst3rd on 8/6/2020
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "chesslib/move.h"

move moveSq(sq from, sq to)
{
	return movePromote(from, to, ptEmpty);
}

move movePromote(sq from, sq to, pieceType promotion)
{
	move m;

	m.from = from;
	m.to = to;
	m.promotion = promotion;

	return m;
}

uint8_t moveEq(move m1, move m2)
{
	return sqEq(m1.from, m2.from) && sqEq(m1.to, m2.to) && (m1.promotion == m2.promotion);
}

// Returns the UCI string of the given move. Must be freed.
char *moveGetUci(move m)
{
	char buf[20];
	char p;
	if (m.promotion)
		p = tolower(pieceTypeGetLetter(m.promotion));
	else
		p = '\0';

	char *sqF = sqGetStr(m.from);
	char *sqT = sqGetStr(m.to);
	sprintf(buf, "%s%s%c", sqF, sqT, p);
	free(sqF);
	free(sqT);

	return strdup(buf);
}

// Returns a move from a UCI string
// Note - currently has no error checking...
move moveFromUci(char *uci)
{
	char fromFile;
	int fromRank;
	char toFile;
	int toRank;
	char promotionLetter;

	sscanf(uci, "%c%d%c%d%c", &fromFile, &fromRank, &toFile, &toRank, &promotionLetter);

	pieceType promotion;
	switch (promotionLetter)
	{
		case 'p': 	// Maybe this should not be here...
			promotion = ptPawn;
			break;

		case 'n':
			promotion = ptKnight;
			break;

		case 'b':
			promotion = ptBishop;
			break;

		case 'r':
			promotion = ptRook;
			break;

		case 'q':
			promotion = ptQueen;
			break;

		case 'k': 	// This is here for chess variants
			promotion = ptKing;
			break;

		default:
			promotion = ptEmpty;
			break;
	}

	move m;
	m.from = sqI(fromFile - ('a' - 1), fromRank);
	m.to = sqI(toFile - ('a' - 1), toRank);
	m.promotion = promotion;
	return m;
}
