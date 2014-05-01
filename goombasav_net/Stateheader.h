#pragma once
#include "../goombasav.h"

using System::String;

namespace Goombasav {
	ref class GoombaSRAM;

	public ref class Stateheader {
	private:
		const stateheader* ptr;
		GoombaSRAM^ parent;
	public:
		static uint16_t STATESAVE = GOOMBA_STATESAVE;
		static uint16_t SRAMSAVE = GOOMBA_SRAMSAVE;
		static uint16_t CONFIGSAVE = GOOMBA_CONFIGSAVE;

		// Constructs an object using the given header pointer and parent object.
		// The parent is only used when the user tries to access the Parent property.
		Stateheader(const stateheader* ptr, GoombaSRAM^ parent) {
			this->parent = parent;
			this->ptr = ptr;
		}

#pragma region properties
		property const stateheader* Pointer {
			const stateheader* get() {
				return this->ptr;
			}
		}

		property GoombaSRAM^ Parent {
			GoombaSRAM^ get() {
				return this->parent;
			}
		}

		property uint16_t Size {
			uint16_t get() {
				return ptr->size;
			}
		}

		property uint16_t Type {
			uint16_t get() {
				return ptr->type;
			}
		}

		// Compressed size of data in Goomba; uncompressed size in Goomba Color.
		property uint32_t DataSize {
			uint32_t get() {
				return ptr->uncompressed_size;
			}
		}

		property uint32_t Framecount {
			uint32_t get() {
				return ptr->framecount;
			}
		}

		property uint32_t ROMChecksum {
			uint32_t get() {
				return ptr->checksum;
			}
		}
		
		property String^ Title {
			String^ get() {
				return gcnew String(ptr->title);
			}
		}
#pragma endregion

		bool IsPlausible() {
			return stateheader_plausible(this->ptr);
		}

		String^ ToString() override {
			return gcnew String(stateheader_summary_str(this->ptr));
		}
	};
}