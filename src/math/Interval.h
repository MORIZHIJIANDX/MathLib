#pragma once

#include "../consolid/consolid.h"
#include "Scalar.h"
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

			Scalar Lower;
			Scalar Upper;
		};

		template <typename Scalar> Scalar Width(const Interval<Scalar>& z) noexcept;
		template <typename Scalar> Scalar Median(const Interval<Scalar>& z) noexcept;
		
#ifdef USE_OSTREAM

		template <typename CharT, typename Traits, typename Scalar>
		std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const Interval<Scalar>& z);

#endif

		template <typename Scalar1, typename Scalar2> bool Overlap(const Interval<Scalar1>& z1, const Interval<Scalar2>& z2) noexcept;
		template <typename Scalar1, typename Scalar2> bool Subset(const Interval<Scalar1>& z1, const Interval<Scalar2>& z2) noexcept;
		template <typename Scalar> bool In(Scalar x, const Interval<Scalar>& z) noexcept;

		template <typename Scalar> Interval<Scalar> Union(Scalar x, Scalar y) noexcept;
		template <typename Scalar> Interval<Scalar> Union(const Interval<Scalar>& z, Scalar x) noexcept;
		template <typename Scalar> Interval<Scalar> Union(Scalar x, const Interval<Scalar>& z) noexcept;
		template <typename Scalar> Interval<Scalar> Union(const Interval<Scalar>& z1, const Interval<Scalar>& z2) noexcept;

		template <typename Scalar> Scalar           Clamp(Scalar x, const Interval<Scalar>& z) noexcept;
		template <typename Scalar> Interval<Scalar> Clamp(const Interval<Scalar>& z1, const Interval<Scalar>& z2) noexcept;



		template<typename Scalar>
		FORCEINLINE constexpr Interval<Scalar>::Interval() noexcept
			: Lower(ScalarTraits<Scalar>::Max())
			, Upper(ScalarTraits<Scalar>::Lowest())
		{
		}

		template<typename Scalar>
		FORCEINLINE constexpr Interval<Scalar>::Interval(Scalar x) noexcept
			: Lower(x)
			, Upper(x)
		{
		}

		template<typename Scalar>
		FORCEINLINE constexpr Interval<Scalar>::Interval(Scalar lower, Scalar upper) noexcept
			: Lower(lower)
			, Upper(upper)
		{
			ASSERT(upper >= lower);
		}

		template<typename Scalar>
		template<typename Scalar2>
		FORCEINLINE Interval<Scalar>::Interval(const Interval<Scalar2> inter) noexcept
			: Lower(Scalar{ inter.Lower })
			, Upper(Scalar{ inter.Upper })
		{
		}


		template<typename Scalar>
		FORCEINLINE Scalar Lower(const Interval<Scalar>& z) noexcept
		{
			return z.Lower;
		}

		template<typename Scalar>
		FORCEINLINE Scalar Upper(const Interval<Scalar>& z) noexcept
		{
			return z.Upper;
		}

		template<typename Scalar>
		FORCEINLINE Scalar Width(const Interval<Scalar>& z) noexcept
		{
			return z.Upper - z.Lower;
		}

		template<typename Scalar>
		FORCEINLINE Scalar Median(const Interval<Scalar>& z) noexcept
		{
			return (z.Upper + z.Lower) * Scalar { 0.5 };
		}

#ifdef USE_OSTREAM
		template<typename CharT, typename Traits, typename Scalar>
		std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const Interval<Scalar>& z)
		{
			os << "[" << z.Lower << ", " << z.Upper << "]";
		}
#endif

		template<typename Scalar1, typename Scalar2>
		FORCEINLINE bool Overlap(const Interval<Scalar1>& z1, const Interval<Scalar2>& z2) noexcept
		{
			return !(z1.Upper < z2.Lower || z2.Upper < z1.Lower);
		}

		template<typename Scalar1, typename Scalar2>
		FORCEINLINE bool Subset(const Interval<Scalar1>& z1, const Interval<Scalar2>& z2) noexcept
		{
			return !(z1.Lower < z2.Lower || (z2.Upper < z1.Upper));
		}

		template<typename Scalar>
		FORCEINLINE bool In(Scalar x, const Interval<Scalar>& z) noexcept
		{
			return !(x < z.Lower || x > z.Upper) ;
		}

		template<typename Scalar>
		FORCEINLINE Interval<Scalar> Union(Scalar x, Scalar y) noexcept
		{
			return x < y ? Interval<Scalar>{ x , y } : Interval<Scalar>{ y , x };
		}

		template<typename Scalar>
		FORCEINLINE Interval<Scalar> Union(const Interval<Scalar>& z, Scalar x) noexcept
		{
			return x < z.Lower ?  Interval<Scalar>{x, z.Upper} : ( x > z.Upper ? Interval<Scalar>{ z.Lower, x } : z);
		}

		template<typename Scalar>
		FORCEINLINE Interval<Scalar> Union(Scalar x, const Interval<Scalar>& z) noexcept
		{
			return Union(z, x);
		}

		template<typename Scalar>
		FORCEINLINE Interval<Scalar> Union(const Interval<Scalar>& z1, const Interval<Scalar>& z2) noexcept
		{
			return Interval<Scalar>{ Min(z1.Lower, z2.Lower) , Max(z1.Upper, z2.Upper) };
		}

		template<typename Scalar>
		FORCEINLINE Scalar Clamp(Scalar x, const Interval<Scalar>& z) noexcept
		{
			return Clamp(x, z.Lower, z.Upper);
		}

		template<typename Scalar>
		FORCEINLINE Interval<Scalar> Clamp(const Interval<Scalar>& z1, const Interval<Scalar>& z2) noexcept
		{
			return z1.Upper < z2.Lower ? Interval<Scalar>(z2.Lower, z2.Lower) :
				z2.Upper < z1.Lower ? Interval<Scalar>(z2.Upper, z2.Upper) :
				Interval<Scalar>{ Max(z1.Lower, z2.Lower), Min(z1.Upper, z2.Upper) };
		}


	}
}