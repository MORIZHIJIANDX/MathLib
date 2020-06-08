#pragma once

#include "ScalarArray.h"

namespace Dash
{
	namespace Math
	{
		template<typename Scalar>
		class ScalarRay
		{
		public:
			ScalarRay() noexcept;
			ScalarRay(const ScalarArray<Scalar, 3>& origin, const ScalarArray<Scalar, 3>& dir, Scalar tmin = ScalarTraits<Scalar>::Epsilon(), Scalar tmax = ScalarTraits<Scalar>::Infinity()) noexcept;
			
			ScalarRay(const ScalarRay& r) noexcept;

			ScalarRay<Scalar>& operator=(const ScalarRay<Scalar>& r) noexcept;

			ScalarArray<Scalar, 3> operator()(Scalar t) const noexcept;

			ScalarArray<Scalar, 3> Origin;
			Scalar TMax;
			ScalarArray<Scalar, 3> Direction;
			Scalar TMin;
		};

		template<typename Scalar>
		FORCEINLINE ScalarRay<Scalar>::ScalarRay() noexcept
			: Origin(ScalarArray<Scalar, 3>{ Zero{} })
			, Direction(ScalarArray<Scalar, 3>{ Unit<2>{} })
			, TMin(ScalarTraits<Scalar>::Epsilon())
			, TMax(ScalarTraits<Scalar>::Infinity())
		{
		}

		template<typename Scalar>
		FORCEINLINE ScalarRay<Scalar>::ScalarRay(const ScalarArray<Scalar, 3>& origin, const ScalarArray<Scalar, 3>& dir, Scalar tmin, Scalar tmax) noexcept
			: Origin(origin)
			, Direction(dir)
			, TMin(tmin)
			, TMax(tmax)
		{
		}

		template<typename Scalar>
		FORCEINLINE ScalarRay<Scalar>::ScalarRay(const ScalarRay& r) noexcept
			: Origin(r.Origin)
			, Direction(r.Direction)
			, TMin(r.TMin)
			, TMax(r.TMax)
		{		
		}

		template<typename Scalar>
		FORCEINLINE ScalarRay<Scalar>& ScalarRay<Scalar>::operator=(const ScalarRay& r) noexcept
		{
			Origin = r.Origin;
			Direction = r.Direction;
			TMin = r.TMin;
			TMax = r.TMax;

			return *this;
		}

		template<typename Scalar>
		FORCEINLINE ScalarArray<Scalar, 3> ScalarRay<Scalar>::operator()(Scalar t) const noexcept
		{
			return Origin + Direction * t;
		}

	}
}