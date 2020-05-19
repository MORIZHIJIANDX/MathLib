#pragma once

#include "ScalarArray.h"

namespace Dash
{
	namespace Math
	{
		template<typename Scalar, std::size_t M, std::size_t N>
		class ScalarMatrix
		{
		public:
			constexpr ScalarMatrix();
			constexpr explicit ScalarMatrix(Zero);
			constexpr explicit ScalarMatrix(Identity);
			constexpr explicit ScalarMatrix(std::initializer_list<Scalar> list);
			template <typename Scalar2> constexpr explicit ScalarMatrix(const Scalar2* v);

			ScalarMatrix<Scalar, M, N>& operator=(Zero);
			ScalarMatrix<Scalar, M, N>& operator+=(Zero);
			ScalarMatrix<Scalar, M, N>& operator-=(Zero);
			ScalarMatrix<Scalar, M, N>& operator*=(Zero);

			ScalarMatrix<Scalar, M, N>& operator=(Identity);

			ScalarMatrix<Scalar, M, N>& operator=(Scalar s);
			ScalarMatrix<Scalar, M, N>& operator*=(Scalar s);
			ScalarMatrix<Scalar, M, N>& operator/=(Scalar s);

			ScalarMatrix<Scalar, M, N>& operator+=(const ScalarMatrix<Scalar, M, N>& a);
			ScalarMatrix<Scalar, M, N>& operator-=(const ScalarMatrix<Scalar, M, N>& a);

		private:
			Scalar mMat[M][N];
		};


#ifdef USE_OSTREAM

		template <typename CharT, typename Traits, typename Scalar, std::size_t M, std::size_t N>
		std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const ScalarMatrix<Scalar, M, N>& a);

#endif

#ifdef USE_ISTREAM

		template <typename CharT, typename Traits, typename Scalar, std::size_t M, std::size_t N>
		std::basic_istream<CharT, Traits>& operator>>(std::basic_istream<CharT, Traits>& is, ScalarMatrix<Scalar, M, N>& a);

#endif

		template <typename Scalar, std::size_t M, std::size_t N> bool operator==(const ScalarMatrix<Scalar, M , N>& a, const ScalarMatrix<Scalar, M , N>& b);

		template <typename Scalar, std::size_t M, std::size_t N> ScalarMatrix<Scalar, M , N> operator-(const ScalarMatrix<Scalar, M , N>& a);

		template <typename Scalar1, typename Scalar2, std::size_t M, std::size_t N>
		ScalarMatrix<typename Promote<Scalar1, Scalar2>::RT, M, N> operator+(const ScalarMatrix<Scalar1, M , N>& a, const ScalarMatrix<Scalar2, M , N>& b);

		template <typename Scalar1, typename Scalar2, std::size_t M, std::size_t N>
		ScalarMatrix<typename Promote<Scalar1, Scalar2>::RT, M, N> operator-(const ScalarMatrix<Scalar1, M , N>& a, const ScalarMatrix<Scalar2, M , N>& b);

#if USE_MATRIX_COMP_MULT
		template <typename Scalar1, typename Scalar2, std::size_t M, std::size_t N>
		ScalarMatrix<typename Promote<Scalar1, Scalar2>::RT, std::size_t M, std::size_t N> operator*(const ScalarMatrix<Scalar1, M , N>& a, const ScalarMatrix<Scalar2, M , N>& b);
#endif

		template <typename Scalar1, typename Scalar2, std::size_t M, std::size_t N>
		ScalarMatrix<typename Promote<Scalar1, Scalar2>::RT, M, N> operator*(const ScalarMatrix<Scalar1, M , N>& a, Scalar2 s);

		template <typename Scalar1, typename Scalar2, std::size_t M, std::size_t N>
		ScalarMatrix<typename Promote<Scalar1, Scalar2>::RT, M, N> operator*(Scalar1 s, const ScalarMatrix<Scalar2, M , N>& a);

		template <typename Scalar1, typename Scalar2, std::size_t M, std::size_t N>
		ScalarMatrix<typename Promote<Scalar1, Scalar2>::RT, M, N> operator/(const ScalarMatrix<Scalar1, M , N>& a, Scalar2 s);

		template <typename Scalar1, typename Scalar2, std::size_t M, std::size_t N>
		ScalarArray<typename Promote<Scalar1, Scalar2>::RT, M> mul(const ScalarMatrix<Scalar1, M , N>& a, const ScalarArray<Scalar2, N>& v);

		template <typename Scalar1, typename Scalar2, std::size_t M, std::size_t N>
		ScalarArray<typename Promote<Scalar1, Scalar2>::RT, N> mul(const ScalarArray<Scalar1, M>& v, const ScalarMatrix<Scalar2, M , N>& a);

		template <typename Scalar1, typename Scalar2, std::size_t M, std::size_t N>
		ScalarMatrix<typename Promote<Scalar1, Scalar2>::RT, M, N> mul(const ScalarMatrix<Scalar1, M , N>& a, const ScalarMatrix<Scalar2, M , N>& b);

		template <typename Scalar1, typename Scalar2, std::size_t M, std::size_t N>
		ScalarMatrix<typename Promote<Scalar1, Scalar2>::RT, M, N> transposeMul(const ScalarMatrix<Scalar1, M , N>& a, const ScalarMatrix<Scalar2, M , N>& b);

		template <typename Scalar1, typename Scalar2, std::size_t M, std::size_t N>
		ScalarMatrix<typename Promote<Scalar1, Scalar2>::RT, M, N> mulTranspose(const ScalarMatrix<Scalar1, M , N>& a, const ScalarMatrix<Scalar2, M , N>& b);

	}
}