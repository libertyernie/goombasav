#pragma once
/* cli_GoombaSRAM.h - class to encapsulate Goomba / Goomba Color SRAM

Copyright (C) 2014-2020 libertyernie

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

#include "cli_PocketNESConfigdata.h"
#include "cli_GoombaConfigdata.h"
#include "cli_SMSAdvanceConfigdata.h"
#include "cli_Stateheader.h"
#include <cstdlib>
#include <cstring>

using namespace System;
using namespace System::Collections::Generic;
using System::Collections::ObjectModel::ReadOnlyCollection;

namespace GoombasavCore {
	public ref class GoombaException : System::Exception {
	public:
		GoombaException(String^ message) : Exception(message) {}
		GoombaException(const char* message) : GoombaException(gcnew String(message)) {}
	};

	public ref class GoombaSRAM {
	private:
		// always GOOMBA_COLOR_SRAM_SIZE bytes, allocated with malloc
		void* data;
		
		// HeaderPtr objects are invalid after data is replaced in the SRAM.
		ReadOnlyCollection<GoombaHeader^>^ headers;

		void init(const void* ptr, bool clean)  {
			this->data = (char*)malloc(GOOMBA_COLOR_SRAM_SIZE);
			memcpy(this->data, ptr, GOOMBA_COLOR_SRAM_SIZE);

			if (clean) {
				char* cleaned = goomba_cleanup(this->data);
				if (cleaned == NULL) {
					throw gcnew GoombaException(goomba_last_error());
				} else if (cleaned != this->data) {
					free(this->data);
					this->data = cleaned;
				}
			}

			uint32_t tag = *(uint32_t*)this->data;

			const stateheader** headers = stateheader_scan(this->data);
			if (headers == NULL) throw gcnew GoombaException(goomba_last_error());
			List<GoombaHeader^>^ list = gcnew List<GoombaHeader^>;
			for (int i = 0; headers[i] != NULL; i++) {
				if (headers[i]->type == GOOMBA_CONFIGSAVE) {
					switch (tag) {
					case POCKETNES_STATEID:
					case POCKETNES_STATEID2:
						list->Add(gcnew PocketNESConfigdata((pocketnes_configdata*)headers[i], this));
						break;
					default:
						configdata* cd = (configdata*)headers[i];
						if (cd->size == sizeof(smsadvance_configdata)) {
							list->Add(gcnew SMSAdvanceConfigdata((smsadvance_configdata*)headers[i], this));
						} else {
							list->Add(gcnew GoombaConfigdata((goomba_configdata*)headers[i], this));
						}
						break;
					}
				} else {
					list->Add(gcnew Stateheader(headers[i], this));
				}
			}
			free(headers);
			this->headers = gcnew ReadOnlyCollection<GoombaHeader^>(list);
		}
	public:
		static int ExpectedSize = GOOMBA_COLOR_SRAM_SIZE;

		// arr must be at least ExpectedSize bytes. If it is more, the extra bytes will be ignored.
		GoombaSRAM(array<unsigned char>^ arr, bool clean) {
			if (arr->Length < GOOMBA_COLOR_SRAM_SIZE) {
				array<unsigned char>^ arr2 = gcnew array<unsigned char>(GOOMBA_COLOR_SRAM_SIZE);
				Array::Copy(arr, arr2, arr->Length);
				arr = arr2;
			}
			pin_ptr<unsigned char> pin = &arr[0];
			init(pin, clean);
		}

		GoombaSRAM(const void* ptr, bool clean) {
			init(ptr, clean);
		}

		~GoombaSRAM() {
			this->!GoombaSRAM(); // call finalizer
		}

		!GoombaSRAM() {
			if (data != NULL) { // safe to run more than once, because Dispose() will call it
				free(data);
				data = NULL;
			}
		}

		property ReadOnlyCollection<GoombaHeader^>^ Headers {
			ReadOnlyCollection<GoombaHeader^>^ get() {
				return headers;
			}
		}

		String^ ToString() override {
			return "Goomba SRAM: " + this->Headers->Count + " headers";
		}

		array<unsigned char>^ Extract(Stateheader^ header) {
			if (!header->IsPlausible()) throw gcnew System::InvalidOperationException("HeaderPtr object is invalid");
			goomba_size_t size;
			void* data = goomba_extract(this->data, header->Pointer, &size);
			if (data == NULL) throw gcnew GoombaException(goomba_last_error());
			array<unsigned char>^ arr = gcnew array<unsigned char>(size);
			pin_ptr<unsigned char> pin = &arr[0];
			memcpy(pin, data, size);
			free(data);
			return arr;
		}

		GoombaSRAM^ CopyAndReplace(Stateheader^ header, array<unsigned char>^ data) {
			pin_ptr<unsigned char> pin = &data[0];
			void* new_data = goomba_new_sav(this->data, header->Pointer, pin, data->Length);
			if (new_data == NULL) throw gcnew GoombaException(goomba_last_error());

			return gcnew GoombaSRAM(new_data, false);
		}

		GoombaSRAM^ CopyAndRemove(Stateheader^ header) {
			void* new_data = goomba_new_sav(this->data, header->Pointer, NULL, 0);
			if (new_data == NULL) throw gcnew GoombaException(goomba_last_error());

			return gcnew GoombaSRAM(new_data, false);
		}

		array<unsigned char>^ ToArray() {
			array<unsigned char>^ arr = gcnew array<unsigned char>(GOOMBA_COLOR_SRAM_SIZE);
			pin_ptr<unsigned char> pin = &arr[0];
			memcpy(pin, this->data, GOOMBA_COLOR_SRAM_SIZE);
			return arr;
		}
	};
}