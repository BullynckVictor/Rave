#include "Engine/Utility/Error.h"
#include "Engine/Utility/String.h"
#include "Engine/Utility/Exception.h"

rv::ErrorInfo::ErrorInfo(const Identifier& identifier)
	:
	ResultInfo(identifier, RV_SEVERITY_ERROR)
{
}

rv::FailedAssertionError::FailedAssertionError(const char* expression)
	:
	ErrorInfo(identifier),
	expression(expression)
{
}

std::string rv::FailedAssertionError::description() const
{
	return str("Assertion \"", expression, "\" failed");
}

rv::BadFileError::BadFileError(const char* file)
	:
	ErrorInfo(identifier),
	file(file)
{
}

std::string rv::BadFileError::description() const
{
	return str("Unable to open file \"", file, '\"');
}

void rv::assert_func(bool cond, const char* expression, const char* file, u64 line, const std::string& message)
{
	if constexpr (cti.build.debug)
	{
		if (!cond)
			throw ResultException(FailedAssertionError(expression), file, line, message);
	}
}

rv::UnknownError::UnknownError()
	:
	ErrorInfo(identifier)
{
}

std::string rv::UnknownError::description() const
{
	return "An unknown error occurred";
}
