#pragma once
#include "Engine/Utility/Result.h"

#ifdef RV_RENDERER_D3D11
#include "Engine/Graphics/DirectX.h"
#include "Engine/Graphics/Device.h"
#include "Engine/Core/Window.h"

namespace rv
{
	namespace d3d11
	{
		struct SwapChainCreateInfo
		{
			bool vsync = true;
			uint nBuffers = 2;
			DXGI_FORMAT format = DXGI_FORMAT_B8G8R8A8_UNORM;
		};

		struct SwapChain
		{
			static Result Create(SwapChain& swap, const Device& device, const Window& window, const SwapChainCreateInfo& info = {});

			Result Present();

			ComPtr<IDXGISwapChain> swap;
			bool vsync = true;
		};
	}
}

#endif