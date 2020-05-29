#pragma once

namespace Dash
{
	namespace Math
	{
		template<typename Scalar>
		class ScalarArray<Scalar, 4>
		{
		public:
			using DataType = std::array<Scalar, 4>;

			using ScalarType = Scalar;
			using SizeType = std::size_t;
			using DifferenceType = typename DataType::difference_type;
			using Pointer = ScalarType*;
			using ConstPointer = const ScalarType*;
			using Reference = ScalarType&;
			using ConstReference = const ScalarType&;

			using Iterator = typename DataType::iterator;
			using ConstIterator = typename DataType::const_iterator;

			using ReverseIterator = typename DataType::reverse_iterator;
			using ConstReverseIterator = typename DataType::const_reverse_iterator;

			constexpr ScalarArray() noexcept;
			constexpr explicit ScalarArray(Zero) noexcept;
			template <std::size_t I> constexpr explicit ScalarArray(Unit<I>) noexcept;
			template <typename Scalar2> constexpr explicit ScalarArray(Scalar2 x, Scalar2 y, Scalar2 z, Scalar2 w = Scalar2{}) noexcept;
			template <typename Scalar2> constexpr explicit ScalarArray(const ScalarArray<Scalar2, 3> v, Scalar2 w = Scalar2{}) noexcept;
			template <typename Scalar2> constexpr explicit ScalarArray(const Scalar2* v) noexcept;
			template <typename Scalar2> constexpr ScalarArray(const ScalarArray<Scalar2, 4>& v) noexcept;

			operator ConstPointer () const noexcept;
			operator Pointer () noexcept;

			ScalarArray<Scalar, 4>& operator=(Zero) noexcept;
			ScalarArray<Scalar, 4>& operator+=(Zero) noexcept;
			ScalarArray<Scalar, 4>& operator-=(Zero) noexcept;
			ScalarArray<Scalar, 4>& operator*=(Zero) noexcept;

			template<std::size_t I> ScalarArray<Scalar, 4>& operator=(Unit<I>) noexcept;
			template<typename Scalar2> ScalarArray<Scalar, 4>& operator=(const Scalar2* v) noexcept;

			template<typename Scalar2> ScalarArray<Scalar, 4>& operator=(const ScalarArray<Scalar2, 4>& v) noexcept;
			template<typename Scalar2> ScalarArray<Scalar, 4>& operator+=(const ScalarArray<Scalar2, 4>& v) noexcept;
			template<typename Scalar2> ScalarArray<Scalar, 4>& operator-=(const ScalarArray<Scalar2, 4>& v) noexcept;
			template<typename Scalar2> ScalarArray<Scalar, 4>& operator*=(const ScalarArray<Scalar2, 4>& v) noexcept;

			ScalarArray<Scalar, 4>& operator*=(Scalar s) noexcept;
			ScalarArray<Scalar, 4>& operator/=(Scalar s) noexcept;

			template<typename Scalar2> void Fill(Scalar2 s) noexcept;

			constexpr SizeType GetSize() const noexcept { return 2; }

			constexpr Iterator Begin() noexcept;
			constexpr ConstIterator Begin() const noexcept;

			constexpr Iterator End() noexcept;
			constexpr ConstIterator End() const noexcept;

			union
			{
				struct { Scalar x, y, z, w; };
				DataType mData;
				ScalarArray<Scalar, 3> xyz;
			};
		};


		template <typename Scalar1, typename Scalar2>
		typename Promote<Scalar1, Scalar2>::RT Dot(const ScalarArray<Scalar1, 4>& a, const ScalarArray<Scalar2, 4>& b) noexcept;

		template <typename Scalar1, typename Scalar2>
		typename Promote<Scalar1, Scalar2>::RT Dot3(const ScalarArray<Scalar1, 4>& a, const ScalarArray<Scalar2, 4>& b) noexcept;

		template <typename Scalar1, typename Scalar2>
		ScalarArray<typename Promote<Scalar1, Scalar2>::RT, 4> Cross(const ScalarArray<Scalar1, 4>& a, const ScalarArray<Scalar2, 4>& b) noexcept;

		template <typename Scalar> ScalarArray<Scalar, 4> Abs(const ScalarArray<Scalar, 4>& a) noexcept;

		template <typename Scalar> Scalar HorizontalAdd3(const ScalarArray<Scalar, 4>& a) noexcept;

		template <typename Scalar1, typename Scalar2>
		void Convert(Scalar1* v, const ScalarArray<Scalar2, 4>& a) noexcept;

		template <typename Scalar> bool Isfinite(const ScalarArray<Scalar, 4>& a) noexcept;

		template <int X, int Y, int Z, int W, typename Scalar>
		ScalarArray<Scalar, 4> Swizzle(const ScalarArray<Scalar, 4>& a) noexcept;

		template <typename Scalar>
		ScalarArray<Scalar, 4> Homogenize(const ScalarArray<Scalar, 4>& a) noexcept;


		//Member Function
		template<typename Scalar>
		FORCEINLINE constexpr ScalarArray<Scalar, 4>::ScalarArray() noexcept
			: x()
			, y()
			, z()
			, w()
		{
		}

		template<typename Scalar>
		FORCEINLINE constexpr ScalarArray<Scalar, 4>::ScalarArray(Zero) noexcept
			: x()
			, y()
			, z()
			, w()
		{
		}

		template<typename Scalar>
		template <std::size_t I>
		FORCEINLINE constexpr ScalarArray<Scalar, 4>::ScalarArray(Unit<I>) noexcept
			: x(Scalar{ I == 0 })
			, y(Scalar{ I == 1 })
			, z(Scalar{ I == 2 })
			, w(Scalar{ I == 3 })
		{
			STATIC_ASSERT(I < 4);
		}

		template<typename Scalar>
		template<typename Scalar2>
		FORCEINLINE constexpr ScalarArray<Scalar, 4>::ScalarArray(Scalar2 x, Scalar2 y, Scalar2 z, Scalar2 w) noexcept
			: x(static_cast<Scalar>(x))
			, y(static_cast<Scalar>(y))
			, z(static_cast<Scalar>(z))
			, w(static_cast<Scalar>(w))
		{
		}

		template<typename Scalar>
		template<typename Scalar2>
		FORCEINLINE constexpr ScalarArray<Scalar, 4>::ScalarArray(const ScalarArray<Scalar2, 3> v, Scalar2 w) noexcept
			: x(static_cast<Scalar>(v.x))
			, y(static_cast<Scalar>(v.y))
			, z(static_cast<Scalar>(v.z))
			, w(static_cast<Scalar>(w))
		{
		}

		template<typename Scalar>
		template<typename Scalar2>
		FORCEINLINE constexpr ScalarArray<Scalar, 4>::ScalarArray(const Scalar2* v) noexcept
			: x(Scalar{ v[0] })
			, y(Scalar{ v[1] })
			, z(Scalar{ v[2] })
			, w(Scalar{ v[3] })
		{
		}

		template<typename Scalar>
		template<typename Scalar2>
		FORCEINLINE constexpr ScalarArray<Scalar, 4>::ScalarArray(const ScalarArray<Scalar2, 4>& v) noexcept
			: x(Scalar{ v.x })
			, y(Scalar{ v.y })
			, z(Scalar{ v.z })
			, w(Scalar{ v.w })
		{
		}

		template<typename Scalar>
		FORCEINLINE ScalarArray<Scalar, 4>::operator ConstPointer() const noexcept
		{
			return mData.data();
		}

		template<typename Scalar>
		FORCEINLINE ScalarArray<Scalar, 4>::operator Pointer() noexcept
		{
			return mData.data();
		}

		template<typename Scalar>
		FORCEINLINE ScalarArray<Scalar, 4>& ScalarArray<Scalar, 4>::operator=(Zero) noexcept
		{
			x = Scalar{};
			y = Scalar{};
			z = Scalar{};
			w = Scalar{};

			return *this;
		}

		template<typename Scalar>
		FORCEINLINE ScalarArray<Scalar, 4>& ScalarArray<Scalar, 4>::operator+=(Zero) noexcept
		{
			return *this;
		}

		template<typename Scalar>
		FORCEINLINE ScalarArray<Scalar, 4>& ScalarArray<Scalar, 4>::operator-=(Zero) noexcept
		{
			return *this;
		}

		template<typename Scalar>
		FORCEINLINE ScalarArray<Scalar, 4>& ScalarArray<Scalar, 4>::operator*=(Zero) noexcept
		{
			x = Scalar{};
			y = Scalar{};
			z = Scalar{};
			w = Scalar{};

			return *this;
		}

		template<typename Scalar>
		template<std::size_t I>
		FORCEINLINE ScalarArray<Scalar, 4>& ScalarArray<Scalar, 4>::operator=(Unit<I>) noexcept
		{
			x = Scalar{ I == 0 };
			y = Scalar{ I == 1 };
			z = Scalar{ I == 2 };
			w = Scalar{ I == 3 };

			return *this;
		}

		template<typename Scalar>
		template<typename Scalar2>
		FORCEINLINE ScalarArray<Scalar, 4>& ScalarArray<Scalar, 4>::operator=(const Scalar2* v) noexcept
		{
			ASSERT(v != nullptr);

			x = v[0];
			y = v[1];
			z = v[2];
			w = v[3];

			return *this;
		}

		template<typename Scalar>
		template<typename Scalar2>
		FORCEINLINE ScalarArray<Scalar, 4>& ScalarArray<Scalar, 4>::operator=(const ScalarArray<Scalar2, 4>& v) noexcept
		{
			ASSERT(v != nullptr);

			x = v.x;
			y = v.y;
			z = v.z;
			w = v.w;

			return *this;
		}

		template<typename Scalar>
		template<typename Scalar2>
		FORCEINLINE ScalarArray<Scalar, 4>& ScalarArray<Scalar, 4>::operator+=(const ScalarArray<Scalar2, 4>& v) noexcept
		{
			ASSERT(v != nullptr);

			x += v.x;
			y += v.y;
			z += v.z;
			w += v.w;

			return *this;
		}

		template<typename Scalar>
		template<typename Scalar2>
		FORCEINLINE ScalarArray<Scalar, 4>& ScalarArray<Scalar, 4>::operator-=(const ScalarArray<Scalar2, 4>& v) noexcept
		{
			ASSERT(v != nullptr);

			x -= v.x;
			y -= v.y;
			z -= v.z;
			w -= v.w;

			return *this;
		}

		template<typename Scalar>
		template<typename Scalar2>
		FORCEINLINE ScalarArray<Scalar, 4>& ScalarArray<Scalar, 4>::operator*=(const ScalarArray<Scalar2, 4>& v) noexcept
		{
			ASSERT(v != nullptr);

			x *= v.x;
			y *= v.y;
			z *= v.z;
			w *= v.w;

			return *this;
		}


		template<typename Scalar>
		FORCEINLINE ScalarArray<Scalar, 4>& ScalarArray<Scalar, 4>::operator*=(Scalar s) noexcept
		{
			x *= s;
			y *= s;
			z *= s;
			w *= s;

			return *this;
		}

		template<typename Scalar>
		FORCEINLINE ScalarArray<Scalar, 4>& ScalarArray<Scalar, 4>::operator/=(Scalar s) noexcept
		{
			ASSERT(!IsZero(s));

			return *this *= (Scalar{ 1 } / s);
		}

		template<typename Scalar>
		template<typename Scalar2>
		FORCEINLINE void ScalarArray<Scalar, 4>::Fill(Scalar2 s) noexcept
		{
			mData.fill(static_cast<Scalar>(s));
		}

		template<typename Scalar>
		FORCEINLINE constexpr typename ScalarArray<Scalar, 4>::Iterator ScalarArray<Scalar, 4>::Begin() noexcept
		{
			using Iterator = typename ScalarArray<Scalar, 4>::Iterator;
			return mData.begin();
		}

		template<typename Scalar>
		FORCEINLINE constexpr typename ScalarArray<Scalar, 4>::ConstIterator ScalarArray<Scalar, 4>::Begin() const noexcept
		{
			using ConstIterator = typename ScalarArray<Scalar, 4>::ConstIterator;
			return mData.begin();
		}

		template<typename Scalar>
		FORCEINLINE constexpr typename ScalarArray<Scalar, 4>::Iterator ScalarArray<Scalar, 4>::End() noexcept
		{
			using Iterator = typename ScalarArray<Scalar, 4>::Iterator;
			return mData.end();
		}

		template<typename Scalar>
		FORCEINLINE constexpr typename ScalarArray<Scalar, 4>::ConstIterator ScalarArray<Scalar, 4>::End() const noexcept
		{
			using ConstIterator = typename ScalarArray<Scalar, 4>::ConstIterator;
			return mData.end();
		}


		//Nomenber Function

		template<typename Scalar1, typename Scalar2>
		FORCEINLINE typename Promote<Scalar1, Scalar2>::RT Dot(const ScalarArray<Scalar1, 4>& a, const ScalarArray<Scalar2, 4>& b) noexcept
		{
			using RT = typename Promote<Scalar1, Scalar2>::RT;

			return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
		}

		template<typename Scalar1, typename Scalar2>
		typename Promote<Scalar1, Scalar2>::RT Dot3(const ScalarArray<Scalar1, 4>& a, const ScalarArray<Scalar2, 4>& b) noexcept
		{
			using RT = typename Promote<Scalar1, Scalar2>::RT;
			return a.x * b.x + a.y * b.y + a.z * b.z;
		}

		template<typename Scalar1, typename Scalar2>
		FORCEINLINE ScalarArray<typename Promote<Scalar1, Scalar2>::RT, 4> Cross(const ScalarArray<Scalar1, 4>& a, const ScalarArray<Scalar2, 4>& b) noexcept
		{
			using RT = typename Promote<Scalar1, Scalar2>::RT;

			return ScalarArray<RT, 4>{ a.y * b.z - b.y * a.z, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x };
		}

		template<typename Scalar>
		FORCEINLINE ScalarArray<Scalar, 4> Abs(const ScalarArray<Scalar, 4>& a) noexcept
		{
			return ScalarArray<Scalar, 4>{ Abs(a.x), Abs(a.y), Abs(a.z), Abs(a.w)};
		}

		template<typename Scalar>
		FORCEINLINE Scalar HorizontalAdd3(const ScalarArray<Scalar, 4>& a) noexcept
		{
			return a.x + a.y + a.z;
		}

		template<typename Scalar1, typename Scalar2>
		FORCEINLINE void Convert(Scalar1* v, const ScalarArray<Scalar2, 4>& a) noexcept
		{
			ASSERT(v != nullptr);

			v[0] = Scalar1(a.x);
			v[1] = Scalar1(a.y);
			v[2] = Scalar1(a.z);
			v[3] = Scalar1(a.w);
		}

		template<typename Scalar>
		FORCEINLINE bool Isfinite(const ScalarArray<Scalar, 4>& a) noexcept
		{
			return IsFinite(a.x) && IsFinite(a.y) && IsFinite(a.z) && IsFinite(a.w);
		}

		template<int X, int Y, int Z, int W, typename Scalar>
		FORCEINLINE ScalarArray<Scalar, 4> Swizzle(const ScalarArray<Scalar, 4>& a) noexcept
		{
			return ScalarArray<Scalar, 4>{a[X], a[Y], a[Z], a[W]};
		}

		template<typename Scalar>
		FORCEINLINE ScalarArray<Scalar, 4> Homogenize(const ScalarArray<Scalar, 4>& a) noexcept
		{
			ASSERT(!IsZero(a.w));
			Scalar s = Scalar{ 1 } / a.w;
			return ScalarArray<Scalar, 4>{ a.x* s, a.y* s, a.z* s, Scalar{ 1 }};
		}


	}
}