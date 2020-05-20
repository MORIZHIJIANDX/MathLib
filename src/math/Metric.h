#pragma once

#include "ScalarArray.h"

namespace Dash
{
	namespace Math
	{
		template <typename Vector> typename Vector::ScalarType LengthSquared(const Vector& v);
		template <typename Vector> typename Vector::ScalarType Length(const Vector& v);
		template <typename Vector> typename Vector::ScalarType DistanceSquared(const Vector& p1, const Vector& p2);
		template <typename Vector> typename Vector::ScalarType Distance(const Vector& p1, const Vector& p2);
		template <typename Vector> Vector Normalize(const Vector& v);
		template <typename Vector, typename Scalar> Vector NLerp(const Vector& v1, const Vector& v2, Scalar t);

		template <typename Vector> typename Vector::ScalarType LengthSquared3(const Vector& v);
		template <typename Vector> typename Vector::ScalarType Length3(const Vector& v);
		template <typename Vector> typename Vector::ScalarType DistanceSquared3(const Vector& p1, const Vector& p2);
		template <typename Vector> typename Vector::ScalarType Distance3(const Vector& p1, const Vector& p2);
		template <typename Vector> Vector Normalize3(const Vector& v);
		template <typename Vector, typename Scalar> Vector NLerp3(const Vector& v1, const Vector& v2, Scalar t);
	}
}

