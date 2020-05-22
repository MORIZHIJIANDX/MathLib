#pragma once

#include "ScalarArray.h"
#include "ScalarMatrix.h"
#include "Quaternion.h"
#include "Metric.h"


namespace Dash
{
	namespace Math
	{
		using Scalar = float;

		using Vector2 = ScalarArray<Scalar, 2>;
		using Vector3 = ScalarArray<Scalar, 3>;
		using Vector4 = ScalarArray<Scalar, 4>;

		using Matrix2x2 = ScalarMatrix<Scalar, 2, 2>;
		using Matrix3x3 = ScalarMatrix<Scalar, 3, 3>;
		using Matrix4x4 = ScalarMatrix<Scalar, 4, 4>;

		using Quaternion = ScalarQuaternion<Scalar>;
	}
}