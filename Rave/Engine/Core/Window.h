#pragma once
#include "Engine/Core/SystemInclude.h"
#include "Engine/Utility/Result.h"
#include "Engine/Utility/types.h"
#include "Engine/Utility/Vector.h"

namespace rv
{
	struct WindowInfo
	{
		const char* title = nullptr;
		ExtentU size = 0;
	};

	class Window
	{
	public:
		static Result<void> Create(const char* title, uint width, uint height);
		static Result<void> Create(const char* title, uint width, uint height);

	private:
		ExtentU size;
	};
}