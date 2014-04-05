#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

#include "goombasav.h"
#include "minilzo-2.06/minilzo.h"
#define SRAMSAVE 1

#define GOOMBA_COLOR_SRAM_SIZE 65536
#define GOOMBASAV_PRINT_DATA

/**
 * Allocates memory to store the uncompressed GB/GBC save file extracted from
 * the Goomba Color save file stored in header_ptr, or returns NULL if the
 * decompression failed.
 */
void* goomba_extract(const void* header_ptr) {
	stateheader* sh = (stateheader*)header_ptr;
	#ifdef GOOMBASAV_PRINT_DATA
	fprintf(stderr, "size: %d\n", sh->size);
	fprintf(stderr, "type: %d\n", sh->type);
	fprintf(stderr, "uncompressed_size: %d\n", sh->uncompressed_size);
	fprintf(stderr, "framecount: %d\n", sh->framecount);
	fprintf(stderr, "checksum: %d\n", sh->checksum);
	fprintf(stderr, "title: %s\n", sh->title);
	#endif

	if (sh->type != 1) {
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

void goomba_extract_file(FILE* in, FILE* out) {
	stateheader* sh = (stateheader*)malloc(GOOMBA_COLOR_SRAM_SIZE);
	fread(sh, 1, 4, in);
	fread(sh, GOOMBA_COLOR_SRAM_SIZE - 4, 1, in);
	
	void* uncompressed_data = goomba_extract(sh);
	if (uncompressed_data != NULL) {
		fwrite(uncompressed_data, 1, sh->uncompressed_size, out);
		free(uncompressed_data);
	}
	free(sh);
}

size_t copy_until_64bytes0(void* dest, const void* src) {
	// if you used almost all 64KB earlier for some reason, it will segfault
	// to prevent this, you could add a zero buffer to the end of the GBA SRAM data when you first read it in

	size_t bytes_copied = 0;
	int bytes_zero = 0;
	uint64_t* dest2 = (uint64_t*)dest; // copy 8 bytes (64 bits) at once
	uint64_t* src2 = (uint64_t*)src;
	while (bytes_zero < 64) { // copy until 64 bytes worth of zeroes are encountered
		*dest2 = *src2;
		if (*src2 == 0) {
			bytes_zero += sizeof(uint64_t);
		} else {
			bytes_zero = 0; // reset counter
		}
		dest2++;
		src2++;
		bytes_copied += sizeof(uint64_t);
	}
	// 64 bytes of zeroes found - subtract those from the count (even though they were copied, they don't need to be copied back later)
	bytes_copied -= bytes_zero;
	fprintf(stderr, "--backed up %d bytes--\n", bytes_copied);
	fprintf(stderr, "--zeroes:   %d bytes--\n", bytes_zero);
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
#ifdef GOOMBASAV_PRINT_DATA
	fprintf(stderr, "size: %d\n", sh->size);
	fprintf(stderr, "type: %d\n", sh->type);
	fprintf(stderr, "uncompressed_size: %d\n", sh->uncompressed_size);
	fprintf(stderr, "framecount: %d\n", sh->framecount);
	fprintf(stderr, "checksum: %d\n", sh->checksum);
	fprintf(stderr, "title: %s\n", sh->title);
#endif

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

	if (sh->type != SRAMSAVE) {
		fprintf(stderr, "The data at gba_sram + gba_header_location is not SRAM data.\n");
		return NULL;
	}

	// backup data that comes after this header
	unsigned char* backup = (unsigned char*)malloc(GOOMBA_COLOR_SRAM_SIZE);
	size_t backup_len = copy_until_64bytes0(backup, gba_header_ptr + sh->size);

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

void goomba_replace_file(FILE* gba, FILE* gbc) {
	unsigned char* gba_data = (unsigned char*)malloc(GOOMBA_COLOR_SRAM_SIZE);
	fread(gba_data, GOOMBA_COLOR_SRAM_SIZE, 1, gba);

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
}
