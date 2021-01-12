#pragma once
/* Configdata.h - subclass for an emulator's configuration data

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

#include "GameBoyAdvanceSRAMHeader.h"

using System::String;

namespace GoombasavCore {
	public ref class Configdata abstract : GameBoyAdvanceSRAMHeader {
	internal:
		Configdata(const void* ptr, GameBoyAdvanceSRAM^ parent) : GameBoyAdvanceSRAMHeader(ptr, parent) {}
	public:
		virtual property uint32_t ROMChecksum {
			uint32_t get() abstract;
		}
	};
}