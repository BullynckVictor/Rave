#pragma once
#include "Engine/Utility/Hash.h"
#include <limits>

namespace rv
{
	struct Identifier
	{
	public:
		constexpr Identifier() : m_name(nullptr), m_hash(std::numeric_limits<size_t>::max()) {}
		constexpr Identifier(const char* name) : m_name(name), m_hash(rv::hash(name)) {}
		
		constexpr size_t hash() const { return m_hash; };
		constexpr const char* name() const { return m_name; }

	private:
		size_t m_hash;
		const char* m_name;
	};
}