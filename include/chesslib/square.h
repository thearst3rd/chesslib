/*
 * Square definitions
 * Created by thearst3rd on 8/6/2020
 */

#pragma once

#include <stdint.h>

typedef struct
{
	int file;
	int rank;
} sq;

#define SQ_INVALID ((sq) {-1, -1})

// Create a square from two ints, i.e. 5, 4
sq sqI(int file, int rank);
// Create a square from a SAN square, i.e. "e4"
sq sqS(const char *str);

// Convert square to and from an index 0-63 (0 is a1, 63 is h8, +1 moves across a rank)
sq sqIndex(int index);
int sqGetIndex(sq s);

// Gets the SAN string representing the given square. Must be freed
char *sqGetStr(sq s);

// Returns 1 if the square is a dark colored square, 0 if light
uint8_t sqIsDark(sq s);

// Compares two squares, returns true if equal and false otherwise
uint8_t sqEq(sq s1, sq s2);
