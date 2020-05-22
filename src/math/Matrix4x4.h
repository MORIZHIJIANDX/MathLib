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
			constexpr ScalarMatrix(Scalar a00, Scalar a01, Scalar a02, Scalar a03,
				Scalar a10, Scalar a11, Scalar a12, Scalar a13,
				Scalar a20, Scalar a21, Scalar a22, Scalar a23,
				Scalar a30, Scalar a31, Scalar a32, Scalar a33);
			constexpr explicit ScalarMatrix(const ScalarArray<Scalar, 4>& r0,
				const ScalarArray<Scalar, 4>& r1,
				const ScalarArray<Scalar, 4>& r2,
				const ScalarArray<Scalar, 4>& r3);

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

			void SetValue(Scalar a00, Scalar a01, Scalar a02, Scalar a03, 
				Scalar a10, Scalar a11, Scalar a12, Scalar a13,
				Scalar a20, Scalar a21, Scalar a22, Scalar a23, 
				Scalar a30, Scalar a31, Scalar a32, Scalar a33) noexcept;

			template<typename Scalar2>
			void SetValue(const Scalar2* v) noexcept;

			void SetRows(const ScalarArray<Scalar, 4>& r0,
				const ScalarArray<Scalar, 4>& r1,
				const ScalarArray<Scalar, 4>& r2,
				const ScalarArray<Scalar, 4>& r3) noexcept;

			void SetRow(int i, const ScalarArray<Scalar, 4>& v) noexcept;
			void SetRow(int i, const ScalarArray<Scalar, 3>& v) noexcept;

			void SetColumns(const ScalarArray<Scalar, 4>& c0,
				const ScalarArray<Scalar, 4>& c1,
				const ScalarArray<Scalar, 4>& c2,
				const ScalarArray<Scalar, 4>& c3) noexcept;

			void SetColumn(int j, const ScalarArray<Scalar, 4>& v) noexcept;

			void SetBasis(const ScalarMatrix<Scalar, 3, 3>& basis) noexcept;

			void SetOrigin(const ScalarArray<Scalar, 3>& origin) noexcept;

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

		template <typename Scalar>
		ScalarMatrix<Scalar, 4, 4> Transpose(const ScalarMatrix<Scalar, 4, 4>& a) noexcept;

		template <typename Scalar>
		ScalarMatrix<Scalar, 4, 4> Determinate(const ScalarMatrix<Scalar, 4, 4>& a) noexcept;

		template <typename Scalar>
		ScalarMatrix<Scalar, 4, 4> Inverse(const ScalarMatrix<Scalar, 4, 4>& a) noexcept;

		template <typename Scalar>
		ScalarArray<Scalar, 3> Origin(const ScalarMatrix<Scalar, 4, 4>& a) noexcept;

		template <typename Scalar>
		ScalarMatrix<Scalar, 3, 3> Basis(const ScalarMatrix<Scalar, 4, 4>& a) noexcept;

		template <typename Scalar> Scalar TDot(const ScalarMatrix<Scalar, 4, 4>& a, int i, const ScalarArray<Scalar, 4>& v) noexcept;

		template <int I, int J, typename Scalar> Scalar Cofactor(const ScalarMatrix<Scalar, 4, 4>& a) noexcept;

		template <typename Scalar>
		Scalar Trace(const ScalarMatrix<Scalar, 4, 4>& a) noexcept;



		//Member Function 
		template<typename Scalar>
		FORCEINLINE constexpr ScalarMatrix<Scalar, 4, 4>::ScalarMatrix()
		{
		}

		template<typename Scalar>
		FORCEINLINE constexpr ScalarMatrix<Scalar, 4, 4>::ScalarMatrix(Zero)
		{
			*this = Zero{};
		}

		template<typename Scalar>
		FORCEINLINE constexpr ScalarMatrix<Scalar, 4, 4>::ScalarMatrix(Identity)
		{
			*this = Identity{};
		}

		template<typename Scalar>
		FORCEINLINE constexpr ScalarMatrix<Scalar, 4, 4>::ScalarMatrix(Scalar s)
		{
			*this = s;
		}

		template<typename Scalar>
		template <typename Scalar2>
		FORCEINLINE constexpr ScalarMatrix<Scalar, 4, 4>::ScalarMatrix(const Scalar2* v)
		{
			SetValue(v);
		}

		template<typename Scalar>
		FORCEINLINE constexpr ScalarMatrix<Scalar, 4, 4>::ScalarMatrix(Scalar a00, Scalar a01, Scalar a02, Scalar a03,
			Scalar a10, Scalar a11, Scalar a12, Scalar a13,
			Scalar a20, Scalar a21, Scalar a22, Scalar a23,
			Scalar a30, Scalar a31, Scalar a32, Scalar a33)
		{
			SetValue(a00, a01, a02, a03,
				a10, a11, a12, a13,
				a20, a21, a22, a23,
				a30, a31, a32, a33);
		}

		template<typename Scalar>
		FORCEINLINE constexpr ScalarMatrix<Scalar, 4, 4>::ScalarMatrix(const ScalarArray<Scalar, 4>& r0,
			const ScalarArray<Scalar, 4>& r1,
			const ScalarArray<Scalar, 4>& r2,
			const ScalarArray<Scalar, 4>& r3)
		{
			mRows[0] = r0;
			mRows[1] = r1;
			mRows[2] = r2;
			mRows[3] = r3;
		}

		template<typename Scalar>
		FORCEINLINE ScalarMatrix<Scalar, 4, 4>::operator const ScalarArray<Scalar, 4>* () const
		{
			return mRows;
		}

		template<typename Scalar>
		FORCEINLINE ScalarMatrix<Scalar, 4, 4>::operator ScalarArray<Scalar, 4>* ()
		{
			return mRows;
		}

		template<typename Scalar>
		FORCEINLINE ScalarMatrix<Scalar, 4, 4>& ScalarMatrix<Scalar, 4, 4>::operator=(Zero zero) noexcept
		{
			mRows[0] = zero;
			mRows[1] = zero;
			mRows[2] = zero;
			mRows[3] = zero;

			return *this;
		}

		template<typename Scalar>
		FORCEINLINE ScalarMatrix<Scalar, 4, 4>& ScalarMatrix<Scalar, 4, 4>::operator+=(Zero) noexcept
		{
			return *this;
		}

		template<typename Scalar>
		FORCEINLINE ScalarMatrix<Scalar, 4, 4>& ScalarMatrix<Scalar, 4, 4>::operator-=(Zero) noexcept
		{
			return *this;
		}

		template<typename Scalar>
		FORCEINLINE ScalarMatrix<Scalar, 4, 4>& ScalarMatrix<Scalar, 4, 4>::operator*=(Zero zero) noexcept
		{
			mRows[0] = zero;
			mRows[1] = zero;
			mRows[2] = zero;
			mRows[3] = zero;

			return *this;
		}

		template<typename Scalar>
		FORCEINLINE ScalarMatrix<Scalar, 4, 4>& ScalarMatrix<Scalar, 4, 4>::operator=(Identity) noexcept
		{
			mRows[0] = Unit<0>{};
			mRows[1] = Unit<1>{};
			mRows[2] = Unit<2>{};
			mRows[3] = Unit<3>{};

			return *this;
		}

		template<typename Scalar>
		FORCEINLINE ScalarMatrix<Scalar, 4, 4>& ScalarMatrix<Scalar, 4, 4>::operator=(Scalar s) noexcept
		{
			SetValue(s, Scalar{}, Scalar{}, Scalar{},
				Scalar{}, s, Scalar{}, Scalar{},
				Scalar{}, Scalar{}, s, Scalar{},
				Scalar{}, Scalar{}, Scalar{}, s);

			return *this;
		}

		template<typename Scalar>
		FORCEINLINE ScalarMatrix<Scalar, 4, 4>& ScalarMatrix<Scalar, 4, 4>::operator*=(Scalar s) noexcept
		{
			mRows[0] *= s;
			mRows[1] *= s;
			mRows[2] *= s;
			mRows[3] *= s;

			return *this;
		}

		template<typename Scalar>
		FORCEINLINE ScalarMatrix<Scalar, 4, 4>& ScalarMatrix<Scalar, 4, 4>::operator/=(Scalar s) noexcept
		{
			ASSERT(!IsZero(s));
			return *this *= (Scalar{ 1 } / s);
		}

		template<typename Scalar>
		FORCEINLINE ScalarMatrix<Scalar, 4, 4>& ScalarMatrix<Scalar, 4, 4>::operator+=(const ScalarMatrix<Scalar, 4, 4>& a) noexcept
		{
			mRows[0] += a[0];
			mRows[1] += a[1];
			mRows[2] += a[2];
			mRows[3] += a[3];

			return *this;
		}

		template<typename Scalar>
		FORCEINLINE ScalarMatrix<Scalar, 4, 4>& ScalarMatrix<Scalar, 4, 4>::operator-=(const ScalarMatrix<Scalar, 4, 4>& a) noexcept
		{
			mRows[0] -= a[0];
			mRows[1] -= a[1];
			mRows[2] -= a[2];
			mRows[3] -= a[3];

			return *this;
		}

		template<typename Scalar>
		FORCEINLINE void ScalarMatrix<Scalar, 4, 4>::SetValue(Scalar a00, Scalar a01, Scalar a02, Scalar a03, 
			Scalar a10, Scalar a11, Scalar a12, Scalar a13, 
			Scalar a20, Scalar a21, Scalar a22, Scalar a23, 
			Scalar a30, Scalar a31, Scalar a32, Scalar a33) noexcept
		{
			mRows[0] = ScalarArray<Scalar, 4>{ a00, a01, a02, a03 };
			mRows[1] = ScalarArray<Scalar, 4>{ a10, a11, a12, a13 };
			mRows[2] = ScalarArray<Scalar, 4>{ a20, a21, a22, a23 };
			mRows[3] = ScalarArray<Scalar, 4>{ a20, a21, a22, a33 };

			return *this;
		}

		template<typename Scalar>
		template<typename Scalar2>
		FORCEINLINE void ScalarMatrix<Scalar, 4, 4>::SetValue(const Scalar2* v) noexcept
		{
			ASSERT(v != nullptr);

			mRows[0] = ScalarArray<Scalar, 4>{ Scalar{v[0]}, Scalar{v[1]}, Scalar{v[2]}, Scalar{v[3]} };
			mRows[1] = ScalarArray<Scalar, 4>{ Scalar{v[4]}, Scalar{v[5]}, Scalar{v[6]}, Scalar{v[7]} };
			mRows[2] = ScalarArray<Scalar, 4>{ Scalar{v[8]}, Scalar{v[9]}, Scalar{v[10]}, Scalar{v[11]} };
			mRows[3] = ScalarArray<Scalar, 4>{ Scalar{v[12]}, Scalar{v[13]}, Scalar{v[14]}, Scalar{v[15]} };

			return *this;
		}

		template<typename Scalar>
		FORCEINLINE void ScalarMatrix<Scalar, 4, 4>::SetRows(const ScalarArray<Scalar, 4>& r0, const ScalarArray<Scalar, 4>& r1, const ScalarArray<Scalar, 4>& r2, const ScalarArray<Scalar, 4>& r3) noexcept
		{
			mRows[0] = r0;
			mRows[1] = r1;
			mRows[2] = r2;
			mRows[3] = r3;
		}

		template<typename Scalar>
		FORCEINLINE void ScalarMatrix<Scalar, 4, 4>::SetRow(int i, const ScalarArray<Scalar, 4>& v) noexcept
		{
			mRows[i] = v;
		}

		template<typename Scalar>
		FORCEINLINE void ScalarMatrix<Scalar, 4, 4>::SetRow(int i, const ScalarArray<Scalar, 3>& v) noexcept
		{
			mRows[i] = ScalarArray<Scalar, 4>{ v.x, v.y, v.z, mRows[i].w };
		}

		template<typename Scalar>
		FORCEINLINE void ScalarMatrix<Scalar, 4, 4>::SetColumns(const ScalarArray<Scalar, 4>& c0, const ScalarArray<Scalar, 4>& c1, const ScalarArray<Scalar, 4>& c2, const ScalarArray<Scalar, 4>& c3) noexcept
		{
			mRows[0] = ScalarArray<Scalar, 4>{ c0.x, c1.x, c2.x, c3.x };
			mRows[1] = ScalarArray<Scalar, 4>{ c0.y, c1.y, c2.y, c3.y };
			mRows[2] = ScalarArray<Scalar, 4>{ c0.z, c1.z, c2.z, c3.z };
			mRows[3] = ScalarArray<Scalar, 4>{ c0.w, c1.w, c2.w, c3.w };
		}

		template<typename Scalar>
		FORCEINLINE void ScalarMatrix<Scalar, 4, 4>::SetColumn(int j, const ScalarArray<Scalar, 4>& v) noexcept
		{
			mRows[0][j] = v.x;
			mRows[1][j] = v.y;
			mRows[2][j] = v.z;
			mRows[3][j] = v.w;
		}

		template<typename Scalar>
		FORCEINLINE void ScalarMatrix<Scalar, 4, 4>::SetBasis(const ScalarMatrix<Scalar, 3, 3>& basis) noexcept
		{
			SetRows(0, basis[0]);
			SetRows(1, basis[1]);
			SetRows(2, basis[2]);
		}

		template<typename Scalar>
		FORCEINLINE void ScalarMatrix<Scalar, 4, 4>::SetOrigin(const ScalarArray<Scalar, 3>& origin) noexcept
		{
			SetRows(3, origin);
		}



		//Nomember Function
#ifdef USE_OSTREAM

		template <typename CharT, typename Traits, typename Scalar>
		std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const ScalarMatrix<Scalar, 4, 4>& a)
		{
			return os << a[0] << ' ' << a[1] << ' ' << a[2] << ' ' << a[3];
		}

#endif

#ifdef USE_ISTREAM

		template <typename CharT, typename Traits, typename Scalar>
		std::basic_istream<CharT, Traits>& operator>>(std::basic_istream<CharT, Traits>& is, ScalarMatrix<Scalar, 4, 4>& a)
		{
			return is >> a[0] >> a[1] >> a[2] >> a[3];
		}

#endif

		template <typename Scalar> 
		FORCEINLINE bool operator==(const ScalarMatrix<Scalar, 4, 4>& a, const ScalarMatrix<Scalar, 4, 4>& b) noexcept
		{
			return a[0] == b[0] && a[1] == b[1] && a[2] == b[2] && a[3] == b[3];
		}

		template <typename Scalar> 
		FORCEINLINE ScalarMatrix<Scalar, 4, 4> operator-(const ScalarMatrix<Scalar, 4, 4>& a) noexcept
		{
			return ScalarMatrix<Scalar, 4, 4>{ -a[0][0], -a[0][1], -a[0][2], -a[0][3], 
				-a[1][0], -a[1][1], -a[1][2], -a[1][3],
				-a[2][0], -a[2][1], -a[2][2], -a[2][3],
				-a[3][0], -a[3][1], -a[3][2], -a[3][3] };
		}

		template <typename Scalar1, typename Scalar2>
		FORCEINLINE ScalarMatrix<typename Promote<Scalar1, Scalar2>::RT, 4, 4> operator+(const ScalarMatrix<Scalar1, 4, 4>& a, const ScalarMatrix<Scalar2, 4, 4>& b) noexcept
		{
			using RT = typename Promote<Scalar1, Scalar2>::RT;
			return ScalarMatrix<RT, 4, 4>{ a[0][0] + b[0][0], a[0][1] + b[0][1], a[0][2] + b[0][2], a[0][3] + b[0][3],
				a[1][0] + b[1][0], a[1][1] + b[1][1], a[1][2] + b[1][2], a[1][3] + b[1][3],
				a[2][0] + b[2][0], a[2][1] + b[2][1], a[2][2] + b[2][2], a[2][3] + b[2][3],
				a[3][0] + b[3][0], a[3][1] + b[3][1], a[3][2] + b[3][2], a[3][3] + b[3][3] };
		}

		template <typename Scalar1, typename Scalar2>
		FORCEINLINE ScalarMatrix<typename Promote<Scalar1, Scalar2>::RT, 4, 4> operator-(const ScalarMatrix<Scalar1, 4, 4>& a, const ScalarMatrix<Scalar2, 4, 4>& b) noexcept
		{
			using RT = typename Promote<Scalar1, Scalar2>::RT;
			return ScalarMatrix<RT, 4, 4>{ a[0][0] - b[0][0], a[0][1] - b[0][1], a[0][2] - b[0][2], a[0][3] - b[0][3],
				a[1][0] - b[1][0], a[1][1] - b[1][1], a[1][2] - b[1][2], a[1][3] - b[1][3],
				a[2][0] - b[2][0], a[2][1] - b[2][1], a[2][2] - b[2][2], a[2][3] - b[2][3],
				a[3][0] - b[3][0], a[3][1] - b[3][1], a[3][2] - b[3][2], a[3][3] - b[3][3] };
		}

#if USE_MATRIX_COMP_MULT
		template <typename Scalar1, typename Scalar2>
		FORCEINLINE ScalarMatrix<typename Promote<Scalar1, Scalar2>::RT> operator*(const ScalarMatrix<Scalar1, 4, 4>& a, const ScalarMatrix<Scalar2, 4, 4>& b)
		{
			using RT = typename Promote<Scalar1, Scalar2>::RT;
			return ScalarMatrix<RT, 4, 4>{ a[0][0] * b[0][0], a[0][1] * b[0][1], a[0][2] * b[0][2], a[0][3] * b[0][3],
				a[1][0] * b[1][0], a[1][1] * b[1][1], a[1][2] * b[1][2], a[1][3] * b[1][3],
				a[2][0] * b[2][0], a[2][1] * b[2][1], a[2][2] * b[2][2], a[2][3] * b[2][3],
				a[3][0] * b[3][0], a[3][1] * b[3][1], a[3][2] * b[3][2], a[3][3] * b[3][3] };
		}
#endif

		template <typename Scalar1, typename Scalar2>
		FORCEINLINE ScalarMatrix<typename Promote<Scalar1, Scalar2>::RT, 4, 4> operator*(const ScalarMatrix<Scalar1, 4, 4>& a, Scalar2 s) noexcept
		{
			using RT = typename Promote<Scalar1, Scalar2>::RT;
			return ScalarMatrix<RT, 4, 4>{ a[0][0] * s, a[0][1] * s, a[0][2] * s, a[0][3] * s,
				a[1][0] * s, a[1][1] * s, a[1][2] * s, a[1][3] * s,
				a[2][0] * s, a[2][1] * s, a[2][2] * s, a[2][3] * s,
				a[3][0] * s, a[3][1] * s, a[3][2] * s, a[3][3] * s };
		}

		template <typename Scalar1, typename Scalar2>
		FORCEINLINE ScalarMatrix<typename Promote<Scalar1, Scalar2>::RT, 4, 4> operator*(Scalar1 s, const ScalarMatrix<Scalar2, 4, 4>& a) noexcept
		{
			return a * s;
		}

		template <typename Scalar1, typename Scalar2>
		FORCEINLINE ScalarMatrix<typename Promote<Scalar1, Scalar2>::RT, 4, 4> operator/(const ScalarMatrix<Scalar1, 4, 4>& a, Scalar2 s) noexcept
		{
			ASSERT(IsZero(s));
			return a * (Scalar1{ 1 } / s);
		}

		template <typename Scalar1, typename Scalar2>
		FORCEINLINE ScalarArray<typename Promote<Scalar1, Scalar2>::RT, 4> Mul(const ScalarMatrix<Scalar1, 4, 4>& a, const ScalarArray<Scalar2, 4>& v) noexcept
		{
			using RT = typename Promote<Scalar1, Scalar2>::RT;
			return ScalarArray<RT, 4>{ Dot(a[0], v),
				Dot(a[1], v),
				Dot(a[2], v),
				Dot(a[3], v) };
		}

		template <typename Scalar1, typename Scalar2>
		FORCEINLINE ScalarArray<typename Promote<Scalar1, Scalar2>::RT, 4> Mul(const ScalarArray<Scalar1, 4>& v, const ScalarMatrix<Scalar2, 4, 4>& a) noexcept
		{
			using RT = typename Promote<Scalar1, Scalar2>::RT;
			return ScalarArray<RT, 3>(TDot(a, 0, v),
				TDot(a, 1, v),
				TDot(a, 2, v),
				TDot(a, 3, v));
		}

		template<typename Scalar1, typename Scalar2>
		FORCEINLINE ScalarMatrix<typename Promote<Scalar1, Scalar2>::RT, 4, 4> Mul(const ScalarMatrix<Scalar1, 4, 4>& a, const ScalarMatrix<Scalar2, 4, 4>& b) noexcept
		{
			using RT = typename Promote<Scalar1, Scalar2>::RT;
			return ScalarMatrix<RT, 4, 4>{TDot(b, 0, a[0]), TDot(b, 1, a[0]), TDot(b, 2, a[0]), TDot(b, 3, a[0]),
				TDot(b, 0, a[1]), TDot(b, 1, a[1]), TDot(b, 2, a[1]), TDot(b, 3, a[1]),
				TDot(b, 0, a[2]), TDot(b, 1, a[2]), TDot(b, 2, a[2]), TDot(b, 3, a[2]), 
				TDot(b, 0, a[3]), TDot(b, 1, a[3]), TDot(b, 2, a[3]), TDot(b, 3, a[3]) };
		}

		template <typename Scalar>
		FORCEINLINE ScalarArray<Scalar, 4> Row(const ScalarMatrix<Scalar, 4, 4>& a, std::size_t i) noexcept
		{
			return a[i];
		}

		template <typename Scalar>
		FORCEINLINE ScalarArray<Scalar, 4> Column(const ScalarMatrix<Scalar, 4, 4>& a, std::size_t i) noexcept
		{
			return ScalarArray<Scalar, 4>{a[0][i], a[1][i], a[2][i], a[3][i]};
		}

		template <typename Scalar>
		FORCEINLINE ScalarMatrix<Scalar, 4, 4> Transpose(const ScalarMatrix<Scalar, 4, 4>& a) noexcept
		{
			return ScalarMatrix<Scalar, 4, 4>{a[0][0], a[1][0], a[2][0], a[3][0], 
				a[0][1], a[1][1], a[2][1], a[3][1],
				a[0][2], a[1][2], a[2][2], a[3][2], 
				a[0][3], a[1][3], a[2][3], a[3][3] };
		}

		template <typename Scalar>
		FORCEINLINE ScalarMatrix<Scalar, 4, 4> Determinate(const ScalarMatrix<Scalar, 4, 4>& a) noexcept
		{
			//Scalar tmp[12];

			//tmp[0] = a[2][2] * a[3][3];
			//tmp[1] = a[3][2] * a[2][3];
			//tmp[2] = a[1][2] * a[3][3];
			//tmp[3] = a[3][2] * a[1][3];
			//tmp[4] = a[1][2] * a[2][3];
			//tmp[5] = a[2][2] * a[1][3];
			//tmp[6] = a[0][2] * a[3][3];
			//tmp[7] = a[3][2] * a[0][3];
			//tmp[8] = a[0][2] * a[2][3];
			//tmp[9] = a[2][2] * a[0][3];
			//tmp[10] = a[0][2] * a[1][3];
			//tmp[11] = a[1][2] * a[0][3];

			//Scalar cof[4];

			//cof[0] = (tmp[0] - tmp[1]) * a[1][1] + (tmp[3] - tmp[2]) * a[2][1] + (tmp[4] - tmp[5]) * a[3][1];
			//cof[1] = (tmp[1] - tmp[0]) * a[0][1] + (tmp[6] - tmp[7]) * a[2][1] + (tmp[9] - tmp[8]) * a[3][1];
			//cof[2] = (tmp[2] - tmp[3]) * a[0][1] + (tmp[7] - tmp[6]) * a[1][1] + (tmp[10] - tmp[11]) * a[3][1];
			//cof[3] = (tmp[5] - tmp[4]) * a[0][1] + (tmp[8] - tmp[9]) * a[1][1] + (tmp[11] - tmp[10]) * a[2][1];

			//return a[0][0] * cof[0] + a[1][0] * cof[1] + a[2][0] * cof[2] + a[3][0] * cof[3];

			Scalar dst[4];
			dst[0] = a[1][1] * a[2][2] * a[3][3] +
				a[1][2] * a[2][3] * a[3][1] +
				a[1][3] * a[2][1] * a[3][2] -
				a[1][1] * a[2][3] * a[3][2] -
				a[1][2] * a[2][1] * a[3][3] -
				a[1][3] * a[2][2] * a[3][1];

			dst[1] = a[1][0] * a[2][3] * a[3][2] +
				a[1][2] * a[2][0] * a[3][3] +
				a[1][3] * a[2][2] * a[3][0] -
				a[1][0] * a[2][2] * a[3][3] -
				a[1][2] * a[2][3] * a[3][0] -
				a[1][3] * a[2][0] * a[3][2];

			dst[2] = a[1][0] * a[2][1] * a[3][3] +
				a[1][1] * a[2][3] * a[3][0] +
				a[1][3] * a[2][0] * a[3][1] -
				a[1][0] * a[2][3] * a[3][1] -
				a[1][1] * a[2][0] * a[3][3] -
				a[1][3] * a[2][1] * a[3][0];

			dst[3] = a[1][0] * a[2][2] * a[3][1] +
				a[1][1] * a[2][0] * a[3][2] +
				a[1][2] * a[2][1] * a[3][0] -
				a[1][0] * a[2][1] * a[3][2] -
				a[1][1] * a[2][2] * a[3][0] -
				a[1][2] * a[2][0] * a[3][1];

			Scalar det = a[0][0] * dst[0] + a[0][1] * dst[1] + a[0][2] * dst[2] + a[0][3] * dst[3];

			return det;
		}

		template <typename Scalar>
		FORCEINLINE ScalarMatrix<Scalar, 4, 4> Inverse(const ScalarMatrix<Scalar, 4, 4>& a) noexcept
		{
			// From: Streaming SIMD Extensions - Inverse of 4x4 Matrix, Intel

			Scalar tmp[12]; /* temp array for pairs */

			ScalarMatrix<Scalar, 4, 4> dst;

			/* calculate pairs for first 8 elements (cofactors) */
			tmp[0] = a[2][2] * a[3][3];
			tmp[1] = a[3][2] * a[2][3];
			tmp[2] = a[1][2] * a[3][3];
			tmp[3] = a[3][2] * a[1][3];
			tmp[4] = a[1][2] * a[2][3];
			tmp[5] = a[2][2] * a[1][3];
			tmp[6] = a[0][2] * a[3][3];
			tmp[7] = a[3][2] * a[0][3];
			tmp[8] = a[0][2] * a[2][3];
			tmp[9] = a[2][2] * a[0][3];
			tmp[10] = a[0][2] * a[1][3];
			tmp[11] = a[1][2] * a[0][3];

			/* calculate first 8 elements (cofactors) */
			dst[0][0] = (tmp[0] - tmp[1]) * a[1][1] + (tmp[3] - tmp[2]) * a[2][1] + (tmp[4] - tmp[5]) * a[3][1];
			dst[0][1] = (tmp[1] - tmp[0]) * a[0][1] + (tmp[6] - tmp[7]) * a[2][1] + (tmp[9] - tmp[8]) * a[3][1];
			dst[0][2] = (tmp[2] - tmp[3]) * a[0][1] + (tmp[7] - tmp[6]) * a[1][1] + (tmp[10] - tmp[11]) * a[3][1];
			dst[0][3] = (tmp[5] - tmp[4]) * a[0][1] + (tmp[8] - tmp[9]) * a[1][1] + (tmp[11] - tmp[10]) * a[2][1];
			dst[1][0] = (tmp[1] - tmp[0]) * a[1][0] + (tmp[2] - tmp[3]) * a[2][0] + (tmp[5] - tmp[4]) * a[3][0];
			dst[1][1] = (tmp[0] - tmp[1]) * a[0][0] + (tmp[7] - tmp[6]) * a[2][0] + (tmp[8] - tmp[9]) * a[3][0];
			dst[1][2] = (tmp[3] - tmp[2]) * a[0][0] + (tmp[6] - tmp[7]) * a[1][0] + (tmp[11] - tmp[10]) * a[3][0];
			dst[1][3] = (tmp[4] - tmp[5]) * a[0][0] + (tmp[9] - tmp[8]) * a[1][0] + (tmp[10] - tmp[11]) * a[2][0];

			/* calculate pairs for second 8 elements (cofactors) */
			tmp[0] = a[2][0] * a[3][1];
			tmp[1] = a[3][0] * a[2][1];
			tmp[2] = a[1][0] * a[3][1];
			tmp[3] = a[3][0] * a[1][1];
			tmp[4] = a[1][0] * a[2][1];
			tmp[5] = a[2][0] * a[1][1];
			tmp[6] = a[0][0] * a[3][1];
			tmp[7] = a[3][0] * a[0][1];
			tmp[8] = a[0][0] * a[2][1];
			tmp[9] = a[2][0] * a[0][1];
			tmp[10] = a[0][0] * a[1][1];
			tmp[11] = a[1][0] * a[0][1];

			/* calculate second 8 elements (cofactors) */
			dst[2][0] = (tmp[0] - tmp[1]) * a[1][3] + (tmp[3] - tmp[2]) * a[2][3] + (tmp[4] - tmp[5]) * a[3][3];
			dst[2][1] = (tmp[1] - tmp[0]) * a[0][3] + (tmp[6] - tmp[7]) * a[2][3] + (tmp[9] - tmp[8]) * a[3][3];
			dst[2][2] = (tmp[2] - tmp[3]) * a[0][3] + (tmp[7] - tmp[6]) * a[1][3] + (tmp[10] - tmp[11]) * a[3][3];
			dst[2][3] = (tmp[5] - tmp[4]) * a[0][3] + (tmp[8] - tmp[9]) * a[1][3] + (tmp[11] - tmp[10]) * a[2][3];
			dst[3][0] = (tmp[2] - tmp[3]) * a[2][2] + (tmp[5] - tmp[4]) * a[3][2] + (tmp[1] - tmp[0]) * a[1][2];
			dst[3][1] = (tmp[8] - tmp[9]) * a[3][2] + (tmp[0] - tmp[1]) * a[0][2] + (tmp[7] - tmp[6]) * a[2][2];
			dst[3][2] = (tmp[6] - tmp[7]) * a[1][2] + (tmp[11] - tmp[10]) * a[3][2] + (tmp[3] - tmp[2]) * a[0][2];
			dst[3][3] = (tmp[10] - tmp[11]) * a[2][2] + (tmp[4] - tmp[5]) * a[0][2] + (tmp[9] - tmp[8]) * a[1][2];

			/* calculate determinant */
			Scalar det = a[0][0] * dst[0][0] + a[1][0] * dst[0][1] + a[2][0] * dst[0][2] + a[3][0] * dst[0][3];

			ASSERT(!IsZero(det));

			Scalar s = Scalar(1) / det;

			return ScalarMatrix<Scalar, 4, 4>(dst[0] * s, dst[1] * s, dst[2] * s, dst[3] * s);
		}

		template<typename Scalar>
		FORCEINLINE ScalarArray<Scalar, 3> Origin(const ScalarMatrix<Scalar, 4, 4>& a) noexcept
		{
			return ScalarArray<Scalar, 3>{ a[3][0], a[3][1], a[3][2] };
		}

		template<typename Scalar>
		FORCEINLINE ScalarMatrix<Scalar, 3, 3> Basis(const ScalarMatrix<Scalar, 4, 4>& a) noexcept
		{
			return ScalarMatrix<Scalar, 3, 3>{ a[0][0], a[0][1], a[0][2],
				a[1][0], a[1][1], a[1][2],
				a[2][0], a[2][1], a[2][2]
			};
		}

		template <typename Scalar> 
		FORCEINLINE Scalar TDot(const ScalarMatrix<Scalar, 4, 4>& a, int i, const ScalarArray<Scalar, 4>& v) noexcept
		{
			return a[0][i] * v.x + a[1][i] * v.y + a[2][i] * v.z + a[3][i] * v.w;
		}

		template <int I, int J, typename Scalar> 
		FORCEINLINE Scalar Cofactor(const ScalarMatrix<Scalar, 4, 4>& a) noexcept
		{
			return a[(I + 1) & 3][(J + 1) & 3] * (a[(I + 2) & 3][(J + 2) & 3] * a[(I + 3) & 3][(J + 3) & 3] - a[(I + 3) & 3][(J + 2) & 3] * a[(I + 2) & 3][(J + 3) & 3]) +
				a[(I + 2) & 3][(J + 1) & 3] * (a[(I + 3) & 3][(J + 2) & 3] * a[(I + 1) & 3][(J + 3) & 3] - a[(I + 1) & 3][(J + 2) & 3] * a[(I + 3) & 3][(J + 3) & 3]) +
				a[(I + 3) & 3][(J + 1) & 3] * (a[(I + 1) & 3][(J + 2) & 3] * a[(I + 2) & 3][(J + 3) & 3] - a[(I + 2) & 3][(J + 2) & 3] * a[(I + 1) & 3][(J + 3) & 3]);
		}

		template <typename Scalar>
		Scalar Trace(const ScalarMatrix<Scalar, 4, 4>& a) noexcept
		{
			return a[0][0] + a[1][1] + a[2][2] + a[3][3];
		}
	}
}
