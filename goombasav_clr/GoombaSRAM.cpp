#include <cstdlib>
#include <cstring>
#include "GoombaSRAM.h"
#include "../goombasav.h"

using System::String;
using System::Collections::Generic::List;
using System::Collections::ObjectModel::ReadOnlyCollection;

namespace Goombasav {
	GoombaSRAM::GoombaSRAM(array<unsigned char>^ arr) {
		if (arr->Length < GOOMBA_COLOR_SRAM_SIZE) {
			throw gcnew GoombaException("Array length is smaller than " + GOOMBA_COLOR_SRAM_SIZE + " bytes");
		} else if (arr->Length > GOOMBA_COLOR_SRAM_SIZE) {
			throw gcnew GoombaException("Array length is larger than " + GOOMBA_COLOR_SRAM_SIZE + " bytes");
		}
		this->data = (char*)malloc(GOOMBA_COLOR_SRAM_SIZE);
		pin_ptr<unsigned char> pin = &arr[0];
		memcpy(this->data, pin, GOOMBA_COLOR_SRAM_SIZE);
		RefreshHeaders();
	}

	GoombaSRAM::~GoombaSRAM() {
		if (data != NULL) {
			free(data);
			data = NULL;
		}
	}

	String^ GoombaSRAM::ToString() {
		return "Goomba SRAM: " + this->Headers->Count + " headers";
	}

	void GoombaSRAM::RefreshHeaders() {
		stateheader** headers = stateheader_scan(this->data);
		List<HeaderPtr^>^ list = gcnew List<HeaderPtr^>;
		for (int i = 0; headers[i] != NULL; i++) {
			list->Add(gcnew HeaderPtr(headers[i]));
		}
		free(headers);
		this->Headers = gcnew ReadOnlyCollection<HeaderPtr^>(list);
	}

	array<unsigned char>^ GoombaSRAM::Extract(HeaderPtr^ header) {
		if (!header->plausible()) throw gcnew System::InvalidOperationException("HeaderPtr object is invalid");
		goomba_size_t size;
		void* data = goomba_extract(this->data, header->sh_ptr(), &size);
		if (data == NULL) throw gcnew GoombaException(goomba_last_error());
		array<unsigned char>^ arr = gcnew array<unsigned char>(size);
		pin_ptr<unsigned char> pin = &arr[0];
		memcpy(pin, data, size);
		free(data);
		return arr;
	}

	array<unsigned char>^ GoombaSRAM::Replace(HeaderPtr^ header, array<unsigned char>^ data) {
		pin_ptr<unsigned char> pin = &data[0];
		void* new_data = goomba_new_sav(this->data, header->sh_ptr(), pin, data->Length);
		if (new_data == NULL) throw gcnew GoombaException(goomba_last_error());

		free(this->data);
		this->data = new_data;
		RefreshHeaders();
	}

	array<unsigned char>^ GoombaSRAM::ToArray() {
		array<unsigned char>^ arr = gcnew array<unsigned char>(GOOMBA_COLOR_SRAM_SIZE);
		pin_ptr<unsigned char> pin = &arr[0];
		memcpy(pin, this->data, GOOMBA_COLOR_SRAM_SIZE);
		return arr;
	}
}