#pragma once

#include "ScalarArray.h"

namespace Dash
{
	namespace Math
	{
		using Scalar = float;

		using Float2 = ScalarArray<Scalar, 2>;
		using Float3 = ScalarArray<Scalar, 3>;
		using Float4 = ScalarArray<Scalar, 4>;
	}
}