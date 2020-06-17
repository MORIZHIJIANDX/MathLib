#pragma once

#include "ScalarArray.h"

namespace Dash
{

	template<typename Scalar, std::size_t M, std::size_t N>
	class ScalarMatrix
	{
	public:
		using DataType = ScalarArray<Scalar, N>;

		using ScalarType = Scalar;
		using SizeType = std::size_t;

		constexpr ScalarMatrix() noexcept;
		constexpr explicit ScalarMatrix(Zero) noexcept;
		constexpr explicit ScalarMatrix(Identity) noexcept;
		constexpr explicit ScalarMatrix(std::initializer_list<Scalar> list) noexcept;
		template <typename Scalar2> constexpr explicit ScalarMatrix(const Scalar2* v) noexcept;

		operator const ScalarArray<Scalar, N>* () const noexcept;
		operator ScalarArray<Scalar, N>* () noexcept;

		ScalarMatrix<Scalar, M, N>& operator=(Zero) noexcept;
		ScalarMatrix<Scalar, M, N>& operator+=(Zero) noexcept;
		ScalarMatrix<Scalar, M, N>& operator-=(Zero) noexcept;
		ScalarMatrix<Scalar, M, N>& operator*=(Zero) noexcept;

		ScalarMatrix<Scalar, M, N>& operator=(Identity) noexcept;

		ScalarMatrix<Scalar, M, N>& operator=(Scalar s) noexcept;
		ScalarMatrix<Scalar, M, N>& operator*=(Scalar s) noexcept;
		ScalarMatrix<Scalar, M, N>& operator/=(Scalar s) noexcept;

		ScalarMatrix<Scalar, M, N>& operator+=(const ScalarMatrix<Scalar, M, N>& a) noexcept;
		ScalarMatrix<Scalar, M, N>& operator-=(const ScalarMatrix<Scalar, M, N>& a) noexcept;

	private:
		ScalarArray<Scalar, N> mRows[M];
	};









	// Non-member Operators 

	// --Declaration-- //


#ifdef USE_OSTREAM

	template <typename CharT, typename Traits, typename Scalar, std::size_t M, std::size_t N>
	std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const ScalarMatrix<Scalar, M, N>& a);

#endif

#ifdef USE_ISTREAM

	template <typename CharT, typename Traits, typename Scalar, std::size_t M, std::size_t N>
	std::basic_istream<CharT, Traits>& operator>>(std::basic_istream<CharT, Traits>& is, ScalarMatrix<Scalar, M, N>& a);

#endif

	template <typename Scalar, std::size_t M, std::size_t N> bool operator==(const ScalarMatrix<Scalar, M, N>& a, const ScalarMatrix<Scalar, M, N>& b) noexcept;

	template <typename Scalar, std::size_t M, std::size_t N> ScalarMatrix<Scalar, M, N> operator-(const ScalarMatrix<Scalar, M, N>& a) noexcept;

	template <typename Scalar1, typename Scalar2, std::size_t M, std::size_t N>
	ScalarMatrix<typename Promote<Scalar1, Scalar2>::RT, M, N> operator+(const ScalarMatrix<Scalar1, M, N>& a, const ScalarMatrix<Scalar2, M, N>& b) noexcept;

	template <typename Scalar1, typename Scalar2, std::size_t M, std::size_t N>
	ScalarMatrix<typename Promote<Scalar1, Scalar2>::RT, M, N> operator-(const ScalarMatrix<Scalar1, M, N>& a, const ScalarMatrix<Scalar2, M, N>& b) noexcept;

	template <typename Scalar1, typename Scalar2, std::size_t M, std::size_t N>
	ScalarMatrix<typename Promote<Scalar1, Scalar2>::RT, M, N> operator*(const ScalarMatrix<Scalar1, M, N>& a, const ScalarMatrix<Scalar2, M, N>& b);

	template <typename Scalar1, typename Scalar2, std::size_t M, std::size_t N>
	ScalarMatrix<typename Promote<Scalar1, Scalar2>::RT, M, N> operator*(const ScalarMatrix<Scalar1, M, N>& a, Scalar2 s) noexcept;

	template <typename Scalar1, typename Scalar2, std::size_t M, std::size_t N>
	ScalarMatrix<typename Promote<Scalar1, Scalar2>::RT, M, N> operator*(Scalar1 s, const ScalarMatrix<Scalar2, M, N>& a) noexcept;

	template <typename Scalar1, typename Scalar2, std::size_t M, std::size_t N>
	ScalarMatrix<typename Promote<Scalar1, Scalar2>::RT, M, N> operator/(const ScalarMatrix<Scalar1, M, N>& a, Scalar2 s) noexcept;









	// Non-member Function

	// --Declaration-- //

	namespace Math
	{
		template <typename Scalar1, typename Scalar2, std::size_t M, std::size_t N>
		ScalarArray<typename Promote<Scalar1, Scalar2>::RT, M> Mul(const ScalarMatrix<Scalar1, M, N>& a, const ScalarArray<Scalar2, N>& v) noexcept;

		template <typename Scalar1, typename Scalar2, std::size_t M, std::size_t N>
		ScalarArray<typename Promote<Scalar1, Scalar2>::RT, N> Mul(const ScalarArray<Scalar1, M>& v, const ScalarMatrix<Scalar2, M, N>& a) noexcept;

		template<typename Scalar1, typename Scalar2, std::size_t M, std::size_t N, std::size_t P>
		ScalarMatrix<typename Promote<Scalar1, Scalar2>::RT, M, P> Mul(const ScalarMatrix<Scalar1, M, N>& a, const ScalarMatrix<Scalar2, N, P>& b) noexcept;

		template <typename Scalar, std::size_t M, std::size_t N>
		ScalarArray<Scalar, N> Row(const ScalarMatrix<Scalar, M, N>& a, std::size_t i);

		template <typename Scalar, std::size_t M, std::size_t N>
		ScalarArray<Scalar, M> Column(const ScalarMatrix<Scalar, M, N>& a, std::size_t i);

		template <typename Scalar, std::size_t M, std::size_t N>
		ScalarMatrix<Scalar, N, M> Transpose(const ScalarMatrix<Scalar, M, N>& a);
	}






	// Member Function

	// --Implementation-- //

	template<typename Scalar, std::size_t M, std::size_t N>
	FORCEINLINE constexpr ScalarMatrix<Scalar, M, N>::ScalarMatrix() noexcept
	{
	}

	template<typename Scalar, std::size_t M, std::size_t N>
	FORCEINLINE constexpr ScalarMatrix<Scalar, M, N>::ScalarMatrix(Zero) noexcept
	{
		for (std::size_t i = 0; i < M; i++)
		{
			mRows[i] = Zero{};
		}
	}

	template<typename Scalar, std::size_t M, std::size_t N>
	FORCEINLINE constexpr ScalarMatrix<Scalar, M, N>::ScalarMatrix(Identity) noexcept
	{
		for (std::size_t i = 0; i < M; i++)
		{
			for (std::size_t j = 0; j < N; j++)
			{
				if (i == j)
				{
					mRows[i][j] = Scalar{ 1 };
				}
				else
				{
					mRows[i][j] = Scalar{ };
				}
			}
		}
	}

	template<typename Scalar, std::size_t M, std::size_t N>
	FORCEINLINE constexpr ScalarMatrix<Scalar, M, N>::ScalarMatrix(std::initializer_list<Scalar> list) noexcept
	{
		ASSERT((M * N) == list.size());

		auto listIter = list.begin();

		for (std::size_t i = 0; i < M; i++)
		{
			for (std::size_t j = 0; j < N; j++)
			{
				mRows[M][N] = *listIter;
				++listIter;
			}
		}
	}

	template<typename Scalar, std::size_t M, std::size_t N>
	template<typename Scalar2>
	FORCEINLINE constexpr ScalarMatrix<Scalar, M, N>::ScalarMatrix(const Scalar2* v) noexcept
	{
		ASSERT(v != nullptr);

		for (std::size_t i = 0; i < M; i++)
		{
			for (std::size_t j = 0; j < N; j++)
			{
				mRows[M][N] = v[i * M + j];
			}
		}
	}

	template<typename Scalar, std::size_t M, std::size_t N>
	FORCEINLINE ScalarMatrix<Scalar, M, N>::operator const ScalarArray<Scalar, N>* () const noexcept
	{
		return mRows;
	}

	template<typename Scalar, std::size_t M, std::size_t N>
	FORCEINLINE ScalarMatrix<Scalar, M, N>::operator ScalarArray<Scalar, N>* () noexcept
	{
		return mRows;
	}

	template<typename Scalar, std::size_t M, std::size_t N>
	FORCEINLINE ScalarMatrix<Scalar, M, N>& ScalarMatrix<Scalar, M, N>::operator=(Zero zero) noexcept
	{
		for (std::size_t i = 0; i < M; i++)
		{
			mRows[i] = zero;
		}

		return *this;
	}

	template<typename Scalar, std::size_t M, std::size_t N>
	FORCEINLINE ScalarMatrix<Scalar, M, N>& ScalarMatrix<Scalar, M, N>::operator+=(Zero) noexcept
	{
		return *this;
	}

	template<typename Scalar, std::size_t M, std::size_t N>
	FORCEINLINE ScalarMatrix<Scalar, M, N>& ScalarMatrix<Scalar, M, N>::operator-=(Zero) noexcept
	{
		return *this;
	}

	template<typename Scalar, std::size_t M, std::size_t N>
	FORCEINLINE ScalarMatrix<Scalar, M, N>& ScalarMatrix<Scalar, M, N>::operator*=(Zero zero) noexcept
	{
		for (std::size_t i = 0; i < M; i++)
		{
			mRows[i] = zero;
		}

		return *this;
	}

	template<typename Scalar, std::size_t M, std::size_t N>
	FORCEINLINE ScalarMatrix<Scalar, M, N>& ScalarMatrix<Scalar, M, N>::operator=(Identity) noexcept
	{
		for (std::size_t i = 0; i < M; i++)
		{
			for (std::size_t j = 0; j < N; j++)
			{
				if (i == j)
				{
					mRows[i][j] = Scalar{ 1 };
				}
				else
				{
					mRows[i][j] = Scalar{ };
				}
			}
		}

		return *this;
	}

	template<typename Scalar, std::size_t M, std::size_t N>
	FORCEINLINE ScalarMatrix<Scalar, M, N>& ScalarMatrix<Scalar, M, N>::operator=(Scalar s) noexcept
	{
		for (std::size_t i = 0; i < M; i++)
		{
			for (std::size_t j = 0; j < N; j++)
			{
				if (i == j)
				{
					mRows[i][j] = s;
				}
				else
				{
					mRows[i][j] = Scalar{ };
				}
			}
		}

		return *this;
	}

	template<typename Scalar, std::size_t M, std::size_t N>
	FORCEINLINE ScalarMatrix<Scalar, M, N>& ScalarMatrix<Scalar, M, N>::operator*=(Scalar s) noexcept
	{
		for (std::size_t i = 0; i < M; i++)
		{
			mRows[i] *= s;
		}

		return *this;
	}

	template<typename Scalar, std::size_t M, std::size_t N>
	FORCEINLINE ScalarMatrix<Scalar, M, N>& ScalarMatrix<Scalar, M, N>::operator/=(Scalar s) noexcept
	{
		for (std::size_t i = 0; i < M; i++)
		{
			mRows[i] /= s;
		}

		return *this;
	}

	template<typename Scalar, std::size_t M, std::size_t N>
	FORCEINLINE ScalarMatrix<Scalar, M, N>& ScalarMatrix<Scalar, M, N>::operator+=(const ScalarMatrix<Scalar, M, N>& a) noexcept
	{
		for (std::size_t i = 0; i < M; i++)
		{
			mRows[i] += a[i];
		}

		return *this;
	}

	template<typename Scalar, std::size_t M, std::size_t N>
	FORCEINLINE ScalarMatrix<Scalar, M, N>& ScalarMatrix<Scalar, M, N>::operator-=(const ScalarMatrix<Scalar, M, N>& a) noexcept
	{
		for (std::size_t i = 0; i < M; i++)
		{
			mRows[i] -= a[i];
		}

		return *this;
	}









	// Non-member Operators 

	// --Implementation-- //

#ifdef USE_OSTREAM
	template<typename CharT, typename Traits, typename Scalar, std::size_t M, std::size_t N>
	FORCEINLINE std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const ScalarMatrix<Scalar, M, N>& a)
	{
		for (std::size_t i = 0; i < M; i++)
		{
			os << Math::Row(a, i);
			if (i != M - 1)
			{
				os << ' ';
			}
		}
		return os;
	}
#endif

#ifdef USE_ISTREAM
	template<typename CharT, typename Traits, typename Scalar, std::size_t M, std::size_t N>
	FORCEINLINE std::basic_istream<CharT, Traits>& operator>>(std::basic_istream<CharT, Traits>& is, ScalarMatrix<Scalar, M, N>& a)
	{
		for (std::size_t i = 0; i < M; i++)
		{
			is >> a[i];
		}

		return is;
	}
#endif

	template<typename Scalar, std::size_t M, std::size_t N>
	FORCEINLINE bool operator==(const ScalarMatrix<Scalar, M, N>& a, const ScalarMatrix<Scalar, M, N>& b) noexcept
	{
		for (std::size_t i = 0; i < M; i++)
		{
			for (std::size_t j = 0; j < N; j++)
			{
				if (a[i][j] != b[i][j])
				{
					return false;
				}
			}
		}

		return true;
	}

	template<typename Scalar, std::size_t M, std::size_t N>
	FORCEINLINE ScalarMatrix<Scalar, M, N> operator-(const ScalarMatrix<Scalar, M, N>& a) noexcept
	{
		ScalarMatrix<Scalar, M, N> result;

		for (std::size_t i = 0; i < M; i++)
		{
			result[i] = -a[i];
		}

		return result;
	}

	template<typename Scalar1, typename Scalar2, std::size_t M, std::size_t N>
	FORCEINLINE ScalarMatrix<typename Promote<Scalar1, Scalar2>::RT, M, N> operator+(const ScalarMatrix<Scalar1, M, N>& a, const ScalarMatrix<Scalar2, M, N>& b) noexcept
	{
		using RT = typename Promote<Scalar1, Scalar2>::RT;
		ScalarMatrix<RT, M, N> result;

		for (std::size_t i = 0; i < M; i++)
		{
			result[i] = a[i] + b[i];
		}

		return result;
	}

	template<typename Scalar1, typename Scalar2, std::size_t M, std::size_t N>
	FORCEINLINE ScalarMatrix<typename Promote<Scalar1, Scalar2>::RT, M, N> operator-(const ScalarMatrix<Scalar1, M, N>& a, const ScalarMatrix<Scalar2, M, N>& b) noexcept
	{
		using RT = typename Promote<Scalar1, Scalar2>::RT;
		ScalarMatrix<RT, M, N> result;

		for (std::size_t i = 0; i < M; i++)
		{
			result[i] = a[i] - b[i];
		}

		return result;
	}

	template<typename Scalar1, typename Scalar2, std::size_t M, std::size_t N>
	FORCEINLINE ScalarMatrix<typename Promote<Scalar1, Scalar2>::RT, M, N> operator*(const ScalarMatrix<Scalar1, M, N>& a, const ScalarMatrix<Scalar2, M, N>& b)
	{
		//using RT = typename Promote<Scalar1, Scalar2>::RT;
		//ScalarMatrix<RT, M, N> result;

		//for (std::size_t i = 0; i < M; i++)
		//{
		//	result[i] = a[i] * b[i];
		//}

		//return result;

		return Math::Mul(a, b);
	}

	template<typename Scalar1, typename Scalar2, std::size_t M, std::size_t N>
	FORCEINLINE ScalarMatrix<typename Promote<Scalar1, Scalar2>::RT, M, N> operator*(const ScalarMatrix<Scalar1, M, N>& a, Scalar2 s) noexcept
	{
		using RT = typename Promote<Scalar1, Scalar2>::RT;
		ScalarMatrix<RT, M, N> result;

		for (std::size_t i = 0; i < M; i++)
		{
			result[i] = a[i] * s;
		}

		return result;
	}

	template<typename Scalar1, typename Scalar2, std::size_t M, std::size_t N>
	FORCEINLINE ScalarMatrix<typename Promote<Scalar1, Scalar2>::RT, M, N> operator*(Scalar1 s, const ScalarMatrix<Scalar2, M, N>& a) noexcept
	{
		return a * s;
	}

	template<typename Scalar1, typename Scalar2, std::size_t M, std::size_t N>
	FORCEINLINE ScalarMatrix<typename Promote<Scalar1, Scalar2>::RT, M, N> operator/(const ScalarMatrix<Scalar1, M, N>& a, Scalar2 s) noexcept
	{
		using RT = typename Promote<Scalar1, Scalar2>::RT;
		ScalarMatrix<RT, M, N> result;

		for (std::size_t i = 0; i < M; i++)
		{
			result[i] = a[i] / s;
		}

		return result;
	}








	// Non-member Function

	// --Implementation-- //

	namespace Math
	{

		template<typename Scalar1, typename Scalar2, std::size_t M, std::size_t N>
		FORCEINLINE ScalarArray<typename Promote<Scalar1, Scalar2>::RT, M> Mul(const ScalarMatrix<Scalar1, M, N>& a, const ScalarArray<Scalar2, N>& v) noexcept
		{
			using RT = typename Promote<Scalar1, Scalar2>::RT;
			ScalarArray<RT, M> result;

			for (std::size_t i = 0; i < M; i++)
			{
				result[i] = a[i] * v;
			}

			return result;
		}

		template<typename Scalar1, typename Scalar2, std::size_t M, std::size_t N>
		FORCEINLINE ScalarArray<typename Promote<Scalar1, Scalar2>::RT, N> Mul(const ScalarArray<Scalar1, M>& v, const ScalarMatrix<Scalar2, M, N>& a) noexcept
		{
			using RT = typename Promote<Scalar1, Scalar2>::RT;
			ScalarArray<RT, N> result;

			//for (std::size_t i = 0; i < M; i++)
			//{
			//	result[i] = v * Column(a, i);
			//}

			for (std::size_t i = 0; i < M; i++)
			{
				result += v[i] * a[i];
			}

			return result;
		}

		template<typename Scalar1, typename Scalar2, std::size_t M, std::size_t N, std::size_t P>
		FORCEINLINE ScalarMatrix<typename Promote<Scalar1, Scalar2>::RT, M, P> Mul(const ScalarMatrix<Scalar1, M, N>& a, const ScalarMatrix<Scalar2, N, P>& b) noexcept
		{
			using RT = typename Promote<Scalar1, Scalar2>::RT;
			ScalarMatrix<RT, M, P> result;

			for (std::size_t i = 0; i < M; i++)
			{
				for (std::size_t j = 0; j < P; j++)
				{
					result[i][j] = Dot(a[i], Column(b, j));
				}
			}

			return result;
		}

		template<typename Scalar, std::size_t M, std::size_t N>
		FORCEINLINE ScalarArray<Scalar, N> Row(const ScalarMatrix<Scalar, M, N>& a, std::size_t i)
		{
			return a[i];
		}

		template<typename Scalar, std::size_t M, std::size_t N>
		FORCEINLINE ScalarArray<Scalar, M> Column(const ScalarMatrix<Scalar, M, N>& a, std::size_t i)
		{
			ScalarArray<Scalar, M> result;

			for (std::size_t j = 0; j < M; j++)
			{
				for (std::size_t k = 0; k < N; k++)
				{
					if (k == i)
					{
						result[j] = a[j][k];
					}
				}
			}

			return result;
		}

		template<typename Scalar, std::size_t M, std::size_t N>
		FORCEINLINE ScalarMatrix<Scalar, N, M> Transpose(const ScalarMatrix<Scalar, M, N>& a)
		{
			ScalarMatrix<Scalar, N, M> result;

			for (std::size_t i = 0; i < M; i++)
			{
				for (std::size_t j = 0; j < N; j++)
				{
					result[i][j] = a[j][i];
				}
			}

			return result;
		}
	}
}

#include "Matrix3x3.h"
#include "Matrix4x4.h"