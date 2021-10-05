#pragma once
#include "Engine/Graphics/Device.h"

#ifdef RV_RENDERER_D3D11
namespace rv
{
	namespace d3d11
	{
		struct Texture2D
		{
			ComPtr<ID3D11Texture2D> texture;
		};
	}
}
#endif