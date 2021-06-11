/*
 * Square implementation
 * Created by thearst3rd on 8/6/2020
 */

#include "chesslib/square.h"

// Create a square from two ints, i.e. 5, 4
sq sqI(int file, int rank)
{
	sq s;
	s.file = file;
	s.rank = rank;
	return s;
}

// Create a position from a SAN square, i.e. "e4"
// TODO: Make this read numbers better for custom size
sq sqS(const char *str)
{
	char c1 = str[0];

	if (c1 < 'a')
		return SQ_INVALID;

	char c2 = str[1];

	if (c2 < '1')
		return SQ_INVALID;

	sq s;
	s.file = c1 - 'a' + 1;
	s.rank = c2 - '0';
	return s;
}

const char *SQ_STRS[64] =
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

// Create a position from the given index (same as the POS_STRS below)
sq sqIndex(int index)
{
	if (index < 0 || index > 63)
		return SQ_INVALID;

	return sqS(SQ_STRS[index]);
}

int sqGetIndex(sq s)
{
	if (s.file < 1 || s.file > 8 || s.rank < 1 || s.rank > 8)
		return -1;

	return (8 * (s.rank - 1)) + (s.file - 1);
}

// Gets the SAN string representing the given square. Does not need to be freed
const char *sqGetStr(sq s)
{
	unsigned int index = sqGetIndex(s);
	if (index > 63)
		return "##";
	return SQ_STRS[index];
}

// Returns 1 if the square is a dark colored square, 0 if light
uint8_t sqIsDark(sq s)
{
	return !((s.rank & 1) ^ (s.file & 1));
}

uint8_t sqEq(sq s1, sq s2)
{
	return s1.file == s2.file && s1.rank == s2.rank;
}
