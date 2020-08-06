/*
 * Position implementation
 * Created by thearst3rd on 8/6/2020
 */

#include "pos.h"

// Create a position from two ints, i.e. 5, 4
pos iPos(uint8_t file, uint8_t rank)
{
	pos p;
	p.file = file;
	p.rank = rank;
	return p;
}

// Create a position from a SAN square, i.e. "e4"
pos sPos(const char *str)
{
	pos p;
	p.file = str[0] - 'a' + 1;
	p.rank = str[1] - '0';
	return p;
}

const char *POS_STRS[64] =
{
	"a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
	"a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
	"a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
	"a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
	"a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
	"a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
	"a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
	"a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8"
};

// Gets the SAN string representing the given pos. Does not need to be freed
const char *posStr(pos p)
{
	unsigned int index = (p.rank - 1) * 8 + (p.file - 1);
	if (index >= 64)
		return "INVALID";
	return POS_STRS[index];
}
