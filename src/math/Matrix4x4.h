#pragma once

namespace Dash
{
	namespace Math
	{
		template<typename Scalar>
		class ScalarMatrix<Scalar, 4, 4>
		{
		public:
			using DataType = ScalarArray<Scalar, 4>;

			using ScalarType = Scalar;
			using SizeType = std::size_t;

			constexpr ScalarMatrix();
			constexpr explicit ScalarMatrix(Zero);
			constexpr explicit ScalarMatrix(Identity);
			constexpr explicit ScalarMatrix(Scalar s);
			template <typename Scalar2> constexpr explicit ScalarMatrix(const Scalar2* v);
			constexpr ScalarMatrix(Scalar a00, Scalar a01, Scalar a02,
				Scalar a10, Scalar a11, Scalar a12,
				Scalar a20, Scalar a21, Scalar a22);
			constexpr explicit ScalarMatrix(const ScalarArray<Scalar, 4>& r0,
				const ScalarArray<Scalar, 4>& r1,
				const ScalarArray<Scalar, 4>& r2);

			operator const ScalarArray<Scalar, 4>* () const;
			operator ScalarArray<Scalar, 4>* ();

			ScalarMatrix<Scalar, 4, 4>& operator=(Zero) noexcept;
			ScalarMatrix<Scalar, 4, 4>& operator+=(Zero) noexcept;
			ScalarMatrix<Scalar, 4, 4>& operator-=(Zero) noexcept;
			ScalarMatrix<Scalar, 4, 4>& operator*=(Zero) noexcept;

			ScalarMatrix<Scalar, 4, 4>& operator=(Identity) noexcept;

			ScalarMatrix<Scalar, 4, 4>& operator=(Scalar s) noexcept;
			ScalarMatrix<Scalar, 4, 4>& operator*=(Scalar s) noexcept;
			ScalarMatrix<Scalar, 4, 4>& operator/=(Scalar s) noexcept;

			ScalarMatrix<Scalar, 4, 4>& operator+=(const ScalarMatrix<Scalar, 4, 4>& a) noexcept;
			ScalarMatrix<Scalar, 4, 4>& operator-=(const ScalarMatrix<Scalar, 4, 4>& a) noexcept;

			void SetValue(Scalar a00, Scalar a01, Scalar a02,
				Scalar a10, Scalar a11, Scalar a12,
				Scalar a20, Scalar a21, Scalar a22) noexcept;

			template<typename Scalar2>
			void SetValue(const Scalar2* v) noexcept;

			void SetRows(const ScalarArray<Scalar, 4>& r0,
				const ScalarArray<Scalar, 4>& r1,
				const ScalarArray<Scalar, 4>& r2) noexcept;

			void SetColumns(const ScalarArray<Scalar, 4>& c0,
				const ScalarArray<Scalar, 4>& c1,
				const ScalarArray<Scalar, 4>& c2) noexcept;

		private:
			ScalarArray<Scalar, 4> mRows[3];
		};

#ifdef USE_OSTREAM

		template <typename CharT, typename Traits, typename Scalar>
		std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const ScalarMatrix<Scalar, 4, 4>& a);

#endif

#ifdef USE_ISTREAM

		template <typename CharT, typename Traits, typename Scalar>
		std::basic_istream<CharT, Traits>& operator>>(std::basic_istream<CharT, Traits>& is, ScalarMatrix<Scalar, 4, 4>& a);

#endif

		template <typename Scalar> bool operator==(const ScalarMatrix<Scalar, 4, 4>& a, const ScalarMatrix<Scalar, 4, 4>& b) noexcept;

		template <typename Scalar> ScalarMatrix<Scalar, 4, 4> operator-(const ScalarMatrix<Scalar, 4, 4>& a) noexcept;

		template <typename Scalar1, typename Scalar2>
		ScalarMatrix<typename Promote<Scalar1, Scalar2>::RT, 4, 4> operator+(const ScalarMatrix<Scalar1, 4, 4>& a, const ScalarMatrix<Scalar2, 4, 4>& b) noexcept;

		template <typename Scalar1, typename Scalar2>
		ScalarMatrix<typename Promote<Scalar1, Scalar2>::RT, 4, 4> operator-(const ScalarMatrix<Scalar1, 4, 4>& a, const ScalarMatrix<Scalar2, 4, 4>& b) noexcept;

#if USE_MATRIX_COMP_MULT
		template <typename Scalar1, typename Scalar2>
		ScalarMatrix<typename Promote<Scalar1, Scalar2>::RT> operator*(const ScalarMatrix<Scalar1, 4, 4>& a, const ScalarMatrix<Scalar2, 4, 4>& b);
#endif

		template <typename Scalar1, typename Scalar2>
		ScalarMatrix<typename Promote<Scalar1, Scalar2>::RT, 4, 4> operator*(const ScalarMatrix<Scalar1, 4, 4>& a, Scalar2 s) noexcept;

		template <typename Scalar1, typename Scalar2>
		ScalarMatrix<typename Promote<Scalar1, Scalar2>::RT, 4, 4> operator*(Scalar1 s, const ScalarMatrix<Scalar2, 4, 4>& a) noexcept;

		template <typename Scalar1, typename Scalar2>
		ScalarMatrix<typename Promote<Scalar1, Scalar2>::RT, 4, 4> operator/(const ScalarMatrix<Scalar1, 4, 4>& a, Scalar2 s) noexcept;

		template <typename Scalar1, typename Scalar2>
		ScalarArray<typename Promote<Scalar1, Scalar2>::RT, 4> Mul(const ScalarMatrix<Scalar1, 4, 4>& a, const ScalarArray<Scalar2, 4>& v) noexcept;

		template <typename Scalar1, typename Scalar2>
		ScalarArray<typename Promote<Scalar1, Scalar2>::RT, 4> Mul(const ScalarArray<Scalar1, 4>& v, const ScalarMatrix<Scalar2, 4, 4>& a) noexcept;

		template<typename Scalar1, typename Scalar2>
		ScalarMatrix<typename Promote<Scalar1, Scalar2>::RT, 4, 4> Mul(const ScalarMatrix<Scalar1, 4, 4>& a, const ScalarMatrix<Scalar2, 4, 4>& b) noexcept;

		template <typename Scalar>
		ScalarArray<Scalar, 4> Row(const ScalarMatrix<Scalar, 4, 4>& a, std::size_t i) noexcept;

		template <typename Scalar>
		ScalarArray<Scalar, 4> Column(const ScalarMatrix<Scalar, 4, 4>& a, std::size_t i) noexcept;
	}
}
