#pragma once
#include "GoombaHeader.h"

namespace Goombasav {
	ref class GoombaSRAM;

	public ref class MiscStrings {
	private:
		String^ sleepStr;
		String^ autoloadStateStr;
		String^ gammaStr;
	public:
		MiscStrings(configdata_misc_strings source) {
			sleepStr = gcnew String(source.sleep);
			autoloadStateStr = gcnew String(source.autoload_state);
			gammaStr = gcnew String(source.gamma);
		}
		property String^ SleepStr {
			String^ get() {
				return sleepStr;
			}
		}
		property String^ AutoloadStateStr {
			String^ get() {
				return autoloadStateStr;
			}
		}
		property String^ GammaStr {
			String^ get() {
				return gammaStr;
			}
		}
	};

	public ref class Configdata : GoombaHeader {
	public:
		// Constructs an object using the given header pointer and parent object.
		// The parent is only used when the user tries to access the Parent property.
		Configdata(const configdata* ptr, GoombaSRAM^ parent)
			: GoombaHeader(ptr, parent) { }

#pragma region properties
		property const configdata* Pointer {
			const configdata* get() {
				return (const configdata*)VoidPointer;
			}
		}

		property uint8_t BorderColor {
			uint8_t get() {
				return Pointer->bordercolor;
			}
		}
		property uint8_t PaletteBank {
			uint8_t get() {
				return Pointer->palettebank;
			}
		}
		/*property uint8_t Misc {
			uint8_t get() {
				return Pointer->misc;
			}
		}
		property uint8_t Reserved3 {
			uint8_t get() {
				return Pointer->reserved3;
			}
		}*/

		property uint32_t ROMChecksum {
			uint32_t get() {
				return Pointer->sram_checksum;
			}
		}

		/*property uint32_t Zero {
			uint32_t get() {
				return Pointer->zero;
			}
		}*/

		property MiscStrings^ GetMiscStrings {
			MiscStrings^ get() {
				return gcnew MiscStrings(configdata_get_misc(this->Pointer->misc));
			}
		}
#pragma endregion
	};
}