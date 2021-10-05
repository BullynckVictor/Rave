#pragma once
#include "Engine/Core/SystemInclude.h"
#include "Engine/Utility/Result.h"

#ifdef RV_RENDERER_D3D11
#include "Engine/Graphics/DirectX.h"
#include <vector>

namespace rv
{
	namespace d3d11
	{
		struct DeviceCreateInfo
		{
			std::vector<D3D_FEATURE_LEVEL> featureLevels = {
				D3D_FEATURE_LEVEL_9_1,
				D3D_FEATURE_LEVEL_9_2,
				D3D_FEATURE_LEVEL_9_3,
				D3D_FEATURE_LEVEL_10_0,
				D3D_FEATURE_LEVEL_10_1,
				D3D_FEATURE_LEVEL_11_0,
				D3D_FEATURE_LEVEL_11_1
			};
			uint flags = 0;
		};

		struct Device
		{
			Device() {};
			~Device();
			static Result Create(Device& device, const DeviceCreateInfo& info = {});

			ComPtr<ID3D11Device> device;
			ComPtr<IDXGIDevice> dxgi;
			ComPtr<ID3D11DeviceContext> context;
		};
	}
}

#endif