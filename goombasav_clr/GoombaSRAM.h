#include "HeaderPtr.h"

namespace Goombasav {
	public ref class GoombaLengthException : System::Exception {
	public:
		GoombaLengthException(System::String^ message) : Exception(message) {}
	};

	public ref class GoombaSRAM {
	private:
		char* data; // always GOOMBA_COLOR_SRAM_SIZE bytes, allocated with malloc
	public:
		GoombaSRAM(array<unsigned char>^ data);
		~GoombaSRAM();
		System::String^ ToString() override;

		System::Collections::Generic::List<HeaderPtr^>^ FindHeaders();
	};
}