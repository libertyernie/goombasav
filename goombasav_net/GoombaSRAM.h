#include "HeaderPtr.h"

namespace Goombasav {
	public ref class GoombaException : System::Exception {
	public:
		GoombaException(System::String^ message) : Exception(message) {}
		GoombaException(const char* message) : GoombaException(gcnew System::String(message)) {}
	};

	public ref class GoombaSRAM {
	private:
		void* data; // always GOOMBA_COLOR_SRAM_SIZE bytes, allocated with malloc
	public:
		// HeaderPtr objects are invalid after data is replaced in the SRAM.
		System::Collections::ObjectModel::ReadOnlyCollection<HeaderPtr^>^ Headers;

		GoombaSRAM(array<unsigned char>^ data);
		GoombaSRAM(const void* data);
		void init(const void* data);
		~GoombaSRAM();
		System::String^ ToString() override;

		array<unsigned char>^ Extract(HeaderPtr^ header);
		GoombaSRAM^ CopyAndReplace(HeaderPtr^ header, array<unsigned char>^ data);

		array<unsigned char>^ ToArray();
	};
}