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
			
			ScalarArray<Scalar, N> Lerp() noexcept;
			ScalarArray<Scalar, N> Offset() noexcept;
			void BoundingSphere() noexcept;

		private:
			ScalarArray<Scalar, N> mLower;
			ScalarArray<Scalar, N> mUpper;
		};
	}
}