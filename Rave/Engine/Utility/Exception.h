#pragma once
#include "Engine/Utility/Result.h"
#include <exception>

namespace rv
{
	struct ResultException : public std::exception
	{
	public:
		ResultException() = default;
		ResultException(const ResultInfo& info, const char* file, const u64 line, const std::string& message = {});
		ResultException(const ResultInfo& info, const std::string& message = {});
		ResultException(const std::shared_ptr<ResultInfo>& info, const char* file, const u64 line, const std::string& message = {});
		ResultException(const std::shared_ptr<ResultInfo>& info, const std::string& message = {});

		const char* what() const override;

	private:
		void format(const ResultInfo& info, const std::string& message = {});

		std::string m_what;
		const char* filename = nullptr;
		const u64 line = 0;
		std::shared_ptr<ResultInfo> pInfo;
	};
}

#define RV_FILE_LINE __FILE__, __LINE__