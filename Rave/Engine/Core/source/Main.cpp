#include "Engine/Core/Main.h"
#include "Engine/Core/SystemInclude.h"
#include "Engine/Utility/Severity.h"
#include "Engine/Utility/Exception.h"
#include <WinBase.h>

void message_box(const char* title, const char* message, rv::Severity severity = rv::RV_SEVERITY_ERROR)
{
	if constexpr (rv::cti.platform.windows)
	{
		UINT type = MB_OK;
		switch (severity)
		{
			case rv::RV_SEVERITY_INFO:
				type |= MB_ICONINFORMATION;
				break;
			case rv::RV_SEVERITY_WARNING:
				type |= MB_ICONWARNING;
				break;
			case rv::RV_SEVERITY_ERROR:
				type |= MB_ICONERROR;
				break;
		}
		MessageBox(nullptr, message, title, type);
	}
}

INT WINAPI WinMain(
	_In_		HINSTANCE	hInstance,
	_In_opt_	HINSTANCE	hPrevInstance,
	_In_		PSTR		lpCmdLine,
	_In_		INT			nCmdShow
)
{
	try
	{
		rave_main().expect("rave_main() exited with an erroneous result");
		return EXIT_SUCCESS;
	}
	catch (rv::ResultException& e)
	{
		message_box("rv::ResultException", e.what());
	}
	catch (const std::exception& e)
	{
		message_box("std::exception", e.what());
	}
	catch (...)
	{
		message_box("Unknown exception", "Something went wrong!");
	}
	return EXIT_FAILURE;
}