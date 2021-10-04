#include "Engine/Utility/Result.h"
#include "Engine/Utility/Exception.h"
#include "Engine/Utility/Error.h"

namespace rv
{
	Result success = Result(RV_SEVERITY_INFO, success_id);

	std::string ResultInfo::optional_info() const
	{
		return std::string();
	}

	Result::Result(const Severity& sev, const Identifier& id, ResultInfo* pinfo)
		:
		m_severity(sev),
		m_type(id),
		m_info(pinfo)
	{
	}
	
	const Identifier& Result::type() const
	{
		return m_type;
	}
	
	const Severity& Result::severity() const
	{
		return m_severity;
	}
	
	const ResultInfo* Result::info() const
	{
		return m_info.get();
	}
	
	bool Result::has_info() const
	{
		return info();
	}
	
	const ResultInfo& Result::get_info() const
	{
		if (has_info())
			return *info();
		throw std::runtime_error("deref null **temp** (result.cpp)");
	}
	
	bool Result::succeeded(const Flags<Severity>& sev) const
	{
		return sev.contains(severity());
	}
	
	bool Result::failed(const Flags<Severity>& sev) const
	{
		return !succeeded(sev);
	}

	void Result::expect(const Flags<Severity>& sev) const
	{
		if (failed(sev))
			throw_exception();
	}
	
	void Result::expect(const std::string& message, const Flags<Severity>& sev) const
	{
		if (failed(sev))
			throw_exception(message);
	}
	
	void Result::throw_exception(const std::string& message) const
	{
		throw ResultException(*this, message);
	}
}