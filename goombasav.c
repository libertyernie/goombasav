#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

#include "goombasav.h"
#include "minilzo-2.06/minilzo.h"

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

void stateheader_print(FILE* stream, const stateheader* sh) {
	fprintf(stream, "size: %u\n", sh->size);
	fprintf(stream, "type: %s (%u)\n", stateheader_typestr(sh->type), sh->type);
	if (sh->type == GOOMBA_CONFIGSAVE) {
		configdata* cd = (configdata*)sh;
		fprintf(stream, "bordercolor: %u\n", cd->bordercolor);
		fprintf(stream, "palettebank: %u\n", cd->palettebank);
		fprintf(stream, "misc: %u\n", cd->misc);
		fprintf(stream, "reserved3: %u\n", cd->reserved3);
		fprintf(stream, "sram_checksum: %8X\n", cd->sram_checksum);
		//fprintf(stream, "zero: %d\n", cd->zero);
	} else {
		fprintf(stream, "uncompressed_size: %u\n", sh->uncompressed_size);
		fprintf(stream, "framecount: %u\n", sh->framecount);
		fprintf(stream, "checksum: %8X\n", sh->checksum);
	}
	fprintf(stream, "title: %s\n", sh->title);
}

void stateheader_print_summary(FILE* stream, const stateheader* sh) {
	fprintf(stream, "%s: %s (%u b / %u uncomp)\n", stateheader_typestr(sh->type), sh->title, sh->size, sh->uncompressed_size);
}

bool stateheader_plausible(stateheader sh) {
	return sh.type < 3 && sh.size >= sizeof(stateheader) && (sh.type == GOOMBA_CONFIGSAVE || sh.uncompressed_size > 0);
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
	if (!stateheader_plausible(*sh)) return NULL;

	uint16_t s = sh->size;
	char* c = (char*)sh;
	c += s;
	return (stateheader*)c;
}

/**
 * Allocates memory to store the uncompressed GB/GBC save file extracted from
 * the Goomba Color save file stored in header_ptr, or returns NULL if the
 * decompression failed.
 */
void* goomba_extract(const void* header_ptr) {
	stateheader* sh = (stateheader*)header_ptr;

	if (sh->type != GOOMBA_SRAMSAVE) {
		fprintf(stderr, "Error: this is not SRAM data\n");
		return NULL;
	}
	
	lzo_uint compressed_size = sh->size - sizeof(stateheader);
	lzo_uint output_size = sh->uncompressed_size;
	const unsigned char* compressed_data = (unsigned char*)header_ptr + sizeof(stateheader);
	unsigned char* uncompressed_data = (unsigned char*)malloc(sh->uncompressed_size);
	int r = lzo1x_decompress_safe(compressed_data, compressed_size,
		uncompressed_data, &output_size,
		(void*)NULL);
	if (r == LZO_E_INPUT_NOT_CONSUMED) {
		fprintf(stderr, "Warning: input not fully consumed. Double-check the result to make sure it works.\n");
	} else if (r < 0) {
		fprintf(stderr, "LZO error code: %d\nLook this up in lzoconf.h.\n", r);
		free(uncompressed_data);
		return NULL;
	}
	return uncompressed_data;
}

size_t copy_until_invalid_header(void* dest, const void* src) {
	size_t bytes_copied = 0;
	while (true) {
		const stateheader* sh = (const stateheader*)src;
		if (!stateheader_plausible(*sh)) break;

		memcpy(dest, src, sh->size);

		src = (char*)src + sh->size;
		dest = (char*)dest + sh->size;
		bytes_copied += sh->size;
	}
	return bytes_copied;
}

/**
 * Modifies the Goomba/Goomba Color SRAM data at gba_header_ptr to contain the
 * save file pointed to by gba_sram. Returns gba_header_ptr on success or NULL
 * if the operation fails.
 */
void* goomba_replace(void* gba_header, const void* gbc_sram, size_t gbc_length) {
	unsigned char* gba_header_ptr = (unsigned char*)gba_header;
	stateheader* sh = (stateheader*)gba_header_ptr;

	if (gbc_length < sh->uncompressed_size) {
		fprintf(stderr, "Error: the length of the GBC data (%d) is too short - expected %d bytes.\n",
			gbc_length, sh->uncompressed_size);
		return NULL;
	} else if (gbc_length - 4 == sh->uncompressed_size) {
		fprintf(stderr, "Note: RTC data (TGB_Dual format) will not be copied\n");
	} else if (gbc_length - 44 == sh->uncompressed_size) {
		fprintf(stderr, "Note: RTC data (old VBA format) will not be copied\n");
	} else if (gbc_length - 48 == sh->uncompressed_size) {
		fprintf(stderr, "Note: RTC data (new VBA format) will not be copied\n");
	} else if (gbc_length > sh->uncompressed_size) {
		fprintf(stderr, "Warning: unknown data at end of GBC save file - last %d bytes will be ignored\n", gbc_length - sh->uncompressed_size);
	}

	if (sh->type != GOOMBA_SRAMSAVE) {
		fprintf(stderr, "The data at gba_sram + gba_header_location is not SRAM data.\n");
		return NULL;
	}

	// backup data that comes after this header
	unsigned char* backup = (unsigned char*)malloc(GOOMBA_COLOR_SRAM_SIZE);
	size_t backup_len = copy_until_invalid_header(backup, gba_header_ptr + sh->size);

	// compress gbc sram
	lzo_uint compressed_size;
	unsigned char* dest = gba_header_ptr + sizeof(stateheader);
	void* wrkmem = malloc(LZO1X_1_MEM_COMPRESS);
	lzo1x_1_compress((const unsigned char*)gbc_sram, sh->uncompressed_size,
		dest, &compressed_size,
		wrkmem);
	free(wrkmem);

	sh->size = compressed_size + sizeof(stateheader);
	// pad to 4 bytes!
	// if I don't do this, goomba color might not load the palette settings, or seemingly 'forget' them later
	// btw, the settings are stored in the configdata struct defined in goombasav.h
	while (sh->size % 4 != 0) {
		gba_header_ptr[sh->size] = 0;
		sh->size++;
	}

	// restore the backup - just assume we have enough space
	memcpy(gba_header_ptr + sh->size, backup, backup_len);

	free(backup);

	return gba_header_ptr;
}
