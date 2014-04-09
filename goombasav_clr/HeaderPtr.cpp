#include "HeaderPtr.h"

HeaderPtr::HeaderPtr(void* ptr) {
	this->ptr = ptr;
}

stateheader* HeaderPtr::sh_ptr() {
	return (stateheader*)this->ptr;
}

configdata* HeaderPtr::cd_ptr() {
	return (configdata*)this->ptr;
}

bool HeaderPtr::plausible() {
	return stateheader_plausible(this->sh_ptr());
}

System::String^ HeaderPtr::ToString() {
	return gcnew System::String(this->sh_ptr()->title);
}
