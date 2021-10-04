#pragma once
#include "Engine/Utility/Types.h"
#include <type_traits>

namespace rv
{
	template<typename E, typename I = u32>
	struct Flags
	{
	public:
		static_assert(std::is_enum_v<E>);
		static_assert(std::is_unsigned_v<I>);

	public:
		constexpr Flags() : data(0) {}
		constexpr Flags(const E& flag) : data(0x1 << flag) {}

	private:
		constexpr Flags(const I& value) : data(value) {}

	public:
		constexpr Flags operator& (const Flags& rhs) const { return data & rhs.data; }
		constexpr Flags operator| (const Flags& rhs) const { return data | rhs.data; }
		constexpr Flags operator^ (const Flags& rhs) const { return data ^ rhs.data; }

		constexpr Flags& operator&= (const Flags& rhs) { data &= rhs.data; return *this; }
		constexpr Flags& operator|= (const Flags& rhs) { data |= rhs.data; return *this; }
		constexpr Flags& operator^= (const Flags& rhs) { data ^= rhs.data; return *this; }

		constexpr bool empty() const { return data == 0; }
		constexpr bool contains(const Flags& rhs) const { return data & rhs.data; }
		constexpr bool get(const E& flag) const { return contains(flag); }

		constexpr Flags flipped() const { return ~data; }

		constexpr void flip() { data = ~data; }
		constexpr void set(const Flags& flags, bool value = true) { value ? (data ^= flags) : (data &= ~flags.data); }

	private:
		I data;
	};

	template<typename E, typename I = u32> static constexpr Flags<E, I> operator& (const E& a, const E& b) { return Flags<E, I>(a) & Flags<E, I>(a); };
	template<typename E, typename I = u32> static constexpr Flags<E, I> operator| (const E& a, const E& b) { return Flags<E, I>(a) | Flags<E, I>(a); };
	template<typename E, typename I = u32> static constexpr Flags<E, I> operator^ (const E& a, const E& b) { return Flags<E, I>(a) ^ Flags<E, I>(a); };
}