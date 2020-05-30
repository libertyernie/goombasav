#pragma once
/* cli_ExtractedROM.h - interface to encapsulate ROM data extracted from another file

Copyright (C) 2016-2020 libertyernie

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

#include <cstdlib>
#include <cstring>
#include <cstdint>

using namespace System;
using namespace System::Collections::Generic;
using System::Collections::ObjectModel::ReadOnlyCollection;

namespace GoombasavCore {
	public interface class ExtractedROM {
	public:
		property array<unsigned char>^ Data {
			array<unsigned char>^ get();
		}
		property String^ Name {
			String^ get();
		}
		uint32_t GetChecksum();
	};
}