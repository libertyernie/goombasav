#pragma once
/* cli_SMSAdvanceROM.h - class to encapsulate Master System / Game Gear ROM extracted from SMSAdvance ROM

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

#include "../smsadvancerom.h"
#include "cli_ExtractedROM.h"
#include <cstdlib>
#include <cstring>

using namespace System;
using namespace System::Collections::Generic;
using System::Collections::ObjectModel::ReadOnlyCollection;

namespace Goombasav {
	public ref class SMSAdvanceROM : public ExtractedROM {
	private:
		String^ name;
		uint32_t flags;
		uint32_t spritefollow;
		array<uint32_t>^ reserved;

		array<unsigned char>^ data;

		SMSAdvanceROM(smsadvance_romheader header, array<unsigned char>^ data) {
			this->name = gcnew String(header.name);
			this->flags = header.flags;
			this->spritefollow = header.spritefollow;
			this->reserved = gcnew array<uint32_t>(4);
			this->reserved[0] = header.reserved[0];
			this->reserved[1] = header.reserved[1];
			this->reserved[2] = header.reserved[2];
			this->reserved[3] = header.reserved[3];
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
			return smsadvance_get_checksum(p);
		}

		static List<SMSAdvanceROM^>^ Extract(array<unsigned char>^ source) {
			List<SMSAdvanceROM^>^ list = gcnew List<SMSAdvanceROM^>();

			pin_ptr<unsigned char> source_ptr = &source[0];
			const smsadvance_romheader* ptr = smsadvance_first_rom(source_ptr, source->Length);
			while (ptr) {
				array<unsigned char>^ copy = gcnew array<unsigned char>(ptr->filesize);
				pin_ptr<unsigned char> copy_ptr = &copy[0];
				memmove(copy_ptr, ptr + 1, copy->Length);
				list->Add(gcnew SMSAdvanceROM(*ptr, copy));
				ptr = smsadvance_next_rom(source_ptr, source->Length, ptr);
			}

			return list;
		}
	};
}