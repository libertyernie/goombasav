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

void extract(FILE* gba, FILE* gbc) {
	char* gba_data = (char*)malloc(GOOMBA_COLOR_SRAM_SIZE);
	fread(gba_data, 1, GOOMBA_COLOR_SRAM_SIZE, gba);
	fclose(gba);

	stateheader* sh = (stateheader*)(gba_data + 4);
	goomba_print_stateheader(stderr, sh);
	uint32_t uncompressed_size = sh->uncompressed_size;

	void* gbc_data = goomba_extract(sh);
	if (gbc_data != NULL) {
		fwrite(gbc_data, 1, uncompressed_size, gbc);
	}
	fclose(gbc);
	free(gbc_data);
	if (gbc_data == NULL) {
		exit(EXIT_FAILURE);
	}
}

void replace(FILE* gba, FILE* gbc) {
	char* gba_data = (char*)malloc(GOOMBA_COLOR_SRAM_SIZE);
	fread(gba_data, GOOMBA_COLOR_SRAM_SIZE, 1, gba);

	goomba_print_stateheader(stderr, (stateheader*)(gba_data + 4));

	fseek(gbc, 0, SEEK_END);
	size_t gbc_length = ftell(gbc);
	fseek(gbc, 0, SEEK_SET);

	void* gbc_data = malloc(gbc_length);
	fread(gbc_data, gbc_length, 1, gbc);

	void* new_gba_sram = goomba_replace(gba_data + 4, gbc_data, gbc_length);
	if (new_gba_sram != NULL) {
		fseek(gba, 4, SEEK_SET);
		fwrite(new_gba_sram, GOOMBA_COLOR_SRAM_SIZE, 1, gba);
	}
	free(gba_data);
	free(gbc_data);
	if (new_gba_sram == NULL) {
		exit(EXIT_FAILURE);
	}
}

int main(int argc, char** argv) {
	if (argc != 4) usage();
	if (argv[1][0] != 'x' && argv[1][0] != 'r') usage();

	FILE* gba;
	FILE* gbc;
	if (argv[1][0] == 'x') {
		gba = (strcmp("-", argv[2]) == 0)
			? stdin
			: fopen(argv[2], "rb");
		if (gba == NULL) could_not_open(argv[2]);

		gbc = (strcmp("-", argv[3]) == 0)
			? stdin
			: fopen(argv[3], "wb");
		if (gbc == NULL) could_not_open(argv[3]);

		extract(gba, gbc);
	} else if (argv[1][0] == 'r') {
		gba = fopen(argv[2], "r+b");
		if (gba == NULL) could_not_open(argv[2]);
		gbc = fopen(argv[3], "rb");
		if (gbc == NULL) could_not_open(argv[3]);

		replace(gba, gbc);
	}
	fclose(gba);
	fclose(gbc);
	return 0;
}
