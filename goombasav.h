#ifndef __GOOMBASAV_H
#define __GOOMBASAV_H

#include <stdint.h>
#define GOOMBA_COLOR_SRAM_SIZE 65536
#define GOOMBA_COLOR_AVAILABLE_SIZE 57344
#define GOOMBA_STATEID 0x57a731d8
#define GOOMBA_STATESAVE 0
#define GOOMBA_SRAMSAVE 1
#define GOOMBA_CONFIGSAVE 2

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

extern void(*goomba_onerror)(const char*);

const char* stateheader_str(const stateheader* sh);
const char* stateheader_summary_str(const stateheader* sh);
int stateheader_plausible(const stateheader* sh);
stateheader* stateheader_advance(const stateheader* sh);
stateheader** stateheader_scan(const void* first_header, size_t max_num_headers);
void* goomba_extract(const void* header_ptr, size_t* size_output);
char* goomba_new_sav(const void* gba_data, const void* gba_header, const void* gbc_sram, size_t gbc_length);

#endif