/*
 * Position definitions
 * Created by thearst3rd on 8/6/2020
 */

#pragma once

#include <stdint.h>

// The position struct. These uint8_t's will be in range 1-8
typedef struct
{
	uint8_t file;
	uint8_t rank;
} pos;

#define POS_INVALID ((pos) {-1, -1})

// Create a position from two ints, i.e. 5, 4
pos posI(uint8_t file, uint8_t rank);
// Create a position from a SAN square, i.e. "e4"
pos posS(const char *str);

// Convert position to and from an index 0-63 (0 is a1, 63 is h8, +1 moves across a rank)
pos posIndex(uint8_t index);
uint8_t posGetIndex(pos p);

// Gets the SAN string representing the given pos. Does not need to be freed
const char *posGetStr(pos p);

// Compares two positions, returns true if equal and false otherwise
uint8_t posEq(pos p1, pos p2);
