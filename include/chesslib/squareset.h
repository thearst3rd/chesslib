/*
 * Square set definitions
 * Created by thearst3rd on 10/31/2020
 */

#pragma once

#include "chesslib/square.h"

// A square set is just a 64-bit number that describes if each square on the board is "on" or "off" in the set

void sqSetSet(uint64_t *ss, sq s, uint8_t value);
uint8_t sqSetGet(uint64_t *ss, sq s);
