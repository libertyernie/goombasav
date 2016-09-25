#pragma once
/* GoombaROM.h - class to encapsulate Game Boy ROM extracted from Goomba ROM or other data file

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

#include "../goombarom.h"
#include <cstdlib>
#include <cstring>

using namespace System;
using namespace System::Collections::Generic;
using System::Collections::ObjectModel::ReadOnlyCollection;

namespace Goombasav {
	public ref class GBROM {
	private:
		array<unsigned char>^ data;

		GBROM(array<unsigned char>^ data) {
			this->data = data;
		}
	public:
		String^ ToString() override {
			pin_ptr<unsigned char> data_ptr = &data[0];
			const char* name = gb_get_title(data_ptr, NULL);
			return gcnew String(name);
		}
		property array<unsigned char>^ Data {
			array<unsigned char>^ get() {
				return data;
			}
		}

		uint32_t GetChecksum() {
			pin_ptr<unsigned char> p = &data[0];

			uint32_t sum = 0;
			int i;
			for (i = 0; i<128; i++) {
				sum += *p | (*(p + 1) << 8) | (*(p + 2) << 16) | (*(p + 3) << 24);
				p += 128;
			}
			return sum;
		}

		static List<GBROM^>^ Extract(array<unsigned char>^ source) {
			List<GBROM^>^ list = gcnew List<GBROM^>();

			pin_ptr<unsigned char> source_ptr = &source[0];
			const void* ptr = gb_first_rom(source_ptr, source->Length);
			while (ptr) {
				array<unsigned char>^ copy = gcnew array<unsigned char>(gb_rom_size(ptr));
				pin_ptr<unsigned char> copy_ptr = &copy[0];
				memmove(copy_ptr, ptr, copy->Length);
				list->Add(gcnew GBROM(copy));
				ptr = gb_next_rom(source_ptr, source->Length, ptr);
			}

			return list;
		}
	};
}