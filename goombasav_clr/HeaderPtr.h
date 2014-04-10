#pragma once
#include "../goombasav.h"

ref class HeaderPtr {
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

	static HeaderPtr^ FromPtr(void* ptr); // factory method, defined later
	virtual System::String^ ToString() override {
		return ptr->type == GOOMBA_CONFIGSAVE ? "Configuration"
			: GOOMBA_STATESAVE ? ("Savestate (" + gcnew System::String(ptr->title) + ")")
			: GOOMBA_SRAMSAVE ? ("SRAM (" + gcnew System::String(ptr->title) + ")")
			: "Unknown";
	}
protected:
	stateheader* ptr;
};

ref class StateheaderPtr : HeaderPtr {
public:
	StateheaderPtr(void* ptr) {
		this->ptr = (stateheader*)ptr;
	}

#pragma region properties
	property uint16_t size {
		uint16_t get() {
			return ptr->size;
		}
	}

	property uint16_t type {
		uint16_t get() {
			return ptr->type;
		}
	}

	property uint32_t uncompressed_size {
		uint32_t get() {
			return ptr->uncompressed_size;
		}
	}

	property uint32_t framecount {
		uint32_t get() {
			return ptr->framecount;
		}
	}

	property uint32_t checksum {
		uint32_t get() {
			return ptr->checksum;
		}
	}

	property System::String^ title {
		System::String^ get() {
			return gcnew System::String(ptr->title);
		}
	}
#pragma endregion
};

ref class ConfigdataPtr : HeaderPtr {
public:
	ConfigdataPtr(void* ptr) {
		this->ptr = (stateheader*)ptr;
	}

#pragma region properties
	property uint16_t size {
		uint16_t get() {
			return ptr->size;
		}
	}

	property uint16_t type {
		uint16_t get() {
			return ptr->type;
		}
	}

	property char bordercolor {
		char get() {
			return ((configdata*)ptr)->bordercolor;
		}
	}

	property char palettebank {
		char get() {
			return ((configdata*)ptr)->palettebank;
		}
	}

	property char misc {
		char get() {
			return ((configdata*)ptr)->misc;
		}
	}

	property char reserved3 {
		char get() {
			return ((configdata*)ptr)->reserved3;
		}
	}

	property uint32_t sram_checksum {
		uint32_t get() {
			return ((configdata*)ptr)->sram_checksum;
		}
	}

	property uint32_t zero {
		uint32_t get() {
			return ((configdata*)ptr)->zero;
		}
	}

	property System::String^ reserved4 {
		System::String^ get() {
			return gcnew System::String(ptr->title);
		}
	}
#pragma endregion
};

HeaderPtr^ HeaderPtr::FromPtr(void* ptr) {
	stateheader* sh = (stateheader*)ptr;
	if (sh->type == GOOMBA_CONFIGSAVE) {
		return gcnew ConfigdataPtr(ptr);
	} else {
		return gcnew StateheaderPtr(ptr);
	}
}