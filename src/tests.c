/*
 * Testing for Chess implementation in C
 * Created by thearst3rd on 8/6/2020
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tests.h"
#include "pos.h"

const char *currTest;

#define RUN_TEST(testName) currTest=#testName; testName();


// Runs tests. Will halt when one is failed
int main(int argc, char *argv[])
{
	// Test Pos
	RUN_TEST(testIPos);
	RUN_TEST(testSPos);
	RUN_TEST(testPosStr);

	// We made it to the end
	printf("Success - all tests passed!\n");
	return 0;
}

// Fail the currently running test and halt the program. NULL for no message
void failTest(const char *msg)
{
	if (msg == NULL)
		fprintf(stderr, "TEST FAILED - %s\n", currTest);
	else
		fprintf(stderr, "TEST FAILED - %s: %s\n", currTest, msg);

	exit(1);
}


//////////////
// TEST POS //
//////////////

void testIPos()
{
	pos p;
	p = iPos(5, 4);

	if (!(p.file == 5 && p.rank == 4))
		failTest("pos (5, 4) should have values 5, 4");

	p = iPos(8, 6);

	if (!(p.file == 8 && p.rank == 6))
		failTest("pos (8, 6) should have values 8, 6");
}

void testSPos()
{
	pos p;
	p = sPos("e4");

	if (!(p.file == 5 && p.rank == 4))
		failTest("pos 'e4' should have values 5, 4");

	p = sPos("h6");

	if (!(p.file == 8 && p.rank == 6))
		failTest("pos 'h6' should have values 8, 6");
}

void testPosStr()
{
	pos p;
	p = sPos("e4");

	if (strcmp(posStr(p), "e4"))
		failTest("posStr of \"e4\" was not \"e4\"");

	p = sPos("h6");

	if (strcmp(posStr(p), "h6"))
		failTest("posStr of \"h6\" was not \"h6\"");
}
