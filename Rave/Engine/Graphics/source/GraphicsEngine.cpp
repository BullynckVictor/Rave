#include "Engine/Graphics/GraphicsEngine.h"
#include "Engine/Utility/Error.h"
#include "Engine/Utility/String.h"

#ifdef RV_RENDERER_D3D11
namespace rv
{
	namespace d3d11
	{
		Result GraphicsEngine::Create(GraphicsEngine& engine)
		{
			rv_result;
			rvrf(Device::Create(engine.device));
			return result;
		}
		
		ResultValue<WindowRenderer*> GraphicsEngine::CreateRenderer(const Identifier& renderer, const WindowCreateInfo& window, const SwapChainCreateInfo& swap)
		{
			rv_result;
			ResultValue<WindowRenderer*> out = CreateNewRenderer<WindowRenderer>(renderer);
			rvrf(out);
			rvrf(WindowRenderer::Create(*out.value(), device, window, swap));
			return out;
		}
		
		ResultValue<WindowRenderer*> GraphicsEngine::CreateRenderer(const Identifier& renderer, const std::string& title, uint width, uint height, bool vsync)
		{
			WindowCreateInfo window{};
			SwapChainCreateInfo swap{};

			window.title = title;
			window.size = { width, height };

			swap.vsync = vsync;

			return CreateRenderer(renderer, window, swap);
		}

		Result GraphicsEngine::Render()
		{
			rv_result;
			for (const auto& renderer : renderers)
				rvrf(renderer.second->Render(device));
			return result;
		}

		Result GraphicsEngine::Render(const Identifier& layer)
		{
			rv_result;
			for (const auto& renderer : renderers)
				rvrf(renderer.second->Render(device, layer));
			return result;
		}

		template<typename T>
		ResultValue<T*> GraphicsEngine::CreateNewRenderer(const Identifier& renderer)
		{
			if (renderers.find(renderer) != renderers.end())
				return rv_error(str("Renderer \"", renderer.name(), "\" already exists"));

			std::unique_ptr<Renderer> newr = std::unique_ptr<Renderer>(new T());
			Renderer* r = newr.get();
			renderers[renderer] = std::move(newr);
			return reinterpret_cast<T*>(r);
		}
	}
}
#endif