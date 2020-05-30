#pragma once
/* cli_Stateheader.h - subclass for SRAM or savestate data

This object will only remain valid while its GoombaSRAM has not yet been
disposed/finalized. GoombaHeader objects can be obtained via the Headers
property of GoombaSRAM.

Copyright (C) 2014-2020 libertyernie

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

https://github.com/libertyernie/goombasav */

#include "cli_GoombaHeader.h"

namespace GoombasavCore {
	ref class GoombaSRAM;

	public ref class Stateheader : GoombaHeader {
	public:
		// Constructs an object using the given header pointer and parent object.
		// The parent is only used when the user tries to access the Parent property.
		Stateheader(const stateheader* ptr, GoombaSRAM^ parent)
			: GoombaHeader(ptr, parent) { }

#pragma region properties
		property const stateheader* Pointer {
			const stateheader* get() {
				return (const stateheader*)VoidPointer;
			}
		}

		///<summary>
		///Compressed size of data in Goomba; uncompressed size in Goomba Color.
		///</summary>
		property uint32_t DataSize {
			uint32_t get() {
				return Pointer->uncompressed_size;
			}
		}

		property uint32_t Framecount {
			uint32_t get() {
				return Pointer->framecount;
			}
		}

		property uint32_t ROMChecksum {
			uint32_t get() {
				return Pointer->checksum;
			}
		}
#pragma endregion
	};
}