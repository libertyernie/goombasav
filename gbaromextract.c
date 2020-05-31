/* gbaromextract.c - command line interface to goombarom, pocketnesrom, and smsadvancerom

Copyright (C) 2014-2020 libertyernie

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
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "goombarom.h"
#include "pocketnesrom.h"
#include "smsadvancerom.h"
#include "platformname.h"

const char* USAGE = "gbaromextract (2020-05-30)\n"
"Usage: gbaromextract {x/extract} in.gba out.bin\n"
"       gbaromextract in.gba\n"
"\n"
"  x:    extract ROM from first file -> store in second file\n"
"\n"
"  one argument: view Goomba/SMSAdvance/PocketNES headers\n"
"\n"
"  -L: license information\n"
"  /? or --help: print this message\n";

const char* GPL_NOTICE = "gbaromextract - extract from Goomba, SMSAdvance, and PocketNES ROMs\n"
"Copyright (C) 2014-2020 libertyernie\n"
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

const void* ask(const void* gba_data, size_t size, const char* prompt) {
	const void* list[1024];
	int romsFound = 0;

	for (const pocketnes_romheader* ptr = pocketnes_first_rom(gba_data, size);
		ptr != NULL && romsFound < 1024;
		ptr = pocketnes_next_rom(gba_data, size, ptr))
	{
		list[romsFound] = ptr;
		fprintf(stderr, "%d. %s\n", romsFound++, ptr->name);
	}

	for (const smsadvance_romheader* ptr = smsadvance_first_rom(gba_data, size);
		ptr != NULL && romsFound < 1024;
		ptr = smsadvance_next_rom(gba_data, size, ptr))
	{
		list[romsFound] = ptr;
		fprintf(stderr, "%d. %s\n", romsFound++, ptr->name);
	}

	for (const void* ptr = gb_first_rom(gba_data, size);
		ptr != NULL && romsFound < 1024;
		ptr = gb_next_rom(gba_data, size, ptr))
	{
		list[romsFound] = ptr;
		fprintf(stderr, "%d. %s\n", romsFound++, gb_get_title(ptr, NULL));
	}

	if (romsFound == 0) {
		fprintf(stderr, "No ROMs found\n");
		exit(EXIT_FAILURE);
	}

	int index;
	fprintf(stderr, "%s", prompt);
	fflush(stderr);
	int dump;
	while (scanf("%d", &index) != 1 || index < 0 || index >= romsFound) {
		fprintf(stderr, "Invalid number entered - try again: ");
		fflush(stderr);
		while ((dump = getchar()) != '\n');
	}

	const void* selected = list[index];
	return selected;
}

void extract(const char* gbafile, const char* gbcfile) {
	FILE* gba = fopen(gbafile, "rb");
	if (gba == NULL) could_not_open(gbafile);

	fseek(gba, 0, SEEK_END);
	size_t size = ftell(gba);
	fseek(gba, 0, SEEK_SET);
	char* gba_data = (char*)malloc(size);
	fread(gba_data, 1, size, gba);

	fclose(gba);

	const void* ptr = ask(gba_data, size, "Extract: ");

	size_t extracted_size = 0;
	if (pocketnes_is_romheader(ptr)) {
		pocketnes_romheader* r = (pocketnes_romheader*)ptr;
		extracted_size = r->filesize;
		ptr = r + 1;
	} else if (smsadvance_is_romheader(ptr)) {
		smsadvance_romheader* r = (smsadvance_romheader*)ptr;
		extracted_size = r->filesize;
		ptr = r + 1;
	} else if (gb_is_rom(ptr)) {
		extracted_size = gb_rom_size(ptr);
	}
	
	FILE* gbc = (strcmp("-", gbcfile) == 0)
		? stdout
		: fopen(gbcfile, "wb");
	if (gbc == NULL) could_not_open(gbcfile);

	fwrite(ptr, 1, extracted_size, gbc);

	if (gbc != stdout) fclose(gbc);
	free(gba_data);
}

void list(const char* gbafile) {
	FILE* gba = fopen(gbafile, "rb");
	if (gba == NULL) could_not_open(gbafile);

	fseek(gba, 0, SEEK_END);
	size_t size = ftell(gba);
	fseek(gba, 0, SEEK_SET);
	char* gba_data = (char*)malloc(size);
	fread(gba_data, 1, size, gba);

	fclose(gba);

	int romsFound = 0;

	for (const pocketnes_romheader* ptr = pocketnes_first_rom(gba_data, size);
		ptr != NULL;
		ptr = pocketnes_next_rom(gba_data, size, ptr))
	{
		printf("%d. %s\n", romsFound++, ptr->name);
		printf("     %p: header\n", (char*)ptr - gba_data);
		printf("     %p: data (%d bytes)\n",
			(char*)(ptr + 1) - gba_data, 
			ptr->filesize);
	}

	for (const smsadvance_romheader* ptr = smsadvance_first_rom(gba_data, size);
		ptr != NULL;
		ptr = smsadvance_next_rom(gba_data, size, ptr))
	{
		printf("%d. %s\n", romsFound++, ptr->name);
		printf("     %p: header\n", (char*)ptr - gba_data);
		printf("     %p: data (%d bytes)\n",
			(char*)(ptr + 1) - gba_data,
			ptr->filesize);
	}

	for (const void* ptr = gb_first_rom(gba_data, size);
		ptr != NULL;
		ptr = gb_next_rom(gba_data, size, ptr))
	{
		printf("%d. %s\n", romsFound++, gb_get_title(ptr, NULL));
		printf("     %p: data (%d bytes)\n",
			(char*)ptr - gba_data,
			gb_rom_size(ptr));
	}

	if (romsFound == 0) {
		fprintf(stderr, "No ROMs found\n");
		exit(EXIT_FAILURE);
	}
}

int main(int argc, char** argv) {
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
		usage();
	} else if (strcmp("x", argv[1]) == 0 || strcmp("extract", argv[1]) == 0) {
		extract(argv[2], argv[3]);
	} else {
		usage();
	}
	return 0;
}
