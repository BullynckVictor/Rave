#include "Engine/Utility/Exception.h"
#include "Engine/Utility/String.h"
#include "Engine/Utility/Error.h"

rv::ResultException::ResultException(const ResultInfo& info, const char* file, const u64 line, const std::string& message)
	:
	filename(file),
	line(line)
{
	format(info, message);
}

rv::ResultException::ResultException(const ResultInfo& info, const std::string& message)
{
	format(info, message);
}

rv::ResultException::ResultException(const std::shared_ptr<ResultInfo>& info, const char* file, const u64 line, const std::string& message)
	:
	filename(file),
	line(line),
	pInfo(info)
{
	rv_assert_msg(info.get(), "A valid std::shared_ptr<ResultInfo> must be passed to ResultException");
	format(*info, message);
}

rv::ResultException::ResultException(const std::shared_ptr<ResultInfo>& info, const std::string& message)
	:
	pInfo(info)
{
	rv_assert_msg(info.get(), "A valid std::shared_ptr<ResultInfo> must be passed to ResultException");
	format(*info, message);
}

const char* rv::ResultException::what() const
{
	return m_what.c_str();
}

void rv::ResultException::format(const ResultInfo& info, const std::string& message)
{
	std::stringstream ss;
	ss << "Exception of type \"" << info.type.name() << "\" occurred!\n" << std::endl;

	if (filename)
		ss << "File: " << filename << '\n'
		   << "Line: " << line << '\n' << '\n';

	std::string opt_info = info.optional_info();
	if (opt_info.size() != 0)
		ss << opt_info << '\n' << std::endl;

	ss << "Description: " << info.description();

	if (message.size() != 0)
		ss << '\n' << std::endl << "Message: " << message;

	m_what = ss.str();
}
