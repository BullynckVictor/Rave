#pragma once
#include <memory>
#include "Engine/Utility/Severity.h"
#include "Engine/Utility/Identifier.h"
#include "Engine/Utility/Flags.h"

namespace rv
{
	struct ResultInfo
	{
		virtual ~ResultInfo() {}

		virtual std::string description() const = 0;
		virtual std::string optional_info() const;
	};

	struct Result
	{
	public:
		Result() = default;
		Result(const Severity& severity, const Identifier& type, ResultInfo* info = nullptr);

		const Identifier& type() const;
		const Severity& severity() const;
		const ResultInfo* info() const;

		bool has_info() const;
		const ResultInfo& get_info() const;
		template<typename R>
		const R& get_info_as() const { return dynamic_cast<const R&>(get_info()); }

		bool succeeded(const Flags<Severity>& severity = RV_SEVERITY_INFO) const;
		bool failed(const Flags<Severity>& severity = RV_SEVERITY_INFO) const;

		void expect(const Flags<Severity>& severity = RV_SEVERITY_INFO) const;
		void expect(const std::string& message, const Flags<Severity>& severity = RV_SEVERITY_INFO) const;

		void throw_exception(const std::string& message = {}) const;

	protected:
		Identifier m_type;
		Severity m_severity;
		std::shared_ptr<ResultInfo> m_info;
	};

	static constexpr Identifier success_id = "Success";
	extern Result success;

	template<typename T>
	struct ResultValue : public Result
	{
	public:
		ResultValue(const T& v = {}) : Result(success), m_value(v) {}
		ResultValue(T&& v) : Result(success), m_value(std::move(v)) {}
		ResultValue(const ResultValue& rhs) : Result(rhs), m_value(rhs.m_value) {}
		ResultValue(ResultValue&& rhs) : Result(rhs), m_value(std::move(rhs.m_value)) {}
		ResultValue(const Result& result) : Result(result), m_value() {}

		ResultValue& operator= (const ResultValue& rhs) 
		{ 
			m_type = rhs.m_type; 
			m_severity = rhs.m_severity; 
			m_info = rhs.m_info; 
			m_value = rhs.m_value; 
			return *this; 
		}
		ResultValue& operator= (ResultValue&& rhs)
		{
			m_type = std::move(rhs.m_type);
			m_severity = std::move(rhs.m_severity);
			m_info = std::move(rhs.m_info);
			m_value = std::move(rhs.m_value);
			return *this;
		}

		T& value() { return m_value; }
		T& get(const Flags<Severity>& sev = RV_SEVERITY_INFO) { expect(sev); return value(); }
		T& get(const std::string& message, const Flags<Severity>& sev = RV_SEVERITY_INFO) { expect(message, sev); return value(); }

		const T& value() const { return m_value; }
		const T& get(const Flags<Severity>& sev = RV_SEVERITY_INFO) const { expect(sev); return value(); }
		const T& get(const std::string& message, const Flags<Severity>& sev = RV_SEVERITY_INFO) const { expect(message, sev); return value(); }

	private:
		T m_value;
	};
}

#define rv_return_failed(r)	if ((result = (r)).failed()) return result
// returns result if failed
#define rvrf(r)				rv_return_failed(r)
#define rv_result			rv::Result result = rv::success
// creates a Result value called 'result'
#define rvr					rv_result