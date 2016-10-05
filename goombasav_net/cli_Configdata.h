#pragma once
/* cli_Configdata.h - subclass for Goomba's configuration data

This object will only remain valid while its GoombaSRAM has not yet been
disposed/finalized. GoombaHeader objects can be obtained via the Headers
property of GoombaSRAM.

Copyright (C) 2014 libertyernie

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
		Configdata(const goomba_configdata* ptr, GoombaSRAM^ parent)
			: GoombaHeader(ptr, parent) { }

#pragma region properties
		property const goomba_configdata* Pointer {
			const goomba_configdata* get() {
				return (const goomba_configdata*)VoidPointer;
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