/*
 * Square implementation
 * Created by thearst3rd on 8/6/2020
 */

#include <stdio.h>
#include <string.h>
#include <math.h>

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
	char file;
	int rank;

	sscanf(str, "%c%d", &file, &rank);

	sq s;
	s.file = file - ('a' - 1);
	s.rank = rank;
	return s;
}

// Create a position from the given index
// This does not have knowledge of the board size and assumes 8x8. See boardSqIndex instead
sq sqIndex(int index)
{
	if (index < 0 || index > 63)
		return SQ_INVALID;

	int file = (index % 8) + 1;
	int rank = floor(index / 8) + 1;
	return sqI(file, rank);
}

int sqGetIndex(sq s)
{
	if (s.file < 1 || s.file > 8 || s.rank < 1 || s.rank > 8)
		return -1;

	return (8 * (s.rank - 1)) + (s.file - 1);
}

// Gets the SAN string representing the given square. Must be freed
// TODO: better support files > 26 (z)
char *sqGetStr(sq s)
{
	char buf[10];
	sprintf(buf, "%c%d", ('a' - 1) + s.file, s.rank);
	return strdup(buf);
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
