/* smsadvancerom.c - functions to find uncompressed Master System / Game Geat
ROM images stored within SMSAdvance ROMs

Copyright (C) 2016 libertyernie

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

#include <stdlib.h>
#include <string.h>
#include "smsadvancerom.h"

const char SMS_WORD[4] = { 'S','M','S',0x1A };

/* Finds the first SMSAdvance ROM header in the given data block by looking for
the segment 534D531A (S,M,S,^Z). If no valid data is found, this method will
return NULL. */
const smsadvance_romheader* smsadvance_first_rom(const void* data, size_t length) {
	const char* ptr = (const char*)data;
	const char* end = ptr + length;
	int logo_pos = 0;
	while (ptr < end) {
		if (*ptr == SMS_WORD[logo_pos]) {
			// match
			logo_pos++;
			if (logo_pos == 4) { // matched all of GB logo - on last byte (0x133)
				// check if length fits
				const smsadvance_romheader* candidate = (smsadvance_romheader*)(ptr - 3);
				size_t filesize = candidate->filesize;
				if (*(uint16_t *)"\0\xff" < 0x100) {
					uint32_t buffer;
					((char*)&buffer)[0] = ((char*)&filesize)[3];
					((char*)&buffer)[1] = ((char*)&filesize)[2];
					((char*)&buffer)[2] = ((char*)&filesize)[1];
					((char*)&buffer)[3] = ((char*)&filesize)[0];
					filesize = buffer;
				}
				const char* candidate_end_ptr = ptr - 3 + sizeof(smsadvance_romheader) + filesize;
				if (candidate_end_ptr <= end) {
					return candidate;
				} else {
					// no match, try again
					logo_pos = 0;
				}
			}
		}
		else {
			// no match, try again
			if (logo_pos > 0) {
				ptr -= logo_pos;
				logo_pos = 0;
			}
		}
		ptr++;
	}
	return NULL;
}

/* Returns a pointer to the next PocketNES ROM header in the data. If the
location where the next ROM header would be does not contain a 'S,M,S,^Z'
segment, this method will return NULL. */
const smsadvance_romheader* smsadvance_next_rom(const void* data, size_t length, const smsadvance_romheader* first_rom) {
	size_t diff = (const char*)first_rom - (const char*)data;
	if (diff > length) {
		return NULL;
	}
	size_t effective_length = length - diff;
	if (effective_length <= 0x200) {
		// Assume there will never be a ROM this small
		return NULL;
	}
	return smsadvance_first_rom(
		(const char*)first_rom + 4 + sizeof(smsadvance_romheader),
		effective_length - 4 - sizeof(smsadvance_romheader));
}

/* Returns true if the given data region looks like a SMSAdvance ROM header
(based on the 'S,M,S,^Z' segment), or false otherwise. */
int smsadvance_is_romheader(const void* data) {
	return memcmp(data, SMS_WORD, 4) == 0;
}

/* Returns the checksum that SMSAdvance would use for this ROM.
You can pass the SMSAdvance ROM header or the SMS/GG ROM itself. */
uint32_t smsadvance_get_checksum(const void* rom) {
	/*const uint8_t* p = (const uint8_t*)rom;

	uint32_t sum = 0;
	int i;
	for (i = 0; i<128; i++) {
		sum += *p | (*(p + 1) << 8) | (*(p + 2) << 16) | (*(p + 3) << 24);
		p += 128;
	}
	return sum;*/
	return 1;
}
