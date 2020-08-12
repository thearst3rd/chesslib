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
	for (uint8_t rank = 1; rank <= 8; rank++)
	{
		for (uint8_t file = 1; file <= 8; file++)
		{
			pos p = iPos(file, rank);

			if (p.file != file || p.rank != rank)
			{
				char msg[50];
				sprintf(msg, "Actual (%d, %d), but expected (%d, %d)", p.file, p.rank, file, rank);
				failTest(msg);
			}
		}
	}
}

void testSPos()
{
	for (uint8_t rank = 1; rank <= 8; rank++)
	{
		for (uint8_t file = 1; file <= 8; file++)
		{
			char str[3] = {'a' + file - 1, '0' + rank, 0};

			pos p = sPos(str);

			if (p.file != file || p.rank != rank)
			{
				char msg[60];
				sprintf(msg, "Actual (%d, %d), but expected (%d, %d) from \"%s\"", p.file, p.rank, file, rank, str);
				failTest(msg);
			}
		}
	}
}

void testPosStr()
{
	for (uint8_t rank = 1; rank <= 8; rank++)
	{
		for (uint8_t file = 1; file <= 8; file++)
		{
			pos p = iPos(file, rank);

			char str[3] = {'a' + file - 1, '0' + rank, 0};

			if (strcmp(posStr(p), str))
			{
				char msg[60];
				sprintf(msg, "Actual \"%s\", but expected \"%s\"", posStr(p), str);
				failTest(msg);
			}
		}
	}
}
