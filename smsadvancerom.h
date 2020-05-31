/* smsadvancerom.h - functions to find uncompressed Master System / Game Gear
ROM images stored within SMSAdvance ROMs

Copyright (C) 2016-2020 libertyernie

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

When compiling in Visual Studio, set the project to compile
as C++ code (Properties -> C/C++ -> Advanced -> Compile As.)
*/

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	uint32_t identifier;
	uint32_t filesize;
	uint32_t flags;
	uint32_t spritefollow;
	uint32_t reserved[4];
	char name[32];
} smsadvance_romheader;

/* Finds the first SMSAdvance ROM header in the given data block by looking for
the segment 534D531A (S,M,S,^Z). If no valid data is found, this method will
return NULL. */
const smsadvance_romheader* smsadvance_first_rom(const void* data, size_t length);

/* Returns a pointer to the next SMSAdvance ROM header in the data. If the
location where the next ROM header would be does not contain a 'S,M,S,^Z'
segment, this method will return NULL. */
const smsadvance_romheader* smsadvance_next_rom(const void* data, size_t length, const smsadvance_romheader* first_rom);

/* Returns true if the given data region looks like a SMSAdvance ROM header
(based on the 'S,M,S,^Z' segment), or false otherwise. */
int smsadvance_is_romheader(const void* data);

/* Returns the checksum that SMSAdvance would use for this ROM.
You can pass the SMSAdvance ROM header or the SMS/GG ROM itself. */
uint32_t smsadvance_get_checksum(const void* rom);

#ifdef __cplusplus
}
#endif
