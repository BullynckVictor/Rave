#pragma once
#include "Engine/Core/SystemInclude.h"

#ifdef RV_PLATFORM_WINDOWS
namespace rv
{
	namespace win32
	{
		extern const char* get_hr_string(HRESULT hr);
	}
}
#endif