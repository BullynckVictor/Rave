#pragma once
#include "Engine/Core/CompileTimeInfo.h"
#include "Engine/Utility/Result.h"
#include "Engine/Utility/Vector.h"

#ifdef RV_RENDERER_D3D11
#include "Engine/Graphics/Device.h"
namespace rv
{
	namespace d3d11
	{
		class Renderer
		{
		public:
			static Result Create(Renderer& renderer, const ComPtr<ID3D11RenderTargetView>& target, const Extent<uint>* size);
			Result Render(const Device& device);
			Result Render(const Device& device, const Identifier& layer);

		private:
			Result RenderLayer(const Device& device, const Identifier& layer);

		private:
			ComPtr<ID3D11RenderTargetView> target;
			const Extent<uint>* size = nullptr;
		};
	}

	typedef d3d11::Renderer Renderer;
}
#endif