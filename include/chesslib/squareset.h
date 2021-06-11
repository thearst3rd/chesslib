/*
 * Square set definitions
 * Created by thearst3rd on 10/31/2020
 */

#pragma once

#include "chesslib/square.h"

// A square set is just a data structure that describes if each square on the board is "on" or "off" in the set
typedef struct
{
	int width;
	int height;
	uint8_t *bytes;
} sqSet;

sqSet *sqSetCreate();
sqSet *sqSetCreateCustomDimensions(int width, int height);
void sqSetFree(sqSet *ss);

void sqSetSetBit(sqSet *ss, int index, uint8_t value);
uint8_t sqSetGetBit(sqSet *ss, int index);

void sqSetSet(sqSet *ss, sq s, uint8_t value);
uint8_t sqSetGet(sqSet *ss, sq s);
