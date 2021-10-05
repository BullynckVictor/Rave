#include "Engine/Graphics/Device.h"
#include "Engine/Utility/Error.h"

#ifdef RV_RENDERER_D3D11

namespace rv
{
	namespace d3d11
	{
        Device::~Device()
        {
			context->ClearState();
			context->Flush();
        }

        Result Device::Create(Device& device, const DeviceCreateInfo& info)
		{
			rv_result;

			uint flags = info.flags | (cti.build.debug ? D3D11_CREATE_DEVICE_DEBUG : 0);
			rvrhr(D3D11CreateDevice(
				nullptr,
				D3D_DRIVER_TYPE_HARDWARE,
				0,
				flags,
				info.featureLevels.data(),
				(uint)info.featureLevels.size(),
				D3D11_SDK_VERSION,
				&device.device,
				nullptr,
				&device.context
			));

			rvrhr(device.device->QueryInterface<IDXGIDevice>(&device.dxgi));

			return result;
		}
	}
}

#endif