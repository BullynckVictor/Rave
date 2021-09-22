#pragma once
#include "Engine/Utility/Result.h"
#include "Engine/Core/CompileTimeInfo.h"

namespace rv
{
	struct ErrorInfo : public ResultInfo
	{
		ErrorInfo() = default;
		ErrorInfo(const Identifier& type);
	};

	static constexpr Identifier assertion_failed	= "Assertion Failed";
	static constexpr Identifier bad_file			= "Bad File";
	static constexpr Identifier unknown_error		= "Unknown Error";

	struct FailedAssertionError : public ErrorInfo
	{
		static constexpr Identifier identifier = assertion_failed;

		FailedAssertionError(const char* expression = nullptr);
		std::string description() const override;

		const char* expression = nullptr;
	};

	struct BadFileError : public ErrorInfo
	{
		static constexpr Identifier identifier = assertion_failed;

		BadFileError(const char* file = nullptr);
		std::string description() const override;

		const char* file = nullptr;
	};

	struct UnknownError : public ErrorInfo
	{
		static constexpr Identifier identifier = unknown_error;

		std::string description() const override;
	};


	void assert_func(bool cond, const char* expression, const char* file, u64 line, const std::string& message = {});
}

#ifdef RV_DEBUG
#define rv_assert(cond)				rv::assert_func(cond, #cond, __FILE__, __LINE__)
#define rv_assert_msg(cond, msg)	rv::assert_func(cond, #cond, __FILE__, __LINE__, msg)
#else
#define rv_assert(cond)
#define rv_assert_msg(cond, msg)
#endif