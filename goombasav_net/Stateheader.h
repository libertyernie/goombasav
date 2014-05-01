#pragma once
#include "GoombaHeader.h"

using System::Tuple;

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

		// A three-byte hash of the compressed data - useful for showing on-screen as an RGB color.
		Tuple<uint8_t, uint8_t, uint8_t>^ CompressedDataHash() {
			uint64_t hash = goomba_compressed_data_checksum(this->Pointer, 3);
			return gcnew Tuple<uint8_t, uint8_t, uint8_t>(
				hash & 0xFF,
				(hash >> 8) & 0xFF,
				(hash >> 16) & 0xFF);
		}
	};
}