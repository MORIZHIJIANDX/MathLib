#pragma once

#include "ScalarArray.h"
#include "ScalarMatrix.h"
#include "Quaternion.h"
#include "Interval.h"
#include "Bounds.h"
#include "Metric.h"


namespace Dash
{
	namespace Math
	{
		using Scalar = float;

		using Vector2f = ScalarArray<Scalar, 2>;
		using Vector3f = ScalarArray<Scalar, 3>;
		using Vector4f = ScalarArray<Scalar, 4>;

		using Vector2i = ScalarArray<int, 2>;
		using Vector3i = ScalarArray<int, 3>;
		using Vector4i = ScalarArray<int, 4>;

		using Matrix2x2 = ScalarMatrix<Scalar, 2, 2>;
		using Matrix3x3 = ScalarMatrix<Scalar, 3, 3>;
		using Matrix4x4 = ScalarMatrix<Scalar, 4, 4>;

		using Quaternion = ScalarQuaternion<Scalar>;
	}
}