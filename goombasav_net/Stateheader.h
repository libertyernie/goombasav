#pragma once
#include "GoombaHeader.h"

namespace Goombasav {
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