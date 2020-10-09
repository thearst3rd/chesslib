/*
 * Board implementation
 * Created by thearst3rd on 8/23/2020
 */

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "chesslib/board.h"
#include "chesslib/piecemoves.h"

board boardCreate()
{
	return boardCreateFromFen(INITIAL_FEN);
}

board boardCreateFromFen(const char *fen)
{
	pos currPos = posI(1, 8);

	char c;

	board board;

	// Read in piece positions

	while ((c = *fen))
	{
		if (c == ' ')
		{
			break;
		}
		else if (c >= '1' && c <= '8')
		{
			int num = c - '0';
			if (currPos.file + num > 9)
			{
				fprintf(stderr, "ERROR IN FEN: Spacer put file over the end\n");
				return board;
			}
			for (int i = 0; i < num; i++)
			{
				boardSetPiece(&board, currPos, pEmpty);
				currPos.file++;
			}
		}
		else if (c == '/')
		{
			if (currPos.file != 9)
			{
				fprintf(stderr, "ERROR IN FEN: Found '/' at wrong position in rank\n");
				return board;
			}
			if (currPos.rank <= 1)
			{
				fprintf(stderr, "ERROR IN FEN: Found '/' after the last rank\n");
				return board;
			}
			currPos.file = 1;
			currPos.rank--;
		}
		else
		{
			if (currPos.file > 8)
			{
				fprintf(stderr, "ERROR IN FEN: Found character '%c' after the end of a rank\n", c);
				return board;
			}

			char lc = tolower(c);
			int isBlack = islower(c);

			switch (lc)
			{
				case 'p':
					boardSetPiece(&board, currPos, isBlack ? pBPawn : pWPawn);
					break;

				case 'n':
					boardSetPiece(&board, currPos, isBlack ? pBKnight : pWKnight);
					break;

				case 'b':
					boardSetPiece(&board, currPos, isBlack ? pBBishop : pWBishop);
					break;

				case 'r':
					boardSetPiece(&board, currPos, isBlack ? pBRook : pWRook);
					break;

				case 'q':
					boardSetPiece(&board, currPos, isBlack ? pBQueen : pWQueen);
					break;

				case 'k':
					boardSetPiece(&board, currPos, isBlack ? pBKing : pWKing);
					break;

				default:
					fprintf(stderr, "ERROR IN FEN: Unknown character '%c'\n", c);
					return board;
			}

			currPos.file++;
		}

		fen++;
	}

	if ((currPos.rank != 1) || (currPos.file != 9))
	{
		fprintf(stderr, "ERROR IN FEN: Ended prematurely\n");
		return board;
	}

	fen++; 	// Go past the last space

	// Read in whose turn it is

	c = *fen;
	switch (c)
	{
		case 'w':
			board.currentPlayer = white;
			break;
		case 'b':
			board.currentPlayer = black;
			break;
		default:
			fprintf(stderr, "ERROR IN FEN: Expected 'w' or 'b', found '%c'\n", c);
			return board;
	}
	fen++;

	c = *fen;
	if (c != ' ')
	{
		fprintf(stderr, "ERROR IN FEN: Expected ' ' after w/b, found '%c'\n", c);
		return board;
	}
	fen++;

	// Read in castling state
	// NOTE: technically this can match abnormal sequences, but it will match all correct sequences
	// Ex: 'kqqqQQ--k' will function the same as 'Qkq'. This is fine by me

	board.castleState = 0;

	c = *fen;
	while ((c = *fen))
	{
		if (c == ' ')
			break;

		switch (c)
		{
			case '-':
				// Nothing needs to be done
				break;

			case 'K':
				board.castleState |= CASTLE_WK;
				break;

			case 'Q':
				board.castleState |= CASTLE_WQ;
				break;

			case 'k':
				board.castleState |= CASTLE_BK;
				break;

			case 'q':
				board.castleState |= CASTLE_BQ;
				break;

			default:
				fprintf(stderr, "ERROR IN FEN: Expected K/Q/k/q or -, found '%c'\n", c);
				return board;
		}

		fen++;
	}

	fen++; 	// Go past the last space

	// Read in the en passant target square

	c = *fen;
	if (c == '-')
	{
		board.epTarget = POS_INVALID;
		fen++;
	}
	else
	{
		char epTargetStr[3] = {fen[0], fen[1], 0};
		pos epTarget = posS(epTargetStr);
		if (epTarget.file == -1) 	// It's invalid
		{
			fprintf(stderr, "ERROR IN FEN: Invalid EP target square \"%s\"\n", epTargetStr);
			return board;
		}
		board.epTarget = epTarget;
		fen += 2;
	}

	c = *fen;
	if (c != ' ')
	{
		fprintf(stderr, "ERROR IN FEN: Expected ' ' after EP target square, found '%c'\n", c);
		return board;
	}
	fen++;

	// Read in half move clock and full move count
	// Note: more error checking should probably be done here

	sscanf(fen, "%u %u", &board.halfMoveClock, &board.moveNumber);

	return board;
}

void boardSetPiece(board *b, pos p, piece pe)
{
	int index = posGetIndex(p);
	b->pieces[index] = pe;
}

piece boardGetPiece(board *b, pos p)
{
	int index = posGetIndex(p);
	return b->pieces[index];
}

// Generates a list of all legal moves. This list must be freed with freeMoveList
moveList *boardGenerateMoves(board *b)
{
	moveList *list = createMoveList();

	for (int i = 0; i < 64; i++)
	{
		pos p = posIndex(i);
		piece pe = boardGetPiece(b, p);

		if (pieceGetColor(pe) != b->currentPlayer)
			continue;

		pieceType type = pieceGetType(pe);
		moveList *currMoves = NULL;

		switch (type)
		{
			case pawn:
				currMoves = getPawnMoves(b, p);
				break;

			case knight:
				currMoves = getKnightMoves(b, p);
				break;

			case bishop:
				currMoves = getBishopMoves(b, p);
				break;

			case rook:
				currMoves = getRookMoves(b, p);
				break;

			case queen:
				currMoves = getQueenMoves(b, p);
				break;

			case king:
				currMoves = getKingMoves(b, p);
				break;

			default:
				// Nothing to do
				break;
		}

		if (currMoves)
		{
			for (moveListNode *n = currMoves->head; n; n = n->next)
			{
				move m = n->move;
				board bCheck = boardPlayMove(b, m);
				if (!boardIsPlayerInCheck(&bCheck, b->currentPlayer))
					addToMoveList(list, m);
			}
			freeMoveList(currMoves);
		}
	}

	// Can we castle?
	uint8_t castleOO = b->currentPlayer == white ? (b->castleState & CASTLE_WK) : (b->castleState & CASTLE_BK);
	uint8_t castleOOO = b->currentPlayer == white ? (b->castleState & CASTLE_WQ) : (b->castleState & CASTLE_BQ);

	if (castleOO || castleOOO)
	{
		uint8_t castleRank = b->currentPlayer == white ? 1 : 8;
		pieceColor attacker = b->currentPlayer == white ? black : white;

		piece ourKing = b->currentPlayer == white ? pWKing : pBKing;
		piece ourRook = b->currentPlayer == white ? pWRook : pBRook;

		if (castleOO)
		{
			if (boardGetPiece(b, posI(5, castleRank)) == ourKing
					&& boardGetPiece(b, posI(8, castleRank)) == ourRook
					&& boardGetPiece(b, posI(6, castleRank)) == pEmpty
					&& boardGetPiece(b, posI(7, castleRank)) == pEmpty
					&& !boardIsSquareAttacked(b, posI(5, castleRank), attacker)
					&& !boardIsSquareAttacked(b, posI(6, castleRank), attacker)
					&& !boardIsSquareAttacked(b, posI(7, castleRank), attacker))
				addToMoveList(list, movePos(posI(5, castleRank), posI(7, castleRank)));
		}

		if (castleOOO)
		{
			if (boardGetPiece(b, posI(5, castleRank)) == ourKing
					&& boardGetPiece(b, posI(1, castleRank)) == ourRook
					&& boardGetPiece(b, posI(4, castleRank)) == pEmpty
					&& boardGetPiece(b, posI(3, castleRank)) == pEmpty
					&& boardGetPiece(b, posI(2, castleRank)) == pEmpty
					&& !boardIsSquareAttacked(b, posI(5, castleRank), attacker)
					&& !boardIsSquareAttacked(b, posI(4, castleRank), attacker)
					&& !boardIsSquareAttacked(b, posI(3, castleRank), attacker))
				addToMoveList(list, movePos(posI(5, castleRank), posI(3, castleRank)));
		}
	}

	return list;
}

uint8_t boardIsSquareAttacked(board *b, pos p, pieceColor attacker)
{
	for (int i = 0; i < 64; i++)
	{
		pos attackerP = posIndex(i);
		piece pe = boardGetPiece(b, attackerP);

		if (pieceGetColor(pe) != attacker)
			continue;

		pieceType type = pieceGetType(pe);
		moveList *currMoves = NULL;

		uint8_t found = 0;

		switch (type)
		{
			case pawn:
				currMoves = getPawnAttacks(b, attackerP);
				break;

			case knight:
				currMoves = getKnightMoves(b, attackerP);
				break;

			case bishop:
				currMoves = getBishopMoves(b, attackerP);
				break;

			case rook:
				currMoves = getRookMoves(b, attackerP);
				break;

			case queen:
				currMoves = getQueenMoves(b, attackerP);
				break;

			case king:
				currMoves = getKingMoves(b, attackerP);
				break;

			default:
				// Nothing to do
				break;
		}

		if (currMoves)
		{
			for (moveListNode *n = currMoves->head; n; n = n->next)
			{
				if (posEq(p, n->move.to))
				{
					found = 1;
					break;
				}
			}
			freeMoveList(currMoves);
			if (found)
				return 1;
		}
	}
	return 0;
}

uint8_t boardIsInCheck(board *b)
{
	return boardIsPlayerInCheck(b, b->currentPlayer);
}

uint8_t boardIsPlayerInCheck(board *b, pieceColor player)
{
	piece royalPiece = (player == white) ? pWKing : pBKing;
	pieceColor otherColor = (player == white) ? black : white;
	for (int i = 0; i < 64; i++)
	{
		pos p = posIndex(i);
		if (boardGetPiece(b, p) == royalPiece)
		{
			if (boardIsSquareAttacked(b, p, otherColor))
				return 1;
		}
	}
	return 0;
}

// Returns a new board on which the given move was played on the given board
// NOTE - this assumes that the move is legal!
board boardPlayMove(board *b, move m)
{
	board newBoard = *b;

	newBoard.currentPlayer = (b->currentPlayer == white) ? black : white;
	newBoard.castleState = b->castleState;

	// CONTEXTUALIZE MOVE - is this a special move of sorts?
	// Is this a castling move?
	pieceType pt = pieceGetType(boardGetPiece(b, m.from));
	if (pt == king)
	{
		// TODO - maybe redesign this to work better with Chess 960, though I don't like Fischer castling >:(
		int8_t diffFile = m.to.file - m.from.file;
		if (diffFile == 2) 	// O-O
		{
			// Move rook from h file to correct file
			boardSetPiece(&newBoard, posI(8, m.to.rank), pEmpty);
			boardSetPiece(&newBoard, posI(m.to.file - 1, m.to.rank), b->currentPlayer == white ? pWRook : pBRook);
		}
		else if (diffFile == -2) 	// O-O-O
		{
			// Move rook from a file to correct file
			boardSetPiece(&newBoard, posI(1, m.to.rank), pEmpty);
			boardSetPiece(&newBoard, posI(m.to.file + 1, m.to.rank), b->currentPlayer == white ? pWRook : pBRook);
		}
		newBoard.castleState &= ~((b->currentPlayer == white) ? (CASTLE_WK | CASTLE_WQ) : (CASTLE_BK | CASTLE_BQ));
	}
	else if (pt == rook)
	{
		if (posEq(m.from, posI(1, b->currentPlayer == white ? 1 : 8)))
			newBoard.castleState &= ~((b->currentPlayer == white) ? CASTLE_WQ : CASTLE_BQ);
		else if (posEq(m.from, posI(8, b->currentPlayer == white ? 1 : 8)))
			newBoard.castleState &= ~((b->currentPlayer == white) ? CASTLE_WK : CASTLE_BK);
	}

	// Is this an en passant capture?
	if (pt == pawn && posEq(b->epTarget, m.to))
	{
		// Remove the captured pawn
		uint8_t delta = (b->currentPlayer == white) ? -1 : 1;
		boardSetPiece(&newBoard, posI(m.to.file, m.to.rank + delta), pEmpty);
	}

	// Move the piece
	boardSetPiece(&newBoard, m.to, m.promotion == empty ? boardGetPiece(b, m.from) : pieceMake(m.promotion, b->currentPlayer));
	boardSetPiece(&newBoard, m.from, pEmpty);

	// Should this set the EP target square?
	int8_t diffRank = m.to.rank - m.from.rank;
	if (pt == pawn && ((diffRank == 2) || (diffRank == -2)))
	{
		int8_t delta = (b->currentPlayer == white) ? -1 : 1;
		newBoard.epTarget = posI(m.to.file, m.to.rank + delta);
	}
	else
	{
		newBoard.epTarget = POS_INVALID;
	}

	// Update the counters

	// Was this an irreversable move?
	if (pt == pawn || (boardGetPiece(b, m.to) != pEmpty))
		newBoard.halfMoveClock = 0;
	else
		newBoard.halfMoveClock = b->halfMoveClock + 1;

	// Is this the end of a move?
	newBoard.moveNumber = b->moveNumber;
	if (b->currentPlayer == black)
		newBoard.moveNumber++;

	return newBoard;
}

// Board equality - returns true if boards are fully equal
uint8_t boardEq(board *b1, board *b2)
{
	if (memcmp(b1, b2, 64 * sizeof(piece)))
		return 0;

	if (b1->currentPlayer != b2->currentPlayer)
		return 0;

	if (b1->castleState != b2->castleState)
		return 0;

	if (!posEq(b1->epTarget, b2->epTarget))
		return 0;

	if (b1->halfMoveClock != b2->halfMoveClock)
		return 0;

	if (b1->moveNumber != b2->moveNumber)
		return 0;

	return 1;
}

// Contextual board equality - doesn't consider counters, and filters out EP target square
uint8_t boardEqContext(board *b1, board *b2)
{
	if (memcmp(b1, b2, 64 * sizeof(piece)))
		return 0;

	if (b1->currentPlayer != b2->currentPlayer)
		return 0;

	if (b1->castleState != b2->castleState)
		return 0;

	// Filter EP target squares
	pos b1EpTarget = b1->epTarget;
	if (!posEq(b1EpTarget, POS_INVALID))
	{
		// Are there actually any pawns that can attack this square? If not, make it POS_INVALID
		int delta = b1->currentPlayer == white ? -1 : 1;
		int pe = b1->currentPlayer == white ? pBPawn : pWPawn;
		if (boardGetPiece(b1, posI(b1EpTarget.file - 1, b1EpTarget.rank + delta)) != pe
				&& boardGetPiece(b1, posI(b1EpTarget.file + 1, b1EpTarget.rank + delta)) != pe)
			b1EpTarget = POS_INVALID;
	}

	pos b2EpTarget = b2->epTarget;
	if (!posEq(b2EpTarget, POS_INVALID))
	{
		// Are there actually any pawns that can attack this square? If not, make it POS_INVALID
		int delta = b2->currentPlayer == white ? -1 : 1;
		int pe = b2->currentPlayer == white ? pBPawn : pWPawn;
		if (boardGetPiece(b2, posI(b2EpTarget.file - 1, b2EpTarget.rank + delta)) != pe
				&& boardGetPiece(b2, posI(b2EpTarget.file + 1, b2EpTarget.rank + delta)) != pe)
			b2EpTarget = POS_INVALID;
	}

	if (!posEq(b1EpTarget, b2EpTarget))
		return 0;

	return 1;
}

// Returns a FEN string from the given board. Must be freed
char *boardGetFen(board *b)
{
	char buf[104];
	char *c = buf;

	// Pieces
	for (uint8_t rank = 8; rank >= 1; rank--)
	{
		uint8_t blanks = 0;
		for (uint8_t file = 1; file <= 8; file++)
		{
			pos p = posI(file, rank);
			piece pe = boardGetPiece(b, p);
			if (pe)
			{
				if (blanks > 0)
				{
					*c++ = '0' + blanks;
					blanks = 0;
				}
				*c++ = pieceGetLetter(pe);
			}
			else
			{
				blanks++;
			}
		}

		if (blanks > 0)
		{
			*c++ = '0' + blanks;
		}

		if (rank > 1)
		{
			*c++ = '/';
		}
	}

	*c++ = ' ';

	// Current player
	if (b->currentPlayer == white)
		*c++ = 'w';
	else
		*c++ = 'b';

	*c++ = ' ';

	// Castling state
	if (b->castleState == 0)
	{
		*c++ = '-';
	}
	else
	{
		if (b->castleState & CASTLE_WK)
			*c++ = 'K';
		if (b->castleState & CASTLE_WQ)
			*c++ = 'Q';
		if (b->castleState & CASTLE_BK)
			*c++ = 'k';
		if (b->castleState & CASTLE_BQ)
			*c++ = 'q';
	}

	*c++ = ' ';

	// EP target
	if (posEq(b->epTarget, POS_INVALID))
	{
		*c++ = '-';
	}
	else
	{
		const char *ep = posGetStr(b->epTarget);
		*c++ = ep[0];
		*c++ = ep[1];
	}

	*c++ = ' ';

	// Fill in the rest of the string with half move clock and move number
	sprintf(c, "%u %u", b->halfMoveClock, b->moveNumber);

	size_t len = strlen(buf);
	char *str = (char *) malloc((len + 1) * sizeof(char));
	strcpy(str, buf);
	return str;
}
