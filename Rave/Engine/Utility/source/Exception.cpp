#include "Engine/Utility/Exception.h"
#include "Engine/Utility/String.h"
#include "Engine/Utility/Error.h"

namespace rv
{
	ErrorInfo::ErrorInfo(const char* filename, u64 line)
		:
		filename(filename),
		line(line)
	{
	}

	ResultException::ResultException(const Result& res, const std::string& message)
		:
		result(res)
	{
		const ResultInfo* info = res.info();
		if (info)
		{
			const ErrorInfo* e = dynamic_cast<const ErrorInfo*>(info);
			if (e)
			{
				line = e->line;
				filename = e->filename;
			}
		}
		format(message);
	}

	ResultException::ResultException(const Result& res, const char* filename, u64 line, const std::string& message)
		:
		result(res),
		filename(filename),
		line(line)
	{
		format(message);
	}

	const char* ResultException::what() const
	{
		return m_what.c_str();
	}

	const char* ResultException::get_filename() const
	{
		return filename;
	}

	u64 ResultException::get_line() const
	{
		return line;
	}

	const Result& ResultException::get_result() const
	{
		return result;
	}

	void ResultException::format(const std::string& message)
	{
		std::stringstream ss;
		ss << "Exception of type \"" << result.type().name() << "\" occurred!\n" << std::endl;
		ss << "Severity: " << result.severity() << std::endl;

		if (filename)
			ss << "File: " << filename << '\n'
			<< "Line: " << line << '\n' << '\n';

		const ResultInfo* info = result.info();
		if (info)
		{
			std::string opt_info = info->optional_info();
			if (opt_info.size() != 0)
				ss << opt_info << '\n' << std::endl;

			ss << "Description: " << info->description();

			bool msg = message.size() != 0;

			if (msg)
				ss << '\n' << std::endl << "Message: " << message;
		}

		m_what = ss.str();
	}
}