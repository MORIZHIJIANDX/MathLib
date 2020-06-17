#pragma once

#include "..//consolid/consolid.h"

#ifdef USE_OSTREAM

#include <ostream>	

#endif // USE_OSTREAM


namespace Dash {
	

	struct Zero
	{
		template<typename Scalar> explicit operator Scalar() const noexcept;
	};

	struct Identity
	{
		template<typename Scalar> explicit operator Scalar() const noexcept;
	};

	template<std::size_t>
	struct Unit
	{
	};




	// Non-member Operators 

	// --Declaration-- //

	Zero operator-(Zero);
	Zero operator+(Zero, Zero);
	Identity operator+(Zero, Identity);
	Identity operator+(Identity, Zero);

	template<typename Element> const Element& operator+(Zero, const Element& e);
	template<typename Element> const Element& operator+(const Element& e, Zero);


	Zero operator-(Zero, Zero);
	Zero operator-(Identity, Identity);
	Identity operator-(Identity, Zero);

	template<typename Element>       Element  operator-(Zero, const Element& e);
	template<typename Element> const Element& operator-(const Element& e, Zero);


	Zero operator*(Zero, Zero);
	Zero operator*(Zero, Identity);
	Zero operator*(Identity, Zero);

	template<typename Element> Zero operator*(Zero, const Element& e);
	template<typename Element> Zero operator*(const Element& e, Zero);

#ifdef USE_OSTREAM

	template<typename CharT, typename Traits>
	std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, Zero);

	template<typename CharT, typename Traits>
	std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, Identity);

	template<typename CharT, typename Traits, std::size_t I>
	std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, Unit<I>);

#endif // USE_OSTREAM







	// Non-member Function

	// --Declaration-- //

	namespace Math
	{
		Identity Inverse(Identity);

		Zero Mul(Zero, Zero);
		Zero Mul(Zero, Identity);
		Zero Mul(Identity, Zero);

		template<typename Element> Zero Mul(Zero, const Element& e);
		template<typename Element> Zero Mul(const Element& e, Zero);


		Identity Mul(Identity, Identity);

		template<typename Element> const Element& Mul(Identity, const Element& e);
		template<typename Element> const Element& Mul(const Element& e, Identity);


		template<typename Element> Element Mul(const Element& a, const Element& b, const Element& c);


		Zero Dot(Zero, Zero);

		template<typename Vector> Zero Dot(Zero, const Vector& v);
		template<typename Vector> Zero Dot(const Vector& v, Zero);


		template<std::size_t I> Identity Dot(Unit<I>, Unit<I>);
		template<std::size_t I, std::size_t J> Zero Dot(Unit<I>, Unit<J>);


		template<typename Vector, std::size_t I> typename Vector::ScalarType Dot(const Vector& v, Unit<I>);
		template<typename Vector, std::size_t I> typename Vector::ScalarType Dot(Unit<I>, const Vector& v);
	}







	// Member Function

	// --Implementation-- //

	template<typename Scalar>
	FORCEINLINE Zero::operator Scalar() const noexcept
	{
		return Scalar{};
	}

	template<typename Scalar>
	FORCEINLINE Identity::operator Scalar() const noexcept
	{
		return Scalar{ 1 };
	}





	// Non-member Operators

	// --Implementation-- //

	FORCEINLINE Zero operator-(Zero)
	{
		return Zero{};
	}

	FORCEINLINE Zero operator+(Zero, Zero)
	{
		return Zero{};
	}

	FORCEINLINE Identity operator+(Zero, Identity)
	{
		return Identity{};
	}

	FORCEINLINE Identity operator+(Identity, Zero)
	{
		return Identity{};
	}

	template<typename Element>
	FORCEINLINE const Element& operator+(Zero, const Element& e)
	{
		return e;
	}

	template<typename Element>
	FORCEINLINE const Element& operator+(const Element& e, Zero)
	{
		return e;
	}

	FORCEINLINE Zero operator-(Zero, Zero)
	{
		return Zero{};
	}

	FORCEINLINE Zero operator-(Identity, Identity)
	{
		return Zero{};
	}

	FORCEINLINE Identity operator-(Identity, Zero)
	{
		return Identity{};
	}

	template<typename Element>
	FORCEINLINE Element operator-(Zero, const Element& e)
	{
		return -e;
	}

	template<typename Element>
	FORCEINLINE const Element& operator-(const Element& e, Zero)
	{
		return e;
	}

	FORCEINLINE Zero operator*(Zero, Zero)
	{
		return Zero{};
	}

	FORCEINLINE Zero operator*(Zero, Identity)
	{
		return Zero{};
	}

	FORCEINLINE Zero operator*(Identity, Zero)
	{
		return Zero{};
	}

	template<typename Element>
	FORCEINLINE Zero operator*(Zero, const Element& e)
	{
		return Zero{};
	}

	template<typename Element>
	FORCEINLINE Zero operator*(const Element& e, Zero)
	{
		return Zero{};
	}

#ifdef USE_OSTREAM

	template<typename CharT, typename Traits>
	std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, Zero)
	{
		return os << "Zero";
	}

	template<typename CharT, typename Traits>
	std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, Identity)
	{
		return os << "Identity";
	}

	template<typename CharT, typename Traits, std::size_t I>
	std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, Unit<I>)
	{
		return os << "Unit" << I;
	}

#endif // USE_OSTREAM








	// Nomember Function

	// --Implementation-- //

	namespace Math
	{
		FORCEINLINE Identity Inverse(Identity)
		{
			return Identity{};
		}

		FORCEINLINE Zero Mul(Zero, Zero)
		{
			return Zero{};
		}

		FORCEINLINE Zero Mul(Zero, Identity)
		{
			return Zero{};
		}

		FORCEINLINE Zero Mul(Identity, Zero)
		{
			return Zero{};
		}

		template<typename Element>
		FORCEINLINE Zero Mul(Zero, const Element& e)
		{
			return Zero{};
		}

		template<typename Element>
		FORCEINLINE Zero Mul(const Element& e, Zero)
		{
			return Zero{};
		}

		FORCEINLINE Identity Mul(Identity, Identity)
		{
			return Identity{};
		}

		template<typename Element>
		FORCEINLINE const Element& Mul(Identity, const Element& e)
		{
			return e;
		}

		template<typename Element>
		FORCEINLINE const Element& Mul(const Element& e, Identity)
		{
			return e;
		}

		template<typename Element>
		FORCEINLINE Element Mul(const Element& a, const Element& b, const Element& c)
		{
			return Mul(Mul(a, b), c);
		}


		FORCEINLINE Zero Dot(Zero, Zero)
		{
			return Zero{};
		}

		template<typename Vector>
		FORCEINLINE Zero Dot(Zero, const Vector& v)
		{
			return Zero{};
		}

		template<typename Vector>
		FORCEINLINE Zero Dot(const Vector& v, Zero)
		{
			return Zero{};
		}

		template<std::size_t I>
		FORCEINLINE Identity Dot(Unit<I>, Unit<I>)
		{
			return Identity{};
		}

		template<std::size_t I, std::size_t J>
		FORCEINLINE Zero Dot(Unit<I>, Unit<J>)
		{
			return Zero{};
		}

		template<typename Vector, std::size_t I>
		FORCEINLINE typename Vector::ScalarType Dot(const Vector& v, Unit<I>)
		{
			using RT = typename Vector::ScalarType;

			return v[I];
		}

		template<typename Vector, std::size_t I>
		FORCEINLINE typename Vector::ScalarType Dot(Unit<I>, const Vector& v)
		{
			using RT = typename Vector::ScalarType;

			return v[I];
		}
	}
}