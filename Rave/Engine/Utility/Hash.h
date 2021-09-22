#pragma once
#include "Engine/Utility/Types.h"
#include "Engine/Utility/StaticFail.h"
#include <type_traits>
#include <limits>
#include <string_view>

namespace rv
{
	static constexpr size_t combine_hash(const size_t& h1, const size_t& h2)
	{
		return h2 ^ (h1 + 0x9e3779b9 + (h2 << 6) + (h2 >> 2));
	}

	namespace hash_alg
	{
		template<typename H>
		struct fnv1a_values {  };

		template<>
		struct fnv1a_values<u32> {
			static constexpr u32 prime			= 0x01000193;
			static constexpr u32 offset_basis	= 0x811c9dc5;
		};

		template<>
		struct fnv1a_values<u64> {
			static constexpr u64 prime			= 0x00000100000001B3;
			static constexpr u64 offset_basis	= 0xcbf29ce484222325;
		};

		template<typename T>
		static constexpr size_t fnv1a(const T& value)
		{
			size_t hash = fnv1a_values<size_t>::offset_basis;
			for (size_t i = 0; i < sizeof(T); ++i)
			{
				hash = hash ^ ((value >> (i * 8)) & 0xFF);
				hash = hash * fnv1a_values<size_t>::prime;
			}
			return hash;
		}

		static constexpr size_t fnv1a(const char* data, const size_t size)
		{
			size_t hash = fnv1a_values<size_t>::offset_basis;
			for (size_t i = 0; i < size; ++i)
			{
				hash = hash ^ data[i];
				hash = hash * fnv1a_values<size_t>::prime;
			}
			return hash;
		}
	}

	template<typename T>
	static constexpr size_t hash_integer(const T& value)
	{
		static_assert(std::is_integral_v<T>, "value must be of integral type");
		return hash_alg::fnv1a(value);
	}

	static constexpr size_t hash_string(const std::string_view str)
	{
		return hash_alg::fnv1a(str.data(), str.size());
	}

	static constexpr size_t hash_string(const std::wstring_view str)
	{
		size_t hash = hash_alg::fnv1a_values<size_t>::offset_basis;
		for (size_t i = 0; i < str.size(); ++i)
		{
			hash = hash ^ (str[i] & 0xFF);
			hash = hash * hash_alg::fnv1a_values<size_t>::prime;
			hash = hash ^ ((str[i] >> 8) & 0xFF);
			hash = hash * hash_alg::fnv1a_values<size_t>::prime;
		}
		return hash;
	}

	template<typename T>
	static constexpr size_t make_hash(const T& value)
	{
		return std::hash<T>()(value);
	}

	template<typename F, typename... Args>
	static constexpr size_t hash(const F& first, const Args&... args)
	{
		size_t h = make_hash(first);
		if constexpr (sizeof...(Args) != 0)
			h = combine_hash(h, hash(args...));
		return h;
	}

	template<> static constexpr size_t make_hash(const i8& value)					{ return hash_integer(value); }
	template<> static constexpr size_t make_hash(const i16& value)					{ return hash_integer(value); }
	template<> static constexpr size_t make_hash(const i32& value)					{ return hash_integer(value); }
	template<> static constexpr size_t make_hash(const i64& value)					{ return hash_integer(value); }
	template<> static constexpr size_t make_hash(const u8& value)					{ return hash_integer(value); }
	template<> static constexpr size_t make_hash(const u16& value)					{ return hash_integer(value); }
	template<> static constexpr size_t make_hash(const u32& value)					{ return hash_integer(value); }
	template<> static constexpr size_t make_hash(const u64& value)					{ return hash_integer(value); }
	template<> static constexpr size_t make_hash(const char* const& value)			{ return hash_string(value); }
	template<> static constexpr size_t make_hash(const wchar_t* const& value)		{ return hash_string(value); }
	template<> static constexpr size_t make_hash(const std::string_view& value)		{ return hash_string(value); }
	template<> static constexpr size_t make_hash(const std::wstring_view& value)	{ return hash_string(value); }
	template<size_t S> static constexpr size_t make_hash(const char(&value)[S])		{ return hash_string(value); }
	template<size_t S> static constexpr size_t make_hash(const wchar_t(&value)[S])	{ return hash_string(value); }
}