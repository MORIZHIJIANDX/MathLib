#pragma once

#include <smmintrin.h>

namespace Dash
{
	namespace Math
	{
		static __m128 _Div(__m128 a, __m128 b)
		{
#ifdef FAST_APPROX
			return _mm_mul_ps(a, _mm_rcp_ps(b));
#else
			return _mm_div_ps(a, b);
#endif // FAST_APPROX
		}

		static __m128 _Div(__m128 a, float s)
		{
			ASSERT(s != 0.0f);
#ifdef FAST_APPROX
			return _mm_mul_ps(a, _mm_set1_ps(1.0f / s));
#else
			return _mm_div_ps(a, _mm_set1_ps(s));
#endif // FAST_APPROX
		}
	}


#define PERMUTE4(V, X,Y,Z,W) (_mm_shuffle_ps(V, V, _MM_SHUFFLE(W,Z,Y,X)))
#define PERMUTE3(V, X,Y,Z) (_mm_shuffle_ps(V, V, _MM_SHUFFLE(3,Z,Y,X)))
#define PERMUTE2(V, X,Y) (_mm_shuffle_ps(V, V, _MM_SHUFFLE(3,2,Y,X)))
#define SHUFFLE4(vec1, vec2, x,y,z,w)    _mm_shuffle_ps(vec1, vec2, _MM_SHUFFLE(w,z,y,x))

	template<>
	class ScalarArray<float, 4>
	{
	public:
		using DataType = std::array<float, 4>;

		using ScalarType = float;
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

		ScalarArray() noexcept;
		explicit ScalarArray(Zero) noexcept;
		constexpr explicit ScalarArray(__m128 v) noexcept;
		template <std::size_t I> constexpr explicit ScalarArray(Unit<I>) noexcept;
		template <typename Scalar2> constexpr explicit ScalarArray(Scalar2 x, Scalar2 y, Scalar2 z, Scalar2 w = 0.0f) noexcept;
		template <typename Scalar2> constexpr explicit ScalarArray(const ScalarArray<Scalar2, 3> v, Scalar2 w = 0.0f) noexcept;
		template <typename Scalar2> constexpr explicit ScalarArray(const Scalar2* v) noexcept;
		template <typename Scalar2> constexpr ScalarArray(const ScalarArray<Scalar2, 4>& v) noexcept;

		operator ConstPointer () const noexcept;
		operator Pointer () noexcept;

		operator __m128() const noexcept;

		ScalarArray<float, 4>& operator=(Zero) noexcept;
		ScalarArray<float, 4>& operator+=(Zero) noexcept;
		ScalarArray<float, 4>& operator-=(Zero) noexcept;
		ScalarArray<float, 4>& operator*=(Zero) noexcept;

		ScalarArray<float, 4>& operator=(__m128 v) noexcept;

		template<std::size_t I> ScalarArray<float, 4>& operator=(Unit<I>) noexcept;

		template<typename Scalar2> ScalarArray<float, 4>& operator=(const Scalar2* v) noexcept;
		template<typename Scalar2> ScalarArray<float, 4>& operator=(const ScalarArray<Scalar2, 4>& v) noexcept;
		template<typename Scalar2> ScalarArray<float, 4>& operator+=(const ScalarArray<Scalar2, 4>& v) noexcept;
		template<typename Scalar2> ScalarArray<float, 4>& operator-=(const ScalarArray<Scalar2, 4>& v) noexcept;
		template<typename Scalar2> ScalarArray<float, 4>& operator*=(const ScalarArray<Scalar2, 4>& v) noexcept;

		ScalarArray<float, 4>& operator=(const float* v) noexcept;
		ScalarArray<float, 4>& operator=(const ScalarArray<float, 4>& v) noexcept;
		ScalarArray<float, 4>& operator+=(const ScalarArray<float, 4>& v) noexcept;
		ScalarArray<float, 4>& operator-=(const ScalarArray<float, 4>& v) noexcept;
		ScalarArray<float, 4>& operator*=(const ScalarArray<float, 4>& v) noexcept;

		ScalarArray<float, 4>& operator*=(float s) noexcept;
		ScalarArray<float, 4>& operator/=(float s) noexcept;

		template<typename Scalar2> void Fill(Scalar2 s) noexcept;

		constexpr SizeType GetSize() const noexcept { return 2; }

		constexpr Iterator Begin() noexcept;
		constexpr ConstIterator Begin() const noexcept;

		constexpr Iterator End() noexcept;
		constexpr ConstIterator End() const noexcept;

		union
		{
			struct { float x, y, z, w; };
			DataType mData;
			ScalarArray<float, 3> xyz;
			__m128 mVec;
		};
	};






	// Non-member Operators 

	// --Declaration-- //

	ScalarArray<float, 4> operator-(const ScalarArray<float, 4>& a) noexcept;

	ScalarArray<float, 4> operator+(const ScalarArray<float, 4>& v1, const ScalarArray<float, 4>& v2) noexcept;
	ScalarArray<float, 4> operator-(const ScalarArray<float, 4>& v1, const ScalarArray<float, 4>& v2) noexcept;
	ScalarArray<float, 4> operator*(const ScalarArray<float, 4>& v1, const ScalarArray<float, 4>& v2) noexcept;

	ScalarArray<float, 4> operator*(const ScalarArray<float, 4>& v, float s) noexcept;
	ScalarArray<float, 4> operator*(float s, const ScalarArray<float, 4>& v) noexcept;
	ScalarArray<float, 4> operator/(const ScalarArray<float, 4>& v, float s) noexcept;







	// Non-member Function

	// --Declaration-- //

	namespace Math
	{
		float HorizontalAdd(const ScalarArray<float, 4>& v) noexcept;
		float HorizontalAdd3(const ScalarArray<float, 4>& v) noexcept;

		float Dot(const ScalarArray<float, 4>& v1, const ScalarArray<float, 4>& v2) noexcept;
		float Dot3(const ScalarArray<float, 4>& v1, const ScalarArray<float, 4>& v2) noexcept;

		ScalarArray<float, 4> Cross(const ScalarArray<float, 4>& v1, const ScalarArray<float, 4>& v2) noexcept;

		template <int X, int Y, int Z, int W>
		ScalarArray<float, 4> Swizzle(const ScalarArray<float, 4>& a) noexcept;

		ScalarArray<float, 4> Ceil(const ScalarArray<float, 4>& v) noexcept;
		ScalarArray<float, 4> Floor(const ScalarArray<float, 4>& v) noexcept;
		ScalarArray<float, 4> Trunc(const ScalarArray<float, 4>& v) noexcept;
		ScalarArray<float, 4> Round(const ScalarArray<float, 4>& v) noexcept;
		ScalarArray<float, 4> Frac(const ScalarArray<float, 4>& v) noexcept;
	}








	// Member Function

	// --Implementation-- //

	FORCEINLINE ScalarArray<float, 4>::ScalarArray() noexcept
		: mVec(_mm_setzero_ps())
	{
	}

	FORCEINLINE ScalarArray<float, 4>::ScalarArray(Zero) noexcept
		: mVec(_mm_setzero_ps())
	{
	}

	FORCEINLINE constexpr ScalarArray<float, 4>::ScalarArray(__m128 v) noexcept
		: mVec(v)
	{
	}

	template<std::size_t I>
	FORCEINLINE constexpr ScalarArray<float, 4>::ScalarArray(Unit<I>) noexcept
		: mVec(_mm_setr_ps(float(I == 0), float(I == 1), float(I == 2), float(I == 3)))
	{
	}

	template <typename Scalar2>
	FORCEINLINE constexpr ScalarArray<float, 4>::ScalarArray(Scalar2 x, Scalar2 y, Scalar2 z, Scalar2 w) noexcept
		: mVec(_mm_setr_ps(float(x), float(y), float(z), float(w)))
	{
	}

	template <typename Scalar2>
	FORCEINLINE constexpr ScalarArray<float, 4>::ScalarArray(const ScalarArray<Scalar2, 3> v, Scalar2 w) noexcept
		: mVec(_mm_setr_ps(float(v.x), float(v.y), float(v.z), float(w)))
	{
	}

	template <typename Scalar2>
	FORCEINLINE constexpr ScalarArray<float, 4>::ScalarArray(const Scalar2* v) noexcept
		: mVec(_mm_setr_ps(float(v[0]), float(v[1]), float(v[2]), float(v[3])))
	{
		ASSERT(v != nullptr);
	}

	template <typename Scalar2>
	FORCEINLINE constexpr ScalarArray<float, 4>::ScalarArray(const ScalarArray<Scalar2, 4>& v) noexcept
		: mVec(_mm_setr_ps(float(v.x), float(v.y), float(v.z), float(v.w)))
	{
	}

	FORCEINLINE ScalarArray<float, 4>::operator ConstPointer() const noexcept
	{
		return mData.data();
	}

	FORCEINLINE ScalarArray<float, 4>::operator Pointer() noexcept
	{
		return mData.data();
	}

	FORCEINLINE ScalarArray<float, 4>::operator __m128() const noexcept
	{
		return mVec;
	}

	FORCEINLINE ScalarArray<float, 4>& ScalarArray<float, 4>::operator=(Zero) noexcept
	{
		mVec = _mm_setzero_ps();
		return *this;
	}

	FORCEINLINE ScalarArray<float, 4>& ScalarArray<float, 4>::operator+=(Zero) noexcept
	{
		return *this;
	}

	FORCEINLINE ScalarArray<float, 4>& ScalarArray<float, 4>::operator-=(Zero) noexcept
	{
		return *this;
	}

	FORCEINLINE ScalarArray<float, 4>& ScalarArray<float, 4>::operator*=(Zero) noexcept
	{
		mVec = _mm_setzero_ps();
		return *this;
	}

	FORCEINLINE ScalarArray<float, 4>& ScalarArray<float, 4>::operator=(__m128 v) noexcept
	{
		mVec = v;
		return *this;
	}

	template<std::size_t I>
	FORCEINLINE ScalarArray<float, 4>& ScalarArray<float, 4>::operator=(Unit<I>) noexcept
	{
		mVec = _mm_setr_ps(float(I == 0), float(I == 1), float(I == 2), float(I == 3));
		return *this;
	}

	template<typename Scalar2>
	FORCEINLINE ScalarArray<float, 4>& ScalarArray<float, 4>::operator=(const Scalar2* v) noexcept
	{
		ASSERT(v != nullptr);
		mVec = _mm_setr_ps(float(v[0]), float(v[1]), float(v[2]), float(v[3]));
		return *this;
	}

	template<typename Scalar2>
	FORCEINLINE ScalarArray<float, 4>& ScalarArray<float, 4>::operator=(const ScalarArray<Scalar2, 4>& v) noexcept
	{
		mVec = _mm_setr_ps(float(v.x), float(v.y), float(v.z), float(v.w));
		return *this;
	}

	template<typename Scalar2>
	FORCEINLINE ScalarArray<float, 4>& ScalarArray<float, 4>::operator+=(const ScalarArray<Scalar2, 4>& v) noexcept
	{
		return *this += ScalarArray<float, 4>{v};
	}

	template<typename Scalar2>
	FORCEINLINE ScalarArray<float, 4>& ScalarArray<float, 4>::operator-=(const ScalarArray<Scalar2, 4>& v) noexcept
	{
		return *this -= ScalarArray<float, 4>{v};
	}

	template<typename Scalar2>
	FORCEINLINE ScalarArray<float, 4>& ScalarArray<float, 4>::operator*=(const ScalarArray<Scalar2, 4>& v) noexcept
	{
		return *this *= ScalarArray<float, 4>{v};
	}

	FORCEINLINE ScalarArray<float, 4>& ScalarArray<float, 4>::operator=(const float* v) noexcept
	{
		ASSERT(v != nullptr);
		//mVec = _mm_setr_ps(v[0], v[1], v[2], v[3]);
		mVec = _mm_load_ps(v);
		return *this;
	}

	FORCEINLINE ScalarArray<float, 4>& ScalarArray<float, 4>::operator=(const ScalarArray<float, 4>& v) noexcept
	{
		mVec = v.mVec;
		return *this;
	}

	FORCEINLINE ScalarArray<float, 4>& ScalarArray<float, 4>::operator+=(const ScalarArray<float, 4>& v) noexcept
	{
		mVec = _mm_add_ps(mVec, v.mVec);
		return *this;
	}

	FORCEINLINE ScalarArray<float, 4>& ScalarArray<float, 4>::operator-=(const ScalarArray<float, 4>& v) noexcept
	{
		mVec = _mm_sub_ps(mVec, v.mVec);
		return *this;
	}

	FORCEINLINE ScalarArray<float, 4>& ScalarArray<float, 4>::operator*=(const ScalarArray<float, 4>& v) noexcept
	{
		mVec = _mm_mul_ps(mVec, v.mVec);
		return *this;
	}

	FORCEINLINE ScalarArray<float, 4>& ScalarArray<float, 4>::operator*=(float s) noexcept
	{
		mVec = _mm_mul_ps(mVec, _mm_set1_ps(s));
		return *this;
	}

	FORCEINLINE ScalarArray<float, 4>& ScalarArray<float, 4>::operator/=(float s) noexcept
	{
		mVec = Math::_Div(mVec, s);
		return *this;
	}

	template<typename Scalar2>
	FORCEINLINE void ScalarArray<float, 4>::Fill(Scalar2 s) noexcept
	{
		mVec = _mm_set1_ps(float(s));
	}

	FORCEINLINE constexpr ScalarArray<float, 4>::Iterator ScalarArray<float, 4>::Begin() noexcept
	{
		return mData.begin();
	}

	FORCEINLINE constexpr ScalarArray<float, 4>::ConstIterator ScalarArray<float, 4>::Begin() const noexcept
	{
		return mData.begin();
	}

	FORCEINLINE constexpr ScalarArray<float, 4>::Iterator ScalarArray<float, 4>::End() noexcept
	{
		return mData.end();
	}

	FORCEINLINE constexpr ScalarArray<float, 4>::ConstIterator ScalarArray<float, 4>::End() const noexcept
	{
		return mData.end();
	}







	// Non-member Operators 

	// --Implementation-- //


	FORCEINLINE ScalarArray<float, 4> operator-(const ScalarArray<float, 4>& a) noexcept
	{
		return ScalarArray<float, 4>{ _mm_sub_ps(_mm_setzero_ps(), a) };
	}

	FORCEINLINE ScalarArray<float, 4> operator+(const ScalarArray<float, 4>& v1, const ScalarArray<float, 4>& v2) noexcept
	{
		return ScalarArray<float, 4>{ _mm_add_ps(v1.mVec, v2.mVec) };
	}

	FORCEINLINE ScalarArray<float, 4> operator-(const ScalarArray<float, 4>& v1, const ScalarArray<float, 4>& v2) noexcept
	{
		return ScalarArray<float, 4>{ _mm_sub_ps(v1.mVec, v2.mVec) };
	}

	FORCEINLINE ScalarArray<float, 4> operator*(const ScalarArray<float, 4>& v1, const ScalarArray<float, 4>& v2) noexcept
	{
		return ScalarArray<float, 4>{ _mm_mul_ps(v1.mVec, v2.mVec) };
	}

	FORCEINLINE ScalarArray<float, 4> operator*(const ScalarArray<float, 4>& v, float s) noexcept
	{
		return ScalarArray<float, 4>{_mm_mul_ps(v.mVec, _mm_set1_ps(s))};
	}

	FORCEINLINE ScalarArray<float, 4> operator*(float s, const ScalarArray<float, 4>& v) noexcept
	{
		return v * s;
	}

	FORCEINLINE ScalarArray<float, 4> operator/(const ScalarArray<float, 4>& v, float s) noexcept
	{
		return ScalarArray<float, 4>{ Math::_Div(v.mVec, s) };
	}







	// Non-member Function

	// --Implementation-- //

	namespace Math
	{
		FORCEINLINE float HorizontalAdd(const ScalarArray<float, 4>& v) noexcept
		{
			__m128 temp = PERMUTE4(v, 1, 0, 2, 3);
			__m128 temp1 = _mm_add_ps(temp, v);
			temp1 = _mm_add_ps(PERMUTE4(temp, 2, 0, 1, 3), temp1);
			return _mm_cvtss_f32(_mm_add_ps(PERMUTE4(temp, 3, 0, 1, 2), temp1));
		}

		FORCEINLINE float HorizontalAdd3(const ScalarArray<float, 4>& v) noexcept
		{
			__m128 temp = PERMUTE3(v, 1, 0, 2);
			__m128 temp1 = _mm_add_ps(temp, v.mVec);
			return _mm_cvtss_f32(_mm_add_ps(PERMUTE3(temp, 2, 0, 1), temp1));
		}

		FORCEINLINE float Dot(const ScalarArray<float, 4>& v1, const ScalarArray<float, 4>& v2) noexcept
		{
			__m128 dot = _mm_mul_ps(v1, v2);
			__m128 temp = PERMUTE4(dot, 1, 0, 2, 3);
			__m128 temp1 = _mm_add_ps(temp, dot);
			temp1 = _mm_add_ps(PERMUTE4(temp, 2, 0, 1, 3), temp1);
			return _mm_cvtss_f32(_mm_add_ps(PERMUTE4(temp, 3, 0, 1, 2), temp1));
		}

		FORCEINLINE float Dot3(const ScalarArray<float, 4>& v1, const ScalarArray<float, 4>& v2) noexcept
		{
			__m128 dot = _mm_mul_ps(v1, v2);
			__m128 temp = PERMUTE3(dot, 1, 0, 2);
			__m128 temp1 = _mm_add_ps(temp, dot);
			return _mm_cvtss_f32(_mm_add_ps(PERMUTE3(temp, 2, 0, 1), temp1));
		}

		FORCEINLINE ScalarArray<float, 4> Cross(const ScalarArray<float, 4>& v1, const ScalarArray<float, 4>& v2) noexcept
		{
			__m128 result = _mm_sub_ps(_mm_mul_ps(v1, PERMUTE3(v2, 1, 2, 0)), _mm_mul_ps(PERMUTE3(v1, 1, 2, 0), v2));
			return ScalarArray<float, 4>{PERMUTE3(result, 1, 2, 0)};
		}

		template <int X, int Y, int Z, int W>
		FORCEINLINE ScalarArray<float, 4> Swizzle(const ScalarArray<float, 4>& a) noexcept
		{
			return ScalarArray<float, 4>{_mm_shuffle_ps(a, a, _MM_SHUFFLE(W, Z, Y, X))};
		}

		FORCEINLINE ScalarArray<float, 4> homogenize(const ScalarArray<float, 4>& a)
		{
			ASSERT(!IsZero(a.w));
			return ScalarArray<float, 4>{_Div(a, PERMUTE4(a, 3, 3, 3, 3))};
		}

		FORCEINLINE ScalarArray<float, 4> Ceil(const ScalarArray<float, 4>& v) noexcept
		{
			return ScalarArray<float, 4>{_mm_ceil_ps(v)};
		}

		FORCEINLINE ScalarArray<float, 4> Floor(const ScalarArray<float, 4>& v) noexcept
		{
			return ScalarArray<float, 4>{_mm_floor_ps(v)};
		}

		FORCEINLINE ScalarArray<float, 4> Trunc(const ScalarArray<float, 4>& v) noexcept
		{
			return ScalarArray<float, 4>{_mm_round_ps(v, _MM_FROUND_TRUNC)};
		}

		FORCEINLINE ScalarArray<float, 4> Round(const ScalarArray<float, 4>& v) noexcept
		{
			return ScalarArray<float, 4>{_mm_round_ps(v, _MM_FROUND_NINT)};
		}

		FORCEINLINE ScalarArray<float, 4> Frac(const ScalarArray<float, 4>& v) noexcept
		{
			return ScalarArray<float, 4>{_mm_sub_ps(v, Floor(v))};
		}
	}
}