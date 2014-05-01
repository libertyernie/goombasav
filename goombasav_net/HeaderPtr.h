#pragma once
#include "../goombasav.h"
namespace Goombasav {
	public ref class HeaderPtr {
	public:
		stateheader* sh_ptr() {
			return this->ptr;
		}
		configdata* cd_ptr() {
			return (configdata*)this->ptr;
		}

		bool plausible() {
			return stateheader_plausible(ptr);
		}

		HeaderPtr(void* ptr) {
			if (ptr == nullptr) throw gcnew System::NullReferenceException("Null pointer encountered in HeaderPtr constructor");
			this->ptr = (stateheader*)ptr;
		}

		virtual System::String^ ToString() override {
			return ptr->type == GOOMBA_CONFIGSAVE ? "Configuration"
				: GOOMBA_STATESAVE ? ("Savestate (" + gcnew System::String(ptr->title) + ")")
				: GOOMBA_SRAMSAVE ? ("SRAM (" + gcnew System::String(ptr->title) + ")")
				: "Unknown";
		}
	protected:
		stateheader* ptr;
	};
}
