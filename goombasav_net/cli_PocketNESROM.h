#pragma once
/* cli_PocketNESROM.h - class to encapsulate NES ROM extracted from PocketNES ROM

Copyright (C) 2016 libertyernie

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

#include "../pocketnesrom.h"
#include "cli_ExtractedROM.h"
#include <cstdlib>
#include <cstring>

using namespace System;
using namespace System::Collections::Generic;
using System::Collections::ObjectModel::ReadOnlyCollection;

namespace Goombasav {
	public ref class PocketNESROM : public ExtractedROM {
	private:
		String^ name;
		uint32_t flags;
		uint32_t spritefollow;
		uint32_t reserved;

		array<unsigned char>^ data;

		PocketNESROM(pocketnes_romheader header, array<unsigned char>^ data) {
			this->name = gcnew String(header.name);
			this->flags = header.flags;
			this->spritefollow = header.spritefollow;
			this->reserved = header.reserved;
			this->data = data;
		}
	public:
		property String^ Name {
			virtual String^ get() {
				return this->name;
			}
		}
		property array<unsigned char>^ Data {
			virtual array<unsigned char>^ get() {
				return data;
			}
		}
		String^ ToString() override {
			return this->Name;
		}

		virtual uint32_t GetChecksum() {
			pin_ptr<unsigned char> p = &data[0];
			return pocketnes_get_checksum(p, data->Length);
		}

		static List<PocketNESROM^>^ Extract(array<unsigned char>^ source) {
			List<PocketNESROM^>^ list = gcnew List<PocketNESROM^>();

			pin_ptr<unsigned char> source_ptr = &source[0];
			const pocketnes_romheader* ptr = pocketnes_first_rom(source_ptr, source->Length);
			while (ptr) {
				array<unsigned char>^ copy = gcnew array<unsigned char>(ptr->filesize);
				pin_ptr<unsigned char> copy_ptr = &copy[0 + sizeof(pocketnes_romheader)];
				memmove(copy_ptr, ptr, copy->Length - sizeof(pocketnes_romheader));
				list->Add(gcnew PocketNESROM(*ptr, copy));
				ptr = pocketnes_next_rom(source_ptr, source->Length, ptr);
			}

			return list;
		}
	};
}