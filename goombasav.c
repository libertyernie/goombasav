#include <memory.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "goombasav.h"
#include "minilzo-2.06/minilzo.h"

#define goomba_error(...) { sprintf(last_error, __VA_ARGS__); }

char last_error[256];

static char goomba_strbuf[256];

const char* goomba_last_error() {
	return (const char*)last_error;
}

const char* stateheader_typestr(uint16_t type) {
	switch (type) {
	case GOOMBA_STATESAVE:
		return "Savestate";
	case GOOMBA_SRAMSAVE:
		return "SRAM";
	case GOOMBA_CONFIGSAVE:
		return "Configuration";
	default:
		return "Unknown"; // Stateheaders with type >2 are rejected by stateheader_plausible
	}
}

/**
 * Returns a multi-line description string that includes all parameters of the
 * given stateheader or configdata structure.
 * This string is stored in a static character buffer, and subsequent calls to
 * stateheader_str or stateheader_summary_str will overwrite this buffer.
 */
const char* stateheader_str(const stateheader* sh) {
	int j = 0;
	j += sprintf(goomba_strbuf + j, "size: %u\n", sh->size);
	j += sprintf(goomba_strbuf + j, "type: %s (%u)\n", stateheader_typestr(sh->type), sh->type);
	if (sh->type == GOOMBA_CONFIGSAVE) {
		configdata* cd = (configdata*)sh;
		j += sprintf(goomba_strbuf + j, "bordercolor: %u\n", cd->bordercolor);
		j += sprintf(goomba_strbuf + j, "palettebank: %u\n", cd->palettebank);
		j += sprintf(goomba_strbuf + j, "misc: %u\n", cd->misc);
		j += sprintf(goomba_strbuf + j, "reserved3: %u\n", cd->reserved3);
		j += sprintf(goomba_strbuf + j, "sram_checksum: %8X\n", cd->sram_checksum);
		//j += sprintf(goomba_strbuf + j, "zero: %d\n", cd->zero);
	} else {
		j += sprintf(goomba_strbuf + j, "uncompressed_size: %u\n", sh->uncompressed_size);
		j += sprintf(goomba_strbuf + j, "framecount: %u\n", sh->framecount);
		j += sprintf(goomba_strbuf + j, "checksum: %8X\n", sh->checksum);
	}
	j += sprintf(goomba_strbuf + j, "title: %s", sh->title);
	return goomba_strbuf;
}

/**
* Returns a one-line summary string displaying the size and title of the
* stateheader or configdata structure.
* This string is stored in a static character buffer, and subsequent calls to
* stateheader_str or stateheader_summary_str will overwrite this buffer.
*/
const char* stateheader_summary_str(const stateheader* sh) {
	sprintf(goomba_strbuf, "%s: %s (%u b / %u uncomp)", stateheader_typestr(sh->type), sh->title, sh->size, sh->uncompressed_size);
	return goomba_strbuf;
}

int stateheader_plausible(const stateheader* sh) {
	return sh->type < 3 && sh->size >= sizeof(stateheader) && // check type (0,1,2) and size (at least 48)
		(sh->type == GOOMBA_CONFIGSAVE || sh->uncompressed_size > 0); // check uncompressed_size, but not for configsave
}

/**
 * When given a pointer to a stateheader, returns a pointer to where the next
 * stateheader will be located (if any). Use stateheader_plausible to
 * determine if there really is a header at this location.
 *
 * If stateheader_plausible determines that the input is not a valid
 * stateheader, NULL will be returned.
 */
stateheader* stateheader_advance(const stateheader* sh) {
	if (!stateheader_plausible(sh)) return NULL;

	uint16_t s = sh->size;
	char* c = (char*)sh;
	c += s;
	return (stateheader*)c;
}

/**
* Scans for valid stateheaders and allocates an array to store them. The array
* will have a capacity of 64, and any difference between that
* and the number of headers found will be filled with NULL entries. The last
* entry (array[63]) is guaranteed to be NULL.
* NOTE: the gba_data parameter can point to a valid header, or to a sequence
* equal to GOOMBA_STATEID immediately before a valid header.
*/
stateheader** stateheader_scan(const void* gba_data) {
	// Do not edit gba_data!
	// We are casting to non-const pointers so the client gets non-const pointers back.
	const size_t psize = sizeof(stateheader*);
	stateheader** headers = (stateheader**)malloc(psize * 64);
	memset(headers, NULL, psize * 64);

	uint32_t* check = (uint32_t*)gba_data;
	if (*check == GOOMBA_STATEID) check++;

	stateheader* sh = (stateheader*)check;
	int i = 0;
	while (stateheader_plausible(sh) && i < 64) {
		headers[i] = sh;
		i++;
		sh = stateheader_advance(sh);
	}
	return headers;
}

int32_t goomba_get_configdata_checksum_field(const void* gba_data) {
	stateheader** headers = stateheader_scan(gba_data);
	if (headers == NULL) return -1;

	int i;
	for (i = 0; headers[i] != NULL; i++) {
		if (headers[i]->type == GOOMBA_CONFIGSAVE) {
			// found configdata
			const configdata* cd = (configdata*)headers[i];
			free(headers);
			return cd->sram_checksum; // 0 = clean, postitive = unclean
		}
	}

	free(headers);
	return -1; // not sure when this would happen
}

/**
 * If there is save data in 0xe000-0xffff (as signaled by the configdata),
 * this function compresses it to where it's supposed to go.
 */
char* goomba_cleanup(const void* gba_data_param) {
	char gba_data[GOOMBA_COLOR_SRAM_SIZE]; // on stack - do not need to free
	memcpy(gba_data, gba_data_param, GOOMBA_COLOR_SRAM_SIZE);

	stateheader** headers = stateheader_scan(gba_data);
	if (headers == NULL) return NULL;

	int i, j;
	for (i = 0; headers[i] != NULL; i++) {
		if (headers[i]->type == GOOMBA_CONFIGSAVE) {
			// found configdata
			configdata* cd = (configdata*)headers[i];
			const uint32_t checksum = cd->sram_checksum;
			for (j = 0; headers[j] != NULL; j++) {
				stateheader* sh = headers[j];
				if (sh->type == GOOMBA_SRAMSAVE && sh->checksum == checksum) {
					// found stateheader
					free(headers); // so make sure we return something before the loop goes around again!!

					cd->sram_checksum = 0; // because we do this here, goomba_new_sav should not complain about an unclean file

					char gbc_data[GOOMBA_COLOR_SRAM_SIZE - GOOMBA_COLOR_AVAILABLE_SIZE];
					memcpy(gbc_data,
						gba_data + GOOMBA_COLOR_AVAILABLE_SIZE,
						sizeof(gbc_data)); // Extract GBC data at 0xe000 to an array

					char* new_gba_data = goomba_new_sav(gba_data, sh, gbc_data, sizeof(gbc_data));
					if (new_gba_data != NULL) memset(new_gba_data + GOOMBA_COLOR_AVAILABLE_SIZE, 0, sizeof(gbc_data));
					return new_gba_data;
				}
			}
		}
	}
	goomba_error("File is already clean\n");
	return NULL;
}

/**
 * Allocates memory to store the uncompressed GB/GBC save file extracted from
 * the Goomba Color save file stored in header_ptr, or returns NULL if the
 * decompression failed.
 */
void* goomba_extract(const void* gba_data, const stateheader* header_ptr, size_t* size_output) {
	const stateheader* sh = (const stateheader*)header_ptr;

	const int32_t ck = goomba_get_configdata_checksum_field(gba_data);
	if (ck < 0) {
		return NULL;
	} else if (ck == sh->checksum) {
		goomba_error("File is unclean - run goomba_cleanup before trying to extract SRAM, or you might get old data\n");
		return NULL;
	} else if (ck != 0) {
		fprintf(stderr, "File is unclean, but it shouldn't affect retrieval of the data you asked for\n");
	}

	if (sh->type != GOOMBA_SRAMSAVE) {
		goomba_error("Error: this is not SRAM data\n");
		return NULL;
	}
	
	lzo_uint compressed_size = sh->size - sizeof(stateheader);
	lzo_uint output_size = 32768;
	const unsigned char* compressed_data = (unsigned char*)header_ptr + sizeof(stateheader);
	unsigned char* uncompressed_data = (unsigned char*)malloc(output_size);
	int r = lzo1x_decompress_safe(compressed_data, compressed_size,
		uncompressed_data, &output_size,
		(void*)NULL);
	goomba_error("Actual uncompressed size: %u\n", output_size);
	if (r == LZO_E_INPUT_NOT_CONSUMED) {
		//goomba_error("Warning: input not fully used. Double-check the result to make sure it works.\n");
	} else if (r < 0) {
		goomba_error("LZO error code: %d\nLook this up in lzoconf.h.\n", r);
		free(uncompressed_data);
		return NULL;
	}
	*size_output = output_size;
	return uncompressed_data;
}

/**
 * Copies data from the source (which must point to a valid stateheader or
 * configdata) to dest, up to and including the first 48 bytes that do not
 * constitute a valid header.
 */
size_t copy_until_invalid_header(void* dest, const stateheader* src_param) {
	const void* src = src_param;
	size_t bytes_copied = 0;
	while (true) {
		const stateheader* sh = (const stateheader*)src;
		if (!stateheader_plausible(sh)) break;

		memcpy(dest, src, sh->size);

		src = (char*)src + sh->size;
		dest = (char*)dest + sh->size;
		bytes_copied += sh->size;
	}
	memcpy(dest, src, sizeof(stateheader)); // copy "footer"
	return bytes_copied + sizeof(stateheader);
}

char* goomba_new_sav(const void* gba_data, const void* gba_header, const void* gbc_sram, size_t gbc_length) {
	unsigned char* gba_header_ptr = (unsigned char*)gba_header;
	stateheader* sh = (stateheader*)gba_header_ptr;

	int32_t ck = goomba_get_configdata_checksum_field(gba_data);
	if (ck < 0) {
		return NULL;
	} else if (ck == sh->checksum) {
		// have to clean file
		goomba_error("File is unclean - run goomba_cleanup before trying to replace SRAM, or your new data might get overwritten");
		return NULL;
	} else if (ck != 0) {
		fprintf(stderr, "File is unclean, but it shouldn't affect replacement of the data you asked for\n");
	}

	if (sh->type != GOOMBA_SRAMSAVE) {
		goomba_error("Error - This program cannot replace non-SRAM data.\n");
		return NULL;
	}

	// sh->uncompressed_size is valid for Goomba Color.
	// For Goomba, it's actually compressed size (and will be less than sh->size).
	size_t uncompressed_size;
	if (sh->size > sh->uncompressed_size) {
		// Uncompress to a temporary location, just so we can see how big it is
		size_t output;
		void* dump = goomba_extract(gba_data, sh, &output);
		if (dump == NULL) {
			return NULL;
		}
		free(dump);
		uncompressed_size = output;
	} else {
		// Goomba Color header - use size from there
		uncompressed_size = sh->uncompressed_size;
	}
	
	if (gbc_length < uncompressed_size) {
		goomba_error("Error: the length of the GBC data (%u) is too short - expected %u bytes.\n",
			gbc_length, uncompressed_size);
		return NULL;
	} else if (gbc_length - 4 == uncompressed_size) {
		goomba_error("Note: RTC data (TGB_Dual format) will not be copied\n");
	} else if (gbc_length - 44 == uncompressed_size) {
		goomba_error("Note: RTC data (old VBA format) will not be copied\n");
	} else if (gbc_length - 48 == uncompressed_size) {
		goomba_error("Note: RTC data (new VBA format) will not be copied\n");
	} else if (gbc_length > uncompressed_size) {
		goomba_error("Warning: unknown data at end of GBC save file - last %u bytes will be ignored\n", gbc_length, uncompressed_size);
	}

	if (sh->type != GOOMBA_SRAMSAVE) {
		goomba_error("The data at gba_header is not SRAM data.\n");
		return NULL;
	}

	char* const goomba_new_sav = (char*)malloc(GOOMBA_COLOR_SRAM_SIZE);
	memset(goomba_new_sav, 0, GOOMBA_COLOR_SRAM_SIZE);
	char* working = goomba_new_sav; // will be incremented throughout

	size_t before_header = (char*)gba_header - (char*)gba_data;
	// copy anything before stateheader
	memcpy(goomba_new_sav, gba_data, before_header);
	working += before_header;
	// copy stateheader
	memcpy(working, sh, sizeof(stateheader));
	stateheader* new_sh = (stateheader*)working;
	working += sizeof(stateheader);

	// backup data that comes after this header
	unsigned char* backup = (unsigned char*)malloc(GOOMBA_COLOR_SRAM_SIZE);
	size_t backup_len = copy_until_invalid_header(backup, (stateheader*)(gba_header_ptr + sh->size));

	// compress gbc sram
	lzo_uint compressed_size;
	unsigned char* dest = (unsigned char*)working;
	void* wrkmem = malloc(LZO1X_1_MEM_COMPRESS);
	lzo1x_1_compress((const unsigned char*)gbc_sram, uncompressed_size,
		dest, &compressed_size,
		wrkmem);
	free(wrkmem);
	working += compressed_size;
	printf("Compressed %u bytes (%u)\n", uncompressed_size, compressed_size);

	if (sh->size > sh->uncompressed_size) {
		// Goomba header (not Goomba Color)
		new_sh->uncompressed_size = compressed_size;
	}

	new_sh->size = compressed_size + sizeof(stateheader);
	// pad to 4 bytes!
	// if I don't do this, goomba color might not load the palette settings, or seemingly 'forget' them later
	// btw, the settings are stored in the configdata struct defined in goombasav.h
	while (new_sh->size % 4 != 0) {
		*working = 0;
		working++;
		new_sh->size++;
	}

	size_t used = working - goomba_new_sav;
	if (used + backup_len > GOOMBA_COLOR_AVAILABLE_SIZE) {
		goomba_error("Not enough room in file for the new save data (0xe000-0xffff must be kept free, I think)\n");
		free(backup);
		free(goomba_new_sav);
		return NULL;
	}
	// restore the backup - just assume we have enough space
	memcpy(working, backup, backup_len);
	free(backup);

	// restore data from 0xe000 to 0xffff
	memcpy(goomba_new_sav + GOOMBA_COLOR_AVAILABLE_SIZE,
		(char*)gba_data + GOOMBA_COLOR_AVAILABLE_SIZE,
		GOOMBA_COLOR_SRAM_SIZE - GOOMBA_COLOR_AVAILABLE_SIZE);

	return goomba_new_sav;
}
