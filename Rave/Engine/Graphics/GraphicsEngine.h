#pragma once
#include "Engine/Graphics/Device.h"
#include "Engine/Graphics/Renderer.h"
#include "Engine/Graphics/WindowRenderer.h"
#include "Engine/Utility/Identifier.h"
#include <map>

#ifdef RV_RENDERER_D3D11
namespace rv
{
	namespace d3d11
	{
		class GraphicsEngine
		{
		public:
			GraphicsEngine() {}
			static Result Create(GraphicsEngine& engine);

			ResultValue<WindowRenderer*> CreateRenderer(const Identifier& renderer, const WindowCreateInfo& window, const SwapChainCreateInfo& swap);
			ResultValue<WindowRenderer*> CreateRenderer(const Identifier& renderer, const std::string& title, uint width, uint height, bool vsync);

			Result Render();
			Result Render(const Identifier& layer);

		private:
			template<typename T>
			ResultValue<T*> CreateNewRenderer(const Identifier& renderer);

		private:
			Device device;
			std::map<Identifier, std::unique_ptr<Renderer>> renderers;
		};
	}

	typedef d3d11::GraphicsEngine GraphicsEngine;
}
#endif