#pragma once

namespace Dash
{
	template<typename Scalar>
	class AABB<Scalar, 3>
	{
	public:
		using ScalarType = Scalar;

		constexpr AABB() noexcept;
		constexpr explicit AABB(const ScalarArray<Scalar, 3>& p) noexcept;
		constexpr AABB(const ScalarArray<Scalar, 3>& lower, const ScalarArray<Scalar, 3>& upper) noexcept;

		const ScalarArray<Scalar, 3>& operator[](int i) const noexcept;
		ScalarArray<Scalar, 3>& operator[](int i) noexcept;

		ScalarArray<Scalar, 3> Lower;
		ScalarArray<Scalar, 3> Upper;
	};







	// Non-member Function

	// --Declaration-- //

	namespace Math
	{
		template<typename Scalar> ScalarArray<Scalar, 3> Lerp(const AABB<Scalar, 3>& b, const ScalarArray<Scalar, 3>& t) noexcept;
		template<typename Scalar> ScalarArray<Scalar, 3> Offset(const AABB<Scalar, 3>& b, const ScalarArray<Scalar, 3>& p) noexcept;
		template<typename Scalar> void BoundingSphere(const AABB<Scalar, 3>& b, ScalarArray<Scalar, 3>& center, Scalar& radius) noexcept;

		template<typename Scalar> ScalarArray<Scalar, 3> Diagonal(const AABB<Scalar, 3>& b) noexcept;
		template<typename Scalar> ScalarArray<Scalar, 3> Center(const AABB<Scalar, 3>& b) noexcept;

		template<typename Scalar> AABB<Scalar, 3> Union(const ScalarArray<Scalar, 3>& p1,
			const ScalarArray<Scalar, 3>& p2) noexcept;

		template<typename Scalar> AABB<Scalar, 3> Union(const AABB<Scalar, 3>& b,
			const ScalarArray<Scalar, 3>& p) noexcept;

		template<typename Scalar> AABB<Scalar, 3> Union(const ScalarArray<Scalar, 3>& p,
			const AABB<Scalar, 3>& b) noexcept;

		template<typename Scalar> AABB<Scalar, 3> Union(const AABB<Scalar, 3>& b1,
			const AABB<Scalar, 3>& b2) noexcept;

		template<typename Scalar> AABB<Scalar, 3> Intersect(const AABB<Scalar, 3>& b1,
			const AABB<Scalar, 3>& b2) noexcept;

		template<typename Scalar> bool Overlaps(const AABB<Scalar, 3>& b1,
			const AABB<Scalar, 3>& b2) noexcept;

		template<typename Scalar> bool Inside(const ScalarArray<Scalar, 3>& p,
			const AABB<Scalar, 3>& b) noexcept;

		template<typename Scalar> bool InsideExclusive(const ScalarArray<Scalar, 3>& p,
			const AABB<Scalar, 3>& b) noexcept;

		template<typename Scalar> AABB<Scalar, 3> Expand(const AABB<Scalar, 3>& p,
			Scalar delta) noexcept;

		template<typename Scalar> Scalar Distance(const ScalarArray<Scalar, 3>& p,
			const AABB<Scalar, 3>& b) noexcept;

		template<typename Scalar> Scalar DistanceSquared(const ScalarArray<Scalar, 3>& p,
			const AABB<Scalar, 3>& b) noexcept;
	}











	// Member Function

	// --Implementation-- //

	template<typename Scalar>
	FORCEINLINE constexpr AABB<Scalar, 3>::AABB() noexcept
	{
		Lower = ScalarArray<Scalar, 3>{ ScalarTraits<Scalar>::Max() , ScalarTraits<Scalar>::Max() , ScalarTraits<Scalar>::Max() };
		Upper = ScalarArray<Scalar, 3>{ ScalarTraits<Scalar>::Lowest() , ScalarTraits<Scalar>::Lowest() , ScalarTraits<Scalar>::Lowest() };
	}

	template<typename Scalar>
	FORCEINLINE constexpr AABB<Scalar, 3>::AABB(const ScalarArray<Scalar, 3>& p) noexcept
		: Lower(p)
		, Upper(p)
	{
	}

	template<typename Scalar>
	FORCEINLINE constexpr AABB<Scalar, 3>::AABB(const ScalarArray<Scalar, 3>& lower, const ScalarArray<Scalar, 3>& upper) noexcept
		: Lower(lower)
		, Upper(upper)
	{
	}


	template<typename Scalar>
	FORCEINLINE const ScalarArray<Scalar, 3>& AABB<Scalar, 3>::operator[](int i) const noexcept
	{
		ASSERT(i == 0 || i == 1);
		return (i == 0) ? &Lower : &Upper;
	}

	template<typename Scalar>
	FORCEINLINE ScalarArray<Scalar, 3>& AABB<Scalar, 3>::operator[](int i) noexcept
	{
		ASSERT(i == 0 || i == 1);
		return (i == 0) ? &Lower : &Upper;
	}









	// Non-member Function

	// --Implementation-- //

	namespace Math
	{
		template<typename Scalar>
		FORCEINLINE ScalarArray<Scalar, 3> Lerp(const AABB<Scalar, 3>& b, const ScalarArray<Scalar, 3>& t) noexcept
		{
			return ScalarArray<Scalar, 3>{
				Lerp(b.Lower.x, b.Upper.x, t.x),
					Lerp(b.Lower.y, b.Upper.y, t.y),
					Lerp(b.Lower.z, b.Upper.z, t.z)
			};
		}

		template<typename Scalar>
		ScalarArray<Scalar, 3> Offset(const AABB<Scalar, 3>& b, const ScalarArray<Scalar, 3>& p) noexcept
		{
			ScalarArray<Scalar, 3> result = p - b.Lower;
			if (b.Upper.x > b.Lower.x)	result.x /= b.Upper.x - b.Lower.x;
			if (b.Upper.y > b.Lower.y)	result.y /= b.Upper.y - b.Lower.y;
			if (b.Upper.z > b.Lower.z)	result.z /= b.Upper.z - b.Lower.z;

			return result;
		}

		template<typename Scalar>
		FORCEINLINE void BoundingSphere(const AABB<Scalar, 3>& b, ScalarArray<Scalar, 3>& center, Scalar& radius) noexcept
		{
			center = (b.Lower + b.Upper) / Scalar{ 2 };
			radius = Inside(center, *this) ? Distance(center, b.Upper) : Scalar{};
		}

		template<typename Scalar>
		FORCEINLINE ScalarArray<Scalar, 3> Diagonal(const AABB<Scalar, 3>& b) noexcept
		{
			return b.Upper - b.Lower;
		}

		template<typename Scalar>
		FORCEINLINE ScalarArray<Scalar, 3> Center(const AABB<Scalar, 3>& b) noexcept
		{
			return (b.Lower + b.Upper) / Scalar{ 2 };
		}

		template<typename Scalar>
		FORCEINLINE AABB<Scalar, 3> Union(const ScalarArray<Scalar, 3>& p1, const ScalarArray<Scalar, 3>& p2) noexcept
		{
			AABB<Scalar, 3> result;
			result.Lower = Min(p1, p2);
			result.Upper = Max(p1, p2);

			return result;
		}

		template<typename Scalar>
		FORCEINLINE AABB<Scalar, 3> Union(const AABB<Scalar, 3>& b, const ScalarArray<Scalar, 3>& p) noexcept
		{
			AABB<Scalar, 3> result;
			result.Lower = Min(b.Lower, p);
			result.Upper = Max(b.Upper, p);

			return result;
		}

		template<typename Scalar>
		FORCEINLINE AABB<Scalar, 3> Union(const ScalarArray<Scalar, 3>& p, const AABB<Scalar, 3>& b) noexcept
		{
			return Union(b, p);
		}

		template<typename Scalar>
		FORCEINLINE AABB<Scalar, 3> Union(const AABB<Scalar, 3>& b1, const AABB<Scalar, 3>& b2) noexcept
		{
			AABB<Scalar, 3> result;
			result.Lower = Min(b1.Lower, b2.Lower);
			result.Upper = Max(b1.Upper, b2.Upper);

			return result;
		}

		template<typename Scalar>
		FORCEINLINE AABB<Scalar, 3> Intersect(const AABB<Scalar, 3>& b1, const AABB<Scalar, 3>& b2) noexcept
		{
			AABB<Scalar, 3> result;
			result.Lower = Max(b1.Lower, b2.Lower);
			result.Upper = Min(b1.Upper, b2.Upper);

			return result;
		}

		template<typename Scalar>
		FORCEINLINE bool Overlaps(const AABB<Scalar, 3>& b1, const AABB<Scalar, 3>& b2) noexcept
		{
			bool x = (b1.Upper.x >= b2.Lower.y) && (b1.Lower.x <= b2.Upper.x);
			bool y = (b1.Upper.x >= b2.Lower.y) && (b1.Lower.x <= b2.Upper.x);
			bool z = (b1.Upper.x >= b2.Lower.y) && (b1.Lower.x <= b2.Upper.x);

			return x && y && z;
		}

		template<typename Scalar>
		FORCEINLINE bool Inside(const ScalarArray<Scalar, 3>& p, const AABB<Scalar, 3>& b) noexcept
		{
			bool x = (p.x <= b.Upper.x) && (p.x >= b.Lower.x);
			bool y = (p.y <= b.Upper.y) && (p.y >= b.Lower.y);
			bool z = (p.z <= b.Upper.z) && (p.z >= b.Lower.z);

			return x && y && z;
		}

		template<typename Scalar>
		FORCEINLINE bool InsideExclusive(const ScalarArray<Scalar, 3>& p, const AABB<Scalar, 3>& b) noexcept
		{
			bool x = (p.x < b.Upper.x) && (p.x >= b.Lower.x);
			bool y = (p.y < b.Upper.y) && (p.y >= b.Lower.y);
			bool z = (p.z < b.Upper.z) && (p.z >= b.Lower.z);

			return x && y && z;
		}

		template<typename Scalar>
		FORCEINLINE AABB<Scalar, 3> Expand(const AABB<Scalar, 3>& b, Scalar delta) noexcept
		{
			ScalarArray<Scalar, 3> deltaVector{ delta, delta, delta };

			return AABB<Scalar, 3>{ b.Lower - deltaVector, b.Upper + deltaVector };
		}

		template<typename Scalar>
		FORCEINLINE Scalar Distance(const ScalarArray<Scalar, 3>& p, const AABB<Scalar, 3>& b) noexcept
		{
			ScalarArray<Scalar, 3> result{
				Max(Scalar{}, b.Lower.x - p.x, p.x - b.Upper.x),
				Max(Scalar{}, b.Lower.y - p.y, p.y - b.Upper.y),
				Max(Scalar{}, b.Lower.z - p.z, p.z - b.Upper.z)
			};

			return Dot(result, result);
		}

		template<typename Scalar>
		FORCEINLINE Scalar DistanceSquared(const ScalarArray<Scalar, 3>& p, const AABB<Scalar, 3>& b) noexcept
		{
			return Sqrt(Distance(p, b));
		}
	}
}