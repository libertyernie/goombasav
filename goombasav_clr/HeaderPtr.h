#pragma once
#include "../goombasav.h"

ref class HeaderPtr {
public:
	stateheader* sh_ptr();
	configdata* cd_ptr();
	bool plausible();

	HeaderPtr(void* ptr);
	virtual System::String^ ToString() override;
private:
	void* ptr;
};
