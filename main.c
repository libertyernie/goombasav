// goombasav command line version. Compiles with Visual Studio 2013 (use /TP compiler option) and gcc 4.8.2 on cygwin.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "goombasav.h"

#define GOOMBA_COLOR

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

stateheader* ask(const void* first_header, const char* prompt) {
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
		if (headers[i]->type == GOOMBA_CONFIGSAVE) {
#ifdef GOOMBA_COLOR
			configdata* cd = (configdata*)headers[i];
			printf("%u\n", cd->sram_checksum);
			if (cd->sram_checksum != 0) {
				fprintf(stderr, "Goomba Color was not cleanly shut down - CFG->sram_checksum is not empty. Run the rom in an emulator and go to menu->exit.\n");
				exit(EXIT_FAILURE);
			}
#endif
		}
		i++;
	}

	int index;
	printf("%s", prompt);
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

void extract(const char* gbafile, const char* gbcfile) {
	FILE* gba = (strcmp("-", gbafile) == 0)
		? stdin
		: fopen(gbafile, "rb");
	if (gba == NULL) could_not_open(gbafile);

	char* gba_data = (char*)malloc(GOOMBA_COLOR_SRAM_SIZE);
	fread(gba_data, 1, GOOMBA_COLOR_SRAM_SIZE, gba);
	fclose(gba);

	stateheader* sh = ask(gba_data + 4, "Extract: ");
	stateheader_print(stderr, sh);
	uint32_t uncompressed_size = sh->uncompressed_size;

	void* gbc_data = goomba_extract(sh);
	if (gbc_data == NULL) {
		exit(EXIT_FAILURE);
	}

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
	stateheader_print(stderr, sh);
	fseek(gbc, 0, SEEK_END);
	size_t gbc_length = ftell(gbc);
	fseek(gbc, 0, SEEK_SET);

	void* gbc_data = malloc(gbc_length);
	fread(gbc_data, gbc_length, 1, gbc);
	fclose(gbc);

	gba = fopen(gbafile, "wb");
	if (gba == NULL) could_not_open(gbafile);
	void* new_gba_sram = goomba_new_sav(gba_data, sh, gbc_data, gbc_length);
	if (new_gba_sram == NULL) {
		exit(EXIT_FAILURE);
	}
	fseek(gba, 0, SEEK_SET);
	fwrite(new_gba_sram, 1, GOOMBA_COLOR_SRAM_SIZE, gba); // Subtract diff from GOOMBA_COLOR_SRAM_SIZE to keep the file at 65536 bytes
	free(gba_data);
	free(gbc_data);
}

void list(const char* gbafile) {
	FILE* gba = (strcmp("-", gbafile) == 0)
		? stdin
		: fopen(gbafile, "rb");
	if (gba == NULL) could_not_open(gbafile);

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
	} else {
		usage();
	}
	return 0;
}
