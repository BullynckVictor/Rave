#pragma once
#include "Engine/Utility/Result.h"
#include <stdexcept>

namespace rv
{
	struct ErrorInfo : public ResultInfo
	{
		ErrorInfo() = default;
		ErrorInfo(const char* filename, u64 line);

		const char* filename = nullptr;
		u64 line = 0;
	};

	struct ResultException : public std::exception
	{
	public:
		ResultException() = default;
		ResultException(const Result& result, const std::string & message = {});
		ResultException(const Result& result, const char* filename, u64 line, const std::string& message = {});

		const char* what() const override;
		const char* get_filename() const;
		u64 get_line() const;
		const Result& get_result() const;

	private:
		void format(const std::string& message = {});

		std::string m_what;
		const char* filename = nullptr;
		u64 line = 0;
		Result result;
	};
}

#define RV_FILE_LINE __FILE__, __LINE__