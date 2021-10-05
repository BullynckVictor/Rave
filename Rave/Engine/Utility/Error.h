#pragma once
#include "Engine/Utility/Result.h"
#include "Engine/Utility/Exception.h"
#include "Engine/Core/CompileTimeInfo.h"
#include "Engine/Core/SystemInclude.h"

namespace rv
{
	static constexpr Identifier assertion_id	= "Assertion";
	static constexpr Identifier info_id			= "Info";
	static constexpr Identifier file_result_id	= "File Result";
	static constexpr Identifier unknown_error	= "Unknown Error";
	static constexpr Identifier ptr_result_id	= "Pointer Result";
	rv_only_windows(
	static constexpr Identifier hr_result_id	= "HRESULT";
	)

	struct InfoError : public ErrorInfo
	{
		InfoError() {}
		InfoError(const std::string& info);
		InfoError(std::string&& info);
		InfoError(const std::string& info, const char* source, u64 line);
		InfoError(std::string&& info, const char* source, u64 line);
		std::string description() const override;

		std::string info;
	};

	struct InfoResult : public ResultInfo
	{
		InfoResult() {}
		InfoResult(const std::string& info);
		InfoResult(std::string&& info);
		std::string description() const override;

		std::string info;
	};

	Result info(const std::string& info, const Severity& severity = RV_SEVERITY_INFO);
	Result info(std::string&& info, const Severity& severity = RV_SEVERITY_INFO);
	Result info(const std::string& info, const Severity& severity, const char* source, u64 line);
	Result info(std::string&& info, const Severity& severity, const char* source, u64 line);

	struct FailedAssertionError : public ErrorInfo
	{
		FailedAssertionError(const char* expression = nullptr);
		FailedAssertionError(const char* expression, const char* source, u64 line);
		std::string description() const override;

		const char* expression = nullptr;
	};

	Result assertion(bool cond, const char* expression, const char* file, u64 line);
	Result assertion(bool cond, const char* expression = nullptr);

	struct BadFileError : public ErrorInfo
	{
		BadFileError(const char* file = nullptr);
		BadFileError(const char* file, const char* source, u64 line);
		std::string description() const override;

		const char* file = nullptr;
	};

	Result file_result(const char* file, const char* source, u64 line);
	Result file_result(const char* file);

	struct NullPointerError : public ErrorInfo
	{
		NullPointerError(const char* pointer = nullptr);
		NullPointerError(const char* pointer, const char* source, u64 line);
		std::string description() const override;

		const char* pointer = nullptr;
	};

	Result void_pointer_result(const void* ptr, const char* ptrname, const char* source, u64 line);
	Result void_pointer_result(const void* ptr, const char* ptrname = nullptr);
	template<typename T>
	ResultValue<T*> pointer_result(T* ptr, const char* ptrname, const char* source, u64 line)
	{
		Result result = void_pointer_result(ptr, ptrname, source, line);
		if (result.failed())
			return result;
		return ptr;
	}
	template<typename T>
	ResultValue<T*> pointer_result(T* ptr, const char* ptrname = nullptr)
	{
		Result result = void_pointer_result(ptr, ptrname);
		if (result.failed())
			return result;
		return ptr;
	}

	Result assert_void_pointer_result(const void* ptr, const char* ptrname, const char* source, u64 line);
	Result assert_void_pointer_result(const void* ptr, const char* ptrname = nullptr);
	template<typename T>
	ResultValue<T*> assert_pointer_result(T* ptr, const char* ptrname, const char* source, u64 line)
	{
		if constexpr (cti.build.debug)
		{
			Result result = assert_void_pointer_result(ptr, ptrname, source, line);
			if (result.failed())
				return result;
		}
		return ptr;
	}
	template<typename T>
	ResultValue<T*> assert_pointer_result(T* ptr, const char* ptrname = nullptr)
	{
		if constexpr (cti.build.debug)
		{
			Result result = assert_void_pointer_result(ptr, ptrname);
			if (result.failed())
				return result;
		}
		return ptr;
	}

#	ifdef RV_PLATFORM_WINDOWS
	namespace win32
	{
		struct HrError : public ErrorInfo
		{
			HrError(HRESULT hr = (HRESULT)GetLastError());
			HrError(HRESULT hr, const char* source, u64 line);
			std::string description() const override;
			std::string optional_info() const override;

			HRESULT hresult;
		};

		Result hr_result(HRESULT hr, const char* source, u64 line);
		Result hr_result(HRESULT hr);
		Result last_result(const char* source, u64 line);
		Result last_result();
		Result last_result_onfail(bool cond, const char* source, u64 line);
		Result last_result_onfail(bool cond);
	}
#	endif

	extern Result failed;
}

#ifdef RV_DEBUG
#define rv_assertion(cond)				rv::assertion(cond, #cond, RV_FILE_LINE)
#define rv_assert(cond)					rv_assertion(cond).expect()
#define rv_assert_msg(cond, ms)			rv_assertion(cond).expect(msg)
#define rv_ptr_assertion(ptr)			rv::assert_pointer_result(ptr, #ptr, RV_FILE_LINE)
#define rv_assert_ptr(ptr)				rv_ptr_assertion(ptr).get()
#define rv_assert_ptr_msg(ptr, msg)		rv_ptr_assertion(ptr).get(msg)

//returns assertion on fail
#define rvra(cond)						rv_return_failed(rv_assertion(cond))
//returns pointer assertion on fail
#define rvrpa(ptr)						rv_return_failed(rv_ptr_assertion(ptr))

#else
#define rv_assertion(cond)				
#define rv_assert(cond)					
#define rv_assert_msg(cond, ms)			
#define rv_ptr_assertion(ptr)			
#define rv_assert_ptr(ptr)				
#define rv_assert_ptr_msg(ptr, msg)		
#define rvra(cond) rv_return_failed(rv_assertion(cond))
#define rvrpa(ptr) rv_return_failed(rv_ptr_assertion(ptr))
#endif

#define rv_ptr_result(ptr)				rv::pointer_result(ptr, #ptr, RV_FILE_LINE)
#define rv_expect_ptr(ptr)				pointer_result(ptr).get()
#define rv_expect_ptr_msg(ptr, msg)		pointer_result(ptr).get(msg)

//returns nullpointer error on fail
#define rvrp(ptr)						rv_return_failed(rv_ptr_result(ptr))

#ifdef RV_PLATFORM_WINDOWS
#define rv_hr_result(hr)				rv::win32::hr_result(hr)
#define rv_last_result()				rv::win32::last_result()
#define rv_last_result_onfail(cond)		rv::win32::last_result_onfail(cond, RV_FILE_LINE)
//returns hr error on fail
#define rvrhr(hr)						rv_return_failed(rv_hr_result(hr))
//returns last hr error
#define rvrlr()							rv_return_failed(rv_last_result())
//returns last hr error on fail
#define rvrlrof(cond)					rv_return_failed(rv_last_result_onfail(cond))
#endif

#define rv_info(msg)					rv::info(msg, rv::RV_SEVERITY_INFO)
#define rv_warning(msg)					rv::info(msg, rv::RV_SEVERITY_WARNING)
#define rv_error(msg)					rv::info(msg, rv::RV_SEVERITY_ERROR, RV_FILE_LINE)