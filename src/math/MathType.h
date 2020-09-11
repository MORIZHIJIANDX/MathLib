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
	using TVector2 = TScalarArray<T, 2>;

	template<typename T>
	using TVector3 = TScalarArray<T, 3>;

	template<typename T>
	using TVector4 = TScalarArray<T, 4>;


	using FVector2f = TScalarArray<Scalar, 2>;
	using FVector3f = TScalarArray<Scalar, 3>;
	using FVector4f = TScalarArray<Scalar, 4>;

	using FVector2i = TScalarArray<int, 2>;
	using FVector3i = TScalarArray<int, 3>;
	using FVector4i = TScalarArray<int, 4>;

	using FMatrix2x2 = TScalarMatrix<Scalar, 2, 2>;
	using FMatrix3x3 = TScalarMatrix<Scalar, 3, 3>;
	using FMatrix4x4 = TScalarMatrix<Scalar, 4, 4>;

	using FRectangle = TAABB<Scalar, 2>;
	using FBoundingBox = TAABB<Scalar, 3>;

	using FRay = TScalarRay<Scalar>;

	using FQuaternion = TScalarQuaternion<Scalar>;



	template<typename T>
	FORCEINLINE EDASH_FORMAT GetFormat()
	{
		ASSERT_FAIL("Invalid Format Type!");
	}

	template<>
	FORCEINLINE EDASH_FORMAT GetFormat<float>()
	{
		return EDASH_FORMAT::R32_FLOAT;
	}

	template<>
	FORCEINLINE EDASH_FORMAT GetFormat<unsigned short>()
	{
		return EDASH_FORMAT::R16_UINT;
	}

	template<>
	FORCEINLINE EDASH_FORMAT GetFormat<unsigned int>()
	{
		return EDASH_FORMAT::R32_UINT;
	}

	template<>
	FORCEINLINE EDASH_FORMAT GetFormat<FVector2f>()
	{
		return EDASH_FORMAT::R32G32_FLOAT;
	}

	template<>
	FORCEINLINE EDASH_FORMAT GetFormat<FVector3f>()
	{
		return EDASH_FORMAT::R32G32B32_FLOAT;
	}

	template<>
	FORCEINLINE EDASH_FORMAT GetFormat<FVector4f>()
	{
		return EDASH_FORMAT::R32G32B32A32_FLOAT;
	}
}