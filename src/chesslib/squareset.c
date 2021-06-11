/*
 * Square set implementation
 * Created by thearst3rd on 10/31/2020
 */

#include <stdlib.h>

#include "chesslib/squareset.h"

sqSet *sqSetCreate()
{
	return sqSetCreateCustomDimensions(8, 8);
}

sqSet *sqSetCreateCustomDimensions(int width, int height)
{
	sqSet *ss = (sqSet *) malloc(sizeof(sqSet));
	ss->width = width;
	ss->height = height;

	int squares = width * height;
	int bytes = squares / 8;
	if (squares % 8 != 0)
		bytes++;

	ss->bytes = (uint8_t *) calloc(bytes, sizeof(uint8_t));

	return ss;
}

void sqSetFree(sqSet *ss)
{
	free(ss->bytes);
	free(ss);
}

void sqSetSetBit(sqSet *ss, int index, uint8_t value)
{
	if (index >= (ss->width * ss->height))
		return;

	int byte = index / 8;
	int bit = index % 8;

	if (value)
		ss->bytes[byte] |= (1 << bit);
	else
		ss->bytes[byte] &= ~(1 << bit);
}

uint8_t sqSetGetBit(sqSet *ss, int index)
{
	if (index >= (ss->width * ss->height))
		return 0;

	int byte = index / 8;
	int bit = index % 8;

	return (ss->bytes[byte] >> bit) & 1;
}

void sqSetSet(sqSet *ss, sq s, uint8_t value)
{
	int index = ((s.rank - 1) * ss->width) + (s.file - 1);
	sqSetSetBit(ss, index, value);
}

uint8_t sqSetGet(sqSet *ss, sq s)
{
	int index = ((s.rank - 1) * ss->width) + (s.file - 1);
	return sqSetGetBit(ss, index);
}
