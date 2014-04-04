// goombasav command line version. Compiles with Visual Studio 2013 (use /TP compiler option) and gcc 4.8.2 on cygwin.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "goombasav.h"

const char* USAGE = "Usage: goombasav {x/r} [Goomba Color save file] [raw GBC save file]\n"
"\n"
"x - extract save data from first file -> store in second file\n"
"r - replace data in first file <- read from second file\n"
"\n"
"For the x command, the input and/or output files can be \"-\" for stdin/stdout\n";

void usage() {
	fprintf(stderr, USAGE);
	exit(EXIT_FAILURE);
}

void could_not_open(const char* filename) {
	fprintf(stderr, "Could not open file: %s\n", filename);
	exit(EXIT_FAILURE);
}

int main(int argc, char** argv) {
	if (argc != 4) usage();
	if (argv[1][0] != 'x' && argv[1][0] != 'r') usage();
	
	if (argv[1][0] == 'x') {
		FILE* gba;
		FILE* gbc;
		gba = (strcmp("-", argv[2]) == 0)
			? stdin
			: fopen(argv[2], "rb");
		if (gba == NULL) could_not_open(argv[2]);
		gbc = (strcmp("-", argv[3]) == 0)
			? stdin
			: fopen(argv[3], "wb");
		if (gbc == NULL) could_not_open(argv[3]);
		goomba_extract_file(gba, gbc);
	} else if (argv[1][0] == 'r') {
		FILE* gba;
		FILE* gbc;
		gba = fopen(argv[2], "r+b");
		if (gba == NULL) could_not_open(argv[2]);
		gbc = (strcmp("-", argv[3]) == 0)
			? stdin
			: fopen(argv[3], "rb");
		if (gbc == NULL) could_not_open(argv[3]);
		goomba_replace(gba, gbc);
	}
	return 0;
}
