/*
 * Square set implementation
 * Created by thearst3rd on 10/31/2020
 */

#include "chesslib/squareset.h"

void sqSetSet(sqSet *ss, sq s, uint8_t value)
{
	if (sqEq(s, SQ_INVALID))
		return;

	if (value)
		*ss |= (1l << (uint64_t) sqGetIndex(s));
	else
		*ss &= (~(1l << (uint64_t) sqGetIndex(s)));
}

uint8_t sqSetGet(sqSet *ss, sq s)
{
	if (sqEq(s, SQ_INVALID))
		return 0;

	return (*ss >> ((uint64_t) sqGetIndex(s))) & 1;
}
