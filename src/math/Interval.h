#pragma once

#include "../consolid/consolid.h"
#include "Promote.h"

#ifdef USE_OSTREAM
#include <ostream>	
#endif // USE_OSTREAM

namespace Dash
{
	namespace Math
	{
		template<typename Scalar>
		class Interval
		{
		public:
			constexpr Interval() noexcept;
			constexpr explicit Interval(Scalar x) noexcept;
			constexpr Interval(Scalar lower, Scalar upper) noexcept;
			template<typename Scalar2> Interval(const Interval<Scalar2> inter) noexcept;

			Scalar Lower() const noexcept;
			Scalar& Lower() noexcept;

			Scalar Upper() const noexcept;
			Scalar& Upper() noexcept;
		private:
			Scalar mLower;
			Scalar mUpper;
		};


		template <typename Scalar> Scalar Lower(const Interval<Scalar>& z) noexcept;
		template <typename Scalar> Scalar Upper(const Interval<Scalar>& z) noexcept;
		template <typename Scalar> Scalar Width(const Interval<Scalar>& z) noexcept;
		template <typename Scalar> Scalar Median(const Interval<Scalar>& z) noexcept;
		
#ifdef USE_OSTREAM

		template <typename CharT, typename Traits, typename Scalar>
		std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const Interval<Scalar>& z);

#endif

		template <typename Scalar1, typename Scalar2> bool Overlap(const Interval<Scalar1>& z1, const Interval<Scalar2>& z2) noexcept;
		template <typename Scalar1, typename Scalar2> bool Subset(const Interval<Scalar1>& z1, const Interval<Scalar2>& z2) noexcept;
		template <typename Scalar> bool In(Scalar x, const Interval<Scalar>& z) noexcept;

		template <typename Scalar> Interval<Scalar> Hull(Scalar x, Scalar y) noexcept;
		template <typename Scalar> Interval<Scalar> Hull(const Interval<Scalar>& z, Scalar x) noexcept;
		template <typename Scalar> Interval<Scalar> Hull(Scalar x, const Interval<Scalar>& z) noexcept;
		template <typename Scalar> Interval<Scalar> Hull(const Interval<Scalar>& z1, const Interval<Scalar>& z2) noexcept;

		template <typename Scalar> Scalar           Clamp(Scalar x, const Interval<Scalar>& z) noexcept;
		template <typename Scalar> Interval<Scalar> Clamp(const Interval<Scalar>& z1, const Interval<Scalar>& z2) noexcept;



		template<typename Scalar>
		FORCEINLINE constexpr Interval<Scalar>::Interval() noexcept
			: mLower()
			, mUpper()
		{
		}

		template<typename Scalar>
		FORCEINLINE constexpr Interval<Scalar>::Interval(Scalar x) noexcept
			: mLower(x)
			, mUpper(x)
		{
		}

		template<typename Scalar>
		FORCEINLINE constexpr Interval<Scalar>::Interval(Scalar lower, Scalar upper) noexcept
			: mLower(lower)
			, mUpper(upper)
		{
			ASSERT(upper >= lower);
		}

		template<typename Scalar>
		template<typename Scalar2>
		FORCEINLINE Interval<Scalar>::Interval(const Interval<Scalar2> inter) noexcept
			: mLower(Scalar{ inter.mLower })
			, mUpper(Scalar{ inter.mUpper })
		{
		}

		template<typename Scalar>
		FORCEINLINE Scalar Interval<Scalar>::Lower() const noexcept
		{
			return mLower;
		}

		template<typename Scalar>
		FORCEINLINE Scalar& Interval<Scalar>::Lower() noexcept
		{
			return &mLower;
		}

		template<typename Scalar>
		FORCEINLINE Scalar Interval<Scalar>::Upper() const noexcept
		{
			return mUpper;
		}

		template<typename Scalar>
		FORCEINLINE Scalar& Interval<Scalar>::Upper() noexcept
		{
			return &mUpper;
		}


		template<typename Scalar>
		FORCEINLINE Scalar Lower(const Interval<Scalar>& z) noexcept
		{
			return z.Lower();
		}

		template<typename Scalar>
		FORCEINLINE Scalar Upper(const Interval<Scalar>& z) noexcept
		{
			return z.Upper();
		}

		template<typename Scalar>
		FORCEINLINE Scalar Width(const Interval<Scalar>& z) noexcept
		{
			return z.Upper() - z.Lower();
		}

		template<typename Scalar>
		FORCEINLINE Scalar Median(const Interval<Scalar>& z) noexcept
		{
			return (z.Upper() + z.Lower()) * Scalar { 0.5 };
		}

#ifdef USE_OSTREAM
		template<typename CharT, typename Traits, typename Scalar>
		std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const Interval<Scalar>& z)
		{
			os << "[" << z.Lower() << ", " << z.Upper() << "]";
		}
#endif

		template<typename Scalar1, typename Scalar2>
		FORCEINLINE bool Overlap(const Interval<Scalar1>& z1, const Interval<Scalar2>& z2) noexcept
		{
			return !(z1.Upper() < z2.Lower() || z2.Upper() < z1.Lower());
		}

		template<typename Scalar1, typename Scalar2>
		FORCEINLINE bool Subset(const Interval<Scalar1>& z1, const Interval<Scalar2>& z2) noexcept
		{
			return !(z1.Lower() < z2.Lower() || (z2.Upper() < z1.Upper()));
		}

		template<typename Scalar>
		FORCEINLINE bool In(Scalar x, const Interval<Scalar>& z) noexcept
		{
			return !(x < z.Lower() || x > z.Upper()) ;
		}


	}
}