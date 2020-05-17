#pragma once

#include "..//consolid/consolid.h"
#include "Algebra.h"
#include "Promote.h"
#include "Scalar.h"

#include <array>
#include <initializer_list>

#ifdef USE_OSTREAM
#include <ostream>	
#endif // USE_OSTREAM

#ifdef USE_ISTREAM
#include <istream>	
#endif // USE_ISTREAM

namespace Dash {

	namespace Math {

		template<typename Scalar, std::size_t N>
		class ScalarArray
		{
		public:
			using DataType = std::array<Scalar, N>;

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

			constexpr ScalarArray();
			constexpr explicit ScalarArray(Zero);
			template <std::size_t I> constexpr explicit ScalarArray(Unit<I>);
			template <typename Scalar2> constexpr explicit ScalarArray(Scalar2 scalar);
			template <typename Scalar2> constexpr explicit ScalarArray(const Scalar2* v);
			constexpr explicit ScalarArray(std::initializer_list<Scalar> list);
			template <typename Scalar2> constexpr explicit ScalarArray(const ScalarArray<Scalar2, N - 1>& v);
			template <typename Scalar2> constexpr ScalarArray(const ScalarArray<Scalar2, N>& v);

			operator ConstPointer () const noexcept;
			operator Pointer () noexcept;

			ScalarArray<Scalar, N>& operator=(Zero) noexcept;
			ScalarArray<Scalar, N>& operator+=(Zero) noexcept;
			ScalarArray<Scalar, N>& operator-=(Zero) noexcept;
			ScalarArray<Scalar, N>& operator*=(Zero) noexcept;

			template<std::size_t I> ScalarArray<Scalar, N>& operator=(Unit<I>) noexcept;
			template<typename Scalar2> ScalarArray<Scalar, N>& operator=(const Scalar2* v) noexcept;

			template<typename Scalar2> ScalarArray<Scalar, N>& operator=(const ScalarArray<Scalar2, N>& v) noexcept;
			template<typename Scalar2> ScalarArray<Scalar, N>& operator+=(const ScalarArray<Scalar2, N>& v) noexcept;
			template<typename Scalar2> ScalarArray<Scalar, N>& operator-=(const ScalarArray<Scalar2, N>& v) noexcept;
			template<typename Scalar2> ScalarArray<Scalar, N>& operator*=(const ScalarArray<Scalar2, N>& v) noexcept;

			ScalarArray<Scalar, N>& operator*=(Scalar s) noexcept;
			ScalarArray<Scalar, N>& operator/=(Scalar s) noexcept;

			constexpr SizeType GetSize() const noexcept { return N; }

			constexpr Iterator Begin() noexcept;
			constexpr ConstIterator Begin() const noexcept;

			constexpr Iterator End() noexcept;
			constexpr ConstIterator End() const noexcept;

		private:
			DataType mData;
		};


#ifdef USE_OSTREAM

		template<typename CharT, typename Traits, typename Scalar, std::size_t N>
		std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const ScalarArray<Scalar, N>& v);

#endif // USE_OSTREAM

#ifdef USE_ISTREAM

		template<typename CharT, typename Traits, typename Scalar, std::size_t N>
		std::basic_istream<CharT, Traits>& operator>>(std::basic_ostream<CharT, Traits>& is, const ScalarArray<Scalar, N>& v);

#endif // USE_ISTREAM

		template<typename Scalar1, typename Scalar2, std::size_t N> bool operator==(const ScalarArray<Scalar1, N>& v1, const ScalarArray<Scalar2, N>& v2) noexcept;
		template<typename Scalar1, typename Scalar2, std::size_t N> bool operator!=(const ScalarArray<Scalar1, N>& v1, const ScalarArray<Scalar2, N>& v2) noexcept;

		template<typename Scalar1, typename Scalar2, std::size_t N>
		ScalarArray<typename Promote<Scalar1, Scalar2>::RT, N> operator+(const ScalarArray<Scalar1, N>& v1, const ScalarArray<Scalar2, N>& v2) noexcept;

		template<typename Scalar1, typename Scalar2, std::size_t N>
		ScalarArray<typename Promote<Scalar1, Scalar2>::RT, N> operator-(const ScalarArray<Scalar1, N>& v1, const ScalarArray<Scalar2, N>& v2) noexcept;

		template<typename Scalar1, typename Scalar2, std::size_t N>
		ScalarArray<typename Promote<Scalar1, Scalar2>::RT, N> operator*(const ScalarArray<Scalar1, N>& v1, const ScalarArray<Scalar2, N>& v2) noexcept;

		template<typename Scalar1, typename Scalar2, std::size_t N>
		ScalarArray<typename Promote<Scalar1, Scalar2>::RT, N> operator*(const ScalarArray<Scalar1, N>& v, Scalar2 s) noexcept;

		template<typename Scalar1, typename Scalar2, std::size_t N>
		ScalarArray<typename Promote<Scalar1, Scalar2>::RT, N> operator*(Scalar2 s, const ScalarArray<Scalar1, N>& v) noexcept;

		template<typename Scalar1, typename Scalar2, std::size_t N>
		ScalarArray<typename Promote<Scalar1, Scalar2>::RT, N> operator/(const ScalarArray<Scalar1, N>& v, Scalar2 s) noexcept;

		template<typename Scalar, std::size_t N>
		std::size_t MaxDimension(const ScalarArray<Scalar, N>& v) noexcept;

		template<typename Scalar, std::size_t N>
		std::size_t MinDimension(const ScalarArray<Scalar, N>& v) noexcept;

		template<typename Scalar, std::size_t N>
		Scalar HorizontalAdd(const ScalarArray<Scalar, N>& v) noexcept;

		template<typename Scalar, std::size_t N>
		Scalar HorizontalMin(const ScalarArray<Scalar, N>& v) noexcept;

		template<typename Scalar, std::size_t N>
		Scalar HorizontalMax(const ScalarArray<Scalar, N>& v) noexcept;

		template<typename Scalar1, typename Scalar2, std::size_t N>
		typename Promote<Scalar1, Scalar2>::RT Dot(const ScalarArray<Scalar1, N>& v1, const ScalarArray<Scalar2, N>& v2) noexcept;

		template<typename Scalar1, typename Scalar2, std::size_t N>
		typename Promote<Scalar1, Scalar2>::RT AbsDot(const ScalarArray<Scalar1, N>& v1, const ScalarArray<Scalar2, N>& v2) noexcept;


		template<typename Scalar, std::size_t N> ScalarArray<Scalar, N> Sin(const ScalarArray<Scalar, N>& v) noexcept;
		template<typename Scalar, std::size_t N> ScalarArray<Scalar, N> Cos(const ScalarArray<Scalar, N>& v) noexcept;
		template<typename Scalar, std::size_t N> ScalarArray<Scalar, N> Tan(const ScalarArray<Scalar, N>& v) noexcept;

		template<typename Scalar, std::size_t N> ScalarArray<Scalar, N> ASin(const ScalarArray<Scalar, N>& v) noexcept;
		template<typename Scalar, std::size_t N> ScalarArray<Scalar, N> ACos(const ScalarArray<Scalar, N>& v) noexcept;
		template<typename Scalar, std::size_t N> ScalarArray<Scalar, N> ATan(const ScalarArray<Scalar, N>& v) noexcept;
		
		template<typename Scalar, std::size_t N> ScalarArray<Scalar, N> Square(const ScalarArray<Scalar, N>& v) noexcept;
		template<typename Scalar, std::size_t N> ScalarArray<Scalar, N> Cube(const ScalarArray<Scalar, N>& v) noexcept;
		template<typename Scalar, std::size_t N> ScalarArray<Scalar, N> Abs(const ScalarArray<Scalar, N>& v) noexcept;
		template<typename Scalar, std::size_t N> ScalarArray<Scalar, N> Saturate(const ScalarArray<Scalar, N>& v) noexcept;
		template<typename Scalar, std::size_t N> ScalarArray<Scalar, N> Clamp(const ScalarArray<Scalar, N>& v, Scalar a, Scalar b) noexcept;
		template<typename Scalar, std::size_t N> ScalarArray<Scalar, N> SmoothStep(const ScalarArray<Scalar, N>& v) noexcept;

		template<typename Scalar, std::size_t N> ScalarArray<Scalar, N> Exp(const ScalarArray<Scalar, N>& v) noexcept;
		template<typename Scalar, std::size_t N> ScalarArray<Scalar, N> Log(const ScalarArray<Scalar, N>& v) noexcept;
		template<typename Scalar, std::size_t N> ScalarArray<Scalar, N> Log2(const ScalarArray<Scalar, N>& v) noexcept;
		template<typename Scalar, std::size_t N> ScalarArray<Scalar, N> Log10(const ScalarArray<Scalar, N>& v) noexcept;
		template<typename Scalar, std::size_t N> ScalarArray<Scalar, N> Sqrt(const ScalarArray<Scalar, N>& v) noexcept;
		template<typename Scalar, std::size_t N> ScalarArray<Scalar, N> Rsqrt(const ScalarArray<Scalar, N>& v) noexcept;
		template<typename Scalar, std::size_t N> ScalarArray<Scalar, N> Cbrt(const ScalarArray<Scalar, N>& v) noexcept;
		template<typename Scalar, std::size_t N> ScalarArray<Scalar, N> Pow(const ScalarArray<Scalar, N>& base, Scalar exp) noexcept;
		
		template<typename Scalar, std::size_t N> ScalarArray<Scalar, N> Min(const ScalarArray<Scalar, N>& a, const ScalarArray<Scalar, N>& b) noexcept;
		template<typename Scalar, std::size_t N> ScalarArray<Scalar, N> Max(const ScalarArray<Scalar, N>& a, const ScalarArray<Scalar, N>& b) noexcept;


		//Member Function

		template<typename T, std::size_t N>
		FORCEINLINE constexpr ScalarArray<T, N>::ScalarArray()
			: mData{}
		{
		}

		template<typename Scalar, std::size_t N>
		FORCEINLINE constexpr ScalarArray<Scalar, N>::ScalarArray(Zero)
		{
			mData.fill(Scalar{ 0 });
		}

		template<typename Scalar, std::size_t N>
		template<std::size_t I>
		FORCEINLINE constexpr ScalarArray<Scalar, N>::ScalarArray(Unit<I>)
		{
			ASSERT(GetSize() >= I);

			mData.fill(Scalar{ 0 });
			mData[I] = Scalar{ 1 };
		}

		template<typename Scalar, std::size_t N>
		template<typename Scalar2>
		FORCEINLINE constexpr ScalarArray<Scalar, N>::ScalarArray(Scalar2 scalar)
		{
			mData.fill(static_cast<Scalar>(scalar));
		}

		template<typename Scalar, std::size_t N>
		template<typename Scalar2>
		FORCEINLINE constexpr ScalarArray<Scalar, N>::ScalarArray(const Scalar2* v)
		{
			ASSERT(v != nullptr);

			for (SizeType i = 0; i < GetSize(); ++i)
			{
				mData[i] = static_cast<Scalar>(v[i]);
			}
		}

		template<typename Scalar, std::size_t N>
		FORCEINLINE constexpr ScalarArray<Scalar, N>::ScalarArray(std::initializer_list<Scalar> list)
		{
			ASSERT(GetSize() >= list.size());

			auto listIter = list.begin();
			auto arrayIter = mData.begin();

			for (; listIter != list.end() && arrayIter != mData.end(); ++listIter, ++arrayIter)
			{
				*arrayIter = *listIter;
			}
		}

		template<typename Scalar, std::size_t N>
		template<typename Scalar2>
		FORCEINLINE constexpr ScalarArray<Scalar, N>::ScalarArray(const ScalarArray<Scalar2, N - 1>& v)
		{
			for (SizeType i = 0; i < GetSize() - 1; ++i)
			{
				mData[i] = static_cast<Scalar>(v[i]);
			}

			mData[GetSize() - 1] = Scalar{};
		}

		template<typename Scalar, std::size_t N>
		template<typename Scalar2>
		FORCEINLINE constexpr ScalarArray<Scalar, N>::ScalarArray(const ScalarArray<Scalar2, N>& v)
		{
			for (SizeType i = 0; i < GetSize(); ++i)
			{
				mData[i] = static_cast<Scalar>(v[i]);
			}
		}

		template<typename Scalar, std::size_t N>
		FORCEINLINE ScalarArray<Scalar, N>::operator ConstPointer() const noexcept
		{
			return mData.data();
		}

		template<typename Scalar, std::size_t N>
		FORCEINLINE ScalarArray<Scalar, N>::operator Pointer() noexcept
		{
			return mData.data();
		}

		template<typename Scalar, std::size_t N>
		FORCEINLINE ScalarArray<Scalar, N>& ScalarArray<Scalar, N>::operator=(Zero) noexcept
		{
			mData.fill(Scalar{});
			return *this;
		}

		template<typename Scalar, std::size_t N>
		FORCEINLINE ScalarArray<Scalar, N>& ScalarArray<Scalar, N>::operator+=(Zero) noexcept
		{
			return *this;
		}

		template<typename Scalar, std::size_t N>
		FORCEINLINE ScalarArray<Scalar, N>& ScalarArray<Scalar, N>::operator-=(Zero) noexcept
		{
			return *this;
		}

		template<typename Scalar, std::size_t N>
		FORCEINLINE ScalarArray<Scalar, N>& ScalarArray<Scalar, N>::operator*=(Zero) noexcept
		{
			mData.fill(Scalar{});
			return *this;
		}

		template<typename Scalar, std::size_t N>
		template<std::size_t I>
		FORCEINLINE ScalarArray<Scalar, N>& ScalarArray<Scalar, N>::operator=(Unit<I>) noexcept
		{
			mData.fill(Scalar{});
			mData[I] = Scalar{ 1 };
			return *this;
		}

		template<typename Scalar, std::size_t N>
		template<typename Scalar2>
		FORCEINLINE ScalarArray<Scalar, N>& ScalarArray<Scalar, N>::operator=(const Scalar2* v) noexcept
		{
			ASSERT(v != nullptr);

			for (SizeType i = 0; i < GetSize(); ++i)
			{
				mData[i] = static_cast<Scalar>(v[i]);
			}

			return *this;
		}

		template<typename Scalar, std::size_t N>
		template<typename Scalar2>
		FORCEINLINE ScalarArray<Scalar, N>& ScalarArray<Scalar, N>::operator=(const ScalarArray<Scalar2, N>& v) noexcept
		{
			for (SizeType i = 0; i < GetSize(); ++i)
			{
				mData[i] = static_cast<Scalar>(v[i]);
			}

			return *this;
		}

		template<typename Scalar, std::size_t N>
		template<typename Scalar2>
		FORCEINLINE ScalarArray<Scalar, N>& ScalarArray<Scalar, N>::operator+=(const ScalarArray<Scalar2, N>& v) noexcept
		{
			for (SizeType i = 0; i < GetSize(); ++i)
			{
				mData[i] += static_cast<Scalar>(v[i]);
			}

			return *this;
		}

		template<typename Scalar, std::size_t N>
		template<typename Scalar2>
		FORCEINLINE ScalarArray<Scalar, N>& ScalarArray<Scalar, N>::operator-=(const ScalarArray<Scalar2, N>& v) noexcept
		{
			for (SizeType i = 0; i < GetSize(); ++i)
			{
				mData[i] -= static_cast<Scalar>(v[i]);
			}

			return *this;
		}

		template<typename Scalar, std::size_t N>
		template<typename Scalar2>
		FORCEINLINE ScalarArray<Scalar, N>& ScalarArray<Scalar, N>::operator*=(const ScalarArray<Scalar2, N>& v) noexcept
		{
			for (SizeType i = 0; i < GetSize(); ++i)
			{
				mData[i] *= static_cast<Scalar>(v[i]);
			}

			return *this;
		}

		template<typename Scalar, std::size_t N>
		FORCEINLINE ScalarArray<Scalar, N>& ScalarArray<Scalar, N>::operator*=(Scalar s) noexcept
		{
			for (SizeType i = 0; i < GetSize(); ++i)
			{
				mData[i] *= s;
			}

			return *this;
		}

		template<typename Scalar, std::size_t N>
		FORCEINLINE ScalarArray<Scalar, N>& ScalarArray<Scalar, N>::operator/=(Scalar s) noexcept
		{
			ASSERT(!IsZero(s));

			return *this *= Scalar{ 1 } / s;
		}

		template<typename Scalar, std::size_t N>
		FORCEINLINE constexpr typename ScalarArray<Scalar, N>::Iterator ScalarArray<Scalar, N>::Begin() noexcept
		{
			using Iterator = typename ScalarArray<Scalar, N>::Iterator;
			return mData.begin();
		}

		template<typename Scalar, std::size_t N>
		FORCEINLINE constexpr typename ScalarArray<Scalar, N>::ConstIterator ScalarArray<Scalar, N>::Begin() const noexcept
		{
			using ConstIterator = typename ScalarArray<Scalar, N>::ConstIterator;
			return mData.begin();
		}

		template<typename Scalar, std::size_t N>
		inline constexpr typename ScalarArray<Scalar, N>::Iterator ScalarArray<Scalar, N>::End() noexcept
		{
			using Iterator = typename ScalarArray<Scalar, N>::Iterator;
			return mData.end();
		}

		template<typename Scalar, std::size_t N>
		inline constexpr typename ScalarArray<Scalar, N>::ConstIterator ScalarArray<Scalar, N>::End() const noexcept
		{
			using ConstIterator = typename ScalarArray<Scalar, N>::ConstIterator;
			return mData.end();
		}


		//Nomember Function

#ifdef USE_OSTREAM

		template<typename CharT, typename Traits, typename Scalar, std::size_t N>
		FORCEINLINE std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const ScalarArray<Scalar, N>& v)
		{
			for (std::size_t i = 0; i < N; ++i)
			{
				os << v[i] << " ";
			}

			os << std::endl;

			return os;
		}

#endif

#ifdef USE_ISTREAM

		template<typename CharT, typename Traits, typename Scalar, std::size_t N>
		FORCEINLINE std::basic_istream<CharT, Traits>& operator>>(std::basic_ostream<CharT, Traits>& is, const ScalarArray<Scalar, N>& v)
		{
			for (std::size_t i = 0; i < N; ++i)
			{
				is >> v[i];
			}

			return is;
		}

#endif

		template<typename Scalar1, typename Scalar2, std::size_t N>
		FORCEINLINE bool operator==(const ScalarArray<Scalar1, N>& v1, const ScalarArray<Scalar2, N>& v2) noexcept
		{
			for (std::size_t i = 0; i < N; ++i)
			{
				if (v1[i] != v2[i])
				{
					return false;
				}
			}

			return true;
		}

		template<typename Scalar1, typename Scalar2, std::size_t N>
		FORCEINLINE bool operator!=(const ScalarArray<Scalar1, N>& v1, const ScalarArray<Scalar2, N>& v2) noexcept
		{
			return !(v1 == v2);
		}

		template<typename Scalar1, typename Scalar2, std::size_t N>
		FORCEINLINE ScalarArray<typename Promote<Scalar1, Scalar2>::RT, N> operator+(const ScalarArray<Scalar1, N>& v1, const ScalarArray<Scalar2, N>& v2) noexcept
		{
			using RT = typename Promote<Scalar1, Scalar2>::RT;
			ScalarArray<RT, N> result;

			for (std::size_t i = 0; i < N; ++i)
			{
				result[i] = static_cast<RT>(v1[i] + v2[i]);
			}

			return result;
		}

		template<typename Scalar1, typename Scalar2, std::size_t N>
		FORCEINLINE ScalarArray<typename Promote<Scalar1, Scalar2>::RT, N> operator-(const ScalarArray<Scalar1, N>& v1, const ScalarArray<Scalar2, N>& v2) noexcept
		{
			using RT = typename Promote<Scalar1, Scalar2>::RT;
			ScalarArray<RT, N> result;

			for (std::size_t i = 0; i < N; ++i)
			{
				result[i] = static_cast<RT>(v1[i] - v2[i]);
			}

			return result;
		}

		template<typename Scalar1, typename Scalar2, std::size_t N>
		FORCEINLINE ScalarArray<typename Promote<Scalar1, Scalar2>::RT, N> operator*(const ScalarArray<Scalar1, N>& v1, const ScalarArray<Scalar2, N>& v2) noexcept
		{
			using RT = typename Promote<Scalar1, Scalar2>::RT;
			ScalarArray<RT, N> result;

			for (std::size_t i = 0; i < N; ++i)
			{
				result[i] = static_cast<RT>(v1[i] * v2[i]);
			}

			return result;
		}

		template<typename Scalar1, typename Scalar2, std::size_t N>
		FORCEINLINE ScalarArray<typename Promote<Scalar1, Scalar2>::RT, N> operator*(const ScalarArray<Scalar1, N>& v, Scalar2 s) noexcept
		{
			using RT = typename Promote<Scalar1, Scalar2>::RT;
			ScalarArray<RT, N> result;

			for (std::size_t i = 0; i < N; ++i)
			{
				result[i] = static_cast<RT>(v[i] * s);
			}

			return result;
		}

		template<typename Scalar1, typename Scalar2, std::size_t N>
		FORCEINLINE ScalarArray<typename Promote<Scalar1, Scalar2>::RT, N> operator*(Scalar2 s, const ScalarArray<Scalar1, N>& v) noexcept
		{
			return v * s;
		}

		template<typename Scalar1, typename Scalar2, std::size_t N>
		FORCEINLINE ScalarArray<typename Promote<Scalar1, Scalar2>::RT, N> operator/(const ScalarArray<Scalar1, N>& v, Scalar2 s) noexcept
		{
			ASSERT(!IsZero(s));
			using RT = typename Promote<Scalar1, Scalar2>::RT;		

			return v * RT{ RT{1} / s };
		}

		template<typename Scalar, std::size_t N>
		FORCEINLINE std::size_t MaxDimension(const ScalarArray<Scalar, N>& v) noexcept
		{
			std::size_t maxDimension{};

			for (std::size_t i = 1; i < N; ++i)
			{
				if (v[i] > v[maxDimension])
				{
					maxDimension = i;
				}
			}

			return maxDimension;
		}

		template<typename Scalar, std::size_t N>
		FORCEINLINE std::size_t MinDimension(const ScalarArray<Scalar, N>& v) noexcept
		{
			std::size_t minDimension{};

			for (std::size_t i = 1; i < N; ++i)
			{
				if (v[i] < v[minDimension])
				{
					minDimension = i;
				}
			}

			return minDimension;
		}

		template<typename Scalar, std::size_t N>
		FORCEINLINE Scalar HorizontalAdd(const ScalarArray<Scalar, N>& v) noexcept
		{
			Scalar result{};

			for (size_t i = 0; i < N; i++)
			{
				result += v[i];
			}

			return result;
		}

		template<typename Scalar, std::size_t N>
		FORCEINLINE Scalar HorizontalMin(const ScalarArray<Scalar, N>& v) noexcept
		{
			Scalar minComponent = v[0];

			for (size_t i = 1; i < N; i++)
			{
				if (minComponent > v[i])
				{
					minComponent = v[i];
				}
			}

			return minComponent;
		}

		template<typename Scalar, std::size_t N>
		FORCEINLINE Scalar HorizontalMax(const ScalarArray<Scalar, N>& v) noexcept
		{
			Scalar maxComponent = v[0];

			for (size_t i = 1; i < N; i++)
			{
				if (maxComponent < v[i])
				{
					maxComponent = v[i];
				}
			}

			return maxComponent;
		}

		template<typename Scalar1, typename Scalar2, std::size_t N>
		FORCEINLINE typename Promote<Scalar1, Scalar2>::RT Dot(const ScalarArray<Scalar1, N>& v1, const ScalarArray<Scalar2, N>& v2) noexcept
		{
			using RT = typename Promote<Scalar1, Scalar2>::RT;
			return HorizontalAdd(v1 * v2);
		}

		template<typename Scalar1, typename Scalar2, std::size_t N>
		FORCEINLINE typename Promote<Scalar1, Scalar2>::RT AbsDot(const ScalarArray<Scalar1, N>& v1, const ScalarArray<Scalar2, N>& v2) noexcept
		{
			using RT = typename Promote<Scalar1, Scalar2>::RT;
			return HorizontalAdd(v1 * v2);
		}

		template<typename Scalar, std::size_t N>
		FORCEINLINE ScalarArray<Scalar, N> Sin(const ScalarArray<Scalar, N>& v) noexcept
		{
			ScalarArray<Scalar, N> result;

			for (size_t i = 0; i < N; i++)
			{
				result[i] = Sin(v[i]);
			}

			return result;
		}

		template<typename Scalar, std::size_t N>
		FORCEINLINE ScalarArray<Scalar, N> Cos(const ScalarArray<Scalar, N>& v) noexcept
		{
			ScalarArray<Scalar, N> result;

			for (size_t i = 0; i < N; i++)
			{
				result[i] = Cos(v[i]);
			}

			return result;
		}

		template<typename Scalar, std::size_t N>
		FORCEINLINE ScalarArray<Scalar, N> Tan(const ScalarArray<Scalar, N>& v) noexcept
		{
			ScalarArray<Scalar, N> result;

			for (size_t i = 0; i < N; i++)
			{
				result[i] = Tan(v[i]);
			}

			return result;
		}

		template<typename Scalar, std::size_t N>
		FORCEINLINE ScalarArray<Scalar, N> ASin(const ScalarArray<Scalar, N>& v) noexcept
		{
			ScalarArray<Scalar, N> result;

			for (size_t i = 0; i < N; i++)
			{
				result[i] = ASin(v[i]);
			}

			return result;
		}

		template<typename Scalar, std::size_t N>
		FORCEINLINE ScalarArray<Scalar, N> ACos(const ScalarArray<Scalar, N>& v) noexcept
		{
			ScalarArray<Scalar, N> result;

			for (size_t i = 0; i < N; i++)
			{
				result[i] = ACos(v[i]);
			}

			return result;
		}

		template<typename Scalar, std::size_t N>
		FORCEINLINE ScalarArray<Scalar, N> ATan(const ScalarArray<Scalar, N>& v) noexcept
		{
			ScalarArray<Scalar, N> result;

			for (size_t i = 0; i < N; i++)
			{
				result[i] = ATan(v[i]);
			}

			return result;
		}

		template<typename Scalar, std::size_t N>
		FORCEINLINE ScalarArray<Scalar, N> Square(const ScalarArray<Scalar, N>& v) noexcept
		{
			ScalarArray<Scalar, N> result;

			for (size_t i = 0; i < N; i++)
			{
				result[i] = Square(v[i]);
			}

			return result;
		}

		template<typename Scalar, std::size_t N>
		FORCEINLINE ScalarArray<Scalar, N> Cube(const ScalarArray<Scalar, N>& v) noexcept
		{
			ScalarArray<Scalar, N> result;

			for (size_t i = 0; i < N; i++)
			{
				result[i] = Cube(v[i]);
			}

			return result;
		}

		template<typename Scalar, std::size_t N>
		FORCEINLINE ScalarArray<Scalar, N> Abs(const ScalarArray<Scalar, N>& v) noexcept
		{
			ScalarArray<Scalar, N> result;

			for (size_t i = 0; i < N; i++)
			{
				result[i] = Abs(v[i]);
			}

			return result;
		}

		template<typename Scalar, std::size_t N>
		FORCEINLINE ScalarArray<Scalar, N> Saturate(const ScalarArray<Scalar, N>& v) noexcept
		{
			ScalarArray<Scalar, N> result;

			for (size_t i = 0; i < N; i++)
			{
				result[i] = Saturate(v[i]);
			}

			return result;
		}

		template<typename Scalar, std::size_t N>
		FORCEINLINE ScalarArray<Scalar, N> Clamp(const ScalarArray<Scalar, N>& v, Scalar a, Scalar b) noexcept
		{
			ScalarArray<Scalar, N> result;

			for (size_t i = 0; i < N; i++)
			{
				result[i] = Clamp(v[i], a, b);
			}

			return result;
		}

		template<typename Scalar, std::size_t N>
		FORCEINLINE ScalarArray<Scalar, N> SmoothStep(const ScalarArray<Scalar, N>& v) noexcept
		{
			ScalarArray<Scalar, N> result;

			for (size_t i = 0; i < N; i++)
			{
				result[i] = SmoothStep(v[i]);
			}

			return result;
		}

		template<typename Scalar, std::size_t N>
		FORCEINLINE ScalarArray<Scalar, N> Exp(const ScalarArray<Scalar, N>& v) noexcept
		{
			ScalarArray<Scalar, N> result;

			for (size_t i = 0; i < N; i++)
			{
				result[i] = Exp(v[i]);
			}

			return result;
		}

		template<typename Scalar, std::size_t N>
		FORCEINLINE ScalarArray<Scalar, N> Log(const ScalarArray<Scalar, N>& v) noexcept
		{
			ScalarArray<Scalar, N> result;

			for (size_t i = 0; i < N; i++)
			{
				result[i] = Log(v[i]);
			}

			return result;
		}

		template<typename Scalar, std::size_t N>
		FORCEINLINE ScalarArray<Scalar, N> Log2(const ScalarArray<Scalar, N>& v) noexcept
		{
			ScalarArray<Scalar, N> result;

			for (size_t i = 0; i < N; i++)
			{
				result[i] = Log2(v[i]);
			}

			return result;
		}

		template<typename Scalar, std::size_t N>
		FORCEINLINE ScalarArray<Scalar, N> Log10(const ScalarArray<Scalar, N>& v) noexcept
		{
			ScalarArray<Scalar, N> result;

			for (size_t i = 0; i < N; i++)
			{
				result[i] = Log10(v[i]);
			}

			return result;
		}

		template<typename Scalar, std::size_t N>
		FORCEINLINE ScalarArray<Scalar, N> Sqrt(const ScalarArray<Scalar, N>& v) noexcept
		{
			ScalarArray<Scalar, N> result;

			for (size_t i = 0; i < N; i++)
			{
				result[i] = Sqrt(v[i]);
			}

			return result;
		}

		template<typename Scalar, std::size_t N>
		FORCEINLINE ScalarArray<Scalar, N> Rsqrt(const ScalarArray<Scalar, N>& v) noexcept
		{
			ScalarArray<Scalar, N> result;

			for (size_t i = 0; i < N; i++)
			{
				result[i] = Rsqrt(v[i]);
			}

			return result;
		}

		template<typename Scalar, std::size_t N>
		FORCEINLINE ScalarArray<Scalar, N> Cbrt(const ScalarArray<Scalar, N>& v) noexcept
		{
			ScalarArray<Scalar, N> result;

			for (size_t i = 0; i < N; i++)
			{
				result[i] = Cbrt(v[i]);
			}

			return result;
		}

		template<typename Scalar, std::size_t N>
		FORCEINLINE ScalarArray<Scalar, N> Pow(const ScalarArray<Scalar, N>& base, Scalar exp) noexcept
		{
			ScalarArray<Scalar, N> result;

			for (size_t i = 0; i < N; i++)
			{
				result[i] = Pow(base[i], exp);
			}

			return result;
		}

		template<typename Scalar, std::size_t N>
		FORCEINLINE ScalarArray<Scalar, N> Min(const ScalarArray<Scalar, N>& a, const ScalarArray<Scalar, N>& b) noexcept
		{
			ScalarArray<Scalar, N> result;

			for (size_t i = 0; i < N; i++)
			{
				result[i] = Min(a[i], b[i]);
			}

			return result;
		}

		template<typename Scalar, std::size_t N>
		FORCEINLINE ScalarArray<Scalar, N> Max(const ScalarArray<Scalar, N>& a, const ScalarArray<Scalar, N>& b) noexcept
		{
			ScalarArray<Scalar, N> result;

			for (size_t i = 0; i < N; i++)
			{
				result[i] = Max(a[i], b[i]);
			}

			return result;
		}


	}
}
