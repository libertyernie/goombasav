// goombasav command line version. Compiles with Visual Studio 2013 (use /TP compiler option) and gcc 4.8.2 on cygwin.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "goombasav.h"

const char* USAGE = "Usage: goombasav {x/r} [Goomba Color save file] [raw GBC save file]\n"
"       goombasav [Goomba Color save file]\n"
"\n"
"  x: extract save data from first file -> store in second file\n"
"  r: replace data in first file <- read from second file\n"
"  no argument: view Goomba Color SRAM header\n"
"\n"
"When viewing or extracting data, the input and/or output files can be \"-\"\n"
"for standard input/output.\n";

void usage() {
	fprintf(stderr, USAGE);
	exit(EXIT_FAILURE);
}

void could_not_open(const char* filename) {
	fprintf(stderr, "Could not open file: %s\n", filename);
	exit(EXIT_FAILURE);
}

/**
 * Scans for valid stateheaders and allocates an array to store them. The array
 * will have a capacity of max_num_headers+1, and any difference between that
 * and the number of headers found will be filled with NULL entries. The last
 * entry (array[max_num_headers]) is guaranteed to be NULL.
 */
stateheader** stateheader_scan(const void* first_header, size_t max_num_headers) {
	const size_t psize = sizeof(stateheader*);
	stateheader** headers = (stateheader**)malloc(psize * (max_num_headers + 1));
	memset(headers, NULL, psize * (max_num_headers + 1));

	stateheader* sh = (stateheader*)first_header;
	int i = 0;
	while (stateheader_plausible(sh)) {
		headers[i] = sh;
		i++;
		sh = stateheader_advance(sh);
	}
	return headers;
}

stateheader* ask(const void* first_header) {
	const char* char_ptr = (const char*)first_header;
	stateheader** headers = stateheader_scan(char_ptr, 20);
	if (headers[0] == NULL) {
		fprintf(stderr, "No headers found\n");
		exit(EXIT_FAILURE);
	}

	int i = 0;
	while (headers[i] != NULL) {
		printf("%d. ", i);
		stateheader_print_summary(stdout, headers[i]);
		i++;
	}

	int index;
	printf("Extract: ");
	fflush(stdout);
	int dump;
	while (scanf("%d", &index) != 1 || index < 0 || index >= i) {
		printf("Invalid number entered - try again: ");
		fflush(stdout);
		while ((dump = getchar()) != '\n');
	}

	stateheader* selected = headers[index];
	free(headers);
	return selected;
}

void extract(FILE* gba, FILE* gbc) {
	char* gba_data = (char*)malloc(GOOMBA_COLOR_SRAM_SIZE);
	fread(gba_data, 1, GOOMBA_COLOR_SRAM_SIZE, gba);

	stateheader* sh = ask(gba_data + 4);
	stateheader_print(stderr, sh);
	uint32_t uncompressed_size = sh->uncompressed_size;

	void* gbc_data = goomba_extract(sh);
	if (gbc_data != NULL) {
		fwrite(gbc_data, 1, uncompressed_size, gbc);
	}
	free(gbc_data);
	if (gbc_data == NULL) {
		exit(EXIT_FAILURE);
	}
}

void replace(FILE* gba, FILE* gbc) {
	char* gba_data = (char*)malloc(GOOMBA_COLOR_SRAM_SIZE);
	fread(gba_data, GOOMBA_COLOR_SRAM_SIZE, 1, gba);

	stateheader* sh = ask(gba_data + 4);
	stateheader_print(stderr, sh);

	fseek(gbc, 0, SEEK_END);
	size_t gbc_length = ftell(gbc);
	fseek(gbc, 0, SEEK_SET);

	void* gbc_data = malloc(gbc_length);
	fread(gbc_data, gbc_length, 1, gbc);

	void* new_gba_sram = goomba_replace(sh, gbc_data, gbc_length);
	if (new_gba_sram != NULL) {
		size_t diff = (char*)sh - gba_data;
		fseek(gba, diff, SEEK_SET);
		fwrite(new_gba_sram, GOOMBA_COLOR_SRAM_SIZE, 1, gba);
	}
	free(gba_data);
	free(gbc_data);
	if (new_gba_sram == NULL) {
		exit(EXIT_FAILURE);
	}
}

int main(int argc, char** argv) {
	if (argc != 4 && argc != 2) usage();

	FILE* gba = NULL;
	FILE* gbc = NULL;
	if (argc == 2) {
		if (strcmp("--help", argv[1]) == 0 || strcmp("/?", argv[1]) == 0) {
			usage();
		} else {
			gba = (strcmp("-", argv[1]) == 0)
				? stdin
				: fopen(argv[1], "rb");
			if (gba == NULL) could_not_open(argv[2]);

			char* gba_data = (char*)malloc(GOOMBA_COLOR_SRAM_SIZE);
			fread(gba_data, 1, GOOMBA_COLOR_SRAM_SIZE, gba);
			stateheader** headers = stateheader_scan(gba_data + 4, 20);
			if (headers[0] == NULL) {
				fprintf(stderr, "No headers found\n");
				exit(EXIT_FAILURE);
			}

			int i = 0;
			while (headers[i] != NULL) {
				printf("%d. ", i);
				stateheader_print_summary(stdout, headers[i]);
				i++;
			}

			exit(EXIT_SUCCESS);
		}
	} else if (argv[1][0] == 'x') {
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
	} else {
		usage();
	}
	if (gba != NULL) fclose(gba);
	if (gbc != NULL) fclose(gbc);
	return 0;
}
