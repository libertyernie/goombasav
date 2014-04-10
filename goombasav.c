#include <memory.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "goombasav.h"
#include "minilzo-2.06/minilzo.h"

void(*goomba_onerror)(const char*) = NULL;

/**
 * Prints an error to standard output.
 * If the function pointer goomba_onerror is not null, the error message will
 * be passed to it as well.
 *
 * Note: this function only compiles in native code on C++/CLI.
 */
void goomba_error(const char* format, ...) {
	va_list args;
	va_start(args, format);
	size_t len = vfprintf(stderr, format, args);
	va_end(args);

	if (goomba_onerror != NULL) {
		char* buf = (char*)malloc(len + 1);
		va_start(args, format);
		sprintf(buf, format, args);
		va_end(args);
		goomba_onerror(buf);
		free(buf);
	}
}

static char goomba_strbuf[256];

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
* will have a capacity of max_num_headers+1, and any difference between that
* and the number of headers found will be filled with NULL entries. The last
* entry (array[max_num_headers]) is guaranteed to be NULL.
*
* If a Goomba Color save file is detected and the configdata signals that the
* SRAM has not yet been compressed to its normal location, this method will
* print an error and return NULL itself.
*/
stateheader** stateheader_scan(const void* first_header, size_t max_num_headers) {
	const size_t psize = sizeof(stateheader*);
	stateheader** headers = (stateheader**)malloc(psize * (max_num_headers + 1));
	memset(headers, NULL, psize * (max_num_headers + 1));

	stateheader* sh = (stateheader*)first_header;
	bool sram_checksum_not_zero = false;
	bool using_regular_goomba = false;
	int i = 0;
	while (stateheader_plausible(sh)) {
		headers[i] = sh;
		// check things
		if (sh->type == GOOMBA_CONFIGSAVE) {
			configdata* cd = (configdata*)sh;
			if (cd->sram_checksum != 0) {
				sram_checksum_not_zero = true;
			}
		} else if (sh->type == GOOMBA_SRAMSAVE) {
			if (sh->size > sh->uncompressed_size) {
				using_regular_goomba = true;
			}
		}
		// end check
		i++;
		sh = stateheader_advance(sh);
	}
	if (sram_checksum_not_zero && !using_regular_goomba) {
		goomba_error("Goomba Color was not cleanly shut down - CFG->sram_checksum is not empty. Run the rom in an emulator and go to menu->exit.\n");
		return NULL;
	}
	return headers;
}

/**
 * Allocates memory to store the uncompressed GB/GBC save file extracted from
 * the Goomba Color save file stored in header_ptr, or returns NULL if the
 * decompression failed.
 */
void* goomba_extract(const void* header_ptr, size_t* size_output) {
	stateheader* sh = (stateheader*)header_ptr;

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

	if (sh->type != GOOMBA_SRAMSAVE) {
		goomba_error("Error - This program cannot replace non-SRAM data.\n");
		return NULL;
	}

	if (sh->size > sh->uncompressed_size) {
		goomba_error("Error - Save file replacement not working yet for regular Goomba.\n");
		return NULL;
	}

	// sh->uncompressed_size is valid for Goomba Color.
	// For Goomba, it's actually compressed size (and will be less than sh->size).
	// TODO: in that case, uncompress the data to a temp buffer and see how big it is, then go by that.
	if (gbc_length < sh->uncompressed_size) {
		goomba_error("Error: the length of the GBC data (%u) is too short - expected %u bytes.\n",
			gbc_length, sh->uncompressed_size);
		return NULL;
	} else if (gbc_length - 4 == sh->uncompressed_size) {
		goomba_error("Note: RTC data (TGB_Dual format) will not be copied\n");
	} else if (gbc_length - 44 == sh->uncompressed_size) {
		goomba_error("Note: RTC data (old VBA format) will not be copied\n");
	} else if (gbc_length - 48 == sh->uncompressed_size) {
		goomba_error("Note: RTC data (new VBA format) will not be copied\n");
	} else if (gbc_length > sh->uncompressed_size) {
		goomba_error("Warning: unknown data at end of GBC save file - last %u bytes will be ignored\n", gbc_length - sh->uncompressed_size);
	}

	if (sh->type != GOOMBA_SRAMSAVE) {
		goomba_error("The data at gba_sram + gba_header_location is not SRAM data.\n");
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
	lzo1x_1_compress((const unsigned char*)gbc_sram, sh->uncompressed_size,
		dest, &compressed_size,
		wrkmem);
	free(wrkmem);
	working += compressed_size;
	printf("Compressed %u bytes (%u)\n", sh->uncompressed_size, compressed_size);

	new_sh->size = compressed_size + sizeof(stateheader);
	// pad to 4 bytes!
	// if I don't do this, goomba color might not load the palette settings, or seemingly 'forget' them later
	// btw, the settings are stored in the configdata struct defined in goombasav.h
	while (new_sh->size % 4 != 0) {
		*working = 0;
		working++;
		printf("(inc)\n");
		new_sh->size++;
	}

	// restore the backup - just assume we have enough space
	memcpy(working, backup, backup_len);
	printf("Copied end (%u)\n", backup_len);

	free(backup);

	return goomba_new_sav;
}
