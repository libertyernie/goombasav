/* main.c - command line interface to goombasav

Copyright (C) 2014-2016 libertyernie

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.

https://github.com/libertyernie/goombasav

When compiling in Visual Studio, set all goombasav files to compile
as C++ code (Properties -> C/C++ -> Advanced -> Compile As.)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "goombasav.h"
#include "platformname.h"

const char* USAGE = "goombasav (2017-08-29)\n"
"Usage: goombasav {x/extract} gba.sav gbc.sav\n"
"       goombasav {r/replace} gba.sav gbc.sav\n"
"       goombasav {c/clean} gba-in.sav [gba-out.sav]\n"
"       goombasav isok [file1.sav [file2.sav [...]]]\n"
"       goombasav gba.sav\n"
"\n"
"  x:    extract save data from first file -> store in second file\n"
"        (\"gbc.sav\" can be - for stdout)\n"
"  r:    replace data in first file <- read from second file\n"
"  c:    clean sram at 0xE000 in 1st file -> write to 2nd file if specified,\n"
"        replace first file otherwise (second file can be - for stdout)\n"
"  isok: check if the file begins with a four-byte sequence that identifies it\n"
"        as a Goomba or PocketNES save file\n"
"\n"
"  one argument: view Goomba/PocketNES headers\n"
"                (file can be - for stdin)\n"
"\n"
"  -L: license information\n"
"  /? or --help: print this message\n";

const char* GPL_NOTICE = "goombasav - extract and replace Goomba/Goomba Color save files\n"
"Copyright (C) 2014-2017 libertyernie\n"
"https://github.com/libertyernie/goombasav\n"
"\n"
"This program is free software: you can redistribute it and/or modify\n"
"it under the terms of the GNU General Public License as published by\n"
"the Free Software Foundation, either version 2 of the License, or\n"
"(at your option) any later version.\n"
"\n"
"This program is distributed in the hope that it will be useful,\n"
"but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the\n"
"GNU General Public License for more details.\n"
"\n"
"You should have received a copy of the GNU General Public License\n"
"along with this program. If not, see <http://www.gnu.org/licenses/>.\n";

void usage() {
	fprintf(stderr, USAGE);
	exit(EXIT_FAILURE);
}

void license() {
	fprintf(stdout, GPL_NOTICE);
#ifdef PLATFORM_NAME
	fprintf(stdout, "\nCompiled for %s\n", PLATFORM_NAME);
#endif
#ifdef ARCH_NAME
	fprintf(stdout, "Arch: %s\n", ARCH_NAME);
#endif
	exit(EXIT_FAILURE);
}

void could_not_open(const char* filename) {
	fprintf(stderr, "Could not open file: %s\n", filename);
	exit(EXIT_FAILURE);
}

void print_indent(const char* prefix, const char* multiline_str) {
	printf("  ");
	while (*multiline_str != '\0') {
		putc(*multiline_str, stdout);
		if (*multiline_str == '\n') {
			printf("  ");
		}
		multiline_str++;
	}
	putc('\n', stdout);
}

stateheader* ask(const void* first_header, const char* prompt) {
	stateheader** headers = stateheader_scan(first_header);
	if (headers == NULL) {
		fprintf(stderr, "Error: %s", goomba_last_error());
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
	goomba_size_t uncompressed_size;

	void* gbc_data = goomba_extract(gba_data, sh, &uncompressed_size);
	if (gbc_data == NULL) {
		fprintf(stderr, "Error: %s", goomba_last_error());
		exit(EXIT_FAILURE);
	}

	free(gba_data);

	FILE* gbc = (strcmp("-", gbcfile) == 0)
		? stdout
		: fopen(gbcfile, "wb");
	if (gbc == NULL) could_not_open(gbcfile);
	fwrite(gbc_data, 1, uncompressed_size, gbc);
	free(gbc_data);
	if (gbc != stdout) fclose(gbc);
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
	if (new_gba_sram == NULL) {
		fprintf(stderr, "Error: %s", goomba_last_error());
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

// infile and outfile are not open at the same time, so they can be the same file path.
void clean(const char* infile, const char* outfile) {
	FILE* gba1 = fopen(infile, "rb");
	if (gba1 == NULL) could_not_open(infile);

	char* gba_data = (char*)malloc(GOOMBA_COLOR_SRAM_SIZE);
	fread(gba_data, GOOMBA_COLOR_SRAM_SIZE, 1, gba1);
	fclose(gba1);

	void* new_gba_data = goomba_cleanup(gba_data);
	if (new_gba_data == NULL) {
		fprintf(stderr, "Error: %s", goomba_last_error());
		exit(EXIT_FAILURE);
	} else if (new_gba_data == gba_data) {
		fprintf(stderr, "File is already clean - copying\n");
	}

	FILE* gba2 = strcmp("-", outfile) == 0
		? stdout
		: fopen(outfile, "wb");
	if (gba2 == NULL) could_not_open(outfile);
	fwrite(new_gba_data, 1, GOOMBA_COLOR_SRAM_SIZE, gba2);
	if (gba2 != stdout) fclose(gba2);
}

void list(const char* gbafile) {
	FILE* gba = (strcmp("-", gbafile) == 0)
		? stdin
		: fopen(gbafile, "rb");
	if (gba == NULL) could_not_open(gbafile);

	char* gba_data = (char*)malloc(GOOMBA_COLOR_SRAM_SIZE);
	fread(gba_data, 1, GOOMBA_COLOR_SRAM_SIZE, gba);

	int64_t uncompressed_data_checksum = goomba_get_configdata_checksum_field(gba_data);
	if (uncompressed_data_checksum < 0) {
		fprintf(stderr, "Error: %s", goomba_last_error());
	} else if (uncompressed_data_checksum) {
		printf("File is dirty: uncompressed data at 0xE000-0xFFFF for ROM with checksum %8X\n", uncompressed_data_checksum);
	}

	stateheader** headers = stateheader_scan(gba_data);
	if (headers == NULL) {
		fprintf(stderr, "Error: %s", goomba_last_error());
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
		print_indent("  ", stateheader_str(headers[i]));
		if (little_endian_conv_16(headers[i]->size) > sizeof(stateheader)) {
			printf("  [3-byte compressed data checksum: %6X]\n", goomba_compressed_data_checksum(headers[i], 3));
		}
		i++;
	}

	free(headers);
	if (gba != stdin) fclose(gba);
}

int main(int argc, char** argv) {
	if (argc >= 2 && strcmp("isok", argv[1]) == 0) {
		uint32_t id;
		int exitcode = 0, i;
		for (i = 2; i < argc; i++) {
			FILE* f = fopen(argv[i], "rb");
			size_t br = fread(&id, 1, 4, f);
			fclose(f);
			int result = br < 4 ? 0 : goomba_is_sram(&id);
			printf("%s: %s\n", argv[i], result ? "OK" : "FAILED");
			if (!result) exitcode = 1;
		}
		return exitcode;
	}

	if (argc > 4 || argc < 2) usage();

	if (argc == 2) {
		if (strcmp("--help", argv[1]) == 0 || strcmp("/?", argv[1]) == 0) {
			usage();
		} else if (strcmp("-L", argv[1]) == 0) {
			license();
		} else {
			list(argv[1]);
		}
	} else if (argc == 3) {
		if (strcmp("c", argv[1]) == 0 || strcmp("clean", argv[1]) == 0) {
			clean(argv[2], argv[2]);
		} else {
			usage();
		}
	} else if (strcmp("x", argv[1]) == 0 || strcmp("extract", argv[1]) == 0) {
		extract(argv[2], argv[3]);
	} else if (strcmp("r", argv[1]) == 0 || strcmp("replace", argv[1]) == 0) {
		replace(argv[2], argv[3]);
	} else if (strcmp("c", argv[1]) == 0 || strcmp("clean", argv[1]) == 0) {
		clean(argv[2], argv[3]);
	} else {
		usage();
	}
	return 0;
}
