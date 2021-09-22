#pragma once
#include <memory>
#include "Engine/Utility/Severity.h"
#include "Engine/Utility/Identifier.h"

namespace rv
{
	struct ResultInfo
	{
		ResultInfo();
		ResultInfo(const Identifier& type, const Severity& severity);
		virtual ~ResultInfo() = default;

		virtual std::string description() const = 0;
		virtual std::string optional_info() const;

		Severity severity;
		Identifier type;
	};

	struct ResultBase
	{
	public:
		ResultBase() = default;
		template<typename R> ResultBase(const R& result) : pInfo(new R(result)) { static_assert(std::is_base_of_v<ResultInfo, R>, "rv::ResultBase(const R& result) argument should be derived from rv::ResultInfo"); }
		template<typename R> ResultBase(R&& result) : pInfo(new R(std::move(result))) { static_assert(std::is_base_of_v<ResultInfo, R>, "rv::ResultBase(R&& result) argument should be derived from rv::ResultInfo"); }

		bool succeeded(const Severity& severity = RV_SEVERITY_INFO) const;
		bool failed(const Severity& severity = RV_SEVERITY_INFO) const;
		bool has_info() const;

		void expect(const char* errormessage) const;
		void expect(const Severity& severity = RV_SEVERITY_INFO, const char* errormessage = nullptr) const;

		const ResultInfo& info() const;
		const ResultInfo& info_unchecked() const;
		template<typename R>
		const R& info_as() const { static_assert(std::is_base_of_v<ResultInfo, R>); return dynamic_cast<const R&>(info()); }

	private:
		std::shared_ptr<ResultInfo> pInfo;
	};

	template<typename T>
	struct Result : public ResultBase
	{
		Result() : value() {};
		template<typename R> Result(const R& result) : ResultBase(result) {}
		template<typename R> Result(R&& result) : ResultBase(std::move(result)) {}
		Result(const T& value) : value(value) {}
		Result(T&& value) : value(std::move(value)) {}

		T value;
	};

	template<>
	struct Result<void> : public ResultBase
	{
		Result() {};
		template<typename R> Result(const R& result) : ResultBase(result) {}
		template<typename R> Result(R&& result) : ResultBase(std::move(result)) {}
	};
}

#define rv_success {}