#include "Engine/Graphics/WindowRenderer.h"
#include "Engine/Graphics/Texture.h"
#include "Engine/Utility/Error.h"

#ifdef RV_RENDERER_D3D11

namespace rv
{
	namespace d3d11
	{
		Result WindowRenderer::Create(WindowRenderer& renderer, const Device& device, const WindowCreateInfo& window, const SwapChainCreateInfo& swap)
		{
			rv_result;
			rvrf(Window::Create(renderer.window, window));
			rvrf(SwapChain::Create(renderer.swap, device, renderer.window, swap));

			Texture2D backBuffer {};
			rvrhr(renderer.swap.swap->GetBuffer(0, IID_PPV_ARGS(&backBuffer.texture)));

			ComPtr<ID3D11RenderTargetView> target = nullptr;
			rvrhr(device.device->CreateRenderTargetView(backBuffer.texture.Get(), nullptr, &target));

			rvrf(Renderer::Create(renderer, target, &renderer.window.Size()));
			return result;
		}
		
		Result WindowRenderer::Present()
		{
			return swap.Present();
		}
	}
}

#endif