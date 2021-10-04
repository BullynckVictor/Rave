#include "Engine/Utility/Error.h"
#include "Engine/Utility/String.h"
#include "Engine/Utility/Exception.h"
#include "Engine/Utility/hresults.h"
#include <comdef.h>
#include <filesystem>

namespace rv
{
	Result failed = Result(RV_SEVERITY_ERROR, unknown_error);

	FailedAssertionError::FailedAssertionError(const char* expression)
		:
		ErrorInfo(),
		expression(expression)
	{
	}

	FailedAssertionError::FailedAssertionError(const char* expression, const char* source, u64 line)
		:
		ErrorInfo(source, line),
		expression(expression)
	{
	}

	std::string FailedAssertionError::description() const
	{
		return str("Assertion \"", expression, "\" failed");
	}

	Result assertion(bool cond, const char* expression, const char* file, u64 line)
	{
		if constexpr (cti.build.debug)
			return cond ? Result(RV_SEVERITY_INFO, assertion_id) : Result(RV_SEVERITY_ERROR, assertion_id, new FailedAssertionError(expression, file, line));
		return Result(RV_SEVERITY_INFO, assertion_id);
	}
	Result assertion(bool cond, const char* expression)
	{
		if constexpr (cti.build.debug)
			return cond ? Result(RV_SEVERITY_INFO, assertion_id) : Result(RV_SEVERITY_ERROR, assertion_id, new FailedAssertionError(expression));
		return Result(RV_SEVERITY_INFO, assertion_id);
	}

	BadFileError::BadFileError(const char* file)
		:
		ErrorInfo(),
		file(file)
	{
	}

	BadFileError::BadFileError(const char* file, const char* source, u64 line)
		:
		ErrorInfo(source, line),
		file(file)
	{
	}

	std::string BadFileError::description() const
	{
		return str("Unable to open file \"", file, '\"');
	}

	Result file_result(const char* file, const char* source, u64 line)
	{
		if (!std::filesystem::exists(file))
			return Result(RV_SEVERITY_ERROR, file_result_id, new BadFileError(file, source, line));
		return Result(RV_SEVERITY_INFO, file_result_id);
	}
	Result file_result(const char* file)
	{
		if (!std::filesystem::exists(file))
			return Result(RV_SEVERITY_ERROR, file_result_id, new BadFileError(file));
		return Result(RV_SEVERITY_INFO, file_result_id);
	}

	NullPointerError::NullPointerError(const char* pointer)
		:
		ErrorInfo(),
		pointer(pointer)
	{
	}

	NullPointerError::NullPointerError(const char* pointer, const char* source, u64 line)
		:
		ErrorInfo(source, line),
		pointer(pointer)
	{
	}

	std::string NullPointerError::description() const
	{
		return str("Pointer \"", pointer, "\" was null");
	}

	Result void_pointer_result(const void* ptr, const char* ptrname, const char* source, u64 line)
	{
		if (ptr)
			return Result(RV_SEVERITY_INFO, ptr_result_id);
		return Result(RV_SEVERITY_ERROR, ptr_result_id, new NullPointerError(ptrname, source, line));
	}
	Result void_pointer_result(const void* ptr, const char* ptrname)
	{
		if (ptr)
			return Result(RV_SEVERITY_INFO, ptr_result_id);
		return Result(RV_SEVERITY_ERROR, ptr_result_id, new NullPointerError(ptrname));
	}
	Result assert_void_pointer_result(const void* ptr, const char* ptrname, const char* source, u64 line)
	{
		if constexpr (cti.build.debug)
			return void_pointer_result(ptr, ptrname, source, line);
		else
			return Result(RV_SEVERITY_INFO, ptr_result_id);
	}
	Result assert_void_pointer_result(const void* ptr, const char* ptrname)
	{
		if constexpr (cti.build.debug)
			return void_pointer_result(ptr, ptrname);
		else
			return Result(RV_SEVERITY_INFO, ptr_result_id);
	}

#	ifdef RV_PLATFORM_WINDOWS
	namespace win32
	{
		HrError::HrError(HRESULT hr)
			:
			ErrorInfo(),
			hresult(hr)
		{
		}
		
		HrError::HrError(HRESULT hr, const char* source, u64 line)
			:
			ErrorInfo(source, line),
			hresult(hr)
		{
		}
		
		std::string HrError::description() const
		{
			return _com_error(hresult).ErrorMessage();
		}
		
		std::string HrError::optional_info() const
		{
			return str("HRESULT: ", get_hr_string(hresult));
		}

		Result hr_result(HRESULT hr, const char* source, u64 line)
		{
			if (FAILED(hr))
				return Result(RV_SEVERITY_ERROR, hr_result_id, new HrError(hr, source, line));
			return Result(RV_SEVERITY_INFO, hr_result_id);
		}
		Result hr_result(HRESULT hr)
		{
			if (FAILED(hr))
				return Result(RV_SEVERITY_ERROR, hr_result_id, new HrError(hr));
			return Result(RV_SEVERITY_INFO, hr_result_id);
		}
		Result win32::last_result(const char* source, u64 line)
		{
			return hr_result((HRESULT)GetLastError(), source, line);
		}
		Result win32::last_result()
		{
			return hr_result((HRESULT)GetLastError());
		}
		Result win32::last_result_onfail(bool cond, const char* source, u64 line)
		{
			if (cond)
				return Result(RV_SEVERITY_INFO, hr_result_id);
			return Result(RV_SEVERITY_ERROR, hr_result_id, new HrError((HRESULT)GetLastError(), source, line));
		}
		Result win32::last_result_onfail(bool cond)
		{
			if (cond)
				return Result(RV_SEVERITY_INFO, hr_result_id);
			return Result(RV_SEVERITY_ERROR, hr_result_id, new HrError((HRESULT)GetLastError()));
		}
	}
#	endif
}