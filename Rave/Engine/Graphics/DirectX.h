#pragma once
#include "Engine/Core/SystemInclude.h"

#ifdef RV_PLATFORM_WINDOWS
#include <wrl.h>
namespace rv
{
	template<typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
}
#endif

#ifdef RV_RENDERER_D3D11
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")
#endif