#pragma once
#include "Configdata.h"
#include "Stateheader.h"
#include <cstdlib>
#include <cstring>

using namespace System;
using namespace System::Collections::Generic;
using System::Collections::ObjectModel::ReadOnlyCollection;

namespace Goombasav {
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

			stateheader** headers = stateheader_scan(this->data);
			List<GoombaHeader^>^ list = gcnew List<GoombaHeader^>;
			for (int i = 0; headers[i] != NULL; i++) {
				if (headers[i]->type == GOOMBA_CONFIGSAVE) {
					list->Add(gcnew Configdata((configdata*)headers[i], this));
				} else {
					list->Add(gcnew Stateheader(headers[i], this));
				}
			}
			free(headers);
			this->headers = gcnew ReadOnlyCollection<GoombaHeader^>(list);
		}
	public:
		GoombaSRAM(array<unsigned char>^ arr, bool clean) {
			if (arr->Length < GOOMBA_COLOR_SRAM_SIZE) {
				throw gcnew GoombaException("Array length is smaller than " + GOOMBA_COLOR_SRAM_SIZE + " bytes");
			} else if (arr->Length > GOOMBA_COLOR_SRAM_SIZE) {
				throw gcnew GoombaException("Array length is larger than " + GOOMBA_COLOR_SRAM_SIZE + " bytes");
			}
			pin_ptr<unsigned char> pin = &arr[0];
			init(pin, clean);
		}

		GoombaSRAM(const void* ptr, bool clean) {
			init(ptr, clean);
		}

		~GoombaSRAM() {
			if (data != NULL) {
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

		array<unsigned char>^ ToArray() {
			array<unsigned char>^ arr = gcnew array<unsigned char>(GOOMBA_COLOR_SRAM_SIZE);
			pin_ptr<unsigned char> pin = &arr[0];
			memcpy(pin, this->data, GOOMBA_COLOR_SRAM_SIZE);
			return arr;
		}
	};
}