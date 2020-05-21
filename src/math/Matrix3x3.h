#pragma once

namespace Dash
{
	namespace Math
	{
		template<typename Scalar>
		class ScalarMatrix<Scalar, 3, 3>
		{
		public:
			using DataType = ScalarArray<Scalar, 3>;

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
			constexpr explicit ScalarMatrix(const ScalarArray<Scalar, 3>& r0,
				const ScalarArray<Scalar, 3>& r1,
				const ScalarArray<Scalar, 3>& r2);

			operator const ScalarArray<Scalar, 3>* () const;
			operator ScalarArray<Scalar, 3>* ();

			ScalarMatrix<Scalar, 3, 3>& operator=(Zero) noexcept;
			ScalarMatrix<Scalar, 3, 3>& operator+=(Zero) noexcept;
			ScalarMatrix<Scalar, 3, 3>& operator-=(Zero) noexcept;
			ScalarMatrix<Scalar, 3, 3>& operator*=(Zero) noexcept;

			ScalarMatrix<Scalar, 3, 3>& operator=(Identity) noexcept;

			ScalarMatrix<Scalar, 3, 3>& operator=(Scalar s) noexcept;
			ScalarMatrix<Scalar, 3, 3>& operator*=(Scalar s) noexcept;
			ScalarMatrix<Scalar, 3, 3>& operator/=(Scalar s) noexcept;

			ScalarMatrix<Scalar, 3, 3>& operator+=(const ScalarMatrix<Scalar, 3, 3>& a) noexcept;
			ScalarMatrix<Scalar, 3, 3>& operator-=(const ScalarMatrix<Scalar, 3, 3>& a) noexcept;

			void SetValue(Scalar a00, Scalar a01, Scalar a02,
				Scalar a10, Scalar a11, Scalar a12,
				Scalar a20, Scalar a21, Scalar a22) noexcept;

			template<typename Scalar2>
			void SetValue(const Scalar2* v) noexcept;

			void SetRows(const ScalarArray<Scalar, 3>& r0,
				const ScalarArray<Scalar, 3>& r1,
				const ScalarArray<Scalar, 3>& r2) noexcept;

			void SetColumns(const ScalarArray<Scalar, 3>& c0,
				const ScalarArray<Scalar, 3>& c1,
				const ScalarArray<Scalar, 3>& c2) noexcept;

		private:
			ScalarArray<Scalar, 3> mRows[3];
		};

#ifdef USE_OSTREAM

		template <typename CharT, typename Traits, typename Scalar>
		std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const ScalarMatrix<Scalar, 3, 3>& a);

#endif

#ifdef USE_ISTREAM

		template <typename CharT, typename Traits, typename Scalar>
		std::basic_istream<CharT, Traits>& operator>>(std::basic_istream<CharT, Traits>& is, ScalarMatrix<Scalar, 3, 3>& a);

#endif

		template <typename Scalar> bool operator==(const ScalarMatrix<Scalar, 3, 3>& a, const ScalarMatrix<Scalar, 3, 3>& b) noexcept;

		template <typename Scalar> ScalarMatrix<Scalar, 3, 3> operator-(const ScalarMatrix<Scalar, 3, 3>& a) noexcept;

		template <typename Scalar1, typename Scalar2>
		ScalarMatrix<typename Promote<Scalar1, Scalar2>::RT, 3, 3> operator+(const ScalarMatrix<Scalar1, 3, 3>& a, const ScalarMatrix<Scalar2, 3, 3>& b) noexcept;

		template <typename Scalar1, typename Scalar2>
		ScalarMatrix<typename Promote<Scalar1, Scalar2>::RT, 3, 3> operator-(const ScalarMatrix<Scalar1, 3, 3>& a, const ScalarMatrix<Scalar2, 3, 3>& b) noexcept;

#if USE_MATRIX_COMP_MULT
		template <typename Scalar1, typename Scalar2>
		ScalarMatrix<typename Promote<Scalar1, Scalar2>::RT> operator*(const ScalarMatrix<Scalar1, 3, 3>& a, const ScalarMatrix<Scalar2, 3, 3>& b);
#endif

		template <typename Scalar1, typename Scalar2>
		ScalarMatrix<typename Promote<Scalar1, Scalar2>::RT, 3, 3> operator*(const ScalarMatrix<Scalar1, 3, 3>& a, Scalar2 s) noexcept;

		template <typename Scalar1, typename Scalar2>
		ScalarMatrix<typename Promote<Scalar1, Scalar2>::RT, 3, 3> operator*(Scalar1 s, const ScalarMatrix<Scalar2, 3, 3>& a) noexcept;

		template <typename Scalar1, typename Scalar2>
		ScalarMatrix<typename Promote<Scalar1, Scalar2>::RT, 3, 3> operator/(const ScalarMatrix<Scalar1, 3, 3>& a, Scalar2 s) noexcept;

		template <typename Scalar1, typename Scalar2>
		ScalarArray<typename Promote<Scalar1, Scalar2>::RT, 3> Mul(const ScalarMatrix<Scalar1, 3, 3>& a, const ScalarArray<Scalar2, 3>& v) noexcept;

		template <typename Scalar1, typename Scalar2>
		ScalarArray<typename Promote<Scalar1, Scalar2>::RT, 3> Mul(const ScalarArray<Scalar1, 3>& v, const ScalarMatrix<Scalar2, 3, 3>& a) noexcept;

		template<typename Scalar1, typename Scalar2>
		ScalarMatrix<typename Promote<Scalar1, Scalar2>::RT, 3, 3> Mul(const ScalarMatrix<Scalar1, 3, 3>& a, const ScalarMatrix<Scalar2, 3, 3>& b) noexcept;

		template <typename Scalar>
		ScalarArray<Scalar, 3> Row(const ScalarMatrix<Scalar, 3, 3>& a, std::size_t i) noexcept;

		template <typename Scalar>
		ScalarArray<Scalar, 3> Column(const ScalarMatrix<Scalar, 3, 3>& a, std::size_t i) noexcept;

		template <typename Scalar>
		ScalarMatrix<Scalar, 3, 3> Transpose(const ScalarMatrix<Scalar, 3, 3>& a) noexcept;

		template <typename Scalar>
		ScalarMatrix<Scalar, 3, 3> Determinate(const ScalarMatrix<Scalar, 3, 3>& a) noexcept;

		template <typename Scalar>
		ScalarMatrix<Scalar, 3, 3> Inverse(const ScalarMatrix<Scalar, 3, 3>& a) noexcept;

		template <typename Scalar> Scalar TDot(const ScalarMatrix<Scalar, 3, 3>& a, int i, const ScalarArray<Scalar, 3>& v) noexcept;

		template <int I, int J, typename Scalar> Scalar Cofactor(const ScalarMatrix<Scalar, 3, 3>& a) noexcept;

		//template <typename Scalar>
		//Quaternion<Scalar> DecomposeRotation(const ScalarMatrix<Scalar, 3, 3>& a);


		//Member Function 
		template<typename Scalar>
		FORCEINLINE constexpr ScalarMatrix<Scalar, 3, 3>::ScalarMatrix()
		{
		}

		template<typename Scalar>
		FORCEINLINE constexpr ScalarMatrix<Scalar, 3, 3>::ScalarMatrix(Zero)
		{
			*this = Zero{};
		}

		template<typename Scalar>
		FORCEINLINE constexpr ScalarMatrix<Scalar, 3, 3>::ScalarMatrix(Identity)
		{
			*this = Identity{};
		}

		template<typename Scalar>
		FORCEINLINE constexpr ScalarMatrix<Scalar, 3, 3>::ScalarMatrix(Scalar s)
		{
			*this = s;
		}

		template<typename Scalar>
		template <typename Scalar2>
		FORCEINLINE constexpr ScalarMatrix<Scalar, 3, 3>::ScalarMatrix(const Scalar2* v)
		{
			SetValue(v);
		}

		template<typename Scalar>
		FORCEINLINE constexpr ScalarMatrix<Scalar, 3, 3>::ScalarMatrix(Scalar a00, Scalar a01, Scalar a02,
			Scalar a10, Scalar a11, Scalar a12,
			Scalar a20, Scalar a21, Scalar a22)
		{
			SetValue(a00, a01, a02,
				a10, a11, a12,
				a20, a21, a22);
		}

		template<typename Scalar>
		FORCEINLINE constexpr ScalarMatrix<Scalar, 3, 3>::ScalarMatrix(const ScalarArray<Scalar, 3>& r0,
			const ScalarArray<Scalar, 3>& r1,
			const ScalarArray<Scalar, 3>& r2)
		{
			mRows[0] = r0;
			mRows[1] = r1;
			mRows[2] = r2;
		}

		template<typename Scalar>
		FORCEINLINE ScalarMatrix<Scalar, 3, 3>::operator const ScalarArray<Scalar, 3>* () const
		{
			return mRows;
		}

		template<typename Scalar>
		FORCEINLINE ScalarMatrix<Scalar, 3, 3>::operator ScalarArray<Scalar, 3>* ()
		{
			return mRows;
		}

		template<typename Scalar>
		FORCEINLINE ScalarMatrix<Scalar, 3, 3>& ScalarMatrix<Scalar, 3, 3>::operator=(Zero zero) noexcept
		{
			mRows[0] = zero;
			mRows[1] = zero;
			mRows[2] = zero;

			return *this;
		}

		template<typename Scalar>
		FORCEINLINE ScalarMatrix<Scalar, 3, 3>& ScalarMatrix<Scalar, 3, 3>::operator+=(Zero) noexcept
		{
			return *this;
		}

		template<typename Scalar>
		FORCEINLINE ScalarMatrix<Scalar, 3, 3>& ScalarMatrix<Scalar, 3, 3>::operator-=(Zero) noexcept
		{
			return *this;
		}

		template<typename Scalar>
		FORCEINLINE ScalarMatrix<Scalar, 3, 3>& ScalarMatrix<Scalar, 3, 3>::operator*=(Zero zero) noexcept
		{
			mRows[0] = zero;
			mRows[1] = zero;
			mRows[2] = zero;

			return *this;
		}

		template<typename Scalar>
		FORCEINLINE ScalarMatrix<Scalar, 3, 3>& ScalarMatrix<Scalar, 3, 3>::operator=(Identity) noexcept
		{
			mRows[0] = Unit<0>{};
			mRows[1] = Unit<1>{};
			mRows[2] = Unit<2>{};

			return *this;
		}

		template<typename Scalar>
		FORCEINLINE ScalarMatrix<Scalar, 3, 3>& ScalarMatrix<Scalar, 3, 3>::operator=(Scalar s) noexcept
		{
			SetValue(s, Scalar{}, Scalar{},
				Scalar{}, s, Scalar{},
				Scalar{}, Scalar{}, s);

			return *this;
		}

		template<typename Scalar>
		FORCEINLINE ScalarMatrix<Scalar, 3, 3>& ScalarMatrix<Scalar, 3, 3>::operator*=(Scalar s) noexcept
		{
			mRows[0] = s;
			mRows[1] = s;
			mRows[2] = s;

			return *this;
		}

		template<typename Scalar>
		FORCEINLINE ScalarMatrix<Scalar, 3, 3>& ScalarMatrix<Scalar, 3, 3>::operator/=(Scalar s) noexcept
		{
			ASSERT(!IsZero(s));

			return *this *= (Scalar{ 1 } / s);
		}

		template<typename Scalar>
		FORCEINLINE ScalarMatrix<Scalar, 3, 3>& ScalarMatrix<Scalar, 3, 3>::operator+=(const ScalarMatrix<Scalar, 3, 3>& a) noexcept
		{
			mRows[0] += a[0];
			mRows[1] += a[1];
			mRows[2] += a[2];

			return *this;
		}

		template<typename Scalar>
		FORCEINLINE ScalarMatrix<Scalar, 3, 3>& ScalarMatrix<Scalar, 3, 3>::operator-=(const ScalarMatrix<Scalar, 3, 3>& a) noexcept
		{
			mRows[0] -= a[0];
			mRows[1] -= a[1];
			mRows[2] -= a[2];

			return *this;
		}

		template<typename Scalar>
		FORCEINLINE void ScalarMatrix<Scalar, 3, 3>::SetValue(Scalar a00, Scalar a01, Scalar a02,
			Scalar a10, Scalar a11, Scalar a12,
			Scalar a20, Scalar a21, Scalar a22) noexcept
		{
			mRows[0] = ScalarArray<Scalar, 3>{ a00, a01, a02 };
			mRows[1] = ScalarArray<Scalar, 3>{ a10, a11, a12 };
			mRows[2] = ScalarArray<Scalar, 3>{ a20, a21, a22 };

			return *this;
		}

		template<typename Scalar>
		template<typename Scalar2>
		FORCEINLINE void ScalarMatrix<Scalar, 3, 3>::SetValue(const Scalar2* v) noexcept
		{
			ASSERT(v != nullptr);

			mRows[0] = ScalarArray<Scalar, 3>{ Scalar{v[0]}, Scalar{v[1]}, Scalar{v[2]} };
			mRows[1] = ScalarArray<Scalar, 3>{ Scalar{v[3]}, Scalar{v[4]}, Scalar{v[5]} };
			mRows[2] = ScalarArray<Scalar, 3>{ Scalar{v[6]}, Scalar{v[7]}, Scalar{v[8]} };

			return *this;
		}

		template<typename Scalar>
		FORCEINLINE void ScalarMatrix<Scalar, 3, 3>::SetRows(const ScalarArray<Scalar, 3>& r0, const ScalarArray<Scalar, 3>& r1, const ScalarArray<Scalar, 3>& r2) noexcept
		{
			mRows[0] = r0;
			mRows[1] = r1;
			mRows[2] = r2;

			return *this;
		}

		template<typename Scalar>
		FORCEINLINE void ScalarMatrix<Scalar, 3, 3>::SetColumns(const ScalarArray<Scalar, 3>& c0, const ScalarArray<Scalar, 3>& c1, const ScalarArray<Scalar, 3>& c2) noexcept
		{
			mRows[0] = ScalarArray<Scalar, 3>{ c0.x, c1.x, c2.x };
			mRows[1] = ScalarArray<Scalar, 3>{ c0.y, c1.y, c2.x };
			mRows[2] = ScalarArray<Scalar, 3>{ c0.z, c1.z, c2.x };

			return *this;
		}



		//Nomember Function

#ifdef USE_OSTREAM

		template <typename CharT, typename Traits, typename Scalar>
		std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const ScalarMatrix<Scalar, 3, 3>& a)
		{
			return os << a[0] << ' ' << a[1] << ' ' << a[2];
		}

#endif

#ifdef USE_ISTREAM

		template <typename CharT, typename Traits, typename Scalar>
		std::basic_istream<CharT, Traits>& operator>>(std::basic_istream<CharT, Traits>& is, ScalarMatrix<Scalar, 3, 3>& a)
		{
			return is >> a[0] >> a[1] >> a[2];
		}

#endif

		template <typename Scalar>
		FORCEINLINE bool operator==(const ScalarMatrix<Scalar, 3, 3>& a, const ScalarMatrix<Scalar, 3, 3>& b) noexcept
		{
			return a[0] == b[0] && a[1] == b[1] && a[2] == b[2];
		}

		template <typename Scalar>
		FORCEINLINE ScalarMatrix<Scalar, 3, 3> operator-(const ScalarMatrix<Scalar, 3, 3>& a) noexcept
		{
			return ScalarMatrix<Scalar, 3, 3>{ -a[0][0], -a[0][1], -a[0][2],
				-a[1][0], -a[1][1], -a[1][2],
				-a[2][0], -a[2][1], -a[2][2] };
		}

		template <typename Scalar1, typename Scalar2>
		FORCEINLINE ScalarMatrix<typename Promote<Scalar1, Scalar2>::RT, 3, 3> operator+(const ScalarMatrix<Scalar1, 3, 3>& a, const ScalarMatrix<Scalar2, 3, 3>& b) noexcept
		{
			using RT = typename Promote<Scalar1, Scalar2>::RT;
			return ScalarMatrix<RT, 3, 3>{a[0][0] + b[0][0], a[0][1] + b[0][1], a[0][2] + b[0][2],
				a[1][0] + b[1][0], a[1][1] + b[1][1], a[1][2] + b[1][2],
				a[2][0] + b[2][0], a[2][1] + b[2][1], a[2][2] + b[2][2]};
		}

		template <typename Scalar1, typename Scalar2>
		FORCEINLINE ScalarMatrix<typename Promote<Scalar1, Scalar2>::RT, 3, 3> operator-(const ScalarMatrix<Scalar1, 3, 3>& a, const ScalarMatrix<Scalar2, 3, 3>& b) noexcept
		{
			using RT = typename Promote<Scalar1, Scalar2>::RT;
			return ScalarMatrix<RT, 3, 3>{a[0][0] - b[0][0], a[0][1] - b[0][1], a[0][2] - b[0][2],
				a[1][0] - b[1][0], a[1][1] - b[1][1], a[1][2] - b[1][2],
				a[2][0] - b[2][0], a[2][1] - b[2][1], a[2][2] - b[2][2]};
		}

#if USE_MATRIX_COMP_MULT
		template <typename Scalar1, typename Scalar2>
		FORCEINLINE ScalarMatrix<typename Promote<Scalar1, Scalar2>::RT> operator*(const ScalarMatrix<Scalar1, 3, 3>& a, const ScalarMatrix<Scalar2, 3, 3>& b)
		{
			using RT = typename Promote<Scalar1, Scalar2>::RT;
			return ScalarMatrix<RT, 3, 3>{a[0][0] * b[0][0], a[0][1] * b[0][1], a[0][2] * b[0][2],
				a[1][0] * b[1][0], a[1][1] * b[1][1], a[1][2] * b[1][2],
				a[2][0] * b[2][0], a[2][1] * b[2][1], a[2][2] * b[2][2]};
		}
#endif

		template <typename Scalar1, typename Scalar2>
		FORCEINLINE ScalarMatrix<typename Promote<Scalar1, Scalar2>::RT, 3, 3> operator*(const ScalarMatrix<Scalar1, 3, 3>& a, Scalar2 s) noexcept
		{
			using RT = typename Promote<Scalar1, Scalar2>::RT;
			return ScalarMatrix<RT, 3, 3>{a[0][0] * s, a[0][1] * s, a[0][2] * s,
				a[1][0] * s, a[1][1] * s, a[1][2] * s,
				a[2][0] * s, a[2][1] * s, a[2][2] * s};
		}

		template <typename Scalar1, typename Scalar2>
		FORCEINLINE ScalarMatrix<typename Promote<Scalar1, Scalar2>::RT, 3, 3> operator*(Scalar1 s, const ScalarMatrix<Scalar2, 3, 3>& a) noexcept
		{
			typedef typename Promote<Scalar1, Scalar2>::RT RT;
			return a * s;
		}

		template <typename Scalar1, typename Scalar2>
		FORCEINLINE ScalarMatrix<typename Promote<Scalar1, Scalar2>::RT, 3, 3> operator/(const ScalarMatrix<Scalar1, 3, 3>& a, Scalar2 s) noexcept
		{
			ASSERT(!IsZero(s));
			return a * (Scalar2{ 1 } / s);
		}

		template <typename Scalar1, typename Scalar2>
		FORCEINLINE ScalarArray<typename Promote<Scalar1, Scalar2>::RT, 3> Mul(const ScalarMatrix<Scalar1, 3, 3>& a, const ScalarArray<Scalar2, 3>& v) noexcept
		{
			using RT = typename Promote<Scalar1, Scalar2>::RT;
			return ScalarArray<RT, 3>{ Dot(a[0], v), 
									   Dot(a[1], v), 
				                       Dot(a[2], v) };
		}
	

		template <typename Scalar1, typename Scalar2>
		FORCEINLINE ScalarArray<typename Promote<Scalar1, Scalar2>::RT, 3> Mul(const ScalarArray<Scalar1, 3>& v, const ScalarMatrix<Scalar2, 3, 3>& a) noexcept
		{
			using RT = typename Promote<Scalar1, Scalar2>::RT;
			return ScalarArray<RT, 3>(TDot(a, 0, v),
									  TDot(a, 1, v),
									  TDot(a, 2, v));
		}

		template<typename Scalar1, typename Scalar2>
		FORCEINLINE ScalarMatrix<typename Promote<Scalar1, Scalar2>::RT, 3, 3> Mul(const ScalarMatrix<Scalar1, 3, 3>& a, const ScalarMatrix<Scalar2, 3, 3>& b) noexcept
		{
			using RT = typename Promote<Scalar1, Scalar2>::RT;
			return ScalarMatrix<RT, 3, 3>{TDot(b, 0, a[0]), TDot(b, 1, a[0]), TDot(b, 2, a[0]),
				TDot(b, 0, a[1]), TDot(b, 1, a[1]), TDot(b, 2, a[1]),
				TDot(b, 0, a[2]), TDot(b, 1, a[2]), TDot(b, 2, a[2])};
		}

		template <typename Scalar>
		FORCEINLINE ScalarArray<Scalar, 3> Row(const ScalarMatrix<Scalar, 3, 3>& a, std::size_t i) noexcept
		{
			return a[i];
		}

		template <typename Scalar>
		FORCEINLINE ScalarArray<Scalar, 3> Column(const ScalarMatrix<Scalar, 3, 3>& a, std::size_t i) noexcept
		{
			return ScalarArray<Scalar, 3>{ a[0][i], a[1][i], a[2][i] };
		}

		template <typename Scalar>
		FORCEINLINE ScalarMatrix<Scalar, 3, 3> Transpose(const ScalarMatrix<Scalar, 3, 3>& a) noexcept
		{
			return ScalarMatrix<Scalar, 3, 3>{a[0][0], a[1][0], a[2][0],
				a[0][1], a[1][1], a[2][1],
				a[0][2], a[1][2], a[2][2]};
		}

		template<typename Scalar>
		FORCEINLINE ScalarMatrix<Scalar, 3, 3> Determinate(const ScalarMatrix<Scalar, 3, 3>& a) noexcept
		{
			return Triple(a[0], a[1], a[2]);
		}

		template<typename Scalar>
		FORCEINLINE ScalarMatrix<Scalar, 3, 3> Inverse(const ScalarMatrix<Scalar, 3, 3>& a) noexcept
		{
			ScalarArray<Scalar, 3> co{ Cofactor<0, 0>(a),
				Cofactor<0, 1>(a),
				Cofactor<0, 2>(a) };
			Scalar det = Dot(a[0], co);
			ASSERT(!IsZero(det));
			Scalar s = Scalar(1) / det;
			return ScalarMatrix<Scalar, 3, 3>{co[0] * s, Cofactor<1, 0>(a)* s, Cofactor<2, 0>(a)* s,
				co[1] * s, Cofactor<1, 1>(a)* s, Cofactor<2, 1>(a)* s,
				co[2] * s, Cofactor<1, 2>(a)* s, Cofactor<2, 2>(a)* s};
		}

		template<typename Scalar>
		FORCEINLINE Scalar TDot(const ScalarMatrix<Scalar, 3, 3>& a, int i, const ScalarArray<Scalar, 3>& v) noexcept
		{
			return a[0][i] * v.x + a[1][i] * v.y + a[2][i] * v.z;
		}

		template<int I, int J, typename Scalar>
		FORCEINLINE Scalar Cofactor(const ScalarMatrix<Scalar, 3, 3>& a) noexcept
		{
			return a[(I + 1) % 3][(J + 1) % 3] * a[(I + 2) % 3][(J + 2) % 3] -
				a[(I + 2) % 3][(J + 1) % 3] * a[(I + 1) % 3][(J + 2) % 3];
		}


	}
}