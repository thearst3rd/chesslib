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

// Create a position from two ints, i.e. 5, 4
pos posI(uint8_t file, uint8_t rank);
// Create a position from a SAN square, i.e. "e4"
pos posS(const char *str);

// Gets the SAN string representing the given pos. Does not need to be freed
const char *posGetStr(pos p);
