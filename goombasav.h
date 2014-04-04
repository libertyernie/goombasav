#ifndef __GOOMBASAV_H
#define __GOOMBASAV_H

#include <stdint.h>

typedef struct {		//(modified stateheader)
	uint16_t size;
	uint16_t type;	//=CONFIGSAVE
	char bordercolor;
	char palettebank;
	char misc;
	char reserved3;
	uint32_t sram_checksum;	//checksum of rom using SRAM e000-ffff	
	uint32_t zero;	//=0
	char reserved4[32];  //="CFG"
} configdata;

typedef struct {
	uint16_t size;	//header+data
	uint16_t type;	//=STATESAVE or SRAMSAVE
	uint32_t uncompressed_size;
	uint32_t framecount;
	uint32_t checksum;
	char title[32];
} stateheader;

void* goomba_extract(const void* header_ptr);
void goomba_extract_file(FILE* in, FILE* out);
void goomba_replace(FILE* gba, FILE* gbc);

#endif