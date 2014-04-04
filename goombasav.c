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

void goomba_replace(FILE* gba, FILE* gbc) {
	stateheader sh;
	fread(&sh, 1, 4, gba);
	fread(&sh, sizeof(stateheader), 1, gba);
	fprintf(stderr, "size: %d\n", sh.size);
	fprintf(stderr, "type: %d\n", sh.type);
	fprintf(stderr, "uncompressed_size: %d\n", sh.uncompressed_size);
	fprintf(stderr, "framecount: %d\n", sh.framecount);
	fprintf(stderr, "checksum: %d\n", sh.checksum);
	fprintf(stderr, "title: %s\n", sh.title);
	if (sh.type != SRAMSAVE) {
		fprintf(stderr, "Not sram data\n");
		exit(1);
	}
	
	unsigned char* uncompressed_data = (unsigned char*)malloc(sh.uncompressed_size);
	// Only read up to the size of the current save file on GBA - skips clock data in gbc .sav file if it exists
	fread(uncompressed_data, 1, sh.uncompressed_size, gbc);
	unsigned char* compressed_data = (unsigned char*)malloc(sh.uncompressed_size); // compressed size is unknown
	void* wrkmem = malloc(LZO1X_1_MEM_COMPRESS);
	lzo_uint compressed_size;
	lzo1x_1_compress(uncompressed_data, sh.uncompressed_size,
		compressed_data, &compressed_size,
		wrkmem);
	sh.size = compressed_size + sizeof(stateheader);
	fseek(gba, 4, SEEK_SET);
	fwrite(&sh, 1, sizeof(stateheader), gba);
	fwrite(compressed_data, 1, sh.size, gba);
	free(wrkmem);
	free(compressed_data);
	free(uncompressed_data);
}
