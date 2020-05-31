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
			ScalarRay(const ScalarArray<Scalar, 3>& origin, const ScalarArray<Scalar, 3>& dir, Scalar tmax = ScalarTraits<Scalar>::Infinity(), Scalar time = Scalar{}) noexcept;
			
			ScalarRay(const ScalarRay& r) noexcept;

			ScalarRay<Scalar>& operator=(const ScalarRay<Scalar>& r) noexcept;

			ScalarArray<Scalar, 3> operator()(Scalar t) const noexcept;

			ScalarArray<Scalar, 3> Origin;
			Scalar TMax;
			ScalarArray<Scalar, 3> Direction;
			Scalar Time;
		};

		template<typename Scalar>
		FORCEINLINE ScalarRay<Scalar>::ScalarRay() noexcept
			: Origin(ScalarArray<Scalar, 3>{ Zero{} })
			, Direction(ScalarArray<Scalar, 3>{ Unit<2>{} })
			, TMax(ScalarTraits<Scalar>::Infinity())
			, Time(Scalar{})
		{
		}

		template<typename Scalar>
		FORCEINLINE ScalarRay<Scalar>::ScalarRay(const ScalarArray<Scalar, 3>& origin, const ScalarArray<Scalar, 3>& dir, Scalar tmax, Scalar time) noexcept
			: Origin(origin)
			, Direction(dir)
			, TMax(tmax)
			, Time(time)
		{
		}

		template<typename Scalar>
		FORCEINLINE ScalarRay<Scalar>::ScalarRay(const ScalarRay& r) noexcept
			: Origin(r.Origin)
			, Direction(r.Direction)
			, TMax(r.TMax)
			, Time(r.Time)
		{		
		}

		template<typename Scalar>
		FORCEINLINE ScalarRay<Scalar>& ScalarRay<Scalar>::operator=(const ScalarRay& r) noexcept
		{
			Origin = r.Origin;
			Direction = r.Direction;
			TMax = r.TMax;
			Time = r.Time;

			return *this;
		}

		template<typename Scalar>
		FORCEINLINE ScalarArray<Scalar, 3> ScalarRay<Scalar>::operator()(Scalar t) const noexcept
		{
			return Origin + Direction * t;
		}

	}
}