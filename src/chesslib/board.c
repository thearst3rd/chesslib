/*
 * Board implementation
 * Created by thearst3rd on 8/23/2020
 */

#include <stdio.h>
#include <ctype.h>

#include "chesslib/board.h"
#include "chesslib/piecemoves.h"

board createBoard()
{
	return createBoardFromFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}

board createBoardFromFen(const char *fen)
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

	return board;
}

void boardSetPiece(board *board, pos pos, piece p)
{
	int index = posGetIndex(pos);
	board->pieces[index] = p;
}

piece boardGetPiece(board *board, pos pos)
{
	int index = posGetIndex(pos);
	return board->pieces[index];
}

// Generates a list of all legal moves. This list must be freed with freeMoveList
moveList *generateMoves(board *b)
{
	moveList *list = createMoveList();

	for (int i = 0; i < 64; i++)
	{
		pos p = posIndex(i);
		piece pe = boardGetPiece(b, p);

		if (getPieceColor(pe) != b->currentPlayer)
			continue;

		pieceType type = getPieceType(pe);
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
			// TODO - only add legal moves that won't leave you in check
			addAllMovesToMoveList(list, currMoves);
			freeMoveList(currMoves);
		}
	}

	return list;
}

uint8_t isSquareAttacked(board *b, pos p, pieceColor attacker)
{
	for (int i = 0; i < 64; i++)
	{
		pos attackerP = posIndex(i);
		piece attackerPe = boardGetPiece(b, attackerP);

		if (getPieceColor(attackerPe) != attacker)
			continue;

		pieceType type = getPieceType(attackerPe);
		moveList *currMoves = NULL;

		uint8_t found = 0;

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

uint8_t isInCheck(board *b)
{
	return isPlayerInCheck(b, b->currentPlayer);
}

uint8_t isPlayerInCheck(board *b, pieceColor player)
{
	piece royalPiece = (player == white) ? pWKing : pBKing;
	pieceColor otherColor = (player == white) ? black : white;
	for (int i = 0; i < 64; i++)
	{
		pos p = posIndex(i);
		if (boardGetPiece(b, p) == royalPiece)
		{
			if (isSquareAttacked(b, p, otherColor))
				return 1;
		}
	}
	return 0;
}
