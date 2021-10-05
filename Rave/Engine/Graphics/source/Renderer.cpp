#include "Engine/Graphics/Renderer.h"
#include "Engine/Utility/Error.h"

#ifdef RV_RENDERER_D3D11

namespace rv
{
	namespace d3d11
	{
		Result Renderer::Create(Renderer& renderer, const ComPtr<ID3D11RenderTargetView>& target, const Extent<uint>* size)
		{
			rv_result;
			renderer.target = target;
			renderer.size = size;
			rvrp(renderer.size);
			return result;
		}
		
		Result Renderer::Render(const Device& device)
		{
			D3D11_VIEWPORT viewport {0};
			viewport.Width = (float)size->width;
			viewport.Height = (float)size->height;
			viewport.MinDepth = 0;
			viewport.MaxDepth = 1;
			device.context->RSSetViewports(1, &viewport);

			return rv::success;
		}
		
		Result Renderer::Render(const Device& device, const Identifier& layer)
		{
			D3D11_VIEWPORT viewport{ 0 };
			viewport.Width = (float)size->width;
			viewport.Height = (float)size->height;
			viewport.MinDepth = 0;
			viewport.MaxDepth = 1;
			device.context->RSSetViewports(1, &viewport);

			return RenderLayer(device, layer);
		}
		
		Result Renderer::RenderLayer(const Device& device, const Identifier& layer)
		{
			rv_result;

			return result;
		}
	}
}

#endif