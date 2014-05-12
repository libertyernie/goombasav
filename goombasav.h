#ifndef __GOOMBASAV_H
#define __GOOMBASAV_H

#include <stdint.h>
#define GOOMBA_COLOR_SRAM_SIZE 65536
#define GOOMBA_COLOR_AVAILABLE_SIZE 57344
#define GOOMBA_STATEID 0x57a731d8
#define GOOMBA_STATESAVE 0
#define GOOMBA_SRAMSAVE 1
#define GOOMBA_CONFIGSAVE 2

typedef uint32_t goomba_size_t; // want a consistent size for printf. This is an alias for uint32_t, but this makes it clear that we're counting the size of something.

/* 16-bit and 32-bit values in the stateheader are stored as little endian
(native to the GBA's ARM chip as well as x86 processors.) Use this function
after getting or before setting a value if your code might run on a big-endian
processor (e.g. PowerPC.) */
uint16_t little_endian_conv_16(uint16_t value);

/* 16-bit and 32-bit values in the stateheader are stored as little endian
(native to the GBA's ARM chip as well as x86 processors.) Use this function
after getting or before setting a value if your code might run on a big-endian
processor (e.g. PowerPC.) */
uint32_t little_endian_conv_32(uint32_t value);

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

typedef struct {
	const char* sleep;
	const char* autoload_state;
	const char* gamma;
} configdata_misc_strings;

const char* goomba_last_error();

configdata_misc_strings configdata_get_misc(char misc);
const char* stateheader_str(const stateheader* sh);
const char* stateheader_summary_str(const stateheader* sh);
int stateheader_plausible(const stateheader* sh);
stateheader* stateheader_advance(const stateheader* sh);
stateheader** stateheader_scan(const void* gba_data);
uint64_t goomba_compressed_data_checksum(const stateheader* sh, int output_bytes);
char* goomba_cleanup(const void* gba_data_param);
void* goomba_extract(const void* gba_data, const stateheader* header_ptr, goomba_size_t* size_output);
char* goomba_new_sav(const void* gba_data, const void* gba_header, const void* gbc_sram, goomba_size_t gbc_length);

#endif
