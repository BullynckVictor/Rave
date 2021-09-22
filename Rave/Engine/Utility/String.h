#pragma once
#include <sstream>

namespace rv
{
	namespace detail
	{
		template<typename C, typename F, typename... Args>
		void str_t(std::basic_stringstream<C>& ss, const F& first, const Args&... args)
		{
			ss << first;
			if constexpr (sizeof...(Args) != 0)
			str_t<C>(ss, args...);
		}
	}

	template<typename C, typename... Args>
	std::basic_string<C> str_t(const Args&... args)
	{
		if constexpr (sizeof...(Args) != 0)
		{
			std::basic_stringstream<C> ss;
			detail::str_t<C>(ss, args...);
			return ss.str();
		}
		else
			return {};
	}

	template<typename... Args>
	std::string str(const Args&... args)
	{
		return str_t<char>(args...);
	}
	template<typename... Args>
	std::wstring wstr(const Args&... args)
	{
		return str_t<wchar_t>(args...);
	}
}