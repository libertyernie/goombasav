#pragma once
#include "../goombasav.h"

using System::String;

namespace Goombasav {
	ref class GoombaSRAM;

	public ref class GoombaHeader abstract {
	private:
		const void* ptr;
		GoombaSRAM^ parent;
	internal:
		// Constructs an object using the given header pointer and parent object.
		// The parent is only used when the user tries to access the Parent property.
		GoombaHeader(const void* ptr, GoombaSRAM^ parent) {
			this->parent = parent;
			this->ptr = (const stateheader*)ptr;
		}

		bool IsPlausible() {
			return stateheader_plausible((const stateheader*)ptr);
		}
	public:
		static uint16_t STATESAVE = GOOMBA_STATESAVE;
		static uint16_t SRAMSAVE = GOOMBA_SRAMSAVE;
		static uint16_t CONFIGSAVE = GOOMBA_CONFIGSAVE;
		static uint32_t STATEID = GOOMBA_STATEID;

#pragma region properties
		property const void* VoidPointer {
			const void* get() {
				return (const void*)this->ptr;
			}
		}

		property GoombaSRAM^ Parent {
			GoombaSRAM^ get() {
				return this->parent;
			}
		}

		property uint16_t Size {
			uint16_t get() {
				return ((const stateheader*)ptr)->size;
			}
		}

		property uint16_t Type {
			uint16_t get() {
				return ((const stateheader*)ptr)->type;
			}
		}

		property String^ Title {
			String^ get() {
				return gcnew String(((const stateheader*)ptr)->title);
			}
		}
#pragma endregion

		String^ GetSummary() {
			return gcnew String(stateheader_summary_str((const stateheader*)ptr));
		}

		String^ GetDescription() {
			return gcnew String(stateheader_str((const stateheader*)ptr));
		}

		String^ ToString() override {
			return GetSummary();
		}
	};
}