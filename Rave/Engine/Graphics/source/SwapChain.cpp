#include "Engine/Graphics/SwapChain.h"
#include "Engine/Utility/Error.h"

#ifdef RV_RENDERER_D3D11

namespace rv
{
	namespace d3d11
	{
		Result SwapChain::Create(SwapChain& swap, const Device& device, const Window& window, const SwapChainCreateInfo& info)
		{
			rv_result;

			swap.vsync = info.vsync;

			ComPtr<IDXGIAdapter> adapter = nullptr;
			rvrhr(device.dxgi->GetAdapter(&adapter));

			ComPtr<IDXGIFactory> factory = nullptr;
			rvrhr(adapter->GetParent(IID_PPV_ARGS(&factory)));

			DXGI_SWAP_CHAIN_DESC desc {0};
			desc.Windowed = true;
			desc.BufferDesc.Width = window.Size().width;
			desc.BufferDesc.Height = window.Size().height;
			desc.BufferDesc.Format = info.format;
			desc.BufferCount = info.nBuffers;
			desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
			desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
			desc.OutputWindow = window.Handle();

			rvrhr(factory->CreateSwapChain(
				device.device.Get(),
				&desc,
				&swap.swap
			));

			return result;
		}
        
		Result SwapChain::Present()
        {
            return rv_hr_result(swap->Present(vsync, 0));
        }
	}
}

#endif