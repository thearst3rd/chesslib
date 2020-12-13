# chesslib

A chess implementation in C.

## Build

To build chesslib, compile the source files in `src/chesslib` with your favorite C compiler. The included makefile supports building on Linux as well as Windows with [MSYS2](https://www.msys2.org/). It might work on MacOS as well, but I do not have a Mac with which to test it.

To build using the makefile, simply run:

```
make
```

To run the tests, type

```
make test
```

### Building on Windows using MSYS2

First, download and install MSYS2.

https://www.msys2.org/

Open an MSYS2 prompt, and download gcc and other required packages using pacman.

```
pacman -S mingw64/mingw-w64-x86_64-{gcc,make}
```

Then, follow the Linux instructions above.

(TODO - make sure those packages are actually enough)

## Example Program

Here is a program which will play a random game of chess, and print the UCI moves that it is playing.

```c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "chesslib/chess.h"

int main()
{
	// Set the random seed
	srand(time(NULL));

	chess *c = chessCreate();

	// Keep playing moves while the game is ongoing
	while (chessGetTerminalState(c) == tsOngoing)
	{
		// Get the list of legal moves
		moveList *moves = chessGetLegalMoves(c);

		// Pick a random move
		int r = rand() % moves->size;
		move m = moveListGet(moves, r);

		// Print the UCI for that move
		char *uci = moveGetUci(m);
		printf("%s ", uci);
		free(uci);

		// Play the move
		chessPlayMove(c, m);
	}

	// Print the FEN of the final position
	char *fen = chessGetFen(c);
	printf("\nResulting FEN:\n%s\n", fen);
	free(fen);

	chessFree(c);

	return 0;
}
```

## Future plans

* Support for SAN
	* Playing chess moves from SAN strings
	* Outputting SAN strings from already played moves
	* Understanding "sloppy" SAN moves such as incorrect capitalization, or overly-disambiguated moves
* Support for PGN
	* Outputting a long string containing the PGN from a chess game
	* Importing a PGN into a chess game
* Full documentation
* Better API (I always want the API to be better...)
