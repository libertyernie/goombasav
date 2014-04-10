// goombasav command line version. Compiles with Visual Studio 2013 (use /TP compiler option) and gcc 4.8.2 on cygwin.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "goombasav.h"

const char* USAGE = "Usage: goombasav {x/r} [Goomba Color save file] [raw GBC save file]\n"
"       goombasav [Goomba Color save file]\n"
"\n"
"  x: extract save data from first file -> store in second file\n"
"     (can be - for stdout)\n"
"  r: replace data in first file <- read from second file\n"
"  c: clean sram at 0xE000 in first file -> write to second file (both GBA sram)\n"
"\n"
"  otherwise: view Goomba Color SRAM header\n"
"             (file can be - for stdin)\n";

void usage() {
	fprintf(stderr, USAGE);
	exit(EXIT_FAILURE);
}

void could_not_open(const char* filename) {
	fprintf(stderr, "Could not open file: %s\n", filename);
	exit(EXIT_FAILURE);
}

stateheader* ask(const void* first_header, const char* prompt) {
	const char* char_ptr = (const char*)first_header;
	stateheader** headers = stateheader_scan(char_ptr, 20);
	if (headers == NULL) {
		fprintf(stderr, "An error occurred scanning for headers. See the console for more information.\n");
		exit(EXIT_FAILURE);
	}
	if (headers[0] == NULL) {
		fprintf(stderr, "No headers found\n");
		exit(EXIT_FAILURE);
	}

	int i = 0;
	while (headers[i] != NULL) {
		fprintf(stderr, "%d. %s\n", i, stateheader_summary_str(headers[i]));
		i++;
	}

	int index;
	fprintf(stderr, "%s", prompt);
	fflush(stderr);
	int dump;
	while (scanf("%d", &index) != 1 || index < 0 || index >= i) {
		fprintf(stderr, "Invalid number entered - try again: ");
		fflush(stderr);
		while ((dump = getchar()) != '\n');
	}

	stateheader* selected = headers[index];
	free(headers);
	return selected;
}

void extract(const char* gbafile, const char* gbcfile) {
	FILE* gba = fopen(gbafile, "rb");
	if (gba == NULL) could_not_open(gbafile);

	char* gba_data = (char*)malloc(GOOMBA_COLOR_SRAM_SIZE);
	fread(gba_data, 1, GOOMBA_COLOR_SRAM_SIZE, gba);
	fclose(gba);

	stateheader* sh = ask(gba_data + 4, "Extract: ");
	fprintf(stderr, "%s\n", stateheader_str(sh));
	size_t uncompressed_size;

	void* gbc_data = goomba_extract(gba_data, sh, &uncompressed_size);
	if (gbc_data == NULL) {
		exit(EXIT_FAILURE);
	}

	free(gba_data);

	FILE* gbc = (strcmp("-", gbcfile) == 0)
		? stdout
		: fopen(gbcfile, "wb");
	if (gbc == NULL) could_not_open(gbcfile);
	fwrite(gbc_data, 1, uncompressed_size, gbc);
	free(gbc_data);
	fclose(gbc);
}

void replace(const char* gbafile, const char* gbcfile) {
	FILE* gba = fopen(gbafile, "rb");
	if (gba == NULL) could_not_open(gbafile);

	FILE* gbc = fopen(gbcfile, "rb");
	if (gbc == NULL) could_not_open(gbcfile);

	char* gba_data = (char*)malloc(GOOMBA_COLOR_SRAM_SIZE);
	fread(gba_data, GOOMBA_COLOR_SRAM_SIZE, 1, gba);
	fclose(gba);

	stateheader* sh = ask(gba_data + 4, "Replace: ");
	fprintf(stderr, "%s\n", stateheader_str(sh));
	fseek(gbc, 0, SEEK_END);
	size_t gbc_length = ftell(gbc);
	fseek(gbc, 0, SEEK_SET);

	void* gbc_data = malloc(gbc_length);
	fread(gbc_data, gbc_length, 1, gbc);
	fclose(gbc);

	void* new_gba_sram = goomba_new_sav(gba_data, sh, gbc_data, gbc_length);
	if (gba == NULL) could_not_open(gbafile);
	if (new_gba_sram == NULL) {
		exit(EXIT_FAILURE);
	}
	gba = fopen(gbafile, "wb");
	fseek(gba, 0, SEEK_SET);
	fwrite(new_gba_sram, 1, GOOMBA_COLOR_SRAM_SIZE, gba); // Subtract diff from GOOMBA_COLOR_SRAM_SIZE to keep the file at 65536 bytes
	fclose(gba);
	free(new_gba_sram);
	free(gba_data);
	free(gbc_data);
}

void clean(const char* gbafile, const char* gbcfile) {
	FILE* gba1 = fopen(gbafile, "rb");
	if (gba1 == NULL) could_not_open(gbafile);

	char* gba_data = (char*)malloc(GOOMBA_COLOR_SRAM_SIZE);
	fread(gba_data, GOOMBA_COLOR_SRAM_SIZE, 1, gba1);
	fclose(gba1);

	void* new_gba_data = goomba_cleanup(gba_data);
	if (new_gba_data == NULL) {
		exit(EXIT_FAILURE);
	}

	FILE* gba2 = fopen(gbcfile, "wb");
	if (gba2 == NULL) could_not_open(gbcfile);
	fwrite(new_gba_data, 1, GOOMBA_COLOR_SRAM_SIZE, gba2);
	fclose(gba2);
}

void list(const char* gbafile) {
	FILE* gba = (strcmp("-", gbafile) == 0)
		? stdin
		: fopen(gbafile, "rb");
	if (gba == NULL) could_not_open(gbafile);

	char* gba_data = (char*)malloc(GOOMBA_COLOR_SRAM_SIZE);
	fread(gba_data, 1, GOOMBA_COLOR_SRAM_SIZE, gba);
	stateheader** headers = stateheader_scan(gba_data + 4, 20);
	if (headers == NULL) {
		exit(EXIT_FAILURE);
	}
	if (headers[0] == NULL) {
		fprintf(stderr, "No headers found\n");
		exit(EXIT_FAILURE);
	}

	int i = 0;
	while (headers[i] != NULL) {
		printf("%d. ", i);
		printf("%s\n", stateheader_summary_str(headers[i]));
		i++;
	}

	free(headers);
	fclose(gba);
}

int main(int argc, char** argv) {
	if (argc != 4 && argc != 2) usage();

	if (argc == 2) {
		if (strcmp("--help", argv[1]) == 0 || strcmp("/?", argv[1]) == 0) {
			usage();
		} else {
			list(argv[1]);
		}
	} else if (argv[1][0] == 'x') {
		extract(argv[2], argv[3]);
	} else if (argv[1][0] == 'r') {
		replace(argv[2], argv[3]);
	} else if (argv[1][0] == 'c') {
		clean(argv[2], argv[3]);
	} else {
		usage();
	}
	return 0;
}
