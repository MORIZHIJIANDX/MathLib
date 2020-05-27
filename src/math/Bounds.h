#pragma once

#include "ScalarArray.h"

namespace Dash
{
	namespace Math
	{
		template<typename Scalar, std::size_t N>
		class Bounds
		{
		public:
			constexpr Bounds() noexcept;
			constexpr explicit Bounds(const ScalarArray<Scalar, N>& p) noexcept;
			constexpr Bounds(const ScalarArray<Scalar, N>& lower, const ScalarArray<Scalar, N>& upper) noexcept;

			ScalarArray<Scalar, N> Lower() const noexcept;
			ScalarArray<Scalar, N>& Lower() noexcept;

			ScalarArray<Scalar, N> Upper() const noexcept;
			ScalarArray<Scalar, N>& Upper() noexcept;
			
			ScalarArray<Scalar, N> Lerp(const ScalarArray<Scalar, N>& t) noexcept;
			ScalarArray<Scalar, N> Offset(const ScalarArray<Scalar, N>& p) noexcept;
			void BoundingSphere(ScalarArray<Scalar, N>& center, Scalar& radius) noexcept;

		private:
			ScalarArray<Scalar, N> mLower;
			ScalarArray<Scalar, N> mUpper;
		};

		template<typename Scalar, std::size_t N> ScalarArray<Scalar, N> Lower(const Bounds<Scalar, N>& b) noexcept;
		template<typename Scalar, std::size_t N> ScalarArray<Scalar, N> Upper(const Bounds<Scalar, N>& b) noexcept;

		template<typename Scalar, std::size_t N> ScalarArray<Scalar, N> Center(const Bounds<Scalar, N>& b) noexcept;

		template<typename Scalar, std::size_t N> Bounds<Scalar, N> Union(const ScalarArray<Scalar, N>& p1,
			const ScalarArray<Scalar, N>& p2) noexcept;

		template<typename Scalar, std::size_t N> Bounds<Scalar, N> Union(const Bounds<Scalar, N>& b,
			const ScalarArray<Scalar, N>& p) noexcept;

		template<typename Scalar, std::size_t N> Bounds<Scalar, N> Union(const ScalarArray<Scalar, N>& p,
			const Bounds<Scalar, N>& b) noexcept;

		template<typename Scalar, std::size_t N> Bounds<Scalar, N> Union(const Bounds<Scalar, N>& b1,
			const Bounds<Scalar, N>& b2) noexcept;

		template<typename Scalar, std::size_t N> Bounds<Scalar, N> Intersect(const Bounds<Scalar, N>& b1,
			const Bounds<Scalar, N>& b2) noexcept;

		template<typename Scalar, std::size_t N> bool Overlaps(const Bounds<Scalar, N>& b1,
			const Bounds<Scalar, N>& b2) noexcept;

		template<typename Scalar, std::size_t N> bool Inside(const ScalarArray<Scalar, N>& p,
			const Bounds<Scalar, N>& b) noexcept;

		template<typename Scalar, std::size_t N> bool InsideExclusive(const ScalarArray<Scalar, N>& p,
			const Bounds<Scalar, N>& b) noexcept;

		template<typename Scalar, std::size_t N> bool Expand(const Bounds<Scalar, N>& p,
			Scalar delta) noexcept;

		template<typename Scalar, std::size_t N> bool Distance(const ScalarArray<Scalar, N>& p,
			const Bounds<Scalar, N>& b) noexcept;

		template<typename Scalar, std::size_t N> bool DistanceSquared(const ScalarArray<Scalar, N>& p,
			const Bounds<Scalar, N>& b) noexcept;
	}
}