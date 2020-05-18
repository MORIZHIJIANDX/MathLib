#pragma once

namespace Dash
{
	namespace Math
	{
		template<typename Scalar>
		class ScalarArray<Scalar, 3>
		{
		public:
			using DataType = std::array<Scalar, 3>;

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

			constexpr ScalarArray();
			constexpr explicit ScalarArray(Zero);
			template <std::size_t I> constexpr explicit ScalarArray(Unit<I>);
			template <typename Scalar2> constexpr explicit ScalarArray(Scalar2 x, Scalar2 y, Scalar2 z);
			template <typename Scalar2> constexpr explicit ScalarArray(const Scalar2* v);
			template <typename Scalar2> constexpr ScalarArray(const ScalarArray<Scalar2, 3>& v);

			operator ConstPointer () const noexcept;
			operator Pointer () noexcept;

			ScalarArray<Scalar, 3>& operator=(Zero) noexcept;
			ScalarArray<Scalar, 3>& operator+=(Zero) noexcept;
			ScalarArray<Scalar, 3>& operator-=(Zero) noexcept;
			ScalarArray<Scalar, 3>& operator*=(Zero) noexcept;

			template<std::size_t I> ScalarArray<Scalar, 3>& operator=(Unit<I>) noexcept;
			template<typename Scalar2> ScalarArray<Scalar, 3>& operator=(const Scalar2* v) noexcept;

			template<typename Scalar2> ScalarArray<Scalar, 3>& operator=(const ScalarArray<Scalar2, 3>& v) noexcept;
			template<typename Scalar2> ScalarArray<Scalar, 3>& operator+=(const ScalarArray<Scalar2, 3>& v) noexcept;
			template<typename Scalar2> ScalarArray<Scalar, 3>& operator-=(const ScalarArray<Scalar2, 3>& v) noexcept;
			template<typename Scalar2> ScalarArray<Scalar, 3>& operator*=(const ScalarArray<Scalar2, 3>& v) noexcept;

			ScalarArray<Scalar, 3>& operator*=(Scalar s) noexcept;
			ScalarArray<Scalar, 3>& operator/=(Scalar s) noexcept;

			template<typename Scalar2> void Fill(Scalar2 s) noexcept;

			constexpr SizeType GetSize() const noexcept { return 2; }

			constexpr Iterator Begin() noexcept;
			constexpr ConstIterator Begin() const noexcept;

			constexpr Iterator End() noexcept;
			constexpr ConstIterator End() const noexcept;

			union
			{
				struct { Scalar x, y, z; };
				DataType mData;
				ScalarArray<Scalar, 2> xy;
			};
		};


		template <typename Scalar1, typename Scalar2>
		typename Promote<Scalar1, Scalar2>::RT Dot(const ScalarArray<Scalar1, 3>& a, const ScalarArray<Scalar2, 3>& b);

		template <typename Scalar1, typename Scalar2>
		ScalarArray<typename Promote<Scalar1, Scalar2>::RT, 3> Cross(const ScalarArray<Scalar1, 3>& a, const ScalarArray<Scalar2, 3>& b);

		template <typename Scalar> ScalarArray<Scalar, 3> Abs(const ScalarArray<Scalar, 3>& a);

		template <typename Scalar>
		Scalar Triple(const ScalarArray<Scalar, 3>& a, const ScalarArray<Scalar, 3>& b, const ScalarArray<Scalar, 3>& c);

		template <typename Scalar>
		ScalarArray<Scalar, 3> Normal(const ScalarArray<Scalar, 3>& a, const ScalarArray<Scalar, 3>& b, const ScalarArray<Scalar, 3>& c);

		template <typename Scalar> std::size_t MaxAxis(const ScalarArray<Scalar, 3>& a);
		template <typename Scalar> std::size_t MinAxis(const ScalarArray<Scalar, 3>& a);

		template <typename Scalar> std::size_t ClosestAxis(const ScalarArray<Scalar, 3>& a);
		template <typename Scalar> std::size_t FurthestAxis(const ScalarArray<Scalar, 3>& a);

		template <typename Scalar1, typename Scalar2>
		bool Dominates(const ScalarArray<Scalar1, 3>& a, const ScalarArray<Scalar2, 3>& b);

		template <typename Scalar1, typename Scalar2>
		void Convert(Scalar1* v, const ScalarArray<Scalar2, 3>& a);

		template <typename Scalar> bool Isfinite(const ScalarArray<Scalar, 3>& a);


		//Member Function
		template<typename Scalar>
		FORCEINLINE constexpr ScalarArray<Scalar, 3>::ScalarArray()
			: x()
			, y()
			, z()
		{
		}

		template<typename Scalar>
		FORCEINLINE constexpr ScalarArray<Scalar, 3>::ScalarArray(Zero)
			: x(0)
			, y(0)
			, z(0)
		{
		}

		template<typename Scalar>
		template <std::size_t I>
		FORCEINLINE constexpr ScalarArray<Scalar, 3>::ScalarArray(Unit<I>)
			: x(Scalar{ I == 0 })
			, y(Scalar{ I == 1 })
			, z(Scalar{ I == 2 })
		{
			STATIC_ASSERT(I < 3);
		}

		template<typename Scalar>
		template<typename Scalar2>
		FORCEINLINE constexpr ScalarArray<Scalar, 3>::ScalarArray(Scalar2 x, Scalar2 y, Scalar2 z)
			: x(static_cast<Scalar>(x))
			, y(static_cast<Scalar>(y))
			, z(static_cast<Scalar>(z))
		{
		}

		template<typename Scalar>
		template<typename Scalar2>
		FORCEINLINE constexpr ScalarArray<Scalar, 3>::ScalarArray(const Scalar2* v)
			: x(Scalar{ v[0] })
			, y(Scalar{ v[1] })
			, z(Scalar{ v[2] })
		{
		}

		template<typename Scalar>
		template<typename Scalar2>
		FORCEINLINE constexpr ScalarArray<Scalar, 3>::ScalarArray(const ScalarArray<Scalar2, 3>& v)
			: x(Scalar{ v.x })
			, y(Scalar{ v.y })
			, z(Scalar{ v.z })
		{
		}

		template<typename Scalar>
		FORCEINLINE ScalarArray<Scalar, 3>::operator ConstPointer() const noexcept
		{
			return mData.data();
		}

		template<typename Scalar>
		FORCEINLINE ScalarArray<Scalar, 3>::operator Pointer() noexcept
		{
			return mData.data();
		}

		template<typename Scalar>
		FORCEINLINE ScalarArray<Scalar, 3>& ScalarArray<Scalar, 3>::operator=(Zero) noexcept
		{
			x = Scalar{};
			y = Scalar{};
			z = Scalar{};

			return *this;
		}

		template<typename Scalar>
		FORCEINLINE ScalarArray<Scalar, 3>& ScalarArray<Scalar, 3>::operator+=(Zero) noexcept
		{
			return *this;
		}

		template<typename Scalar>
		FORCEINLINE ScalarArray<Scalar, 3>& ScalarArray<Scalar, 3>::operator-=(Zero) noexcept
		{
			return *this;
		}

		template<typename Scalar>
		FORCEINLINE ScalarArray<Scalar, 3>& ScalarArray<Scalar, 3>::operator*=(Zero) noexcept
		{
			x = Scalar{};
			y = Scalar{};
			z = Scalar{};

			return *this;
		}

		template<typename Scalar>
		template<std::size_t I>
		FORCEINLINE ScalarArray<Scalar, 3>& ScalarArray<Scalar, 3>::operator=(Unit<I>) noexcept
		{
			x = Scalar{ I == 0 };
			y = Scalar{ I == 1 };
			z = Scalar{ I == 2 };

			return *this;
		}

		template<typename Scalar>
		template<typename Scalar2>
		FORCEINLINE ScalarArray<Scalar, 3>& ScalarArray<Scalar, 3>::operator=(const Scalar2* v) noexcept
		{
			ASSERT(v != nullptr);

			x = v[0];
			y = v[1];
			z = v[2];

			return *this;
		}

		template<typename Scalar>
		template<typename Scalar2>
		FORCEINLINE ScalarArray<Scalar, 3>& ScalarArray<Scalar, 3>::operator=(const ScalarArray<Scalar2, 3>& v) noexcept
		{
			ASSERT(v != nullptr);

			x = v.x;
			y = v.y;
			z = v.z;

			return *this;
		}

		template<typename Scalar>
		template<typename Scalar2>
		FORCEINLINE ScalarArray<Scalar, 3>& ScalarArray<Scalar, 3>::operator+=(const ScalarArray<Scalar2, 3>& v) noexcept
		{
			ASSERT(v != nullptr);

			x += v.x;
			y += v.y;
			z += v.z;

			return *this;
		}

		template<typename Scalar>
		template<typename Scalar2>
		FORCEINLINE ScalarArray<Scalar, 3>& ScalarArray<Scalar, 3>::operator-=(const ScalarArray<Scalar2, 3>& v) noexcept
		{
			ASSERT(v != nullptr);

			x -= v.x;
			y -= v.y;
			z -= v.z;

			return *this;
		}

		template<typename Scalar>
		template<typename Scalar2>
		FORCEINLINE ScalarArray<Scalar, 3>& ScalarArray<Scalar, 3>::operator*=(const ScalarArray<Scalar2, 3>& v) noexcept
		{
			ASSERT(v != nullptr);

			x *= v.x;
			y *= v.y;
			z *= v.z;

			return *this;
		}


		template<typename Scalar>
		FORCEINLINE ScalarArray<Scalar, 3>& ScalarArray<Scalar, 3>::operator*=(Scalar s) noexcept
		{
			x *= s;
			y *= s;
			z *= s;

			return *this;
		}

		template<typename Scalar>
		FORCEINLINE ScalarArray<Scalar, 3>& ScalarArray<Scalar, 3>::operator/=(Scalar s) noexcept
		{
			ASSERT(!IsZero(s));

			return *this *= (Scalar{ 1 } / s);
		}

		template<typename Scalar>
		template<typename Scalar2>
		FORCEINLINE void ScalarArray<Scalar, 3>::Fill(Scalar2 s) noexcept
		{
			mData.fill(static_cast<Scalar>(s));
		}

		template<typename Scalar>
		FORCEINLINE constexpr typename ScalarArray<Scalar, 3>::Iterator ScalarArray<Scalar, 3>::Begin() noexcept
		{
			using Iterator = typename ScalarArray<Scalar, 3>::Iterator;
			return mData.begin();
		}

		template<typename Scalar>
		FORCEINLINE constexpr typename ScalarArray<Scalar, 3>::ConstIterator ScalarArray<Scalar, 3>::Begin() const noexcept
		{
			using ConstIterator = typename ScalarArray<Scalar, 3>::ConstIterator;
			return mData.begin();
		}

		template<typename Scalar>
		FORCEINLINE constexpr typename ScalarArray<Scalar, 3>::Iterator ScalarArray<Scalar, 3>::End() noexcept
		{
			using Iterator = typename ScalarArray<Scalar, 3>::Iterator;
			return mData.end();
		}

		template<typename Scalar>
		FORCEINLINE constexpr typename ScalarArray<Scalar, 3>::ConstIterator ScalarArray<Scalar, 3>::End() const noexcept
		{
			using ConstIterator = typename ScalarArray<Scalar, 3>::ConstIterator;
			return mData.end();
		}


		//Nomenber Function

		template<typename Scalar1, typename Scalar2>
		FORCEINLINE typename Promote<Scalar1, Scalar2>::RT Dot(const ScalarArray<Scalar1, 3>& a, const ScalarArray<Scalar2, 3>& b)
		{
			using RT = typename Promote<Scalar1, Scalar2>::RT;

			return a.x * b.x + a.y * b.y + a.z * b.z;
		}

		template<typename Scalar1, typename Scalar2>
		FORCEINLINE ScalarArray<typename Promote<Scalar1, Scalar2>::RT, 3> Cross(const ScalarArray<Scalar1, 3>& a, const ScalarArray<Scalar2, 3>& b)
		{
			using RT = typename Promote<Scalar1, Scalar2>::RT;

			typedef typename Promote<Scalar1, Scalar2>::RT RT;
			return ScalarArray<RT, 3>{a.y* b.z - a.z * b.y,
				a.z* b.x - a.x * b.z,
				a.x* b.y - a.y * b.x};
		}

		template<typename Scalar>
		FORCEINLINE ScalarArray<Scalar, 3> Abs(const ScalarArray<Scalar, 3>& a)
		{
			return ScalarArray<Scalar, 3>{ Abs(a.x), Abs(a.y), Abs(a.z) };
		}

		template<typename Scalar>
		Scalar Triple(const ScalarArray<Scalar, 3>& a, const ScalarArray<Scalar, 3>& b, const ScalarArray<Scalar, 3>& c)
		{
#if SPEED_OVER_ACCURACY
			return Dot(a, Cross(b, c));
#else
			ScalarArray<Scalar, 3> e[3];

			e[0] = a;
			e[1] = b;
			e[2] = c;

			ScalarArray<Scalar, 3> d{ Dot(e[0], e[0]),
				Dot(e[1], e[1]),
				Dot(e[2], e[2]) };

			int axis = MaxAxis(d);

			return Dot(e[axis], Cross(e[(axis + 1) % 3], e[(axis + 2) % 3]));
#endif
		}

		template<typename Scalar>
		ScalarArray<Scalar, 3> Normal(const ScalarArray<Scalar, 3>& a, const ScalarArray<Scalar, 3>& b, const ScalarArray<Scalar, 3>& c)
		{
#if SPEED_OVER_ACCURACY
			return Cross(b - a, c - b);
#else
			ScalarArray<Scalar, 3> e[3];

			e[0] = b - a;
			e[1] = c - b;
			e[2] = a - c;

			ScalarArray<Scalar, 3> d{ Dot(e[0], e[0]),
				Dot(e[1], e[1]),
				Dot(e[2], e[2]) };

			int axis = MaxAxis(d);

			return Cross(e[(axis + 1) % 3], e[(axis + 2) % 3]);
#endif
		}

		template<typename Scalar>
		FORCEINLINE std::size_t MaxAxis(const ScalarArray<Scalar, 3>& a)
		{
			int c0 = IsLessnn(a.x, a.y);
			int c1 = IsLessnn(a.x, a.z);
			int c2 = IsLessnn(a.y, a.z);
			return (c0 & ~c2) | ((c1 & c2) << 1);
		}

		template<typename Scalar>
		FORCEINLINE std::size_t MinAxis(const ScalarArray<Scalar, 3>& a)
		{
			int c0 = IsLessnn(a.y, a.x);
			int c1 = IsLessnn(a.z, a.x);
			int c2 = IsLessnn(a.z, a.y);
			return (c0 & ~c2) | ((c1 & c2) << 1);
		}

		template<typename Scalar>
		FORCEINLINE std::size_t ClosestAxis(const ScalarArray<Scalar, 3>& a)
		{
			return MaxAxis(a * a);
		}

		template<typename Scalar>
		FORCEINLINE std::size_t FurthestAxis(const ScalarArray<Scalar, 3>& a)
		{
			return MinAxis(a * a);
		}

		template<typename Scalar1, typename Scalar2>
		FORCEINLINE bool Dominates(const ScalarArray<Scalar1, 3>& a, const ScalarArray<Scalar2, 3>& b)
		{
			return !IsLessnn(a.x, b.x) &&
				!IsLessnn(a.y, b.y) &&
				!IsLessnn(a.z, b.z);
		}

		template<typename Scalar1, typename Scalar2>
		FORCEINLINE void Convert(Scalar1* v, const ScalarArray<Scalar2, 3>& a)
		{
			ASSERT(v != nullptr);

			v[0] = Scalar1(a.x);
			v[1] = Scalar1(a.y);
			v[2] = Scalar1(a.z);
		}

		template<typename Scalar>
		FORCEINLINE bool Isfinite(const ScalarArray<Scalar, 3>& a)
		{
			return IsFinite(a.x) && IsFinite(a.y) && IsFinite(a.z);
		}


	}
}