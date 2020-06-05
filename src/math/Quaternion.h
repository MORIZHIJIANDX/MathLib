#pragma once

#ifdef USE_OSTREAM
#include <ostream>
#endif

#ifdef USE_ISTREAM
#include <istream>
#endif

#include "ScalarMatrix.h"
#include "Metric.h"

namespace Dash
{
	namespace Math
	{
		template <typename Scalar>
		class ScalarQuaternion
		{
		public:
			typedef Scalar ScalarType;

			ScalarQuaternion() noexcept;
			ScalarQuaternion(Identity) noexcept;
			ScalarQuaternion(Scalar x, Scalar y, Scalar z, Scalar w) noexcept;

			template <typename Scalar2> explicit ScalarQuaternion(const Scalar2* v) noexcept;
			template <typename Scalar2> explicit ScalarQuaternion(const ScalarQuaternion<Scalar2>& a) noexcept;
			template <typename Scalar2> explicit ScalarQuaternion(const ScalarArray<Scalar2, 4>& a) noexcept;
			template <typename Scalar2> ScalarQuaternion(const ScalarArray<Scalar2, 3>& axis, Scalar2 angle) noexcept;
			template <typename Scalar2> ScalarQuaternion(const ScalarArray<Scalar2, 3>& from, const ScalarArray<Scalar2, 3>& to) noexcept;

			operator const Scalar* () const noexcept;
			operator Scalar* () noexcept;

			template <typename Scalar2> ScalarQuaternion<Scalar>& operator*=(const ScalarQuaternion<Scalar2>& other) noexcept;

			template <typename Scalar2> ScalarArray<typename Promote<Scalar, Scalar2>::RT, 3> operator()(const ScalarArray<Scalar2, 3>& v) const noexcept;

			union
			{
				struct { Scalar x, y, z, w; };
				ScalarArray<Scalar, 3> xyz;
			};
		};

#ifdef USE_OSTREAM

		template <typename CharT, typename Traits, typename Scalar>
		std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const ScalarQuaternion<Scalar>& a);

#endif

#ifdef USE_ISTREAM

		template <typename CharT, typename Traits, typename Scalar>
		std::basic_istream<CharT, Traits>& operator>>(std::basic_istream<CharT, Traits>& is, ScalarQuaternion<Scalar>& a);

#endif

		template <typename Scalar1, typename Scalar2> bool operator==(const ScalarQuaternion<Scalar1>& a, const ScalarQuaternion<Scalar2>& b) noexcept;
		template <typename Scalar1, typename Scalar2> bool operator!=(const ScalarQuaternion<Scalar1>& a, const ScalarQuaternion<Scalar2>& b) noexcept;

		template <typename Scalar1, typename Scalar2>
		ScalarQuaternion<typename Promote<Scalar1, Scalar2>::RT> operator*(const ScalarQuaternion<Scalar1>& a, const ScalarQuaternion<Scalar2>& b) noexcept;

		template <typename Scalar1, typename Scalar2>
		typename Promote<Scalar1, Scalar2>::RT Dot(const ScalarQuaternion<Scalar1>& a, const ScalarQuaternion<Scalar2>& b) noexcept;

		template <typename Scalar1, typename Scalar2>
		ScalarQuaternion<typename Promote<Scalar1, Scalar2>::RT> Mul(const ScalarQuaternion<Scalar1>& a, const ScalarQuaternion<Scalar2>& b) noexcept;

		template <typename Scalar1, typename Scalar2>
		ScalarArray<typename Promote<Scalar1, Scalar2>::RT, 3> Mul(const ScalarQuaternion<Scalar2>& a, const ScalarArray<Scalar1, 3>& b) noexcept;

		template <typename Scalar> ScalarQuaternion<Scalar> Conjugate(const ScalarQuaternion<Scalar>& a) noexcept;
		template <typename Scalar> ScalarQuaternion<Scalar> Inverse(const ScalarQuaternion<Scalar>& a) noexcept;

		template <typename Scalar> ScalarQuaternion<Scalar> FromAxisAngle(const ScalarArray<Scalar, 3>& axis, Scalar angle) noexcept;
		template <typename Scalar> ScalarQuaternion<Scalar> FromAxisAngle(int axis, Scalar theta) noexcept;
		template <typename Scalar> void ToAxisAngle(ScalarArray<Scalar, 3>& axis, Scalar& theta, const ScalarQuaternion<Scalar>& q) noexcept;

		template <typename Scalar> ScalarQuaternion<Scalar> FromEuler(Scalar yaw, Scalar pitch, Scalar roll) noexcept;
		template <typename Scalar> ScalarQuaternion<Scalar> FromEuler(const ScalarArray<Scalar, 3>& euler) noexcept;

		template <typename Scalar> void ToEuler(Scalar& yaw, Scalar& pitch, Scalar& roll, const ScalarQuaternion<Scalar>& q) noexcept;
		template <typename Scalar> void ToEuler(ScalarArray<Scalar, 3>& euler, const ScalarQuaternion<Scalar>& q) noexcept;

		template <typename Scalar> ScalarQuaternion<Scalar> FromSpherical(Scalar rho, Scalar phi, Scalar theta) noexcept;
		template <typename Scalar> void ToSpherical(Scalar& rho, Scalar& phi, Scalar& theta, const ScalarQuaternion<Scalar>& u) noexcept;

		template <typename Scalar> ScalarArray<Scalar, 2> CartesianToSpherical(const ScalarArray<Scalar, 3>& norm);
		template <typename Scalar> ScalarArray<Scalar, 3> SphericalToCartesian(const ScalarArray<Scalar, 2>& s);

		template <typename Scalar> ScalarQuaternion<Scalar> FromToRotation(const ScalarArray<Scalar, 3>& from, const ScalarArray<Scalar, 3>& to) noexcept;

		template <typename Scalar> ScalarQuaternion<Scalar> FromMatrix(const ScalarMatrix<Scalar, 3, 3>& m) noexcept;
		template <typename Scalar> ScalarQuaternion<Scalar> FromMatrix(const ScalarMatrix<Scalar, 4, 4>& m) noexcept;
		template <typename Scalar> void ToMatrix(ScalarMatrix<Scalar, 4, 4>& m, const ScalarQuaternion<Scalar>& q) noexcept;
		template <typename Scalar> void ToMatrix(ScalarMatrix<Scalar, 3, 3>& m, const ScalarQuaternion<Scalar>& q) noexcept;

		template <typename Scalar> ScalarQuaternion<Scalar> Normalize(const ScalarQuaternion<Scalar>& a) noexcept;

		template <typename Scalar> ScalarQuaternion<Scalar> LerpAndNormalize(const ScalarQuaternion<Scalar>& a, const ScalarQuaternion<Scalar>& b, Scalar t) noexcept;

		template <typename Scalar> ScalarQuaternion<Scalar> Slerp(const ScalarQuaternion<Scalar>& a, const ScalarQuaternion<Scalar>& b, Scalar t) noexcept;



		//Member Function

		template<typename Scalar>
		FORCEINLINE ScalarQuaternion<Scalar>::ScalarQuaternion() noexcept
			: x()
			, y()
			, z()
			, w()
		{}

		template<typename Scalar>
		FORCEINLINE
			ScalarQuaternion<Scalar>::ScalarQuaternion(Identity) noexcept
			: x()
			, y()
			, z()
			, w(Scalar{ 1 })
		{}

		template<typename Scalar>
		FORCEINLINE ScalarQuaternion<Scalar>::ScalarQuaternion(Scalar x, Scalar y, Scalar z, Scalar w) noexcept
			: x(x)
			, y(y)
			, z(z)
			, w(w)
		{}

		template<typename Scalar>
		template<typename Scalar2>
		FORCEINLINE ScalarQuaternion<Scalar>::ScalarQuaternion(const Scalar2* v) noexcept
			: x(Scalar{ v[0] })
			, y(Scalar{ v[1] })
			, z(Scalar{ v[2] })
			, w(Scalar{ v[3] })
		{
			ASSERT(v != nullptr);
		}

		template<typename Scalar>
		template<typename Scalar2>
		FORCEINLINE ScalarQuaternion<Scalar>::ScalarQuaternion(const ScalarQuaternion<Scalar2>& a) noexcept
			: x(Scalar{ a.x })
			, y(Scalar{ a.y })
			, z(Scalar{ a.z })
			, w(Scalar{ a.w })
		{}

		template<typename Scalar>
		template<typename Scalar2>
		FORCEINLINE ScalarQuaternion<Scalar>::ScalarQuaternion(const ScalarArray<Scalar2, 4>& a) noexcept
			: x(Scalar{ a.x })
			, y(Scalar{ a.y })
			, z(Scalar{ a.z })
			, w(Scalar{ a.w })
		{
		}

		template<typename Scalar>
		template<typename Scalar2>
		FORCEINLINE ScalarQuaternion<Scalar>::ScalarQuaternion(const ScalarArray<Scalar2, 3>& axis, Scalar2 angle) noexcept
		{
			Scalar halfTheta = angle * Scalar(0.5);
			ScalarArray<Scalar, 3> normalizedAxis = Normalize(axis) * Sin(halfTheta);
			x = normalizedAxis.x;
			y = normalizedAxis.y;
			z = normalizedAxis.z;
			w = Cos(halfTheta);
		}

		template<typename Scalar>
		template<typename Scalar2>
		FORCEINLINE ScalarQuaternion<Scalar>::ScalarQuaternion(const ScalarArray<Scalar2, 3>& from, const ScalarArray<Scalar2, 3>& to) noexcept
		{
			const ScalarArray<Scalar, 3> normStart = Normalize(from);
			const ScalarArray<Scalar, 3> normEnd = Normalize(to);
			const Scalar d = Dot(normStart, normEnd);

			if (d > Scalar{ -1 } + ScalarTraits<Scalar>::Epsilon())
			{
				const ScalarArray<Scalar, 3> c = Cross(normStart,normEnd);
				const Scalar s = Sqrt((Scalar{ 1 } +d) * Scalar { 2 });
				const Scalar invS = 1.0f / s;

				x = c.x * invS;
				y = c.y * invS;
				z = c.z * invS;
				w = Scalar{ 0.5 } *s;
			}
			else
			{
				ScalarArray<Scalar, 3> axis = Cross(ScalarArray<Scalar, 3>{ Unit<0>{} }, normStart);

				if (Length(axis) < ScalarTraits<Scalar>::Epsilon())
				{
					axis = Cross(ScalarArray<Scalar, 3>{ Unit<1>{} }, normStart);
				}

				Scalar halfTheta = Math::Radians(Scalar{180}) * Scalar(0.5);
				ScalarArray<Scalar, 3> normalizedAxis = Normalize(axis) * Sin(halfTheta);
				x = normalizedAxis.x;
				y = normalizedAxis.y;
				z = normalizedAxis.z;
				w = Cos(halfTheta);
			}
		}

		template<typename Scalar>
		FORCEINLINE ScalarQuaternion<Scalar>::operator const Scalar* () const noexcept
		{
			return &x;
		}

		template<typename Scalar>
		FORCEINLINE ScalarQuaternion<Scalar>::operator Scalar* () noexcept
		{
			return &x;
		}

		template<typename Scalar>
		template<typename Scalar2>
		FORCEINLINE ScalarQuaternion<Scalar>& ScalarQuaternion<Scalar>::operator*=(const ScalarQuaternion<Scalar2>& other) noexcept
		{
			ScalarQuaternion<Scalar> temp;
			temp.x = w * other.x + x * other.w + y * other.z - z * other.y;
			temp.y = w * other.y - x * other.z + y * other.w + z * other.x;
			temp.z = w * other.z + x * other.y - y * other.x + z * other.w;
			temp.w = w * other.w - x * other.x - y * other.y - z * other.z;
			*this = temp;

			return *this;
		}

		template<typename Scalar>
		template<typename Scalar2>
		FORCEINLINE ScalarArray<typename Promote<Scalar, Scalar2>::RT, 3> ScalarQuaternion<Scalar>::operator()(const ScalarArray<Scalar2, 3>& v) const noexcept
		{
			using RT = typename Promote<Scalar, Scalar2>::RT;

			ScalarArray<RT, 3> u{ x, y, z };

			ScalarArray<RT, 3> c1 = Cross(u, v);
			ScalarArray<RT, 3> c2 = Cross(u, c1);

			return v + RT{ 2 } *(c1 * w + c2);
		}




		//Nomember Function

#ifdef USE_OSTREAM

		template<typename CharT, typename Traits, typename Scalar>
		FORCEINLINE std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const ScalarQuaternion<Scalar>& a)
		{
			return os << a.x << ' ' << a.y << ' ' << a.z << ' ' << a.w;
		}

#endif

#ifdef USE_ISTREAM

		template<typename CharT, typename Traits, typename Scalar>
		FORCEINLINE std::basic_istream<CharT, Traits>& operator>>(std::basic_istream<CharT, Traits>& is, ScalarQuaternion<Scalar>& a)
		{
			return is >> a.x >> a.y >> a.z >> a.w;
		}

#endif

		template<typename Scalar1, typename Scalar2>
		FORCEINLINE bool operator==(const ScalarQuaternion<Scalar1>& a, const ScalarQuaternion<Scalar2>& b) noexcept
		{
			return (a.x == b.x) && (a.y == b.y) && (a.z == b.z) && (a.w == b.w);
		}

		template<typename Scalar1, typename Scalar2>
		FORCEINLINE bool operator!=(const ScalarQuaternion<Scalar1>& a, const ScalarQuaternion<Scalar2>& b) noexcept
		{
			return (a.x != b.x) || (a.y != b.y) || (a.z != b.z) || (a.w != b.w);
		}

		template<typename Scalar1, typename Scalar2>
		FORCEINLINE ScalarQuaternion<typename Promote<Scalar1, Scalar2>::RT> operator*(const ScalarQuaternion<Scalar1>& a, const ScalarQuaternion<Scalar2>& b) noexcept
		{
			typedef typename Promote<Scalar1, Scalar2>::RT RT;
			return ScalarQuaternion<RT>(a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y,
				a.w * b.y + a.y * b.w + a.z * b.x - a.x * b.z,
				a.w * b.z + a.z * b.w + a.x * b.y - a.y * b.x,
				a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z);
		}

		template<typename Scalar1, typename Scalar2>
		FORCEINLINE typename Promote<Scalar1, Scalar2>::RT Dot(const ScalarQuaternion<Scalar1>& a, const ScalarQuaternion<Scalar2>& b) noexcept
		{
			return typename Promote<Scalar1, Scalar2>::RT(a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w);
		}

		template<typename Scalar1, typename Scalar2>
		FORCEINLINE
			ScalarQuaternion<typename Promote<Scalar1, Scalar2>::RT> Mul(const ScalarQuaternion<Scalar1>& a, const ScalarQuaternion<Scalar2>& b) noexcept
		{
			typedef typename Promote<Scalar1, Scalar2>::RT RT;
			return ScalarQuaternion<RT>(a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y,
				a.w * b.y + a.y * b.w + a.z * b.x - a.x * b.z,
				a.w * b.z + a.z * b.w + a.x * b.y - a.y * b.x,
				a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z);
		}

		template<typename Scalar1, typename Scalar2>
		FORCEINLINE ScalarArray<typename Promote<Scalar1, Scalar2>::RT, 3> Mul(const ScalarQuaternion<Scalar2>& q, const ScalarArray<Scalar1, 3>& v) noexcept
		{
			typedef typename Promote<Scalar1, Scalar2>::RT RT;

			ScalarArray<RT, 3> u{ q.xyz };

			ScalarArray<RT, 3> c1 = Cross(u, v);
			ScalarArray<RT, 3> c2 = Cross(u, c1);

			return v + RT{ 2 } *(c1 * q.w + c2);
		}

		template<typename Scalar>
		FORCEINLINE ScalarQuaternion<Scalar> Conjugate(const ScalarQuaternion<Scalar>& q) noexcept
		{
			return ScalarQuaternion<Scalar>{-q.x, -q.y, -q.z, q.w};
		}

		template<typename Scalar>
		FORCEINLINE ScalarQuaternion<Scalar> Inverse(const ScalarQuaternion<Scalar>& q) noexcept
		{
			Scalar invLength = Scalar(1) / Length(q);
			ASSERT(IsPositive(invLength));
			return ScalarQuaternion<Scalar>(-q.x * invLength, -q.y * invLength, -q.z * invLength, q.w * invLength);
		}

		template<typename Scalar>
		FORCEINLINE ScalarQuaternion<Scalar> FromAxisAngle(const ScalarArray<Scalar, 3>& axis, Scalar angle) noexcept
		{
			return ScalarQuaternion<Scalar>(axis, angle);
		}

		template<typename Scalar>
		FORCEINLINE ScalarQuaternion<Scalar> FromAxisAngle(int axis, Scalar angle) noexcept
		{ 
			ASSERT(0 <= axis && axis < 3);
			Scalar halfTheta = angle * Scalar{ 0.5 };
			ScalarQuaternion<Scalar> result(Scalar(), Scalar(), Scalar(), Cos(halfTheta));
			result[axis] = Sin(halfTheta);
			return result;
		}

		template<typename Scalar>
		FORCEINLINE void ToAxisAngle(ScalarArray<Scalar, 3>& axis, Scalar& theta, const ScalarQuaternion<Scalar>& q) noexcept
		{
			Scalar s = Sqrt(q.x * q.x + q.y * q.y + q.z * q.z);
			if (!IsZero(s))
			{
				theta = Atan2(s, q.w) * Scalar { 2 };
				axis = ScalarArray<Scalar, 3>(q.x, q.y, q.z) / s;
			}
			else
			{
				theta = Scalar();
				axis = Unit<0>();
			}
		}

		template<typename Scalar>
		FORCEINLINE ScalarQuaternion<Scalar> FromEuler(Scalar yaw, Scalar pitch, Scalar roll) noexcept
		{
			ScalarArray<Scalar, 4> angles{ pitch, yaw, roll, Scalar(0) };
			angles *= Scalar(0.5);
			ScalarArray<Scalar, 4> sinAngles{ Sin(angles.x), Sin(angles.y), Sin(angles.z), Scalar(0) };
			ScalarArray<Scalar, 4> cosAngles{ Cos(angles.x), Cos(angles.y), Cos(angles.z), Scalar(0) };

			ScalarArray<Scalar, 4> p0{ sinAngles.x, cosAngles.x, cosAngles.x, cosAngles.x };
			ScalarArray<Scalar, 4> y0{ cosAngles.y, sinAngles.y, cosAngles.y, cosAngles.y };
			ScalarArray<Scalar, 4> r0{ cosAngles.z, cosAngles.z, sinAngles.z, cosAngles.z };
			ScalarArray<Scalar, 4> p1{ cosAngles.x, sinAngles.x, sinAngles.x, sinAngles.x };
			ScalarArray<Scalar, 4> y1{ sinAngles.y, cosAngles.y, sinAngles.y, sinAngles.y };
			ScalarArray<Scalar, 4> r1{ sinAngles.z, sinAngles.z, cosAngles.z, sinAngles.z };

			ScalarArray<Scalar, 4> sign{ Scalar{1}, -Scalar{1}, -Scalar{1}, Scalar{1} };

			ScalarArray<Scalar, 4> q1 = p0 * sign;
			ScalarArray<Scalar, 4> q0 = p0 * y0;
			q1 = q1 * y1;
			q0 = q0 * r0;

			return ScalarQuaternion<Scalar>(q1 * r1 + q0);
		}

		template<typename Scalar>
		FORCEINLINE ScalarQuaternion<Scalar> FromEuler(const ScalarArray<Scalar, 3>& euler) noexcept
		{
			return FromEuler(euler.x, euler.y, euler.z);
		}

		template<typename Scalar>
		FORCEINLINE void ToEuler(Scalar& yaw, Scalar& pitch, Scalar& roll, const ScalarQuaternion<Scalar>& q) noexcept
		{
			yaw = Atan2(Scalar{ 2 } *(q.w * q.y + q.z * q.x), Scalar{ 1 } -Scalar{ 2 } *(q.x * q.x + q.y * q.y));
			pitch = ASin(Scalar{ 2 } *(q.w * q.x - q.y * q.z));
			roll = Atan2(Scalar{ 2 } *(q.w * q.z + q.x * q.y), Scalar{ 1 } -Scalar{ 2 } *(q.z * q.z + q.x * q.x));
		}

		template<typename Scalar>
		FORCEINLINE void ToEuler(ScalarArray<Scalar, 3>& euler, const ScalarQuaternion<Scalar>& q) noexcept
		{
			ToEuler(euler.x, euler.y, euler.z, q);
		}

		template<typename Scalar>
		FORCEINLINE ScalarQuaternion<Scalar> FromSpherical(Scalar rho, Scalar phi, Scalar theta) noexcept
		{
			ScalarArray<Scalar, 2> c1 = ScalarArray<Scalar, 2>{ Cos(theta), Sin(theta) };
			ScalarArray<Scalar, 2> c2 = ScalarArray<Scalar, 2>{ Cos(phi), Sin(phi) };
			ScalarArray<Scalar, 3> axis(c1.x * c2.y, c1.y * c2.y, c2.x);
			return FromAxisAngle(axis, rho);
		}

		template<typename Scalar>
		FORCEINLINE void ToSpherical(Scalar& rho, Scalar& phi, Scalar& theta, const ScalarQuaternion<Scalar>& u) noexcept
		{
			rho = ACos(u.w) * Scalar(2);
			ScalarArray<Scalar, 3> axis = Normalize(u.xyz);
			phi = ACos(axis.z);
			theta = Atan2(axis.y, axis.x);
		}

		template<typename Scalar>
		FORCEINLINE ScalarArray<Scalar, 2> CartesianToSpherical(const ScalarArray<Scalar, 3>& norm)
		{
			Scalar theta = Atan2(norm.z, norm.x);
			Scalar phi = ACos(norm.y);

			return ScalarArray<Scalar, 2>{theta, phi};
		}

		template<typename Scalar>
		FORCEINLINE ScalarArray<Scalar, 3> SphericalToCartesian(const ScalarArray<Scalar, 2>& s)
		{
			Scalar sinTheta, cosTheta;
			SinCos(s.x, sinTheta, cosTheta);
			
			Scalar sinPhi = Sqrt(1 - s.y * s.y);
			Scalar cosPhi = s.y;

			return ScalarArray<Scalar, 3>{ sinPhi* cosTheta, cosPhi, sinPhi* sinTheta };
		}

		template<typename Scalar>
		FORCEINLINE ScalarArray<Scalar, 3> SphericalToCartesian(Scalar theta, Scalar phi)
		{
			Scalar sinTheta, cosTheta;
			SinCos(theta, sinTheta, cosTheta);

			Scalar sinPhi, cosPhi;
			SinCos(phi, sinPhi, cosPhi);

			return ScalarArray<Scalar, 3>{ sinPhi* cosTheta, cosPhi, sinPhi* sinTheta };
		}

		template<typename Scalar>
		FORCEINLINE ScalarQuaternion<Scalar> FromToRotation(const ScalarArray<Scalar, 3>& from, const ScalarArray<Scalar, 3>& to) noexcept
		{
			const ScalarArray<Scalar, 3> normStart = Normalize(from);
			const ScalarArray<Scalar, 3> normEnd = Normalize(to);
			const Scalar d = Dot(normStart, normEnd);

			if (d > Scalar{ -1 } +ScalarTraits<Scalar>::Epsilon())
			{
				const ScalarArray<Scalar, 3> c = Cross(normStart, normEnd);
				const Scalar s = Sqrt((Scalar{ 1 } +d) * Scalar { 2 });
				const Scalar invS = 1.0f / s;

				return ScalarQuaternion<Scalar>{
					c.x * invS,
					c.y * invS,
					c.z * invS,
					Scalar{ 0.5 } *s
				};
			}
			else
			{
				ScalarArray<Scalar, 3> axis = Cross(ScalarArray<Scalar, 3>{ Unit<0>{} }, normStart);

				if (Length(axis) < ScalarTraits<Scalar>::Epsilon())
				{
					axis = Cross(ScalarArray<Scalar, 3>{ Unit<1>{} }, normStart);
				}

				Scalar halfTheta = Math::Radians(Scalar{ 180 }) * Scalar(0.5);
				ScalarArray<Scalar, 3> normalizedAxis = Normalize(axis) * Sin(halfTheta);

				return ScalarQuaternion<Scalar>{
						normalizedAxis.x,
						normalizedAxis.y,
						normalizedAxis.z,
						Cos(halfTheta)
				};
			}
		}

		template<typename Scalar>
		FORCEINLINE ScalarQuaternion<Scalar> FromMatrix(const ScalarMatrix<Scalar, 3, 3>& a) noexcept
		{
			Scalar t = Trace(a);

			if (IsPositive(t))
			{
				Scalar d = Sqrt(t + Scalar{ 1 });
				Scalar s = Scalar(0.5) / d;

				return ScalarQuaternion<Scalar>{(a[2][1] - a[1][2])* s,
					(a[0][2] - a[2][0])* s,
					(a[1][0] - a[0][1])* s,
					d* Scalar{ 0.5 }};
			}
			else
			{
				ScalarQuaternion<Scalar> result;

				int i = a[0][0] < a[1][1] ? (a[1][1] < a[2][2] ? 2 : 1) : (a[0][0] < a[2][2] ? 2 : 0);
				int j = (i + 1) % 3;
				int k = (i + 2) % 3;

				Scalar d = Sqrt(a[i][i] - a[j][j] - a[k][k] + Scalar{ 1 });
				Scalar s = Scalar{ 0.5 } / d;

				result[i] = d * Scalar{ 0.5 };
				result[j] = (a[j][i] + a[i][j]) * s;
				result[k] = (a[k][i] + a[i][k]) * s;
				result[3] = (a[k][j] - a[j][k]) * s;

				return result;
			}
		}

		template<typename Scalar>
		FORCEINLINE ScalarQuaternion<Scalar> FromMatrix(const ScalarMatrix<Scalar, 4, 4>& m) noexcept
		{
			return FromMatrix(Basis(m));
		}

		template<typename Scalar>
		FORCEINLINE void ToMatrix(ScalarMatrix<Scalar, 4, 4>& m, const ScalarQuaternion<Scalar>& q) noexcept
		{
			m = ScalarMatrix<Scalar, 4, 4>{ Identity{} };
			m[0][0] = 1 - 2 * q.y * q.y - 2 * q.z * q.z;
			m[0][1] = 2 * q.x * q.y + 2 * q.z * q.w;
			m[0][2] = 2 * q.x * q.z - 2 * q.y * q.w;

			m[1][0] = 2 * q.x * q.y - 2 * q.z * q.w;
			m[1][1] = 1 - 2 * q.x * q.x - 2 * q.z * q.z;
			m[1][2] = 2 * q.y * q.z + 2 * q.x * q.w;

			m[2][0] = 2 * q.x * q.z + 2 * q.y * q.w;
			m[2][1] = 2 * q.y * q.z - 2 * q.x * q.w;
			m[2][2] = 1 - 2 * q.x * q.x - 2 * q.y * q.y;
		}

		template<typename Scalar>
		FORCEINLINE void ToMatrix(ScalarMatrix<Scalar, 3, 3>& m, const ScalarQuaternion<Scalar>& q) noexcept
		{
			m = ScalarMatrix<Scalar, 3, 3>{ Identity{} };
			m[0][0] = 1 - 2 * q.y * q.y - 2 * q.z * q.z;
			m[0][1] = 2 * q.x * q.y + 2 * q.z * q.w;
			m[0][2] = 2 * q.x * q.z - 2 * q.y * q.w;

			m[1][0] = 2 * q.x * q.y - 2 * q.z * q.w;
			m[1][1] = 1 - 2 * q.x * q.x - 2 * q.z * q.z;
			m[1][2] = 2 * q.y * q.z + 2 * q.x * q.w;

			m[2][0] = 2 * q.x * q.z + 2 * q.y * q.w;
			m[2][1] = 2 * q.y * q.z - 2 * q.x * q.w;
			m[2][2] = 1 - 2 * q.x * q.x - 2 * q.y * q.y;
		}

		template<typename Scalar>
		FORCEINLINE ScalarQuaternion<Scalar> Normalize(const ScalarQuaternion<Scalar>& a) noexcept
		{ 
			Scalar invLength = Scalar(1) / Length(a);
			return ScalarQuaternion<Scalar>(a.x * invLength, a.y * invLength, a.z * invLength, a.w * invLength);
		}

		template<typename Scalar>
		FORCEINLINE ScalarQuaternion<Scalar> LerpAndNormalize(const ScalarQuaternion<Scalar>& a, const ScalarQuaternion<Scalar>& b, Scalar t) noexcept
		{
			Scalar lerpParam = 1 - t;
			return Normalize(a + ScalarQuaternion<Scalar>{ b.x* lerpParam, b.y* lerpParam, b.z* lerpParam, b.w* lerpParam  });
		}

		template<typename Scalar>
		FORCEINLINE ScalarQuaternion<Scalar> Slerp(const ScalarQuaternion<Scalar>& a, const ScalarQuaternion<Scalar>& b, Scalar t) noexcept
		{
			ScalarArray<Scalar, 4> shortQx{ b };
			ScalarArray<Scalar, 4> vx{ b };
			ScalarArray<Scalar, 4> vy{ b };
			if (LengthSquared(vx - vy) > LengthSquared(vx + vy))
			{
				shortQx *= Scalar(-1);
			}

			Scalar cosTheta = Dot(vx, shortQx);

			if (cosTheta > Scalar(0.9999))
			{
				return LerpAndNormalize(a, ScalarQuaternion{ shortQx }, t);
			}

			Scalar theta = ACos(cosTheta);
			Scalar sinTheta = ASin(theta);

			return ScalarQuaternion<Scalar>((vx * Sin(Scalar(1) - t) * theta + shortQx * Sin(t * theta)) / sinTheta);
		}
	}
}