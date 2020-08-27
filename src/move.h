/*
 * Move definitions
 * Created by thearst3rd on 8/6/2020
 */

#pragma once

#include <stdint.h>
#include "pos.h"
#include "piece.h"

typedef struct
{
	pos from;
	pos to;
	pieceType promotion;
} move;

move movePos(pos from, pos to);
move movePromote(pos from, pos to, pieceType promotion);

// Gets the UCI notation for a move. Must be freed
char *moveGetUci(move m);
move moveFromUci(char *uci);
