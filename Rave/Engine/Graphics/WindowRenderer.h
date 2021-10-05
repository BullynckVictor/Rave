#pragma once
#include "Engine/Core/CompileTimeInfo.h"
#include "Engine/Core/Window.h"
#include "Engine/Graphics/Renderer.h"
#include "Engine/Graphics/SwapChain.h"

#ifdef RV_RENDERER_D3D11
namespace rv
{
	namespace d3d11
	{
		class WindowRenderer : public Renderer
		{
		public:
			WindowRenderer() {}
			static Result Create(WindowRenderer& renderer, const Device& device, const WindowCreateInfo& window = {}, const SwapChainCreateInfo& swap = {});

			Result Present();

		public:
			Window window;

		private:
			SwapChain swap;
		};
	}

	typedef d3d11::WindowRenderer WindowRenderer;
}
#endif