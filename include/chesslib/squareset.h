/*
 * Square set definitions
 * Created by thearst3rd on 10/31/2020
 */

#pragma once

#include "chesslib/square.h"

// A square set is just a 64 bit word that describes if each square on the board is "on" or "off" in the set
typedef uint64_t sqSet;

void sqSetSet(sqSet *ss, sq s, uint8_t value);
uint8_t sqSetGet(sqSet *ss, sq s);
