#include "Engine/Utility/Result.h"
#include "Engine/Utility/Exception.h"
#include "Engine/Utility/Error.h"

rv::ResultInfo::ResultInfo()
	:
	severity(RV_SEVERITY_NULL),
	type()
{
}

rv::ResultInfo::ResultInfo(const Identifier& type, const Severity& severity)
	:
	severity(severity),
	type(type)
{
}

std::string rv::ResultInfo::optional_info() const
{
	return {};
}

bool rv::ResultBase::succeeded(const Severity& severity) const
{
	if (has_info())
		return true;

	return info().severity <= severity;
}

bool rv::ResultBase::failed(const Severity& severity) const
{
	if (has_info())
		return info_unchecked().severity > severity;

	return false;
}

bool rv::ResultBase::has_info() const
{
	return pInfo.get();
}

void rv::ResultBase::expect(const char* errormessage) const
{
	expect(RV_SEVERITY_INFO, errormessage);
}

void rv::ResultBase::expect(const Severity& severity, const char* errormessage) const
{
	if (failed(severity))
		throw ResultException(pInfo, RV_FILE_LINE, errormessage);
}

const rv::ResultInfo& rv::ResultBase::info() const
{
	rv_assert_msg(pInfo.get(), "Cannot pass a reference to an ampty ResultInfo member, call has_info() to check");
	return *pInfo;
}

const rv::ResultInfo& rv::ResultBase::info_unchecked() const
{
	return *pInfo;
}
