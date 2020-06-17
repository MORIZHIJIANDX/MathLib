#pragma once

#include "ScalarArray.h"
#include "ScalarMatrix.h"
#include "Quaternion.h"
#include "Interval.h"
#include "AABB.h"
#include "Ray.h"
#include "Metric.h"
#include "Enums.h"


namespace Dash
{
	using Scalar = float;

	template<typename T>
	using Vector2 = ScalarArray<T, 2>;

	template<typename T>
	using Vector3 = ScalarArray<T, 3>;

	template<typename T>
	using Vector4 = ScalarArray<T, 4>;


	using Vector2f = ScalarArray<Scalar, 2>;
	using Vector3f = ScalarArray<Scalar, 3>;
	using Vector4f = ScalarArray<Scalar, 4>;

	using Vector2i = ScalarArray<int, 2>;
	using Vector3i = ScalarArray<int, 3>;
	using Vector4i = ScalarArray<int, 4>;

	using Matrix2x2 = ScalarMatrix<Scalar, 2, 2>;
	using Matrix3x3 = ScalarMatrix<Scalar, 3, 3>;
	using Matrix4x4 = ScalarMatrix<Scalar, 4, 4>;

	using Rectangle = AABB<Scalar, 2>;
	using BoundingBox = AABB<Scalar, 3>;

	using Ray = ScalarRay<Scalar>;

	using Quaternion = ScalarQuaternion<Scalar>;
}