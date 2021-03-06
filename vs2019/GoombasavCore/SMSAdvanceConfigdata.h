#pragma once
/* SMSAdvanceConfigdata.h - subclass for Goomba's configuration data

This object will only remain valid while its GameBoyAdvanceSRAM has not yet
been disposed/finalized. GameBoyAdvanceSRAMHeader objects can be obtained via
the Headers property of GameBoyAdvanceSRAM.

Copyright (C) 2016-2021 libertyernie

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

#include "Configdata.h"

namespace GoombasavCore {
	ref class GameBoyAdvanceSRAM;

	public ref class SMSAdvanceConfigdata : Configdata {
	public:
		// Constructs an object using the given header pointer and parent object.
		// The parent is only used when the user tries to access the Parent property.
		SMSAdvanceConfigdata(const smsadvance_configdata* ptr, GameBoyAdvanceSRAM^ parent)
			: Configdata(ptr, parent) { }

#pragma region properties
		property const smsadvance_configdata* Pointer {
			const smsadvance_configdata* get() {
				return (const smsadvance_configdata*)VoidPointer;
			}
		}

		property uint32_t ROMChecksum {
			uint32_t get() override {
				return Pointer->sram_checksum;
			}
		}

		property String^ Title {
			String^ get() override {
				return gcnew String(Pointer->reserved3);
			}
		}
#pragma endregion
	};
}