#pragma once

#include "ScalarArray.h"
#include "ScalarTraits.h"

namespace Dash
{
	namespace Math
	{
		template<typename Scalar, std::size_t N>
		class AABB
		{
		public:
			using ScalarType = Scalar;

			constexpr AABB() noexcept;
			constexpr explicit AABB(const ScalarArray<Scalar, N>& p) noexcept;
			constexpr AABB(const ScalarArray<Scalar, N>& lower, const ScalarArray<Scalar, N>& upper) noexcept;

			const ScalarArray<Scalar, N>& operator[](int i) const noexcept;
			ScalarArray<Scalar, N>& operator[](int i) noexcept;

			ScalarArray<Scalar, N> Lower;
			ScalarArray<Scalar, N> Upper;
		};

		template<typename Scalar, std::size_t N> bool operator==(const AABB<Scalar, N>& b1, const AABB<Scalar, N>& b2) noexcept;
		template<typename Scalar, std::size_t N> bool operator!=(const AABB<Scalar, N>& b1, const AABB<Scalar, N>& b2) noexcept;

		template<typename Scalar, std::size_t N> ScalarArray<Scalar, N> Lerp(const AABB<Scalar, N>& b, const ScalarArray<Scalar, N>& t) noexcept;
		template<typename Scalar, std::size_t N> ScalarArray<Scalar, N> Offset(const AABB<Scalar, N>& b, const ScalarArray<Scalar, N>& p) noexcept;
		template<typename Scalar, std::size_t N> void BoundingSphere(const AABB<Scalar, N>& b, ScalarArray<Scalar, N>& center, Scalar& radius) noexcept;

		template<typename Scalar, std::size_t N> ScalarArray<Scalar, N> Diagonal(const AABB<Scalar, N>& b) noexcept;
		template<typename Scalar, std::size_t N> ScalarArray<Scalar, N> Center(const AABB<Scalar, N>& b) noexcept;

		template<typename Scalar, std::size_t N> AABB<Scalar, N> Union(const ScalarArray<Scalar, N>& p1,
			const ScalarArray<Scalar, N>& p2) noexcept;

		template<typename Scalar, std::size_t N> AABB<Scalar, N> Union(const AABB<Scalar, N>& b,
			const ScalarArray<Scalar, N>& p) noexcept;

		template<typename Scalar, std::size_t N> AABB<Scalar, N> Union(const ScalarArray<Scalar, N>& p,
			const AABB<Scalar, N>& b) noexcept;

		template<typename Scalar, std::size_t N> AABB<Scalar, N> Union(const AABB<Scalar, N>& b1,
			const AABB<Scalar, N>& b2) noexcept;

		template<typename Scalar, std::size_t N> AABB<Scalar, N> Intersect(const AABB<Scalar, N>& b1,
			const AABB<Scalar, N>& b2) noexcept;

		template<typename Scalar, std::size_t N> bool Overlaps(const AABB<Scalar, N>& b1,
			const AABB<Scalar, N>& b2) noexcept;

		template<typename Scalar, std::size_t N> bool Inside(const ScalarArray<Scalar, N>& p,
			const AABB<Scalar, N>& b) noexcept;

		template<typename Scalar, std::size_t N> bool InsideExclusive(const ScalarArray<Scalar, N>& p,
			const AABB<Scalar, N>& b) noexcept;

		template<typename Scalar, std::size_t N> AABB<Scalar, N> Expand(const AABB<Scalar, N>& p,
			Scalar delta) noexcept;

		template<typename Scalar, std::size_t N> Scalar Distance(const ScalarArray<Scalar, N>& p,
			const AABB<Scalar, N>& b) noexcept;

		template<typename Scalar, std::size_t N> Scalar DistanceSquared(const ScalarArray<Scalar, N>& p,
			const AABB<Scalar, N>& b) noexcept;



		template<typename Scalar, std::size_t N>
		FORCEINLINE constexpr AABB<Scalar, N>::AABB() noexcept
		{
			Lower.Fill(ScalarTraits<Scalar>::Max());
			Upper.Fill(ScalarTraits<Scalar>::Lowest()); 
		}

		template<typename Scalar, std::size_t N>
		FORCEINLINE constexpr AABB<Scalar, N>::AABB(const ScalarArray<Scalar, N>& p) noexcept
			: Lower(p)
			, Upper(p)
		{
		}

		template<typename Scalar, std::size_t N>
		FORCEINLINE constexpr AABB<Scalar, N>::AABB(const ScalarArray<Scalar, N>& lower, const ScalarArray<Scalar, N>& upper) noexcept
			: Lower(lower)
			, Upper(upper)
		{
		}


		template<typename Scalar, std::size_t N>
		FORCEINLINE const ScalarArray<Scalar, N>& AABB<Scalar, N>::operator[](int i) const noexcept
		{
			ASSERT(i == 0 || i == 1);
			return (i == 0) ? &Lower : &Upper;
		}

		template<typename Scalar, std::size_t N>
		FORCEINLINE ScalarArray<Scalar, N>& AABB<Scalar, N>::operator[](int i) noexcept
		{
			ASSERT(i == 0 || i == 1);
			return (i == 0) ? &Lower : &Upper;
		}





		template<typename Scalar, std::size_t N>
		bool operator==(const AABB<Scalar, N>& b1, const AABB<Scalar, N>& b2) noexcept
		{
			return b1.Lower == b2.Lower && b1.Upper == b2.Upper;
		}

		template<typename Scalar, std::size_t N>
		bool operator!=(const AABB<Scalar, N>& b1, const AABB<Scalar, N>& b2) noexcept
		{
			return !(b1 == b2);
		}

		template<typename Scalar, std::size_t N>
		FORCEINLINE ScalarArray<Scalar, N> Lerp(const AABB<Scalar, N>& b, const ScalarArray<Scalar, N>& t) noexcept
		{
			ScalarArray<Scalar, N> result;
			for (std::size_t i = 0; i < N; i++)
			{
				result[i] = Lerp(b.Lower[i], b.Upper[i], t[i]);
			}

			return result;
		}

		template<typename Scalar, std::size_t N>
		ScalarArray<Scalar, N> Offset(const AABB<Scalar, N>& b, const ScalarArray<Scalar, N>& p) noexcept
		{
			ScalarArray<Scalar, N> result = p - b.Lower;
			for (std::size_t i = 0; i < N; i++)
			{
				if (b.Upper[i] > b.Lower[i])
				{
					result[i] /= b.Upper[i] - b.Lower[i];
				}
			}

			return result;
		}

		template<typename Scalar, std::size_t N>
		FORCEINLINE void BoundingSphere(const AABB<Scalar, N>& b, ScalarArray<Scalar, N>& center, Scalar& radius) noexcept
		{
			center = (b.Lower + b.Upper) / Scalar{ 2 };
			radius = Inside(center, *this) ? Distance(center, b.Upper) : Scalar{};
		}

		template<typename Scalar, std::size_t N>
		FORCEINLINE ScalarArray<Scalar, N> Diagonal(const AABB<Scalar, N>& b) noexcept
		{
			return b.Upper - b.Lower;
		}

		template<typename Scalar, std::size_t N>
		FORCEINLINE ScalarArray<Scalar, N> Center(const AABB<Scalar, N>& b) noexcept
		{
			return (b.Lower + b.Upper) / Scalar{ 2 };
		}

		template<typename Scalar, std::size_t N>
		FORCEINLINE AABB<Scalar, N> Union(const ScalarArray<Scalar, N>& p1, const ScalarArray<Scalar, N>& p2) noexcept
		{
			AABB<Scalar, N> result;
			for (std::size_t i = 0; i < N; i++)
			{
				result.Lower[i] = Min(p1[i], p2[i]);
				result.Upper[i] = Max(p1[i], p2[i]);
			}

			return result;
		}

		template<typename Scalar, std::size_t N>
		FORCEINLINE AABB<Scalar, N> Union(const AABB<Scalar, N>& b, const ScalarArray<Scalar, N>& p) noexcept
		{
			AABB<Scalar, N> result;
			for (std::size_t i = 0; i < N; i++)
			{
				result.Lower[i] = Min(b.Lower[i], p[i]);
				result.Upper[i] = Max(b.Upper[i], p[i]);
			}

			return result;
		}

		template<typename Scalar, std::size_t N>
		FORCEINLINE AABB<Scalar, N> Union(const ScalarArray<Scalar, N>& p, const AABB<Scalar, N>& b) noexcept
		{
			return Union(b, p);
		}

		template<typename Scalar, std::size_t N>
		FORCEINLINE AABB<Scalar, N> Union(const AABB<Scalar, N>& b1, const AABB<Scalar, N>& b2) noexcept
		{
			ScalarArray<Scalar, N> result;
			for (std::size_t i = 0; i < N; i++)
			{
				result.Lower[i] = Min(b1.Lower[i], b2.Lower[i]);
				result.Upper[i] = Max(b1.Upper[i], b2.Upper[i]);
			}

			return result;
		}

		template<typename Scalar, std::size_t N>
		FORCEINLINE AABB<Scalar, N> Intersect(const AABB<Scalar, N>& b1, const AABB<Scalar, N>& b2) noexcept
		{
			AABB<Scalar, N> result;
			for (std::size_t i = 0; i < N; i++)
			{
				result.Lower[i] = Max(b1.Lower[i], b2.Lower[i]);
				result.Upper[i] = Min(b1.Upper[i], b2.Upper[i]);
			}

			return result;
		}

		template<typename Scalar, std::size_t N>
		FORCEINLINE bool Overlaps(const AABB<Scalar, N>& b1, const AABB<Scalar, N>& b2) noexcept
		{
			ScalarArray<bool, N> result;
			for (std::size_t i = 0; i < N; i++)
			{
				result[i] = (b1.Upper[i] >= b2.Lower[i]) && (b1.Lower[i] <= b2.Upper[i]);
			}

			for (std::size_t i = 1; i < N; i++)
			{
				result[0] = result[0] && result[i];
			}

			return result[0];
		}

		template<typename Scalar, std::size_t N>
		FORCEINLINE bool Inside(const ScalarArray<Scalar, N>& p, const AABB<Scalar, N>& b) noexcept
		{
			ScalarArray<bool, N> result;
			for (std::size_t i = 0; i < N; i++)
			{
				result[i] = (p[i] <= b.Upper[i]) && (p[i] >= b.Lower[i]);
			}

			for (std::size_t i = 1; i < N; i++)
			{
				result[0] = result[0] && result[i];
			}

			return result[0];
		}

		template<typename Scalar, std::size_t N>
		FORCEINLINE bool InsideExclusive(const ScalarArray<Scalar, N>& p, const AABB<Scalar, N>& b) noexcept
		{
			ScalarArray<bool, N> result;
			for (std::size_t i = 0; i < N; i++)
			{
				result[i] = (p[i] < b.Upper[i]) && (p[i] >= b.Lower[i]);
			}

			for (std::size_t i = 1; i < N; i++)
			{
				result[0] = result[0] && result[i];
			}

			return result[0];
		}

		template<typename Scalar, std::size_t N>
		FORCEINLINE AABB<Scalar, N> Expand(const AABB<Scalar, N>& b, Scalar delta) noexcept
		{
			ScalarArray<Scalar, N> deltaVector;
			deltaVector.Fill(delta);

			return AABB<Scalar, N>{ b.Lower - deltaVector, b.Upper + deltaVector };
		}

		template<typename Scalar, std::size_t N>
		FORCEINLINE Scalar Distance(const ScalarArray<Scalar, N>& p, const AABB<Scalar, N>& b) noexcept
		{
			ScalarArray<Scalar, N> result;

			for (std::size_t i = 0; i < N; i++)
			{
				result[i] = Max(Scalar{}, b.Lower[i] - p[i], p[i] - b.Upper[i]);
			}

			return Dot(result, result);
		}

		template<typename Scalar, std::size_t N>
		FORCEINLINE Scalar DistanceSquared(const ScalarArray<Scalar, N>& p, const AABB<Scalar, N>& b) noexcept
		{
			return Sqrt(Distance(p, b));
		}
	}
}

#include "AABB2.h"
#include "AABB3.h"