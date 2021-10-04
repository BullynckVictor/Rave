#pragma once
#include <array>
#include <algorithm>
#include "Engine/Utility/Types.h"

namespace rv
{
	template<size_t S, typename T, typename I>
	struct VectorWrapper;

	namespace detail 
	{
		struct VectorIdentifierStruct {};
		struct ExtentIdentifierStruct {};
	}

	template<size_t S, typename T>
	struct VectorWrapper<S, T, detail::VectorIdentifierStruct>
	{
		using value_type = T;

		constexpr VectorWrapper(const T& value = {})
			:
			array()
		{
			for (auto& v : array)
				v = value;
		}
		constexpr VectorWrapper(const std::array<T, S>& array) : array(array) {}
		constexpr VectorWrapper(std::array<T, S>&& array) : array(std::move(array)) {}
		template<typename... Args>
		constexpr VectorWrapper(const Args&... args)
			:
			array({ args... })
		{
		}

				T* begin()				{ return array.begin(); }
		const	T* begin()		const	{ return array.begin(); }
		const	T* cbegin()		const	{ return array.cbegin(); }
				T* rbegin()				{ return array.rbegin(); }
		const	T* rbegin()		const	{ return array.rbegin(); }
		const	T* crbegin()	const	{ return array.crbegin(); }

				T* end()				{ return array.end(); }
		const	T* end()		const	{ return array.end(); }
		const	T* cend()		const	{ return array.cend(); }
				T* rend()				{ return array.rend(); }
		const	T* rend()		const	{ return array.rend(); }
		const	T* crend()		const	{ return array.crend(); }

				T* data()		{ return array.data(); }
		const	T* data() const	{ return array.data(); }

		template<size_t Index> constexpr		T& get_element()		{ return array[Index]; }
		template<size_t Index> constexpr const	T& get_element() const	{ return array[Index]; }

		static constexpr size_t size() { return S; }

		std::array<T, S> array;
	};

	template<typename T>
	struct VectorWrapper<2, T, detail::ExtentIdentifierStruct>
	{
		using value_type = T;

		VectorWrapper(const T& value = {}) : width(value), height(value) {}
		VectorWrapper(const T& width, const T& height) : width(width), height(height) {}
		VectorWrapper(T&& width, T&& height) : width(std::move(width)), height(std::move(height)) {}

		template<size_t Index>	constexpr		T& get_element();
		template<>				constexpr		T& get_element<0>()			{ return width; }
		template<>				constexpr		T& get_element<1>()			{ return height; }
		template<size_t Index>	constexpr const	T& get_element()	const;
		template<>				constexpr const	T& get_element<0>()	const	{ return width; }
		template<>				constexpr const T& get_element<1>()	const	{ return height; }

		static constexpr size_t size() { return 2; }

				T* begin()				{ return &width; }
		const	T* begin()		const	{ return &width; }
		const	T* cbegin()		const	{ return &width; }
				T* rbegin()				{ return &height; }
		const	T* rbegin()		const	{ return &height; }
		const	T* crbegin()	const	{ return &height; }

				T* end()				{ return &height + 1; }
		const	T* end()		const	{ return &height + 1; }
		const	T* cend()		const	{ return &height + 1; }
				T* rend()				{ return &begin - 1; }
		const	T* rend()		const	{ return &begin - 1; }
		const	T* crend()		const	{ return &begin - 1; }

				T* data()		{ return &width; }
		const	T* data() const	{ return &width; }

		T width;
		T height;
	};

	namespace detail
	{
		template<size_t S, typename T, typename I, size_t D = 0>
		static constexpr bool v_element_is_equal(const VectorWrapper<S, T, I>& a, const VectorWrapper<S, T, I>& b)
		{
			return a.get_element<D>() == b.get_element<D>();
		}

		template<typename T> static constexpr T v_element_add(const T& a, const T& b) { return a + b; }
		template<typename T> static constexpr T v_element_sub(const T& a, const T& b) { return a - b; }
		template<typename T> static constexpr T v_element_mul(const T& a, const T& b) { return a * b; }
		template<typename T> static constexpr T v_element_dev(const T& a, const T& b) { return a / b; }

		template<typename T> static constexpr void v_element_add_self(T& a, const T& b) { a += b; }
		template<typename T> static constexpr void v_element_sub_self(T& a, const T& b) { a -= b; }
		template<typename T> static constexpr void v_element_mul_self(T& a, const T& b) { a *= b; }
		template<typename T> static constexpr void v_element_dev_self(T& a, const T& b) { a /= b; }

		template<size_t S, typename T, typename I, size_t D = 0>
		static constexpr bool v_is_equal(const VectorWrapper<S, T, I>& a, const VectorWrapper<S, T, I>& b)
		{
			if constexpr (D < b.size() - 1)
				return v_element_is_equal<S, T, I, D>(a, b) && v_is_equal<S, T, I, D + 1>(a, b);
			else
				return v_element_is_equal<S, T, I, D>(a, b);
		}

		template<size_t S, typename T, typename I, size_t D = 0>
		static constexpr bool v_is_nequal(const VectorWrapper<S, T, I>& a, const VectorWrapper<S, T, I>& b)
		{
			if constexpr (D < b.size() - 1)
				return !v_element_is_equal<S, T, I, D>(a, b) || v_is_nequal<S, T, I, D + 1>(a, b);
			else
				return !v_element_is_equal<S, T, I, D>(a, b);
		}

		template<size_t S, typename T, typename I, typename VectorWrapper<S, T, I>::value_type (*op)(const typename VectorWrapper<S, T, I>::value_type&, const typename VectorWrapper<S, T, I>::value_type&), size_t D = 0>
		static constexpr void v_operation(VectorWrapper<S, T, I>& out, const VectorWrapper<S, T, I>& a, const VectorWrapper<S, T, I>& b)
		{
			out.get_element<D>() = op(a.get_element<D>(), b.get_element<D>());
			if constexpr (D < b.size() - 1)
				v_operation<S, T, I, op, D + 1>(out, a, b);
		}

		template<size_t S, typename T, typename I, void(*op)(typename VectorWrapper<S, T, I>::value_type&, const typename VectorWrapper<S, T, I>::value_type&), size_t D = 0>
		static constexpr void v_operation_self(VectorWrapper<S, T, I>& a, const VectorWrapper<S, T, I>& b)
		{
			op(a.get_element<D>(), b.get_element<D>());
			if constexpr (D < b.size() - 1)
				v_operation_self<S, T, I, op, D + 1>(a, b);
		}
		
		template<size_t S, typename T, typename I, typename VectorWrapper<S, T, I>::value_type(*op)(const typename VectorWrapper<S, T, I>::value_type&, const typename VectorWrapper<S, T, I>::value_type&), size_t D = 0>
		static constexpr void v_operation(VectorWrapper<S, T, I>& out, const VectorWrapper<S, T, I>& a, const typename VectorWrapper<S, T, I>::value_type& b)
		{
			out.get_element<D>() = op(a.get_element<D>(), b);
			if constexpr (D < a.size() - 1)
				v_operation<S, T, I, op, D + 1>(out, a, b);
		}
	}

	template<size_t S, typename T, typename I>
	static constexpr bool operator== (const VectorWrapper<S, T, I>& a, const VectorWrapper<S, T, I>& b)
	{
		return detail::v_is_equal<S, T, I>(a, b);
	}
	template<size_t S, typename T, typename I>
	static constexpr bool operator!= (const VectorWrapper<S, T, I>& a, const VectorWrapper<S, T, I>& b)
	{
		return detail::v_is_nequal<S, T, I>(a, b);
	}

	template<size_t S, typename T, typename I>
	static constexpr VectorWrapper<S, T, I> operator+ (const VectorWrapper<S, T, I>& a, const VectorWrapper<S, T, I>& b)
	{
		VectorWrapper<S, T, I> result{};
		detail::v_operation<S, T, I, detail::v_element_add>(result, a, b);
		return result;
	}
	template<size_t S, typename T, typename I>
	static constexpr VectorWrapper<S, T, I> operator- (const VectorWrapper<S, T, I>& a, const VectorWrapper<S, T, I>& b)
	{
		VectorWrapper<S, T, I> result{};
		detail::v_operation<S, T, I, detail::v_element_sub>(result, a, b);
		return result;
	}
	template<size_t S, typename T, typename I>
	static constexpr VectorWrapper<S, T, I> operator* (const VectorWrapper<S, T, I>& a, const VectorWrapper<S, T, I>& b)
	{
		VectorWrapper<S, T, I> result{};
		detail::v_operation<S, T, I, detail::v_element_mul>(result, a, b);
		return result;
	}
	template<size_t S, typename T, typename I>
	static constexpr VectorWrapper<S, T, I> operator/ (const VectorWrapper<S, T, I>& a, const VectorWrapper<S, T, I>& b)
	{
		VectorWrapper<S, T, I> result{};
		detail::v_operation<S, T, I, detail::v_element_dev>(result, a, b);
		return result;
	}

	template<size_t S, typename T, typename I>
	static constexpr VectorWrapper<S, T, I>& operator+= (VectorWrapper<S, T, I>& a, const VectorWrapper<S, T, I>& b)
	{
		detail::v_operation_self<S, T, I, detail::v_element_add_self>(a, b);
		return a;
	}
	template<size_t S, typename T, typename I>
	static constexpr VectorWrapper<S, T, I>& operator-= (VectorWrapper<S, T, I>& a, const VectorWrapper<S, T, I>& b)
	{
		detail::v_operation_self<S, T, I, detail::v_element_sub_self>(a, b);
		return a;
	}
	template<size_t S, typename T, typename I>
	static constexpr VectorWrapper<S, T, I>& operator*= (VectorWrapper<S, T, I>& a, const VectorWrapper<S, T, I>& b)
	{
		detail::v_operation_self<S, T, I, detail::v_element_mul_self>(a, b);
		return a;
	}
	template<size_t S, typename T, typename I>
	static constexpr VectorWrapper<S, T, I>& operator/= (VectorWrapper<S, T, I>& a, const VectorWrapper<S, T, I>& b)
	{
		detail::v_operation_self<S, T, I, detail::v_element_dev_self>(a, b);
		return a;
	}

	template<size_t S, typename T, typename I>
	static constexpr VectorWrapper<S, T, I> operator+ (const VectorWrapper<S, T, I>& a, const typename VectorWrapper<S, T, I>::value_type& b)
	{
		VectorWrapper<S, T, I> result{};
		detail::v_operation<S, T, I, detail::v_element_add>(result, a, b);
		return result;
	}
	template<size_t S, typename T, typename I>
	static constexpr VectorWrapper<S, T, I> operator- (const VectorWrapper<S, T, I>& a, const typename VectorWrapper<S, T, I>::value_type& b)
	{
		VectorWrapper<S, T, I> result{};
		detail::v_operation<S, T, I, detail::v_element_sub>(result, a, b);
		return result;
	}
	template<size_t S, typename T, typename I>
	static constexpr VectorWrapper<S, T, I> operator* (const VectorWrapper<S, T, I>& a, const typename VectorWrapper<S, T, I>::value_type& b)
	{
		VectorWrapper<S, T, I> result{};
		detail::v_operation<S, T, I, detail::v_element_mul>(result, a, b);
		return result;
	}
	template<size_t S, typename T, typename I>
	static constexpr VectorWrapper<S, T, I> operator/ (const VectorWrapper<S, T, I>& a, const typename VectorWrapper<S, T, I>::value_type& b)
	{
		VectorWrapper<S, T, I> result{};
		detail::v_operation<S, T, I, detail::v_element_dev>(result, a, b);
		return result;
	}

	template<size_t S, typename T, typename I>
	static constexpr VectorWrapper<S, T, I>& operator+= (VectorWrapper<S, T, I>& a, const typename VectorWrapper<S, T, I>::value_type& b)
	{
		detail::v_operation_self<S, T, I, detail::v_element_add_self>(a, b);
		return a;
	}
	template<size_t S, typename T, typename I>
	static constexpr VectorWrapper<S, T, I>& operator-= (VectorWrapper<S, T, I>& a, const typename VectorWrapper<S, T, I>::value_type& b)
	{
		detail::v_operation_self<S, T, I, detail::v_element_sub_self>(a, b);
		return a;
	}
	template<size_t S, typename T, typename I>
	static constexpr VectorWrapper<S, T, I>& operator*= (VectorWrapper<S, T, I>& a, const typename VectorWrapper<S, T, I>::value_type& b)
	{
		detail::v_operation_self<S, T, I, detail::v_element_mul_self>(a, b);
		return a;
	}
	template<size_t S, typename T, typename I>
	static constexpr VectorWrapper<S, T, I>& operator/= (VectorWrapper<S, T, I>& a, const typename VectorWrapper<S, T, I>::value_type& b)
	{
		detail::v_operation_self<S, T, I, detail::v_element_dev_self>(a, b);
		return a;
	}

	template<size_t S, typename T>
	using Vector = VectorWrapper<S, T, detail::VectorIdentifierStruct>;
	template<typename T>
	using Extent = VectorWrapper<2, T, detail::ExtentIdentifierStruct>;

	using ExtentF	= Extent<float>;
	using ExtentI	= Extent<int>;
	using ExtentU	= Extent<unsigned int>;
	using Extent32	= Extent<u32>;
	using Extent64	= Extent<u64>;
}
