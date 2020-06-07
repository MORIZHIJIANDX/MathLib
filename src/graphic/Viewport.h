#pragma once

#include "../math/MathType.h"

namespace Dash
{
	struct Viewport
	{
		explicit Viewport(Math::Scalar x = 0, Math::Scalar y = 0, Math::Scalar width = 0, Math::Scalar height = 0, 
			Math::Scalar minDepth = 0, Math::Scalar maxDepth = 1)
			: TopLeftX(x)
			, TopLeftY(y)
			, Width(width)
			, Height(height)
			, MinDepth(minDepth)
			, MaxDepth(maxDepth)
		{}

		Math::Scalar TopLeftX;
		Math::Scalar TopLeftY;
		Math::Scalar Width;
		Math::Scalar Height;
		Math::Scalar MinDepth;
		Math::Scalar MaxDepth;
	};
}