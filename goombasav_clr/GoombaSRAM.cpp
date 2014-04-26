#include <cstdlib>
#include <cstring>
#include "GoombaSRAM.h"
#include "../goombasav.h"

using System::String;
using System::Collections::Generic::List;

namespace Goombasav {
	GoombaSRAM::GoombaSRAM(array<unsigned char>^ arr) {
		if (arr->Length < GOOMBA_COLOR_SRAM_SIZE) {
			throw gcnew GoombaLengthException("Array length is smaller than " + GOOMBA_COLOR_SRAM_SIZE + " bytes");
		} else if (arr->Length > GOOMBA_COLOR_SRAM_SIZE) {
			throw gcnew GoombaLengthException("Array length is larger than " + GOOMBA_COLOR_SRAM_SIZE + " bytes");
		}
		this->data = (char*)malloc(GOOMBA_COLOR_SRAM_SIZE);
		pin_ptr<unsigned char> pin = &arr[0];
		memcpy(this->data, pin, GOOMBA_COLOR_SRAM_SIZE);
	}

	GoombaSRAM::~GoombaSRAM() {
		if (data != NULL) {
			free(data);
			data = NULL;
		}
	}

	String^ GoombaSRAM::ToString() {
		return gcnew String(stateheader_summary_str((stateheader*)(data + 4)));
	}

	List<HeaderPtr^>^ GoombaSRAM::FindHeaders() {
		stateheader** headers = stateheader_scan(this->data);
		List<HeaderPtr^>^ list = gcnew List<HeaderPtr^>;
		for (int i = 0; headers[i] != NULL; i++) {
			list->Add(gcnew HeaderPtr(headers[i]));
		}
		free(headers);
		return list;
	}
}