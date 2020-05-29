#pragma once

namespace Dash
{
	namespace Math
	{
		template<typename Scalar>
		class ScalarArray<Scalar, 2>
		{
		public:
			using DataType = std::array<Scalar, 2>;

			using ScalarType = Scalar;
			using SizeType = std::size_t;
			using DifferenceType = typename DataType::difference_type;
			using Pointer = ScalarType*;
			using ConstPointer = const ScalarType*;
			using Reference = ScalarType&;
			using ConstReference = const ScalarType&;

			using Iterator = typename DataType::iterator;
			using ConstIterator = typename DataType::const_iterator;

			using ReverseIterator = typename DataType::reverse_iterator;
			using ConstReverseIterator = typename DataType::const_reverse_iterator;

			constexpr ScalarArray() noexcept;
			constexpr explicit ScalarArray(Zero) noexcept;
			template <std::size_t I> constexpr explicit ScalarArray(Unit<I>) noexcept;
			template <typename Scalar2> constexpr explicit ScalarArray(Scalar2 x, Scalar2 y) noexcept;
			template <typename Scalar2> constexpr explicit ScalarArray(const Scalar2* v) noexcept;
			template <typename Scalar2> constexpr ScalarArray(const ScalarArray<Scalar2, 2>& v) noexcept;

			operator ConstPointer () const noexcept;
			operator Pointer () noexcept;
		
			ScalarArray<Scalar, 2>& operator=(Zero) noexcept;
			ScalarArray<Scalar, 2>& operator+=(Zero) noexcept;
			ScalarArray<Scalar, 2>& operator-=(Zero) noexcept;
			ScalarArray<Scalar, 2>& operator*=(Zero) noexcept;

			template<std::size_t I> ScalarArray<Scalar, 2>& operator=(Unit<I>) noexcept;
			template<typename Scalar2> ScalarArray<Scalar, 2>& operator=(const Scalar2* v) noexcept;

			template<typename Scalar2> ScalarArray<Scalar, 2>& operator=(const ScalarArray<Scalar2, 2>& v) noexcept;
			template<typename Scalar2> ScalarArray<Scalar, 2>& operator+=(const ScalarArray<Scalar2, 2>& v) noexcept;
			template<typename Scalar2> ScalarArray<Scalar, 2>& operator-=(const ScalarArray<Scalar2, 2>& v) noexcept;
			template<typename Scalar2> ScalarArray<Scalar, 2>& operator*=(const ScalarArray<Scalar2, 2>& v) noexcept;

			ScalarArray<Scalar, 2>& operator*=(Scalar s) noexcept;
			ScalarArray<Scalar, 2>& operator/=(Scalar s) noexcept;

			template<typename Scalar2> void Fill(Scalar2 s) noexcept;

			constexpr SizeType GetSize() const noexcept { return 2; }

			constexpr Iterator Begin() noexcept;
			constexpr ConstIterator Begin() const noexcept;

			constexpr Iterator End() noexcept;
			constexpr ConstIterator End() const noexcept;

			union
			{		
				struct { Scalar u, v; };
				struct { Scalar x, y; };			
				DataType mData;
			};
		};


		template <typename Scalar1, typename Scalar2>
		typename Promote<Scalar1, Scalar2>::RT Dot(const ScalarArray<Scalar1, 2>& a, const ScalarArray<Scalar2, 2>& b) noexcept;

		template <typename Scalar1, typename Scalar2>
		typename Promote<Scalar1, Scalar2>::RT PerpDot(const ScalarArray<Scalar1, 2>& a, const ScalarArray<Scalar2, 2>& b) noexcept;

		template <typename Scalar> ScalarArray<Scalar, 2> Abs(const ScalarArray<Scalar, 2>& a) noexcept;
		template <typename Scalar> ScalarArray<Scalar, 2> Perp(const ScalarArray<Scalar, 2>& a) noexcept;

		template <typename Scalar> std::size_t MaxAxis(const ScalarArray<Scalar, 2>& a) noexcept;
		template <typename Scalar> std::size_t MinAxis(const ScalarArray<Scalar, 2>& a) noexcept;

		template <typename Scalar> std::size_t ClosestAxis(const ScalarArray<Scalar, 2>& a) noexcept;
		template <typename Scalar> std::size_t FurthestAxis(const ScalarArray<Scalar, 2>& a) noexcept;

		template <typename Scalar1, typename Scalar2>
		bool Dominates(const ScalarArray<Scalar1, 2>& a, const ScalarArray<Scalar2, 2>& b) noexcept;

		template <typename Scalar1, typename Scalar2>
		void Convert(Scalar1* v, const ScalarArray<Scalar2, 2>& a) noexcept;

		template <typename Scalar> bool Isfinite(const ScalarArray<Scalar, 2>& a) noexcept;

		template <typename Scalar> constexpr ScalarArray<Scalar, 2> Min(const ScalarArray<Scalar, 2> a, const ScalarArray<Scalar, 2> b) noexcept;
		template <typename Scalar> constexpr ScalarArray<Scalar, 2> Max(const ScalarArray<Scalar, 2> a, const ScalarArray<Scalar, 2> b) noexcept;


		//Member Function
		template<typename Scalar>
		FORCEINLINE constexpr ScalarArray<Scalar, 2>::ScalarArray() noexcept
			: x()
			, y()
		{
		}

		template<typename Scalar>
		FORCEINLINE constexpr ScalarArray<Scalar, 2>::ScalarArray(Zero) noexcept
			: x(0)
			, y(0)
		{
		}

		template<typename Scalar>
		template <std::size_t I>
		FORCEINLINE constexpr ScalarArray<Scalar, 2>::ScalarArray(Unit<I>) noexcept
			: x(Scalar{ I == 0 })
			, y(Scalar{ I == 1 })
		{

		}

		template<typename Scalar>
		template<typename Scalar2>
		FORCEINLINE constexpr ScalarArray<Scalar, 2>::ScalarArray(Scalar2 x, Scalar2 y) noexcept
			: x(static_cast<Scalar>(x))
			, y(static_cast<Scalar>(y))
		{
		}

		template<typename Scalar>
		template<typename Scalar2>
		FORCEINLINE constexpr ScalarArray<Scalar, 2>::ScalarArray(const Scalar2* v) noexcept
			: x(Scalar{ v[0] })
			, y(Scalar{ v[1] })
		{
		}

		template<typename Scalar>
		template<typename Scalar2>
		FORCEINLINE constexpr ScalarArray<Scalar, 2>::ScalarArray(const ScalarArray<Scalar2, 2>& v) noexcept
			: x(Scalar{ v.x })
			, y(Scalar{ v.y })
		{
		}

		template<typename Scalar>
		FORCEINLINE ScalarArray<Scalar, 2>::operator ConstPointer() const noexcept
		{
			return mData.data();
		}

		template<typename Scalar>
		FORCEINLINE ScalarArray<Scalar, 2>::operator Pointer() noexcept
		{
			return mData.data();
		}

		template<typename Scalar>
		FORCEINLINE ScalarArray<Scalar, 2>& ScalarArray<Scalar, 2>::operator=(Zero) noexcept
		{
			x = Scalar{};
			y = Scalar{};

			return *this;
		}

		template<typename Scalar>
		FORCEINLINE ScalarArray<Scalar, 2>& ScalarArray<Scalar, 2>::operator+=(Zero) noexcept
		{
			return *this;
		}

		template<typename Scalar>
		FORCEINLINE ScalarArray<Scalar, 2>& ScalarArray<Scalar, 2>::operator-=(Zero) noexcept
		{
			return *this;
		}

		template<typename Scalar>
		FORCEINLINE ScalarArray<Scalar, 2>& ScalarArray<Scalar, 2>::operator*=(Zero) noexcept
		{
			x = Scalar{};
			y = Scalar{};

			return *this;
		}

		template<typename Scalar>
		template<std::size_t I>
		FORCEINLINE ScalarArray<Scalar, 2>& ScalarArray<Scalar, 2>::operator=(Unit<I>) noexcept
		{
			x = Scalar{ I == 0 };
			y = Scalar{ I == 1 };

			return *this;
		}

		template<typename Scalar>
		template<typename Scalar2>
		FORCEINLINE ScalarArray<Scalar, 2>& ScalarArray<Scalar, 2>::operator=(const Scalar2* v) noexcept
		{
			ASSERT(v != nullptr);

			x = v[0];
			y = v[1];

			return *this;
		}

		template<typename Scalar>
		template<typename Scalar2>
		FORCEINLINE ScalarArray<Scalar, 2>& ScalarArray<Scalar, 2>::operator=(const ScalarArray<Scalar2, 2>& v) noexcept
		{
			ASSERT(v != nullptr);

			x = v.x;
			y = v.y;

			return *this;
		}

		template<typename Scalar>
		template<typename Scalar2>
		FORCEINLINE ScalarArray<Scalar, 2>& ScalarArray<Scalar, 2>::operator+=(const ScalarArray<Scalar2, 2>& v) noexcept
		{
			ASSERT(v != nullptr);

			x += v.x;
			y += v.y;

			return *this;
		}

		template<typename Scalar>
		template<typename Scalar2>
		FORCEINLINE ScalarArray<Scalar, 2>& ScalarArray<Scalar, 2>::operator-=(const ScalarArray<Scalar2, 2>& v) noexcept
		{
			ASSERT(v != nullptr);

			x -= v.x;
			y -= v.y;

			return *this;
		}

		template<typename Scalar>
		template<typename Scalar2>
		FORCEINLINE ScalarArray<Scalar, 2>& ScalarArray<Scalar, 2>::operator*=(const ScalarArray<Scalar2, 2>& v) noexcept
		{
			ASSERT(v != nullptr);

			x *= v.x;
			y *= v.y;

			return *this;
		}


		template<typename Scalar>
		FORCEINLINE ScalarArray<Scalar, 2>& ScalarArray<Scalar, 2>::operator*=(Scalar s) noexcept
		{
			x *= s;
			y *= s;

			return *this;
		}

		template<typename Scalar>
		FORCEINLINE ScalarArray<Scalar, 2>& ScalarArray<Scalar, 2>::operator/=(Scalar s) noexcept
		{
			ASSERT(!IsZero(s));

			return *this *= (Scalar{ 1 } / s);
		}

		template<typename Scalar>
		template<typename Scalar2>
		FORCEINLINE void ScalarArray<Scalar, 2>::Fill(Scalar2 s) noexcept
		{
			mData.fill(static_cast<Scalar>(s));
		}

		template<typename Scalar>
		FORCEINLINE constexpr typename ScalarArray<Scalar, 2>::Iterator ScalarArray<Scalar, 2>::Begin() noexcept
		{
			using Iterator = typename ScalarArray<Scalar, 2>::Iterator;
			return mData.begin();
		}

		template<typename Scalar>
		FORCEINLINE constexpr typename ScalarArray<Scalar, 2>::ConstIterator ScalarArray<Scalar, 2>::Begin() const noexcept
		{
			using ConstIterator = typename ScalarArray<Scalar, 2>::ConstIterator;
			return mData.begin();
		}

		template<typename Scalar>
		FORCEINLINE constexpr typename ScalarArray<Scalar, 2>::Iterator ScalarArray<Scalar, 2>::End() noexcept
		{
			using Iterator = typename ScalarArray<Scalar, 2>::Iterator;
			return mData.end();
		}

		template<typename Scalar>
		FORCEINLINE constexpr typename ScalarArray<Scalar, 2>::ConstIterator ScalarArray<Scalar, 2>::End() const noexcept
		{
			using ConstIterator = typename ScalarArray<Scalar, 2>::ConstIterator;
			return mData.end();
		}


		//Nomenber Function

		template<typename Scalar1, typename Scalar2>
		FORCEINLINE typename Promote<Scalar1, Scalar2>::RT Dot(const ScalarArray<Scalar1, 2>& a, const ScalarArray<Scalar2, 2>& b) noexcept
		{
			using RT = typename Promote<Scalar1, Scalar2>::RT;

			return a.x * b.x + a.y * b.y;
		}

		template<typename Scalar1, typename Scalar2>
		FORCEINLINE typename Promote<Scalar1, Scalar2>::RT PerpDot(const ScalarArray<Scalar1, 2>& a, const ScalarArray<Scalar2, 2>& b) noexcept
		{
			using RT = typename Promote<Scalar1, Scalar2>::RT;

			return a.x * b.y - a.y * b.x;
		}

		template<typename Scalar>
		FORCEINLINE ScalarArray<Scalar, 2> Abs(const ScalarArray<Scalar, 2>& a) noexcept
		{
			return ScalarArray<Scalar, 2>{ Abs(a.x), Abs(a.y) };
		}

		template<typename Scalar>
		FORCEINLINE ScalarArray<Scalar, 2> Perp(const ScalarArray<Scalar, 2>& a) noexcept
		{
			return ScalarArray<Scalar, 2>{ -a.y, a.x };
		}

		template<typename Scalar>
		FORCEINLINE std::size_t MaxAxis(const ScalarArray<Scalar, 2>& a) noexcept
		{
			return IsLessnn(a.x, a.y);
		}

		template<typename Scalar>
		FORCEINLINE std::size_t MinAxis(const ScalarArray<Scalar, 2>& a) noexcept
		{
			return IsLessnn(a.y, a.x);
		}

		template<typename Scalar>
		FORCEINLINE std::size_t ClosestAxis(const ScalarArray<Scalar, 2>& a) noexcept
		{
			return MaxAxis(a * a);
		}

		template<typename Scalar>
		FORCEINLINE std::size_t FurthestAxis(const ScalarArray<Scalar, 2>& a) noexcept
		{
			return MinAxis(a * a);
		}

		template<typename Scalar1, typename Scalar2>
		FORCEINLINE bool Dominates(const ScalarArray<Scalar1, 2>& a, const ScalarArray<Scalar2, 2>& b) noexcept
		{
			return !IsLessnn(a.x, a.x) && !IsLessnn(a.y, a.y);
		}

		template<typename Scalar1, typename Scalar2>
		FORCEINLINE void Convert(Scalar1* v, const ScalarArray<Scalar2, 2>& a) noexcept
		{
			ASSERT(v != nullptr);

			v[0] = a.x;
			v[1] = a.y;
		}

		template<typename Scalar>
		FORCEINLINE bool Isfinite(const ScalarArray<Scalar, 2>& a) noexcept
		{
			return IsFinite(a.x) && IsFinite(a.y);
		}

		template<typename Scalar>
		FORCEINLINE constexpr ScalarArray<Scalar, 2> Min(const ScalarArray<Scalar, 2> a, const ScalarArray<Scalar, 2> b) noexcept
		{
			return ScalarArray<Scalar, 2>{ Min(a.x, b.x), Min(a.y, b.y) };
		}

		template<typename Scalar>
		FORCEINLINE constexpr ScalarArray<Scalar, 2> Max(const ScalarArray<Scalar, 2> a, const ScalarArray<Scalar, 2> b) noexcept
		{
			return ScalarArray<Scalar, 2>{ Max(a.x, b.x), Max(a.y, b.y) };
		}

	}
}